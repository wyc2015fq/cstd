# shell向awk传值 - Orisun - 博客园







# [shell向awk传值](https://www.cnblogs.com/zhangchaoyang/articles/2355230.html)





方法1

awk  'pattern { action }'  awkvar1=shellvar1  awkvar2=shellvar2  ...  input_files

awk_script_file  awkvar1=shellvar1  awkvar2=shellvar2  ...  input_files

这要在pattern或action中就可以使用awkvar1、awkvar2了

方法2

#!/bin/bash

name="Jack White"

awk  '$1~/"'"$name"'"/  {printf $1"的成绩是：" $5}'  input_files

"'"是一个双引号、一个单引号、再一个双引号












