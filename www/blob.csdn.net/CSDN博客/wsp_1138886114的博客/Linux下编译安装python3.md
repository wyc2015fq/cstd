# Linux下编译安装python3 - wsp_1138886114的博客 - CSDN博客





2018年07月24日 18:51:02[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：71








##### 前言：

Linux下大部分系统默认自带python2.x的版本，最常见的是python2.6或python2.7版本，默认的python被系统很多程序所依赖，比如centos下的yum就是python2写的，所以默认版本不要轻易删除，否则会有一些问题，如果需要使用最新的Python3那么我们可以编译安装源码包到独立目录，这和系统默认环境之间是没有任何影响的，python3和python2两个环境并存即可

##### 安装步骤：

> 
###### 1. 首先去官网下载python3的源码包，网址：[https://www.python.org/](https://www.python.org/)

下载页网址：[https://www.python.org/downloads/release/python-370/](https://www.python.org/downloads/release/python-370/)
![这里写图片描述](https://img-blog.csdn.net/20180724184857822?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```
包括源码包、Mac OSX安装包、Windows安装包 

下载的就是源码包：Python-3.5.1.tgz，下载好之后上传到linux系统。 
准备安装python安装之前需要一些必要的模块，比如openssl，readline等，比如没有openssl则不支持ssl相关的功能， 
并且pip3在安装模块的时候会直接报错；没有readline则python交互式界面删除键和方向键都无法正常使用， 
至于需要什么模块在make完之后python会给出提示，通过提示进行安装即可装全， 
下面是需要提前预装的依赖：
```

```
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
```

###### 2.接下来安装python了，编译过程中会自动安装依赖.执行命令：

**释放文件**：`tar -xvzf Python-3.5.1.tgz`
**进入目录：**`cd Python-3.5.1/`

**配置编译：**上面依赖包是用yum安装而不是自己编译的，所以都是安装在系统默认目录下，因此各种选项不用加默认即可生效：

`./configure --prefix=/usr/python --enable-shared CFLAGS=-fPIC`

```
补充：加上--enable-shared和-fPIC，将python3的动态链接库编译出来， 方便编译第三方库需要python
```

**编译源码：**`make`
**执行安装：**`make install`

  安装目录就在/usr/python 
### 配置环境

> - 
安装完成后配置：  

  即将python库路径添加到 `/etc/ld.so.conf` 配置中，然后执行`ldconfig` 生效；  

  或者添加到 $LD_LIBRARY_PATH 中，  

  这样在接下来运行python3是就不会报找不到库文件的错误了.- 
系统中原来的python在`/usr/bin/python`，通过`ls -l`可以看到,  

  python是一个链接到本目录下的python2.7下的软链接 

  我们可以不用把这个删除，不对原来默认的环境做任何修改，只新建一个python3的软链接即可，  

  只是需要执行python3代码时python要改成python3，  

  或者python脚本头部解释器要改为#!/usr/bin/python3 
建立有关的软链接如下：  
`ln -s /usr/python/bin/python3 /usr/bin/python3`
`ln -s /usr/python/bin/pip3 /usr/bin/pip3`

  直接执行python3命令就可以调用python3了











