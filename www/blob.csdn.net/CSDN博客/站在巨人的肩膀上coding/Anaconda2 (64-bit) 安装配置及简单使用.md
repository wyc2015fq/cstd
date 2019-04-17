# Anaconda2 (64-bit) 安装配置及简单使用 - 站在巨人的肩膀上coding - CSDN博客





2018年04月26日 13:56:48[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：566








# [Python Anaconda2 (64-bit) 安装后启动jupyter-notebook默认目录更改](http://www.cnblogs.com/whaozl/p/5514563.html)





python notebook 现在改名叫 jupyter-notebook ，被集成在Anaconda中。

Anaconda集成了python、Spyder、jupyter等python的编辑器，也集成了很多科学计算的lib库，当然有些库没有被集成进去

1、下载Anaconda

     网址：https://www.continuum.io/downloads

    下载python27版本 64位

    如果需要免费的学术license，可以

　　对于学生来说，可以申请免费的学术License，以安装额外的功能包，以实现计算过程的加速。

　　申请地址： [https://store.continuum.io/cshop/academicanaconda](https://store.continuum.io/cshop/academicanaconda)

　　申请后，会得到一个license文件，将其放在~/.continuum目录下即可。

2、安装一路next，环境变量设置就全选

    安装目录直接推荐到C盘下，不要默认，即：c:\anaconda2下

    但是有一个地方特别注意，安装时选择所有用户使用时提示错误，所以安装时选择当前用户

3、如果要安装新库，有两种方式：

    注意：安装好了Anaconda，已经给你安装和设置了所有环境变量，所以cmd下python是可以用的

            如果你需要配置eclipse的python，直接定位到c:\anaconda2\python.exe即可

    （1）pip安装，pip同样适用于Anaconda，你可以理解为Anaconda是python的一个插件而已。

           pip安装方法请参考[python的whl文件安装](http://www.cnblogs.com/whaozl/p/5250385.html)  pip list

    （2） 采用Anaconda自带的安装lib方法，比如要安装比Matplotlib更简单的库(Anaconda没装)seaborn

            conda install seaborn

　　　  conda install scikit-learn#也没有这个库

           查看已经安装的库：conda list

更新一个库：conda update scipy

升级conda插件：conda update conda

升级anaconda：conda update anaconda

4、更改jupyter-notebook的启动参数默认目录（用户目录）

     右键jupyter-notebook，打开位置，然后定位到Anaconda2 的安装目录下的python.exe

     再右键这个python.exe，看到起始位置了吧，放一个路径进去比如D:\down,确定。然后再去打开jupyter-notebook

     自动打开浏览器后，你就会发现你的目录已经变化了






# Windows下Anaconda的安装和简单使用





Anaconda is a completely free Python distribution (including for commercial use and redistribution). It includes over 195 of the most popular [Python packages](http://docs.continuum.io/anaconda/pkg-docs)for science, math, engineering, data analysis.

# 1、安装anaconda

之前什么都不需要安装，直接在[官网](http://www.continuum.io/downloads)下载anaconda，我下载的是Python2.7，32位的。 
![这里写图片描述](https://img-blog.csdn.net/20150726085300623)
下载后的文件是： 
![这里写图片描述](https://img-blog.csdn.net/20150726085348157)
直接双击安装，可以自己选定安装位置（这个你自己决定）。 
安装完成后，我们可以看到： 
![这里写图片描述](https://img-blog.csdn.net/20150726085325252)
安装完anaconda，就相当于安装了Python、IPython、集成开发环境Spyder、一些包等等。

# 2、Python（shell）

![这里写图片描述](https://img-blog.csdn.net/20150726085404345)
这个和我们直接安装Python得到的Python shell用法一样。当然由于安装了anaconda，所以在这里好多包我们都可以使用了。

# 3、IPython

## 3.1 IPython(shell)

我们可以直接点击打开，也可以像下面这样在命令提示符中输入ipython 
![这里写图片描述](https://img-blog.csdn.net/20150726085559618)
这个和我们pip install ipython安装的ipython用法一样。

## 3.2 Ipython QTConsole

直接点击打开， 
![这里写图片描述](https://img-blog.csdn.net/20150726085632144)

## 3.3 IPython Notebook

我们直接点击打开，或者在命令提示符中输入ipython.exe notebook。

# 4、Spyder（IDE）

## 4.1 Spyder

Spyder的最大优点就是模仿MATLAB的“工作空间”。 
直接点击打开： 
![这里写图片描述](https://img-blog.csdn.net/20150726085713125)

## 4.2 其他的IDE使用anaconda

由于安装完anaconda就自带了Spyder集成开发环境了，所以不需要任何配置可以直接使用，但是其他你自己安装的IDE要想使用anaconda需要配置。 
配置的方法我们可以去[官网](http://docs.continuum.io/anaconda/ide_integration)了解。下面我们以Pycharm为例。 
其实如果anaconda在Pycharm之前安装，那么遇到解释器选择的时候会有一个选项，我们直接选择就OK了。如果在Pycharm之后安装，我们只需要手动设置一下Pycharm所用的解释器就行了。 
![这里写图片描述](https://img-blog.csdn.net/20150726085812718)

# 5、安装包

其实安装完anaconda后，基本上就把我们常用的部分包安装好了，但是毕竟只是安装了一小部分（我们可以去官网查看安装了哪些包），还有一部分需要我们自己来安装。 
查看已经安装的包我们可以在命令提示符中输入pip list或者用anaconda自带的包管理器conda（conda list）。

## 5.1 pip 或conda安装

我们可以在命令提示符中输入pip install 包名，或者conda install 包名。conda和pip的用法基本上一致（更新？卸载？还不确定）。但是我们使用conda安装的时候不仅会安装当前你要安装的包还会提示更新其他已经安装过的包，所以我基本上都是直接用pip安装。

## 5.2 文件安装

pip或conda不能安装的话，我们就下载文件安装，比如exe文件（双击安装）或者whl文件（pip安装）等等。

# 6、anaconda自带的命令窗口

上面我们说的命令提示符基本上都是在系统盘上的操作。其实anaconda自带一个命令窗口。直接点击打开 
![这里写图片描述](https://img-blog.csdn.net/20150726085726974)
所有可以在“命令提示符”中运行的都可以在这里运行，只不过文件的路径改变了而已。就比如我们在这两个命令窗口中分别输入ipython.exe notebook，默认的文件.ipynb存储在不同的位置。

# 参考文献：

# 1、[官网](http://www.continuum.io/downloads)

# 2、[Python科学计算环境推荐——Anaconda](http://www.jb51.net/article/51651.htm)

# 3、[Python科学计算发行版—Anaconda](http://seisman.info/anaconda-scientific-python-distribution.html)

# 4、[Installing Python 3.3 on Anaconda Python for Windows](http://www.tuicool.com/articles/MjAz2m)








