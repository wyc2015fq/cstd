# C/C++中宏使用总结  - ljx0305的专栏 - CSDN博客
2010年04月07日 17:22:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：583标签：[编译器																[token																[语言																[linker																[dll																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=dll&t=blog)](https://so.csdn.net/so/search/s.do?q=linker&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=token&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)
个人分类：[C++																[C](https://blog.csdn.net/ljx0305/article/category/394717)](https://blog.csdn.net/ljx0305/article/category/380566)
.C/C++中宏总结C程序的源代码中可包括各种编译指令，这些指令称为预处理命令。虽然它们实际上不是C语言的一部分，但却扩展了C程 
序设计的环境。本节将介绍如何应用预处理程序和注释简化**程序开发**过程，并提高程序的可读性。 
ANSI标准定义的C语言预处理程序包括下列命令： 
#define，#error，＃i 
nclude，#if，#else，#elif，#endif，#ifdef，#ifndef，#undef，#line，#pragma等。非常明显，所有预处理命令均以符号#开头，下面分别加以介绍。
1、#define 
命令#define定义了一个标识符及一个串。在源程序中每次遇到该标识符时，均以定义的串代换它。ANSI标准将标识符定义为宏名，将替换过程称为宏 
替换。命令的一般形式为： 
#define identifier string 
注意： 
? 该语句没有分号。在标识符和串之间可以有任意个空格，串一旦开始，仅由一新行结束。 
? 宏名定义后，即可成为**其它**宏名定义中的一部分。 
? 宏替换仅仅是以文本串代替宏标识符，前提是宏标识符必须独立的识别出来，否则不进行替换。例如： #define XYZ 
this is a test，使用宏printf("XYZ")；//该段不打印"this is a test"而打印"XYZ"。因为预编译器识 
别出的是"XYZ" 
? 如果串长于一行，可以在该行末尾用一反斜杠' /'续行。
2、#error 
处理器命令#error强迫编译程序停止编译，主要用于程序调试。 
3、＃i nclude 
命令＃i nclude使编译程序将另一源文件嵌入带有＃i nclude的源文件，被读入的源文件必须用双引号或尖括号括起来。例如： 
＃i nclude"stdio.h"或者＃i nclude 
这两行代码均使用C编译程序读入并编译用于处理磁盘文件库的子程序。 
将文件嵌入＃i nclude命令中的文件内是可行的，这种方式称为嵌套的嵌入文件，嵌套层次依赖于具体实现。 
如果显式路径名为文件标识符的一部分，则仅在哪些子目录中搜索被嵌入文件。否则，如果文件名用双引号括起来，则首先检索当前工作目录。如果未发现文件， 
则在命令行中说明的所有目录中搜索。如果仍未发现文件，则搜索实现时定义的标准目录。 
如果没有显式路径名且文件名被尖括号括起来，则首先在编译命令行中的目录内检索。 
如果文件没找到，则检索标准目录，不检索当前工作目录。
4、条件编译命令 
有几个命令可对程序源代码的各部分有选择地进行编译，该过程称为条件编译。商业软件公司广泛应用条件编译来提供和维护某一程序的许多顾客版本。 
#if、#else，#elif及#endif 
#if的一般含义是如果#if后面的常量表达式为true，则编译它与#endif之间的代码，否则跳过这些代码。命令#endif标识一个#if块的 
结束。 
#if constant-expression 
statement sequence 
#endif 
跟在#if后面的表达式在编译时求值，因此它必须仅含常量及已定义过的标识符，不可使用变量。表达式不许含有操作符sizeof（sizeof也是编译 
时求值）。 
#else命令的功能有点象C语言中的else；#else建立另一选择（在#if失败的情况下）。 
注意，# else属于# if块。 
#elif命令意义与ELSE IF 相同，它形成一个if else-if阶梯状语句，可进行多种编译选择。 
#elif 后跟一个常量表达式。如果表达式为true，则编译其后的代码块，不对其它#elif表达式进行测试。否则，顺序测试下一块。 
#if expression 
statement sequence 
#elif expression1 
statement sequence 
#endif 
在嵌套的条件编译中#endif、#else或#elif与最近#if或#elif匹配。 
# ifdef 和# ifndef 
条件编译的另一种方法是用#ifdef与#ifndef命令，它们分别表示"如果有定义"及"如果无定义"。 
# ifdef的一般形式是： 
# ifdef macroname 
statement sequence 
#endif 
#ifdef与#ifndef可以用于#if、#else，#elif语句中，但必须与一个#endif。
5、#undef 
命令#undef 取消其后那个前面已定义过有宏名定义。一般形式为： 
#undef macroname
6、#line 
命令# line改变__LINE__与__FILE__的内容，它们是在编译程序中预先定义的标识符。命令的基本形式如下： 
# line number["filename"] 
其中的数字为任何正整数，可选的文件名为任意有效文件标识符。行号为源程序中当前行号，文件名为源文件的名字。命令# line主要用于调试及其它特殊 
应用。 
注意：在#line后面的数字标识从下一行开始的数字标识。
7、预定义的宏名 
ANSI标准说明了C中的五个预定义的宏名。它们是： 
__LINE__ 
__FILE__ 
__DATE__ 
__TIME__ 
__STDC__ 
如果编译不是标准的，则可能仅支持以上宏名中的几个，或根本不支持。记住编译程序也许还提供其它预定义的宏名。 
__LINE__及__FILE__宏指令在有关# line的部分中已讨论，这里讨论其余的宏名。 
__DATE__宏指令含有形式为月/日/年的串，表示源文件被翻译到代码时的日期。 
源代码翻译到目标代码的时间作为串包含在__TIME__中。串形式为时：分：秒。 
如果实现是标准的，则宏__STDC__含有十进制常量1。如果它含有任何其它数，则实现是非标准的。编译C++程序时，编译器自动定义了一个预处理名 
字__cplusplus，而编译标准C时，自动定义名字__STDC__。 
注意：宏名的书写由标识符与两边各二条下划线构成。 
(部分内容出自：http://www.bc-cn.net/Article/kfyy/cyy/jc/200511/919.html)
8、C、C++宏体中出现的#，#@，## 
宏体中，#的功能是将其后面的宏参数进行字符串化操作（Stringfication），简单说就是在对它所引用的宏变量通过替换后在其左右各加上一个 
双引号。 
而##被称为连接符（concatenator），用来将两个Token连接为一个Token。注意这里连接的对象是Token就行，而不一定是宏的变 
量。比如你要做一个菜单项命令名和函数指针组成的结构体的数组，并且希望在函数名和菜单项命令名之间有直观的、名字上的关系。那就可以使用：宏参数## 
固定部分。当然还可以n个##符号连接 n+1个Token，这个特性也是#符号所不具备的。 
#@的功能是将其后面的宏参数进行字符化。
9、C宏中的变参... 
...在C宏中称为Variadic Macro，也就是变参宏。比如： 
#define myprintf(templt,...) fprintf(stderr,templt,__VA_ARGS__) 
或者#define myprintf(templt,args...) fprintf(stderr,templt,args) 
第一个宏中由于没有对变参起名，我们用默认的宏__VA_ARGS__来替代它。第二个宏中，我们显式地命名变参为args，那么我们在宏定义中就可以 
用args来代指变参了。同C语言的stdcall一样，变参必须作为参数表的最后有一项出现。当上面的宏中我们只能提供第一个参数templt时，C 
标准要求我们必须写成： myprintf(templt,);的形式。这时的替换过程为：myprintf("Error!/n",);替换为： 
fprintf(stderr,"Error!/n",). 
这是一个语法错误，不能正常编译。这个问题一般有两个解决方法。首先，GNU CPP提供的解决方法允许上面的宏调用写成： 
myprintf(templt);而它将会被通过替换变成： fprintf(stderr,"Error!/n",); 
很明显，这里仍然会产生编译错误（非本例的某些情况下不会产生编译错误）。除了这种方式外，c99和GNU CPP都支持下面的宏定义方式： 
#define myprintf(templt, ...) fprintf(stderr,templt, ##__VAR_ARGS__) 
这时，##这个连接符号充当的作用就是当__VAR_ARGS__为空的时候，消除前面的那个逗号。那么此时的翻译过程如下： 
myprintf(templt);被转化为： fprintf(stderr,templt); 
这样如果templt合法，将不会产生编译错误。
10、#pragma的使用【转载】 
在所有的预处理指令中，#Pragma 指令可能是最复杂的了，它的作用是设定编译器的状态或者是指示编译器完成一些特定的动作。#pragma指令对 
每个编译器给出了一个方法,在保持与C和C ++语言完全兼容的情况下,给出主机或操作系统专有的特征。依据定义,编译指示是机器或操作系统专有的,且 
对于每个编译器都是不同的。 
其格式一般为: #Pragma Para，其中Para 为参数，下面来看一些常用的参数。 
(1)message 参数。 Message 参数是我最喜欢的一个参数，它能够在编译信息输出窗口中输出相应的信息，这对于源代码信息的控制是非常 
重要的。其使用方法为： 
#Pragma message("消息文本") 
当编译器遇到这条指令时就在编译输出窗口中将消息文本打印出来。 
当我们在程序中定义了许多宏来控制源代码版本的时候，我们自己有可能都会忘记有没有正确的设置这些宏，此时我们可以用这条指令在编译的时候就进行检查。 
假设我们希望判断自己有没有在源代码的什么地方定义了_X86这个宏可以用下面的方法 
#ifdef _X86 
#Pragma message("_X86 macro activated!") 
#endif 
当我们定义了_X86这个宏以后，应用程序在编译时就会在编译输出窗口里显示"_ 
X86 macro activated!"。我们就不会因为不记得自己定义的一些特定的宏而抓耳挠腮了。 
(2)另一个使用得比较多的pragma参数是code_seg。格式如： 
#pragma code_seg( ["section-name"[,"section-class"] ] ) 
它能够设置程序中函数代码存放的代码段，当我们开发驱动程序的时候就会使用到它。 
(3)#pragma once (比较常用） 
只要在头文件的最开始加入这条指令就能够保证头文件被编译一次，这条指令实际上在VC6中就已经有了，但是考虑到兼容性并没有太多的使用它。 
(4)#pragma hdrstop表示预编译头文件到此为止，后面的头文件不进行预编译。BCB可以预编译头文件以加快链接的速度，但如果所有头文 
件都进行预编译又可能占太多磁盘空间，所以使用这个选项排除一些头文件。 
有时单元之间有依赖关系，比如单元A依赖单元B，所以单元B要先于单元A编译。你可以用#pragma startup指定编译优先级，如果使用了 
#pragma package(smart_init) ，BCB就会根据优先级的大小先后编译。 
(5)#pragma resource "*.dfm"表示把*.dfm文件中的资源加入工程。*.dfm中包括窗体、外观的定义。 
(6)#pragma warning( disable : 4507 34; once : 4385; error : 164 ) 
等价于： 
#pragma warning(disable:4507 34) // 不显示4507和34号警告信息 
#pragma warning(once:4385) // 4385号警告信息仅报告一次 
#pragma warning(error:164) // 把164号警告信息作为一个错误。 
同时这个pragma warning 也支持如下格式： 
#pragma warning( push [ ,n ] ) 
#pragma warning( pop ) 
这里n代表一个警告等级(1---4)。 
#pragma warning( push )保存所有警告信息的现有的警告状态。 
#pragma warning( push, n)保存所有警告信息的现有的警告状态，并且把全局警告等级设定为n。 
#pragma warning( pop )向栈中弹出最后一个警告信息，在入栈和出栈之间所作的一切改动取消。例如： 
#pragma warning( push ) 
#pragma warning( disable : 4705 ) 
#pragma warning( disable : 4706 ) 
#pragma warning( disable : 4707 ) 
//....... 
#pragma warning( pop ) 
在这段代码的最后，重新保存所有的警告信息(包括4705，4706和4707)。
（7）pragma comment(...) 
该指令将一个注释记录放入一个对象文件或可执行文件中。 
常用的lib关键字，可以帮我们连入一个库文件。 
(8)用pragma导出dll中的函数 
传统的到出 DLL 函数的方法是使用模块定义文件 (.def)，Visual C++ 提供了更简洁方便的方法，那就 
是"__declspec()"关键字后面跟"dllexport"，告诉连接去要导出这个函数，例如： 
__declspec(dllexport) int __stdcall MyExportFunction(int iTest); 
把"__declspec(dllexport)"放在函数声明的最前面，连接生成的 DLL 就会导出函 
数"_MyExportFunction@4"。 
上面的导出函数的名称也许不是我的希望的，我们希望导出的是原版的"MyExportFunction"。还好，VC 提供了一个预处理指示 
符"#pragma"来指定连接选项 (不仅仅是这一个功能，还有很多指示功能) ，如下： 
#pragma comment(linker,"/EXPORT:MyExportFunction=_MyExportFunction@4") 
这下就天如人愿了：）。如果你想指定导出的顺序，或者只将函数导出为序号，没有 Entryname，这个预处理指示符 (确切地说是连接器) 都能够 
实现，看看 MSDN 的语法说明： 
/EXPORT:entryname[,@ordinal[,NONAME]][,DATA] 
@ordinal 指定顺序；NONAME 指定只将函数导出为序号；DATA 关键字指定导出项为数据项。 
⑨每个编译程序可以用#pragma指令激活或终止该编译程序支持的一些编译功能。例如，对循环优化功能： 
#pragma loop_opt(on) // 激活 
#pragma loop_opt(off) // 终止 
有时，程序中会有些函数会使编译器发出你熟知而想忽略的警告，如"Parameter xxx is never used in function 
xxx"，可以这样： 
#pragma warn -100 // Turn off the warning message for warning #100 
int insert_record(REC *r) 
{ /* function body */ } 
#pragma warn +100 // Turn the warning message for warning #100 back 
on 
函数会产生一条有唯一特征码100的警告信息，如此可暂时终止该警告。 
每个编译器对#pragma的实现不同，在一个编译器中有效在别的编译器中几乎无效。可从编译器的文档中查看。
⑩#pragm pack()的使用 
#pragma pack规定的对齐长度，实际使用的规则是： 
? 结构，联合，或者类的数据成员，第一个放在偏移为0的地方，以后每个数据成员的对齐，按照#pragma pack指定的数值和这 
个数据成员自身长度中，比较小的那个进行。 
? 也就是说，当#pragma pack的值等于或超过所有数据成员长度的时候，这个值的大小将不产生任何效果。 
? 而结构整体的对齐，则按照结构体中最大的数据成员 和 #pragma pack指定值之间，较小的那个进行。 
注意：文件使用#pragma pack(n) 改变了缺省设置而不恢复，通常可以使用#pragma pack(push, n)和#pragma 
pack(pop)进行设置与恢复。 
注：关于宏函数的内容在另外的专题。关于宏使用的误区在描述宏的时候已经在文中提到了，最后再给出一个例子，描述的Side Effect是指宏在展开 
的时候对其参数可能进行多次Evaluation（也就是取值）对程序造成的错误影响。 
假设在一个系统中，有一个32b的寄存器(REG)保存状态，其中高16b表示一种含义，低16b表示另一种含义（这在程序中经常出现）。现在要把高低 
16b分开，不考虑实际中的特殊要求，将代码写成： 
#define High16bit(REG) (REG>>16) 
#define Low16bit(REG) ((REG<<16)>>16) 
对于这种写法完成的功能在大多数情况是足够了，这里不讨论。主要谈论这种写法的负面影响，如果在程序中分别在不同的语句中使用High16bit和 
Low16bit，那么就可能那就是Side effect，特别寄存器REG是状态寄存器，他的状态可能随时变化，那么引起的问题就是高低16b根本 
取的不是同一个时刻状态寄存器。这种错误在程序中找出就比较难了。在这里我把条件弱化了，试想在一个宏体中，如果对参数多次取值也是可能引起问题，那就 更难了。
引用:http://www.cnblogs.com/areliang/archive/2007/08/02/840455.html
