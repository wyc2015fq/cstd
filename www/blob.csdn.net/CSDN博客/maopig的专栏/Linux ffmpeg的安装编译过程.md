# Linux ffmpeg的安装编译过程 - maopig的专栏 - CSDN博客
2011年07月05日 20:19:34[maopig](https://me.csdn.net/maopig)阅读数：2742
### [Linux ffmpeg的安装编译过程](http://blog.csdn.net/sunjing/article/details/5457474)
　　1、下载ffmpeg。
　  　  在网上搜索一下,或者到官方网站下载
　　2、解压
　　   tar命令解压
　　3、配置
　　./configure --enable-shared --prefix=/usr/local/ffmpeg
　　其中：--enable-shared 是允许其编译产生动态库，在以后的编程中要用到这个几个动态库。--prefix设置的安装目录。
　　4、编译并安装
　　make
　　make install
　　5、安装之后在/usr/local/ffmpeg会看到有三个目录
　　lib 动态链接库位置
　　include 编程要用到头文件
　　bin 执行文件所在的目录
　　6、为了以后方便编程，我们把lib中的三个链接库libavcodec.so libavformat.so libavutil.so复制到/usr/lib下。把include目录下的ffmpeg目录复制到/usr/include下。
　　执行bin目录下的ffplay，可以去播放音频或者视频文件。例如播放1.mp3
　　./ffplay 1.mp3
　　另外，bin目录下还有两个文件:ffmpeg和ffserver
　　ffmpeg是一个很好的视频和音频的格式转化工具。网上有很多它的说明文档。如果不想生成ffserver，只要在./configure的时候加--disable-ffserver即可。
　　7、编程
　　如果写了一个test.c文件，要包含ffmpeg的头文件，可以这样写：
　　#include 
　　编译：gcc -o test test.c -lavformat -lavcodec -lavtuil (前提是有第6步的操作)
　　如果没有第6部的操作，则编译的时候如下：
　　gcc -o test test.c -I/usr/local/ffmpeg/include -L/usr/local/ffmpeg/lib -lavformat -lavcodec -lavtuil
　　编译成功之后，执行的时候还是需要动态库的支持，还是要把那三个动态库文件复制到/usr/lib或者/lib中，不然执行的时候会说找不到动态库链接。还有一个方法可以解决这个问题，就是把/usr/local/ffmpeg/lib这个目录加入到/etc/ld.so.config中，然后执行ldconfig，或者重启电脑，这样执行的时候系统就可以从/usr/local/ffmpeg/lib这个目录下去找这三个动态库文件了。
　　以上的方式是采用动态库编译ffmpeg的，如果在configure的时候不加上--enable-shared的，则采用静态链接的方式，不会生成那三个动态库。同时生成的ffplay、ffmpeg的执行文件也比较的大，因为他们不需要动态库的支持，就可以执行。但是不利于再次开发，所以我采用动态链接的方式。configure中还有很多的选项，可以通过./configure --help查看，也可以直接查看configure文件。这在配置的时候很重要。
