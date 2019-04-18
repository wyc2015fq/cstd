# Python:你需要知道的七七八八（基础篇） - 长歌行 - CSDN博客





2017年07月08日 22:27:50[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：621
所属专栏：[Python与GIS](https://blog.csdn.net/column/details/pygis.html)









平时都是使用C++的，工作中运用python的机会不多，所以python的学习都是利用业余时间完成的。这样一来，就没有系统的，全面的研究python，只能够七七八八的写一些随笔的杂记。不过可惜，目前没有更多的时间整理各种常用库的用法。以后有机会就补上代码和说明。 

我的代码会放在这儿：[bentan2013/PythonAndGIS](https://github.com/bentan2013/PythonAndGIS)

### Anaconda多版本python的配置

众所周知，python主要分2.x 和 3.x，其中又有很多小版本，而且安装某些软件的的时候，会默认安装各个版本的python。例如老版本的arcmap会安装python2.7。ARCGIS Pro会安装python3.5。这一切都会对系统环境带来影响。所以需要使用各种方法来配置虚拟环境。

我最先接触的就是anaconda了。列出一些简单的命令。

**创建一个虚拟环境**
`conda create -n yourenvname python=x.x anaconda`
**激活虚拟环境**
*注意，linux一般要加source，win不用*
`source activate yourenvname`
**安装包**

激活虚拟环境之后，输入 
`conda install -n yourenvname [package]`
**退出虚拟环境**
`source deactivate`

**删除虚拟环境**
`conda remove -n yourenvname -all`

*另外，在linux下，如果不想使用conda，就在.bashrc中注释掉conda的path就行*

*不使用虚拟环境的话，可以在bashrc  etc/profile中制定自己的python路径*
`export PYTHONPATH=$PYTHONPATH:yourpath`

### 各种库各种包

**安装**

- 使用pip或者pip3安装  

- 使用各种虚拟环境提供的工具安装 

常用的numpy, panda, matplotlib …就不在此一一介绍，下载使用很方便。而且安装了anaconda之后，会给你安装常用的各种包。 

另外 
**部署**

如果需要到处你项目中的各个包可以使用 
`pip freeze > requirements.txt`

如果需要安装别人项目中的各个包可以使用 
`pip install -r requirements.txt`
但是很多时候，如果你不使用虚拟环境的话，pip freeze导出的是你环境中的所有包（虽然建议是一个虚拟环境跑一个大项目）

试试这个

```
pip install pipreqs
pipreqs /path/to/project
```

**打包**

PyInstaller还是很好用的。就不详细介绍了。 
[PyInstaller](http://www.pyinstaller.org/)

顺便列一个[py2exe](http://www.py2exe.org/index.cgi/FAQ)吧。
反正第一次用会出现各种问题，善用搜索引擎就好。

然后使用python写的工具和库能极大的丰富自己的程序，例如ArcGIS桌面端产品中，基本上都能使用python进行扩展。

这几年，对于python我用得比较多的还是脚本化，自动化部分和Flask，ROS，机器学习部分。

### 多语言编程

这部分的内容太多了，估计以后要重新开一篇，大概属于高级部分吧。我用的最多的还是python和c++的交互。使用swig把c++封装为python库。真的很简单（我指的是使用方便） 

先放一个链接吧:[SWIG and Python](http://www.swig.org/Doc2.0/Python.html)

想多了解这部分，大概要稍微了解下，编译，链接，库。python py、pyc、pyo、pyd区别。Python如何写模块。等等。。

### 随便看看

[Python3.5 doc](https://docs.python.org/3.5/c-api/intro.html#include-files)
[500lines](https://github.com/aosabook/500lines/blob/master/README.md)
[fullstackpython](https://www.fullstackpython.com/)
[Interfacing C/C++ and Python with SWIG](https://docs.python.org/3.5/c-api/intro.html#include-files)






