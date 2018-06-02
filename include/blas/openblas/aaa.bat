@echo off
del openblas_inc.inl
for %%a in (cblas_sgemm cblas_sscal) do (
  cat %%a.c>>openblas_inc.inl
 echo.>>openblas_inc.inl
 for %%b in (USE_MIN  CBLAS ASMNAME  ASMFNAME  NAME  CNAME  CHAR_NAME CHAR_CNAME COMPLEX) do (
   echo #undef %%b>>openblas_inc.inl
 )
 echo //>>openblas_inc.inl
)
