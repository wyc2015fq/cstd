@echo off
::http://vdisk.weibo.com/s/ugXIxSW8JC6TT
::curl -o aa.htm "http://vdisk.weibo.com/u/1778120900"
::codecvt.exe aa.htm
::http://221.235.187.165/file3.data.weipan.cn/31146720/1fc37fdbfddfde981e8e52e52d3411de2c9b9446?ip=1489331540,113.247.235.178&ssig=4KSIc7lQUx&Expires=1489332140&KID=sae,l30zoo1wmz&fn=%E7%A4%BC%E4%BB%AA%E5%AD%A6--%E8%83%A1%E9%9D%99%202006.pdf&skiprd=2&se_ip_debug=113.247.235.178&corp=2&from=1221134&wsiphost=local
::http://vdisk.weibo.com/s/ugXIxSW8JC1SO?category_id=0&parents_ref=ugXIxSW8JC6TT



for /f "delims=*" %%z in ('dir /b /s list.txt') do (
pushd "%%~dpz"
echo %%~dpz
cat list.txt|sed "/vd_folder/d">list1.txt
for /f "tokens=1 delims=?#" %%i in (list1.txt) do (

echo %%i
md htm
if not exist htm/%%~ni.htm (
 curl -o htm/%%~ni.htm "%%i"
 codecvt.exe htm/%%~ni.htm
)

 cat htm/%%~ni.htm|sed "s/[""]/\n/g"|sed "s#\\\/#/#g"|finds "data.weipan.cn"|sort|uniq>htm/%%~ni_list.txt
for /f %%a in (htm/%%~ni_list.txt) do (
 for /f "tokens=10 delims==&" %%b in ('codecvt /gburl "%%a"') do (
  if not exist "%%b" (
   echo %%b
   curl -o "%%b" "%%a"
  )
 )
)

)


popd

)
