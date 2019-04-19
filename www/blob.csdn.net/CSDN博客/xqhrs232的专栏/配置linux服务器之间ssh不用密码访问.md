# 配置linux服务器之间ssh不用密码访问 - xqhrs232的专栏 - CSDN博客
2017年05月04日 11:43:04[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2470
原文地址::[http://blog.csdn.net/emili/article/details/3856622](http://blog.csdn.net/emili/article/details/3856622)
相关文章
1、使ssh登陆不用输入密码的方法----[http://www.jb51.net/os/RedHat/1228.html](http://www.jb51.net/os/RedHat/1228.html)
2、[ssh或登陆linux主机不需要输入密码的几种方法](http://blog.chinaunix.net/uid-12306154-id-3053425.html) ----[http://blog.chinaunix.net/uid-12306154-id-3053425.html](http://blog.chinaunix.net/uid-12306154-id-3053425.html)
3、如何通过linux ssh远程linux不用输入密码登入----[http://jingyan.baidu.com/article/f3ad7d0fe7bd0d09c2345b75.html](http://jingyan.baidu.com/article/f3ad7d0fe7bd0d09c2345b75.html)
如果想在 A 这太机器上可以不需要密码就 ssh 到 B 、 C 两台机器上，可以采用如下的方法：
（ 1 ） 在 A 机器上： ssh-keygen -t rsa
Generating public/private rsa key pair.
Enter file in which to save the key (/nutch/home/.ssh/id_rsa): 不输入任何东西，直接回车
Enter passphrase (empty for no passphrase): 不输入任何东西，直接回车
Enter same passphrase again: 不输入任何东西，直接回车
Your identification has been saved in /nutch/home/.ssh/id_rsa.
Your public key has been saved in /nutch/home/.ssh/id_rsa.pub.
The key fingerprint is:
40:b8:e9:8f:0a:f6:ce:e0:73:a8:6a:7e:52:b4:45:ca nutch@linux1
这个命令将为 A 上的用户生成其密钥对，询问其保存路径时直接回车采用默认路径，当提示要为生成的密钥输入passphrase 的时候，直接回车，也就是将 其设定为空密码。生成的密钥对 id_rsa ， id_rsa.pub ，默认存储在/home/username/.ssh 目录下。
(2) 在 A 机器上：将 id_rsa.pub 的内容复制到 A 、 B 、 C 三台机器 /home/username/.ssh/authorized_keys 文件中，如果机器上已经有 authorized_keys 这个文件了，就在文件末尾加上 id_rsa.pub 中的内容，如果没有 authorized_keys 这个文件，直接 cp 或者 scp 就好了，下面的操作假设各个机器上都没有 authorized_keys 文件。（如果B、C机器上没有~/.ssh目录可以手动自己创建,权限要设置成可以写，因为要copyauthorized_keys)
  对于 A ： cp id_rsa.pub authorized_keys
  对于 B 和 C ： scp authorized_keys username@B:~/.ssh/ 此处的 username 是要访问的 B 机器上的用户名。
(3) chmod 644 authorized_keys
  这一步非常关键，必须保证 authorized_keys 只对其所有者有读写权限，其他人不允许有写的权限，否则 SSH 是不会工作的
(4) 可以直接 ssh username@ 机器名了，同一台机器的不同用户直接也可以以这种方式来访问
如果经过上面的步骤还是提示需要密码，注意一下权限问题，主要有三个权限
  (1) /home/username这目录的权限应该是700
  (2) /home/username/.ssh 这个目录的权限也应该是700
  (3) /home/username/.ssh/ authorized_keys这个文件的权限是644
如果还是不正确，那么tail /var/log/secure一下这个日志文件，就能发现问题。
-------------其他的ssh问题--------------------------------------------
刚装了ubuntu,ssh localhost如果提示ssh: connect to host lcoalhost port 22: Connection refused
那么有可能是openssh-server没有安装，可以通过命令ps -e|grep ssh 来查看ssh-server是否开启，如果ps -e|grep ssh只显示ssh-agent，并且/etc/init.d/目录下没有ssh，那说明系统并没有安装ssh-server，因此可以通过如下命令来安装ssh-server：
sudo apt-get install openssh-server
并可以通过/etc/init.d/ssh start命令开启（一般安装openssh-server后会自动开动的）
可以通过命令netstat -tlp来查看sshd是否运行
tcp6    0    0 *:ssh                *:*                   LISTEN     -
看到上面这一行输出说明ssh-server已经在运行了。
