# shell:读取文件的每一行内容并输出 - sxf_123456的博客 - CSDN博客
2017年04月26日 19:12:47[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：775标签：[脚本																[shell](https://so.csdn.net/so/search/s.do?q=shell&t=blog)](https://so.csdn.net/so/search/s.do?q=脚本&t=blog)
个人分类：[shell](https://blog.csdn.net/sxf_123456/article/category/6881619)
                
shell:读取文件的每一行内容并输出 
写法一：
----------------------------------------------------------------------------
#!/bin/bash
while read line
do
    echo $line
done < file(待读取的文件)
----------------------------------------------------------------------------
写法二：
----------------------------------------------------------------------------
#!/bin/bash
cat file(待读取的文件) | while read line
do
    echo $line
done
----------------------------------------------------------------------------
写法三：
----------------------------------------------------------------------------
for line in `cat file(待读取的文件)`
do
    echo $line
done
----------------------------------------------------------------------------
说明：
for逐行读和while逐行读是有区别的,如:
$ cat file
aaaa
bbbb
cccc dddd
$ cat file | while read line; do echo $line; done
aaaa
bbbb
cccc dddd
$ for line in $(<file); do echo $line; done
aaaa
bbbb
cccc
dddd
== 实践 === 
#! bin/sh
#$str='http://images.stylight.de/static/res200/s2870/2870657.1.jpg%0D'
#echo ${str##*fo}
#echo ${str#fo}
while read line
do
   wget -p ${line:0:59}
done < '/root/mysql/mysql.log';
            
