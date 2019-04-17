# 《Linux程序设计》——入门 - 一世豁然的专栏 - CSDN博客





2014年09月17日 21:20:22[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1187








一、UNIX、linux和GNU

 1、UNIX

  1）、UNIX简史

  2）、UNIX哲学




 2、Linux




 3、GNU项目和自由软件基金会

  1）、GNU主要项目软件

    A、GCC：GNU编译器集，包括GNU C编译器

    B、G++：C++编译器，是GCC的一部分

    C、GDB：源代码级的调试器。

    D、GNU make：UNIXmake命令的免费版本。

    E、Bison ：与UNIX vacc兼容的语法分析程序生成器。


    F、bash：命令解释器（shell）。

   G、GNU Emacs：文本编译器及环境。




二、Linux程序设计

 1、设计语言

   1）、P4 表1-1

  2、Linux程序

   1）、特殊文件：可执行文件和脚本文件（Linux并不要求可执行文件和脚本文件具有特殊的文件名或脚本名）

   1）、搜索路径，通常包含存储系统的标准路径。

     I、/bin：二进制文件目录，用于存放启动系统时用到的程序。

     II、/usr/bin:用户二进制文件目录，用于存放用户使用的标准程序。

    III、/usr/local/bin:本地二进制文件目录，用于存放软件安装的程序。




 3、文本编译器（emacs、vim、vi）




 4、C语言编译器（GCC）




三、开发系统导引

  1、应用程序

   1）、/usr/bin:系统为正常使用提供的程序，包括用于程序开发的工具。

   2）、/usr/local/bin或/opt：系统管理员为某个特定的主机或本地网络增加的程序。

   3）、/usr/X11或/usr/bin/X11：X视窗系统。

   4）、/usr/bin或/usr/local/bin或usr/lib/gcc:驱动程序GCC。

  2、头文件

    1）、/usr/inlude:头文件

    2）、依赖于特定Linux版本的头文件：/usr/inlude/sys和/usr/inlude/linux

    3）、其他编程系统也有各自的头文件，这些头文件被存储在可被相应编译器自动搜索到目录里。

 3、库文件

    I、标准系统库文件一般存储在/lib和/usr/lib目录中。

    II、.a代表传统的静态函数库，.so代表共享函数库。

 4、共享库


 5、静态库







四、获取帮助（man info）
            


