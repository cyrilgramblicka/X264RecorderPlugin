/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is COID/comm module.
 *
 * The Initial Developer of the Original Code is
 * PosAm.
 * Portions created by the Initial Developer are Copyright (C) 2003
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 * Brano Kemen
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#ifndef __COID_COMM_ALLOC__HEADER_FILE__
#define __COID_COMM_ALLOC__HEADER_FILE__

#include "../namespace.h"

#include "sseg.h"
#include "chunkalloc.h"
#include "../sync/mutex.h"
#include "../singleton.h"
#include "../binstream/binstream.h"


COID_NAMESPACE_BEGIN

////////////////////////////////////////////////////////////////////////////////
///Memory allocator. Used to allocate arrays of items, stores the number of items
/// to (uint) directly preceding the address of returned memory block. This is used
/// by the dynarray class and derived stuff.
///The allocator uses two other allocator classes: chunk_allocator<page> for allocating
/// memory pages, and ssegpage allocator for managing blocks within the memory page.
///Another property of the allocator is that blocks allocated by the allocator would be
/// resized and deleted by the same allocator even if the block is manipulated by
/// a different dynamically loaded library.
class seg_allocator
{
public:

    COIDNEWDELETE("seg_allocator");

    static const char* class_name()     { return "seg_allocator"; }

    typedef ssegpage::block      block;

    struct HEADER : block {
        //uints   _size;          //byte size
        uints   _count;         //item count
    };

    HEADER* alloc (uints count, uints chunk);
    HEADER* realloc (HEADER* p, uints count, uints chunk, bool keep);
    HEADER* reserve (HEADER* p, uints count, uints chunk, bool keep);

    HEADER* allocset (uints count, uints chunk, uchar set);
    HEADER* reallocset (HEADER* p, uints count, uints chunk, bool keep, uchar set);
    HEADER* reserveset (HEADER* p, uints count, uints chunk, bool keep, uchar set);

    void  free (HEADER* p);

    //bool is_valid_ptr (const HEADER* p) const;

    uints get_segsize() const        { return _segsize; }


    seg_allocator( bool usemutex=true, uints segsize = 1U<<18 )
    {
        _segsize = nextpow2(segsize);
        SINGLETON( chunk_allocator<page> );
        _nseg = 0;
        _last = 0;
        _last_succ = 0;
        if(usemutex)
        {
            _pgcreatemutex = new comm_mutex(100, false);
            _pgcreatemutex->set_name( "seg_allocator (_pgcreatemutex)" );
        }
    }

    ~seg_allocator();


    uints used_memory( uints* total )
    {
        uints np=0,usm=0;

        page* p = _last;
        for( ; p; p=p->prev )
        {
            ++np;
            usm += p->spage->get_used_size();
        }

        if(total)  *total = np * _segsize;
        return usm;
    }

    uints total_memory() const
    {
        return _nseg * _segsize;
    }

    void discard();


    struct load_data
    {
        void* base;
        int   diff;

        bool operator < ( const load_data& p ) const
        {
            return base < p.base;
        }

        bool operator < ( const void* p ) const
        {
            return base < p;
        }
    };

    opcd save( binstream& bin );
    opcd load( binstream& bin, void* dynarray_ald );

protected:

    struct page
    {
        ssegpage* spage;
        page*    prev;
    };

    local<comm_mutex> _pgcreatemutex;

    uints        _segsize;
    uints        _nseg;

    page*       _last;
    page*       _last_succ;
};

////////////////////////////////////////////////////////////////////////////////
COID_NAMESPACE_END

#endif //#ifndef __COID_COMM_ALLOC__HEADER_FILE__

