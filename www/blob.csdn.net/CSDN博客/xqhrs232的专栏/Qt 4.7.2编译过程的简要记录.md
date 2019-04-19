# Qt 4.7.2编译过程的简要记录 - xqhrs232的专栏 - CSDN博客
2016年06月12日 19:06:50[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：379
个人分类：[X11/Qt/Qt quick/Qxml界面技术](https://blog.csdn.net/xqhrs232/article/category/6455019)

    编译Qt 4.7.2还是折腾了不少时间，这里做个简单的记录了！！！
1>QT源代码使用的版本----qt-everywhere-opensource-src-4.7.2.tar.gz
2>编译前需要安装的软件包------如果不去安装这些软件包，直接去配置编译就会报缺少下面这些包的错误！！！
    //1111----sudo apt-get install  g++
    //2222----sudo apt-get install libX11-dev
    //3333----sudo apt-get install libXext-dev 
    //4444----sudo apt-get install libXtst-dev
    //5555----apt-get install libxrandr-dev---------------------------------------------------不安装报------/usr/bin/ld: cannot find -lXrender
3>编译的过程
    //1111----解压源代码
    //2222----configure-------------------这个有很多配置项的，但都没去设置
    //3333----make
   //4444----make install
   //5555----qmake -v   测试
   //6666----qmake 编译一个应用程序
 4>编译过程遇到的坑
   //1111----ln: creating symbolic link XXXXXX : Operation not supported--------不把文件放windows与linux共享的文件夹下编译就不会产生这样的错误！！！
   //2222----/usr/bin/ld: cannot find -lXrender----apt-get install libxrandr-dev 
   //3333----/usr/local/Trolltech/Qt-4.7.2/include/QtCore/qstring.h:60:22: fatal error: string: No such file or directory
 #    include <string> ^ compilation terminated.
-----原来是应用程序存成了c文件格式，改称cpp的后缀就好

