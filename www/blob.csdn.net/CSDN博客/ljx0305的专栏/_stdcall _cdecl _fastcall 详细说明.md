# _stdcall _cdecl _fastcall 详细说明 - ljx0305的专栏 - CSDN博客
2008年07月30日 11:32:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：504标签：[pascal																[winapi																[generation																[c																[编译器																[汇编](https://so.csdn.net/so/search/s.do?q=汇编&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=generation&t=blog)](https://so.csdn.net/so/search/s.do?q=winapi&t=blog)](https://so.csdn.net/so/search/s.do?q=pascal&t=blog)
个人分类：[C++																[C](https://blog.csdn.net/ljx0305/article/category/394717)](https://blog.csdn.net/ljx0305/article/category/380566)
(1) _stdcall调用　　_stdcall是Pascal程序的缺省调用方式，参数采用从右到左的压栈方式，被调函数自身在返回前清空堆栈。　　WIN32 Api都采用_stdcall调用方式，这样的宏定义说明了问题： 　　#define WINAPI _stdcall 　　按C编译方式，_stdcall调用约定在输出函数名前面加下划线，后面加“@”符号和参数的字节数，形如[_functionname@number](mailto:_functionname@number)。 
(2) _cdecl调用　　_cdecl是C/C++的缺省调用方式，参数采用从右到左的压栈方式，传送参数的内存栈由调用者维护。_cedcl约定的函数只能被C/C++调用，每一个调用它的函数都包含清空堆栈的代码，所以产生的可执行文件大小会比调用_stdcall函数的大。 　　由于_cdecl调用方式的参数内存栈由调用者维护，所以变长参数的函数能（也只能）使用这种调用约定。　　由于Visual C++默认采用_cdecl 调用方式，所以VC中中调用DLL时，用户应使用_stdcall调用约定。　　按C编译方式，_cdecl调用约定仅在输出函数名前面加下划线，形如_functionname。 　　
(3) _fastcall调用　　_fastcall调用较快，它通过CPU内部寄存器传递参数。 （实际上，它用ECX和EDX传送前两个双字（DWORD）或更小的参数，剩下的参数仍旧自右向左压栈传送，被调用的函数在返回前清理传送参数的内存栈），　　按C编译方式，_fastcall调用约定在输出函数名前面加“@”符号，后面加“@”符号和参数的字节数，形如@functionname@number 
（4）thiscall仅仅应用于“C++”成员函数。this指针存放于CX寄存器，参数从右到左压。thiscall不是关键词，因此不能被程序员指定。
关键字 __stdcall、__cdecl和__fastcall可以直接加在要输出的函数前，也可以在编译环境的Setting.../C/C++ /Code Generation项选择。当加在输出函数前的关键字与编译环境中的选择不同时，直接加在输出函数前的关键字有效。它们对应的命令行参数分别为/Gz、/Gd和/Gr。缺省状态为/Gd，即__cdecl。 
要完全模仿PASCAL调用约定首先必须使用__stdcall调用约定，至于函数名修饰约定，可以通过其它方法模仿。还有一个值得一提的是WINAPI宏，Windows.h支持该宏，它可以将出函数翻译成适当的调用约定，在WIN32中，它被定义为__stdcall。使用WINAPI宏可以创建自己的APIs。 
2)名字修饰约定 
1、修饰名(Decoration name) 
“C”或者“C++”函数在内部（编译和链接）通过修饰名识别。修饰名是编译器在编译函数定义或者原型时生成的字符串。有些情况下使用函数的修饰名是必要的，如在模块定义文件里头指定输出“C++”重载函数、构造函数、析构函数，又如在汇编代码里调用“C””或“C++”函数等。 
修饰名由函数名、类名、调用约定、返回类型、参数等共同决定。 
2、名字修饰约定随调用约定和编译种类(C或C++)的不同而变化。函数名修饰约定随编译种类和调用约定的不同而不同，下面分别说明。 
a、C编译时函数名修饰约定规则： 
__stdcall调用约定在输出函数名前加上一个下划线前缀，后面加上一个“@”符号和其参数的字节数，格式为_functionname@number。 
__cdecl调用约定仅在输出函数名前加上一个下划线前缀，格式为_functionname。 
__fastcall调用约定在输出函数名前加上一个“@”符号，后面也是一个“@”符号和其参数的字节数，格式为@functionname@number。 
它们均不改变输出函数名中的字符大小写，这和PASCAL调用约定不同，PASCAL约定输出的函数名无任何修饰且全部大写。 
b、C++编译时函数名修饰约定规则： 
__stdcall调用约定： 
1、以“?”标识函数名的开始，后跟函数名； 
2、函数名后面以“@@YG”标识参数表的开始，后跟参数表； 
3、参数表以代号表示： 
X--void ， 
D--char， 
E--unsigned char， 
F--short， 
H--int， 
I--unsigned int， 
J--long， 
K--unsigned long， 
M--float， 
N--double， 
_N--bool， 
.... 
PA--表示指针，后面的代号表明指针类型，如果相同类型的指针连续出现，以“0”代替，一个“0”代表一次重复； 
4、参数表的第一项为该函数的返回值类型，其后依次为参数的数据类型,指针标识在其所指数据类型前； 
5、参数表后以“@Z”标识整个名字的结束，如果该函数无参数，则以“Z”标识结束。 
其格式为“?functionname@@YG*****@Z”或“?functionname@@YG*XZ”，例如 
int Test1（char *var1,unsigned long）-----“?Test1@@YGHPADK@Z” 
void Test2（） -----“?Test2@@YGXXZ” 
__cdecl调用约定： 
规则同上面的_stdcall调用约定，只是参数表的开始标识由上面的“@@YG”变为“@@YA”。 
__fastcall调用约定： 
规则同上面的_stdcall调用约定，只是参数表的开始标识由上面的“@@YG”变为“@@YI”。 
VC++对函数的省缺声明是"__cedcl",将只能被C/C++调用. 
CB在输出函数声明时使用4种修饰符号 
//__cdecl 
cb的默认值，它会在输出函数名前加_，并保留此函数名不变，参数按照从右到左的顺序依次传递给栈，也可以写成_cdecl和cdecl形式。 
//__fastcall 
她修饰的函数的参数将尽肯呢感地使用寄存器来处理，其函数名前加@，参数按照从左到右的顺序压栈； 
//__pascal 
它说明的函数名使用Pascal格式的命名约定。这时函数名全部大写。参数按照从左到右的顺序压栈； 
//__stdcall 
使用标准约定的函数名。函数名不会改变。使用__stdcall修饰时。参数按照由右到左的顺序压栈，也可以是_stdcall；
引用:http://blog.csdn.net/qinmi/archive/2007/08/15/1744951.aspx
