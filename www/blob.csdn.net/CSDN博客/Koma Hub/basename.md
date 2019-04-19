# basename - Koma Hub - CSDN博客
2019年01月22日 21:29:48[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：49
libgen.h
```cpp
#ifndef _LIBGEN_H
#define _LIBGEN_H	1
#include <features.h>
__BEGIN_DECLS
/* Return directory part of PATH or "." if none is available.  */
extern char *dirname (char *__path) __THROW;
/* Return final component of PATH.
   This is the weird XPG version of this function.  It sometimes will
   modify its argument.  Therefore we normally use the GNU version (in
   <string.h>) and only if this header is included make the XPG
   version available under the real name.  */
extern char *__xpg_basename (char *__path) __THROW;
#define basename	__xpg_basename
__END_DECLS
#endif /* libgen.h */
```
xpg_basename.c
```cpp
/* Return basename of given pathname according to the weird XPG specification.
   Copyright (C) 1997-2018 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@cygnus.com>, 1997.
   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
#include <string.h>
#include <libgen.h>
char *
__xpg_basename (char *filename)
{
  char *p;
  if (filename == NULL || filename[0] == '\0')
    /* We return a pointer to a static string containing ".".  */
    p = (char *) ".";
  else
    {
      p = strrchr (filename, '/');
      if (p == NULL)
	/* There is no slash in the filename.  Return the whole string.  */
	p = filename;
      else
	{
	  if (p[1] == '\0')
	    {
	      /* We must remove trailing '/'.  */
	      while (p > filename && p[-1] == '/')
		--p;
	      /* Now we can be in two situations:
		 a) the string only contains '/' characters, so we return
		    '/'
		 b) p points past the last component, but we have to remove
		    the trailing slash.  */
	      if (p > filename)
		{
		  *p-- = '\0';
		  while (p > filename && p[-1] != '/')
		    --p;
		}
	      else
		/* The last slash we already found is the right position
		   to return.  */
		while (p[1] != '\0')
		  ++p;
	    }
	  else
	    /* Go to the first character of the name.  */
	    ++p;
	}
    }
  return p;
}
```
