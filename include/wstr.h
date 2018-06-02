
#ifndef _WSTR_H_
#define _WSTR_H_


#define isprintW isprint
CC_INLINE size_t wstr_len(const wchar_t* wcs)
{
  const wchar_t *eos = wcs;
  while( *eos++ );
  return( (size_t)(eos - wcs - 1) );
}


#endif // _WSTR_H_
