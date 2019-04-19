# 使用pscp命令Linux文件上传与下载 - 三少GG - CSDN博客
2011年11月28日 13:45:35[三少GG](https://me.csdn.net/scut1135)阅读数：1507
**以下是代码片段：**
pscp D:\java\apache-tomcat-5.5.27\webapps\szfdc.rardev@192.168.68.249:/home/dev
　　3.输入密码 ok 文件已经上传到目标机器的/home/dev目录下了
　　(二)下载
　　1、开始→运行→cmd进入到dos模式输入以下命令
　　pscp dev@192.168.68.248:/home/dev/gren.sql d:\gren.sql;输入密码。ok!
　　其中：dev为linux的用户名，192.168.68.248为远程Linux主机ip地址，/home/dev/gren.sql为linux下的文件，d:\gren.sql为保存在本地的文件
　　其他：
**　pscp是putty安装包所带的远程文件传输工具，使用和Linux下scp命令相似**，
具体的使用方法如下：
　　PSCP和SCP功能相同，在windows下使用，只有一个文件，建议将pscp.exe放到C:\WINDOWS\system32下面，这样就可以在任何地方调用了。语法与scp相同，下面是几个有用的options。
　　-p 拷贝文件的时候保留源文件建立的时间。
　　-q 执行文件拷贝时，不显示任何提示消息。
　　-r 拷贝整个目录
　　-v 拷贝文件时，显示提示信息。
　　Usage:
　　pscp [options] [user@]host:source target
　　pscp [options] source [source...] [user@]host
　　pscp [options] -ls user@host:filespec
　　用法：
**pscp [选项] [用户名@]主机：源文件 目标文件　　pscp [选项] 源文件 [源文件……] [用户名@]主机**
　　pscp [选项] -ls 用户名@主机：文件空间？
　　Options:
　　选项：
　　-p preserve file attributes 保护文件属性
　　-q quiet, don't show statistics 安静，不显示统计
　　-r copy directories recursively 复制子文件夹
　　-V print version information and exit 显示信息
　　-v show verbose messages
　　-load sessname Load settings from saved session
　　-load 加载，加载保存节的设定
　　-P port connect to specified port
　　-P 端口，连接到指定空间端口
　　-l user connect with specified username
　　-l 用户，用指定的用户连接空间
　　-pw passw login with specified password
　　-pw 密码，用指定的密码登录空间
　　-1/-2 强迫 ssh 使用的版本
　　-4 -6 force use of IPv4 or IPv6
　　-C 打开压缩
　　-i key 钥匙，证明用的钥匙文件
　　-batch 关闭交互能力，也许
　　-unsafe 不安全，允许伺服端取代字符 (危险的)
　　常用的方法：
　　1、把本地文件file传输到Linux服务器的/root/
　　C:\>pscp.exe file 192.168.32.50:/root/
　　它会提示你输入密码，就像Linux下使用scp那样。
　　【注意】这里pscp会使用你Windows登陆的用户名，因此你可能需要指定Linux用户名。
　　C:\>pscp.exe file root@192.168.32.50:/root/
　　或者
　　C:\>pscp.exe -l root file 192.168.32.50:/root/
　　【注意】这里"-l root"要在file之前
　　2、**把本地目录dir、文件file传输到Linux服务器的/root/，**并指定服务器端口2009
　　C:\>pscp.exe -P 2009 -r dir file root@192.168.32.50:/root/
　　3、**把服务器上的/root/file文件取回来本地当前目录**
　　C:\>pscp.exe root@192.168.32.50:/root/file .
　　4、把服务器上的/root/dir目录取回本地"C:\My Documents\data\"目录
　　C:\>pscp.exe -r root@192.168.32.50:/root/dir "C:\MyDocuments\data\"
