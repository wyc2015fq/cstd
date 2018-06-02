/*
 * $Id: interpol.h,v 1.9 2014/01/31 03:43:40 sfeam Exp $
 */

/* GNUPLOT - interpol.h */



#ifndef GNUPLOT_INTERPOL_H
# define GNUPLOT_INTERPOL_H

/* #if... / #include / #define collection: */

#include "syscfg.h"
#include "graphics.h"

/* Type definitions */

/* Variables of interpol.c needed by other modules: */

/* Prototypes of functions exported by interpol.c */
void gen_interp __PROTO((struct curve_points* plot));
void gen_interp_unwrap __PROTO((struct curve_points* plot));
void gen_interp_frequency __PROTO((struct curve_points* plot));
void mcs_interp __PROTO((struct curve_points* plot));
void sort_points __PROTO((struct curve_points* plot));
void cp_implode __PROTO((struct curve_points* cp));

#endif /* GNUPLOT_INTERPOL_H */
