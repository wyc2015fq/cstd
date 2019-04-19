# VS2005下编译LLMozLib - 逍遥剑客 - CSDN博客
2008年01月20日 23:09:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2867标签：[mozilla																[firefox																[cvs																[build																[ps3																[patch](https://so.csdn.net/so/search/s.do?q=patch&t=blog)](https://so.csdn.net/so/search/s.do?q=ps3&t=blog)](https://so.csdn.net/so/search/s.do?q=build&t=blog)](https://so.csdn.net/so/search/s.do?q=cvs&t=blog)](https://so.csdn.net/so/search/s.do?q=firefox&t=blog)](https://so.csdn.net/so/search/s.do?q=mozilla&t=blog)
个人分类：[乱七八糟](https://blog.csdn.net/xoyojank/article/category/259560)
1.下载Mozilla build tools,解压到c:/mozilla-build
[http://ftp.mozilla.org/pub/mozilla.org/mozilla/libraries/win32/MozillaBuildSetup-1.1.exe](http://ftp.mozilla.org/pub/mozilla.org/mozilla/libraries/win32/MozillaBuildSetup-1.1.exe)
2.从CVS上检出FireFox到c:/mozilla
- CVS      :pserver:anonymous@cvs-mirror.mozilla.org:/cvsroot 
      MODULE   mozilla 
      TAG      FIREFOX_2_0_RELEASE
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/Mozilla-CVS.JPG)
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/Mozilla-CVS-TAG.JPG)
3.下载LLMozLib的源码
[http://s3.amazonaws.com/callum-linden/llmozlib_win_src_2006_11_06.zip](http://s3.amazonaws.com/callum-linden/llmozlib_win_src_2006_11_06.zip)
4.拷贝编译配置文件到c:/mozilla
如果是DEBUG模式,则把llmozlib_win_src_2006_11_06/build_mozilla/.mozconfig.debug拷贝过去,RELEASE则拷贝llmozlib_win_src_2006_11_06/build_mozilla/.mozconfig.optimized;
并且把文件名改为”.mozconfig”.因为windows下不能直接创建以.开头的文件,所以要用命令行改名(我用的TotalCommander可以直接改^_^)
5.启动start-msvc8.bat(需要系统管理员权限)
6.把当前目录定位到c:/mozilla
执行 cd /c/Mozilla
7.把LLMozLib带的补丁打上:
       patch -p0 < linden_updates_2006_10_13.patch
8.编译mozilla
       make -f client.mk build
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/LLMozLib-Build.JPG)
9.编译过程中会有几个exe提示出错,这是因为缺少.manifest文件导致的.可以在C:/mozilla/objdir-debug-xulrunner-small/(DEBUG模式)或C:/mozilla/objdir-opt-xulrunner-small/(RELEASE模式)下搜索.manifest和.exe,并把缺少.manifest的拷贝到同主文件名的.exe的目录下,如此几次就可以通过编译了……
PS1:这个过程很漫长,中间还会把内存占用完,弄得我还以为机器死机了呢
PS2:最后mingw竟然显示有2errors,把我给骗了,实际上已经编译成功了
PS3:DEBUG和RELEASE都编译过后,这个文件夹的大小很恐怖…
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/Mozilla-Disk-Space.JPG)
10.拷贝LLMozLib所需的文件到工程目录
这个简单,执行llmozlib_win_src_2006_11_06/build_mozilla/copy_mozilla_files.bat就OK啦
11.你可以打开VS2005编译LLMozLib和相应的工程了.不过ubrowser带的库文件VS2005链接会出问题
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/LLMozLib-VS2005.JPG)
