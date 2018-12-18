
#ifndef _STDC_INTTYPES_H_
#define _STDC_INTTYPES_H_

#include <limits.h>

typedef signed char schar;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
// WIN32
#ifdef _WIN32
typedef __int64 int64;
typedef unsigned __int64 uint64;
typedef  INT_PTR intptr_t;
#else
// linux
#include <inttypes.h>
#if 0
typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
#endif

typedef int64_t int64;
typedef uint64_t uint64;

typedef uint8_t BYTE;
typedef uint16_t WORD;
typedef unsigned long DWORD;
typedef uint64_t DWORD64;
typedef unsigned int UINT;
typedef long LONG;
typedef unsigned long ULONG;

typedef  intptr_t INT_PTR;
#endif

typedef uint32 COLOR;
typedef uint64 COLOR64;

#endif // _STDC_INTTYPES_H_
