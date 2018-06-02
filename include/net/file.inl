#include "cfile.h"
//---------------------------------------------------------------------------
BOOL FileExists(LPCTSTR lpszFileName)
{
  DWORD dwAttributes = GetFileAttributes(lpszFileName);
  if (dwAttributes == 0xFFFFFFFF) {
    return FALSE;
  }
  if ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY)
      == FILE_ATTRIBUTE_DIRECTORY) {
    return FALSE;
  }
  else {
    return TRUE;
  }
}
#define DIRSP '\\'
int path_copy(char* p, const char* s, int n, int isaddsp)
{
  int i, j = 0, prev = 0;
  char sp = '\\';
  for (i = 0; i < n; ++i) {
    if ('/' != s[i] && '\\' != s[i]) {
      p[j++] = s[i];
      prev = 0;
    }
    else {
      if (0 == prev && j > 0) {
        p[j++] = DIRSP;
      }
      prev = 1;
    }
  }
  if (isaddsp) {
    if (0 == prev && j > 0) {
      p[j++] = DIRSP;
    }
  }
  else {
    if (j > 0 && DIRSP == p[j]) {
      --j;
    }
  }
  return j;
}

int mkpath(char* buf, int buflen, str_t dir, str_t path, str_t filename)
{
  int i, len = dir.l + path.l + filename.l + 3;
  if (len <= buflen) {
    int n0 = path_copy(buf, dir.s, dir.l, 1);
    int n1 = path_copy(buf + n0, path.s, path.l, 1);
    int n2 = path_copy(buf + n0 + n1, filename.s, filename.l, 0);
    i = 1 + cstr_rfindchr(buf, 0, n0 + n1 + n2, DIRSP);
    i += str2filename(buf + i, -1, buf + i, buflen - n0 - n1);
    buf[len = i] = 0;
  }
  return len;
}
FILE* GetFilePointer(const char* filepath)
{
  FILE* fp;
  char* str = 0;
  int j = 0, n = strlen(filepath), prev = 0;
  if (FileExists(filepath)) {
    // 打开已有文件进行写数据
    // fp = fopen(filepath, "r+b");
    fp = fopen(filepath, "r+b");
  }
  else {
    int len = cstr_rfindchr(filepath, 0, n, DIRSP) + 1;
    char* p = NULL;
    cstr_nredup(&p, filepath, len);
    sys_mkdir(p);
    free(p);
    // 创建新文件进行写数据.
    fp = fopen(filepath, "w+b");
  }
  return fp;
}
DWORD GetFileSizeByName(LPCTSTR lpszFileName)
{
  struct _stat ST;
  if (!FileExists(lpszFileName)) {
    return 0;
  }
  // 获取文件长度.
  _stat(lpszFileName, &ST);
  return ST.st_size;
}
//---------------------------------------------------------------------------
// 从全程文件名中提取短文件名.
char* GetShortFileName(const char* lpszFullPathName)
{
  char* p = strrchr(lpszFullPathName, '/');
  return p ? p + 1 : NULL;
}

