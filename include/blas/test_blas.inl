//
#include "openblas/openblas.inl"


int test_blas() {
  enum {N=100, };
  //cblas_sgemm() sgemm_otcopy
  float a[N*N];
  float b[N*N];
  float c[N*N];
  int i;
  for (i=0; i<N*N; ++i) {
    a[i]=1;
    b[i]=1;
    c[i]=1;
  }
  uint64 aa, bb, cc;
  aa = rpcc();
  cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, N, N, N, 1, a, N, b, N, 1, c, N);
  bb = rpcc();
  cc = bb - aa;
  return 0;
}
#if 0



--------------------Configuration: testc - Win32 Debug--------------------
Compiling...
testcpp.cpp
Linking...
testcpp.obj : error LNK2001: unresolved external symbol "int __cdecl dgemm_kernel(long,long,long,double,double *,double *,double *,long)" (?dgemm_kernel@@YAHJJJNPAN00J@Z)
testcpp.obj : error LNK2001: unresolved external symbol "int __cdecl dgemm_oncopy(long,long,double *,long,double *)" (?dgemm_oncopy@@YAHJJPANJ0@Z)
testcpp.obj : error LNK2001: unresolved external symbol "int __cdecl dgemm_otcopy(long,long,double *,long,double *)" (?dgemm_otcopy@@YAHJJPANJ0@Z)
testcpp.obj : error LNK2001: unresolved external symbol "int __cdecl dgemm_beta(long,long,long,double,double *,long,double *,long,double *,long)" (?dgemm_beta@@YAHJJJNPANJ0J0J@Z)
Debug/testc.exe : fatal error LNK1120: 4 unresolved externals
Error executing link.exe.
Creating browse info file...

testc.exe - 5 error(s), 0 warning(s)




#endif