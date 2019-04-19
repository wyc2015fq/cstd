# qt5.5程序打包发布以及依赖 - xqhrs232的专栏 - CSDN博客
2018年01月11日 17:18:30[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：693
原文地址::[http://blog.sina.com.cn/s/blog_66cd08930102w74s.html](http://blog.sina.com.cn/s/blog_66cd08930102w74s.html)
相关文章
1、Qt程序打包发布方法（使用官方提供的windeployqt工具）----[http://blog.csdn.net/iw1210/article/details/51253458](http://blog.csdn.net/iw1210/article/details/51253458)
2、QT5的程序打包发布（将QT5的工程项目打包成一个exe程序）----[http://blog.csdn.net/windsnow1/article/details/78004265](http://blog.csdn.net/windsnow1/article/details/78004265)
3、Enigma Virtual Box 单文件制作工具7.10去广告版----[http://download.csdn.net/download/jgcsxzy/6755371](http://download.csdn.net/download/jgcsxzy/6755371)
玩qt5也有一段时间了，惭愧的是一直没有好好的发布过程序，因为写的都是小程序没啥需要用到发布，而且qt也说不上很熟悉，本来打算到基本掌握qt之后再来研究研究怎么打包程序，最近晚上的空闲时间多了，闲着也是闲着，于是便来试试。
在网上搜索了一下资料，发现资料比较少，qt5打包的也只看见几篇博文，也不是很详细，所以当我成功的打包了我的第一个程序之后心里想着的就是一定要把经验共享出来，这方法不一定是最好的，而且可能也还存在很多问题，这都有待我以后慢慢挖掘，也望大家多多指点，共同进步。
打包：
（1）首先程序要想发布，在编译器编译一定要选择“release”而不是“debug”，编译成release版本；
![qt5.5程序打包发布以及依赖](http://s1.sinaimg.cn/mw690/001SIIFlgy70ipQRFPG20&690)
  （2）编译成release之后，找到目录：
![qt5.5程序打包发布以及依赖](http://s14.sinaimg.cn/mw690/001SIIFlgy70iq1uvQF9d&690)
其实如果直接在这里双击“chat.exe”也是会提示缺少的dll文件，然后在qt的安装目录找对应的dll文件，将dll复制到该目录即可，dll在下面的路径：（VS版本可能有点不一样，不过都是bin目录下的）
![qt5.5程序打包发布以及依赖](http://s2.sinaimg.cn/mw690/001SIIFlgy70iqcbtIJf1&690)但是这样我试过很多次，即使提示的dll都复制进来了，双击也可以运行了，可是放到其他电脑还是无法运行，这是因为依旧缺少文件，如plugins下的一些插件，因此这样还是无法打包成功的。
于是为了更加方便快捷的解决这个问题，我们使用qt自带的windeployqt.exe寻找依赖的文件，我们使用批处理bat，在当前目录（即编译成release后的可执行程序所在目录）新建个bat文件：
![qt5.5程序打包发布以及依赖](http://s15.sinaimg.cn/mw690/001SIIFlgy70iqKIeWW3e&690)
使用记事本打开或者其他编辑工具亦可，然后把下面的代码复制进去保存;
如果使用的msvc版本的：
C:\Qt\Qt5.4.2\5.4\msvc2013_64\bin\windeployqt.exe myexe.exe
pause
如果使用的是mingw版本的：
C:\QT5\5.5\mingw492_32\bin\windeployqt.exe myexe.exe
pause
代码解析：C:\Qt\Qt5.4.2\5.4\msvc2013_64\bin\windeployqt.exe这个是windeployqt.exe工具的路径，要根据自己的qt安装情况进行修改，首先在这里看能不能找到windeployqt.exe，找到了直接把其路径复制过来即可；myexe.exe则是你的可执行程序的名称，不要照抄这里的。
（3）上面步骤完成之后直接双击这个bat文件就会把需要用到的所有dll文件自动复制到当前目录下：
![qt5.5程序打包发布以及依赖](http://s7.sinaimg.cn/mw690/001SIIFlgy70ircLkHQ56&690)
很便捷。
这个时候再去双击你的可执行程序应该就可以运行了。
（4）接着就需要把这些文件打包成一个exe文件了，我们可以用Enigma
 Virtual Box这款软件， 现在最新的版本已经是7.4了，百度一搜索就有，然后就开始添加文件打包了：
![qt5.5程序打包发布以及依赖](http://s1.sinaimg.cn/mw690/001SIIFlgy70j2l06B2e0&690)
![qt5.5程序打包发布以及依赖](http://s13.sinaimg.cn/mw690/001SIIFlgy70j2lGKaobc&690)
最后点击“执行封包”就能生成一个exe文件，如图示：
![qt5.5程序打包发布以及依赖](http://s7.sinaimg.cn/bmiddle/001SIIFlgy70j2rXCnk96&690)于是这个打包好的程序就可以放到其他电脑上运行啦。
（5）如果有需要制作成安装包程序可以使用Inno setup这款软件制作安装包，当然网上还有很多其他类似的，就需要大家自己去尝试啦，制作安装包的方法也是类似的，准备所需要的文件之后添加进去就可以了，网上很多这样的教程，大家参考这篇博文：http://blog.csdn.net/ruifangcui7758/article/details/6662646。
总结，我测试过很多次打包成exe在其他电脑上运行都是没有问题的，但是在群里跟他人讨论的时候他们说在某些情况还是会遇到问题，比如用到OpenGL库，一些第三方库如opencv等这样打包运行时就会出问题，我估计是
qt自带的windeployqt.exe这款工具有些库还是找不到的，尤其是外部的库，也就是说还是有一定的局限性，当然这只是我的猜测，我现在还没遇到，等遇到这种情况的时候我会再去寻找解决方法的，到时会继续更新这篇博文。
若有什么错漏之处，欢迎大家指出，共同学习，一起进步啦！！

