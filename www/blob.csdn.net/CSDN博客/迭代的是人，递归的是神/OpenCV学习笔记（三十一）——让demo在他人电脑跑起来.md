# OpenCV学习笔记（三十一）——让demo在他人电脑跑起来 - 迭代的是人，递归的是神 - CSDN博客





2011年11月29日 12:49:45[yang_xian521](https://me.csdn.net/yang_xian521)阅读数：20510
所属专栏：[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html)









这一节的内容感觉比较土鳖。这从来就是一个老生常谈的问题。学MFC的时候就知道这个事情了，那时候记得老师强调多次，如果写的demo想在人家那里演示一下，一定要选择静态库使用mfc，而不是选择动态链接库，否则在人家电脑里没有对应的dll文件，是无法运行起来的。可见老师在这方面吃过亏啊。昨天用OpenCV写了个东西，发过去让人家测试，可人家告诉我：“你这土鳖程序在我这无法运行“，好囧啊。这里把我的解决过程记录一下。希望能对大家遇到类似的问题有所帮助。

首先，介绍一下我的开发环境32bits+winXP+VS2008+OpenCV，他的电脑32bits+Win7+none（他电脑其他开发环境基本没装，是个裸机）。

我程序里用到的库文件包括core、highgui、imgproc、video，用的是debug版本，写的是个控制台程序。所以把相应的dll文件copy过去，分别是opencv_core231d.dll、opencv_highgui231d.dll、opencv_imgproc231d.dll、opencv_video231d.dll（如果是release版本，要copy相应的没有d结尾的动态链接库，下面相应的dll都存在类似的问题）。本以为这样就ok了，第一次传给他，他告诉我运行不了，没有任何提示，用命令行运行，提示应用程序缺少相应的并行配置，百度出来的答案千奇百怪，感觉都没有切中要害。

于是又找了一台装的xp的电脑试验，这次弹出错误是缺少tbb_debug.dll，我忽然想起来我第一次运行程序的时候也是弹出过这个错误。于是根据电脑的配置是32位机，开发环境是vs9，找到对应目录下的tbb_debug.dll文件，copy过去，再试。

这次弹出的错误比较离奇，提示缺少msvcp100d.dll，可是大哥，我用的开发环境是vs2008，对应的文件应该是msvcp90d.dll才对吧（这个问题目前没想出合理解释），不管了，死马当活马医，又去别人电脑copy过来msvcp100d.dll和msvcr100d.dll，再试。

这次弹出缺少msvcp90d.dll了，呵呵，该来的还是会来。这是由于他的电脑没有vs平台运行时的对应dll造成的，网上搜了下解决办法。去C:\Program Files\Microsoft Visual Studio 9.0\VC\redist\x86\Microsoft.VC90.CRT目录把Microsoft.VC90.CRT.manifest、msvcm90.dll、msvcp90.dll、msvcr90.dll四个文件都copy过来，又去C:\Program
 Files\Microsoft Visual Studio 9.0\VC\redist\x86\Microsoft.VC90.OPENMP目录下把Microsoft.VC90.OpenMP.manifest、vcomp90.dll两个文件copy过去。

这次总算离胜利比较近了，不报错了，但是视频文件无法打开，感觉还是OpenCV相关的动态链接库没有copy过去，又copy了几个dll，比如opencv_ffmpeg.dll。这次总算可以运行了。oh yeah~~

问题是解决了，不过对于dll文件的理解还是不够深刻，网上还有说是因为运行时库的问题，需要在相关项目设置里把MDD设置为MTD，但我试过，发现不是这个问题。要是能有办法，像MFC那样把相应的dll进行静态编译，都做到exe里面就好了。不知道是我知之甚少，还是OpenCV的程序不行。期待高手指点一二。

感谢大家指导，我把心得又写了一篇[http://blog.csdn.net/yang_xian521/article/details/7027190](http://blog.csdn.net/yang_xian521/article/details/7027190)



