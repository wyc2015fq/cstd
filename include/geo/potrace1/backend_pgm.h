/* Copyright (C) 2001-2005 Peter Selinger.
   This file is part of potrace. It is pfree software and it is covered
   by the GNU General Public License. See the file COPYING for details. */

/* $Id: backend_pgm.h,v 1.7 2005/02/22 21:31:23 selinger Exp $ */

#ifndef BACKEND_PGM_H
#define BACKEND_PGM_H

#include <stdio.h>

#include "potracelib.h"
#include "main.h"

int page_pgm(FILE *fout, potrace_path_t *plist, imginfo_t *imginfo);

#endif /* BACKEND_PGM_H */
