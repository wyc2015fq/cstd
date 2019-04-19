# GNU的gsl - 三少GG - CSDN博客
2010年12月09日 11:26:00[三少GG](https://me.csdn.net/scut1135)阅读数：1440
&l
t;p>
  很有可能某一天，在程序里面你会需要用到一些函数，比如 Γ 函数，β 函数，还有普通的对数吖之类的，你会想到什么，不会是＃i nclude <math.h>吧？计算机科学发展了这么多年，已经有了许许多多优秀的科学计算的函数库，涵盖了各种方面，你所要做的，就是导入它们，把参 数代入，然后等待结果，呵呵。忘掉那个又大，又慢，又难看，又难操作的MatLab吧（软件要是都象工程或者科学软件那样来开发，也就是只讲功能，根本不 管不顾使用者的习惯或者难受程度，计算机行业早就倒闭了……）
       下面的文字默认你已经熟练掌握了各种C++编译器尤其是MingW的命令行编译，当然，如果你bs windows并且在linux下面编译大规模源代码得心应手，那……
              目前号称有三大库支持科学计算，它们是GNU的gsl，blitz++以及MTL，我们这里要使用GNU的gsl来做科学计算。
If you want to use Windows and VC, suggest that you can download "gsl-1.8.exe". 
[http://sourceforge.net/projects/gnuwin32/](http://sourceforge.net/projects/gnuwin32/)
select Download GnuWin32
You can find gsl ( last version 1.8 )
Then download "gsl-1.8.exe"! 
It's so easy to install and pass the compiler...
This implementation of SIFT algorithm need "OpneCV", "GTK+ 2.0", and "GSL". 
GSL means "GNU Scientific Library".
然后类似opencv一样，将C:/GnuWin32/include/【gsl】和C:/GnuWin32/lib添加到项目属性中！
以上即可解决问题！！！下面为参考内容~
=====================================================================
“
编译他的代码需要安装OpenCV和 
GSL（http://gnuwin32.sourceforge.net/packages/gsl.htm） 
在编译时候GSL也是和OpenCV一样要把头文件和lib的路径指定好。
”
====================================================================
VC下ＧＳＬ的配置问题解决了 ![:)](http://www.opencv.org.cn/forum/images/smilies/icon_smile.gif) ＧＳＬ版本：gsl-1.8.exe
在Tools下选择Option选项，在“Include Files”下添加“C:/Program Files/GNUWIN32/Include”；“Library Files"下添加“C:/Program Files/GnuWin32/lib”　编译运行就OK了 ![:)](http://www.opencv.org.cn/forum/images/smilies/icon_smile.gif) 曾经为此困扰，特写下来，希望对后来人有帮助 ![:)](http://www.opencv.org.cn/forum/images/smilies/icon_smile.gif)
=====================================================================
### [Re: Rob Hess的SIFT算法的C语言实现（基于OpenCV）](http://www.opencv.org.cn/forum/viewtopic.php?f=10&t=1472&sid=a0d009010aa3dce616b46e33b48c5dee&start=25)
![帖子](http://www.opencv.org.cn/forum/styles/prosilver/imageset/icon_post_target.gif)由 **[watermoon](http://www.opencv.org.cn/forum/memberlist.php?mode=viewprofile&u=5532)** » 2008-12-18 14:20
首先谢谢楼主的分享。
我也说一下我编译通过的过程。
刚下载下来就编译的话，提示缺少gsl.h文件，按照楼上
senna提供的网址下载gsl 
[http://sourceforge.net/projects/gnuwin32/](http://sourceforge.net/projects/gnuwin32/)
直接点右上角的下载，下拉找到gsl，安装即可。
然后在工程中加入include和lib目录。
例如我的是XP和VS2005则在：
工具－选项－工程和解决方案，然后分别加入（注意：默认是执行目录，
要下拉为include 和 lib 目录 ）
include 和 lib 路径（gsl 的安装路径下的include 和lib 目录)。
