# vs2015用cmake编译opencv问题总结 - qq229873466的博客 - CSDN博客

2016年10月23日 21:33:56[qq229873466](https://me.csdn.net/qq229873466)阅读数：7447


1.直接用cmake点击configure会出现以下错误：

The CXX compiler identification is unknown

The C compiler identification is unknown

CMake Error at CMakeLists.txt:93 (project):

No CMAKE_CXX_COMPILER could be found.

正确应该进入vs2015->工具->visual studio 命令提示

![](https://img-blog.csdn.net/20161023213233271?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

进入以后直接输入cmake-gui.exe的全路径打开，之后就不会出现问题了

2.用vs2015配置opencv2.4.10，然后调用官方的build里面的文件，当运行cout输出矩阵时会出现错误

![](https://img-blog.csdn.net/20161024201454103?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

感觉官方是用vs2010编译生成才会出错，解决方法是自己用cmake选择平台vs2015编译生成新的编译文件再运行就不会出错了。

3.下载的opencv3.10进行编译通常需要下载opencv_ffmepg.dll等几个文件，因为cmake是这么写的- -，这里有链接方便下载[http://download.csdn.net/detail/panda1234lee/9656265](http://download.csdn.net/detail/panda1234lee/9656265)。

