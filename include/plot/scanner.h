/*
 * $Id: scanner.h,v 1.8 2011/11/28 19:49:53 sfeam Exp $
 */

/* GNUPLOT - scanner.h */



#ifndef GNUPLOT_SCANNER_H
# define GNUPLOT_SCANNER_H

/* #if... / #include / #define collection: */

#include "syscfg.h"
#include "stdfn.h"

/* Type definitions */

/* Variables of scanner.c needed by other modules: */

extern int curly_brace_count;

/* Prototypes of functions exported by scanner.c */

TBOOLEAN legal_identifier __PROTO((char* p));
int scanner __PROTO((char** expression, size_t* line_lengthp));

#endif /* GNUPLOT_SCANNER_H */
