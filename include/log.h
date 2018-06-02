#ifndef _LOG_H_
#define _LOG_H_
#include <stdio.h>
static FILE* logfile = NULL;
static int log_setfile(const char* fname)
{
  if (NULL != logfile) {
    fclose(logfile);
    logfile = NULL;
  }
  if (NULL != fname) {
    logfile = fopen(fname, "a+");
  }
  return NULL != logfile;
}
int log_printf(const char* FMT, ...)
{
  if (NULL == logfile) {
    log_setfile("log.txt");
  }
  {
    va_list ARGLIST;
    va_start(ARGLIST, FMT);
    vfprintf(logfile, FMT, ARGLIST);
    va_end(ARGLIST);
  }
  return 0;
}
#endif // _LOG_H_

