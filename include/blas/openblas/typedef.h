
#undef NUMOPT
#undef ZERO
#undef ONE
#undef FLOAT
#undef XFLOAT
#undef SIZE
#undef BASE_SHIFT
#undef ZBASE_SHIFT
#undef COMPSIZE


#ifdef XDOUBLE
#define NUMOPT	QNUMOPT
#elif defined DOUBLE
#define NUMOPT	DNUMOPT
#else
#define NUMOPT	SNUMOPT
#endif

#ifndef ZERO
#ifdef XDOUBLE
#define ZERO  0.e0L
#elif defined DOUBLE
#define ZERO  0.e0
#else
#define ZERO  0.e0f
#endif
#endif

#ifndef ONE
#ifdef XDOUBLE
#define ONE  1.e0L
#elif defined DOUBLE
#define ONE  1.e0
#else
#define ONE  1.e0f
#endif
#endif


#ifdef XDOUBLE
#define FLOAT	xdouble
#ifdef QUAD_PRECISION
#define XFLOAT	xidouble
#endif
#ifdef QUAD_PRECISION
#define SIZE	32
#define  BASE_SHIFT 5
#define ZBASE_SHIFT 6
#else
#define SIZE	16
#define  BASE_SHIFT 4
#define ZBASE_SHIFT 5
#endif
#elif defined(DOUBLE)
#define FLOAT	double
#define SIZE	8
#define  BASE_SHIFT 3
#define ZBASE_SHIFT 4
#else
#define FLOAT	float
#define SIZE    4
#define  BASE_SHIFT 2
#define ZBASE_SHIFT 3
#endif

#ifndef XFLOAT
#define XFLOAT	FLOAT
#endif

#ifndef COMPLEX
#define COMPSIZE  1
#else
#define COMPSIZE  2
#endif

#include "common_macro.h"
