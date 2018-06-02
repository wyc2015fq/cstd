/* Copyright (C) 2001-2005 Peter Selinger.
   This file is part of potrace. It is pfree software and it is covered
   by the GNU General Public License. See the file COPYING for details. */

/* $Id: backend_eps.h,v 1.7 2005/02/22 21:31:23 selinger Exp $ */

#ifndef BACKEND_EPS_H
#define BACKEND_EPS_H

#include "potracelib.h"
#include "main.h"

int init_ps(FILE *fout);
int page_ps(FILE *fout, potrace_path_t *plist, imginfo_t *imginfo);
int term_ps(FILE *fout);

int page_eps(FILE *fout, potrace_path_t *plist, imginfo_t *imginfo);

#endif /* BACKEND_EPS_H */

