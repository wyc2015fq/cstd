# C++ 异常处理机制的实现 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [Sheng Gordon](http://www.jobbole.com/members/shenggordon) 翻译。未经许可，禁止转载！
英文出处：[Vishal Kochhar](http://www.codeproject.com/Articles/2126/How-a-C-compiler-implements-exception-handling)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
本文深入讨论了VC++编译器异常处理的实现机制。附件源代码包含了一个VC++的异常处理库。
[**下载源代码**** – 19 Kb**](http://www.codeproject.com/KB/cpp/Exceptionhandler/Exceptionhandler_src.zip)
### 介绍
相对于传统语言，C++ 的革命性特征之一，就是它对异常处理的支持。传统异常处理技术有缺陷并且易于出错，而 C++ 提供了一个非常优秀的替代方案。它将正常流程代码与错误处理代码清晰的隔离出来，使得程序更加健壮，易于维护。这篇文章将讨论编译器是如何实现异常处理的。假定读者已经对异常处理机制及其语法已经有了大致的了解。我用 VC++ 实现了本文中介绍的异常处理库。要将异常处理器替换成我的 VC++ 实现方式，调用下面的函数：

C++
```
install_my_handler();
```
在此之后，程序中发生的任何异常——从抛出一个异常到栈展开、调用catch块、然后恢复执行——都被我的异常处理库处理。
和C++的其他特性一样，C++标准没有关于应该如何实现异常处理的任何说明。这意味着每个编译器厂商都可以自由选择它认为合适的任何实现方式。我将介绍VC++是如何实现这一特性的。对使用其他编译器或者操作系统的开发者[1]，它应该是一个很好的学习资料。VC++基于Windows操作系统的结构化异常处理（SEH），构建了它的异常处理支持[2]。
### 结构化异常处理——概述
在此讨论中，我将考虑那些显式抛出或者被 0 除、访问空指针等操作导致的异常。当异常发生时，将产生中断，然后控制权转移给操作系统。操作系统调用异常处理器，它将从产生异常的函数开始检查函数调用序列，然后执行栈展开，并转移控制权。我们可以写自己的异常处理器，然后注册到操作系统；这样当异常事件发生时，操作系统会调用它。
为了注册，Windows定义了一个特殊的结构体，叫做EXCEPTION_REGISTRATION：


C++
```
struct EXCEPTION_REGISTRATION
{
   EXCEPTION_REGISTRATION *prev;
   DWORD handler;
};
```
要注册你自己的异常处理器，就需要创建这个结构体，然后将它的地址存储到寄存器 FS 所指位置的0偏移处，如下伪汇编代码所示：


```
mov FS:[0], exc_regp
```
字段 prev 表明 EXCEPTION_REGISTRATION 结构体是一个链表结构。当我们注册 EXCEPTION_REGISTRATION 结构体时，我们需要将前一个注册的结构体地址存入prev字段。
那么异常回调函数是什么样子呢？Windows要求异常处理器的函数签名如下，它在EXCPT.h中定义：


C++
```
EXCEPTION_DISPOSITION (*handler)(
    _EXCEPTION_RECORD *ExcRecord,
    void * EstablisherFrame, 
    _CONTEXT *ContextRecord,
    void * DispatcherContext);
```
目前你可以忽略所有的参数和返回值类型。下面的程序向操作系统注册异常处理器，并通过除以0产生了一个异常。这个异常被异常处理器捕捉到。处理器没做多余的工作，只是输出了一条信息，然后退出程序。


C++
```
#include <iostream>
#include <windows.h>
using std::cout;
using std::endl;
struct EXCEPTION_REGISTRATION
{
   EXCEPTION_REGISTRATION *prev;
   DWORD handler;
};
EXCEPTION_DISPOSITION myHandler(
    _EXCEPTION_RECORD *ExcRecord,
    void * EstablisherFrame, 
    _CONTEXT *ContextRecord,
    void * DispatcherContext)
{
	cout << "In the exception handler" << endl;
	cout << "Just a demo. exiting..." << endl;
	exit(0);
	return ExceptionContinueExecution; //will not reach here
}
int g_div = 0;
void bar()
{
	//initialize EXCEPTION_REGISTRATION structure
	EXCEPTION_REGISTRATION reg, *preg = ®
	reg.handler = (DWORD)myHandler;
	
	//get the current head of the exception handling chain	
	DWORD prev;
	_asm
	{
		mov EAX, FS:[0]
		mov prev, EAX
	}
	reg.prev = (EXCEPTION_REGISTRATION*) prev;
	
	//register it!
	_asm
	{
		mov EAX, preg
		mov FS:[0], EAX
	}
	//generate the exception
	int j = 10 / g_div;  //Exception. Divide by 0. 
}
int main()
{
	bar();
	return 0;
}
/*-------output-------------------
In the exception handler
Just a demo. exiting...
---------------------------------*/
```
请注意，Windows有一个严格的规则要求：EXCEPTION_REGISTRATION 结构体应该放在栈上，并且它的内存地址应该比前一个节点要小。如果Windows发现不满足这个规则，将会结束进程。
### 函数与栈
栈是一块连续的内存区域，用来存储函数的局部变量。具体来说，每个函数都对应着一个栈帧（stack frame），用来存储这个函数的所有局部变量，以及函数内表达式产生的中间值。请注意下图是一个典型的示例。真实情况下，编译器为了达到快速访问的目的，可能会把部分或者全部变量存储到寄存器中。栈是处理器级别的概念。处理器提供内部寄存器和操作寄存器的特殊指令。
图2展示了函数foo调用函数bar，而bar调用函数widget时栈的典型情况。注意，此时栈是向下增长的。这意味着，后压入栈的变量地址会比先压入栈的变量地址要小。
![](http://ww1.sinaimg.cn/mw690/6941baebjw1f68dkeirwdg20gl0a20sl.gif)
编译器使用EBP寄存器来标识当前活动栈帧。在这个例子中，widget函数将被执行，因此EBP寄存器指向widget的栈帧，如图所示。函数通过偏移这个帧指针来获取局部变量。在编译阶段，编译器将局部变量的名称绑定到相对于帧指针的一个固定偏移值。例如，widget函数的一个局部变量，会通过栈指针向下偏移固定字节数来访问，称作EBP-24。
图中也展示了ESP寄存器，它是栈指针，指向栈的最后一个数据。在这个例子中，ESP指向widget帧的尾部。下一个帧会在这个位置创建。
处理器支持两种栈操作：压栈和出栈。如：
**pop EAX**
它的意思是从ESP指向的位置读取4个字节，然后ESP增加4（记住，这里栈是向下增长的）。同样地，
**push EBP**
它的意思是ESP递减4，然后将EBP寄存器的值写入到ESP指向的位置。
当编译器编译函数时，它会在函数开始的地方加入一些代码，称作初始化段（prologue），它负责创建并初始化函数的栈帧。同样地，编译器在函数尾部也添加一些代码，称作清理段（epilogue），它负责弹出当前函数的栈帧。
编译器一般会为初始化段生成如下的代码序列：


C++
```
Push EBP      ; save current frame pointer on stack
Mov EBP, ESP  ; Activate the new frame
Sub ESP, 10   ; Subtract. Set ESP at the end of the frame
```
第一行语句把当前帧指针EBP保存到栈上。第二行语句通过修改调用函数帧位置的EBP寄存器，激活被调用函数的帧。第三行语句通过把ESP减去函数所创建的所有局部变量与中间值的大小，将ESP寄存器移动到当前帧的尾部。在编译阶段，编译器知道函数的所有局部变量的类型与大小，因此它能够计算出帧的大小。
清理段的工作与初始化段相反，它把当前帧从栈上移除：


C++
```
Mov ESP, EBP    
Pop EBP         ; activate caller's frame
Ret             ; return to the caller
```
它将调用函数保存的帧指针恢复到ESP（即被调用函数帧指针指向的位置），将它弹出到EBP，因此激活了调用函数的栈帧，然后执行ret指令。
当处理器遇到ret指令时，它做下面的工作：它从栈上弹出返回地址，然后将控制转移到这个地址。在调用函数执行call指令时，返回地址被压到栈上。Call指令首先将下一条指令的地址，即控制返回时要执行的指令，压到栈上，然后跳转到被调用函数的起始处。图3展示了一个更详细的栈帧图。如图示，函数参数也是函数栈帧的一部分。调用函数将被调用函数的参数压到栈上。当函数返回时，调用函数通过给ESP加上参数的大小，从栈上移除被调用函数的参数。这个大小在编译期就可以确定。
**Add ESP, args_size**
同样地，被调用函数通过给ret指令指定所有参数的大小，也能够移除参数，这个大小也可以在编译期确定。假设参数总大小为24，下面的指令在返回到调用函数之前，从栈上移除24个字节：
**Ret 24**
上面两种方式，一次函数调用只能使用其中一种，它取决于被调用函数的调用约定。另外请注意进程中的每个线程都有自己的栈。
![](http://ww1.sinaimg.cn/mw690/6941baebjw1f68dke0u2jg20ei0fdq2u.gif)
### C++和异常
我在第一节中曾经提到过EXCEPTION_REGISTRATION结构体。它被用来向操作系统注册异常回调函数，这个回调函数在异常发生时被调用。
VC++通过在其尾部增加两个字段，扩展了它的语义：


C++
```
struct EXCEPTION_REGISTRATION
{
   EXCEPTION_REGISTRATION *prev;
   DWORD handler;
   int   id;
   DWORD ebp;
};
```
VC++为每个函数创建一个EXCEPTION_REGISTRATION结构体，作为函数的局部变量，少数函数除外[3]。结构体的最后一个字段与帧指针EBP指向的位置重叠。函数的初始化段在其栈帧上创建这个结构体，并将其注册给操作系统。清理段代码恢复调用者的EXCEPTION_REGISTRATION。我将在下一节中讨论id字段的作用。
当VC++编译一个函数时，它会为函数生成两类数据：
A、异常回调函数
B、一个包含函数重要信息的数据结构，如catch块，catch希望接收的异常类型信息等。我把这个数据结构称作funcinfo，下一节中我会做详细介绍。
图4展示了栈运行时包含异常处理代码的结构。Widget函数的异常回调函数位于异常链的头部，FS:[0]指向它（在widget函数的初始化段设置）。异常处理器将widget函数的funcinfo结构体地址传递给__CxxFrameHandler函数，此函数检测这个数据结构，查看是否有希望接收当前异常类型的catch块。如果没有发现，它把ExceptionContinueSearch值返回给操作系统。操作系统从异常处理链中取出下一个节点，然后调用其异常处理器（即当前函数的调用者的处理器）。
![](http://ww2.sinaimg.cn/mw690/6941baebjw1f68dkdkt15g20go0h1wg5.gif)
这个过程一直持续到异常处理器找到了能够接收当前异常的catch块，这种情况下它不会将控制权返回给操作系统。但是在调用catch块之前（它能够从funcinfo结构体获取到catch块的地址，见图4），异常处理器必须执行栈展开：清理这个函数帧之下的所有函数的栈帧。清理栈帧的过程有些复杂：异常处理器必须找到异常发生时函数帧上所有存活的局部变量，然后调用它们的析构器。我后面会详细介绍。
这个异常处理器将清理帧的任务委托给这个帧的异常处理器。从FS:[0]所指向的异常处理链表的头部开始，依次调用每个节点的异常处理器，通知它栈即将被展开。作为回应，节点的异常处理器调用所有局部变量的析构器，然后返回。这样一直到达自己所在的节点。
由于catch块是函数的一部分，它与所在的函数共用同一个栈帧。因此异常处理器需要在调用catch块之前激活其栈帧。另外，每个catch块都会接收一个参数，就是它希望捕获的异常类型。异常处理器应该复制异常对象或其引用到catch块的帧上面。它能够从funcinfo结构体获取到异常对象。编译器非常慷慨地生成了这些信息。
在复制异常与激活帧之后，异常处理器调用了catch块。Catch块将try-catch块执行结束后的下一条指令地址返回给异常处理器。注意，在此刻，虽然栈已经展开，帧已经清理完毕，但是它们在物理上仍然占据着栈的空间，没有被移除。这是因为异常处理器仍然在执行，和其他普通函数一样，它也使用栈来存储其临时变量，它的帧在发生异常的函数帧下面。当catch块返回时，它需要析构异常对象。这发生在异常处理器移除了所有帧（包括异常处理器自身），并且将控制权转交给try-catch块后面代码之后。移除所有帧的方法是将ESP指向函数帧的尾部（它转交控制权的那个帧）。它如何确定这个函数帧的尾部呢？它没有办法确定。但是编译器已经通过函数的初始化段把它保存到栈上了，只需要异常处理器找到就行了。再看图4，它的位置在帧指针EBP下面16个字节。
Catch块可能会抛出一个新的异常，或者把异常重新抛出。异常处理器必须检测到这种情形，并作出适当的处理。如果catch块抛出了一个新的异常，异常处理器必须析构旧的异常对象；如果catch块重新抛出异常，那么异常处理器必须复制旧的异常对象。
有一个重点需要指出：因为每个线程都有自己的栈，所以每个线程都拥有自己独立的EXCEPTION_REGISTRATION链表。
C++和异常 – 2
图5描述了funcinfo结构体的布局。注意，不同的VC++编译器下字段的名称是不同的。另外，我只列出了部分相关的字段。展开表（unwind table）的结构体在下一节中讨论。
![](http://ww4.sinaimg.cn/mw690/6941baebjw1f68dkdc3j1g20fp0ek3yg.gif)
当异常处理器在函数中查找catch块时，它要确定的第一件事是在异常发生的代码处是否被try块所包围。如果没有找到try块，那么它就会返回。否则，它会在try块的对应catch块列表中进行查找。
首先，让我们看看它是如何查找try块的。在编译阶段，编译器给每个try块分配一个起始id和结束id。异常处理器通过funcinfo结构体能够访问到这两个id。看图5。编译器为函数中的每个try块生成了名称为trydata的数据结构。
在上一节中，我谈到了VC++扩展了EXCEPTION_REGISTRATION结构体，增加了id字段。并且，这个数据结构是出现在函数的栈帧上的。看图4。在异常发生时，异常处理器从帧上读取这个id值，判断这个id是否在tryblock结构体的起始id与结束id范围内。如果在，那么异常就发生在这个try块内。否则，异常处理器在tryblocktable的下一个tryblock结构体中查找。
那么是谁在栈上写这个id值呢？这个id值应该是多少呢？编译器会在函数的不同位置添加代码，用来更新能够反映出当前运行状态的id值。例如，编译器会在进入try块时添加代码，而这个代码能够向栈帧上写入此try块的起始id。
一旦异常处理器站到了try块，它能够遍历这个try块对应的catchblock表，来检查是否有catch块希望捕获当前异常。请注意，为了处理嵌套的try块，在内部try块中出现的异常，也会在外部try块中出现。异常处理器应该先查找内部try块的catch块。如果没有找到，它再查找外部try块的catch块。在初始化tryblock表时，VC++将内部try块放在外部try块之前。
异常处理器是如何确定一个catch块（从catchblock结构体获取）希望捕获当前异常呢？通过比较异常类型与catch块的类型参数。如：


C++
```
void foo()
{
   try {
      throw E();
   }
   catch(H) {
      //.
   }
}
```
当H和E类型完全相同时，Catch块能够捕捉这个异常。异常处理必须在运行时比较这两个类型。通常，像C这样的语言不提供运行时对象类型信息。而C++提供运行时类型识别机制（RTTI），有运行时比较类型的标准方法。它在标准头文件<typeinfo>中定义了一个type_info类，它能够在运行时表示一种类型。Catchblock结构体的第二个字段（见图5）是一个指向type_info的指针，它能够表示catch块参数的运行时类型。type_info有operator ==符号重载，可以确定两个类型是否完全相同。因此，所有的异常处理器要做的是，从catchblock结构体中得到参数类型的type_info，与异常对象的type_info进行比较（调用 operator ==），来确定catch块是否希望捕获当前异常。
异常处理器从funcinfo结构体知道catch块的参数类型了，那么它是怎么知道异常的funcinfo呢？当编译器遇到类似下面的语句时：
throw E();
它会为被抛出的异常生成excpt_info结构体。看图6。请注意在不同的VC++编译器上名称可能不同，并且我只列出了部分相关的字段。如图所示，异常的type_info可以通过excpt_info结构体获取到。在某个时间点，异常处理器需要析构异常对象（在catch块执行后）。为了帮助异常处理器做这个工作，编译器把异常的析构器，复制构造器和对象大小写入excpt_info结构体，供异常处理器使用。
![](http://ww2.sinaimg.cn/mw690/6941baebjw1f68dkd22phg20fz07uweb.gif)
如果catch块的参数类型是一个基类，而异常是其基类，那么异常处理器应该能够触发这个catch块。然而，在这里比较两者的typeinfo会得到错误的结果，因为它们的类型并不相等。type_info类也没有提供任何成员函数能够告诉一个类是另一个类的基类。但是，异常处理器必须能够触发这个catch块。为了做到这个效果，编译器为异常处理器生成了更多的信息。如果异常是一个派生类，那么可以从excpt_info结构体中获取到etypeinfo_table，它包含etype_info（type_info的扩展）指针，这个指针能够指向继承层次的所有类。因此异常处理器将catch块参数的type_info，与excpt_info结构体中的所有type_info进行比较。有任何一个比较成功，就会触发catch块。
在我结束这一节之前，还有最后一个问题：异常处理器如何能够感知到异常和except_info结构体？我将在接下来的讨论中尝试回答这个问题。
VC++将throw语句翻译成类似下面的代码：


C++
```
//throw E(); //compiler generates excpt_info structure for E.
E e = E();  //create exception on the stack
_CxxThrowException(&e, E_EXCPT_INFO_ADDR);
```
_CxxThrowException将控制权转交给操作系统（通过软件中断，见RaiseException函数），同时会传递两个参数。操作系统将这两个参数打包成_EXCEPTION_RECORD结构体，然后调用异常回调函数。从FS:[0]指向的EXCEPTION_REGISTRATION列表的头部节点开始，调用此节点的异常处理器。指向这个EXCEPTION_REGISTRATION的指针也是异常处理器的第二个参数。回忆之前的内容，在VC++中，每个函数会在其栈帧上创建自己的EXCEPTION_REGISTRATION并注册。将第二个参数传递给异常处理器，使得它能够获取到重要的信息，如EXCEPTION_REGISTRATION的id字段（用来查找catch块）。此参数同时也使异常处理器感知到函数的栈帧（用来清除栈帧），以及EXCEPTION_REGISTRATION节点在异常列表中的位置（用来栈展开）。第一个参数是指向_EXCEPTION_RECORD结构体的指针，通过它可以获取到异常对象指针，以及excpt_info结构体。异常处理器的函数签名在EXCPT.H中声明：


C++
```
EXCEPTION_DISPOSITION (*handler)(
    _EXCEPTION_RECORD *ExcRecord,
    void * EstablisherFrame, 
    _CONTEXT *ContextRecord,
    void * DispatcherContext);
```
你可以忽略最后的两个参数。返回值类型是一个枚举值（定义见EXCPT.H）。我前面说过，如果异常处理器不能找到catch块，它会返回ExceptionContinueSearch值给操作系统。这种情况下，其他的返回值并不重要。_EXCEPTION_RECORD结构体在WINNT.H中定义：


C++
```
struct _EXCEPTION_RECORD
{
    DWORD ExceptionCode;
    DWORD ExceptionFlags; 
    _EXCEPTION_RECORD *ExcRecord;
    PVOID   ExceptionAddress; 
    DWORD NumberParameters;
    DWORD ExceptionInformation[15]; 
} EXCEPTION_RECORD;
```
ExceptionInformation数组中的数据数量以及入口类型取决于ExceptionCode字段。如果ExceptionCode是一个C++异常（异常代码为0xe06d7363，一般是通过throw抛出的异常），那么ExceptionInformation数组中包含异常对象指针以及excpt_info结构体。对于其他类型的异常，几乎都没有任何入口。其他类型的异常可以是除零错误，内存访问错误等，它们的值可以在WINNT.H中找到。
异常处理器判断_EXCEPTION_RECORD结构体的ExceptionFlags字段，决定采取哪种动作。如果它的值是EH_UNWINDING（在Except.inc中定义），这就表明栈即将被展开，异常处理器应该清理栈帧，然后返回。清理过程涉及到查找在异常发生时，帧上所有存活的局部对象，并调用它们的析构器。下一节中将讨论这个过程。否则，异常处理器应该在函数中查找catch块，如果找到的话，执行它。
### 清理栈帧
C++标准要求，栈展开之前，所有在发生异常时存活的局部对象的析构器都应该被调用。考虑：


C++
```
int g_i = 0;
void foo()
{
   T o1, o2;
   {
       T o3;
   }
   10/g_i; //exception occurs here
   T o4;
   //...
}
```
当异常发生时，局部对象o1和o2在foo的帧上，而o3已经结束了生存周期。O4还没有被创建。异常处理器应该知道这个状况，并调用o1和o2的析构器。
我前面写过，编译器会在函数的不同位置添加代码，在函数运行过程中记录运行时的状态。它给函数的特殊代码段赋id值。例如，try块的入口点是一个特殊的位置。就像我们前面讨论过的那样，编译器会在函数内try块进入的位置添加语句，向函数的帧上写入try块的起始id。
函数中另一个特殊的位置是局部对象创建或者释放的地方。用另一句话说，编译器为每个局部对象赋一个唯一的id值。当编译器遇到如下的对象定义时：


C++
```
void foo()
{
   T t1;
   //.
}
```
它在定义之后增加语句（在对象创建之后）来向帧上写id值：


C++
```
void foo()
{
   T t1;
   _id = t1_id; //statement added by the compiler
   //.
}
```
编译器创建一个隐藏的局部变量（即上面代码中的_id），它与EXCEPTION_REGISTRATION结构体的id字段重叠。同样地，编译器也在对象的析构函数之前增加代码，来写入前面的id。
当异常处理器需要清空帧时，它从帧上读取id值（EXCEPTION_REGISTRATION结构体的id字段，或者帧指针EBP下面的4个字节）。这个id值表明，在此id对应位置之前的所有代码没有产生异常。此位置之前的所有对象都已经创建了。此位置之前的所有或者部分对象的析构器需要被调用。请注意这些对象的一部分可能已经被析构了，因为它们可能位于子代码块内。这些对象的析构器不应该被调用。
编译器还为函数生成了另一数据结构，unwindtable（我取的名字），这是一个展开结构体的数组。可以通过funcinfo结构体获取到它。看图5。函数中的每个特殊代码段，都对应一个展开结构体。Unwindtable中展开结构体的排列顺序，与它们对应的代码在函数中出现的顺序相同。展开结构体对应的对象是我们所关注的（记住，每个对象定义表示一个特殊代码段，并关联一个id）。它也包含析构这个对象的信息。当编译器遇到对象定义，它生产一小段代码，这段代码能够知道此对象在帧上的地址（或者相对于帧指针的偏移量），并释放这个对象。展开结构体的字段之一包含这段代码的地址。


C++
```
typedef  void (*CLEANUP_FUNC)();
struct unwind
{
    int prev;
    CLEANUP_FUNC  cf;
};
```
Try块的展开结构体的第二个字段值为0。Prev字段表明unwindtable是一个展开结构体的链表。当异常处理器需要清理帧时，它从帧上读取id值，并将其作为展开表的索引。它从这个索引处读取展开结构体，调用其第二个字段的清理函数。这将释放此id对应的对象。然后异常处理器通过prev字段读取前一个展开结构体，如此循环，直到链表结束（prev等于-1）。图7展示了函数中展开表的样子。
![](http://ww2.sinaimg.cn/mw690/6941baebjw1f68dkcxc1dg20bu0bsmx0.gif)
考虑new操作符的例子：
T* p = new T();
系统首先为T申请内存，然后调用构造器。如果构造器抛出一个异常，那么系统必须释放已经申请的内存。为了达到这个目的，VC++也为每个new操作符生成一个id，当然只针对有非空构造器的对象。这样在展开表中new操作就会有对应入口，以及释放内存的清理代码。在调用构造器之前，它在EXCEPTION_REGISTRATION结构体中为内存申请写入id。当构造器成功返回后，它恢复之前写入的id。
另外，对象的构造函数抛出异常前，可以已经部分初始化了。如果在异常发生时，对象的子成员对象，或者基类的子成员对象已经构造完成，那么必须调用这些对象的析构器。编译器会为构造器生成一组与前面普通函数中相同的数据，来完成这个任务。
请注意，当栈展开时异常处理器会调用用户自定义的析构器。析构器中可能会抛出异常。C++标准要求，在栈展开时，析构器不能抛出异常。如果真的发生了，系统将调用std::terminate。
### 实现
这一节将讨论3个问题：
A、安装异常处理器；
B、catch块重新抛出异常，或者抛出新的异常；
C、支持每个线程的异常处理。
请阅读源代码中的readme.txt文件，获取构建说明[1]。其中也包含了一个demo项目。
第一个任务是安装异常处理库，或者说，替换掉VC++提供的库。从上面的讨论中已经知道，VC++提供__CxxFrameHandler函数，它是处理所有异常的入口点。对每个函数，编译器为函数内发生的异常生成异常处理代码。这个代码把funcinfo指针传递给__CxxFrameHandler函数。
install_my_handler()函数在__CxxFrameHandler开始处插入代码，能够跳转到my_exc_handler()。但是__CxxFrameHandler驻留在只读代码页，任何写入操作的尝试都会引发禁止访问错误。因此第一步要通过Windows API函数VirtualProtectEx改变这个页权限为读写权限。在内存写入后，我们再恢复成原来的保护权限。这个函数将jmp_instr结构体的内容写入到__CxxFrameHandler的开始处。


C++
```
//install_my_handler.cpp
#include <windows.h>
#include "install_my_handler.h"
//C++'s default exception handler
extern "C" 
EXCEPTION_DISPOSITION  __CxxFrameHandler(
     struct _EXCEPTION_RECORD *ExceptionRecord,
     void * EstablisherFrame,
     struct _CONTEXT *ContextRecord,
     void * DispatcherContext
     );
namespace
{
    char cpp_handler_instructions[5];
    bool saved_handler_instructions = false;
}
namespace my_handler
{
    //Exception handler that replaces C++'s default handler.
    EXCEPTION_DISPOSITION my_exc_handler(
         struct _EXCEPTION_RECORD *ExceptionRecord,
         void * EstablisherFrame,
         struct _CONTEXT *ContextRecord,
         void * DispatcherContext
         ) throw();
#pragma pack(1)
    struct jmp_instr
    {
        unsigned char jmp;
        DWORD         offset;
    };
#pragma pack()
    
    bool WriteMemory(void * loc, void * buffer, int size)
    {
        HANDLE hProcess = GetCurrentProcess();
        
        //change the protection of pages containing range of memory 
        //[loc, loc+size] to READ WRITE
        DWORD old_protection;
        
        BOOL ret;
        ret = VirtualProtectEx(hProcess, loc, size, 
                         PAGE_READWRITE, &old_protection);
        if(ret == FALSE)
            return false;
        ret = WriteProcessMemory(hProcess, loc, buffer, size, NULL);
       
        //restore old protection
        DWORD o2;
        VirtualProtectEx(hProcess, loc, size, old_protection, &o2);
		return (ret == TRUE);
    }
    bool ReadMemory(void *loc, void *buffer, DWORD size)
    {
        HANDLE hProcess = GetCurrentProcess();
        DWORD bytes_read = 0;
        BOOL ret;
        ret = ReadProcessMemory(hProcess, loc, buffer, size, &bytes_read);
        return (ret == TRUE && bytes_read == size);
    }
    bool install_my_handler()
    {
        void * my_hdlr = my_exc_handler;
        void * cpp_hdlr = __CxxFrameHandler;
        jmp_instr jmp_my_hdlr; 
        jmp_my_hdlr.jmp = 0xE9;
        //We actually calculate the offset from __CxxFrameHandler+5
        //as the jmp instruction is 5 byte length.
        jmp_my_hdlr.offset = reinterpret_cast<char*>(my_hdlr) - 
                    (reinterpret_cast<char*>(cpp_hdlr) + 5);
        
        if(!saved_handler_instructions)
        {
            if(!ReadMemory(cpp_hdlr, cpp_handler_instructions,
                        sizeof(cpp_handler_instructions)))
                return false;
            saved_handler_instructions = true;
        }
        return WriteMemory(cpp_hdlr, &jmp_my_hdlr, sizeof(jmp_my_hdlr));
    }
    bool restore_cpp_handler()
    {
        if(!saved_handler_instructions)
            return false;
        else
        {
            void *loc = __CxxFrameHandler;
            return WriteMemory(loc, cpp_handler_instructions, 
                           sizeof(cpp_handler_instructions));
        }
    }
}
```
jmp_instr结构体定义处的#pragma pack(1)指令告诉编译器，这个结构体的内存布局不需要对齐。如果没有这个指令，这个结构体的尺寸将是8个字节。我们定义了这个指令之后，它的大小是5个字节。
回到异常处理，当异常处理器调用catch块时，catch块可能重新抛出这个异常，或者抛出一个新的异常。如果catch块抛出了一个新的异常，那么异常处理器必须释放掉前一个异常对象。如果catch块决定要重新抛出，那么异常处理器必须复制当前异常对象。此时，异常处理器必须解决两个问题：它如何知道catch块中一个新的异常产生？它又如何跟踪旧的异常对象？我解决这个问题的方法是，在异常处理器调用catch块之前，它把当前异常对象存储在exception_storage对象内，并注册一个有特殊目的的异常处理器，catch_block_protector。exception_storage对象可以通过调用函数get_exception_storage()得到：

C++
```
exception_storage* p = get_exception_storage();
p->set(pexc, pexc_info);
register catch_block_protector
call catch block
//....
```
如果在catch块中异常被（重新）抛出，控制权进入catch_block_protector。它可以从exception_storage对象中取出前一个异常对象，当catch块抛出了新异常时释放它。如果catch块重新抛出（可以通过检查ExceptionInformation数组的前两个入口判断，这两个都是0，见下面的代码），那么处理器需要在ExceptionInformation数组中复制它得到当前异常的拷贝。下面是catch_block_protector()函数：


C++
```
//-------------------------------------------------------------------
// If this handler is calles, exception was (re)thrown from catch 
// block. The  exception  handler  (my_handler)  registers this 
// handler before calling the catch block. Its job is to determine
// if the  catch block  threw  new  exception or did a rethrow. If 
// catch block threw a  new  exception, then it should destroy the 
// previous exception object that was passed to the catch block. If 
// the catch block did a rethrow, then this handler should retrieve
// the original exception and save in ExceptionRecord for the 
// exception handlers to use it.
//-------------------------------------------------------------------
EXCEPTION_DISPOSITION  catch_block_protector(
	 _EXCEPTION_RECORD *ExceptionRecord,
	 void * EstablisherFrame,
	 struct _CONTEXT *ContextRecord,
	 void * DispatcherContext
	 ) throw()
{
    EXCEPTION_REGISTRATION *pFrame;
    pFrame = reinterpret_cast<EXCEPTION_REGISTRATION*>
    
    (EstablisherFrame);if(!(ExceptionRecord->ExceptionFlags & (  
          _EXCEPTION_UNWINDING | _EXCEPTION_EXIT_UNWIND)))
    {
        void *pcur_exc = 0, *pprev_exc = 0;
        const excpt_info *pexc_info = 0, *pprev_excinfo = 0;
        exception_storage *p = 
        get_exception_storage();  pprev_exc=
        p->get_exception();  pprev_excinfo=
        p->get_exception_info();p->set(0, 0);
        bool cpp_exc = ExceptionRecord->ExceptionCode == MS_CPP_EXC;
        get_exception(ExceptionRecord, &pcur_exc);
        get_excpt_info(ExceptionRecord, &pexc_info);
        if(cpp_exc && 0 == pcur_exc && 0 ==   pexc_info)
        //rethrow
            {ExceptionRecord->ExceptionInformation[1] = 
                reinterpret_cast<DWORD>
            (pprev_exc);ExceptionRecord->ExceptionInformation[2] = 
                reinterpret_cast<DWORD>(pprev_excinfo);
        }
        else
        {
            exception_helper::destroy(pprev_exc, pprev_excinfo);
        }
    }
    return ExceptionContinueSearch;
}
```
考虑get_exception_storage()函数的一个可能实现方式：


C++
```
exception_storage* get_exception_storage()
{
    static exception_storage es;
    return &es;
}
```
这将是一个完美的实现，除了多线程环境。在超过一个线程的情况下考虑保存这个对象并在其中存储异常对象，将是一个灾难。每个线程有自己的栈，以及异常处理链。我们需要的是一个线程相关的exception_storage对象。每个线程都有自己的对象，在线程启动时创建，并在线程结束时释放。Windows提供了线程局部存储（thread local storage）实现这个功能。线程局部存储使一个对象能够在每个线程内有自己独立的私有拷贝，并通过全局的接口来访问。系统提供了TLSGetValue()和TLSSetValue()函数来实现这个特性。
Excptstorage.cpp文件定义了get_exception_storage()函数。这个文件被编译成DLL。这样做的原因是，它可以确保我们知道任意一个线程的创建与销毁。每次一个线程被创建或销毁，Windows会调用每个DLL（已经被加载到进程的地址空间）的DllMain()函数。这个函数在新创建的线程中被调用。这给了我们一个初始化线程私有数据的机会，即我们例子中的exception_storage对象。

```
//excptstorage.cpp
#include "excptstorage.h"
#include <windows.h>
 
namespace
{
    DWORD dwstorage;
}
 
namespace my_handler
{
    __declspec(dllexport) exception_storage* get_exception_storage() throw()
    {
        void *p = TlsGetValue(dwstorage);
        return reinterpret_cast<exception_storage*>(p);
    }
}
 
BOOL APIENTRY DllMain( HANDLE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                                             )
 
{
    using my_handler::exception_storage;
    exception_storage *p;
    switch(ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        //For the first main thread, this case also contains implicit
        //DLL_THREAD_ATTACH, hence there is no DLL_THREAD_ATTACH for
        //the first main thread.
 
        dwstorage = TlsAlloc(); 
        if(-1 == dwstorage) 
            return FALSE;
 
        p = new exception_storage(); 
        TlsSetValue(dwstorage, p); 
        break;
 
    case DLL_THREAD_ATTACH: 
        p = new exception_storage(); 
        TlsSetValue(dwstorage, p); 
        break;
 
    case DLL_THREAD_DETACH: 
        p = my_handler::get_exception_storage(); 
        delete p;
        break;
 
    case DLL_PROCESS_DETACH:
        p = my_handler::get_exception_storage(); 
        delete p;
        break;
    }
    return TRUE;
}
```
### 总结
如上面讨论过的，C++编译器和运行时异常库，以及操作系统的支持，共同完成异常处理。
### 备注与参考文章
1、 此文进行过程中，Visual Studio 7.0已经发布。我编译与测试异常处理库，主要是在奔腾处理器，Windows 2000系统，VC++ 6.0环境中。我也在VC++ 5.0和VC++ 7.0 beta环境下测试过。在6.0与7.0之间有一些很小的差别。6.0在调用catch块之前，首先在catch块的帧上复制异常（或者其引用），然后执行栈展开。7.0的库先执行栈展开。在这点上，我的库与6.0的库相似。
2、 参看MSDN上Matt Pietrek的精彩文章[结构化异常处理](http://www.microsoft.com/msj/defaultframe.asp?page=/msj/0197/exception/exception.htm&nav=/msj/0197/newnav.htm)。
3、 当一个函数没有try块，并且没有定义任何包含析构非空析构函数的对象时，编译器可能不会生成任何与异常相关的数据。
