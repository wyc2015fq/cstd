

/*
 * AUTHORS
 *
 * Alexander Lehmann (collected functions from misc.c and binary.c)
 *
 */

#include "alloc.h"
#include "util.h" /* for int_error() */

#ifndef GP_FARMALLOC
# ifdef FARALLOC
#  define GP_FARMALLOC(size) farmalloc ((size))
#  define GP_FARREALLOC(p,size) farrealloc ((p), (size))
# else
#  ifdef MALLOC_ZERO_RETURNS_ZERO
#   define GP_FARMALLOC(size) malloc ((size_t)((size==0)?1:size))
#  else
#   define GP_FARMALLOC(size) malloc ((size_t)(size))
#  endif
#  define GP_FARREALLOC(p,size) realloc ((p), (size_t)(size))
# endif
#endif

/* gp_alloc:
 * allocate memory
 * This is a protected version of malloc. It causes an int_error
 * if there is not enough memory. If message is NULL, we allow NULL return.
 * Otherwise, we handle the error, using the message to create the int_error string.
 * Note cp/sp_extend uses realloc, so it depends on this using malloc().
 */

generic*
gp_alloc(size_t size, const char* message)
{
  char* p;      /* the new allocation */

  p = GP_FARMALLOC(size); /* try again */

  if (p == NULL) {
    /* really out of memory */
    if (message != NULL) {
      int_error(NO_CARET, "out of memory for %s", message);
      /* NOTREACHED */
    }

    /* else we return NULL */
  }

  return (p);
}

/*
 * note gp_realloc assumes that failed realloc calls leave the original mem
 * block allocated. If this is not the case with any C compiler, a substitue
 * realloc function has to be used.
 */

generic*
gp_realloc(generic* p, size_t size, const char* message)
{
  char* res;      /* the new allocation */

  /* realloc(NULL,x) is meant to do malloc(x), but doesn't always */
  if (!p) {
    return gp_alloc(size, message);
  }

  res = GP_FARREALLOC(p, size);

  if (res == (char*) NULL) {
    if (message != NULL) {
      int_error(NO_CARET, "out of memory for %s", message);
      /* NOTREACHED */
    }

    /* else we return NULL */
  }

  return (res);
}


#ifdef FARALLOC
void
gpfree(generic* p)
{
#ifdef _Windows
  HGLOBAL hGlobal = GlobalHandle(p);
  GlobalUnlock(hGlobal);
  GlobalFree(hGlobal);
#else
  farfree(p);
#endif
}

#endif
