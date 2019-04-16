# shell 脚本学习 - youbingchen的博客 - CSDN博客





2016年05月25日 22:03:25[youbingchen](https://me.csdn.net/youbingchen)阅读数：638标签：[shell](https://so.csdn.net/so/search/s.do?q=shell&t=blog)
个人分类：[Shell脚本](https://blog.csdn.net/youbingchen/article/category/6244865)









shell 本身是一个用C语言编写的程序，它是用户使用linux/Unix的桥梁，用户大部分工作都是通过shell完成的。它虽然不是Unix/Linux系统内核的一部分，但它调用了系统核心的大部分功能来执行程序、建立文件并以并行的方式协调各个程序的运行。因此，对于用户来说，shell是最重要的实用程序，深入了解和熟练掌握shell的特性极其使用方法，是用好Unix/Linux系统的关键。

#### Shell有两种执行方式:
- 交互式：解释执行用户的命令，用户输入一条命令，shell就解释执行一条
- 批处理：用户事先写一个shell脚本，其中有很多条命令，让shell一次性把这些命令执行完，而不必一条一条地敲命令。

Shell脚本和编程语言很相似，也有变量和流程控制语句，但Shell脚本是解释执行的，不需要编译，Shell程序从脚本中一行一行读取并执行这些命令，相当于一个用户把脚本中的命令一行一行敲到Shell提示符下执行。

# Shell 脚本的用处

因为Shell似乎是各UNIX系统之间通用的功能，并且经过了POSIX的标准化。因此，Shell脚本只要“用心写”一次，即可应用到很多系统上。因此，之所以要使用Shell脚本是基于： 

简单性：Shell是一个高级语言；通过它，你可以简洁地表达复杂的操作。 

可移植性：使用POSIX所定义的功能，可以做到脚本无须修改就可在不同的系统上执行。 

开发容易：可以在短时间内完成一个功能强大又妤用的脚本。
但是，考虑到Shell脚本的命令限制和效率问题，下列情况一般不使用Shell： 

资源密集型的任务，尤其在需要考虑效率时（比如，排序，hash等等）。 

需要处理大任务的数学操作，尤其是浮点运算，精确运算，或者复杂的算术运算（这种情况一般使用C++或FORTRAN 来处理）。 

有跨平台（操作系统）移植需求（一般使用C 或Java）。 

复杂的应用，在必须使用结构化编程的时候（需要变量的类型检查，函数原型，等等）。 

对于影响系统全局性的关键任务应用。 

对于安全有很高要求的任务，比如你需要一个健壮的系统来防止入侵、破解、恶意破坏等等。 

项目由连串的依赖的各个部分组成。 

需要大规模的文件操作。 

需要多维数组的支持。 

需要数据结构的支持，比如链表或数等数据结构。 

需要产生或操作图形化界面 GUI。 

需要直接操作系统硬件。 

需要 I/O 或socket 接口。 

需要使用库或者遗留下来的老代码的接口。 

私人的、闭源的应用（shell 脚本把代码就放在文本文件中，全世界都能看到）。
# 变量

**注意，变量名和等号之间不能有空格，这可能和你熟悉的所有编程语言都不一样**。同时，变量名的命名须遵循如下规则： 

+ 首个字符必须为字母（a-z，A-Z）。 

+ 中间不能有空格，可以使用下划线（_）。 

+ 不能使用标点符号。 

+ 不能使用bash里的关键字（可用help命令查看保留关键字）
# shell 特殊变量
|变量|含义|
|----|----|
|$0|当前脚本的文件名|
|$n|传递给脚本或函数的参数。n 是一个数字，表示第几个参数。例如，第一个参数是$1，第二个参数是$2。|
|$#|传递给脚本或函数的参数个数。|
|$*|传递给脚本或函数的所有参数。|
|$@|传递给脚本或函数的所有参数。被双引号(” “)包含时，与 $* 稍有不同|
|$?|上个命令的退出状态，或函数的返回值。|
|$$|当前Shell进程ID。对于 Shell 脚本，就是这些脚本所在的进程ID。|

**$* 和 $@ 的区别**

$* 和 $@ 都表示传递给函数或脚本的所有参数，不被双引号(” “)包含时，都以”$1” “$2” … “$n” 的形式输出所有参数。

但是当它们被双引号(“”)包含时，”$*” 会将所有的参数作为一个整体，以”$1 $2 … $n”的形式输出所有参数；”$@” 会将各个参数分开，以”\$1” “\$2” … “\$n” 的形式输出所有参数。’

# shell运算

方法一：原生bash不支持简单的数学运算，但是可以通过其他命令来实现，例如 awk 和 expr，expr 最常用。
`var= `expr 2+2``
方法二使用双小括号

```java
var=$((2+2))
```

如果要学习逻辑关系，先学习测试类型

# 文件类型判断
|测试选项|作用|
|----|----|
|-b 文件|判断该文件是否存在，并且是否为块设备文件|
|-c 文件|判断文件是否存在，并且是否为字符设备文件|
|-d 文件|判断文件是否存在，并且是否为目录文件|
|-e 文件|判断文件是否存在|
|-f 文件|判断是否存在，并且是否为普通文件|
|-L 文件|判断文件是否存在，并且是否为符号链接文件|
|-p 文件|判断该文件是否传真，并且是否为管道文件|
|-s 文件|判断文件是否存在，并且是否非空|
|-S 文件|判断文件是否存在，并且是否为套接字文件|

两种判断格式

```bash
test -e 文件名
```
`[ -e 文件名 ]  //这种更适合shell脚本`
# 文件权限
|测试选项|作用|
|----|----|
|-r 文件|判断文件是否存在，并且是否该文件拥有读权限|
|-w 文件|判断文件是否存在，并且是否该文件拥有写权限|
|-x 文件|判断文件是否存在，并且是否该文件拥有执行权限|
|-u 文件|判断文件是否存在，并且是否文件拥有SUID权限|
|-g文件|判断文件是否存在，并且该文件是否拥有SGID权限|
|-h 文件|判断文件是否存在，并且该文件是否拥有SBit权限|

# 文件之间进行比较
|测试选项|作用|
|----|----|
|文件1 -nt 文件2|判断文件1修改时间是否比文件2的新|
|文件1 -ot 文件2|判断文件1的修改时间是否比文件2的旧|
|文件1 -ef 文件2|判断文件1是否和文件2的Inode号一致，可以理解为两个文件是否为同一个文件，这个判断用于判断硬链接是很好的办法|

# 两个整数的比较
|测试选项|作用|
|----|----|
|整数1 -eq 整数2|判断整数1是否和整数2相等|
|整数1 -ne 整数2|判断整数1是否和整数2不相等|
|整数1 -gt 整数2|判断整数1是否大于整数2|
|整数1 -lt 整数2|判断整数1是否小于整数2|
|整数1 -ge 整数2|判断整数1是否大于等于整数2|
|整数 -le 整数2|判断整数1是否小于等于整数2|

# 字符串的判断
|测试选项|作用|
|----|----|
|-z 字符串|判断字符串是否为空|
|-n 字符串|判断字符串是否为非空|
|字符串1===字符串2|判断字符串1是否和字符串2相等|
|字符串1 !=字符串2|判断字符串是否和字符串不相等|

# 多重条件判断
|测试条件|作用|
|----|----|
|判断1 -a 判断2|逻辑与，判断1和判断2都成立，最终结果才为真|
|判断1 -o 判断2|逻辑或，判断1和判断2有一个成立，最终结果为真|
|！判断|逻辑非，是原始判断式取反|

归纳完判断的条件，接下来主要是主要从判断流程，循环流程来讲解

# if语句

```bash
if [ 条件判断式 ];then
   程序
 fi
```

或者

```bash
if [ 条件判断式 ]
then
程序
fi
```

多分支判断

```bash
if [ 条件判断 ]
then
  程序  
  elif [ 条件判断2 ]
  then
  程序
  else 程序
  fi
```

# case 语句

```
case $变量名 in
    "值1"）
    如果变量的值等于1，执行程序
    ;;  //两个分号不能省略
    "值2"）
    如果变量的值等于2，执行程序
    ;;
    *) //其他情况
    ;;
esac
```

# for语句

语法一

```bash
for 变量 in 值1 值2 值3
do
程序
done
```

语法二

```
for((初始值;循环控制条件；变量变化))  //(()) 就可以进行数值运算
do
程序
done
```

例子

```bash
#!/bin/bash

s=0

for ((i=1;i<=100;i=i+1))
   do
   s=$(($i+$s))
   done
echo $s
```

# while循环与until循环

while是不定循环，也称条件循环，for是固定循环

```bash
while [ 条件判断式 ]
do
程序
done
```

例子“

```bash
i=1
s=0
while [ $i -le 100 ]
do
   s=$(($s+$i))
   i=$(($i+1))
 done

 echo $s
```

#### until循环

until循环和while循环相反，until循环时只要条件不成立才实行

```bash
until [ 条件判断式 ]
do
程序
done
```

例子：

```bash
i=1
s=0
until [ $i -gt 100 ]
do
   s=$(($s+$i))
   i=$(($i+1))
 done

 echo $s
```

# 福利-shell脚本自动发送邮件

```bash
#!/bin/bash
#采用html格式发送

from='nidvd105@163.com'

to='chenyoubing@stu.xmu.edu.cn'

email_date=''
localPath=$(pwd)

echo $localPath
email_content=$localPath"/email_content.html"

email_subject='rosetta_program_inform'

email_meassage=''
function write_email_content()
{
   if [ -d email_content.html ] 
   then
   echo $email_message > $email_content

   else 
   touch $email_content

   fi


   if [ -d 1ukf.m1.* ] #不同的程序是不一样的（判断结果文件是否存在）
   then
     email_message='<html><body><h1>congratulation</h1><p>the program have executed successfully and got some results.</p>'
   else
     email_message='<html><body>sorry<h1></h1><p>the program have run error or not get result.</p>'
   fi
     email_message=$email_message'<p></p><p></p><p></p><p>================================</p><p>send automatically,no reply</p></body></html>'
  # echo $email_message
   echo $email_message > $email_content
}  

function send_email()
{
      email_date=$(date)

      echo $email_date

      email_subject=$email_subject"_"$email_date


      write_email_content

      cat $email_content | formail -I "From:$from" -I "MIME-Version:1.0" -I "Content-type:text/html;charset=gb2312" -I "Subject:$email_subject" | /usr/sbin/sendmail -oi $to

}


send_email
```

注意：shell 的脚本中的单引号和双引号的差别，单引号会按照单引号的内容原样输出，包括变量$,双引号就会输出变量的值






