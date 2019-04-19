# shell常见用法几则 - 三少GG - CSDN博客
2013年08月10日 16:57:29[三少GG](https://me.csdn.net/scut1135)阅读数：1149
## 【Shell】逐行读取并处理文本（文件）
对输入的文本按行读取并进行处理，可使用如下代码： 
```
catdata.dat | while read line
do
    echo"File: ${line}"
done
 
while read line
do
    echo"File: ${line}"
done< data.dat
```

以上两种取其一即可，只是第一种由于采用了管道引用更广，可将其他命令的输出作为输入；以上只是简单了进行了echo输出，可采用更复杂的命令进行代替。
如果输入文本每行中没有空格，也可以采用以下两种进行处理，代码如下：
```
for line in$(catdata.dat)
do
    echo"File: ${line}"
done
 
for line in`catdata.dat`
do
    echo"File: ${line}"
done
```
如输入文本行中包括空格，那么会以空格来区分开多个元素，结果就不是按行读取了。
当然如果只是单纯的数据或文本的按行读取和显示的话，使用awk命令会更为方便。
p.s. Csh中逐行读取并处理文本的做法见[这里](http://witmax.cn/csh-read-line.html)。
*************************************************************************************************************
### [linux shell 数组的长度计算、修改、循环输出等操作](http://blog.csdn.net/yy_done/article/details/7242309)
分类： [shell编程](http://blog.csdn.net/snrqtdhuqf/article/category/1072972)
[shell](http://blog.csdn.net/tag/details.html?tag=shell)[linux](http://blog.csdn.net/tag/details.html?tag=linux)[c](http://blog.csdn.net/tag/details.html?tag=c)
在shell中，数组变量的复制有两种方法：
(1) name = (value1 ... valuen)此时下标从0开始
(2) name[index] = value
example:
**[plain]**[view
 plain](http://blog.csdn.net/yy_done/article/details/7242309#)[copy](http://blog.csdn.net/yy_done/article/details/7242309#)
- #1/bin/sh  
- #arrayTest  
- name=(yunix yhx yfj)  
- echo "array is:${name[@]}"  
- echo "array length is:${#name[*]}"  
- echo ${name[1]}  
- name[1]=yang  
- echo ${name[1]}  
- read -a name  
- echo ${name[1]}  
- echo "loop the array"  
- len=${#name[*]}  
- i=0  
- while [ $i -lt $len ]  
- do  
- echo ${name[$i]}  
- let i++  
- done  
result:
array is:yunix yhx yfj
array length is:3
yhx
yang
a b c d e
b
loop the array
a
b
c
d
e
下面的是关于数组的输出实例
example:
**[plain]**[view
 plain](http://blog.csdn.net/yy_done/article/details/7242309#)[copy](http://blog.csdn.net/yy_done/article/details/7242309#)
- #!/bin/sh  
- #arrayLoopOut  
- read -a array  
- len=${#array[*]}  
- echo "array's length is $len"  
- echo "use while out the array:"  
- i=0  
- while [ $i -lt $len ]  
- do  
-         echo -n "${array[$i]}"  
- let i++  
- done  
- echo  
- echo "use for out the array:"  
- for ((j=0;j<"$len";j=j+1))  
- do  
-         echo -n ${array[$j]}  
- done  
- echo  
- echo "use for in out the array:"  
- for value in ${array[*]}  
- do  
- echo -n $value  
- done  
result:
a b c d e f g
array's length is 7
use while out the array:
abcdefg
use for out the array:
abcdefg
use for in out the array:
abcdefg
****************************************************************************************
Linux Shell学习：使用read从标准输入中读取一行
r**ead是Linux的一个内建命令**，主要完成对参数的赋值，类似C语言中的 scanf。其不仅可以赋值变量，还可以赋值数组；其输入不仅是屏幕，还可以是文件描述符。
## 一、read 命令用法和参数
**用法：**
**read [-ers] [-a array] [-d delim] [-i text] [-n nchars] [-N nchars] [-p prompt] [-t timeout] [-u fd] [name ...]**
或
**read [ -p ][  -r ][ -s ][ -u[ n ] ] [  VariableName?Prompt ][ VariableName ... ]**
**说明：**
read 命令从标准输入中读取一行，并把输入行的每个字段的值指定给 shell 变量，用 IFS（内部字段分隔符）变量中的字符作为分隔符。VariableName 参数指定 shell 变量的名称，shell 变量获取输入行一个字段的值。
由VariableName 参数指定的第一个 shell 变量指定给每一个字段的值，由VariableName 参数指定的第二个 shell 变量指定给第二个字段的值，以此类推，直到最后一个字段。如果标准输入行的字段比相应的由 VariableName 参数指定的 shell 变量的个数多，把全部余下的字段的值赋给指定的最后的 shell 变量。如果比 shell 变量的个数少，则剩余的 shell 变量被设置为空字符串。
**参数：**
**-a** 后跟一个变量，该变量会被认为是个数组，然后给其赋值，默认是以空格为分割符。
**-d** 后面跟一个标志符，其实只有其后的第一个字符有用，作为结束的标志，会举例说  明。
**-p** 后面跟提示信息，即在输入前打印提示信息。
**-e** 在输入的时候可以时候命令补全功能。
**-n** 后跟一个数字，定义输入文本的长度，很实用。
**-r** 屏蔽，如果没有该选项，则作为一个转义字符，有的话 就是个正常的字符了。
**-s** 安静模式，在输入字符时不再屏幕上显示，例如login时输入密码。
**-t** 后面跟秒数，定义输入字符的等待时间。
**-u** 后面跟fd，从文件描述符中读入，该文件描述符可以是exec新开启的。
## 二、示例
**1、-a 举例：**
[root@Centos6 ~]# read -a tao
qq ww ee rr
[root@Centos6 ~]# echo ${tao[1]}
Ww
hh
（输入一个e）
[root@Centos6 ~]#echo ${tao[3]}
Hh
**2、-b 举例：**
[root@Centos6 ~]# read -d eof -a tao
ww
dd
gg
**3、-n 举例：**
[root@Centos6 ~]# read -n3 -p “you can input 3 word:”
you can input 3 word:xxx
（输入3个字符后自动退出！）
**4、-e 举例：**
[root@Centos6 ~]#read -e file
（tab键补全）
exp1               file               ngis_post.sh       text
exp5               linux-2.6.27.54/   test               xen-3.0.1-install/
**5、-s 举例：**
[root@Centos6 ~]#read -p password: -s passwd
password:
（这时候输入就不再显示在屏幕上了！）
**6、-u 举例：**
[root@Centos6 ~]# exec 3<file
[root@Centos6 ~]# read -u 3 tao
[root@Centos6 ~]# echo $tao
hello world!
注意看下读入的次序：
[root@Centos6 ~]# cat file
hello world!
i am good
[root@Centos6 ~]# exec 3<file
[root@Centos6 ~]# read -u 3 tao
[root@Centos6 ~]#test#echo $tao
hello world!
[root@Centos6 ~]# read -u 3 tao
[root@Centos6 ~]# echo $tao
i am good
[root@Centos6 ~]#
这个选项很有用的，特别是自循环读入的时候。
7、下列脚本打印一个文件，这个文件中每行的第一个字段被移动到本行的末尾。
while read -r xx yy
do
print printf "%s %s/n" $yy $xx
done < InputFile
8、为创建一个共同进程，用 print -p 写到共同进程，并用 read -p 从共同进程中读取输入，请输入：
(read; print "hello $REPLY")
print -p "world"
read-p line
变量 line 的值应该是 "hello world."
9、读取一行，把它分成字段，并使用 "Please enter: " 作为提示符，
请输入：
read word1?"Please enter:  " word2
系统显示：
Please enter:
You enter:
hello world
变量 word1 的值应该是 "hello"，变量 word2 应该是 "world."
10、为把输入行的副本保存为历史文件中的一个命令，请输入：
read -s line < input_file
如果输入文件包含 "echo hello world," ，那么在历史记录文件中将会把 "echo hello world"　保存为一个命令。
************************************************************************************************************
读文件的方法：
**第一步：** 将文件的内容通过管道（|）或重定向（<）的方式传给while
**第二步： **while中调用read将文件内容一行一行的读出来，并付值给read后跟随的变量。变量中就保存了当前行中的内容。
例如读取文件/sites/linuxpig.com.txt
**1）管道的方式：**
     cat /sites/linuxpig.com.txt |while read LINE
     do
         echo $LINE
     done
    当然也可以将cat /sites/linuxpig.com.txt 写成一些复杂一些的，比如：
** 示例1：**
    find -type f -name "*.txt" -exec cat |while read LINE
     do
         echo $LINE
     done
    可以将当前目录所有以 .txt 结尾的文件读出
**   示例2：**
    grep -r "linuxpig.com" ./ | awk -F":" '{print $1}' | cat |while read LINE
     do
         echo $LINE
     done
    可以将含有 "linuxpig.com" 字符串的所有文件打开并读取。。
    示例没有实际测试，如果使用请先测试。。。。。:-)
**2）重定向的方式：**
**  2.1 利用重定向符<**
     while read LINE
     do
         echo $LINE
     done < /sites/linuxpig.com.txt
**  2.2 利用文件描述符（0~9）和重定向符 <**
     exec 3<&0
     exec 0</sites/linuxpig.com.txt
     while read LINE
     do
         echo $LINE
     done
     exec 0<&3
*************************************************************************************************************
### linux中的shell脚本创建文件夹，若文件夹存在，则删除重新创建，若不存在，直接创建
阅
#!/bin/bash
dire="/tmp/mydire/"
if [ -d "$dire" ]; then
rmdir "$dire"
mkdir "$dire"
else
mkdir "$dire"
fi
if [ ! -d "$dire" ]; then
mkdir "$dire"
fi
注意：if判断时 [ ]与里面的语句要有个空格:if [ -d "$dire" ]; then
find ./ -type f |awk -F"./" '{print $2}' >filename即可得到文件名文件
find ./ -type f |awk -F"./" '{print "rm -rf",$2}' >filename即可得到删除所有文件的命令脚本
************************************************************************************************************
### linux shell的一些技巧（五）exec与文件描述符
|字号订阅
在*nux系统中，一般在操作文件时会直接使用文件名，在支持POSIX的各种环境中，提供了一系列用于操作文件的工具，短小精悍却又强大得变态。在shell环境下，还经常文件描述符（正整数）来操作文件。
系统默认打开的描述符有3个：
    0   - 标准输入
    1   - 标准输出
    2   - 标准出错
所以，只要够斯文（别整太大的数，它不仅难记，而且你不需要），几乎所有的正整数都可以绑定到某个文件。
1、通过exec分配文件描述符
#!/bin/sh
exec 3<>hello.txt           # 以读写方式绑定到文件描述符"3"
echo "hello exec" >&3       # 写入"hello exec"，如果之前有内容，这里将会从文件开头进行覆盖
echo "hello world" >&3      # 写入"hello world“，新的一行！
exec 3>&-                   # 关闭写，禁止写,然而，实际上它也不能读了～
                            # 如果是exec 3<&-，关闭读，同时它也不能写了～
在上面的示例中，将文件hello.txt绑定到了描述符3，如果要将输出重定向到hello.txt，只需要略作修改
2、将标准输出重定向到hello.txt
#!/bin/sh
exec 1>hello.txt        # 将输出重定向到文件hello.txt，从此以后，这个脚本中的输出都将被写入文件hello.txt
echo "hello exec"
echo "hello world"
就这么简单，在终端再也见不着输出了，因为你已经明确地告诉他，把这些信息写到文件去……
3、在上面这个示例中，输出被重定向了，如果要恢复怎么办？
#!/bin/sh
exec 100>&1             # 将文件描述符100连接到标准输出
                        # 由于之后还要输出到终端，所以我们不得不使用一个临时的描述符来保存它！
exec 1>hello.txt        # 将输出重定向到文件hello.txt，从此以后，这个脚本中的输出都将被写入文件hello.txt
echo "hello exec"
echo "hello world"
exec 1>&100 100>&-      # 将标准输出连接到100，这是之前保存的标准输出
                        # 将描述符100关了，一了百了，因为已经还原标准输出了，留着它实在没必要
echo "oh, my god!"      # 从这句开始将显示在终端上
4、标准输入的操作与2、3相似，只是使用的是"<"
#!/bin/sh
exec 100<&0
exec <hello.txt
read line1
echo $line1
read line2
echo $line2
exec 0<&100 100>&-
read custom
*****************************************************************************
[shell--重定向](http://bbsanwei.iteye.com/blog/376299)
标准输入是文件描述符0.它是命令的输入，缺省是键盘，也可以是文件或其它的命令输出 
标准输出 
标准输出是文件描述符1.它是命令的输出，缺省是屏幕，也可以是文件 
标准错误 
标准错误是文件件描述符2。它是命令错误码率的输出，缺省是屏幕，同样也可以是文件. 
重定向操作符 描述 
>        将命令输出写入到文件或设备（如打印机），而不是命令提示符窗口或句柄。 
<        从文件而不是从键盘或句柄读入命令输入。 
>>       将命令输出添加到文件末尾而不删除文件中已有的信息。 
>&       将一个句柄的输出写入到另一个句柄的输入中。 
<&       从一个句柄读取输入并将其写入到另一个句柄输出中。 
|        从一个命令中读取输出并将其写入另一个命令的输入中。也称作管道。 
常用文件重定向命令 
Shell代码  ![收藏代码](http://bbsanwei.iteye.com/images/icon_star.png)
- command > filename              把标准输出重定向到一个新文件中  
- command >> filename             把标准输出重定向到一个文件中(追加)  
- command 1 > fielname            把标准输出重定向到一个文件中  
- command > filename 2>&1         把标准输出和标准错误一起重定向到一个文件中  
- command 2 > filename            把标准错误重定向到一个文件中  
- command 2 >> filename           把标准输出重定向到一个文件中(追加)  
- command >> filename 2>&1        把标准输出和标准错误一起重定向到一个文件中(追加)  
- command < filename >filename2   把command命令以filename文件作为标准输入，以filename文件作为标准输出  
- command < filename              把command命令以filename文件作为标准输入  
- command << delimiter            把从标准输入中读入，直至遇到d e l i m i t e r分界符  
- command <&m                     把把文件描述符m作为标准输入  
- command >&m                     把把标准输出重定向到文件描述符m中  
- 
*********************************************************************************************
## Linux shell下查看文件/文件夹大小的命令
Linux shell下查看文件/文件夹大小的命令是"du"，如下
#** du -s -h filename**
选项"-s"：如果输入为文件夹，表示文件夹总共的大小，如果是文件，则不起作用。
选项"-h"：表示human readable，显示人可读的单位KB, MB, GB。
至于"du"命令的其它选项查手册就可以了。
********************************************************************************************
[linux
 shell 脚本实现tcp/upd协议通讯（重定向应用）](http://www.cnblogs.com/chengmo/archive/2010/10/22/1858302.html)
**三、通过shell脚本重定向实现监控memcache状态**
> 
**实例：**
[?](http://www.cnblogs.com/chengmo/archive/2010/10/22/1858302.html#)
```
```bash
#!/bin/sh
```
```bash
#通过传入ip
 以及端口，发送指令获得返回数据
```
```bash
#copyright
 chengmo qq:8292669
```
```bash
#函数往往放到最上面
```
```bash
function
```
```bash
sendmsg()
```
```bash
{
```
```bash
```
```bash
msg=$1;
```
```bash
```
```bash
echo
```
```bash
"$1"
```
```bash
>&8;
```
```bash
```
```bash
getout;
```
```bash
}
```
```bash
#向socket通道发送指令，并且调用获得返回参数
```
```bash
function
```
```bash
getout()
```
```bash
{
```
```bash
```
```bash
#read
 命令 -u 从打开文件描述符 8 读取数据，-d读取数据忽略掉:\r换行符
```
```bash
```
```bash
while
```
```bash
read
```
```bash
-u
 8 -d $
```
```bash
'\r'
```
```bash
name;
```
```bash
```
```bash
do
```
```bash
```
```bash
if
```
```bash
[
```
```bash
"${name}"
```
```bash
==
```
```bash
"END"
```
```bash
-o
```
```bash
"${name}"
```
```bash
==
```
```bash
"ERROR"
```
```bash
];
```
```bash
then
```
```bash
```
```bash
break
```
```bash
;
```
```bash
```
```bash
fi
```
```bash
;
```
```bash
```
```bash
echo
```
```bash
$name;
```
```bash
```
```bash
done
```
```bash
}
```
```bash
#由于：memcached每次通讯完毕，会返回：END或者ERROR(出错），通过判断是否是"END"觉得读取是不是结束，否则循环不会停止
```
```bash
if
```
```bash
[
 $
```
```bash
#
 -lt 2 ];then
```
```bash
```
```bash
echo
```
```bash
"usage:$0
 host port [command]"
```
```bash
;
```
```bash
```
```bash
exit
```
```bash
1;
```
```bash
fi
```
```bash
;
```
```bash
[[
 $
```
```bash
#
 -gt 2 ]]&&command=$3;
```
```bash
#设置默认值
 如果command为定义则为：stats
```
```bash
command
```
```bash
=
```
```bash
"${command=stats}"
```
```bash
;
```
```bash
host=
```
```bash
"$1"
```
```bash
;
```
```bash
port=
```
```bash
"$2"
```
```bash
;
```
```bash
exec
```
```bash
8<>
```
```bash
/dev/tcp/
```
```bash
${host}/${port};
```
```bash
#打开通向通道是8
```
```bash
if
```
```bash
[
```
```bash
"$?"
```
```bash
!=
```
```bash
"0"
```
```bash
];
```
```bash
then
```
```bash
```
```bash
echo
```
```bash
"open
 $host  $port fail!"
```
```bash
;
```
```bash
```
```bash
exit
```
```bash
1;
```
```bash
fi
```
```bash
sendmsg
```
```bash
"$command"
```
```bash
;
```
```bash
#发送指定命令
```
```bash
sendmsg
```
```bash
"quit"
```
```bash
;
```
```bash
#发送退出通向命令
```
```bash
exec
```
```bash
8<&-;
```
```bash
exec
```
```bash
8>&-;
```
```bash
#关闭socket通道
```
```bash
exit
```
```bash
0;
```
```
这是通过重定向，实现socket通讯中，发送然后获取返回的例子。其实，上面代码看似一次只能发送一段。时间上。我们可以反复调用：sendmsg ，捕捉输出数据。实现连续的，读与写操作。
实例截图：
**其它实现方法：**
其实通过：telnet也可以实现的。
[chengmo@centos5 shell]$ (echo "stats";sleep 2)|telnet 127.0.0.1 11211
通过nc命令实现：
[chengmo@centos5 shell]$ (echo "stats")|nc 127.0.0.1 11211
不需要加延迟，直接打开通道
/***********************************
Shell Scripts 
./serversocket   (ip:192.168.1.123 port:8090)
exec 8<>/dev/tcp/192.168.1.123/8090;echo "abc">&8
netstat |grep 8090
************************************/
