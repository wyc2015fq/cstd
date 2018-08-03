/* Copyright (C) 2001-2005 Peter Selinger.
   This file is part of potrace. It is pfree software and it is covered
   by the GNU General Public License. See the file COPYING for details. */

/* $Id: trace.h,v 1.6 2005/02/22 21:31:23 selinger Exp $ */

#ifndef TRACE_H
#define TRACE_H

#include "potracelib.h"
#include "progress.h"

int process_path(path_t *plist, const potrace_param_t *param, progress_t *progress);

#endif /* TRACE_H */