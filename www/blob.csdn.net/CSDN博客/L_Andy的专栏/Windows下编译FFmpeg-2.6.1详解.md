# Windows下编译FFmpeg-2.6.1详解 - L_Andy的专栏 - CSDN博客

2017年10月05日 11:24:31[卡哥](https://me.csdn.net/L_Andy)阅读数：2973


## Windows下编译FFmpeg-2.6.1详解

          在诸多网友帮助下终于搞定了FFmpeg V2.6.1，由于编译环境和程序版本的不同，造成了很多不必要的时间浪费，特在此将编译过程和遇到的问题解决方法写出来，以便方便大家。

          编译环境:PC Windows7 64bit、编译工具:MinGW+MSYS+yasm  编译软件:ffmpeg-snapshot-git.tar.bz2

## 一、下载FFmpeg

               FFMPEG官网：[http://ffmpeg.org/download.html](http://ffmpeg.org/download.html)

               FFmpeg SVN：svn://svn.mplayerhq.hu/ffmpeg/trunk

               FFmpeg GIT:    git://source.ffmpeg.org/ffmpeg.git ffmpeg

由于FFmpeg的SVN代码停止了更新，所以从 svn://svn.mplayerhq.hu/ffmpeg/trunk 上Down下来的代码编译后与在官网上下载的头文件不匹配！所以只能从git上下载或下载git压缩包！

               我的下载步骤：打开[http://ffmpeg.org/download.html](http://ffmpeg.org/download.html)

![](https://img-blog.csdn.net/20150409162059914?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhvdXlvbmdrdQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

        下载完ffmpeg-snapshot-git.tar.bz2后解压到D:\work\code\FFmpeg 路径任意(最好为英文,方便MinGW操作)

## 二、下载MinGW+msys

             MinGW - Minimalist GNU for Windows-是Windows下模拟linux运行的库、头文件等的集合。

             msys-Minimal SYStem-是Windows下模拟linux运行的最新系统，可以通过类似于linux bash的命令行窗口进行代码编译。

             登陆 [http://sourceforge.net/projects/mingw/files/](http://sourceforge.net/projects/mingw/files/)

![](https://img-blog.csdn.net/20150409163107236?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhvdXlvbmdrdQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20150409163433112?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhvdXlvbmdrdQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

下载完成之后点击安装 关键是选择一个路径C:\MinGW作为安装路径，直接下一步就可以

![](https://img-blog.csdn.net/20150409163838716?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhvdXlvbmdrdQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20150409163808377?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhvdXlvbmdrdQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20150409163847468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhvdXlvbmdrdQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


最后点击continue进行安装完毕确认 并进入下载环节

![](https://img-blog.csdn.net/20150409164219207?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhvdXlvbmdrdQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

              点击左边的列表框，选中 Basic Setup，在右侧会出现一些必要的下载项，全选即可。

![](https://img-blog.csdn.net/20150409164445866?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhvdXlvbmdrdQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

            选择完毕后点击菜单Installation->Apply Changes 开始下载

![](https://img-blog.csdn.net/20150409164704987?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhvdXlvbmdrdQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20150409164810023?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhvdXlvbmdrdQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20150409165010593?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhvdXlvbmdrdQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## 三、配置MinGW

         MinGW下载完成后进入安装目录，如果是C:\MinGW 则目录为C:\MinGW\msys\1.0

         用记事本打开msys.bat文件，在第一行加入：

call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\vcvars32.bat"

         call对应的目录为你的VC安装程序所在目录

![](https://img-blog.csdn.net/20150409165332024?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhvdXlvbmdrdQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## 四、下载yasm

     ffmpeg编译的时候需要yasm.exe   打开[http://yasm.tortall.net/Download.html](http://yasm.tortall.net/Download.html)    下载与你版本相匹配的yasm
 我用的是win764位 所以下载的是 [Win64 .exe](http://www.tortall.net/projects/yasm/releases/yasm-1.3.0-win64.exe) (for
 general use on 64-bit Windows) 如果你的32位系统请下载32位的程序

![](https://img-blog.csdn.net/20150409165938250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhvdXlvbmdrdQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## 五、配置yasm

将下载好的yasm-1.3.0-win64.exe 或者 yasm-1.3.0-win32.exe 修改为 yasm.exe并拷贝到c:\windows\system32文件夹下--这样不需要做过多的环境变量配置

## 六、编译FFmpeg

###             1、运行软件

###                       运行C:\MinGW\msys\1.0\msys.bat 在弹出窗口中进入ffmpeg的代码解压路径

                                  cd   D:\work\code\FFmpeg

###           2、配置FFmpeg makefile

                     输入   ./configure --enable-shared --disable-static --enable-memalign-hack

###            3、编译FFmpeg

 make

ffmpeg生成的文件在各自的目录下，可以通过windows自带的查找文件功能 搜索*.dll和*.lib 导出这些生成库

## 七、编译时问题

             如果编译的是老版本的FFmpeg 则会出现以下这些问题

###             1、gcc is unable to create an executable file.

这是由于gcc编译器没有准备好，应该是安装过程中出了错误，重装一下。

###           2、error: static declaration of 'lrint' follows non-static declaration

### ../libavutil/libm.h:62: error: static declaration of 'lrint' follows non-static declaration../libavutil/libm.h:69: error: static declaration of 'lrintf' follows non-static declaration../libavutil/libm.h:76: error: static declaration of 'round' follows non-static declaration../libavutil/libm.h:83: error: static declaration of 'roundf' follows non-static declaration

        将ffmpeg代码路径下的config.h中的lrint、lrintf、round、roundf等定义为1

###          3、implicit declaration of function 'strcasecmp'

         这个问题是由于MinGW的环境中没有strcasecmp函数造成，可以通过自己编写代码加入到工程中，也可以自己编写代码保存成文件放到MinGW的include目录下。

         将下列代码存储成strcasecmp.h于mingw include目录下。

将这个文件include进avstring.h和metadata.h中，问题解决。

[cpp][view
 plain](http://blog.csdn.net/zhouyongku/article/details/44961447#)[copy](http://blog.csdn.net/zhouyongku/article/details/44961447#)

- #ifndef __STRCASECMP_H____
- #define __STRCASECMP_H____
- 
- staticinlinechar __hack_charget( char c )  
- {  
- if(c >= 'a' && c <= 'z')  
-         c += 'A' - 'a';  
- return c;  
- }  
- 
- staticinlineint hack_strcasecmp( charconst *a, charconst *b )  
- {  
- char ac, bc;  
- int r;  
- for(;;) {  
-         ac = __hack_charget(*a++);  
-         bc = __hack_charget(*b++);  
-         r = (int)ac - (int)bc;  
- if(r)  
- return r;  
- 
- if(!ac)  
- return 0;  
-     }  
- }  
- 
- staticinlineint hack_strncasecmp( charconst *a, charconst *b, int n )  
- {  
- char ac, bc;  
- int r, i;  
- 
- for(i = 0; i < n; ++i) {  
-         ac = __hack_charget(*a++);  
-         bc = __hack_charget(*b++);  
-         r = (int)ac - (int)bc;  
- if(r)  
- return r;  
- 
- if(!ac)  
- return 0;  
-     }  
- return 0;  
- }  
- 
- #define strcasecmp hack_strcasecmp
- #define strncasecmp hack_strncasecmp
- 
- #endif

###         4、-Werror=implicit-提示

这个表示将隐式声明函数错误，可以通过修改FFmpeg目录下通过./configure生成的config.mak文件，将Werror相关的地方删除即可。如：

-Werror=format-security -Werror=implicit-function-declaration -Werror=missing-prototypes -Werror=return-type -Werror=vla  则删除-Werror=implicit-function-declaration

 引用：[http://blog.sina.com.cn/s/blog_4ae178ba01010353.html](http://blog.sina.com.cn/s/blog_4ae178ba01010353.html)

[http://blog.chinaunix.net/uid-20718335-id-2980793.html](http://blog.chinaunix.net/uid-20718335-id-2980793.html)

[http://www.xuebuyuan.com/1428648.html](http://www.xuebuyuan.com/1428648.html)

