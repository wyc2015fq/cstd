/*
 * $Id: readline.h,v 1.11 2008/12/12 21:06:13 sfeam Exp $
 */

/* GNUPLOT - readline.h */



#ifndef GNUPLOT_READLINE_H
# define GNUPLOT_READLINE_H

/* #if... / #include / #define collection: */

#include "syscfg.h"
/* Type definitions */

/* Variables of readline.c needed by other modules: */

/* Prototypes of functions exported by readline.c */

#if defined(HAVE_LIBREADLINE)
# include "stdfn.h" /* <readline/readline.h> needs stdio.h... */
# include <readline/readline.h>
#endif
#if defined(HAVE_LIBEDITLINE)
# include <editline/readline.h>
#endif

#if defined(HAVE_LIBEDITLINE)
int getc_wrapper __PROTO((FILE* fp));
#endif

#if defined(READLINE) && !defined(HAVE_LIBREADLINE) && !defined(HAVE_LIBEDITLINE)
char* readline __PROTO((const char*));
#endif

/*
 * The following 'readline_ipc' routine is usual 'readline' for OS2_IPC,
 * and a special one for IPC communication.
 */
char* readline_ipc __PROTO((const char*));

#endif /* GNUPLOT_READLINE_H */
