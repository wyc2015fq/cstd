# errno：No such file or directory - Koma Hub - CSDN博客
2019年02月25日 21:56:26[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：34
**Table of Contents**
[dl-minimal.c    elf    9436    7/5/2007    205](#dl-minimal.c%C2%A0%C2%A0%20%C2%A0elf%C2%A0%C2%A0%20%C2%A09436%C2%A0%C2%A0%20%C2%A07%2F5%2F2007%C2%A0%C2%A0%20%C2%A0205)
[errlist.c    sysdeps\gnu    40518    12/25/2005    1](#errlist.c%C2%A0%C2%A0%20%C2%A0sysdeps%5Cgnu%C2%A0%C2%A0%20%C2%A040518%C2%A0%C2%A0%20%C2%A012%2F25%2F2005%C2%A0%C2%A0%20%C2%A01)
[errno.h    sysdeps\mach\hurd\bits    15606    8/27/2002](#errno.h%C2%A0%C2%A0%20%C2%A0sysdeps%5Cmach%5Churd%5Cbits%C2%A0%C2%A0%20%C2%A015606%C2%A0%C2%A0%20%C2%A08%2F27%2F2002%C2%A0%C2%A0%20%C2%A0)
### dl-minimal.c    elf    9436    7/5/2007    205
```cpp
/* Define our own version of the internal function used by strerror.  We
   only provide the messages for some common errors.  This avoids pulling
   in the whole error list.  */
char * weak_function
__strerror_r (int errnum, char *buf, size_t buflen)
{
  char *msg;
  switch (errnum)
    {
    case ENOMEM:
      msg = (char *) "Cannot allocate memory";
      break;
    case EINVAL:
      msg = (char *) "Invalid argument";
      break;
    case ENOENT:
      msg = (char *) "No such file or directory";
      break;
    case EPERM:
      msg = (char *) "Operation not permitted";
      break;
    case EIO:
      msg = (char *) "Input/output error";
      break;
    case EACCES:
      msg = (char *) "Permission denied";
      break;
    default:
      /* No need to check buffer size, all calls in the dynamic linker
	 provide enough space.  */
      buf[buflen - 1] = '\0';
      msg = _itoa (errnum, buf + buflen - 1, 10, 0);
      msg = memcpy (msg - (sizeof ("Error ") - 1), "Error ",
		    sizeof ("Error ") - 1);
      break;
    }
  return msg;
}
```
### errlist.c    sysdeps\gnu    40518    12/25/2005    1
```cpp
#ifdef ENOENT
/*
TRANS No such file or directory.  This is a ``file doesn't exist'' error
TRANS for ordinary files that are referenced in contexts where they are
TRANS expected to already exist. */
    [ERR_REMAP (ENOENT)] = N_("No such file or directory"),
#if ENOENT > ERR_MAX
#undef ERR_MAX
#define ERR_MAX ENOENT
#endif
#endif
```
### errno.h    sysdeps\mach\hurd\bits    15606    8/27/2002    
```cpp
enum __error_t_codes
{
#undef EDOM
#undef ERANGE
	EPERM           = _HURD_ERRNO (1),
#define	EPERM           _HURD_ERRNO (1) /* Operation not permitted */
	ENOENT          = _HURD_ERRNO (2),
#define	ENOENT          _HURD_ERRNO (2) /* No such file or directory */
	ESRCH           = _HURD_ERRNO (3),
#define	ESRCH           _HURD_ERRNO (3) /* No such process */
	EINTR           = _HURD_ERRNO (4),
#define	EINTR           _HURD_ERRNO (4) /* Interrupted system call */
	EIO             = _HURD_ERRNO (5),
#define	EIO             _HURD_ERRNO (5) /* Input/output error */
	ENXIO           = _HURD_ERRNO (6),
#define	ENXIO           _HURD_ERRNO (6) /* No such device or address */
...
}
```
errno.h    sysdeps\unix\bsd\bsd4.4\bits    7008    7/20/2002    
```cpp
#define	EPERM		1		/* Operation not permitted */
#define	ENOENT		2		/* No such file or directory */
#define	ESRCH		3		/* No such process */
#define	EINTR		4		/* Interrupted system call */
#define	EIO		5		/* Input/output error */
#define	ENXIO		6		/* Device not configured */
```
