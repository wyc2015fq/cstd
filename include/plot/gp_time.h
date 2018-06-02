/*
 * $Id: gp_time.h,v 1.9 2013/04/05 18:36:53 sfeam Exp $
 */

/* GNUPLOT - gp_time.h */



#ifndef GNUPLOT_GP_TIME_H
# define GNUPLOT_GP_TIME_H

/* #if... / #include / #define collection: */

#include "syscfg.h"
#include "stdfn.h"

/* Define the zero point for internal storage of time+date as some number of seconds */
/* Through gnuplot version 4.6 this was taken as 1-jan-2000, i.e. 30 years off from  */
/* the conventional unix epoch date 1-jan-1970. This caused problems when converting */
/* internal <-> external dates given in seconds, so now we change it to agree with   */
/* the rest of the unix world.                   */
#if (0)
#define ZERO_YEAR 2000
#define JAN_FIRST_WDAY 6  /* 1 jan 2000 was a Saturday (cal 1 2000 on unix) */
#define SEC_OFFS_SYS  946684800.0 /*  zero gnuplot (2000) - zero system (1970) */
#else
#define ZERO_YEAR 1970
#define JAN_FIRST_WDAY 4  /* 1 jan 1970 was a Thursday (cal 1 1970 on unix) */
#define SEC_OFFS_SYS  0.0 /* difference between internal and external epochs */
#endif

/* defines used for timeseries, seconds */
#define YEAR_SEC  31557600.0  /* avg, incl. leap year */
#define MON_SEC   2629800.0 /* YEAR_SEC / 12 */
#define WEEK_SEC  604800.0
#define DAY_SEC   86400.0

/* Type definitions */

/* Variables of time.c needed by other modules: */

/* Prototypes of functions exported by time.c */

/* string to *tm */
char* gstrptime __PROTO((char*, char*, struct tm*, double*));

/* seconds to string */
size_t gstrftime __PROTO((char*, size_t, const char*, double));

/* *tm to seconds */
double gtimegm __PROTO((struct tm*));

/* seconds to *tm */
int ggmtime __PROTO((struct tm*, double));



#endif /* GNUPLOT_GP_TIME_H */
