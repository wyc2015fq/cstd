
#ifndef __FACERECOGNITION_H_
#define __FACERECOGNITION_H_

#ifdef _OPENMP
#include <omp.h>
#endif // _OPENMP
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#if defined WIN32
#define __PC
#else
#define __DSP
#endif

#ifdef __PC
#if defined WIN32
//#include "loaddll.h" // for real
#else
#define GETFUN(_FUNNAME)  0
#define LOADDLL_BEGIN(_x)
#define LOADDLL_END()
#endif
#include "distance.h"
#include "../img/filters.h"
//#include "../imresize.h"

#else
#define GETFUN(_FUNNAME)  0
#define LOADDLL_BEGIN(_x)
#define LOADDLL_END()
#include "distance.h"
#include "../img/filters.h"

//#define utime_start(_start_count)    int _start_count = CLK_gethtime()
//#define utime_elapsed(_start_count)  (double)((CLK_gethtime() - _start_count)*1./CLK_countspms())
#endif

#endif // __FACERECOGNITION_H_

