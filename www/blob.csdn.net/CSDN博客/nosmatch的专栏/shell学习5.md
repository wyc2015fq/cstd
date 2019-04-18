# shell学习5 - nosmatch的专栏 - CSDN博客
2011年08月26日 08:48:48[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：459标签：[shell																[terminal																[扩展																[终端																[作业																[kill](https://so.csdn.net/so/search/s.do?q=kill&t=blog)](https://so.csdn.net/so/search/s.do?q=作业&t=blog)](https://so.csdn.net/so/search/s.do?q=终端&t=blog)](https://so.csdn.net/so/search/s.do?q=扩展&t=blog)](https://so.csdn.net/so/search/s.do?q=terminal&t=blog)](https://so.csdn.net/so/search/s.do?q=shell&t=blog)
个人分类：[shell](https://blog.csdn.net/HDUTigerkin/article/category/874908)
1，计算指定文件中指定字符的个数的？
**cat file|tr -cd letters|wc -c**
   说明：tr -cd letters只要不匹配letters的字符都删除，其中-c选项是反转的意思，对不匹配的letters的字符进行操作，-d选项就是删除了。wc -c的作用是对字符计数。
- [root@PC shell]# cat 1 
- 1 2 3 
- 2 3 4 
- 3 4 5 
- 4 5 6 
- [root@PC shell]# cat 1 | tr -cd 3 | wc -c 
- 3 
- [root@PC shell]# cat 1 | tr -cd 1 | wc -c 
- 1 
- [root@PC shell]# cat 1 | tr -cd 4 | wc -c 
- 3 
   很好奇wc -c对中文是怎么处理的结果：
- [root@PC shell]# echo 中 | wc -c 
- 4 
- [root@PC shell]# echo -n 中 | wc -c # 事实说明是一个中文被表示成了3 
- 3 
- [root@PC shell]# echo 123 | wc -c 
- 4 
- [root@PC shell]# echo -n 123 | wc -c # 注意采用echo 结合 wc计数时，加上 -n 选项
- 3 
2，stty
   设置、复位和报告终端操作参数。
   好吧，我得说这真是个很难记的命令，熟练使用就暂时别想了。
   首先呢，先说个有趣的用法，隐形：
- [root@PC shell]# stty -echo # 关闭回显功能，就隐形了，你输入的命令将不会显示，但是结果还是会输出 
- [root@PC shell]# 1  2  3  4  5  6  7  8  a  c  scripts  xx 
- [root@PC shell]# Linux PC 2.6.18-194.32.1.el5 #1 SMP Wed Jan 5 17:53:09 EST 2011 i686 i686 i386 GNU/Linux  
- [root@PC shell]# 你知道我输入了什么吗 
- [root@PC shell]# [root@PC shell]#   #在这开启了echo，stty echo 
- [root@PC shell]# ls 
- 1  2  3  4  5  6  7  8  a  c  scripts  xx 
- [root@PC shell]# uname -a 
- Linux PC 2.6.18-194.32.1.el5 #1 SMP Wed Jan 5 17:53:09 EST 2011 i686 i686 i386 GNU/Linux 
- [root@PC shell]# echo "你知道我输入了什么吗" 
- 你知道我输入了什么吗 
   当然还有其他很重要的使用，对终端有很灵活的控制。
- [root@PC shell]# stty -a  # 打印出所有的设置 
- speed 38400 baud; rows 27; columns 91; line = 0; 
- intr = ^C; quit = ^\; erase = ^?; kill = ^U; eof = ^D; eol = <undef>; eol2 = <undef>; 
- swtch = <undef>; start = ^Q; stop = ^S; susp = ^Z; rprnt = ^R; werase = ^W; lnext = ^V; 
- flush = ^O; min = 1; time = 0; 
- -parenb -parodd cs8 -hupcl -cstopb cread -clocal -crtscts -cdtrdsr 
- -ignbrk -brkint -ignpar -parmrk -inpck -istrip -inlcr -igncr icrnl ixon -ixoff -iuclc -ixany 
- -imaxbel -iutf8 
- opost -olcuc -ocrnl onlcr -onocr -onlret -ofill -ofdel nl0 cr0 tab0 bs0 vt0 ff0 
- isig icanon iexten echo echoe echok -echonl -noflsh -xcase -tostop -echoprt echoctl echoke 
- [root@PC shell]# 
   其中：
- 
eof : 输入结束
- 
erase : 向后删除字符，
- 
intr : 中断当前程序
- 
kill : 删除整条命令
- 
quit :退出当前程序
- 
start : 启动屏幕输出
- 
stop :停止屏幕输出；
- 
susp : terminal stop当前程序。
  更加具体的使用请参看：[http://study.chyangwa.com/IT/AIX/aixcmds5/stty.htm](http://study.chyangwa.com/IT/AIX/aixcmds5/stty.htm)
3，pidof
   获取一个正在运行作业的进程ID(PID)。
- [root@PC shell]# pidof vsftpd 
- 1368 
- [root@PC shell]# pidof httpd 
- 8326 8286 8091 8062 7755 7566 7418 7249 7102 2129 2101 
4，fuser
   fuser 命令用指定的文件或者文件系统显示进程进程号，默认情况下每一个文件名后会跟着一个字母来表示类型。 
   必要参数
-l 显示信号
-4 IPv4
-6 IPv6
-a 与程序相关的所有文件
-k 删除程序相关的所有文件
-I 互交模式
-s 不显示处理信息
-u PID显示用户名
-v 运行时，显示相信处理信息
- 清零
-m 挂载文件系统
-signal 发送信号
-n 指定关键字进行搜索,如 文件、UDP、TCP
-V 显示版本显示
   列出进程号后面都跟随一个字母，该字母指示进程如何使用文件。
   c：当前目录
   e：扩展
   f：打开的文件
   r：根目录
   m：共享库
- [root@PC lib]# fuser /home/max # 看下啥进程在用/home/max目录 
- /home/max:           14606c 
- [root@PC lib]# ps aux | grep 14606 | grep -v grep # 查找下 
- max      14606  0.6  0.1   5008  1468 pts/1    Ss+  01:21   0:00 -bash 
- # 事实上，是我另一个putty窗口中登录了max用户，正如上面所示 
   还有些巧用，只是技巧太多需要，多练习：
- [root@PC lib]# fuser -un tcp 21 # 众所周知21为ftp所用 
- 21/tcp:               1368(root) 
- [root@PC lib]# ps aux | grep 1368 | grep -v grep # 果然 vsftpd 
- root      1368  0.0  0.0   5296   528 ?        Ss   Mar12   0:00 /usr/sbin/vsftpd /etc/vsftpd/vsftpd.conf 
5，[[ ]]、[ ]、(( ))
   当然，这几个东西真的有点让人糊涂。
   使用[[ ... ]]条件判断结构, 而不是[ ... ], 能够防止脚本中的许多逻辑错误. 比如, &&, ||, <,和>操作符能够正常存在于[[ ]]条件判断结构中, 但是如果出现在[ ]结构中的话, 会报错。(( ))结构扩展并计算一个算术表达式的值. 如果表达式的结果为0, 那么返回的退出状态码为1, 或者是"假"。 而一个非零值的表达式所返回的退出状态码将为0, 或者是"true"。
   下面我们针对上面几句话做些实验,实验之前先说个东西，在shell中下边这个结构：dosomething&&thendo 的含义是只有dosomething执行成功后才执行thendo，而dosomething||thendo 则是dosomething失败后再执行thendo。
- [root@PC lib]# [[ 1&&2 ]] 
- [root@PC lib]# [ 1&&2 ] # 不能执行 
- -bash: [: missing `]' 
- [root@PC lib]# a=2+3 # 这样是不能计算的 
- [root@PC lib]# echo $a 
- 2+3 
- [root@PC lib]# ((a=2+3)) # 正确计算 
- [root@PC lib]# echo $a 
- 5 
- [root@PC lib]# (a=2+3) # 单个括号也能进行计算 
- [root@PC lib]# echo $a 
- 5 
- [root@PC lib]# (a++) # 但是却不能用这种方式。。。。 
- -bash: a++: command not found 
- [root@PC lib]# ((a++)) # 这时候(( ))的威力出来了。
- [root@PC li 
    好了，这两天的学习成果就化为两篇博客了。静下心来多练习练习吧。。。
