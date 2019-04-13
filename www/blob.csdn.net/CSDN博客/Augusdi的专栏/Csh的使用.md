
# Csh的使用 - Augusdi的专栏 - CSDN博客


2016年10月14日 17:38:55[Augusdi](https://me.csdn.net/Augusdi)阅读数：4749


﻿﻿
在*unix系统中，常用的shell有sh，bash，csh/tcsh, ksh.
sh来自于systemV的Unix，是传统的Unix的shell，直到现在很多的系统管理员仍然喜欢使用sh。
bash来自于BSD Unix，语法非常类似于C语言，所以通常有C/C++编程背景的开发人员最喜欢使用。
ksh是对sh的扩展，且吸收了csh的一些有用的功能，但是由于开始ksh的license是AT&T，所以后来出现了很多的ksh的开源版本，例如mksh，pdksh等。
bash是现在很多Linux的发行版中默认的shell，综合了其他shell的很多优点。

下面描述csh的一些基本的语法(在sh环境中执行csh切换到csh环境)：

1） 变量
通过set来定义局部变量x,通过$x或${x}来使用变量x的值，$%x表示变量的值的长度, $?x来判断变量x是否设置，如设置则为1，否则为0。
set x = 5
echo $x
echo ${x}kg
echo $%x
全局变量的定义setenv v  value 该变量将被此shell派生的所有子shell继承。

$$
表示当前进程的PID， $status 或 $? 表示退出状态。

2）数组
定义数组myarr, 通过$myarr[index]来访问数组中的值，注意index是从1开始的。通过$myarr或$myarr[*]来访问数组所有的元素。通过$\#myarr来查看元素的个数。
set myarr = (str1, str2,str3)
echo $myarr[2]
echo $myarr
echo $myarr[*]

3）命令替换
通过set x = `cmd`来执行命令，且结果赋值给变量。
set d = `date`
echo $d
echo $d[6]-$d[2]-$d[3]

4)命令行参数
通过$argv[1],$argv[2]或$1,$2来访问命令行参数。命令行参数的个数为$\#argv。

5）文件名扩展的元字符
只能使用?,*,[abc],[a-c]。

6）IO重定向和管道
将命令的输出重定向到文件为>。
将命令的输出重定向并追加到文件为>>。
将命令的输入重定向到文件为<。
将命令的报错信息重定向到一个文件(cmd>/dev/tty)>&errors。
将命令的输出和错误输出分别重定向(cmd > goodstuff)  >& badstuff。
将命令的输出和报错信息重定向到一个文件cmd>&file。
将命令的输出经管道发往另一个命令cmd|cmd.
将命令的输出和报错信息经管道发往另一个命令cmd|&cmd。
条件语句为 cmd && cmd 或 cmd || cmd。
command<<WORD 表示将command的输入重定向为从第一个WORD处开始，到下一个WORD处之间的内容（即here文档）。

7）从键盘读取并保存到变量中
set var = $<

8)算术
@ var = 5 + 5
echo $var
@ v2 = $var + 3
echo $v2

9)代字符号扩展
~username 表示username的home目录。

10)别名
alias m more 为more创建别名m。
alias 列出所有的alias。
unalias m 用来删除more的alias定义。

11)初始化文件
.login 在登录时执行的文件。
.cshrc 在每次调用shell时都执行的文件。

12) label 和 goto
csh中没有函数的概念，使用类似windows批处理中的label和goto。
goto label
......
label:
....

13) if/else/switch/case
if(expression)then
commands
endif
if {(command)} then
commands
endif

if(expression) then
commands
else if(expression) then
commands
else
commands
endif

switch("$value")
case pattern1:
commands
breaksw
case pattern2:
commands
breaksw
default:
commands
breaksw
endsw

14 while/foreach
while(expression)
commands
continue
break
end
foreach var (wordlist)
commands
end

15 repeat
repeat表示重复执行后面的命令。
repeat 3 "echo helloworld"
16 csh中设置环境变量PATH的方法
csh中使用path代替PATH，设置类似于数组的使用。
set path = ($path /home)
echo $path
echo $PATH
17 source等价于其他shell中的.
source使得程序在当前的shell中被执行，而不是派生子进程来执行。
18转义字符与单双引号
引号必须成对出现，而且必须在同一行上配对。可以用反斜杠来转义换行符，这样就能在下一行配对了。
单引号可用于保护双引号，双引号也可以用来保护单引号。
单引号保护除历史字符(！)之外的所有元字符不被解释。
双引号保护除历史字符(！)，变量替换字符($)和反引号(用于命令替换)之外的所有元字符，使其不被解释。
19 历史command
history用来查看command执行的历史。
!!用来执行上一条命令。
20 pushd和popd用来维护目录栈
21 csh -vx用来显示输入的原样和变量替换后的脚本，用来帮助调试。
22在脚本中处理中断
onintr finish
<script continues here>
finish:
onintr - \# Disable further interrupts
echo Cleaning temp files
exit 1
onintr 命令后跟一个标号名，finish是用户自定义的标号。如果发生中断，控制将被转移到finish标号。通常该行位于脚本的开头。除非当程序正在执行时按ctrl+C（中断键），此时控制将被转移到该标号。onintr - 表示屏蔽所有的中断，此时按下ctrl+C将会被忽略。
23 noclobber 禁止覆盖变量，设定 $noclobber 预设变量改变输出重定向特性.
变量设定语法 set noclobber
取消变量设定语法 unset noclobber
这个 noclobber 变量，它的功能便是停止重定向符号“>”的覆盖（overwiting）已存在文件以及符号“>>”要将字符写入一个不存在的文件时，自动产生该文件的特性。
仅用两个例子让读者明白，设定后的实际使用状况。
例子一：
% ps axu > testfile
% set noclobber
% echo "test set noclobber" > testfile
testfile: File exists.
% echo "test set noclobber" >! testfile
%

例子二：
% set noclobber
% cat /etc/passwd >> nopass
nopass: No such file or directory
% cat /etc/passwd >>! nopass
%

完！
csh脚本语法实例
```python
#!/bin/csh -vx
#csh -vx show the command before running to help debug
#just to check syntax
#csh -n $0
#argv
if ($#argv < 2) then
    echo "Sorry, but you entered too few parameters"
    echo "usage:  $0 arg1 arg2
    exit
endif
set arg1 = $1
set arg2 = #2
foreach i ($*)
   echo $i
end
   
#execute commands
echo "Hello there `whoami`.  How are you today?"
echo "You are currently using `hostname` and the time is `date`"
echo "Your directory is `pwd`"
whoami
hostname
date
pwd
#var
set name = Mark
echo $name
set name = "Mark Meyer" # if the string has space, must use ""
echo $name
# it means set to NULL
set name = 
unset name
# get user input
set x = $<  
set current_user = `whoami`
#buildin vars
echo $user      # who am I?
echo $status    # a numeric variable, usually used to retun error codes
#Arithmetic variables
@ i = 2
@ k = ($x - 2) * 4
@ k = $k + 1
@ i--
@ i++
#array
set name = (mark sally kathy tony)
echo $#name    # num of the array
echo $name[1]
echo $name[4]
echo $name[2-3]
echo $name[2-]        # all elements from 2 to the end
echo $name[1-3]
echo $name[$i]
set name = ($name doran)
set name = (doran $name)
set name = ($name[1-2] alfie $name[3-])
shift name  # get rid of the frist element of the array
shift #if no argument is given, it will get rid of argv
#Expressions and operators
==        equal     (either strings or numbers)
!=        not equal     (either strings or numbers)
=~        string match
!~        string mismatch
<=        numerical less than or equal to
>=        numerical greater than or equal to
>         numerical greater than
<         numerical less than
-e file           file merely exists (may be protected from user)
-r file           file exists and is readable by user
-w file           file is writable by user
-x file           file is executable by user
-o file           file is owned by user
-z file           file has size 0
-f file           file is an ordinary file
-d file           file is a directory
!   -- negate                 
&&  -- logical and
||  -- logical or
#if-else
# run cmd as if expression
if ({grep -s junk $1}) then  
   echo "We found junk in file $1"
endif
# check if the var is defined
if ($?dirname) then 
    ls $dirname
endif
if (-e somefile) then
 grep $1 somefile
else
 echo "Grievous error!  Database file does not exist".
endif
#foreach
foreach i (*)
    if (-f $i) then
        echo "============= $i ==================="
        head $i
    endif
    if (-d $i) then
        (cd $i; headers)
    endif
end
#while
while ($#argv > 0)
    grep $something $argv[1]
end
@ n = 5
while ($n) 
     # do something
     @ n--
end
#switch-case
switch ($argv[$i])
 case quit:
        break        # leave the switch statement
 case list:
        ls
        breaksw
 case delete:
 case erase:
        @ k = $i + 1
        rm $argv[$k]
        breaksw
endsw
    
#here document
grep $i <<HERE
John Doe   101 Surrey Lane    London, UK    5E7 J2K
Angela Langsbury   99 Knightsbridge, Apt. K4     Liverpool
John Major  10 Downing Street  London
HERE
cat > tempdata <<ENDOFDATA
53.3 94.3 67.1
48.3 01.3 99.9
42.1 48.6 92.8
ENDOFDATA
exit 0
```


