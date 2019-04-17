# Linux 虚拟环境安装和配置 - ChihkAnchor的博客 - CSDN博客





2018年11月28日 22:51:09[Chihk-Anchor](https://me.csdn.net/weixin_40871455)阅读数：25








## 安装和配置虚拟环境：

1）pip install virtualenv #安装虚拟环境

2）pip install virtualenvwrapper #安装虚拟环境扩展包

3）编辑家目录（可使用 cd ~  进入家目录）下面的.bashrc文件，添加下面两行。

****export WORKON_HOME=$HOME/.virtualenvs****

****source /usr/local/bin/virtualenvwrapper.sh****

4）使用source .bashrc使其生效一下。

如果source后显示  bash: /usr/local/bin/virtualenvwrapper.sh: No such file or directory  

可使用 find / -name virtualenvwrapper.sh  查找其目录，然后修改 .bashrc 文件

## ******使用****** ：

****创建虚拟环境命令：****

mkvirtualenv 虚拟环境名

mkvirtualenv -p python3  Test   创建名为Test的python3虚拟环境

****进入虚拟环境工作：****

workon 虚拟环境名

****查看机器上有多少个虚拟环境：****

workon 空格 + 两个tab键

****退出虚拟环境：****

deactivate

****删除虚拟环境：****

rmvirtualenv 虚拟环境名



