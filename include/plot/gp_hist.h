/*
 * $Id: gp_hist.h,v 1.13 2013/12/17 00:49:52 sfeam Exp $
 */

/* GNUPLOT - gp_hist.h */



#ifndef GNUPLOT_HISTORY_H
# define GNUPLOT_HISTORY_H

#include "syscfg.h"

/* Type and struct definitions */

struct hist {
  char* line;
  struct hist* prev;
  struct hist* next;
};

#define HISTORY_SIZE 500

/* Variables of history.c needed by other modules: */

extern struct hist* history;
extern struct hist* cur_entry;

extern int gnuplot_history_size;
extern TBOOLEAN history_quiet;
extern TBOOLEAN history_full;

/* Prototypes of functions exported by history.c */

/* GNU readline
 * Only required by two files directly,
 * so I don't put this into a header file. -lh
 */
#ifdef HAVE_LIBREADLINE
# include <readline/history.h>
#endif

/* NetBSD editline
 * (almost) compatible readline replacement
 */
#if defined(HAVE_LIBEDITLINE)
# include <editline/readline.h>
#endif

#if defined(READLINE) && !defined(HAVE_LIBREADLINE) && !defined(HAVE_LIBEDITLINE)
void add_history __PROTO((char* line));
void write_history_n __PROTO((const int, const char*, const char*));
void write_history __PROTO((char*));
void read_history __PROTO((char*));
const char* history_find __PROTO((char*));
const char* history_find_by_number __PROTO((int));
int history_find_all __PROTO((char*));
#elif defined(HAVE_LIBREADLINE) || defined(HAVE_LIBEDITLINE)
void write_history_n __PROTO((const int, const char*, const char*));
const char* history_find __PROTO((char*));
const char* history_find_by_number __PROTO((int));
int history_find_all __PROTO((char*));
#endif /* READLINE && !HAVE_LIBREADLINE && !HAVE_LIBEDITLINE */

#endif /* GNUPLOT_HISTORY_H */
