# 使用virtualenv搭建Python虚拟环境 - zhusongziye的博客 - CSDN博客





2018年03月07日 21:06:15[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：1514








virtualenv用来部署独立Python的一个工具，用来解决版本依赖，及不兼容的项目。

##### virtualenv介绍

virtualenv部署时拥有自己的安装目录，不与其它任何环境共享一个库，能够管理Python的版本和库。

如同时开发多个应用时，这些应用需要共用一个Python，应用A在2.6版本，应用B需要2.7版本，这时候会出现版本不统一情况，利用virtualenv可以为每个应用做一个“隔离”的Python运行环境。

### 安装virtualenv

进入到windows终端（命令提示符），输入

> 
pip install virtualenv 


安装virtualenv,下方出现Successfully installed...字样时说明安装成功。

![](https://img-blog.csdn.net/2018030721021712?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### virtualenv使用

1.进入到创建虚拟环境的磁盘和目录，输入virtualenv vir_test1创建虚拟环境（此处在e:/vir_test目录下创建了vir_test1虚拟环境）

![](https://img-blog.csdn.net/20180307210233896?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

2.进入环境,首先进入vir_test1下的Scripts目录（此虚拟目录使用的是默认的Python3.6版本），并运行activate.bat激活虚拟环境（虚拟环境激活后，前面会有环境名称）

![](https://img-blog.csdn.net/20180307210311539?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




3.查看虚拟环境安装库，并在虚拟环境中安装docker

![](https://img-blog.csdn.net/20180307210345699?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




![](https://img-blog.csdn.net/20180307210357962?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




4.退出virtualenv，执行命令deactivate.bat
1.创建指定版本的虚拟环境

> virtualenv -p D:\Python36\python.exe test

D:\Python36\python.exe 是版本执行文件，test是虚拟环境名称

2.激活虚拟环境，运行Python命令
![](https://img-blog.csdn.net/20180307210423226?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

1.通常在写Python时我们会借用Python开发工具，此处以pycharm开发工具为例给大家讲讲如何在pycharm中使用virtualenv，打开pycharm并在file->Settings打开设置

![](https://img-blog.csdn.net/20180307210442952?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

2.Project:PythonProj中选择Project Interpreter，在下拉列表中选择Create Virtualenv

![](https://img-blog.csdn.net/20180307210500741?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




3.输入虚拟环境名称并确定

![](https://img-blog.csdn.net/20180307210518214?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




4.在项目中找到新创建的虚拟环境，并在此环境中打印hello

![](https://img-blog.csdn.net/2018030721053994?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




### virtualenv 常用命令

pip list 查看已安装的包

pip list --outdated 检查哪些包需要更新

pip install --upgrade Some Package 升级包

pip uninstall SomePackage 卸载包

pip --help 帮助

rmvirtualenv 虚拟环境名称

pip install 包名称 安装环境

virtualenv -version 查看当前版本号

virtualenv -clear 清空非root用户的安装，并重头开始创建隔离环境

virtualenv -no-site-packages 令隔离环境可以访问系统全局的site-package目录

virtualenv –unzip-setuptools 安装时解压Setuptools或Distribute

virtualenv –relocatable 重定位某个已存在的隔离环境

virtualenv –distribute 使用Distribute代替Setuptools

virtualenv –extra-search-dir=SEARCH_DIRS 用于查找setuptools/distribute/pip发布包的目录

virtualenv –never-download 禁止从网上下载任何数据

virtualenv –prompt==PROMPT 定义隔离环境的命令行前缀






