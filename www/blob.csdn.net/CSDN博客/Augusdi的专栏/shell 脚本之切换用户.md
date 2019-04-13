
# shell 脚本之切换用户 - Augusdi的专栏 - CSDN博客


2015年05月13日 14:47:57[Augusdi](https://me.csdn.net/Augusdi)阅读数：24970


﻿﻿
用su - 用户 -c 命令格式来执行. 有时候需要切换到root用户做一些操作，如果你的用户没有添加sudo权限，那么在脚本中实现就需要使用expect命令了，
具体实现如下：
```python
#!/usr/bin/expect
spawn su root
expect "Password:"
send "rootpassword\n"
interact
expect eof
exit
```
Bash代码
```python
#!/bin/sh 
expect -c " 
spawn su - root 
expect \"Password:\" 
send \"111111\r\" 
interact 
"
```
111111是密码    root是用户  可以用变量代替。这个是bash里调用的expect，我没直接写expect脚本

简单的方法如下：

```python
localhost ~ # cat switchUser.sh 
#!/bin/bash
whoami
su - yanu <<!
whoami
exit
!
whoami
localhost ~ # ./switchUser.sh 
root
yanu
root
localhost ~ #
```
复制代码代码如下:
\#!/bin/bash
su - test <<EOF
pwd;
exit;
EOF
执行结果图:
![](http://files.jb51.net/file_images/article/201412/20141231105006427.jpg?20141131105024)
当然也可以用下面的命令来执行
su - test -c "pwd"
![](http://files.jb51.net/file_images/article/201412/20141231105039478.jpg?20141131105046)
ps:
切换用户只执行一条命令的可以用: su - oracle -c command
切换用户执行一个shell文件可以用:su - oracle -s /bin/bash shell.sh

```python
passwd=load 
su - load <<! 
$passwd 
cd ~/backup 
rm *.txt 
exit 
!
```
用expect可以参考这里的做法：[http://qyiyunso.blog.163.com/blog/static/350776862009112395940647/](http://qyiyunso.blog.163.com/blog/static/350776862009112395940647/)；核心代码如下：|\#!/usr/bin/expect -f
\# ch2git.expect
 
\#set timeout 5
set username git
set userpass git
spawn su $username
expect "Password:"
send "$userpass\r"
interact


|

这里用户名和密码都是git，注意前面的两个set的行；运行结果如下：
|$ch2git.expect 
spawn su git
Password: 
$ exit


|

你也可以把输出的信息重定向掉；


