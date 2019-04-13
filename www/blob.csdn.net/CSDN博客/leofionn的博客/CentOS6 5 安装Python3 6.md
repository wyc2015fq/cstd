
# CentOS6.5 安装Python3.6 - leofionn的博客 - CSDN博客


2018年04月15日 08:06:08[leofionn](https://me.csdn.net/qq_36142114)阅读数：644


首先去python官网下载python3的源码包，网址：https://www.python.org/
进去之后点击导航栏的Downloads，也可以鼠标放到Downloads上弹出菜单选择Source code，表示源码包，这里选择最新版本，当然下面也有很多其他历史版本，点进去之后页面下方可以看到下载链接，包括源码包、Mac OSX安装包、Windows安装包
![](https://images2015.cnblogs.com/blog/734555/201602/734555-20160204152757772-228746966.png)
Python安装之前需要一些必要的模块，比如openssl，readline等，如果没有这些模块后来使用会出现一些问题，比如没有openssl则不支持ssl相关的功能，并且pip3在安装模块的时候会直接报错；没有readline则python交互式界面删除键和方向键都无法正常使用，至于需要什么模块在make完之后python会给出提示，通过提示进行安装即可装全
yum -y install zlib zlib-devel
yum -y install bzip2 bzip2-devel
yum -y install ncurses ncurses-devel
yum -y install readline readline-devel
yum -y install openssl openssl-devel
yum -y install openssl-static
yum -y install xz lzma xz-devel
yum -y install sqlite sqlite-devel
yum -y install gdbm gdbm-devel
yum -y install tk tk-devel
接下来可以安装python了，编译过程中会自动包含这些依赖.
tar -xvzf Python-3.6.5.tgz
cd Python-3.6.5/
./configure --prefix=/usr/python
make && make install
mv /usr/bin/python /usr/bin/python-2.6
ln -s /usr/python/bin/python3.6 /usr/bin/python
vi /usr/bin/yum
将第一行中的“\#!/usr/bin/python”
修改为“\#!/usr/bin/python-2.6”，保存即可

二、安装 pip
2.1 yum 安装
\# 首先安装 epel 扩展源
yum -y install epel-release
\# 安装 python-pip
yum -y install python-pip
\# 清除 cache
yum clean all
通过这种方式貌似只能安装 pip2，想要安装 Python 3 的 pip，可以通过以下的源代码安装方式。
2.2 源码安装
\#下载源代码
wget --no-check-certificate https://github.com/pypa/pip/archive/9.0.1.tar.gz
tar -xf 9.0.1.tar     \# 解压文件
cd pip-9.0.1
\# 使用 Python 3 安装
python3 setup.py install
创建链接：
sudo ln -s /usr/local/python3/bin/pip /usr/bin/pip3
2.3 升级 pip
pip install --upgrade pip


