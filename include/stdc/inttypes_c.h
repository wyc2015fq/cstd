
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
#ifdef _WIN32
typedef __int64 int64;
typedef unsigned __int64 uint64;
#else
#include <inttypes.h>
typedef int64_t int64;
typedef uint64_t uint64;
#endif

typedef uint32 COLOR;
typedef uint64 COLOR64;

#endif // _STDC_INTTYPES_H_
