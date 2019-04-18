# 通过Unison实现CentOS 5.5下文件的双向同步 - weixin_33985507的博客 - CSDN博客
2012年04月06日 20:37:06[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：10
**===================================================================****联系方式：**Author：**300second**QQ:**84287030**Blog:[**http://300second.blog.51cto.com**](http://300second.blog.51cto.com/)运维交流群：**80068448****（欢迎广大朋友一起交流****windows/linux/unix****技术）**
**文档信息：**
文档版本：Version 1.0
修改记录：2012-4-6
系统环境：CentOS 5.5（适合32bit and 64bit）
**格式约定：**灰色底：输入的系统命令
黄色底：输入的系统命令返回的信息或者配置文件文本信息
绿色底：技巧或需要注意的地方
红色底：需要特别注意的地方
蓝色字：内容注释
**=====================================================================**Unison是Windows、Linux以及其他Unix平台下都可以使用的文件同步工具，它能使两个文件夹（本地或网络上的）保持内容的一致，拥有与其它一些同步工具或文件系统的相同的特性，但也有自身的特点：
1）支持跨平台使用；
2）对内核和用户权限没有特别要求；
3）Unison是双向的，它能自动处理两分拷贝中更新没有冲突的部分，有冲突的部分将会显示出来让用户选择更新策略；
4）只要是能连通的两台主机，就可以运行unison，可以直接使用socket连接或安全的ssh连接方式，对带宽的要求不高，使用类似rsync的压缩传输协议。
**环境如下：**
master：192.168.1.80
slave：192.168.1.88
**一、编译安装Unison**
Linux下通过源码包编译安装Unison时，需要用到Objective Caml compiler。
1）编译安装ocaml
[root@master ~]# cd /usr/local/src
[root@master src]# wget [http://caml.inria.fr/pub/distrib/ocaml-3.12/ocaml-3.12.1.tar.gz](http://caml.inria.fr/pub/distrib/ocaml-3.12/ocaml-3.12.1.tar.gz)
[root@master src]# tar -zxvf ocaml-3.12.1.tar.gz
[root@master src]# cd ocaml-3.12.1
[root@master ocaml-3.12.1]# ./configure
 [root@master ocaml-3.12.1]# make world opt
[root@master ocaml-3.12.1]# make install
2）编译安装Unison
[root@master src]# tar -zxvf unison-2.40.63.tar.gz
[root@master src]# cd unison-2.40.63
[root@master unison-2.40.63]# make UISTYLE=text
[root@master unison-2.40.63]# make install
在执行make install的过程中，出现以下错误提示：
make[1]: Leaving directory `/usr/local/src/unison-2.40.63'
mv /root/bin//unison /tmp/unison-10558
mv: 无法 stat “/root/bin//unison”: 没有那个文件或目录
make: [doinstall] 错误 1 (忽略)
cp unison /root/bin/
cp: 无法创建一般文件“/root/bin/”: 是一个目录
make: *** [doinstall] 错误 1
出现错误的原因在于Unison默认是将文件Copy到/root/bin目录，但Linux默认是没有该目录的，因此需要将生成的可执行文件unison复制到系统的PATH目录。
[root@master bin]# whereis $PATH#查看系统的PATH目录
bin: /usr/local/bin
[root@master unison-2.40.63]# cp unison /usr/local/bin
3）将可执行文件unison上传到远程主机slave:192.168.1.88
[root@master unison-2.40.63]# scp unison root@192.168.1.88:/root/
The authenticity of host '192.168.1.88 (192.168.1.88)' can't be established.
RSA key fingerprint is 57:7a:b5:4d:fa:6d:37:db:70:12:c6:54:87:6b:7b:f8.
Are you sure you want to continue connecting (yes/no)? yes
Warning: Permanently added '192.168.1.88' (RSA) to the list of known hosts.
root@192.168.1.88's password:
unison100% 1473KB1.4MB/s00:01
4）通过SSH登陆到远程主机slave ，再将unison复制到slave的PATH目录
[root@master unison-2.40.63]# ssh 192.168.1.88
root@192.168.1.88's password: 
Last login: Fri Apr6 09:59:35 2012 from 192.168.1.96
[root@slave ~]# ll
总计 1536
-rw------- 1 root root1026 01-24 19:17 anaconda-ks.cfg
drwxr-xr-x 2 root root4096 02-28 21:52 Desktop
-rw-r--r-- 1 root root26313 01-24 19:16 install.log
-rw-r--r-- 1 root root4343 01-24 19:08 install.log.syslog
-rwxr-xr-x 1 root root 1508530 04-06 10:56 unison
[root@slave ~]# cp unison /usr/local/bin
[root@slave ~]# ll /usr/local/bin
总计 1480
-rwxr-xr-x 1 root root 1508530 04-06 11:03 unison
**二、配置ssh key信任**
建议通过普通用户进行操作，理由是通过root操作本身就危险，免密码登陆的root就更危险了。
1）分别在两台服务器上创建用户：admin
[root@master ~]# useradd -m admin
[root@master ~]# passwd admin
[root@slave ~]# useradd -m admin
[root@slave ~]# passwd admin
2）在master上创建key并配置slave的信任
[root@master ~]# su - admin
[admin@master ~]$ ssh-keygen -t rsa
Generating public/private rsa key pair.
Enter file in which to save the key (/home/admin/.ssh/id_rsa):
Created directory '/home/admin/.ssh'.
Enter passphrase (empty for no passphrase): 
Enter same passphrase again: 
Your identification has been saved in /home/admin/.ssh/id_rsa.
Your public key has been saved in /home/admin/.ssh/id_rsa.pub.
The key fingerprint is:
74:b9:99:6a:24:a1:e8:d7:64:46:65:ad:86:60:0e:ad admin@master
在提示保存私钥（key）和公钥（public key）的位置时，使用默认值；
在提示是否需要私钥密码（passphrase）时，直接敲回车，即不使用私钥密码。
之后，将生成一对密钥，id_rsa（私钥文件）和id_rsa.pub（公钥文件），保存在/home/admin/.ssh/目录下。
3）将公钥添加到slave的 authorized_keys 文件中
a、将文件上传到slave主机
[admin@master ~]$ scp ~/.ssh/id_rsa.pub admin@192.168.1.88:/home/admin/
The authenticity of host '192.168.1.88 (192.168.1.88)' can't be established.
RSA key fingerprint is 57:7a:b5:4d:fa:6d:37:db:70:12:c6:54:87:6b:7b:f8.
Are you sure you want to continue connecting (yes/no)? yes
Warning: Permanently added '192.168.1.88' (RSA) to the list of known hosts.
admin@192.168.1.88's password:
id_rsa.pub100%3940.4KB/s
b、通过SSH到登陆到slave主机，并将公钥添加到 authorized_keys 文件中
[admin@master ~]$ ssh admin@192.168.1.88
admin@192.168.1.88's password:
[admin@slave ~]$ mkdir .ssh
[admin@slave ~]$ chmod 700 .ssh
[admin@slave ~]$ mv ~/id_rsa.pub ~/.ssh/authorized_keys
[admin@slave ~]$ chmod 600 ~/.ssh/authorized_keys
4）同理，执行以下步骤在slave主机上创建key并配置master主机的信任
[root@slave ~]# su - admin
[admin@slave ~]$ ssh-keygen -t rsa
Generating public/private rsa key pair.
Enter file in which to save the key (/home/admin/.ssh/id_rsa): 
Enter passphrase (empty for no passphrase): 
Enter same passphrase again: 
Your identification has been saved in /home/admin/.ssh/id_rsa.
Your public key has been saved in /home/admin/.ssh/id_rsa.pub.
The key fingerprint is:
50:71:69:00:20:49:a7:18:7f:10:79:b4:31:04:95:62 admin@slave
[admin@slave ~]$ scp ~/.ssh/id_rsa.pub admin@192.168.1.80:/home/admin/
The authenticity of host '192.168.1.80 (192.168.1.80)' can't be established.
RSA key fingerprint is 57:7a:b5:4d:fa:6d:37:db:70:12:c6:54:87:6b:7b:f8.
Are you sure you want to continue connecting (yes/no)? yes
Warning: Permanently added '192.168.1.80' (RSA) to the list of known hosts.
admin@192.168.1.80's password: 
id_rsa.pub100%3930.4KB/s
通过SSH到登陆到master主机，并将公钥添加到 authorized_keys 文件中
[admin@slave ~]$ ssh admin@192.168.1.80
admin@192.168.1.80's password:
[admin@master ~]$ mv ~/id_rsa.pub ~/.ssh/authorized_keys
5）重启两台服务器的SSH服务
[root@master ~]# /etc/init.d/sshd restart
[root@slave ~]# /etc/init.d/sshd restart
**三、Unison的配置与使用**
在两台服务器上创建test目录，用于测试
[root@master ~]# su - admin
[admin@master ~]$ mkdir test
[root@slave ~]# su - admin
[admin@slave ~]$ mkdir test
在两台服务器上分别执行一次unison，出现提示确认，则直接敲回车选择默认值
[admin@master ~]$ unison /home/admin/test/ ssh://admin@192.168.1.88//home/admin/test/
[admin@slave ~]$ unison /home/admin/test/ ssh://admin@192.168.1.80//home/admin/test/
修改两台服务器的unison配置文件，输入以下内容
[admin@master ~]$ vim /home/admin/.unison/default.prf
#Unison preferences file
root = /home/admin/test
root = ssh://admin@192.168.1.88//home/admin/test/
#force =
#ignore =
batch = true
#repeat = 1
#retry = 3
owner = true
group = true
perms = -1
fastcheck = false
rsync = false
sshargs = -C
xferbycopying = true
log = true
logfile = /home/admin/.unison/unison.log
[admin@slave ~]$ vim /home/admin/.unison/default.prf
#Unison preferences file
root = /home/admin/test
root = ssh://admin@192.168.1.80//home/admin/test/
#force =
#ignore =
batch = true
#repeat = 1
#retry = 3
owner = true
group = true
perms = -1
fastcheck = false
rsync = false
sshargs = -C
xferbycopying = true
log = true
logfile = /home/admin/.unison/unison.log
**相关注解如下：**
force表示会以本地所指定文件夹为标准，将该目录同步到远端。这里需要注意，如果指定了force参数，那么Unison就变成了单项同步了，也就是说会以force指定的文件夹为准进行同步，类似于rsync。
ignore = Path表示忽略指定目录，即同步时不同步它。
batch = true，表示全自动模式，接受缺省动作，并执行。
fastcheck = true 表示同步时仅通过文件的创建时间来比较，如果选项为false，Unison则将比较两地文件的内容。
log = true 表示在终端输出运行信息。
logfile 指定输出的log文件。
另外，Unison有很多参数，这里仅介绍常用的几个，详细的请参看Unison手册。
-auto //接受缺省的动作，然后等待用户确认是否执行。
-batch //batch mode, 全自动模式，接受缺省动作，并执行。
-ignore xxx //增加 xxx 到忽略列表中
-ignorecase [true|false|default] //是否忽略文件名大小写
-follow xxx //是否支持对符号连接指向内容的同步
owner = true //保持同步过来的文件属主
group = true //保持同步过来的文件组信息
perms = -1 //保持同步过来的文件读写权限
repeat = 1 //间隔1秒后,开始新的一次同步检查
retry = 3 //失败重试
sshargs = -C //使用ssh的压缩传输方式
xferbycopying = true"
-immutable xxx //不变目录，扫描时可以忽略
-silent //安静模式
-times //同步修改时间
-path xxx 参数 //只同步 -path 参数指定的子目录以及文件，而非整个目录，-path 可以多次出现。
**PS：Windows下的unison配置文件默认位于C:\Documents and Settings\currentuser\.unison目录，默认的配置文件名是default.prf。**
**四、Unison双向同步测试**
首先分别在master与slave的/home/admin/test目录下创建文件或目录，然后在master上执行unison，接着如果在master与slave上都能看到各自创建的文件，就说明同步成功。
分别在master与slave上创建文件
[admin@master ~]$ cd test
[admin@master test]$ touch 001.txt 003.txt
[admin@master test]$ ls
001.txt003.txt
[admin@slave ~]$ cd test
[admin@slave test]$ touch 002.txt 004.txt
[admin@slave test]$ ls
002.txt004.txt
在master上执行unison
[admin@master test]$ unison
在master与slave上分别查看文件是否同步
[admin@master test]$ ls
001.txt002.txt003.txt004.txt
[admin@slave test]$ ls
001.txt002.txt003.txt004.txt
均看到了“001.txt002.txt003.txt004.txt”文件，说明文件同步已经成功！
**五、设置定期执行同步**
通过crontab计划任务来定期执行同步，以下方式设置每5分钟执行一次
[admin@master ~]$ crontab -e
*/5 * * * * /usr/local/bin/unison
[admin@slave ~]$ crontab -e
*/5 * * * * /usr/local/bin/unison
