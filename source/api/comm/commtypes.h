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

#ifndef __COID_COMM_COMMTYPES__HEADER_FILE__
#define __COID_COMM_COMMTYPES__HEADER_FILE__

#include "namespace.h"

#if defined(__MINGW32__)

# define SYSTYPE_WIN        1
# define SYSTYPE_MINGW      1

# ifndef __MSVCRT_VERSION__
#  define __MSVCRT_VERSION__ 0x0800
# elif __MSVCRT_VERSION__<0x0800
#  error comm needs 0x0800 version of msvcrt
# endif

# if !defined(NDEBUG) && !defined(_DEBUG)
#  define _DEBUG
# endif

#elif defined(__CYGWIN__)

# define SYSTYPE_WIN        1
# define SYSTYPE_CYGWIN     1

# if !defined(NDEBUG) && !defined(_DEBUG)
#  define _DEBUG
# endif

#elif defined(_MSC_VER)

# define SYSTYPE_WIN        1
# define SYSTYPE_MSVC       1

# if !defined(_DEBUG) && !defined(NDEBUG)
#  define NDEBUG
# endif

#elif defined(__linux__)

# define SYSTYPE_LINUX     1

# if !defined(NDEBUG) && !defined(_DEBUG)
#  define _DEBUG
# endif

#endif


#ifndef FORCEINLINE
# if defined(_MSC_VER) && (_MSC_VER >= 1200)
#  define FORCEINLINE __forceinline
# elif defined(__GNUC__) && ((__GNUC__ > 3) || \
                            ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 2)))
#  define FORCEINLINE __attribute__((always_inline))
# else
#  define FORCEINLINE inline
# endif
#endif


#ifdef SYSTYPE_WIN
# if defined(_WIN64)
#  define SYSTYPE_64
# else
#  define SYSTYPE_32
# endif
#else
# ifdef __LP64__
#  define SYSTYPE_64
# else
#  define SYSTYPE_32
# endif
#endif

//#define _USE_32BIT_TIME_T
#include <sys/types.h>
#include <stddef.h>
#include "trait.h"

/// Operator new for preallocated storage
inline void * operator new (size_t, const void *p) { return (void*)p; }
inline void operator delete (void *, const void *) {}


namespace coid {

#ifdef SYSTYPE_WIN
typedef short               __int16_t;
typedef long                __int32_t;
typedef __int64             __int64_t;

typedef unsigned short      __uint16_t;
typedef unsigned long       __uint32_t;
typedef unsigned __int64    __uint64_t;

typedef signed char     	__int8_t;
typedef unsigned char     	__uint8_t;
#endif

// standart data types
typedef __int8_t            int8;
typedef __int16_t           int16;
typedef __int32_t           int32;
typedef __int64_t           int64;
typedef __uint8_t           uint8;
typedef __uint16_t          uint16;
typedef __uint32_t          uint32;
typedef __uint64_t      	uint64;
typedef float               flt32;
typedef double              flt64;


#ifndef __USE_MISC      // defined on linux systems in sys/types.h
#define COID_UINT_DEFINED
    typedef unsigned int		uint;
    typedef unsigned long       ulong;
    typedef unsigned short      ushort;
#endif


typedef uint8               uchar;
typedef int8                schar;

///Integer types with the same size as pointer on the platform
typedef size_t              uints;
typedef ptrdiff_t           ints;

} //namespace coid

#ifndef COID_COMMTYPES_IN_NAMESPACE
using coid::int8;
using coid::int16;
using coid::int32;
using coid::int64;
using coid::uint8;
using coid::uint16;
using coid::uint32;
using coid::uint64;
using coid::uchar;
using coid::schar;
using coid::uints;
using coid::ints;
# ifdef COID_UINT_DEFINED
using coid::uint;
using coid::ulong;
using coid::ushort;
# endif
#endif


COID_NAMESPACE_BEGIN

////////////////////////////////////////////////////////////////////////////////
TYPE_TRIVIAL(bool);

TYPE_TRIVIAL(uint8);
TYPE_TRIVIAL(int8);
TYPE_TRIVIAL(int16);
TYPE_TRIVIAL(uint16);
TYPE_TRIVIAL(int32);
TYPE_TRIVIAL(uint32);
TYPE_TRIVIAL(int64);
TYPE_TRIVIAL(uint64);

TYPE_TRIVIAL(char);

#ifdef SYSTYPE_WIN
# ifdef SYSTYPE_32
TYPE_TRIVIAL(ints);
TYPE_TRIVIAL(uints);
# else
TYPE_TRIVIAL(int);
TYPE_TRIVIAL(uint);
# endif
#elif defined(SYSTYPE_32)
TYPE_TRIVIAL(long);
TYPE_TRIVIAL(ulong);
#endif

TYPE_TRIVIAL(float);
TYPE_TRIVIAL(double);
TYPE_TRIVIAL(long double);



////////////////////////////////////////////////////////////////////////////////
template<class INT>
struct SIGNEDNESS
{
    //typedef int     SIGNED;
    //typedef uint    UNSIGNED;
};

#define SIGNEDNESS_MACRO(T,S,U,B) \
template<> struct SIGNEDNESS<T> { typedef S SIGNED; typedef U UNSIGNED; static const int isSigned=B; };


SIGNEDNESS_MACRO(int8,int8,uint8,1);
SIGNEDNESS_MACRO(uint8,int8,uint8,0);
SIGNEDNESS_MACRO(int16,int16,uint16,1);
SIGNEDNESS_MACRO(uint16,int16,uint16,0);
SIGNEDNESS_MACRO(int32,int32,uint32,1);
SIGNEDNESS_MACRO(uint32,int32,uint32,0);
SIGNEDNESS_MACRO(int64,int64,uint64,1);
SIGNEDNESS_MACRO(uint64,int64,uint64,0);

#ifdef SYSTYPE_WIN
# ifdef SYSTYPE_32
SIGNEDNESS_MACRO(ints,ints,uints,1);
SIGNEDNESS_MACRO(uints,ints,uints,0);
# else
SIGNEDNESS_MACRO(int,int,uint,1);
SIGNEDNESS_MACRO(uint,int,int,0);
# endif
#elif defined(SYSTYPE_32)
SIGNEDNESS_MACRO(long,long,ulong,1);
SIGNEDNESS_MACRO(ulong,long,ulong,0);
#endif

/*
#ifdef SYSTYPE_MSVC
    //SIGNEDNESS_MACRO(__int64);
    template<> struct SIGNEDNESS<int32> { typedef int32 SIGNED; typedef uint32 UNSIGNED; };
    template<> struct SIGNEDNESS<uint32> { typedef int32 SIGNED; typedef uint32 UNSIGNED; };
    template<> struct SIGNEDNESS<int64> { typedef int64 SIGNED; typedef uint64 UNSIGNED; };
    template<> struct SIGNEDNESS<uint64> { typedef int64 SIGNED; typedef uint64 UNSIGNED; };
#else
    SIGNEDNESS_MACRO(int);
# if (__WORDSIZE==32)
    template<> struct SIGNEDNESS<__S64_TYPE> { typedef __S64_TYPE SIGNED; typedef __U64_TYPE UNSIGNED; };
    template<> struct SIGNEDNESS<__U64_TYPE> { typedef __S64_TYPE SIGNED; typedef __U64_TYPE UNSIGNED; };
# endif
#endif //_MSC_VER
*/

////////////////////////////////////////////////////////////////////////////////
#ifdef SYSTYPE_MSVC
# define TEMPLFRIEND
#else
# define TEMPLFRIEND <>
#endif


////////////////////////////////////////////////////////////////////////////////
///Structure used as argument to constructors that should not initialize the object
struct NOINIT_t
{};

#define NOINIT      NOINIT_t()

////////////////////////////////////////////////////////////////////////////////
///Shift pointer address in bytes
template <class T>
T* ptr_byteshift( T* p, ints b )
{
    return (T*) ((char*)p + b);
}

template<class T>
inline T* ptr_advance( T* p, ints i ) { return p+i; }

template<>
inline void* ptr_advance( void* p, ints i ) { return (uint8*)p + i; }


////////////////////////////////////////////////////////////////////////////////
void *_xmemcpy( void *dest, const void *src, size_t count );
#if 0//defined _DEBUG
#define xmemcpy     _xmemcpy
#else
#define xmemcpy     ::memcpy
#endif

////////////////////////////////////////////////////////////////////////////////

//used to detect char ptr types
template<typename T, typename R> struct is_char_ptr {};
template<typename R> struct is_char_ptr<const char *, R> { typedef R type; };
template<typename R> struct is_char_ptr<char *, R>       { typedef R type; };


COID_NAMESPACE_END




#ifdef SYSTYPE_MSVC
    #define COMM_ALIGNAS(k) __declspec( align(k) )
#else
    #define COMM_ALIGNAS(k) __attribute__((__aligned__(k)))
#endif


#ifdef SYSTYPE_64 
	#define UMAXS       static_cast<coid::uints>(0xffffffffffffffffULL)
#else
	#define UMAXS       static_cast<coid::uints>(0xffffffffUL)
#endif

#define UMAX32      0xffffffffUL
#define UMAX64      0xffffffffffffffffULL
#define WMAX        0xffff


#include "net_ul.h"


#endif //__COID_COMM_COMMTYPES__HEADER_FILE__
