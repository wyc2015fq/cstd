#ifndef lint
static char* RCSid()
{
  return RCSid("$Id: version.c,v 1.111.2.1 2014/08/22 16:07:34 sfeam Exp $");
}
#endif

/* GNUPLOT - version.c */



#include "version.h"

#include "syscfg.h"   /* for FAQ_LOCATION */


const char gnuplot_version[] = "5.0";
const char gnuplot_patchlevel[] = "rc2";
#ifdef DEVELOPMENT_VERSION
#include "timestamp.h"
#else
const char gnuplot_date[] = "2014-08-22 ";
#endif
const char gnuplot_copyright[] = "Copyright (C) 1986-1993, 1998, 2004, 2007-2014";

const char faq_location[] = FAQ_LOCATION;

char* compile_options = (void*)0;   /* Will be loaded at runtime */

const char bug_email[] = "gnuplot-beta@lists.sourceforge.net";
const char help_email[] = "gnuplot-beta@lists.sourceforge.net";
