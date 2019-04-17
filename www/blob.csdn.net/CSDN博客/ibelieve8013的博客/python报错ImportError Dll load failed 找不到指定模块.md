# python报错ImportError:Dll load failed:找不到指定模块 - ibelieve8013的博客 - CSDN博客





2018年04月17日 12:15:41[ibelieve8013](https://me.csdn.net/ibelieve8013)阅读数：513















Python中安装外部的包可以通过在anacondaprompt中的命令使用Pip安装：如我要安装贝叶斯的包：pipinstall pgmpy。一般来说这是可以的，还有一种更简单的方法：在pycharm中，可以通过setting中，右边绿色的加号，选择你要添加的包，如输入：pgmpy，会有相应的选项，以及Install选项。（其实此处执行的命令本质上也是在cmd中的：pip install pgmpy）。

![](https://img-blog.csdn.net/20180417121455829?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

以上方法都属于第一种方法：

1.命令： *pip install *安装的库名（例：pip install nummy）

2.下载whl文件（从此网站：[Python Extension Packages forWindows](https://link.zhihu.com/?target=http%3A//www.lfd.uci.edu/~gohlke/pythonlibs/)）到某一目录，然后使用命令：*pip install 文件名*（如果不是在该文件的目录下使用，则文件名要加上路径），注意下载的whl文件要和使用**Python的版本**及**计算机位数**符合。

然而，当我安装好了库之后，却意外的出现的了题目所述的问题，找了好多办法，网上好多同学也是遇到了这个问题，他们都提到了一个问题：就是版本的问题，如scipy与安装的包的版本不一致的话，可能会出现这个问题。解决方法就是**将所有包都统一来源，要么全部使用官方出版的包，要么全部使用whl里面的包，问题就解决了。**

于是我就通过pip uninstall scipy，pgmpy,然后统一通过pycharm中的库安装方法进行安装，竟然就可以了，也是很意外。

知乎上@黄哥说的:

**先安装Visual C++ Redistributablefor Visual Studio 2015**

**再pip3 install scipy**

对于我的机子也不行，因为老师报错说无法Importpytoml.也不知是什么鬼问题。

总之python 的坑太多，比如现在，我就又遇到了下一个问题：在使用pgmpy库时，报错：object of type 'dict_keyiterator'has no len()。

很纳闷，我上一次在实验室用这个的时候完全没这个问题。现在实验室电脑莫名其妙的抛出这个问题，我才在笔记本上安装PgmPy,结果又掉坑里了，看来对绕不开了。



