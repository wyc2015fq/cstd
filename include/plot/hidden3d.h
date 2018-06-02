/*
 * $Id: hidden3d.h,v 1.13 2013/06/27 19:37:14 sfeam Exp $
 */

/* GNUPLOT - hidden3d.h */



#ifndef GNUPLOT_HIDDEN3D_H
# define GNUPLOT_HIDDEN3D_H

/* #if... / #include / #define collection: */

#include "syscfg.h"
#include "stdfn.h"
#include "graph3d.h"
#include "util3d.h"

/* Type definitions */

#define PT_ARROWHEAD -10
#define PT_BACKARROW -11

/* Variables of hidden3d.c needed by other modules: */

extern TBOOLEAN disable_mouse_z;

/* Prototypes of functions exported by hidden3d.c */

void set_hidden3doptions __PROTO((void));
void show_hidden3doptions __PROTO((void));
void reset_hidden3doptions __PROTO((void));
void save_hidden3doptions __PROTO((FILE* fp));
void init_hidden_line_removal __PROTO((void));
void reset_hidden_line_removal __PROTO((void));
void term_hidden_line_removal __PROTO((void));
void plot3d_hidden __PROTO((struct surface_points* plots, int pcount));
void draw_line_hidden __PROTO((p_vertex, p_vertex, lp_style_type*));
void draw_label_hidden __PROTO((p_vertex, lp_style_type*, int, int));

#endif /* GNUPLOT_HIDDEN3D_H */
