/*
 * $Id: help.h,v 1.8 2004/04/13 17:23:56 broeker Exp $
 */

/* GNUPLOT - help.h */




#ifndef GNUPLOT_HELP_H
# define GNUPLOT_HELP_H

#include "gp_types.h"

/* Exit status returned by help() */
#define H_FOUND   0 /* found the keyword */
#define H_NOTFOUND  1 /* didn't find the keyword */
#define H_ERROR   (-1)  /* didn't find the help file */

/* Prototypes from file "help.c" */

int  help __PROTO((char* keyword, char* path, TBOOLEAN* subtopics));
void FreeHelp __PROTO((void));
void StartOutput __PROTO((void));
void OutLine __PROTO((const char* line));
void EndOutput __PROTO((void));

#endif /* GNUPLOT_HELP_H */
