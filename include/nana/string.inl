
#ifndef __STRING_INL__
#define __STRING_INL__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <tchar.h>

#define countof(arr)  sizeof(arr)/sizeof(arr[0])
#define T_SWAP(T, a, b)   {T t=a;a=b,b=t;}

#define ASSERT assert
typedef wchar_t wchar;

#define MYREALLOC(ptr, len)   *((char**)&ptr) = (char*)realloc(ptr, len*sizeof(*ptr))
#define FREE(ptr)    if (ptr) {free(ptr);ptr=NULL;}

static void astr_clear(char*& astr) {
  MYREALLOC(astr, 1);
  *astr = 0;
}

static void wstr_clear(wchar*& astr) {
  MYREALLOC(astr, 1);
  *astr = 0;
}

static void astr_cpy(char*& astr, int& pos, const char* a) {
  int len = strlen(a);
  MYREALLOC(astr, pos+len+1);
  memcpy(astr+pos, a, len+1);
  pos += len;
}

static void wstr_cpy(wchar*& astr, int& pos, const wchar* a) {
  int len = wcslen(a);
  MYREALLOC(astr, pos+len+1);
  memcpy(astr+pos, a, sizeof(wchar)*(len+1));
  pos += len;
}

#define CString string
struct string {
  char *data;
  typedef string self;
  string() {data = NULL;}
  string(const char* str) {data = NULL; set_str(str); }
  string(const char* str, int l) {data = NULL; set_str(str, l); }
  string(const self& o) {data = NULL; set_str(o.data); }
  ~string() {FREE(data); }
  char* c_str() {return data;};
  const char* c_str() const {return data;};
  int length() const {return strlen(data);}
  int resize(int l) { MYREALLOC(data, l+1); }
  int set_str(const char* str, int l) { resize(l); memcpy(data, str, l); data[l] = 0; }
  int set_str(const char* str) { int l=strlen(str); set_str(str, l); }
};

#endif // __STRING_INL__
