# Makefile - 三少GG - CSDN博客
2010年09月24日 13:54:00[三少GG](https://me.csdn.net/scut1135)阅读数：867标签：[makefile																[编译器																[fortran																[工具																[macros																[unix](https://so.csdn.net/so/search/s.do?q=unix&t=blog)](https://so.csdn.net/so/search/s.do?q=macros&t=blog)](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=fortran&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=makefile&t=blog)
个人分类：[Ubuntu/OS X系统](https://blog.csdn.net/scut1135/article/category/680808)
# Makefile
Makefile文件 Makefile 一个工程中的源文件不计数，其按类型、功能、模块分别放在若干个目录中，makefile定义了一系列的规则来指定，哪些文件需要先编译，哪些文件需要后编译，哪些文件需要重新编译，甚至于进行更复杂的功能操作，因为 makefile就像一个Shell脚本一样，其中也可以执行操作系统的命令。
## 自动化编译
makefile带来的好处就是——“自动化编译”，一旦写好，只需要一个make命令，整个工程完全自动编译，极大的提高了软件开发的效率。make是一个命令工具，是一个解释makefile中指令的命令工具，一般来说，大多数的IDE都有这个命令，比如：Delphi的make，Visual C++的nmake，Linux下GNU的make。可见，makefile都成为了一种在工程方面的编译方法。
[编辑本段](http://baike.baidu.com/view/974566.htm#)
主要功能
Make工具最主要也是最基本的功能就是通过makefile文件来描述源程序之间的相互关系并自动维护编译工作。而makefile 文件需要按照某种语法进行编写，文件中需要说明如何编译各个源文件并连接生成可执行文件，并要求定义源文件之间的依赖关系。makefile 文件是许多编译器--包括 Windows NT 下的编译器--维护编译信息的常用方法，只是在集成开发环境中，用户通过友好的界面修改 makefile 文件而已。　　在 UNIX 系统中，习惯使用 Makefile 作为 makfile 文件。如果要使用其他文件作为 makefile，则可利用类似下面的 make 命令选项指定 makefile 文件：
[编辑本段](http://baike.baidu.com/view/974566.htm#)
$ make -f Makefile.debug
例如，一个名为prog的程序由三个C源文件filea.c、fileb.c和filec
[](http://baike.baidu.com/image/a8ad9413c4f60de9f6039e28)![](http://imgsrc.baidu.com/baike/abpic/item/a8ad9413c4f60de9f6039e28.jpg)
Makefile文件工程样例
.c以及库文件LS编译生成，这三个文件还分别包含自己的头文件a.h 、b.h和c.h。通常情况下，C编译器将会输出三个目标文件filea.o、fileb.o和filec.o。假设filea.c和fileb.c都要声明用到一个名为defs的文件，但filec.c不用。即在filea.c和fileb.c里都有这样的声明：　　#include "defs"　　那么下面的文档就描述了这些文件之间的相互联系:　　---------------------------------------------------------　　#It is a example for describing makefile　　prog : filea.o fileb.o filec.o　　cc filea.o fileb.o filec.o -LS -o prog　　filea.o : filea.c a.h defs　　cc -c filea.c　　fileb.o : fileb.c b.h defs　　cc -c fileb.c　　filec.o : filec.c c.h　　cc -c filec.c　　----------------------------------------------------------　　这个描述文档就是一个简单的makefile文件。　　从上面的例子注意到，第一个字符为 # 的行为注释行。第一个非注释行指定prog由三个目标文件filea.o、fileb.o和filec.o链接生成。第三行描述了如何从prog所依赖的文件建立可执行文件。接下来的4、6行分别指定三个目标文件，以及它们所依赖的.c和.h文件以及defs文件。而5、7、9行则指定了如何从目标所依赖的文件建立目标。　　当filea.c或a.h文件在编译之后又被修改，则 make 工具可自动重新编译filea.o，如果在前后两次编译之间，filea.c 和c.h 均没有被修改，而且filec.o还存在的话，就没有必要重新编译。这种依赖关系在多源文件的程序编译中尤其重要。通过这种依赖关系的定义，make 工具可避免许多不必要的编译工作。当然，利用Shell脚本也可以达到自动编译的效果，但是，Shell 脚本将全部编译任何源文件，包括哪些不必要重新编译的源文件，而 make 工具则可根据目标上一次编译的时间和目标所依赖的源文件的更新时间而自动判断应当编译哪个源文件。
[编辑本段](http://baike.baidu.com/view/974566.htm#)
文件定义与命令
Makefile文件作为一种描述文档一般需要包含以下内容:　　◆ 宏定义
[](http://baike.baidu.com/image/8367d1fc63fde5bbb801a001)![](http://imgsrc.baidu.com/baike/abpic/item/8367d1fc63fde5bbb801a001.jpg)
命令行中执行makefile
◆ 源文件之间的相互依赖关系　　◆ 可执行的命令　　Makefile中允许使用简单的宏指代源文件及其相关编译信息，在Linux中也称宏为变量。在引用宏时只需在变量前加$符号，但值得注意的是，如果变量名的长度超过一个字符，在引用时就必须加圆括号（）。
[编辑本段](http://baike.baidu.com/view/974566.htm#)
下面都是有效的宏引用：
$(CFLAGS)　　$Z　　$(Z)　　其中最后两个引用是完全一致的。　　需要注意的是一些宏的预定义变量，在Unix系统中，$*、$@、$?和$<四个特殊宏的值在执行命令的过程中会发生相应的变化，而在GNU make中则定义了更多的预定义变量。关于预定义变量的详细内容，宏定义的使用可以使我们脱离那些冗长乏味的编译选项，为编写makefile文件带来很大的方便。　　---------------------------------------------------------　　# Define a macro for the object files　　OBJECTS= filea.o fileb.o filec.o　　# Define a macro for the library file　　LIBES= -LS　　# use macros rewrite makefile　　prog: $(OBJECTS)　　cc $(OBJECTS) $(LIBES) -o prog　　……　　---------------------------------------------------------　　此时如果执行不带参数的make命令，将连接三个目标文件和库文件LS；但是如果在make命令后带有新的宏定义：　　make "LIBES= -LL -LS" #如何实现？　　则命令行后面的宏定义将覆盖makefile文件中的宏定义。若LL也是库文件，此时make命令将连接三个目标文件以及两个库文件LS和LL。　　在Unix系统中没有对常量NULL作出明确的定义，因此我们要定义NULL字符串时要使用下述宏定义：　　STRINGNAME= //这里有待考证　　makefile 中的变量（宏）　　GNU 的 make 工具除提供有建立目标的基本功能之外，还有许多便于表达依赖性关系　　以及建立目标的命令的特色。其中之一就是变量或宏的定义能力。如果你要以相同的编译　　选项同时编译十几个 C 源文件，而为每个目标的编译指定冗长的编译选项的话，将是非　　常乏味的。但利用简单的变量定义，可避免这种乏味的工作：　　# Define macros for name of compiler　　CC = gcc　　# Define a macr o for the CC flags　　CCFLAGS = -D_DEBUG -g -m486　　# A rule for building a object file　　test.o: test.c test.h　　$(CC) -c $(CCFLAGS) test.c　　在上面的例子中，CC 和 CCFLAGS 就是 make 的变量。GNU make 通常称之为变量，　　而其他 UNIX 的 make 工具称之为宏，实际是同一个东西。在 makefile 中引用变量的值　　时，只需变量名之前添加 $ 符号，如上面的 $(CC) 和 $(CCFLAGS)。　　GNU make 有许多预定义的变量，这些变量具有特殊的含义，可在规则中使用。表 13-2　　给出了一些主要的预定义变量，除这些变量外，GNU make 还将所有的环境变量作为自己　　的预定义变量。　　表 13-2 GNU make 的主要预定义变量
[编辑本段](http://baike.baidu.com/view/974566.htm#)
预定义变量
含义　　$*　　不包含扩展名的目标文件名称。　　$+　　所有的依赖文件，以空格分开，并以出现的先后为序，可能包含重复的依赖文件。　　$<　　第一个依赖文件的名称。　　$?　　所有的依赖文件，以空格分开，这些依赖文件的修改日期比目标的创建日期晚。　　$@　　目标的完整名称。　　$^　　所有的依赖文件，以空格分开，不包含重复的依赖文件。　　$%　　如果目标是归档成员，则该变量表示目标的归档成员名称。例如，如果目标名称为　　(image.o)，则 $@ 为 ，而 $% 为 image.o。　　AR　　归档维护程序的名称，默认值为 ar。　　ARFLAGS　　归档维护程序的选项。　　AS　　汇编程序的名称，默认值为 as。　　ASFLAGS　　汇编程序的选项。　　CC　　C 编译器的名称，默认值为 cc。　　CFLAGS　　C 编译器的选项。　　CPP　　C 预编译器的名称，默认值为 $(CC) -E。　　CPPFLAGS　　C 预编译的选项。　　CXX　　C++ 编译器的名称，默认值为 g++。　　CXXFLAGS　　C++ 编译器的选项。　　FC　　FORTRAN 编译器的名称，默认值为 f77。　　FFLAGS　　FORTRAN 编译器的选项。　　Makefile以文件名:文件名的形式比较冒号右边的文件是不是较左边的文件有更新，如果有更新则执行下一行的程序代码。因此Makefile可以把文件关联起来
            
