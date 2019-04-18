# shell学习2 - nosmatch的专栏 - CSDN博客
2011年08月26日 08:47:05[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：367标签：[shell																[脚本																[cmd																[function																[file																[kill](https://so.csdn.net/so/search/s.do?q=kill&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=cmd&t=blog)](https://so.csdn.net/so/search/s.do?q=脚本&t=blog)](https://so.csdn.net/so/search/s.do?q=shell&t=blog)
个人分类：[shell](https://blog.csdn.net/HDUTigerkin/article/category/874908)
                
 1. 首先介绍一个命令：fuser。之前很少用到，现在做个标记吧。
fuser命令在linux系统中用来根据文件或文件结构识别进程。
使用权限：
超级用户
语法格式：
fuser [ -c | -d | -f ] [ -k | -K { SignalNumber | SignalName }] [ -u ] [ -x ] [ -V ]File ...
使用说明：
此 fuser 命令列出了本地进程的进程号，那些本地进程使用 File 参数指定的本地或远程文件。
对于阻塞特别设备，此命令列出了使用该设备上任何文件的进程。
每个进程号后面都跟随一个字母，该字母指示进程如何使用文件。
c 将此文件作为当前目录使用。 
e 将此文件作为程序的可执行对象使用。 
r 将此文件作为根目录使用。 
s 将此文件作为共享库（或其他可装载对象）使用。 
进程号被写入标准输出（在进程号之间有空格的行中）。
一个换行符被写入标准错误（在每个文件操作数的最后一个输出之后）。其他所有输出被写入标准错误。
此 fuser 命令不会检测有 mmap 区域的进程，其中相关的文件描述符已从此被关闭。
主要参数：
-c 包含 File 的文件系统中关于任何打开的文件的报告。 
-d 暗示使用了 -c 和 -x 标志。
关于任何与文件系统（自父目录删除的）无链接的打开文件的报告。
当与 -V 标志一起使用时，它也会报告被删除文件的节点号和大小。 
-f 仅对文件的打开实例报告。 
-K SignalNumber | SignalName 将指定信号发送到每个本地进程。
仅有 root 用户能终止另一用户的进程。信号可以指定为信号名称（如 -9）或 KILL（用于 SIGKILL 信号）。
SignalName 的有效值是 kill -l 命令所显示的那些值。 
-k 将 SIGKILL 信号发送到每个本地进程。仅有 root 用户能终止另一用户的进程。 
注:
fuser -k 或 -K 可能无法检测和杀死程序开始运行后立即创建的新进程。 
-u 为进程号后圆括号中的本地进程提供登录名。 
-V 提供详细输出。 
-x 与 -c 或 -f 连用，报告除标准 fuser 输出以外的可执行的和可载入的对象。 
应用实例：
要列出使用 /etc/passwd 文件的本地进程的进程号，请输入： 
fuser /etc/passwd
要列出使用 /etc/filesystems 文件的进程的进程号和用户登录名，请输入： 
fuser -u /etc/filesystems
要列出正在使用已从给定文件系统删除的文件的全部进程，请输入： 
fuser -d /usr
    2. set的使用
    在脚本中set可以用来设置位置参数，如下例：
- #!/bin/bash 
- 
- # script "set-test" 
- 
- # 使用3个命令行参数来调用这个脚本, 
- # 比如, "./set-test one two three". 
- 
- echo 
- echo "Positional parameters before  set \`uname -a\` :" 
- echo "Command-line argument #1 = $1" 
- echo "Command-line argument #2 = $2" 
- echo "Command-line argument #3 = $3" 
- 
- 
- set `uname -a` # 把`uname -a`的命令输出设置 
-                # 为新的位置参数. 
- 
- echo $_        # unknown(译者注: 这要看你的uname -a输出了,这句打印出的就是输出的最后一个单词.) 
- # 在脚本中设置标志. 
- 
- echo "Positional parameters after  set \`uname -a\` :" 
- # $1, $2, $3, 等等. 这些位置参数将被重新初始化为`uname -a`的结果 
- echo "Field #1 of 'uname -a' = $1" 
- echo "Field #2 of 'uname -a' = $2" 
- echo "Field #3 of 'uname -a' = $3" 
- echo --- 
- echo $_        # --- 
- echo 
- 
- exit 0 
    不使用任何选项或参数来调用set命令的话, 将会列出所有的环境变量和其他所有的已经初始化过的变量
    set -- $variable  #将位置参数的内容设为变量"$variable"的内容，set --
如果没指定变量,那么将会unset所有的位置参数：
- #!/bin/bash 
- 
- variable="one two three four five"
- 
- set -- $variable 
- # 将位置参数的内容设为变量"$variable"的内容. 
- 
- first_param=$1 
- second_param=$2 
- shift; shift        # 将最前面的两个位置参数移除. 
- remaining_params="$*"
- 
- echo 
- echo "first parameter = $first_param"             # one 
- echo "second parameter = $second_param"           # two 
- echo "remaining parameters = $remaining_params"   # three four five 
- 
- echo; echo 
- 
- # 再来一次. 
- set -- $variable 
- first_param=$1 
- second_param=$2 
- echo "first parameter = $first_param"             # one 
- echo "second parameter = $second_param"           # two 
- 
- # ====================================================== 
- 
- set -- 
- # 如果没指定变量,那么将会unset所有的位置参数. 
- 
- first_param=$1 
- second_param=$2 
- echo "first parameter = $first_param"             # (null value) 
- echo "second parameter = $second_param"           # (null value) 
- 
- exit 0 
    3.常用的命令shell命令
**declare**, **typeset：** 用来指定或限制变量的属性.
**readonly：**与declare -r作用相同, 设置变量的只读属性, 或者可以认为这个变量就是一个常量. 设置了这种属性之后, 如果你还要修改它, 那么将会得到一个错误信息. 这种情况与*C*语言中的**const**常量类型是相同的.
**getopts：理解起来有点难，有时间好好研究下。**
**source：**当在命令行中调用的时候, 这个命令将会执行一个脚本. 当在脚本中调用的时候,
source file-name 将会加载file-name文件. sourc一个文件(或点命令)将会在脚本中*引入*代码, 并将这些代码附加到脚本中(与*C*语言中的#include指令效果相同). 最终的结果就像是在使用"source"的行上插入了相应文件的内容.
 在多个脚本需要引用相同的数据, 或者需要使用函数库的情况下, 这个命令非常有用.
**exec：**这个shell内建命令将使用一个特定的命令来取代当前进程. 
一般的当shell遇到一个命令, 它会forks off一个子进程来真正的运行命令. 使用**exec**内建命令, shell就不会fork了, 并且命令的执行将会替换掉当前shell.
 因此, 在脚本中使用时, 一旦**exec**所执行的命令执行完毕, 那么它就会强制退出脚本。
****caller：**将caller**命令放到函数中, 将会在stdout上打印出函数的*调用者*信息。
- #!/bin/bash 
- #caller 0 的作用 
- 
- function1 () 
- { 
- caller 0 #将显示调用者信息。 
- 
- } 
- function1 
- 
- caller 0 
****type [cmd]：****与外部命令which很相像,
**type cmd**将会给出"cmd"的完整路径. 与**which**命令不同的是,
**type**命令是Bash内建命令. `-a`是**type**命令的一个非常有用的选项, 它用来鉴别参数是*关键字*还是*内建命令*, 也可以用来定位同名的系统命令. 
- [max@localhost shell]$ type cd 
- cd is a shell builtin 
- [max@localhost shell]$ which cd 
- /usr/bin/which: no cd in (/usr/kerberos/bin:/usr/local/bin:/bin:/usr/bin:/home/max/bin) 
实验结果好像是type cmd 将显示cmd的类型，which cmd将显示cmd的路径？
本文出自 “[alkshao](http://alkshao.blog.51cto.com)” 博客，请务必保留此出处[http://alkshao.blog.51cto.com/1953382/508613](http://alkshao.blog.51cto.com/1953382/508613)
