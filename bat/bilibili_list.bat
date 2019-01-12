@echo off
if not exist "新建文本文档.txt" (
pclip >"新建文本文档.txt"
cat "新建文本文档.txt"|grep "meta data-vue-met"|sed "s#{\"cid\"#\n{\"cid\"#g"|grep "cid"|grep "vupload"|sed "s/[""]//g">list.txt
mv /y "新建文本文档.txt" "新建文本文档_htm.txt"
(for /f "tokens=4,8 delims=:," %%a in (list.txt) do (
  echo %%a、%%b
))>新建文本文档.txt
)

python "D:\code\git\cstd\py\rename_bilibili.py" "%cd%"
pause