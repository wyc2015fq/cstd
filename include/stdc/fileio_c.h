
#ifndef _FILEIO_C_H_
#define _FILEIO_C_H_

#include "mem_c.h"

#ifdef _WIN32
#include <io.h>
#include <direct.h>
#else
#include <sys/types.h>
#include <sys/stat.h> 
#include <unistd.h>


#define _mkdir(a) mkdir(a, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)
#define _chdir(a) chdir(a)
#endif // 


/*
For Windows
value	mode
00		Existence only
02		Write-only
04		Read-only
06		Read and write

For linux
F_OK	Existence only
R_OK	Read-only
W_OK	Write-only
X_OK	Executable
*/
static int CheckFileExist(const char* szFile)
{
#ifdef _WIN32
  return _access(szFile, 0) != -1;
#else
  return access(szFile, F_OK) != -1;
#endif
}

static long fsize(FILE* stream)
{
  long pos, size;
  pos = ftell(stream);
  fseek(stream, 0, SEEK_END);
  size = ftell(stream);
  fseek(stream, pos, SEEK_SET);
  return size;
}
static int buf_load(const char* fname, buf_t* bf)
{
  int len, readed_len;
  FILE* pf;
  pf = fname ? fopen(fname, "rb") : NULL;
  if (pf) {
    len = fsize(pf);
    bfsetsize(bf, len);
    readed_len = fread(bf->data, 1, len, pf);
    assert(readed_len == len);
    fclose(pf);
    return 1;
  }
  return 0;
}
#endif // _FILEIO_C_H_
