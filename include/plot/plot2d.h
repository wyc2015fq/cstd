/*
 * $Id: plot2d.h,v 1.15 2012/03/21 00:22:25 sfeam Exp $
 */

/* GNUPLOT - plot2d.h */



#ifndef GNUPLOT_PLOT2D_H
# define GNUPLOT_PLOT2D_H

#include "syscfg.h"

/* Variables of plot2d.c needed by other modules: */

extern struct curve_points* first_plot;

extern double boxwidth;
extern TBOOLEAN boxwidth_is_absolute;

/* prototypes from plot2d.c */

void plotrequest __PROTO((void));
void refresh_bounds __PROTO((struct curve_points* first_plot, int nplots));

/* internal and external variables */
void cp_free __PROTO((struct curve_points* cp));
void cp_extend __PROTO((struct curve_points* cp, int num));

#include "gp_types.h"
#include "gadgets.h"
struct text_label* store_label __PROTO((struct text_label*, struct coordinate*,
    int i, char* string, double colorval));

#endif /* GNUPLOT_PLOT2D_H */
