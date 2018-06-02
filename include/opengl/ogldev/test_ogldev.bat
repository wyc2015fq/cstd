@echo off
rm test_ogldev.inl
for %%a in (tutorial*.inl) do (
 echo #include "%%a">test_ogldev.inl
)
