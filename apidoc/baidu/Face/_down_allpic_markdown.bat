@echo off


:: set file=../CPU与内存的那些事.md

md asset
pushd asset

cat "../*.md"|findstr "!["|sed "s/!\[/\n!\[/g"|findstr "!\[.*)"|sed "s/)/)\n/g"|findstr "!\[.*)"|sed "s/[()]/\n/g"|sed "s#//#/#g"|findstr "http.*:"|findstr "jpg png gif">_list.txt

for /f "tokens=1,2 delims=:" %%a in (_list.txt) do (
 if not exist %%a md %%a
 if not exist %%a%%b (
  pushd "%%a"
  echo %%b
  wget -c -x "%%a:/%%b"
  popd
 )
)


popd

:end
