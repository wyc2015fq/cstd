# Ubuntu16 64位安装android studio的32位库 sudo apt-get install lib32bz2-1.0失败 - YZXnuaa的博客 - CSDN博客
2018年04月23日 18:14:21[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：225
个人分类：[ubuntu操作](https://blog.csdn.net/YZXnuaa/article/category/7388698)
Ubuntu16 64位安装android studio时需要使用以下命令安装一些 32 位库：
**[plain]**[view plain](https://blog.csdn.net/ljttianqin/article/details/77367759#)[copy](https://blog.csdn.net/ljttianqin/article/details/77367759#)
- sudo apt-get install lib32z1 lib32ncurses5 lib32bz2-1.0 lib32stdc++6  
但安装时出现错误：
**[plain]**[view plain](https://blog.csdn.net/ljttianqin/article/details/77367759#)[copy](https://blog.csdn.net/ljttianqin/article/details/77367759#)
- E: Unable to locate package lib32bz2-1.0  
- E: Couldn't find any package by glob 'lib32bz2-1.0'  
- E: Couldn't find any package by regex 'lib32bz2-1.0'  
解决方法，使用以下命令进行安装：
**[plain]**[view plain](https://blog.csdn.net/ljttianqin/article/details/77367759#)[copy](https://blog.csdn.net/ljttianqin/article/details/77367759#)
- sudo apt-get install lib32z1 lib32ncurses5 libbz2-1.0:i386 lib32stdc++6  
参考：
1、[https://developer.android.com/studio/install.html](https://developer.android.com/studio/install.html)
2、[https://askubuntu.com/questions/637113/unable-to-locate-package-lib32bz2-1-0](https://askubuntu.com/questions/637113/unable-to-locate-package-lib32bz2-1-0)
