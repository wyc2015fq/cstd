# 一个跨平台的 C++ 内存泄漏检测器 - ljx0305的专栏 - CSDN博客
2009年02月20日 22:12:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：465标签：[跨平台																[c++																[delete																[编译器																[compiler																[borland](https://so.csdn.net/so/search/s.do?q=borland&t=blog)](https://so.csdn.net/so/search/s.do?q=compiler&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=delete&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=跨平台&t=blog)
个人分类：[C++](https://blog.csdn.net/ljx0305/article/category/380566)
2004 年 3 月 01 日
> 内存泄漏对于C/C++程序员来说也可以算作是个永恒的话题了吧。在Windows下，MFC的一个很有用的功能就是能在程序运行结束时报告是否发生了内存泄漏。在Linux下，相对来说就没有那么容易使用的解决方案了：像mpatrol之类的现有工具，易用性、附加开销和性能都不是很理想。本文实现一个极易于使用、跨平台的C++内存泄漏检测器。并对相关的技术问题作一下探讨。
基本使用
对于下面这样的一个简单程序test.cpp：
int main()
{
	int* p1 = new int;
	char* p2 = new char[10];
	return 0;
}
我们的基本需求当然是对于该程序报告存在两处内存泄漏。要做到这点的话，非常简单，只要把debug_new.cpp也编译、链接进去就可以了。在Linux下，我们使用：
g++ test.cpp debug_new.cpp -o test
输出结果如下所示：
Leaked object at 0x805e438 (size 10, <Unknown>:0)
Leaked object at 0x805e410 (size 4, <Unknown>:0)
如果我们需要更清晰的报告，也很简单，在test.cpp开头加一行
#include "debug_new.h"
即可。添加该行后的输出如下：
Leaked object at 0x805e438 (size 10, test.cpp:5)
Leaked object at 0x805e410 (size 4, test.cpp:4)
非常简单！
![](http://www.ibm.com/i/v14/rules/blue_rule.gif)
![](http://www.ibm.com/i/c.gif)|![](http://www.ibm.com/i/c.gif)|![](http://www.ibm.com/i/v14/icons/u_bold.gif)|[**回页首**](http://writeblog.csdn.net/#main)||![](http://www.ibm.com/i/v14/icons/u_bold.gif)|[**回页首**](http://writeblog.csdn.net/#main)|
|----|----|----|
|![](http://www.ibm.com/i/v14/icons/u_bold.gif)|[**回页首**](http://writeblog.csdn.net/#main)| |
背景知识
在new/delete操作中，C++为用户产生了对operator new和operator delete的调用。这是用户不能改变的。operator new和operator delete的原型如下所示：
void *operator new(size_t) throw(std::bad_alloc);
void *operator new[](size_t) throw(std::bad_alloc);
void operator delete(void*) throw();
void operator delete[](void*) throw();
对于"new int"，编译器会产生一个调用"operator new(sizeof(int))"，而对于"new char[10]"，编译器会产生"operator new[](sizeof(char) * 10)"（如果new后面跟的是一个类名的话，当然还要调用该类的构造函数）。类似地，对于"delete ptr"和"delete[] ptr"，编译器会产生"operator delete(ptr)"调用和"operator delete[](ptr)"调用（如果ptr的类型是指向对象的指针的话，那在operator delete之前还要调用对象的析构函数）。当用户没有提供这些操作符时，编译系统自动提供其定义；而当用户自己提供了这些操作符时，就覆盖了编译系统提供的版本，从而可获得对动态内存分配操作的精确跟踪和控制。
同时，我们还可以使用placement new操作符来调整operator new的行为。所谓placement new，是指带有附加参数的new操作符，比如，当我们提供了一个原型为
void* operator new(size_t size, const char* file, int line);
的操作符时，我们就可以使用"new("hello", 123) int"来产生一个调用"operator new(sizeof(int), "hello", 123)"。这可以是相当灵活的。又如，C++标准要求编译器提供的一个placement new操作符是
void* operator new(size_t size, const std::nothrow_t&);
其中，nothrow_t通常是一个空结构（定义为"struct nothrow_t {};"），其唯一目的是提供编译器一个可根据重载规则识别具体调用的类型。用户一般简单地使用"new(std::nothrow) 类型"（nothrow是一个nothrow_t类型的常量）来调用这个placement new操作符。它与标准new的区别是，new在分配内存失败时会抛出异常，而"new(std::nothrow)"在分配内存失败时会返回一个空指针。
要注意的是，没有对应的"delete(std::nothrow) ptr"的语法；不过后文会提到另一个相关问题。
要进一步了解以上关于C++语言特性的信息，请参阅[Stroustrup1997]，特别是6.2.6、10.4.11、15.6、19.4.5和B.3.4节。这些C++语言特性是理解本实现的关键。
![](http://www.ibm.com/i/v14/rules/blue_rule.gif)
![](http://www.ibm.com/i/c.gif)|![](http://www.ibm.com/i/c.gif)|![](http://www.ibm.com/i/v14/icons/u_bold.gif)|[**回页首**](http://writeblog.csdn.net/#main)||![](http://www.ibm.com/i/v14/icons/u_bold.gif)|[**回页首**](http://writeblog.csdn.net/#main)|
|----|----|----|
|![](http://www.ibm.com/i/v14/icons/u_bold.gif)|[**回页首**](http://writeblog.csdn.net/#main)| |
检测原理
和其它一些内存泄漏检测的方式类似，debug_new中提供了operator new重载，并使用了宏在用户程序中进行替换。debug_new.h中的相关部分如下：
void* operator new(size_t size, const char* file, int line);
void* operator new[](size_t size, const char* file, int line);
#define new DEBUG_NEW
#define DEBUG_NEW new(__FILE__, __LINE__)
拿上面加入debug_new.h包含后的test.cpp来说，"new char[10]"在预处理后会变成"new("test.cpp", 4) char[10]"，编译器会据此产生一个"operator new[](sizeof(char) * 10, "test.cpp", 4)"调用。通过在debug_new.cpp中自定义"operator new(size_t, const char*, int)"和"operator delete(void*)"（以及"operator new[]…"和"operator delete[]…"；为避免行文累赘，以下不特别指出，说到operator new和operator delete均同时包含数组版本），我可以跟踪所有的内存分配调用，并在指定的检查点上对不匹配的new和delete操作进行报警。实现可以相当简单，用map记录所有分配的内存指针就可以了：new时往map里加一个指针及其对应的信息，delete时删除指针及对应的信息；delete时如果map里不存在该指针为错误删除；程序退出时如果map里还存在未删除的指针则说明有内存泄漏。
不过，如果不包含debug_new.h，这种方法就起不了作用了。不仅如此，部分文件包含debug_new.h，部分不包含debug_new.h都是不可行的。因为虽然我们使用了两种不同的operator new --"operator new(size_t, const char*, int)"和"operator new(size_t)"-- 但可用的"operator delete"还是只有一种！使用我们自定义的"operator delete"，当我们删除由"operator new(size_t)"分配的指针时，程序将认为被删除的是一个非法指针！我们处于一个两难境地：要么对这种情况产生误报，要么对重复删除同一指针两次不予报警：都不是可接受的良好行为。
看来，自定义全局"operator new(size_t)"也是不可避免的了。在debug_new中，我是这样做的：
void* operator new(size_t size)
{
	return operator new(size, "<Unknown>", 0);
}
但前面描述的方式去实现内存泄漏检测器，在某些C++的实现中（如GCC 2.95.3中带的SGI STL）工作正常，但在另外一些实现中会莫名其妙地崩溃。原因也不复杂，SGI STL使用了内存池，一次分配一大片内存，因而使利用map成为可能；但在其他的实现可能没这样做，在map中添加数据会调用operator new，而operator new会在map中添加数据，从而构成一个死循环，导致内存溢出，应用程序立即崩溃。因此，我们不得不停止使用方便的STL模板，而使用手工构建的数据结构：
struct new_ptr_list_t
{
	new_ptr_list_t*		next;
	const char*			file;
	int					line;
	size_t				size;
};
我最初的实现方法就是每次在使用new分配内存时，调用malloc多分配 sizeof(new_ptr_list_t) 个字节，把分配的内存全部串成一个一个链表（利用next字段），把文件名、行号、对象大小信息分别存入file、line和size字段中，然后返回(malloc返回的指针 + sizeof(new_ptr_list_t))。在delete时，则在链表中搜索，如果找到的话（(char*)链表指针 + sizeof(new_ptr_list_t) == 待释放的指针），则调整链表、释放内存，找不到的话报告删除非法指针并abort。
至于自动检测内存泄漏，我的做法是生成一个静态全局对象（根据C++的对象生命期，在程序初始化时会调用该对象的构造函数，在其退出时会调用该对象的析构函数），在其析构函数中调用检测内存泄漏的函数。用户手工调用内存泄漏检测函数当然也是可以的。
基本实现大体就是如此。
![](http://www.ibm.com/i/v14/rules/blue_rule.gif)
![](http://www.ibm.com/i/c.gif)|![](http://www.ibm.com/i/c.gif)|![](http://www.ibm.com/i/v14/icons/u_bold.gif)|[**回页首**](http://writeblog.csdn.net/#main)||![](http://www.ibm.com/i/v14/icons/u_bold.gif)|[**回页首**](http://writeblog.csdn.net/#main)|
|----|----|----|
|![](http://www.ibm.com/i/v14/icons/u_bold.gif)|[**回页首**](http://writeblog.csdn.net/#main)| |
可用性改进
上述方案最初工作得相当好，直到我开始创建大量的对象为止。由于每次delete时需要在链表中进行搜索，平均搜索次数为(链表长度/2)，程序很快就慢得像乌龟爬。虽说只是用于调试，速度太慢也是不能接受的。因此，我做了一个小更改，把指向链表头部的new_ptr_list改成了一个数组，一个对象指针放在哪一个链表中则由它的哈希值决定。--用户可以更改宏DEBUG_NEW_HASH和DEBUG_NEW_HASHTABLESIZE的定义来调整debug_new的行为。他们的当前值是我测试下来比较满意的定义。
使用中我们发现，在某些特殊情况下（请直接参看debug_new.cpp中关于DEBUG_NEW_FILENAME_LEN部分的注释），文件名指针会失效。因此，目前的debug_new的缺省行为会复制文件名的头20个字符，而不只是存储文件名的指针。另外，请注意原先new_ptr_list_t的长度为16字节，现在是32字节，都能保证在通常情况下内存对齐。
此外，为了允许程序能和 new(std::nothrow) 一起工作，我也重载了operator new(size_t, const std::nothrow_t&) throw()；不然的话，debug_new会认为对应于 new(nothrow) 的delete调用删除的是一个非法指针。由于debug_new不抛出异常（内存不足时程序直接报警退出），所以这一重载的操作只不过是调用 operator new(size_t) 而已。这就不用多说了。
前面已经提到，要得到精确的内存泄漏检测报告，可以在文件开头包含"debug_new.h"。我的惯常做法可以用作参考：
#ifdef _DEBUG
#include "debug_new.h"
#endif
包含的位置应当尽可能早，除非跟系统的头文件（典型情况是STL的头文件）发生了冲突。在某些情况下，可能会不希望debug_new重定义new，这时可以在包含debug_new.h之前定义DEBUG_NEW_NO_NEW_REDEFINITION，这样的话，在用户应用程序中应使用debug_new来代替new（顺便提一句，没有定义DEBUG_NEW_NO_NEW_REDEFINITION时也可以使用debug_new代替new）。在源文件中也许就该这样写：
#ifdef _DEBUG
#define DEBUG_NEW_NO_NEW_REDEFINITION
#include "debug_new.h"
#else
#define debug_new new
#endif
并在需要追踪内存分配的时候全部使用debug_new（考虑使用全局替换）。
用户可以选择定义DEBUG_NEW_EMULATE_MALLOC，这样debug_new.h会使用debug_new和delete来模拟malloc和free操作，使得用户程序中的malloc和free操作也可以被跟踪。在使用某些编译器的时候（如Digital Mars C++ Compiler 8.29和Borland C++ Compiler 5.5.1），用户必须定义NO_PLACEMENT_DELETE，否则编译无法通过。用户还可以使用两个全局布尔量来调整debug_new的行为：new_verbose_flag，缺省为false，定义为true时能在每次new/delete时向标准错误输出显示跟踪信息；new_autocheck_flag，缺省为true，即在程序退出时自动调用check_leaks检查内存泄漏，改为false的话用户必须手工调用check_leaks来检查内存泄漏。
需要注意的一点是，由于自动调用check_leaks是在debug_new.cpp中的静态对象析构时，因此不能保证用户的全局对象的析构操作发生在check_leaks调用之前。对于Windows上的MSVC，我使用了"#pragma init_seg(lib)"来调整对象分配释放的顺序，但很遗憾，我不知道在其他的一些编译器中（特别是，我没能成功地在GCC中解决这一问题）怎么做到这一点。为了减少误报警，我采取的方式是在自动调用了check_leaks之后设new_verbose_flag为true；这样，就算误报告了内存泄漏，随后的delete操作还是会被打印显示出来。只要泄漏报告和delete报告的内容一致，我们仍可以判断出没有发生内存泄漏。
Debug_new也能检测对同一指针重复调用delete（或delete无效指针）的错误。程序将显示错误的指针值，并强制调用abort退出。
还有一个问题是异常处理。这值得用专门的一节来进行说明。
![](http://www.ibm.com/i/v14/rules/blue_rule.gif)
![](http://www.ibm.com/i/c.gif)|![](http://www.ibm.com/i/c.gif)|![](http://www.ibm.com/i/v14/icons/u_bold.gif)|[**回页首**](http://writeblog.csdn.net/#main)||![](http://www.ibm.com/i/v14/icons/u_bold.gif)|[**回页首**](http://writeblog.csdn.net/#main)|
|----|----|----|
|![](http://www.ibm.com/i/v14/icons/u_bold.gif)|[**回页首**](http://writeblog.csdn.net/#main)| |
构造函数中的异常
我们看一下以下的简单程序示例：
#include <stdexcept>
#include <stdio.h>
void* operator new(size_t size, int line)
{
	printf("Allocate %u bytes on line %d//n", size, line);
	return operator new(size);
}
class Obj {
public:
	Obj(int n);
private:
	int _n;
};
Obj::Obj(int n) : _n(n)
{
	if (n == 0) {
		throw std::runtime_error("0 not allowed");
	}
}
int main()
{
	try {
		Obj* p = new(__LINE__) Obj(0);
		delete p;
	} catch (const std::runtime_error& e) {
		printf("Exception: %s//n", e.what());
	}
}
看出代码中有什么问题了吗？实际上，如果我们用MSVC编译的话，编译器的警告信息已经告诉我们发生了什么：
test.cpp(27) : warning C4291: 'void *__cdecl operator new(unsigned int,int)' : 
no matching operator delete found; memory will not be freed if initialization throws an exception
好，把debug_new.cpp链接进去。运行结果如下：
Allocate 4 bytes on line 27 Exception: 0 not allowed Leaked object at 00342BE8 (size 4, <Unknown>:0) 	
啊哦，内存泄漏了不是！
当然，这种情况并非很常见。可是，随着对象越来越复杂，谁能够保证一个对象的子对象的构造函数或者一个对象在构造函数中调用的所有函数都不会抛出异常？并且，解决该问题的方法并不复杂，只是需要编译器对 C++ 标准有较好支持，允许用户定义 placement delete 算符（[C++1998]，5.3.4节；网上可以找到1996年的标准草案，比如下面的网址 [http://www.comnets.rwth-aachen.de/doc/c++std/expr.html#expr.new](http://www.comnets.rwth-aachen.de/doc/c++std/expr.html#expr.new)）。在我测试的编译器中，GCC（2.95.3或更高版本，Linux/Windows）和MSVC（6.0或更高版本）没有问题，而Borland C++ Compiler 5.5.1和Digital Mars C++ Compiler（到v8.38为止的所有版本）则不支持该项特性。在上面的例子中，如果编译器支持的话，我们就需要声明并实现 operator delete(void*, int) 来回收new分配的内存。编译器不支持的话，需要使用宏让编译器忽略相关的声明和实现。如果要让debug_new在Borland C++ Compiler 5.5.1或Digital Mars C++ Compiler下编译的话，用户必须定义宏NO_PLACEMENT_DELETE；当然，用户得自己注意小心构造函数中抛出异常这个问题了。 
![](http://www.ibm.com/i/v14/rules/blue_rule.gif)
![](http://www.ibm.com/i/c.gif)|![](http://www.ibm.com/i/c.gif)|![](http://www.ibm.com/i/v14/icons/u_bold.gif)|[**回页首**](http://writeblog.csdn.net/#main)||![](http://www.ibm.com/i/v14/icons/u_bold.gif)|[**回页首**](http://writeblog.csdn.net/#main)|
|----|----|----|
|![](http://www.ibm.com/i/v14/icons/u_bold.gif)|[**回页首**](http://writeblog.csdn.net/#main)| |
方案比较
IBM developerWorks上刊载了洪琨先生设计实现的一个Linux上的内存泄漏检测方法（[洪琨2003]）。我的方案与其相比，主要区别如下：
优点： 
- 跨平台：只使用标准函数，并且在GCC 2.95.3/3.2（Linux/Windows）、MSVC 6、Digital Mars C++ 8.29、Borland C++ 5.5.1等多个编译器下调试通过。（虽然Linux是我的主要开发平台，但我发现，有时候能在Windows下编译运行代码还是非常方便的。） 
- 易用性：由于重载了operator new(size_t)--洪琨先生只重载了operator new(size_t, const char*, int)--即使不包含我的头文件也能检测内存泄漏；程序退出时能自动检测内存泄漏；可以检测用户程序（不包括系统/库文件）中malloc/free产生的内存泄漏。 
- 灵活性：有多个灵活的可配置项，可使用宏定义进行编译时选择。 
- 可重入性：不使用全局变量，没有嵌套delete问题。 
- 异常安全性：在编译器支持的情况下，能够处理构造函数中抛出的异常而不发生内存泄漏。 
缺点： 
- 单线程模型：跨平台的多线程实现较为麻烦，根据项目的实际需要，也为了代码清晰简单起见，我的方案不是线程安全的；换句话说，如果多个线程中同时进行new或delete操作的话，后果未定义。 
- 未实现运行中内存泄漏检测报告机制：没有遇到这个需求J；不过，如果要手工调用check_leaks函数实现的话也不困难，只是跨平台性就有点问题了。 
- 不能检测带 [] 算符和不带 [] 算符混用的不匹配：主要也是需求问题（如果要修改实现的话并不困难）。 
- 不能在错误的delete调用时显示文件名和行号：应该不是大问题；由于我重载了operator new(size_t)，可以保证delete出错时程序必然有问题，因而我不只是显示警告信息，而且会强制程序abort，可以通过跟踪程序、检查abort时程序的调用栈知道问题出在哪儿。 
另外，现在已存在不少商业和Open Source的内存泄漏检测器，本文不打算一一再做比较。Debug_new与它们相比，功能上总的来说仍较弱，但是，其良好的易用性和跨平台性、低廉的附加开销还是具有很大优势的。
![](http://www.ibm.com/i/v14/rules/blue_rule.gif)
![](http://www.ibm.com/i/c.gif)|![](http://www.ibm.com/i/c.gif)|![](http://www.ibm.com/i/v14/icons/u_bold.gif)|[**回页首**](http://writeblog.csdn.net/#main)||![](http://www.ibm.com/i/v14/icons/u_bold.gif)|[**回页首**](http://writeblog.csdn.net/#main)|
|----|----|----|
|![](http://www.ibm.com/i/v14/icons/u_bold.gif)|[**回页首**](http://writeblog.csdn.net/#main)| |
总结和讨论
以上段落基本上已经说明了debug_new的主要特点。下面做一个小小的总结。
重载的算符： 
- operator new(size_t, const char*, int) 
- operator new[](size_t, const char*, int) 
- operator new(size_t) 
- operator new[](size_t) 
- operator new(size_t, const std::nothrow_t&) 
- operator new[](size_t, const std::nothrow_t&) 
- operator delete(void*) 
- operator delete[](void*) 
- operator delete(void*, const char*, int) 
- operator delete[](void*, const char*, int) 
- operator delete(void*, const std::nothrow_t&) 
- operator delete[](void*, const std::nothrow_t&) 
提供的函数： 
- check_leaks() 
检查是否发生内存泄漏 
提供的全局变量 
- new_verbose_flag 
是否在new和delete时"罗嗦"地显示信息 
- new_autocheck_flag 
是否在程序退出是自动检测一次内存泄漏 
可重定义的宏： 
- NO_PLACEMENT_DELETE 
假设编译器不支持placement delete（全局有效） 
- DEBUG_NEW_NO_NEW_REDEFINITION 
不重定义new，假设用户会自己使用debug_new（包含debug_new.h时有效） 
- DEBUG_NEW_EMULATE_MALLOC 
重定义malloc/free，使用new/delete进行模拟（包含debug_new.h时有效） 
- DEBUG_NEW_HASH 
改变内存块链表哈希值的算法（编译debug_new.cpp时有效） 
- DEBUG_NEW_HASHTABLE_SIZE 
改变内存块链表哈希桶的大小（编译debug_new.cpp时有效） 
- DEBUG_NEW_FILENAME_LEN 
如果在分配内存时复制文件名的话，保留的文件名长度；为0时则自动定义DEBUG_NEW_NO_FILENAME_COPY（编译debug_new.cpp时有效；参见文件中的注释） 
- DEBUG_NEW_NO_FILENAME_COPY 
分配内存时不进行文件名复制，而只是保存其指针；效率较高（编译debug_new.cpp时有效；参见文件中的注释） 
我本人认为，debug_new目前的一个主要缺陷是不支持多线程。对于某一特定平台，要加入多线程支持并不困难，难就难在通用上（当然，条件编译是一个办法，虽然不够优雅）。等到C++标准中包含线程模型时，这个问题也许能比较完美地解决吧。另一个办法是使用像boost这样的程序库中的线程封装类，不过，这又会增加对其它库的依赖性--毕竟boost并不是C++标准的一部分。如果项目本身并不用boost，单为了这一个目的使用另外一个程序库似乎并不值得。因此，我自己暂时就不做这进一步的改进了。
另外一个可能的修改是保留标准operator new的异常行为，使其在内存不足的情况下抛出异常（普通情况）或是返回NULL（nothrow情况），而不是像现在一样终止程序运行（参见debug_new.cpp的源代码）。这一做法的难度主要在于后者：我没想出什么方法，可以保留 new(nothrow) 的语法，同时能够报告文件名和行号，并且还能够使用普通的new。不过，如果不使用标准语法，一律使用debug_new和debug_new_nothrow的话，那还是非常容易实现的。
如果大家有改进意见或其它想法的话，欢迎来信讨论。
debug_new 的源代码目前可以在 [dbg_new.zip](http://writeblog.csdn.net/dbg_new.zip)处下载。 
在这篇文章的写完之后，我终于还是实现了一个线程安全的版本。该版本使用了一个轻量级的跨平台互斥体类fast_mutex（目前支持Win32和POSIX线程，在使用GCC（Linux/MinGW）、MSVC时能通过命令行参数自动检测线程类型）。有兴趣的话可在 [http://mywebpage.netscape.com/yongweiwu/dbg_new.tgz](http://mywebpage.netscape.com/yongweiwu/dbg_new.tgz)下载。 
参考资料 
[C++1998] ISO/IEC 14882. Programming Languages-C++, 1st Edition. International Standardization Organization, International Electrotechnical Commission, American National Standards Institute, and Information Technology Industry Council, 1998
[Stroustrup1997] Bjarne Stroustrup. The C++ Programming Language, 3rd Edition. Addison-Wesley, 1997
[洪琨2003] 洪琨。 [《如何在 linux 下检测内存泄漏》](http://writeblog.csdn.net/developerworks/cn/linux/l-mleak/index.html)，IBM developerWorks 中国网站。 
关于作者
![](http://www.ibm.com/i/c.gif)
吴咏炜，目前在Linux上从事高性能入侵检测系统的研发。对于开发跨平台、高性能、可重用的C++代码有着浓厚的兴趣。 [adah@sh163.net](mailto:adah@sh163.net?cc=adah@netstd.com)可以跟他联系
引用:http://blog.csdn.net/adcxf/archive/2009/01/18/3824209.aspx
