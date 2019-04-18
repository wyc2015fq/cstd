# Linux中使用gzip来压缩/解压 *.gz文件 - z69183787的专栏 - CSDN博客
2018年08月16日 13:35:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：719
[https://blog.csdn.net/qingsong3333/article/details/77462165](https://blog.csdn.net/qingsong3333/article/details/77462165)
gzip 是linux中常见的压缩/解压工具，最常见的使用对象是*.gz格式的文件，这里简单介绍下它最常见的用法，
GZIP(1) General Commands Manual GZIP(1)
**NAME**
     gzip, gunzip, zcat - compress or expand files
**SYNOPSIS**
     gzip [ -acdfhklLnNrtvV19 ] [--rsyncable] [-S suffix] [ name ... ]
     gunzip [ -acfhklLnNrtvV ] [-S suffix] [ name ... ]
     zcat [ -fhLV ] [ name ... ]
**OPTIONS**
     -c --stdout --to-stdout 结果写到标准输出，原文件保持不变
     -d --decompress --uncompress 解压
     -k --keep 压缩或者解压过程中，保留原文件
     -r --recursive
     -t --test 检查压缩文件的完整性
     -v --verbose 显示每个文件的名子和压缩率
     -# --fast --best 取值从-1(最快)到-9(最好)，默认是-6
**示例1，压缩文件**
原文件名为file1.txt，压缩后原文件消失，压缩后文件名为file1.txt.gz
root@ubuntu:/tmp# ls -l file1.*
-rw-r--r-- 1 root root 12383865 Aug 21 08:08 file1.txt
root@ubuntu:/tmp# gzip file1.txt
root@ubuntu:/tmp# ls -l file1.*
-rw-r--r-- 1 root root 134416 Aug 21 08:08 file1.txt.gz
**示例2，解压文件**
root@ubuntu:/tmp# gzip -d file1.txt.gz
root@ubuntu:/tmp# ls -lh file1.*
-rw-r--r-- 1 root root 12M Aug 21 08:08 file1.txt
**示例3，压缩的时候，显示压缩率**
root@ubuntu:/tmp# gzip -v file1.txt
file1.txt: 98.9% -- replaced with file1.txt.gz
**示例4，一条命令压缩多个文件，压缩之后，是各自分开的：**
root@ubuntu:/tmp# gzip file1.txt file2.txt
root@ubuntu:/tmp# ls -l
total 1348
-rw-r--r-- 1 root root 134416 Aug 21 08:08 file1.txt.gz
-rw-r--r-- 1 root root 392 Aug 21 08:15 file2.txt.gz
**示例5，压缩过程中，保留原文件**
root@ubuntu:/tmp# gzip -k file1.txt
root@ubuntu:/tmp# ls file1.*
file1.txt file1.txt.gz
**示例6，压缩到标准输出中**
可以连接两个文件
root@ubuntu:/tmp# cat file1.txt file2.txt | gzip > foo.gz
或者
root@ubuntu:/tmp# gzip -c file1.txt file2.txt > foo.gz
