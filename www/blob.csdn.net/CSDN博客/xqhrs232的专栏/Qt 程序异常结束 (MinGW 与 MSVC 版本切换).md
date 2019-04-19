# Qt 程序异常结束 (MinGW 与 MSVC 版本切换) - xqhrs232的专栏 - CSDN博客
2018年01月08日 10:25:54[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：727
原文地址::[http://blog.csdn.net/csnd_ayo/article/details/72723635](http://blog.csdn.net/csnd_ayo/article/details/72723635)
# 引言
针对Qt的两种版本，是不是有很多的疑惑，或者说在切换时，遇到了程序异常结束，函数未定义等很多奇怪的问题？今天我们来对常见的几种问题进行分析与解决。
# 简介
操作系统：window7 x64 
编程IDE：Qt Creator 4.2.1 
Qt版本：`5.8.0(MinGW)` and `5.7.1(MSVC)`
- [引言](http://blog.csdn.net/csnd_ayo/article/details/72723635#%E5%BC%95%E8%A8%80)
- [简介](http://blog.csdn.net/csnd_ayo/article/details/72723635#%E7%AE%80%E4%BB%8B)
- [版本介绍](http://blog.csdn.net/csnd_ayo/article/details/72723635#%E7%89%88%E6%9C%AC%E4%BB%8B%E7%BB%8D)- [MinGW](http://blog.csdn.net/csnd_ayo/article/details/72723635#mingw)
- [MSVC](http://blog.csdn.net/csnd_ayo/article/details/72723635#msvc)
- [结语](http://blog.csdn.net/csnd_ayo/article/details/72723635#%E7%BB%93%E8%AF%AD)
- [版本切换](http://blog.csdn.net/csnd_ayo/article/details/72723635#%E7%89%88%E6%9C%AC%E5%88%87%E6%8D%A2)- [MinGW
 转 MSVC](http://blog.csdn.net/csnd_ayo/article/details/72723635#mingw-%E8%BD%AC-msvc)
- [结语](http://blog.csdn.net/csnd_ayo/article/details/72723635#%E7%BB%93%E8%AF%AD-1)
- [切换错误](http://blog.csdn.net/csnd_ayo/article/details/72723635#%E5%88%87%E6%8D%A2%E9%94%99%E8%AF%AF)- [错误类型](http://blog.csdn.net/csnd_ayo/article/details/72723635#%E9%94%99%E8%AF%AF%E7%B1%BB%E5%9E%8B)
- [详细介绍](http://blog.csdn.net/csnd_ayo/article/details/72723635#%E8%AF%A6%E7%BB%86%E4%BB%8B%E7%BB%8D)
- [EditPlus的使用](http://blog.csdn.net/csnd_ayo/article/details/72723635#editplus%E7%9A%84%E4%BD%BF%E7%94%A8)
- [结语](http://blog.csdn.net/csnd_ayo/article/details/72723635#%E7%BB%93%E8%AF%AD-2)
# 版本介绍
首先我们要在这之前先对版本进行一个必要的了解，有助于我们后面的操作。
## MinGW
MinGW，是Minimalist GNUfor Windows的缩写，发音基本上就是min(民)GW(字母发音)。
它是一个可自由使用和自由发布的Windows特定头文件和使用GNU工具集导入库的集合，允许你在GNU/Linux和Windows平台生成本地的Windows程序而不需要第三方C运行时（C Runtime）库，其最主要的包是移植自Unix-like系统的gcc/make/gdb/autoconf等开发工具，外加windows开发必备的winapi等。
MinGW 是一组包含文件和端口库，其功能是允许控制台模式的程序使用微软的标准C运行时（C Runtime）库。
## MSVC
```
网上资料太少，我用自己的话简单的对他进行介绍.
```
- 1
- 2
MSVC，是 `Microsoft Visual C++` 的缩写。它是微软的编译器和 
```
Visual
 Studio
```
 用的一样的编译器，允许你在 `Window` 平台生成本地的程序。
## 结语
简而言之，他们都是编译器，对于我们来说，我们需要关注的，它们之间的差别，想必应该是它们对于字符集的支持，在上述的介绍中我未曾提及，这里我补充一下，
`MinGW` 默认使用 `UTF-8` 作为编码字符集，而 `MSVC` 是微软家的，所以他和VS一样默认使用 `GB2312` 作为编码字符集。
VC只能读取GB2312的源代码，他会将很多其他代码在编译时自动转换成GB2312，例如中文注释。 
而gcc只能读取UTF-8的源代码。 
因为在编码上的出入，引发的一些不明所以的错误。
# 版本切换
如果还没有两种版本的同学，可以自行前往官网下载 [[前往官网下载]](http://download.qt.io/archive/qt/)
## MinGW 转 MSVC
在编译器切换的时候，注意两个部分，一个是 `Qt版本` 另一个则是 `编译器` 在高级的Qt
 Creator中，他们把他分开了，所以我们在选择的时候，务必将他们有效的统一对应起来。
## 结语
在window下，大多数人都使用的MSVC作为主要编译器，人在屋檐下，不得不低头。但是在Qt5之后，Qt已经统一使用了 utf-8 作为标准字符集，优劣都很明显。
若使用 MSVC 作为长期使用的编译器版本时，请做以下修改。
- 
打开文件编码，将GB2312作为默认的字符集.
打开方式： 工具(Alt+T) –> 选型(O) –> 文本编辑器 –> 行为 –> 文件编码(右侧中部)
- 
任何显式的字符串常量都添加 QStringLiteral
其实这也称得上是一种好习惯.
```bash
QString myname = QStringLiteral("OE");
```
- 1
# 切换错误
相信通过上面顺利的版本切换，同学们还是会遇到许多错误性的问题，这里我们做一个简单的拾遗。
## 错误类型
|编 号|代码|编译|错误|解决提示|
|----|----|----|----|----|
|1|代码对|编译过|程序异常结束(启动时)|检查编译器的和Qt版本是否对应，重新构建项目(清理moc qrc等文件)|
|2|代码对|编译不过|变量未声明等莫名其妙的错误|检查代码的字符集|
|3|不确定正确性|编译失败|%^&*!#)$&^@|这种世纪性的难题，可是难倒我了，这个问题我暂且搁置，希望有人能够为这段谱写爱的篇章。|
## 详细介绍
- 
第一种
这里出现的程序异常结束，百分之90%源于编译器的问题。 
调校编译器与Qt版本后，千万不要忘记去清理原项目产生的 .o moc 等文件。 
Qt Creator 和其它一些IDE 都存在一些编译优化的功能，所以务必记得清理干净重新编译。
- 
第二种
MinGW以 `utf-8` 的方式去编译源码，但在编译的过程中，有些代码文件是 `gb2312` 编码或其它编码，我们代码要是全英文就算了，可我们使用中文注释和英文源码进行了交叉编译，导致字符集紊乱了把编译器给搞傻了，MinGW用自己的方式边读边编译的时候，发现读不通了，他就报个错误给你看。
宝宝生气了，说好的，讲 `utf-8` 的，你给我念 `gb2312`，宝宝听不懂，但是宝宝要装作很懂的样子，然后给你报个其他错，气死你！
 @_@!
- 
第三种
导演，这里过！
## EditPlus的使用
这里我们可能会使用到一个文本编辑器 EditPlus [[一键下载]](https://www.baidu.com/link?url=6aHsPGgP57UR2dnMAfTOpGTi1IB2YtLVklzuDkvZfWFFu2Rm5pG269UfYliPd04-XXC7PLJmbERj14vOe6eOT3xSeY3EOIF6lagRicMzZPu&wd=&eqid=e60f4bc90007b9660000000259263c5d)
这里我们主要是对文本文件进行批量转换，逐一转换太过繁琐，所以我们借助 `EditPlus` 完成我们所需操作。
- 
搜索要修改的文件
![1](https://img-blog.csdn.net/20170525105932948?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQ1NORF9BeW8=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
- 
填写搜索规则
这里我填写了 `*` 我搜索当前文件夹下所有文件
![2](https://img-blog.csdn.net/20170525105947877?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQ1NORF9BeW8=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
- 
打开所有搜索到的文件
![3](https://img-blog.csdn.net/20170525110050315?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQ1NORF9BeW8=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
- 
批量编码转换
![4](https://img-blog.csdn.net/20170525110130941?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQ1NORF9BeW8=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
- 
选择转换的文件
![5](https://img-blog.csdn.net/20170525110200138?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQ1NORF9BeW8=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
- 
选择对应转换后编码
![6](https://img-blog.csdn.net/20170525110246082?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQ1NORF9BeW8=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
- 
保存文件
![7](https://img-blog.csdn.net/20170525110300499?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQ1NORF9BeW8=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
- 
结语
自此，我们对文件的批量转码操作就算是完结了，不用特意学习，平时用的时候，看着教程多转几次自然游刃有余。
# 结语
如果有需要补充的地方，希望朋友们可以通过留言，说出问题复现的方法，我在本机分析解决后会在本章补充。
版权声明：分享得以延续，交流方知彼此。作者：_OE_ 博客地址：http://blog.csdn.net/csnd_ayo
- 
本文已收录于以下专栏：
- [《Qter
 必知必会》](http://blog.csdn.net/column/details/15254.html)

