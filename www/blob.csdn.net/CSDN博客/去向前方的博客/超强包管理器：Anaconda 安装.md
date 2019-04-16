# 超强包管理器：Anaconda 安装 - 去向前方的博客 - CSDN博客





2018年07月12日 15:38:59[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：519








# 目录：



- [目录：](#目录)
- [Anaconda简介](#anaconda简介)
- [安装步骤](#安装步骤)- [下载](#下载)

- [安装](#安装)
- [程序](#程序)- [配置](#配置)- [设置国内镜像](#设置国内镜像)


- [Python环境管理](#python环境管理)
- [Conda的包管理](#conda的包管理)
- [运行Python](#运行python)
- [参考资料](#参考资料)




# Anaconda简介

作为编程小白，我曾一度因为Python库函数的安装头疼不已，也经常觉得Jupyter Notebook的启动颇麻烦，直到我遇到了Anaconda，觉得超方便，而且它还可以管理Rstudio，对于数据科学家非常方便，于是为了避免广大像我一样的小白们为pip忙得焦头烂额，于是就东拼西凑出一篇专门给小白看的安装Anaconda的文章，因为面对的是初学者，很多东西都没写出来（其实是自己不会），如果需要的话可以到文章末尾的链接访问。

Anaconda是Python的一个开源发行版本，主要面向科学计算。在我看来主要优点是预装了很多第三方库，而且Anaconda中增加了conda install命令，安装新package格外方便，还自带Spyder IDE 和Jupyter Notebook，至于其他优点依旧还在摸索中。

# 安装步骤

## 下载

[从官网下载](https://www.continuum.io/downloads)：不过官网速度比较慢，不太推荐。 

 从[清华镜像](https://mirrors.tuna.tsinghua.edu.cn/anaconda/archive/)下载：豆瓣的镜像也可以，国内的较快(推荐)

# 安装

![这里写图片描述](https://img-blog.csdn.net/20180712152214205?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

前面的一直点Next就好， 

第一个勾是是否把Anaconda加入环境变量，这涉及到能否直接在cmd中使用conda、jupyter、ipython等命令，推荐打勾。 

第二个勾的意思是否把Anaconda自带的python设置成系统默认的Python，这个要看情况选择了，如果你希望你默认的python是其他功能的那你就不添加，如果想只使用anaconda的python，那就改变环境吧。
![图2](https://img-blog.csdn.net/20180712152500166?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

安装完后你的电脑会多出这几个程序:

# 程序

## 配置

### 设置国内镜像

别着急，现在还没完事呢。 

如果你现在就猴急猴急地去安装很多packages，你会被conda的龟速感动得声泪俱下，因为Anaconda.org的服务器在国外。所幸的是，清华TUNA镜像源有Anaconda仓库的镜像，我们将其加入conda的配置即可。 

请在cmd中运行以下代码：
```
conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free/
conda config --set show_channel_urls yes
# 设置搜索时显示通道地址
```

# Python环境管理

我们现在安装的是最新版Anaconda3，其自带的Python版本为3.6，如果我们需要 

添加2.7版本的Python，可以进行如下操作。（同理，如果要添加Python 3.x，之后操作里的2.7改为3.6或3.5即可）

```
#语法说明 conda create -n 环境名 python=python版本
conda create -n py27 python=2.7
```

之后我们可以通过activate py27和deactivate 命令激活、退出该环境。

py

通过python -v来查看当前Python的版本 

通过conda info -e命令查看已有的环境 

通过conda list 查看安装了的库 

或通过conda remove -n env_name –all来删除指定的环境（如果不添–all参数，而是指明某个库名，则是删除该库）。
也可以点开Anaconda Navigator查看已配置的环境和安装的包

![Navigator](https://img-blog.csdn.net/2018071215305827?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# Conda的包管理

用Conda安装新的包比较轻松，比如要安装scipy

```
# 安装scipy
conda install scipy
conda的一些常用操作如下：

# 查看当前环境下已安装的包
conda list

# 查看某个指定环境的已安装包
conda list -n python36

# 查找package信息
conda search numpy

# 安装package
conda install -n python36 numpy
# 如果不用-n指定环境名称，则被安装在当前活跃环境
# 也可以通过-c指定通过某个channel安装

# 更新package
conda update numpy

# 删除package
conda remove numpy
```

# 运行Python

Anaconda自带的Spyder就是个不错的IDE 

当然也有许多人偏爱Pycharm 

Tools–>Preferences–>python Interpreter–>Python Interpreter 
![这里写图片描述](https://img-blog.csdn.net/20180712153644522?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
剩下的IDE配置请参看[官网](https://docs.continuum.io/anaconda/ide_integration#id8)

# 参考资料

-[http://python.jobbole.com/86236/](http://python.jobbole.com/86236/)

-[https://www.zhihu.com/question/30123965](https://www.zhihu.com/question/30123965)

-[https://zhuanlan.zhihu.com/p/25198543](https://zhuanlan.zhihu.com/p/25198543)

-[http://blog.csdn.net/bitcarmanlee/article/details/51917642](http://blog.csdn.net/bitcarmanlee/article/details/51917642)
人生苦短，我用Python 

祝大家开始享受Python的快感吧！








