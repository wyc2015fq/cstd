# linux expect spawn的用法 - xqhrs232的专栏 - CSDN博客
2017年06月23日 17:03:53[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：451
原文地址::[http://www.cnblogs.com/jason2013/articles/4356352.html](http://www.cnblogs.com/jason2013/articles/4356352.html)
相关文章
1、[expect spawn、linux expect 用法小记](http://blog.csdn.net/hshl1214/article/details/7408223)----[http://blog.csdn.net/hshl1214/article/details/7408223/](http://blog.csdn.net/hshl1214/article/details/7408223/)
首先，要安装expect,linux expect的安装
1.安装相应的包
    yum install -y tcl tclx tcl-devel
2.下载expect-5.43.tar.gz包（我这里用的这个包，大家也可以用别的）
 根据参数，运行./configure
 ./configure --with-tcl=/usr/lib --with-tclinclude=/usr/include/tcl-private/generic
3.make && make install  安装完毕
简单的登陆别的机器执行命令，可行 （更详细的还得参考expect的有关说明）
expect.sh
#!/usr/local/bin/expect
set timeout 10
spawn ssh [root@ip](mailto:root@ip)
expect "*password*"
send "123456/r"
expect "#"
send "service crond restart/r"
expect eof
注：expect的功能是很强大的，实现了无须人与系统交互的功能，已经成为SA的一个强大助手，要研究的彻底，还是需要大量的时间的。

一、概述
　　我们通过Shell可以实现简单的控制流功能，如：循环、判断等。但是对于需要交互的场合则必须通过人工来干预，有时候我们可能会需要实现和交互程序如telnet[服务器](http://server.chinaitlab.com/)等进行交互的功能。而Expect就使用来实现这种功能的工具。
　　Expect是一个免费的编程工具语言，用来实现自动和交互式任务进行通信，而无需人的干预。Expect的作者Don Libes在1990年开始编写Expect时对Expect做有如下定义：Expect是一个用来实现自动交互功能的软件套件 (Expect [is a] software suite for automating interactive tools)。使用它系统管理员 的可以创建脚本用来实现对命令或程序提供输入，而这些命令和程序是期望从终端（terminal）得到输入，一般来说这些输入都需要手工输入进行的。 Expect则可以根据程序的提示模拟标准输入提供给程序需要的输入来实现交互程序执行。甚至可以实现实现简单的BBS聊天机器人。:)
　　Expect是不断发展的，随着时间的流逝，其功能越来越强大，已经成为系统管理员的的一个强大助手。Expect需要Tcl编程语言的支持，要在系统上运行Expect必须首先安装Tcl。
　　二、Expect工作原理
　　从最简单的层次来说，Expect的工作方式象一个通用化的Chat脚本工具。Chat脚本最早用于UUCP网络内，以用来实现计算机之间需要建立连接时进行特定的登录会话的自动化。
　　Chat脚本由一系列expect-send对组成：expect等待输出中输出特定的字符，通常是一个提示符，然后发送特定的响应。例如下面的 Chat脚本实现等待标准输出出现Login:字符串，然后发送somebody作为用户名；然后等待Password:提示符，并发出响应 sillyme。
　　引用：Login: somebody Password: sillyme
　　这个脚本用来实现一个登录过程，并用特定的用户名和密码实现登录。
　　Expect最简单的脚本操作模式本质上和Chat脚本工作模式是一样的。
　　例子：
　　1、实现功能
　　下面我们分析一个响应chsh命令的脚本。我们首先回顾一下这个交互命令的格式。假设我们要为用户chavez改变登录脚本，要求实现的命令交互过程如下：
　　引用：# chsh chavez
　　Changing the login shell for chavez
　　Enter the new value, or press return for the default
　　Login Shell [/bin/bash]: /bin/tcsh
　　#
　　可以看到该命令首先输出若干行提示信息并且提示输入用户新的登录shell。我们必须在提示信息后面输入用户的登录shell或者直接回车不修改登录shell。
　　2、下面是一个能用来实现自动执行该命令的Expect脚本：
　　#!/usr/bin/expect
　　# Change a login shell to tcsh
　　set user [lindex $argv 0]
　　spawn chsh $user
　　expect "]:"
　　send "/bin/tcsh "
　　expect eof
　　exit
　　这个简单的脚本可以解释很多Expect程序的特性。和其他脚本一样首行指定用来执行该脚本的命令程序，这里是/usr/bin/expect。程序第一行用来获得脚本的执行参数(其保存在数组$argv中，从0号开始是参数)，并将其保存到变量user中。
　　第二个参数使用Expect的spawn命令来启动脚本和命令的会话，这里启动的是chsh命令，实际上命令是以衍生子进程的方式来运行的。
　　随后的expect和send命令用来实现交互过程。脚本首先等待输出中出现]:字符串，一旦在输出中出现chsh输出到的特征字符串(一般特征 字符串往往是等待输入的最后的提示符的特征信息)。对于其他不匹配的信息则会完全忽略。当脚本得到特征字符串时，expect将发送/bin/tcsh和 一个回车符给chsh命令。最后脚本等待命令退出(chsh结束)，一旦接收到标识子进程已经结束的eof字符，expect脚本也就退出结束。
　　3、决定如何响应
　　管理员往往有这样的需求，希望根据当前的具体情况来以不同的方式对一个命令进行响应。我们可以通过后面的例子看到expect可以实现非常复杂的条件响应，而仅仅通过简单的修改预处理脚本就可以实现。下面的例子是一个更复杂的expect-send例子：
　　expect -re "/[(.*)]:"
　　if {$expect_out(1,string)!="/bin/tcsh"} {
　　send "/bin/tcsh" }
　　send " "
　　expect eof
　　在这个例子中，第一个expect命令现在使用了-re参数，这个参数表示指定的的字符串是一个正则表达式，而不是一个普通的字符串。对于上面这个例子里是查找一个左方括号字符(其必须进行三次逃逸(escape)，因此有三个符号，因为它对于expect和正则表达时来说都是特殊字符)后面跟有 零个或多个字符，最后是一个右方括号字符。这里.*表示表示一个或多个任意字符，将其存放在()中是因为将匹配结果存放在一个变量中以实现随后的对匹配结果的访问。
　　当发现一个匹配则检查包含在[]中的字符串，查看是否为/bin/tcsh。如果不是则发送/bin/tcsh给chsh命令作为输入，如果是则仅仅发送一个回车符。这个简单的针对具体情况发出不同相响应的小例子说明了expect的强大功能。
　　在一个正则表达时中，可以在()中包含若干个部分并通过expect_out数组访问它们。各个部分在表达式中从左到右进行编码，从1开始(0包含有整个匹配输出)。()可能会出现嵌套情况，这这种情况下编码从最内层到最外层来进行的。
　　4、使用超时
　　下一个expect例子中将阐述具有超时功能的提示符函数。这个脚本提示用户输入，如果在给定的时间内没有输入，则会超时并返回一个默认的响应。这个脚本接收三个参数：提示符字串，默认响应和超时时间(秒)。
　　#!/usr/bin/expect
　　# Prompt function with timeout and default.
　　set prompt [lindex $argv 0]
　　set def [lindex $argv 1]
　　set response $def
　　set tout [lindex $argv 2]
　　脚本的第一部分首先是得到运行参数并将其保存到内部变量中。
　　send_tty "$prompt: "
　　set timeout $tout
　　expect " " {
　　set raw $expect_out(buffer)
　　# remove final carriage return
　　set response [string trimright "$raw" " "]
　　}
　　if {"$response" == "} {set response $def}
　　send "$response
　　这是脚本其余的内容。可以看到send_tty命令用来实现在终端上显示提示符字串和一个冒号及空格。set timeout命令设置后面所有的expect命令的等待响应的超时时间为$tout(-l参数用来关闭任何超时设置)。
然后expect命令就等待输出中出现回车字符。如果在超时之前得到回车符，那么set命令就会将用户输入的内容赋值给变脸raw。随后的命令将用户输入内容最后的回车符号去除以后赋值给变量response。
　　然后，如果response中内容为空则将response值置为默认值(如果用户在超时以后没有输入或者用户仅仅输入了回车符)。最后send命令将response变量的值加上回车符发送给标准输出。
　　一个有趣的事情是该脚本没有使用spawn命令。 该expect脚本会与任何调用该脚本的进程交互。
　　如果该脚本名为prompt，那么它可以用在任何C风格的shell中。
　　% set a='prompt "Enter an answer" silence 10'
　　Enter an answer: test
　　% echo Answer was "$a"
　　Answer was test
　　prompt设定的超时为10秒。如果超时或者用户仅仅输入了回车符号，echo命令将输出
　　Answer was "silence"
　　5、一个更复杂的例子
　　下面我们将讨论一个更加复杂的expect脚本例子，这个脚本使用了一些更复杂的控制结构和很多复杂的交互过程。这个例子用来实现发送write命令给任意的用户，发送的消息来自于一个文件或者来自于键盘输入。
　　#!/usr/bin/expect
　　# Write to multiple users from a prepared file
　　# or a message input interactively
　　if {$argc<2} {
　　send_user "usage: $argv0 file user1 user2 ... "
　　exit
　　}
　　send_user命令用来显示使用帮助信息到父进程(一般为用户的shell)的标准输出。
　　set nofile 0
　　# get filename via the Tcl lindex function
　　set file [lindex $argv 0]
　　if {$file=="i"} {
　　set nofile 1
　　} else {
　　# make sure message file exists
　　if {[file isfile $file]!=1} {
　　send_user "$argv0: file $file not found. "
　　exit }}
　　这部分实现处理脚本启动参数，其必须是一个储存要发送的消息的文件名或表示使用交互输入得到发送消的内容的"i"命令。
　　变量file被设置为脚本的第一个参数的值，是通过一个Tcl函数lindex来实现的，该函数从列表/数组得到一个特定的元素。[]用来实现将函数lindex的返回值作为set命令的参数。
　　如果脚本的第一个参数是小写的"i"，那么变量nofile被设置为1，否则通过调用Tcl的函数isfile来验证参数指定的文件存在，如果不存在就报错退出。
　　可以看到这里使用了if命令来实现逻辑判断功能。该命令后面直接跟判断条件，并且执行在判断条件后的{}内的命令。if条件为false时则运行else后的程序块。
　　set procs {}
　　# start write processes
　　for {set i 1} {$i<$argc}
　　{incr i} {
　　spawn -noecho write
　　[lindex $argv $i]
　　lappend procs $spawn_id
　　}
　　最后一部分使用spawn命令来启动write进程实现向用户发送消息。这里使用了for命令来实现循环控制功能，循环变量首先设置为1，然后因 此递增。循环体是最后的{}的内容。这里我们是用脚本的第二个和随后的参数来spawn一个write命令，并将每个参数作为发送消息的用户名。 lappend命令使用保存每个spawn的进程的进程ID号的内部变量$spawn_id在变量procs中构造了一个进程ID号列表。
　　if {$nofile==0} {
　　setmesg [open "$file" "r"]
　　} else {
　　send_user "enter message,
　　ending with ^D: " }
　　最后脚本根据变量nofile的值实现打开消息文件或者提示用户输入要发送的消息。
　　set timeout -1
　　while 1 {
　　if {$nofile==0} {
　　if {[gets $mesg chars] == -1} break
　　set line "$chars "
　　} else {
　　expect_user {
　　-re " " {}
　　eof break }
　　set line $expect_out(buffer) }
　　foreach spawn_id $procs {
　　send $line }
　　sleep 1}
　　exit
　　上面这段代码说明了实际的消息文本是如何通过无限循环while被发送的。while循环中的 if判断消息是如何得到的。在非交互模式下，下一行内容从消息文件中读出，当文件内容结束时while循环也就结束了。(break命令实现终止循环) 。
　　在交互模式下，expect_user命令从用户接收消息，当用户输入ctrl+D时结束输入，循环同时结束。两种情况下变量$line都被用来保存下一行消息内容。当是消息文件时，回车会被附加到消息的尾部。
　　foreach循环遍历spawn的所有进程，这些进程的ID号都保存在列表变量$procs中，实现分别和各个进程通信。send命令组成了 foreach的循环体，发送一行消息到当前的write进程。while循环的最后是一个sleep命令，主要是用于处理非交互模式情况下，以确保消息 不会太快的发送给各个write进程。当while循环退出时，expect脚本结束

