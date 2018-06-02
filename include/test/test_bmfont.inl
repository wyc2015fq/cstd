
#define fopen_s(pf, fn, mode)  (*(pf) = fopen(fn, mode))

#include "E:/code/c/file/Font/bmfont/libbmfont.inl"
#pragma comment(lib,"Comctl32.lib")
#pragma comment(lib,"Usp10.lib")
int test_bmfont() {
  return 0;
}
