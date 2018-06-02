/*
 * $Id: plot3d.h,v 1.19 2010/11/06 22:02:37 juhaszp Exp $
 */

/* GNUPLOT - plot3d.h */



#ifndef GNUPLOT_PLOT3D_H
# define GNUPLOT_PLOT3D_H

#include "syscfg.h"

/* typedefs of plot3d.c */

typedef enum en_data_mapping {
  MAP3D_CARTESIAN,
  MAP3D_SPHERICAL,
  MAP3D_CYLINDRICAL
} t_data_mapping;

/* Variables of plot3d.c needed by other modules: */

extern struct surface_points* first_3dplot;
extern int plot3d_num;

extern t_data_mapping mapping3d;

extern int dgrid3d_row_fineness;
extern int dgrid3d_col_fineness;
extern int dgrid3d_norm_value;
extern int dgrid3d_mode;
extern double dgrid3d_x_scale;
extern double dgrid3d_y_scale;
extern TBOOLEAN dgrid3d;
extern TBOOLEAN dgrid3d_kdensity;

/* prototypes from plot3d.c */

void plot3drequest __PROTO((void));
void refresh_3dbounds __PROTO((struct surface_points* first_plot, int nplots));
void sp_free __PROTO((struct surface_points* sp));

#endif /* GNUPLOT_PLOT3D_H */
