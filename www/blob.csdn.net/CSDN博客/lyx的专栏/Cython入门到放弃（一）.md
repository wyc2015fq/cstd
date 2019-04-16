# Cython入门到放弃（一） - lyx的专栏 - CSDN博客





2018年06月07日 21:16:53[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：8721








        python作为一门强大的脚本语言，优势自然不必说，目前中低频的量化投资基本都是使用python作为research和production作为语言。但是，当我们的模型较复杂，运算量较大的时候，python的短板就会出现，就是运算速度慢。

        当然，解决这一问题有很多方法，比如笔者先前提到的pypy的jit技术，但是抛开稳定性不提，使用pypy有很多限制，而且不是那么工程化。所以，从本文开始，笔者向大家介绍Cython的一系列文章，方便将来量化或者别的应用场景中的一些重计算的部分单独取出来，然后用Cython改写，独立成模块来提高运算速度。

        首先，大家都知道，C语言是编译性语言，而Python则是解释性语言，简而言之，就是Python运行是逐行运行，不需要实现编译，而C需要在运行前编译。那么，可想而知，如果运行前，需要事先编译，那么其实有一种“上帝视角”的感觉，自然可以做很多优化，让代码运行的更快。

        而Cython的出现就是这个目的，让Python也可以被编译，然后执行。大家要区别Cpython和Cython，Cpython大家可以认为是python的一种，其实大家平时使用的基本都是cpython。而Cython大家可以直接理解为一种语言，Cython是一种部分包含和改变C语言，以及完全包含pyhton语言的一个语言集合。学习一段时间后大家肯定会更加理解这句话。

        我们先来过一下cython编写的流程吧。

        在写cython之前，我们需要一个c编译器，笔者目前使用的是win10，不知道怎么的，就是不能像以前win7一样好好玩，所以直接在linux下面编译了。如果大家是windows平台，需要先安装一个专门为python准备的编译器，笔者的版本是2.7的，链接如下：

https://www.microsoft.com/en-us/download/confirmation.aspx?id=44266

        然后就是测试一下流程了。首先我们要知道，cython的文件后缀是.cyx，而且既然cython是python语言的一个超集，那么，cyx里面运行纯python文件也是没有问题的了。

        我们新建一个hello.pyx文件，在里面输入

```python
print "Hello World"
```

        没有任何特殊，然后我们新建一个setup.py脚本，这个python文件的目的就是使用cython编译器去编译pyx折一个cython文件。

        setup文件的代码很简单：

```python
from distutils.core import setup
from Cython.Build import cythonize

setup(
    ext_modules=cythonize("hello.pyx")
)
```

        然后我们运行一下setup文件，python setup.py build_ext --inplace. build_est大概就是编译extension文件的意思，inplace则是放到源代码pyx旁边，也就是同一目录下的意思。

        然后，我们就可以看到多了两个文件，一个是hello.c一个是hello.so（如果你是windows，那么应该是.pyd）

        这里，c文件就是cython将我们的pyx文件解析成一个c文件件，不依赖与平台，而后面的so或者pyd文件，则是将c文件进行编译后的一个文件，依赖于平台。

        这个时候，so文件或者pyd文件就可以像普通的python文件一样，被import。

        当然，每次如果都要这样写个setup文件编译一下，其实挺麻烦的，还好cython给我们提供了一个很方便的解决方案。

写完hello.pyx的cython代码之后，不需要编译，当我们需要使用的时候，在文件前面加上

```python
import pyximport
pyximport.install()
import hello
```
        然后就可以了。其实，不是不用编译了，而是cython默默帮我们编译好了，放在其他地方罢了，对于我们使用者而言，整体就简洁好的诺。

        这就是一个cython使用的初步，性能如何，以及cython语法，将在后面的部分中讲述。

参考数据：Cython A Guide for Python Programming



