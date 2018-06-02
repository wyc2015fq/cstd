/*
 * $Id: plot.h,v 1.55 2014/01/04 02:55:06 markisch Exp $
 */

/* GNUPLOT - plot.h */



#ifndef GNUPLOT_PLOT_H
# define GNUPLOT_PLOT_H

/* #if... / #include / #define collection: */

#include "syscfg.h"
#include "gp_types.h"

#ifdef USE_MOUSE
# include "mouse.h"
#endif

/* Type definitions */

/* Variables of plot.c needed by other modules: */

extern TBOOLEAN interactive;
extern TBOOLEAN noinputfiles;
extern TBOOLEAN persist_cl;

extern const char* user_shell;

extern TBOOLEAN ctrlc_flag;

#ifdef OS2
extern TBOOLEAN CallFromRexx;
#endif

/* Prototypes of functions exported by plot.c */

#if defined(__GNUC__)
void bail_to_command_line __PROTO((void)) __attribute__((noreturn));
#else
void bail_to_command_line __PROTO((void));
#endif

void init_constants __PROTO((void));
void init_session __PROTO((void));

#if defined(_Windows)
int gnu_main __PROTO((int argc, char** argv));
#endif

void interrupt_setup __PROTO((void));
void gp_expand_tilde __PROTO((char**));
void get_user_env __PROTO((void));

#ifdef LINUXVGA
void drop_privilege __PROTO((void));
void take_privilege __PROTO((void));
#endif /* LINUXVGA */

#ifdef OS2
int ExecuteMacro __PROTO((char*, int));
#endif

void restrict_popen __PROTO((void));

#ifdef GNUPLOT_HISTORY
void cancel_history __PROTO((void));
#else
#define cancel_history()  {}
#endif

#endif /* GNUPLOT_PLOT_H */
