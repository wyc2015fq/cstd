# 《Linux程序设计》——调试 - 一世豁然的专栏 - CSDN博客





2014年10月09日 20:57:13[一世豁然](https://me.csdn.net/Explorer_day)阅读数：770标签：[调试																[linux																[编程																[开发工具](https://so.csdn.net/so/search/s.do?q=开发工具&t=blog)
个人分类：[liunx应用编程](https://blog.csdn.net/Explorer_day/article/category/2276817)





一、错误类型

 1、功能定义错误

 2、设计规划错误

 3、代码编写错误







二、常用调试技巧

 1、程序调试的五个阶段

   1）、测试：找出程序中存在的缺陷或错误

   2）、固化：让程序的错误可重现

   3）、定位：确定相关的代码行

   4）、纠正：修改代码纠正错误

   5）、验证：确定修改解决了问题




 2、有漏洞的程序（P362-P363）




 3、代码检查（利用GCC编译器）




 4、取样法

   1）、定义：在程序中增加一些代码以收集更多与程序运行时的行为相关的信息。

   2）、取样法俄两种方式

       I、用C语言预处理器有选择地包括取样代码，这样只需要重新编译程序就可以包含或去除调试代码。

       II、使用printf函数——应该将调试信息输出到标准错误输出stderr，或者，如果因为程序的原因不能这样做，还可以使用syslog函数提供日志功能。




 5、程序的受控执行







三、使用gdb进行调试

 1、启动gdb（P368例程）

 2、运行一个程序（run  P369例程）

 3、栈跟踪（backtrace  P369例程）

 4、检查变量（print  P370例程）

 5、列出程序源代码（list  P370-P371例程）

 6、设置断点（breakpoint  P371-P374例程）

 7、用调试器补丁（P374-P375例程）

 8、深入学习gdb







四、其他调试工具

 1、lint：清理程序中的“垃圾”（P376-P378例程）




 2、函数调用工具

   1）、ctags：为程序中的所有函数创建索引。每个函数对应一个列表，在列表中列出该函数在程序中的调用位置。语法如下：

**  ctags  [-a]  [-f  filename]  soucefile  soucefile  . . .**

**           ctags  -x  soucefile  soucefile  . . .**

         I、默认情况下，ctags在当前目录下创建tags。在该文件中包含每个输入源文件中声明的每个函数，文件中每行的格式如下所示：

              announce   app_ui.c     /^static  void  announce(void)/

             文件中的每行由函数名、声明该函数的文件和一个可以用来在文件中查找该函数定义的正则表达式组成。

         II、 可以使用**-x**选项在标准输出上列出雷士上面的格式的内容。可以用-f选项将输出重定向到另一个不同的文件中。可以用-a选项将输出结果附加到一个已有文件的结尾。

   2）、cxref：分析C语言源代码并生成一个交叉引用表。它可以显示每个符号都在程序的那个位置使用过。

   3）、cflow：打印一个函数调用树，他显示了函数之间调用的关系。

        I、-i选项它将产生一个反向的函数调用树。针对每个函数，cflow列出调用他的其它函数。




 3、用prof/gprof产生执行存档

   1）、想查找程序的性能问题时，一种常用的的技巧是使用执行存档。程序的存档可以显示执行它所花费的时间具体都用在什么操作上了。

   2）、编译程序时，给编译器加上-p标志（针对prof）或-pg（针对gprof程序）就可以创建出profile程序。







五、断言

 1、assert宏

     #include<assert.h>

     void  assert（int  expression）


     I、assert宏对表达式进行求值，如果结果为零，它就往标准错误写一些诊断信息，然后调用abort函数结束程序的运行。

     II、可以在编译期间使用-DNDEBUG关闭断言功能或把语句#define  NDEBUG加到每个源文件中，但这条语句必须在#include<assert.h>语句前。







六、内存测试

 1、ElectricFence函数库（例程P383-P384）

 2、valgring（例程P384-P387）](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=调试&t=blog)




