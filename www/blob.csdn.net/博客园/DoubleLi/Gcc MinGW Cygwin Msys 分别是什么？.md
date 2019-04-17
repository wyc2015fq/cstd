# Gcc/MinGW/Cygwin/Msys 分别是什么？ - DoubleLi - 博客园






一、GCC的历史

GCC是一个原本用于Unix-like系统下编程的编译器。

不过，现在GCC也有了许多Win32下的移植版本。

所以，也许对于许多Windows开发者来说，GCC还是一个比较陌生的东西。

所以，我希望通过这章的叙述，让你——一个Windows开发者对GCC这一个优秀的编译器有一个大概的了解。

GCC是GNU公社的一个项目。

是一个用于编程开发的自由编译器。

最初，GCC只是一个[C语言](http://lib.csdn.net/base/c)编译器，他是GNU C Compiler 的英文缩写。

随着众多自由开发者的加入和GCC自身的发展，如今的GCC以经是一个包含众多语言的编译器了。

其中包括 C,C++,Ada,Object C和[Java](http://lib.csdn.net/base/javase)等。所以，GCC也由原来的GNU C Compiler变为GNU Compiler Collection。

也就是 GNU编译器家族 的意思。当然，如今的GCC借助于他的特性，具有了交叉编译器的功能，即在一个平台下编译另一个平台的代码。

直到现在，GCC的历史仍然在继续，他的传奇仍然被人所传颂。

**二、Windows下的GCC家族**

起初，GCC是用于Unix,Unix-like系统的编译器。

不过，现在Windows下也出现了GCC的稳定移植版。

这要感谢Internet上众多程序员的共同努力。

如今，在Windows下比较流行的GCC移植版主要有三个。他们是 MinGW,Cygwin和Djgpp。

虽然，Djgpp是应用于DOS系统的，考虑到windows对DOS的兼容，所以也将Djgpp纳入其中了。

总体来说，MinGW，Cygwin和Djgpp各有特色，针对不同的应用场合，可以选择不同的移植版来满足需要。

**MinGW 的主要方向是让GCC的Windows移植版能使用Win32API来编程**。
**Cygwin 的目标是能让Unix-like下的程序代码在Windows下直接被编译**。
Djgpp 则是想让DOS下也能用上GCC。

所以，对于开发人员不同的需求。选择合适的移植版会让工作变得轻松而高效。

**三、分别介绍**

MinGW

Minimalistic GNU for Windows。
她是一个建立在GCC和binutils 项目上的编译器系统。和其他GCC的移植版相比，她可以说是最接近Win32的一个了。
因为，MinGW几乎支持所有的Win32API，这也是MinGW的特色之一。
她所连接的程序，不需要任何第三方库就可以运行了。
在某种程度上看，MinGW更像是VC的替代品。
官网: http://www.mingw.org

Cygwin

其实并不是一个GCC。她是让Windows拥有Unix-like环境的软件。
所以，GCC自然也就会包含在里面。
不过，对于开发者，Cygwin是一个开发环境。而对于用户来说Cygwin是一个运行环境。
Cygwin唯一和MinGW最大的区别在于，使用Cygwin可以在Windows下调用Unix-like的系统函数。
比如进程函数，等等。所以，虽然说，Cygwin是运行在Windows下的，但是她还是使用的是Unix-like系统的函数和思想。
官网: http://www.cygwin.com/

MSYS

Unix-like command line utilities
包括基本的bash, make, gawk and grep 等等。通常也可以认为是小型的UNIX on Windows。提供在windows上模拟Unix环境来使用MinGW。
msys-cn :http://code.google.com/p/msys-cn/MSYS
中国发行版，用UNIX开发环境开发Windows程序。
**MSYS在windows下模拟了一个类unix的终端，它只提供了MinGW的用户载入环境，在MSYS模拟的unix环境下使用MinGW，就像在Unix使用gcc一样**。

**转载自**[点击打开链接](http://www.6zou.net/tech/whatis-gcc-mingw-cygwin-msys.html)









