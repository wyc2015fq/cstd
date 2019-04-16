# 转载和积累系列 - win下安装c语言开发环境 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2014年01月15日 11:47:14[initphp](https://me.csdn.net/initphp)阅读数：1166
个人分类：[转载和积累系列](https://blog.csdn.net/initphp/article/category/2609089)









## 安装Mingw




### MinGW 是什么

MinGW 提供了一套简单方便的Windows下的基于GCC 程序开发环境。MinGW 收集了一系列免费的Windows 使用的头文件和库文件；同时整合了GNU ([http://www.gnu.org/](http://www.gnu.org/) )的工具集，特别是GNU 程序开发工具，如经典gcc, g++, make等。MinGW是完全免费的自由软件，它在Windows平台上模拟了Linux下GCC的开发环境，为C++的跨平台开发提供了良好基础支持，为了在Windows下工作的程序员熟悉Linux下的C++工程组织提供了条件。




### 下载和安装

下载地址：http://sourceforge.net/projects/mingw/files/

![](https://img-blog.csdn.net/20140115114121546?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下载这个安装文件，安装即可。这个是一个在线安装c c++等环境的安装工具。安装成功后，有一个可视化界面可以选择安装需要的软件，但是我们这边只安装gcc的环境，所以我只下载gcc就可以了。个人比较喜欢命令行的安装方式。

首先在window cmd命令下进入D:\MinGW\bin bin目录。

可以看到有一个mingw-get.exe文件，这个就是用来安装环境的命令。

执行命令：



```php
mingw-get udgrade
mingw-get install gcc
```


然后需要配置环境变量：

1)、在PATH里加入D:\MinGW\bin，记得，如果里面还有其他的变 量，记得要加个分号啊，分号得在英文输入模式下输入的。

2)、新建LIBRARY_PATH变量，如果有的话，在值中加入D:\MinGW \lib，这是标准库的位置。

3)、新建C_INCLUDEDE_PATH变量，值设为D\MinGW\include。





配置完成后，打开cmd ,输入gcc -v

![](https://img-blog.csdn.net/20140115123534843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




## Eclipse开发c程序

1. 新建一个c语言的项目

2. 运行之前必须要先构建一下运行环境

3. 然后运行







