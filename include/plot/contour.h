/*
 * $Id: contour.h,v 1.9 2004/08/09 00:51:29 sfeam Exp $
 */

/* GNUPLOT - contour.h */



#ifndef GNUPLOT_CONTOUR_H
# define GNUPLOT_CONTOUR_H

/* #if... / #include / #define collection: */

#include "syscfg.h"
#include "gp_types.h"

#include "dynarray.h"
#include "graph3d.h"

#define DEFAULT_CONTOUR_LEVELS 5
#define DEFAULT_NUM_APPROX_PTS 5
#define DEFAULT_CONTOUR_ORDER  4
#define MAX_BSPLINE_ORDER      10

/* Type definitions */

typedef enum en_contour_kind {
  /* Method of drawing the contour lines found */
  CONTOUR_KIND_LINEAR,
  CONTOUR_KIND_CUBIC_SPL,
  CONTOUR_KIND_BSPLINE
} t_contour_kind;

typedef enum en_contour_levels_kind {
  /* How contour levels are set */
  LEVELS_AUTO,    /* automatically selected */
  LEVELS_INCREMENTAL,   /* user specified start & incremnet */
  LEVELS_DISCRETE   /* user specified discrete levels */
} t_contour_levels_kind;


/* Used to allocate the tri-diag matrix. */
typedef double tri_diag[3];

/* Variables of contour.c needed by other modules: */

extern char contour_format[32];
extern t_contour_kind contour_kind;
extern t_contour_levels_kind contour_levels_kind;
extern int contour_levels;
extern int contour_order;
extern int contour_pts;

/* storage for z levels to draw contours at */
extern dynarray dyn_contour_levels_list;
#define contour_levels_list ((double *)dyn_contour_levels_list.v)

/* Prototypes of functions exported by contour.c */

struct gnuplot_contours* contour __PROTO((int num_isolines, struct iso_curve* iso_lines));
int solve_tri_diag __PROTO((tri_diag m[], double r[], double x[], int n));


#endif /* GNUPLOT_CONTOUR_H */
