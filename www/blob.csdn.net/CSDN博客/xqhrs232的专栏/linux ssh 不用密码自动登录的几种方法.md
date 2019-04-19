# linux ssh 不用密码自动登录的几种方法 - xqhrs232的专栏 - CSDN博客
2017年06月30日 11:46:08[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：271
原文地址::[http://blog.csdn.net/chaixiachen/article/details/6932858](http://blog.csdn.net/chaixiachen/article/details/6932858)
1. 自动ssh/scp方法==
A为本地主机(即用于控制其他主机的机器) ;
B为远程主机(即被控制的机器Server), 假如ip为192.168.60.110;
A和B的系统都是Linux
在A上运行命令:
# ssh-keygen -t rsa (连续三次回车,即在本地生成了公钥和私钥,不设置密码)
# ssh root@192.168.60.110 "mkdir .ssh" (需要输入密码)
# scp ~/.ssh/id_rsa.pub root@192.168.60.110:.ssh/id_rsa.pub (需要输入密码)
在B上的命令:
# touch /root/.ssh/authorized_keys (如果已经存在这个文件, 跳过这条)
# cat /root/.ssh/id_rsa.pub >> /root/.ssh/authorized_keys (将id_rsa.pub的内容追加到authorized_keys 中)
回到A机器:
# ssh root@192.168.60.110 (不需要密码, 登录成功) 
如出现Agent admitted failure to sign using the key错误
则在A机器上：
# ssh-add   ~/.ssh/id_rsa  
2. 控制n个机器如上所述自动登录
那就需要n对钥匙(密钥和公钥), ssh-keygen 命令可以随意更改钥匙对的名字, 比如:
# ssh-keygen -t rsa
Generating public/private rsa key pair.
Enter file in which to save the key (/root/.ssh/id_rsa): /root/.ssh/id_rsa_192.168.60.110
这样私钥和公钥的名字分别就是: id_rsa_192.168.60.110和 id_rsa_192.168.60.110.pub;然后将 id_rsa_192.168.60.110.pub 文件的内容, 追加到sever的 ~/.ssh/authorized_keys文件中,最后, 在本地用ssh命令的 -i 参数指定本地密钥, 并登录:
# ssh -i /root/.ssh/id_rsa_192.168.60.110 someone@192.168.60.110
scp也是一样的
# scp -i /root/.ssh/id_rsa_192.168.60.110 filename someone@192.168.60.110:/home/someone
在文件.bashrc中加下两行，每次做同样的操作就不用敲入这样长的命令了：
alias sshcell='ssh -i /root/.ssh/id_rsa_192.168.60.110 someone@192.168.60.110'
alias scpcell='scp -i /root/.ssh/id_rsa_192.168.60.110 filename someone@192.168.60.110:/home/someone'
这样，直接键入一下指令实现ssh和scp自动登录：
# sshcell
# scpcell
3. 自动ssh/scp脚本
如果需要从A，到B，然后才能够到C，那么需要ssh和scp两次，是比较麻烦的。
ssh自动登录：
#!/usr/bin/expect -f
set timeout 30
spawn ssh weiqiong@B
expect "password:"
send "pppppp\r"
expect "]*"
send "ssh weiqiong@C\r"
expect "password:"
send "pppppp\r"
interact
scp从A拷贝文件到C：
#!/usr/bin/expect -f
set timeout 300
set file [lindex $argv 0]
spawn scp $file weiqiong@B:/home/weiqiong
expect "password:"
send "pppppp\r"
expect "]*"
spawn ssh weiqiong@B
expect "password:"
send "pppppp\r"
expect "]*"
send "scp $file weiqiong@C:/home/weiqiong\r"
expect "password:"
send "pppppp\r"
expect "]*"
exit
interact
scp从C拷贝文件到A：
#!/usr/bin/expect -f
set timeout 300
set file [lindex $argv 0]
spawn ssh weiqiong@B
expect "password:"
send "pppppp\r"
expect "]*"
send "scp weiqiong@C:/home/weiqiong/$file .\r"
expect "password:"
send "pppppp\r"
expect "]*"
send "exit\r"
expect "]*"
spawn scp weiqiong@B:/home/weiqiong/$file .
expect "password:"
send "pppppp\r"
interact
4. 建立ssh/scp通道
比如说我的机器是A，中间服务器为B，目标服务器是C
从A可以ssh到B，从B可以ssh到C，但是A不能直接ssh到C
现在展示利用ssh通道技术从A直接传输文件到C
1. ssh -L1234:C:22 userid@B
input B's password
(1234是本机A的空闲端口，该指令需要A机器上的root用户权限，实际上是在本机1234端口建立了一个通道)
2. 打开一个新的console，键入：
scp -P1234 filename userid@localhost:
input C's password 
