/*
 * $Id: setshow.h,v 1.50 2014/03/23 12:17:59 markisch Exp $
 */

/* GNUPLOT - setshow.h */




#ifndef GNUPLOT_SETSHOW_H
# define GNUPLOT_SETSHOW_H

#include "stdfn.h"

#include "axis.h"
#include "gadgets.h"
#include "term_api.h"

#define PROGRAM "G N U P L O T"  /* FIXME: move to show.c! */

#define SAVE_NUM_OR_TIME(fp, x, axis)       \
  do{               \
    if (axis_array[axis].datatype == DT_TIMEDATE) {   \
      char s[80];           \
      \
      putc('"', fp);            \
      gstrftime(s,80,axis_array[axis].timefmt,(double)(x)); \
      fputs(conv_text(s), fp);        \
      putc('"', fp);            \
    } else {              \
      fprintf(fp,"%#g",x);          \
    }               \
  } while(0)


/* The set and show commands, in setshow.c */
void set_command __PROTO((void));
void unset_command __PROTO((void));
void reset_command __PROTO((void));
void show_command __PROTO((void));
/* and some accessible support functions */
void show_version __PROTO((FILE* fp));
void set_format __PROTO((void));
void set_colorsequence __PROTO((int option));
char* conv_text __PROTO((const char* s));
void delete_linestyle __PROTO((struct linestyle_def**, struct linestyle_def*, struct linestyle_def*));
void delete_dashtype __PROTO((struct custom_dashtype_def*, struct custom_dashtype_def*));
/* void delete_arrowstyle __PROTO((struct arrowstyle_def *, struct arrowstyle_def *)); */
void reset_key __PROTO((void));
void free_marklist __PROTO((struct ticmark* list));
extern int enable_reset_palette;
void reset_palette __PROTO((void));
void rrange_to_xy __PROTO((void));

/* Called from set_label(), plot2d.c and plot3d.c */
extern void parse_label_options __PROTO((struct text_label* this_label, TBOOLEAN));
extern struct text_label* new_text_label __PROTO((int tag));
extern void disp_value __PROTO((FILE*, struct value*, TBOOLEAN));
extern struct ticmark* prune_dataticks __PROTO((struct ticmark* list));


#endif /* GNUPLOT_SETSHOW_H */
