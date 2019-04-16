# 环境安装系列 - Linux上搭建简易Git服务01 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2015年01月08日 17:33:26[initphp](https://me.csdn.net/initphp)阅读数：1254








这篇文章主要讲述简易的GIT服务器环境搭建，环境是CentOS。authorized_keys

#### 1. 安装GIT

这边比较简单的方法就是通过yum的方式安装


`sudo yum install git`



#### 2. 创建一个git用户

```
useradd git -d /home/git
passwd git
```

/home/git为服务端版本存储的目录地址。这个时候就可以通过git这个账号，登录服务器了。




#### 3. 添加公钥，SSH免登

首先，我们需要在本地生成公钥。生成的命令如下：


`ssh-keygen -t rsa`

最后会生成id_rsa和id_rsa.pub，其中id_rsa_pub为你的公钥。有了公钥后，我们放到服务器上就可以免登了。公钥内容类似下面这样的：



```
ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQCB007n/ww+ouN4gSLKssMxXnBOvf9LGt4L
ojG6rs6hPB09j9R/T17/x4lhJA0F3FR1rP6kYBRsWj2aThGw6HXLm9/5zytK6Ztg3RPKK+4k
Yjh6541NYsnEAZuXz0jTTyAUfrtU3Z5E003C4oxOj6H0rfIF1kKI9MAQLMdpGW1GYEIgS9Ez
Sdfd8AcCIicTDWbqLAcU4UpkaX8KyGlLwsNuuGztobF8m72ALC/nLF6JLtPofwFBlgc+myiv
O7TCUSBdLQlgMVOFq1I2uPWQOkOWQAHukEOmfjy2jctxSDBQ220ymjaNsHT4kgtZg2AYYgPq
dAv8JggJICUvax2T9va5 gsg-keypair
```


因为我们的目的是需要git命令的免登，所以需要在/home/git目录下创建.ssh文件夹和authorized_keys


```
mkdir .ssh
chmod 700 .ssh
cd .ssh
touch authorized_keys
chmod 600 authorized_keys
```



其中 .ssh文件夹必须是700的权限，authorized_keys文件必须是600的权限

然后将我们的公钥传到这个服务器上，将公钥内容拷贝进入authorized_keys文件中。


`cat /home/admin/id_rsa.pub > /home/git/.ssh/authorized_keys`



#### 4. 创建GIT服务器端仓库



```
cd /home/git
mkdir project.git
cd project.git/
git --bare init
```
这个时候服务端仓库就创建成功了。






#### 5. 客户端clone代码

这个时候，客户端就可以通过git命令clone代码了。


`git clone git@192.168.136.100:/home/git/project.git`

这个时候发现我竟然还需要输入密码，这种情况，我略感受伤，每次输入密码太麻烦了。



```
$ git clone git@192.168.136.100:/home/git/project.git
Cloning into 'project'...
git@192.168.136.100's password:
```



于是，我们检查刚才配置的.ssh文件夹，发现权限还是root root的，需要修改成 git的权限。



```
drwxr-xr-x. 7 root root 4096 1月   8 17:20 project.git
drwx------. 2 root root   28 1月   8 17:16 .ssh
```



执行命令：



```
chown -R git:git project.git
chown -R git:git .ssh
```
这个时候，我们发现可以直接不需要密码登陆就能下载git了。






#### 6. 管理git账号

最开始，我们创建了git的账号，但是这个账号权限太大了，拥有shell的登录权限。如果git账号泄露岂不是可以给服务器造成破坏。于是，可以修改git账号权限，限制git账号只能用于git的操作。


`vim /etc/passwd`


找到git的权限：


`git:x:1003:1003::/home/git:/bin/bash`

修改为：




`git:x:1003:1003::/home/git:/usr/bin/git-shell`


如果有其它的用户也需要GIT权限怎么办？可以将其它用户端公钥也放到 authorized_keys中去。

但是这种办法只适合于小团队范围的GIT项目版本管理。







