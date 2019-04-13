
# GIT安装和配置 - 嵌入式Linux - CSDN博客

2013年09月13日 17:36:16[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：844


GIT这个功能非常强大。在ubuntu下面用起来非常的爽。
下面是整理的一些资料。
1. 服务器安装ssh,git, python-setuptools
sudo apt-get install openssh-server openssh-client
sudo apt-get install git-core
sudo apt-get install python-setuptools
2. 服务器安装gitosis**这一步好像我没有用，安装服务器的时间有点久，我用我的台式做服务器，笔记本做客户端**
cd /tmp
\#git clone git://eagain.net/gitosis.git
git clone https://github.com/res0nat0r/gitosis.git
git clone git://github.com/res0nat0r/gitosis.git
cd gitosis
sudo python setup.py install
**3. 客户端**
**sudo apt-get install openssh-server openssh-client**
**sudo apt-get install git-core**
**ssh-keygen -t rsa**
**cd /home/wqf/.ssh/**
**scp .ssh/id_rsa.pub wqf@192.168.1.188:/tmp****我的台式机的用户名是wqf  .  IP地址是192.168.1.188**
4. 服务器配置 git服务器
创建git服务器管理用户
sudo useradd -m git
sudo passwd git
vim /etc/passwd //将git /bin/sh改为/bin/bash
sudo mkdir /home/git/repositories
sudo chown git:git /home/git/repositories
sudo chmod 755 /home/git/repositories
git config --global user.name "cjcheng"
git config --global user.email "caijicheng2006@163.com"
5. 服务器初始化gitosis
**cd /tmp**
sudo -H -u git gitosis-init < /tmp/id_rsa.pub
sudo chmod 755 /home/git/repositories/gitosis-admin.git/hooks/post-update
**sudo chmod 777 /home/git/repositories/rk3168_jb422.git/hooks/post-update.sample**
6. 客户端clone gitosis
**git clone git@192.168.1.188:/home/repo/repositories/rk3168_jb422.git**


