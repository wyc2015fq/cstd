
# scp免密码远程拷贝 - 高科的专栏 - CSDN博客

2015年03月10日 17:13:33[高科](https://me.csdn.net/pbymw8iwm)阅读数：2100


有些时候，我们在复制/移动文件 到另一台机器时会用到scp，因为它比较安全。但如果每次
都要输入密码，就比较烦了，尤其是在script里。不过，ssh有另一种用密钥对来验证的方
式。下面写出我生成密匙对的过程，供大家参考。
第一步：生成密匙对，我用的是rsa的密钥。使用命令 "ssh-keygen -t rsa"
[user1@rh user1]$ ssh-keygen -t rsa
Generating public/private rsa key pair.
Enter file in which to save the key (/home/user1/.ssh/id_rsa):
Created directory '/home/user1/.ssh'.
Enter passphrase (empty for no passphrase):
Enter same passphrase again:
Your identification has been saved in /home/user1/.ssh/id_rsa.
Your public key has been saved in /home/user1/.ssh/id_rsa.pub.
The key fingerprint is:
e0:f0:3b:d3:0a:3d:da:42:01:6a:61:2f:6c:a0:c6:e7 user1@rh.test.com
[user1@rh user1]$
复制代码

生成的过程中提示输入密钥对保存位置，直接回车，接受默认值就行了。接着会提示输入一
个不同于你的password的密码，直接回车，让它空着。当然，也可以输入一个。(我比较懒
，不想每次都要输入密码。) 这样，密钥对就生成完了。
其中公共密钥保存在 ~/.ssh/id_rsa.pub
私有密钥保存在 ~/.ssh/id_rsa
然后改一下 .ssh 目录的权限，使用命令 "chmod 600 ~/.ssh"
[user1@rh user1]$ chmod -R 600 ~/.ssh
[user1@rh user1]$
复制代码 之后把这个密钥对中的公共密钥复制到你要访问的机器上去，并保存为
~/.ssh/authorized_keys.
scp ~/.ssh/id_rsa.pub 192.168.11.51:/root/.ssh/authorized_keys
user1@rh1's password:
id_rsa.pub 100% 228 3.2MB/s 00:00
scp ./newfile  root@192.168.11.51:/home


