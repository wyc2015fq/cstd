/*
 * $Id: specfun.h,v 1.13 2013/07/13 05:52:44 sfeam Exp $
 */

/* GNUPLOT - specfun.h */



#ifndef GNUPLOT_SPECFUN_H
# define GNUPLOT_SPECFUN_H

/* #if... / #include / #define collection: */

#include "syscfg.h"
#include "eval.h"

/* Type definitions */

/* Variables of specfun.c needed by other modules: */

/* Prototypes of functions exported by specfun.c */
double chisq_cdf __PROTO((int dof, double chisqr));

/* These are the more 'special' functions built into the stack machine. */
void f_erf __PROTO((union argument* x));
void f_erfc __PROTO((union argument* x));
void f_ibeta __PROTO((union argument* x));
void f_igamma __PROTO((union argument* x));
void f_gamma __PROTO((union argument* x));
void f_lgamma __PROTO((union argument* x));
void f_rand __PROTO((union argument* x));
void f_normal __PROTO((union argument* x));
void f_inverse_normal __PROTO((union argument* x));
void f_inverse_erf __PROTO((union argument* x));
void f_lambertw __PROTO((union argument* x));
void f_airy __PROTO((union argument* x));
void f_expint __PROTO((union argument* x));

#ifndef HAVE_LIBCERF
void f_voigt __PROTO((union argument* x));
#endif

#endif /* GNUPLOT_SPECFUN_H */
