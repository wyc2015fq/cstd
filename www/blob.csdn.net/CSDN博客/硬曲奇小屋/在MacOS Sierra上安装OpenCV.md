# 在MacOS Sierra上安装OpenCV - 硬曲奇小屋 - CSDN博客





2017年10月17日 19:32:20[HardCookies](https://me.csdn.net/james_616)阅读数：800








上次在Mac上配置OpenCV还是本科做毕设的时候，但是这次安装遇到问题。首先下载opencv的源码，然后用安装包安装Cmake。 

按照opencv官网的[安装教程](https://docs.opencv.org/master/d7/d9f/tutorial_linux_install.html)执行cmke指令: 
`cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local ..`

报错说没有cmake，参考Cmake - Tools - How to Install for Command Line Use. 

![这里写图片描述](https://img-blog.csdn.net/20171017163800615?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


以上三种方式都可以，我们采取第二种。输入 
`sudo "/Application/Cmake.app/Contents/bin/cmake-gui" --install`

之后，cmake指令就可以使用了。下面进行build，执行 
`make -j8`

终端开始跑，之后就会遇到问题！！！
```
/Users/James/Downloads/opencv-3.1.0/modules/videoio/src/cap_qtkit.mm:46:9: fatal error: 'QTKit/QTKit.h' file not found
#import <QTKit/QTKit.h>
        ^
1 error generated.
make[2]: *** [modules/videoio/CMakeFiles/opencv_videoio.dir/src/cap_qtkit.mm.o] Error 1
make[1]: *** [modules/videoio/CMakeFiles/opencv_videoio.dir/all] Error 2
make: *** [all] Error 2
```

错误原因是无法找到’QTKit/QTKit.h’，有人在[博客](http://blog.csdn.net/ph1de2/article/details/52958543)中提到

> 
Apple的API Reference QTKit 提到OS X v10.9后的系统对QTKit.framework将不做支持,替代框架为AVFoundation .


所以无法找到，解决方案是用brew安装，并加上option选项为HEAD。只能放弃之前的安装方式，换做brew安装。 
`brew install opencv3 --HEAD`

然而出现错误：`Error: No head is defined for opencv`

需要更新到science库 
`brew tap homebrew/science`

再`brew install opencv3 --HEAD`一下，还是不行。 

查一下opencv3的信息 
`brew info opencv3`

会显示需要安装的组件
```
opencv: stable 3.3.0 (bottled)
Open source computer vision library
http://opencv.org/
Not installed
From: https://github.com/Homebrew/homebrew-core/blob/master/Formula/opencv.rb
==> Dependencies
Build: cmake ✘, pkg-config ✔
Required: eigen ✘, ffmpeg ✘, jpeg ✘, libpng ✘, libtiff ✘, openexr ✘, numpy ✘
==> Requirements
Required: python ✔, python3 ✔
```

有些我没有安装，最后发现HEAD实在多此一举。直接`brew install opencv3`就可以了。开始下载需要的依赖 

==> Installing dependencies for opencv: sqlite, python3, python, eigen, lame, x264, xvid, ffmpeg, jpeg, libpng, libtiff, ilmbase, openexr, numpy 

最后安装成功： 
![这里写图片描述](https://img-blog.csdn.net/20171017192521336?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

历经千辛万苦，终于修得正果。 

所以，最简单的安装就是直接`brew install opencv3`. 






