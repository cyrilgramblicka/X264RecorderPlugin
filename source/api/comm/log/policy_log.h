
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
 * Ladislav Hrabcak
 * Portions created by the Initial Developer are Copyright (C) 2007
 * the Initial Developer. All Rights Reserved.
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

#ifndef __COMM_LOG_POLICY_LOG_H__
#define __COMM_LOG_POLICY_LOG_H__
/*
#include "../ref.h"
#include "../atomic/pool.h"

class ref_base;

COID_NAMESPACE_BEGIN

class logmsg;

struct policy_log : public policy_queue_pooled<logmsg>
{
protected:
	explicit policy_log( logmsg* const obj,pool_t* const pl=0 ) 
		: policy_queue_pooled( obj,pl ) {}

public:
	virtual void destroy();

	static this_t* create() { return SINGLETON(pool_t).create(); }

	static this_t* create(pool_t* p) { DASSERT(p!=0); return p->create(); }

	static this_t* internal_create(pool_t* pl);
};

COID_NAMESPACE_END
*/
#endif // __COMM_LOG_POLICY_LOG_H__
