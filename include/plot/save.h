/*
 * $Id: save.h,v 1.18 2014/03/17 16:26:57 juhaszp Exp $
 */

/* GNUPLOT - save.h */



#ifndef GNUPLOT_SAVE_H
# define GNUPLOT_SAVE_H

/* #if... / #include / #define collection: */

#include "syscfg.h"
#include "stdfn.h"

#include "axis.h"

/* Type definitions */

/* Variables of save.c needed by other modules: */

/* Prototypes of functions exported by save.c */
void save_functions __PROTO((FILE* fp));
void save_variables __PROTO((FILE* fp));
void save_set __PROTO((FILE* fp));
void save_term __PROTO((FILE* fp));
void save_all __PROTO((FILE* fp));
void save_range __PROTO((FILE*, AXIS_INDEX));
void save_textcolor __PROTO((FILE*, const struct t_colorspec*));
void save_pm3dcolor __PROTO((FILE*, const struct t_colorspec*));
void save_fillstyle __PROTO((FILE*, const struct fill_style_type*));
void save_offsets __PROTO((FILE*, char*));
void save_histogram_opts __PROTO((FILE* fp));
#ifdef EAM_OBJECTS
void save_object __PROTO((FILE*, int));
#endif
void save_style_parallel __PROTO((FILE*));
void save_data_func_style __PROTO((FILE*, const char*, enum PLOT_STYLE));
void save_linetype __PROTO((FILE*, lp_style_type*, TBOOLEAN));
void save_dashtype __PROTO((FILE*, int, const t_dashtype*));

#endif /* GNUPLOT_SAVE_H */
