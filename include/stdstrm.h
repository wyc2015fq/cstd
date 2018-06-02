#ifndef _STDSTRM_H_
#define _STDSTRM_H_
//#define SUPOTSTRM
//#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "tchar_x.h"
typedef void* shandle_t;
typedef void* data_t;
//typedef void (*STRMErrorHandler)(const TCHAR*, const TCHAR*, va_list);
typedef int (*STRMReadWriteProc)(shandle_t, data_t, int);
typedef int (*STRMSeekProc)(shandle_t, int, int);
typedef int (*STRMCloseProc)(shandle_t);
typedef int (*STRMSizeProc)(shandle_t);
typedef int (*STRMTellProc)(shandle_t);
typedef int (*STRMVprintfProc)(shandle_t h, const TCHAR* format, va_list va);
//typedef int (*STRMMapFileProc)(shandle_t, data_t*, int*);
//typedef void (*STRMUnmapFileProc)(shandle_t, data_t, int);
//typedef void (*STRMExtendProc)(STRM*);
typedef struct STRM {
  shandle_t h;
  STRMReadWriteProc read;
  STRMReadWriteProc write;
  STRMSeekProc seek;
  STRMCloseProc close;
  STRMSizeProc size;
  STRMTellProc tell;
  STRMVprintfProc vprintf;
}
STRM;
#define sread(_s, _buffer, _size)  (_s)->read((_s)->h, _buffer, _size)
#define swrite(_s, _buffer, _size) (_s)->write((_s)->h, (void*)(_buffer), _size);
#define sseek(_s, _offset, _origin)  (_s)->seek((_s)->h, _offset, _origin)
#define ssize(_s)  (_s)->size((_s)->h)
#define stell(_s)  (_s)->tell((_s)->h)
#ifdef _WIN
#define sclose(_s) ((_s)->close((_s)->h), free(_s), (_s)=(0))
#else
#define sclose(_s) ((_s)->close((_s)->h), free(_s))
#endif
#define svprintf(_s, _format, _ARGLIST) (_s)->vprintf((_s)->h, _format, _ARGLIST)
#if 0
static int sfprintf(STRM* s, const TCHAR* format, ...)
{
  int re;
  va_list ARGLIST;
  va_start(ARGLIST, format);
  re = svprintf(s, format, ARGLIST);
  va_end(ARGLIST);
  return re;
}
#endif
//static int sSize(STRM*);
#define STDSTRM_C_INL
#ifdef STDSTRM_C_INL
static STRM* strmopen(const TCHAR* filename, const TCHAR* mode, const TCHAR* type);
#include "stdstrm_c.inl"
#else
STRM* strmopen(const TCHAR* filename, const TCHAR* mode, const TCHAR* type);
#endif //inl
#endif // _STDSTRM_H_

