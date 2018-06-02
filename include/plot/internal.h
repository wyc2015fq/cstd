/*
 * $Id: internal.h,v 1.25 2014/03/30 19:05:46 markisch Exp $
 */

/* GNUPLOT - internal.h */



#ifndef GNUPLOT_INTERNAL_H
# define GNUPLOT_INTERNAL_H

/* #if... / #include / #define collection: */

#include "syscfg.h"
#include "gp_types.h"
#include "eval.h"

/* Prototypes from file "internal.c" */
void eval_reset_after_error __PROTO((void));

/* the basic operators of our stack machine for function evaluation: */
void f_push __PROTO((union argument* x));
void f_pushc __PROTO((union argument* x));
void f_pushd1 __PROTO((union argument* x));
void f_pushd2 __PROTO((union argument* x));
void f_pushd __PROTO((union argument* x));
void f_pop __PROTO((union argument* x));
void f_call __PROTO((union argument* x));
void f_calln __PROTO((union argument* x));
void f_sum __PROTO((union argument* x));
void f_lnot __PROTO((union argument* x));
void f_bnot __PROTO((union argument* x));
void f_lor __PROTO((union argument* x));
void f_land __PROTO((union argument* x));
void f_bor __PROTO((union argument* x));
void f_xor __PROTO((union argument* x));
void f_band __PROTO((union argument* x));
void f_uminus __PROTO((union argument* x));
void f_eq __PROTO((union argument* x));
void f_ne __PROTO((union argument* x));
void f_gt __PROTO((union argument* x));
void f_lt __PROTO((union argument* x));
void f_ge __PROTO((union argument* x));
void f_le __PROTO((union argument* x));
void f_leftshift __PROTO((union argument* x));
void f_rightshift __PROTO((union argument* x));
void f_plus __PROTO((union argument* x));
void f_minus __PROTO((union argument* x));
void f_mult __PROTO((union argument* x));
void f_div __PROTO((union argument* x));
void f_mod __PROTO((union argument* x));
void f_power __PROTO((union argument* x));
void f_factorial __PROTO((union argument* x));

void f_concatenate __PROTO((union argument* x));
void f_eqs __PROTO((union argument* x));
void f_nes __PROTO((union argument* x));
void f_gprintf __PROTO((union argument* x));
void f_range __PROTO((union argument* x));
void f_sprintf __PROTO((union argument* x));
void f_strlen __PROTO((union argument* x));
void f_strstrt __PROTO((union argument* x));
void f_system __PROTO((union argument* x));
void f_word __PROTO((union argument* x));
void f_words __PROTO((union argument* x));
void f_strftime __PROTO((union argument* x));
void f_strptime __PROTO((union argument* x));
void f_time __PROTO((union argument* x));
void f_assign __PROTO((union argument* x));
void f_value __PROTO((union argument* x));

#endif /* GNUPLOT_INTERNAL_H */
