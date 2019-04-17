# OpenCV学习笔记（三十二）——制作静态库的demo，没有dll也能hold住 - 迭代的是人，递归的是神 - CSDN博客





2011年11月30日 18:00:02[yang_xian521](https://me.csdn.net/yang_xian521)阅读数：20178
所属专栏：[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html)









感谢大家对我博客的支持，昨天写的那个土鳖的blog[http://blog.csdn.net/yang_xian521/article/details/7022701](http://blog.csdn.net/yang_xian521/article/details/7022701)，为了让自己的程序在别人那里运行起来，竟然加了十余个dll，才搞定，太不方便了。对于我这土鳖的办法，有好心的网友看不下去了，告诉我OpenCV是可以制作静态链接库。我顿时来了兴致，百度之，发现确实有办法，但很多都是老版本的数据结构，还是对cv.lib等等的处理。我这里用2.3版实现了一下，把我的心得分享给大家。

首先用cmake重新生成vs2008的解决方案，之前我都用的编译好的OpenCV，现在用的多了，才知道cmake的妙处。因为是要生成opencv的静态库, 去掉一些无关的选项. 去掉BUILD_NEW_PYTHON_SUPPORT，BUILD_SHARED_LIBS, BUILD_TESTS, 勾选OPENCV_BUILD_3RDPARTY_LIBS, WITH_TBB，WITH_JASPER, WITH_JPEG, WITH_PNG, WITH_TIFF选项，然后点击configure. 提示TBB_INCLUDE_DIR找不到，忽略即可，直接点击configure，配置完成，点击generate，完成后关闭cmake得到解决方案后，打开OpenCV.sln，哇，要不要这么多工程...一个一个来吧。根据OpenCV编译好的动态链接的lib，有calib3d、contrib、core、features2d、flann、gpu、haartraining_engine（这个是静态链接的，不需要重新编译）、highgui、imgproc、legacy、ml、objdetect、ts、video，以上这些工程需要重新编译为静态链接库。打开对应工程的属性，找到下图中的对应项，

![](http://hi.csdn.net/attachment/201111/30/0_1322637312kM1w.gif)


把输出目录改为自己设定的目录，把配置类型由动态库（.dll）改为静态库（.lib），把运行时库改为静态。重新生成（ctrl+F7）。得到了我心爱的lib文件，不过也吓了我一跳，每个lib比动态链接的lib要大出几十倍，都10M左右，推荐大家写程序，如果不是做成demo，给别人展示，还是不要用静态链接的好，否则做出来的exe好大啊...

我这里把生成的lib库整理了一下，debug版本的在后缀加了231ds，（s表示静态库），release后缀加上了231s。（由于发现OpenCV有做好的静态lib，我就不上传了）

做好了库之后，准备把lib文件copy到我OpenCV的安装目录下准备使用，才悲催的发现原来这个工作OpenCV早已给我做好，都放在build文件夹下面的对应的staticlib目录中了，但和我自己做出来的lib文件大小不一样（估计是cmake时候的选项选择的不同）。还有一个问题就是悲催的发现OpenCV自带的静态lib文件和动态lib文件命名是相同的，这可如何在附加依赖项中选择填写啊，还是用我自己写的后缀名不同的库做测试吧（后来发现只有在配置的vc++目录里的库文件目录中添加staticlib路径，然后把该路径的顺序调整到lib路径前，就可以优先调用staticlib了）。我在我原来的程序里测试了一下，还是不能编译通过，很让我恼火，求助了高人发现是附加依赖库并没有添加完全，找到对应的3rdparty\lib文件夹下面的zlibd.lib、libjasperd.lib、libjpegd.lib、libpngd.lib、libtiffd.lib。把这几个文件copy到安装目录下，并在vs2008中配置好，这次有些可以编译通过了，但涉及到video的highgui.lib还是不能通过。查了一下，是没有调用系统库文件vfw32.lib和videoInput.lib（这个lib可以在cmake时选择是否使用），新的gui还调用了滑杆控件，还需要添加comctl32.lib这个lib，把这两个库链接时添上即可。总结一下，就是需要多添加comctl32.lib
 vfw32.lib zlibd.lib libjasperd.lib libjpegd.lib libpngd.lib libtiffd.lib opencv_core231d.lib opencv_objdetect231d.lib opencv_highgui231d.lib opencv_imgproc231d.lib，对应的release版本就把带d后缀的去掉即可。

最后再补充一下上一讲没说清楚的运行时库的问题，我的程序想在朋友电脑（没有vs开发环境）上运行，就需要运行时库的支持。除了我上一讲比较土鳖的办法（把运行时库的dll全部copy过去），还有一种办法就是在工程的属性设置里把c++ -->代码生成 --> 运行时库的对应项选好。这里介绍一下运行库（Runtime Library）。运行库是最基本的库，配合C++的语法及操作系统实现了一些基本的功能，如内存操作（new/delete等）等。可以说运行库是任何 程序，库的基础。在VC（2005以上）中有四种运行库：Multi-threaded、Multi-threaded-Debug、Multi-
 threaded-dll、Multi-threaded-debug-dll：前两个为一组，是静态类型库，提供的函数会被链接到最后的程序中，其中两 者的区别就在于一个带些调试用的信息及检查代码；后两个为一组，是动态库，最后会以动态链接库的形式（如在VC2008中为MSVCR90.dll或 MSVCR90D.dll），提供函数给程序调用。这里把MDD改为MTD（不然好像编译也无法通过）。就不需要copy过去多余的dll文件啦。

至此，只需要copy过去一个tbb_debug.dll，我的程序就能正常运行啦，我想通过cmake重新得到不嵌入tbb的opencv的lib，应该就能解决，但对于tbb是个啥东西还不懂，还是学精了再拿出来分享吧。路漫漫啊，感谢大家的阅读和支持。



