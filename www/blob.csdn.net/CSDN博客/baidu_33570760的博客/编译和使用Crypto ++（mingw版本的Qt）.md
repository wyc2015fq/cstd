# 编译和使用Crypto ++（mingw版本的Qt） - baidu_33570760的博客 - CSDN博客
2017年07月07日 17:35:23[carman_风](https://me.csdn.net/baidu_33570760)阅读数：916
原文地址：http://www.qtcentre.org/threads/28809-Compiling-amp-using-Crypto-with-mingw-version-of-Qt
我个人对这些很麻烦。
显式编译的crypto ++版本（cryptopp530win32win64.zip）是使用MSVC构建的，不能与mingw一起使用。
幸运的是我可以让它最终工作。
所以我也告诉你，一步一步，怎么做。
首先下载cryptopp552.zip（crypto ++ v5.5.2源）
为什么cryptopp552.zip？显然这是用mingw成功编译的最新版本。
提取cryptopp552.zip的内容到C：\ cryptopp552
编辑C：\ cryptopp552 \ fipstest.cpp，并用'OutputDebugStringA'替换每个'OutputDebugString'。
（共3个替换）
别忘了保存吧！
删除C：\ cryptopp552 \ GNUmakefile
打开Qt命令提示符（我用Qt SDK 2009.05）
在Qt命令行输入以下命令：
C：
cd \ cryptopp552
qmake -project
打开cryptopp552.pro（现在在C：\ cryptopp552中创建）
在里面：
将TEMPLATE = app更改为TEMPLATE = lib
最后添加一行包含LIBS + = -lws2_32。
在Qt命令行中键入以下命令：
QMAKE
mingw32-make all
等待构建过程完成（可能需要很多分钟）
现在我们应该在目录C：\ cryptopp552 \ release和C：\ cryptopp552 \ debug中有名为libcryptopp552.a和cryptopp552.dll的文件
将C：\ cryptopp552 \ release \ libcryptopp552.a复制到<Qt dir> \ lib
请注意，在Qt SDK安装目录中还有另一个名为lib一级的目录。所以不要混淆他们。
将C：\ cryptopp552 \ release \ cryptopp552.dll复制到<Qt dir> \ bin
请注意，在Qt SDK安装目录中还有另一个名为bin一级的目录。所以不要混淆他们。
在<Qt dir> \ include中创建一个名为cryptopp的目录。
将所有头文件（.h）从C：\ cryptopp552复制到<Qt dir> \ include \ cryptopp。
使用的时候 引入对应的头文件
记住，在开始构建它之前，应该将这些行添加到其.pro文件中：
LIBS + = -lcryptopp552
