@echo off
::pclip|finds ": error"|finds "undeclared identifier"|sed "s/_//g">aaa.txt
del clapack.inl
for %%i in (out\*.inl) do (
  echo #include "out\%%~ni.inl">>clapack.inl
)
cat clapack.inl
