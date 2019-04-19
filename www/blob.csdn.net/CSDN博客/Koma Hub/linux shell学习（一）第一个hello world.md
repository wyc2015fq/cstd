# linux shell学习（一）第一个hello world - Koma Hub - CSDN博客
2017年12月14日 21:43:51[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：153
```
#!/bin/bash
#we have less than 3 arguents
# $#表示命令行参数的个数
if [ $# -lt 3 ];then 
    # $*表示命令行参数
    echo $*　
elif [ $# -eq 3 ];then
    echo $*
elif [ $# -gt 3 ];then
    echo $*
fi
# file是在命令行参数中的循环
for file in $*; do
    # 判断file是否是目录中存在的文件
    if [ -f "$file" ]; then
        echo $file is exsit
        #如果有两个参数，那么改名字
        if [ $# -eq 2 ];then
            mv "$1" "$2"
        fi
    fi
done
```
结果：
```
rongtao@rongtao:~/shell$ sh Demo.sh Hello World !
Hello World !
rongtao@rongtao:~/shell$
```
