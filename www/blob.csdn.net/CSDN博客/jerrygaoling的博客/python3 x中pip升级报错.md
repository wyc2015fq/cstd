
# python3.x中pip升级报错 - jerrygaoling的博客 - CSDN博客


2018年07月16日 19:03:35[jerrygaoling](https://me.csdn.net/jerrygaoling)阅读数：678


# python3.x中使用pip安装包
在使用pip进行安装python扩展包时，可能遇到警告或安装失败，提示语句为：
**You are using pip version 8.1.1, however version 9.0.1 is available. You should consider upgrading via the ‘python -m pip install –upgrade pip’ command.**
可能导致安装失败。
提示语句是告知可以对pip进行升级操作，操作语句为：
**python -m pip install –upgrade pip**
安装过程如下：
![这里写图片描述](https://img-blog.csdn.net/20180716190154894?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
再次安装，将无警告语句。

