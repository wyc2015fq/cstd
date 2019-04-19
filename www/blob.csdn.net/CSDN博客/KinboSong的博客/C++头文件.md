# C++头文件 - KinboSong的博客 - CSDN博客
2017年04月02日 12:12:05[songjinbo3](https://me.csdn.net/KinboSong)阅读数：279
                
C++中#include包含头文件带 .h 和不带 .h 的区别？
 如 #include <iostream> 和 #include <iostream.h> 包含的东西有哪些不同？
 之前在写C++程序的时候只知道使用 #include <iostream> 的时候，使用函数前要用 using namespace std; 导入命名空间，而 #include <iostream.h> 则不用，这个得看C++标准化过程为C++开发者做了哪些有意义的工作。
C++标准化过程中，其中一个环节，解决了以下问题：
（1）C++增加了名称空间概念，借以将原来声明在全局空间下的标识符声明在了namespace std下。
（2）统一C++各种后缀名，如.h、.hpp、.hxx等。标准化之前的头文件就是带后缀名的文件，标准化后的头文件就是不带后缀名的文件。C++ 98 规定用户应使用新版头文件，对旧版本头文件不在进行强制规范，但大多数编译器厂商依然提供旧版本头文件，以求向下兼容。
 也就是说带 .h 的头文件是旧标准的，如果想用新的标准的头文件就不要带 .h。
 另外，为了和C语言兼容，C++标准化过程中，原有C语言头文件标准化后，头文件名前带个c字母，如cstdio、cstring、ctime、ctype等等。这些头文件都可以在 C:\Program Files\Microsoft Visual Studio 10.0\VC\include 这个目录下找到（以VC2010为例）。也就是说，我们如果要用C++标准化了的C语言头文件，就得作如下的转换
#include <stdio.h> --> #include <cstdio>
 #include <stdlib.h> --> #include <cstdlib>
 #include <string.h> --> #include <cstring>
还要提及的一点是，我在看C++标准库的时候，看到一个特殊情况 <memory.h> 和 <memory>，这两个头文件是完全不同的，因为我发现 <memory.h>头文件件包含了 <mem.h>；而 <memory> 包含 <memory.stl>
            
