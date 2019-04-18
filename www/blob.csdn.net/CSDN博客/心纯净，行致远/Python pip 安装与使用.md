# Python pip 安装与使用 - 心纯净，行致远 - CSDN博客





2019年02月24日 17:10:20[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：50标签：[pip																[Python](https://so.csdn.net/so/search/s.do?q=Python&t=blog)](https://so.csdn.net/so/search/s.do?q=pip&t=blog)
个人分类：[【Python】](https://blog.csdn.net/zhanshen112/article/category/7930576)








pip 是 Python 包管理工具，该工具提供了对Python 包的查找、下载、安装、卸载的功能。

目前如果你在 [python.org](https://www.python.org) 下载最新版本的安装包，则是已经自带了该工具。

### Python 2.7.9 + 或 Python 3.4+ 以上版本都自带 pip 工具。

pip 官网：[https://pypi.org/project/pip/](https://pypi.org/project/pip/)

### 你可以通过以下命令来判断是否已安装：

### pip --version

如果你还未安装，则可以使用以下方法来安装：

$ curl https://bootstrap.pypa.io/get-pip.py -o get-pip.py   # 下载安装脚本
$ sudo python get-pip.py    # 运行安装脚本
> 
**注意：**用哪个版本的 Python 运行安装脚本，pip 就被关联到哪个版本，如果是 Python3 则执行以下命令：

$ sudo python3 get-pip.py    # 运行安装脚本。
一般情况 pip 对应的是 Python 2.7，pip3 对应的是 Python 3.x。


部分 Linux 发行版可直接用包管理器安装 pip，如 Debian 和 Ubuntu：

sudo apt-get install python-pip
### pip 最常用命令

**显示版本和路径**

pip --version
**获取帮助**

pip --help
**升级 pip**

pip install -U pip
> 
如果这个升级命令出现问题 ，可以使用以下命令：

sudo easy_install --upgrade pip

**安装包**

pip install SomePackage              # 最新版本
pip install SomePackage==1.0.4       # 指定版本
pip install 'SomePackage>=1.0.4'     # 最小版本
比如我要安装 Django。用以下的一条命令就可以，方便快捷。

pip install Django==1.7
**升级包**

pip install --upgrade SomePackage
升级指定的包，通过使用==, >=, <=, >, < 来指定一个版本号。

**卸载包**

pip uninstall SomePackage
**搜索包**

pip search SomePackage
**显示安装包信息**

pip show 
**查看指定包的详细信息**

pip show -f SomePackage
**列出已安装的包**

pip list
**查看可升级的包**

pip list -o
### pip的help如下：

![](https://img-blog.csdnimg.cn/20190224170947273.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)






