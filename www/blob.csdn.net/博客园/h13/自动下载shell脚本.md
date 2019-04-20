# 自动下载shell脚本 - h13 - 博客园
```
#/bin/sh 
i=0 
chapter=$1 
book_name=$2 
while [ $i != $3 ] 
do 
    echo "第 $i 正在下载第 $book_name 个文件..." 
    wget http://www.qqshuwu.com/files/article/attachment/5/5759/$chapter/$book_name.gif 
    i=`expr $i + 1` 
    book_name=`expr $book_name + 1` 
    chapter=`expr $chapter + 1` 
done
```
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/do2jiang/archive/2010/04/21/5511043.aspx](http://blog.csdn.net/do2jiang/archive/2010/04/21/5511043.aspx)
