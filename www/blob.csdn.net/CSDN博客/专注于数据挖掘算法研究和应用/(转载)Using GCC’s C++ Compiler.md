# (转载)Using GCC’s C++ Compiler - 专注于数据挖掘算法研究和应用 - CSDN博客





2013年07月15日 10:00:39[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2226
个人分类：[C/C++](https://blog.csdn.net/fjssharpsword/article/category/6480441)











本文讨论GNU编译器集合（GCC）中的C++编译器（g++）的典型用法，主要是指命令行选项的构造。GCC的C++编译器正常安装后，可以使用g++或c++命令执行。

GCC Option Refresher

本节回顾GCC的C编译器的基本使用方法。

g++编译器的选项可以是单字符，比如-o，也可以多字符，比如-ansi。所以你不可以把多个单字符选项合写到一起，这和许多其他GNU和UNIX下的程序不同。例如，多字符选项-pg不表示2个单字符选项-p -g。选项-pg表示在最终的2进制文件里生成额外的代码，用来输出GNU code profiler的信息gprof；而选项-p -g则表示在目标2进制文件里生成额外的代码，用来产生prof code profiler需要的信息（-p），并在目标里加入调试信息（-g）。

既然g++把多字符的选项进行了区分，你就可以随意安排各个选项的顺序了。比如：

g++ -pg -fno-strength-reduce -g myprog.c -o myprog

和

g++ myprog.c -o myprog -g -fno-strength-reduce -pg

是一样的。

一般情况下，这些选项的顺序是无所谓的。但是在有些情况下，顺序会变得重要，比如你多次使用同一类的选项。举个例子，-I选项指定了搜索include文件的目录，如果你用-I指定了多个目录，gcc会按照你指定目录的顺序搜索需要的文件。

用g++编译单个源文件myprog.cc很简单，只要把文件名当参数传给g++就行了。

$ g++ myprog.cc



$ ls -l


-rwxr-xr-x 1 wvh users 13644 Oct 5 16:17 a.out



-rw-r--r-- 1 wvh users 220 Oct 5 16:17 myprog.cc


默认情况下，UNIX和LINUX操作系统生成的目标文件是当前目录下的a.out，只要输入./a.out就可以执行。在Cygwin系统下，你得到的是a.exe，通过输入./a或者./a.exe都可以执行。

要定义g++的输出文件名，使用-o选项如下所示：

$ g++ myprog.cc -o runme

$ ls -l



-rw-r--r-- 1 wvh users 220 Oct 5 16:17 myprog.cc


-rwxr-xr-x 1 wvh users 13644 Oct 5 16:28 runme


如果编译多个源文件，只要在命令行里列出它们就行了，如下所示，最终产生的输出文件是showdate：

$ g++ showdate.cc helper.cc –o showdate

如果你想先编译这些源文件，最后再把它们链接成一个2进制文件，可以用-c选项，那么g++就只产生object文件，如下所示：

$ g++ -c showdate.cc

$ g++ -c helper.cc

$ g++ showdate.o helper.o –o showdate

$ ls -l



total 124


-rw-r--r-- 1 wvh users 210 Oct 5 12:42 helper.cc


-rw-r--r-- 1 wvh users 45 Oct 5 12:29 helper.h


-rw-r--r-- 1 wvh users 1104 Oct 5 13:50 helper.o


-rwxr-xr-x 1 wvh users 13891 Oct 5 13:51 showdate


-rw-r--r-- 1 wvh users 208 Oct 5 12:44 showdate.cc


-rw-r--r-- 1 wvh users 1008 Oct 5 13:50 showdate.o


**注意**

**所有的GCC**编译器都是通过文件的后缀名来判断文件类型的，然后选择应该进行的操作（比如，后缀名为.o**的文件只需要进行链接），文件类型到操作的映射记录在GCC**的specs**文件里。在GCC**版本4**以前，specs**文件是标准文本文件，可以用任何文本编辑器修改；但是GCC**版本4**以后specs**文件是内建文件，必须要进行解压才能修改。**********************

很显然，当你的项目文件稍微多点，使用命令行来编译就不可接受了，特别是还要加上搜索目录、优化选项和其他g++选项。解决的方案就是make，不过本文并不讨论它。

C++源文件扩展名

前面说过所有GCC编译器都通过文件后缀名来决定采用的操作。下表列出了g++认识的文件类型和相应的操作。



Suffix     Operation


.C         C++ source code to preprocess.

.cc        C++ source code to preprocess. This is the standard extension for C++ source files.

.cpp       C++ source code to preprocess.

.cxx       C++ source code to preprocess



.ii        C++ source code not to preprocess.


如果一个文件的后缀名未知，那么就当成object文件进行链接。这并不是说你只能使用上表列出的文件名后缀来区分源代码文件和其他文件，你可以用-x
*lang*选项指定一个或多个输入文件的代码类型，不使用标准的文件名后缀规则。lang参数指定代码的类型；对于C++，输入文件可以是c++（标准的C++源文件）或c++-cpp-output（已经被预处理过的C++源文件，不需再进行预处理）。

**注意**

**当GCC**编译器遇到上表列出的文件后缀，它会当成C++**文件。但是，有些GCC**编译器（比如gcc**）不能处理C++**程序里很复杂的依赖关系，比如复杂的类库，于是编译失败。所以你应该用g++**（或c++**）来编译C++**程序。******************

GCC的C++编译器的命令行选项

许多命令行选项对于GCC编译器家族都是通用的，下表只列出g++专有的命令行参数。
|Option|Description|
|----|----|
|-fabi-version=n|指定编译代码需要符合的C++ ABI（application binary interface）版本。对于GCC版本3.4及更高，默认的ABI版本是2。|
|-fcheck-new|保证new操作返回的指针为非空。|
|-fconserve-space|把全局变量的初始化操作延迟到运行的时候，common segment里的全局变量不初始化，这样减少可执行文件的大小。|
|-fdollars-in-identifiers|允许标识符里出现$符号（默认）。|
|-fms-extensions|使g++忽略Microsoft Foundation Classes (MFC)中非标准用法的警告信息。|
|-fno-access-control|禁止访问检查|
|-fno-const-strings|强制g++把字符串常量的类型定义成char *，而不管ISO C++标准是否要求是const char *。|
|-fno-elide-constructors|强制g++总是调用copy构造函数，即使在用临时对象初始化另一个同类型对象的时候。|
|-fno-enforce-eh-specs|禁止在运行时检查异常处理违例。|
|-ffor-scope|对于for语句初始化部分申明的变量，限制其作用域是for循环以内。你也可以用-fno-for-scope选项强制其作用域为下一个‘}’之前，虽然这和ISO标准冲突，但是旧版本g++和许多其他传统的C++编译器都是这样做的。|
|-fms-extensions|禁止对Microsoft Foundation Classes代码的不必要的警告。|
|-fno-gnu-keywords|禁止把typeof作为一个关键字，这样就可以用它作为标识符使用，你仍可以使用__typeof__关键字来代替它。该选项被包含在了-ansi选项里面。|
|-fno-implement-inlines|Saves space by not creating out-of-line copies of inline functions controlled by #pragma statements. Using this option will generate linker errors if the*such* functions are not inlined everywhere they are called|
|-fno-implicit-inline-templates|不创建隐含的模板实例以节省空间。（详见-fno-implicit-templates）|
|-fno-implicit-templates|只创建外联（非内联）模板的显式实例以节省空间。|
|-fno-nonansi-builtins|禁止使用非ANSI/ISO标准的内置属性，包括ffs、alloca、_exit、index、bzero、conjf及其他相关的函数。|
|-fno-operator-names|禁止使用and、bitand、bitor、compl、not、or和xor关键字作为对应操作符的同义词。|
|-fno-optional-diags|禁止非标准的内部语法诊断，比如类中特殊的名字应该在何时使用各种不同的形式。|
|-fno-rtti|禁止给类的虚函数产生运行时类型信息（RTTI）|
|-fno-threadsafe-statics|使g++不产生用于线程安全检查的代码，这样能减少代码量，如果不需要线程安全的话。|
|-fno-weak|使g++不使用弱符合支持，即使链接器支持它。这个选择用于g++测试的时候，其他时候请不要使用。|
|-fpermissive|把代码的语法错误作为警告，并继续编译进程。|
|-frepo|允许模板实例化在连接时自动进行。该选项包含了-fno-implicit-templates选项。|
|-fstats|编译完成后显示前端的统计信息。该选项一般只有g++开发人员使用。|
|-ftemplate-depth-*n*|保证模板实例化的递归深度不超过整数*n*。|
|-fuse-cxa-atexit|注册静态对象的析构函数时，使用__cxa_atexit而不是atexit。|
|-fvisibility=*value*|（GCC 4.02或以后）使g++不导出ELF（Executable and Linking Format，Linux和Solaris等系统上默认的2进制文件格式）中用hidden标识的object模块内或库内的符号。该选项能减少目标文件大小，加快符号表的查找，从而改善运行性能。但是，该选项也会因为不同的visibility等级而导致模块间抛出异常发生问题，详见后面的““Visibility Attributes and Pragmas for GCC C++ Libraries”一节。如果没有使用该选项，那么默认的visibility值是default，即导出所有目标文件和库里的符号。|
|-nostdinc++|禁止在C++的标准目录里搜索头文件。|

g++编译器的其他一些C++选项处理优化、警告和代码生成的任务，我们在其他章节里讨论。下表总结了专对C++的警告选项。
|Option|Description|
|----|----|
|-Wabi|当编译器生成的代码和标准C++ ABI不兼容的时候发出警告。对于GCC版本3.4和更高，默认的ABI版本是2。|
|-Wctor-dtor-privacy|当一个类的所有构造函数和析构函数都是私有时发出警告。|
|-Weffc++|当出现不符合《Effective C++》（Scott Meyers，Addison-Wesley，2005，ISBN: 0-321-33487-6）风格的代码时给出警告|
|-Wno-deprecated|使用已过时C++属性和用法时不给出警告。|
|-Wno-non-template-friend|当非模板的友元函数定义在模板里时不给出警告。In the C++ language template specification, a friend must declare or define a nontemplate function if the name of the friend is an unqualified identifier.|
|-Wno-pmf-conversions|当把一个指向类成员函数的指针隐式转化成一般指针的时候不给出警告。|
|-Wnon-virtual-dtor|当一个类需要虚析构函数而又没有申明虚析构函数的时候给出警告。该选项被包含在-Wall选项里。|
|-Wold-style-cast|当在C++源代码里使用了传统C语言风格的类型转换方式时，给出警告。|
|-Woverloaded-virtual|当子类的函数申明覆盖基类虚函数的时候给出警告。|
|-Wreorder|当类成员变量的初始化顺序和申明顺序不一致的时候给出警告。g++编译器会自动记录所有变量的正确初始化顺序。该选项被包含在-Wall选项里。|
|-Wsign-promo|当一个重载操作把一个有符号数值转换成无符号数值的时候给出警告。在版本3.4及以前，g++对无符号类型进行了保护，但是这和C++标准不一致。|
|-Wstrict-null-sentinel|当用一个无类型的NULL作为哨兵的时候发出警告。哨兵是指一个无效的输入值，通常代表输入的结束。此问题的原因是无类型的NULL在不同的编译器实现里有不同的大小，所以必须先转化成固定的类型。|

ABI Differences in g++ Versions

C++ ABI是一套API标准，定义了C++库提供的数据类型、类、方法、头文件等的接口和规范。对库和目标文件来说，物理组织、参数传递方式和命名方式是很重要的，所以需要一个统一的接口，使编译出来的C++程序与提供的库的接口一致。这种一致性对语言特有的一些属性更加重要，比如抛出异常和捕捉异常的时候。

从版本3开始的GNU C++编译器，都遵循一个工业标准的C++ ABI规范，定义在http://www.codesourcery.com/cxx-abi/abi.html。虽然这个规范是为64位Itanium定制的，但是它适用于任何平台，并且已经作为GNU/Linux和BSD系统的C++ ABI的实现。

版本3.4以前的g++使用ABI版本1，之后使用ABI版本2。不同ABI版本之间的程序和库不能混用。如果你不确定自己g++的ABI版本，可以用g++ --version命令检查g++的版本，或用一个伪编译命令显示ABI标识符，命令行如下：

g++ -E -dM - < /dev/null | awk '/GXX_ABI/ {print $3}'

如果显示102，那么就是版本1；如果显示1002，就是版本2。如果你必须用到以前版本ABI的库，那么给g++加上选项-fabi-version=*n*，其中n就是你要兼容的ABI版本。这样做只能算作权宜之计，把所有旧的代码和库更新到当前版本才是最佳解决方案。

GNU C++ Implementation Details and Extensions

本文虽然不讨论怎样写好C++程序，但是当你用GCC的C++编译器编译你的C++程序的时候，你可以从GCC的扩展中得到许多好处，包括编译器自身的优势和g++使用的标准C++库libstdc++的优势。本节提炼出最为重要的一些扩展特性，并讨论它们在C++规范和编译器行为方面的一些差异。

**Attribute Definitions Specific to g++**

作为对visibility属性（详见于“Visibility Attributes and Pragmas for GCC C++ Libraries”）的补充，g++提供了2个额外的属性，即init_priority(priority)和java_interface属性。

**The init_priority Attribute**

该属性允许用户控制某个名字空间里的对象的初始化顺序。通常，对象的初始化顺序是它们在某个代码单元里的定义顺序。init_priority只有一个整型参数，值为101到65535，越小表示优先级越大。比如，在下面的伪码里，类MyClass将比类YourClass先初始化：

class MyClass

{

…

};

class YourClass

{

__attribute__ ((visibility("default"))) void MyMethod();

…

};

要改变它们的初始化顺序，你可以把代码改成下面这样：

class MyClass

{

__attribute__ ((init_priority(65535)));

…

};

class YourClass

{

__attribute__ ((init_priority(101)));

…

};

你只需要注意所使用的优先级数值的顺序，具体使用了哪个数值则无所谓（*即只要MyClass的优先级数值比YourClass大就行了，是不是65535和101则无所谓*）。

**The java_interface Attribute**

该属性通知g++某个类是一个Java接口类，并只能在标识了extern “Java”的模块内使用。调用这个类的函数使用的是GCC Java编译器的接口表机制（interface table mechanism），而不是通常的C++虚函数表机制（virtual function table mechanism）。

**提示**

**记住，Java**的运行时环境需要更多的初始化工作。当你混合使用C++**和Java**代码时，最好用Java**写主程序，这样能保证调用Java**函数前初始化工作已经做足了。************

**C++ Template Instantiation in g++**

模板是C++最有用和最有趣的特性之一，能减少重复代码，提高复用率，简化调试和代码维护工作。模板也有利于编译时的类型检查，比如，使用了模板就不用再传递void指针，因为你可以把模板参数实例化成任何需要的类型。

g++通过增加3个功能扩展了标准的ISO模板定义：

l 支持使用extern关键词对实例化类型进行前置申明；

l The ability to instantiate the support data required by the compiler for a named template class without actually instantiating it by using the inline keyword

l The ability to only instantiate the static data members of a class without instantiating support data or member functions by using the static keyword

基本上，GCC的g++编译器支持Borland和Cfront（AT&T）两种模板特性。要支持Borland的模板实例化和使用特性，g++使用-frepo选项允许预处理器在处理每个翻译单元（源代码文件）时进行模板实例化，并把信息存在.rpo文件里。这些文件被后面的编译过程使用，并由链接器最后合并成单个编译单元。要支持Cfront特性，g++内置了一个模板实例化库并在链接的时候合并到代码里。Cfront要求使用模板的代码要么进行显式实例化，要么包含定义模板的申明文件。你可以把显式实例化放在代码的任何地方，或一个包含的头文件里。对于后者，你可能要去掉-fno-implicit-templates选项，这样你只得到了显式实例化的那些实例。

**Function Name Identifiers in C++ and C**

GCC编译器预定义了2个标识符存储当前函数的标识。__FUNCTION__标识符只存储函数名字，__PRETTY_FUNCTION__则存储函数的全称。在C程序里，这2种名字是一样的，但是在C++程序里它们有区别。下面的程序展示了这种区别：

#include <iostream>

using namespace std;

class c {

public:

    void method_a(void)

    {

        cout<<"Function "<<__FUNCTION__<<" in "<<__FILE__<< endl;

        cout<<"Pretty Function "<<__PRETTY_FUNCTION__<<" in "

            << __FILE__ << endl;

    }

};

int main(void)

{

    c C;

    C.method_a();

    return 0;

}

运行的输出是：



$ ./a.out


Function method_a in FUNCTION_example.cc


Pretty Function void c::method_a() in FUNCTION_example.cc


在C++里，__FUNCTION__和__PRETTY_FUNCTION__是变量，而不是宏定义，所以#ifdef __FUNCTION__是没有意义的。

**注意**

**如果你的GCC**是3.2**版本或更高，那么__FUNCTION__**和__PRETTY_FUNCTION__**的行为就和C99**定义的__func__**变量是一样的。早于3.2**版本的GCC**编译器把__FUNCTION__**和__PRETTY_FUNCTION__**定义成字符串，所以它们可以和其他字符串进行串接操作。**********************

**Minimum and Maximum Value Operators**

g++编译器加入了<?和>?操作符，分别表示2个数值中较小的和较大的那个。比如，下面的代码把10赋给min变量：

min = 10 <? 15;

而下面的代码把15赋给max：

max = 10 >? 15;

**提示**

**既然这些操作符是语言提供的，那么它们也能对任何类或enum**类型进行重载。****

**Using Java Exception Handling in C++ Applications**

Java和C++的异常处理模型是不同的，虽然g++能猜测C++代码何时使用了Java异常，你最好还是明确标识出这种情况，避免链接错误。要告诉g++一块代码可能使用Java异常，把下面的代码放在该翻译单元中任何catch和throw代码之前：

#pragma GCC java_exceptions

你不能在一个翻译单元里同时使用Java和C++异常。

**Visibility Attributes and Pragmas for GCC C++ Libraries**

写C++库的时候一个普遍的问题就是可见的ELF符号太多了，其实许多符号都不能被外部使用，也不用对外公开。GCC版本4.02及更高提供了-fvisibility=*value*选项和相关的内置属性，使你可以控制这种行为，使用的方式和微软C++编译器提供的__declspec(dllexport)方式相似。新的-fhidden选项有2个可选值：default，导出目标文件的所有符号（这也是默认的行为）；hidden，不导出当前目标模块的符号。还可以在函数或类前加如下代码来进行设置：__attribute__
 ((visibility("default")))和__attribute__ ((visibility("hidden")))。

默认情况下，ELF导出全部符号。要隐藏特定目标文件的符号，需要在编译该文件的时候加上-fvisibility=hidden选项。这将导致makefile的复杂性大大增加，因为你要么需要手动设置每个文件的编译选项，要么改变全局编译选项导致任何符号都不能导出。这在类库正常抛出异常或者调试某些变量的时候实在是个灾难。

让指定的符号可见的好方式是联合使用代码属性设置和编译选项-fvisibility=hidden。如果要导出某个符号，先在它们的定义前加上__attribute__((visibility("default")))，比如下面这样：

class MyClass

{

    int i;

    __attribute__ ((visibility("default"))) void MyMethod();

    …

};

然后给makefile增加-fvisibility=hidden的编译选项，这样所有其他的符号就被隐藏了。另一个稍微好点的方法是定义一个宏，并放到所有你不想导出的符号定义前面，然后使用默认的导出所有符号，如下所示：

#define LOCAL __attribute__ ((visibility("hidden")))

class MyClass

{

    int i;

    LOCAL void MyMethod();

    …

};

编译时不使用-fvisibility=value选项，这样除了MyMethod被隐藏，其他符号都被导出。

还有一种控制可见属性的pragma语法现在还能使用，不过将来可能要去掉，如下面这样：

extern void foo(int);

#pragma GCC visibility push(hidden)

extern void bar(int);

#pragma GCC visibility pop
符号foo会被导出，但是bar则不会。这种方式虽然很简单方便，但是建议你还是使用visibility和__attribute__。




