# awk调用shell命令与getline和system函数 - sxf_123456的博客 - CSDN博客
2017年04月25日 10:49:57[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：314标签：[shell																[awk](https://so.csdn.net/so/search/s.do?q=awk&t=blog)](https://so.csdn.net/so/search/s.do?q=shell&t=blog)
个人分类：[shell](https://blog.csdn.net/sxf_123456/article/category/6881619)
## awk调用shell命令与getline和system函数
如下内容from [http://blog.sina.com.cn/s/blog_3ef3b17a0100dzw0.html](http://blog.sina.com.cn/s/blog_3ef3b17a0100dzw0.html)有颜色的内容为自己添加的。
awk可以用getline或system来调用shell的命令
1getline
通过在awk内使用管道,可以把shell命令的输出传送给awk
$ awk 'BEGIN{ "date" | getline date; print date; }'
Sun Mar 8 22:21:52 2009
要记得：
awk 'BEGIN{“date”|getline d;print d;close(“date”)}'（close(“date”)）执行完一个命令，最好立即调用close（command）)
awk 'BEGIN{while("ls"|getline ls)print ls;}'
CVS
CVSROOT
Client
EJB_CMP2.0
EJB_EntityBean_id
EJB_EntityBean_id_BMP
EJB_SessionBean
EJB_Session_facade
EJB_local_remote
MDB
注意：这里不是getline执行了shell命令是
 “command”中的双引号被解析为执行引号内部的命令，getline只是获得了执行结果的输出而已.
顺便说一下getline的用法.
getline 是awk里用于输入重定向的一个函数,他可以从标准输入/一个管道/文件读取输入, 而不只是从当前被处理的文件来处理, 他取得输入的下一行并给NF,NR,FNR等内制变量置值, 如果找到一条记录则getline返回1,如果到了文件结束(EOF)则返回0,如果错误则返回-1
getline除了可以**通过管道从shell命令里读取数据**外,它还可以从标准输入(用"-"指定从标准输入读入,或者如果命令行没有任何输入文件且不用
 重定向符"<"指定文件,默认也是从标准输入读)和文件里读取数据;如果getline后面没有指定变量,则读取的数据会放到$0里面
$ awk 'BEGIN{ getline; print $0 }'
hello, for note <==这一行是从键盘输入的（因为没有任何输入文件，默认键盘为标准输入）
hello, for note <==这一行是awk的print
 $0 输出的
$ cat sed.test
hello
$ awk 'BEGIN{ getline < "sed.test"; print $0 }'
hello
$ awk 'BEGIN{ getline < "-"; print $0 }'
hello
hello
如果读取成功,getline返回1;如果读到文件结束符,getline返回0;如果发生错误,例如打不开文件,getline返回-1. 所以,getline可以用于while循环中
$ cat grep.test
hello
myboy
third
fourth
$ awk 'BEGIN{ while( getline line < "grep.test" ){ print line } }'
hello
myboy
third
fourth
2 **system**
system的调用形式是system(cmd).**system****的返回值是cmd的退出状态.如果要获得cmd的输出,就要和getline结合使用**
$ awk 'BEGIN{ while( system("ls -l") | getline line ){ print line } }'
//这个命令和awk
 'BEGIN{while("ls"|getline ls)print ls;}' 结果相同。故双引号是被替换为system，其内容当作cmd被执行
total 14
-rw-r--r-- 1 Administrator None 2 Mar 3 20:20 4
-rw-r--r-- 1 Administrator None 0 Feb 28 19:49 4]]
-rw-r--r-- 1 Administrator None 87 Mar 5 20:22 awk.test
-rwxr-xr-x 1 Administrator None 409 Mar 3 22:24 bc.test
-rwxrwxrwx 1 Administrator None 411 Mar 3 22:23 bc.test.bak
-rw-r--r-- 1 Administrator None 48 Feb 21 21:06 func.test
-rw-r--r-- 1 Administrator None 25 Feb 27 21:37 grep.test
-rw-r--r-- 1 Administrator None 6 Feb 27 20:59 kk
-rw-r--r-- 1 Administrator None 5 Mar 8 15:11 log.test
-rw-r--r-- 1 Administrator None 0 Feb 27 21:22 orld
-rw-r--r-- 1 Administrator None 75 Mar 8 12:14 perl.test
-rw-r--r-- 1 Administrator None 6 Mar 6 20:22 sed.test
-rw-r--r-- 1 Administrator None 12 Feb 27 21:32 sed.write
sh: 0: command not found
getline line < "small.txt"`awk 'BEGIN { while ("ls -l" | getline var) system("echo " var );}'`
