# 大数据学习笔记(九) -- linux脚本编程(一)之概念引入 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年09月25日 21:30:11[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：52标签：[source																[.																[#!																[脚本编程概念](https://so.csdn.net/so/search/s.do?q=脚本编程概念&t=blog)](https://so.csdn.net/so/search/s.do?q=#!&t=blog)](https://so.csdn.net/so/search/s.do?q=.&t=blog)](https://so.csdn.net/so/search/s.do?q=source&t=blog)
个人分类：[大数据](https://blog.csdn.net/weixin_40247263/article/category/8073779)


Bash 不光能执行交互模式下的命令，还可以执行文本当中的命令。

执行 文本中的命令可以通过 source 或 . 

如下

```bash
[root@node001 sh01]# cat sh01.sh 
ls -l /
echo "hello world"
```

```bash
[root@node001 sh01]# source sh01.sh 
总用量 94
dr-xr-xr-x.  2 root root        4096 9月  22 12:08 bin
dr-xr-xr-x.  5 root root        1024 9月  21 03:27 boot
drwxr-xr-x. 18 root root        3700 9月  21 06:01 dev
drwxr-xr-x. 75 root root        4096 9月  22 12:08 etc
drwxr-xr-x.  7 root root        4096 9月  22 04:22 home
dr-xr-xr-x.  8 root root        4096 9月  22 06:58 lib
dr-xr-xr-x. 10 root root       12288 9月  22 12:08 lib64
drwx------.  2 root root       16384 9月  21 03:25 lost+found
drwxr-xr-x.  2 root root        4096 9月  23 2011 media
dr-xr-xr-x.  7 root root        4096 3月  29 2017 mnt
drwxrwx---.  2 root groupshare  4096 9月  22 05:17 ooxx
drwxr-xr-x.  3 root root        4096 9月  22 07:33 opt
dr-xr-xr-x. 87 root root           0 9月  21 06:00 proc
dr-xr-x---.  4 root root        4096 9月  22 12:33 root
dr-xr-xr-x.  2 root root       12288 9月  22 09:30 sbin
drwxr-xr-x.  7 root root           0 9月  21 06:00 selinux
drwxr-xr-x.  2 root root        4096 9月  23 2011 srv
drwxr-xr-x. 13 root root           0 9月  21 06:00 sys
drwxrwxrwt.  3 root root        4096 9月  22 12:28 tmp
drwxr-xr-x. 14 root root        4096 9月  22 08:24 usr
drwxr-xr-x. 17 root root        4096 9月  21 03:26 var
hello world
[root@node001 sh01]# . sh01.sh 
总用量 94
dr-xr-xr-x.  2 root root        4096 9月  22 12:08 bin
dr-xr-xr-x.  5 root root        1024 9月  21 03:27 boot
drwxr-xr-x. 18 root root        3700 9月  21 06:01 dev
drwxr-xr-x. 75 root root        4096 9月  22 12:08 etc
drwxr-xr-x.  7 root root        4096 9月  22 04:22 home
dr-xr-xr-x.  8 root root        4096 9月  22 06:58 lib
dr-xr-xr-x. 10 root root       12288 9月  22 12:08 lib64
drwx------.  2 root root       16384 9月  21 03:25 lost+found
drwxr-xr-x.  2 root root        4096 9月  23 2011 media
dr-xr-xr-x.  7 root root        4096 3月  29 2017 mnt
drwxrwx---.  2 root groupshare  4096 9月  22 05:17 ooxx
drwxr-xr-x.  3 root root        4096 9月  22 07:33 opt
dr-xr-xr-x. 87 root root           0 9月  21 06:00 proc
dr-xr-x---.  4 root root        4096 9月  22 12:33 root
dr-xr-xr-x.  2 root root       12288 9月  22 09:30 sbin
drwxr-xr-x.  7 root root           0 9月  21 06:00 selinux
drwxr-xr-x.  2 root root        4096 9月  23 2011 srv
drwxr-xr-x. 13 root root           0 9月  21 06:00 sys
drwxrwxrwt.  3 root root        4096 9月  22 12:28 tmp
drwxr-xr-x. 14 root root        4096 9月  22 08:24 usr
drwxr-xr-x. 17 root root        4096 9月  21 03:26 var
hello world
```

通过 bash 命令可以新建一个子bash，通过echo $$ 可以查看当前bash的进程号，通过pstree命令可以查看进程树，我们找到pstree命令，然后往前看，可以看出bash的层级关系

```bash
[root@node001 sh01]# echo $$
21502
[root@node001 sh01]# pstree
init─┬─auditd───{auditd}
     ├─crond
     ├─login───bash
     ├─master─┬─pickup
     │        └─qmgr
     ├─5*[mingetty]
     ├─nginx───nginx
     ├─rsyslogd───3*[{rsyslogd}]
     ├─sshd───sshd───bash───pstree
     └─udevd───2*[udevd]

[root@node001 sh01]# bash
[root@node001 sh01]# echo $$
21834
[root@node001 sh01]# pstree
init─┬─auditd───{auditd}
     ├─crond
     ├─login───bash
     ├─master─┬─pickup
     │        └─qmgr
     ├─5*[mingetty]
     ├─nginx───nginx
     ├─rsyslogd───3*[{rsyslogd}]
     ├─sshd───sshd───bash───bash───pstree
     └─udevd───2*[udevd]

[root@node001 sh01]# exit
exit
[root@node001 sh01]# echo $$
21502
```

bash 命令 再创建一个 bash子进程的同时可以 执行文本中的命令，执行完毕以后会退出子进程

```bash
[root@node001 sh01]# bash sh01.sh
```

如果在文件中加入 一行 #! /bin/bash ，就可以通过 ./文件名的方式去执行该文本文件中的命令，并且是自动新建一个bash进程去执行的该文本文件

原理，当前bash会执行命令行的命令，命令行给出了当前路径下的一个文件，然后就会读取文件的内容发现第一行是

#！ bin/bash，就会新启动一个bash进程再次读取该文件，这是它发现第一行是 # 号开头，代表注释掉了，然后就继续执行下面的指令，执行完毕以后推出当前bash。

```bash
[root@node001 sh01]# cat sh01.sh 
#! /bin/bash
ls -l /
echo "hello world"
```

注意，以  ./文件名的方式去执行的话，需要给该文件添加上可执行权限

```bash
[root@node001 sh01]# chmod +x sh01.sh 
[root@node001 sh01]# ll
总用量 4
-rwxr-xr-x. 1 root root 40 9月  22 13:12 sh01.sh
[root@node001 sh01]# ./sh01.sh
```

同理，awk也可以这样使用

```bash
[root@node001 ~]# cat command.awk 
#! /bin/awk -f
{ 
	split($3,date,"-");
	if(date[2]=="01"){
		
		name[$1]+=$5;
		
		if($2=="0"){
			role[$1]="M";
		}else{
			role[$1]="W";
		}
		
	}
} 
END{
	for(i in name){
		print i "\t" name[i] "\t" role[i];
	}
} 
[root@node001 ~]# cat awk.txt 
Tom	0	2012-12-11	car	3000
John    1	2013-01-13	bike    1000
vivi	1	2013-01-18	car	2800
Tom	0	2013-01-20	car	2500
John	1	2013-01-28	bike	3500
[root@node001 ~]# ./command.awk awk.txt 
vivi	2800	W
Tom	2500	M
John	4500	W
```

总结 :    .  和 source去执行 是在当前bash中去执行的，哪怕文本中 第一行写了 #!  ...  。 如果使用 ./  且配合 第一行的 #! ... ，则会开启一个新的bash进程去执行文本中的命令，执行完毕以后会关闭该bash进程。 

bash还支持函数编程

如下

```bash
[root@node001 ~]# test(){
> echo "hello world"
> echo $param
> }
[root@node001 ~]# param=123
[root@node001 ~]# test
hello world
123
```


