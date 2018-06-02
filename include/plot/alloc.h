/*
 * $Id: alloc.h,v 1.14 2014/08/18 04:43:29 sfeam Exp $
 */

/* GNUPLOT - alloc.h */



#ifndef GNUPLOT_ALLOC_H
# define GNUPLOT_ALLOC_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "stdfn.h"

/* prototypes from "alloc.c" */

generic* gp_alloc __PROTO((size_t size, const char* message));
generic* gp_realloc __PROTO((generic* p, size_t size, const char* message));

#endif /* GNUPLOT_ALLOC_H */
