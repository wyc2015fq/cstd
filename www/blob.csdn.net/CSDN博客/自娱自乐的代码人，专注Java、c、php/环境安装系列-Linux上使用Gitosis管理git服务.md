# 环境安装系列 - Linux上使用Gitosis管理git服务 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2015年04月20日 19:09:11[initphp](https://me.csdn.net/initphp)阅读数：1295








前面我们使用了非常简单的git服务器搭建环境的方法 [环境安装系列 - Linux上搭建简易Git服务01](http://blog.csdn.net/initphp/article/details/42527089)

这里我们介绍使用Gitosis管理git服务。这边主要是centos环境。

### 安装git和gitosis

1. 如果你还没在服务器上安装git，则先用yum安装git服务器。


`sudo yum install git-core`





2. 下载gitosis。我们放在/home/目录下面。


`sudo git clone git://github.com/res0nat0r/gitosis.git`





3. 然后我们需要安装python，因为gitosis是python的。


`sudo yum install python-setuptools`



4. 然后就去gitosis目录里面执行一下python呗



```
cd gitosis/
sudo python setup.py install
```






5. 我们需要创建一个git用户，并且在/home/目录下生成/home/git文件夹，这个文件夹我们会用来做git的仓库文件夹。


`sudo useradd git`



6. 生成ssh的key。可以参考文章[《转载和积累系列 - linux 生成KEY的方法与使用 》](http://blog.csdn.net/initphp/article/details/45096151)




`ssh-keygen -b 2048 -t rsa`因为我们使用admin账号登陆，所以key会生成在/home/admin/.ssh/目录下面。





```
[admin@localhost .ssh]$ ls
id_rsa  id_rsa.pub  known_hosts
```






7. 初始化Gitosis。这边使用服务器上的public key，所以服务器本身就可以管理gitosis-admin这个仓库。

gitosis-admin仓库主要是用来管理git的用户组以及配置文件的。



`sudo -H -u git gitosis-init < /home/admin/.ssh/id_rsa.pub`如果出现下面的情况，就是创建成功，该公钥的拥有者就能修改用于配置 Gitosis 的那个特殊 Git 仓库了。





```
[admin@localhost home]$ sudo -H -u git gitosis-init < /home/admin/.ssh/id_rsa.pub 
[sudo] password for admin: 
初始化空的 Git 版本库于 /home/git/repositories/gitosis-admin.git/
重新初始化现存的 Git 版本库于 /home/git/repositories/gitosis-admin.git/
```




8. 对Gitosis钩子文件夹设置为可执行的权限。





```
sudo chmod 755 /home/git/repositories/gitosis-admin.git/hooks/post-update
```






9. 进入我的/home/admin目录，并且克隆gitosis-admin仓库


`git clone git@localhost:gitosis-admin.git`可以进入gitosis-admin目录看一下文件夹：





```
[admin@localhost gitosis-admin]$ ls
gitosis.conf  keydir
```
gitosis.conf：主要用于配置用户组和用户权限，以及git的项目仓库



keydir：存放每个用户的public key




10. 我通过服务器，将我自己的电脑上的public key放到keydir目录下面，并且在gitosis.conf中加入了gitosis-admin文件夹的可读写权限。

然后本地就可以下载gitosis-admin这个仓库了。

![](https://img-blog.csdn.net/20150420194319033?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

keydir目录：


![](https://img-blog.csdn.net/20150420194449442?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


gitosis.conf配置文件：



```
[gitosis]

[group gitosis-admin]
members = admin@localhost.localdomain
writable = gitosis-admin
```


11. 创建一个project的仓库，my@abc用户拥有读写权限；test仓库，my@abc只有读的权限，则需要配置gitosis.conf文件。



```
[gitosis]

[group gitosis-admin]
members = admin@localhost.localdomain my@abc
writable = gitosis-admin

[group project]
members = my@abc
writable = project

[group test]
members = my@abc
readonly = test
```


然后再本地初始化一个空的git项目，并且提交到远程服务器。





```
git init
git remote add origin git@192.168.136.100:project.git
git push origin master
```

















