# Windows下用Visual Studio来build ImageMagick - DoubleLi - 博客园






参考：

[http://www.imagemagick.org/script/install-source.php#windows](http://www.imagemagick.org/script/install-source.php#windows)

[http://blog.163.com/anteaus_20/blog/static/24422224200811924810941/](http://blog.163.com/anteaus_20/blog/static/24422224200811924810941/)



【Build过程】

1）根据[ImageMagick在Windows下的安装指南](http://http//www.imagemagick.org/script/install-source.php#windows)，在[这里](http://www.imagemagick.org/download/windows/ImageMagick-windows.zip)下载源码包（以6.8.9为例），解压，进入解压后的目录。

2）在Visual Studio（以VS2008为例）中，打开 ImageMagick-6.8.9/VisualMagick/configure 中的 configure.sln，如果提示转换格式，就转。然后 Build->Build Solution。

3）会在 ImageMagick-6.8.9\VisualMagick\configure 中出现 configure.exe，双击运行

![](http://img.blog.csdn.net/20140623221816171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWFzaV94aQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

4）点击“下一步”

![](http://img.blog.csdn.net/20140623221911781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWFzaV94aQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

5）选择"Static Multi-threaded DLL runtime"，然后下一步直到结束。会在 ImageMagick-6.8.9\VisualMagick 下生成 VisualStaticMTDLL.sln。

6）在Visual Studio 中打开 VisualStaticMTDLL.sln，编译整个solution。



【一处编译错误】

在编译过程中，CORE_pango工程报下面的编译错误：

![](http://img.blog.csdn.net/20140623221104937?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWFzaV94aQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

【解决】

所有报“newline in constant”错误的地方，删除出错行的字符串，填充一个ASCII字符串，比如 "..."，重新编译。



【Build结果】

最终会在 ImageMagick-6.8.9\VisualMagick\bin 下生成很多exe文件、lib、dll文件。其中包括下面要用到的** convert.exe**。

在 ImageMagick-6.8.9\VisualMagick\lib 下生成一堆.lib文件。



【[测试](http://lib.csdn.net/base/softwaretest)】

按照 [[ImageMagick 学习] Fred's ImageMagick Scripts 转 C++ 的统一解决方案](http://blog.csdn.net/yasi_xi/article/details/8887175) 帖子中的例子，对下图 raw_text.jpg 做 消除背景噪声 的处理。

![](http://img.blog.csdn.net/20140623222454750?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWFzaV94aQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

将 convert.exe 和 raw_text.jpg 都拷贝到一个目录，比如 D:/ 下，在该目录下进入命令行，执行下面的命令





**[plain]**[view plain](http://blog.csdn.net/yasi_xi/article/details/33793913#)[copy](http://blog.csdn.net/yasi_xi/article/details/33793913#)



- convert ( raw_text.jpg -colorspace gray -type grayscale -contrast-stretch 0 ) ( -clone 0 -colorspace gray -negate -lat 25x25+10% -contrast-stretch 0 ) -compose copy_opacity -composite -fill "white" -opaque none +matte -deskew 40% -sharpen 0x1 out.jpg  






将会得到下面的 out.jpg

![](http://img.blog.csdn.net/20140623222653437?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWFzaV94aQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)









