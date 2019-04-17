# Python安装、配置 - 李鑫o_O - CSDN博客





2016年03月10日 21:03:27[hustlx](https://me.csdn.net/HUSTLX)阅读数：644
个人分类：[python](https://blog.csdn.net/HUSTLX/article/category/6128600)










1、Python简介：

Python在Linux、windows、Mac os等操作系统下都有相应的版本，不管在什么操作系统下，它都能够正常工作。除非使用平台相关功能，或特定平台的程序库，否则可以跨平台使用。python有许多优点，如：简单、易学、免费开源、高层语言、可移植性、解释性、面向对象、可扩展性、可嵌入性、丰富的库。

2 安装python 

Windows之所以使用的这么普遍，是因为在windows下所有的操作都变的非常简单，安装软件只要点点“下一步”就可以了。Python也不例外，可以到网上下个Windows版的python（网上到处都是）我使用的是python-2.7.2,下好后直接安装就ok了。最后记得配置一下环境变量，具体操作如下：

1）在系统变量中找到path，

![clip_image002](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311102140569-892497835.gif)


2）编辑path值，添加你安装的python路径，

![clip_image004](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311102141288-697358579.gif)


3）检验python是否安装配置成功，打开cmd，输入python，如果出现以下界面，则说 明你的python安装成功了。

![clip_image006](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311102142007-1662750985.gif)

#### 3、python配置numpy、matplotlib、scipy

[numpy、matplotlib、scipy](http://www.scipy.org/install.html) 的安装比较简单，下载相应版本的exe文件，直接点击运行就行。它会自动检索到Python的安装路径。但是安装完成之后


import  matplotlib.pyplot as plt  之后提示有错误，提示说缺少 dateutil，安装dateutil，dateutil安装完成之后会提示说缺少[pyparsing](http://www.lfd.uci.edu/~gohlke/pythonlibs/#python-dateutil) ，然后下载安装。但是还会出现错误提示说no
 module named six，解决方法： 把路径如：C:\ Python27\Lib\site-packages\scipy\lib中的six.py six.pyc six.pyo三个文件拷贝到C:\ Python27\Lib\site-packages目录下。


至此windows下python已经成功配置numpy、matplotlib、scipy。




