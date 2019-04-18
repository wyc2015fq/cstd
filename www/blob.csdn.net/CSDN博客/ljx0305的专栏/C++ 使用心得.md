# C++ 使用心得 - ljx0305的专栏 - CSDN博客
2008年05月15日 20:55:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：720标签：[c++																[reference																[string																[dll																[interface																[vector](https://so.csdn.net/so/search/s.do?q=vector&t=blog)](https://so.csdn.net/so/search/s.do?q=interface&t=blog)](https://so.csdn.net/so/search/s.do?q=dll&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=reference&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
1 std::string不能和""直接比较（都会false），只能用getlength() == 0来比较
其在结构中时候占4×4个字节，其中，第二个4字节是buffer指针，第三个4字节是长度
2 push 结构等类型进stl容器时，必须使用拷贝构造函数（默认拷贝构造函数是位拷贝），否则会有2558等编译错误
java c#把所有对象放在堆上，c++不是
3 无符号数的相减不会出现负数
4  0x2 & 0x2 == 0x2这种前面是必须加括号的（优先级的问题）
5 list的迭代器只能 ＋＋i，不能 i += 2（编译通不过），vector重载了[]，list没有
6 iterator一般具有!=  ==  <  <=  >  >=  +  - 的重载
7 栈，堆和静态区的关系
栈在最上，由高向低生长。
静态区（全局变量，静态变量等）在内存最低区。
静态区之上是堆，由低向高增长。
8不能在循环iterator时对容器进行插入和删除
 vector 可以 erase(it--)的形式在循环里删除
 map连erase(it--)都不行，删除最后一个时会在循环头那里出错，可以复制一个临时map去循环（如果源map小的话）
9 小心else的悬挂问题（else只与最近的if相匹配）
10 vector插入元素时可能整体迁移，erase时是把后面的元素整体迁移一个位置（header不变），后面多出来的空间内容不变
iterator纯粹就是指针
11 编译时的多态：overload和template
     运行时的多态：virtual function
    (multimethod也是运行时多态的一种，可以在有多个对象参与时决定正确的方法调用，比如void DoubleDispatch(Shape& lhs, Shape& rhs)
，Shape有很多子类时，如果进行函数内的动态转换来判断具体子类型，将会非常麻烦)
由于编写代码的时候并不能确定被调用的是基类的函数还是哪个派生类的函数，所以被成为“虚”函数。虚函数只能用指针或引用达到多态的效果（即Base a; a.foo();是没有多态的）；
虚表(VTable)的使用原理：编译器把这样的调用a->foo();转化为虚表的调用(a->vptr[1])();从而实现了运行时决定调用函数（“推迟联编”或“动态联编“）
纯虚函数：virtual void foo()=0; // =0标志一个虚函数为纯虚函数，这个类将不能实例化
一个类的虚函数在它自己的构造函数和析构函数中被调用的时候，它们就变成普通函数了，不“虚”了。
12 Modern C++ Design  = 'generic patterns' or 'pattern templates'
Type-list 是基础。
Generalized Functors ： Functor is a delayed call to a function, a functor, or a member function. It stores the callee and exposes operator() for invoking it.
Multimethods are generalized virtual functions可以在多个类之间进行运行时的方法分派（原虚函数只能是同一个类之间）。
13 c++中的建议（相对于C）
      a 使用const或enum取代宏，注意定义字符串常量时，最好const char * const authorName = "Scott Meyers";指向常量的常量指针
      b 可以在需要的时候才声明变量以立即初始化（c才把所有变量放函数前面）
      c 用new取代malloc
      d 不要使用void *，通常强制类型转换意味着设计错误，static_cast<int*>(malloc(100))（相关类型转换），reinterprete_cast<IO_device*>(0Xff00)（不相关类型转换，最不安全的，甚至不管被转指针是否有效），运行时类型转化检查dynamic_cast<T*>(p)（主要可用于类的指针的转换测试，比如多态时父类指针向下转换，static_cast是不作这种检查的）
      e 不要使用数组和c风格字符串，请使用std::vector和std::string 
      f   为保证系统间的兼容性,不使用int类型(因为不同系统之间的存储字节长度往往不同，比如16位系统里int是2字节)，应使用long或short型
14 两大类stl容器：sequence和assosiative（序列型和关联型）
15 c的str函数到std::string函数的映射
     strcpy  =
     strncpy  =dest.substr(0,n)
     strcat  append函数 或 +=
     strlen  length函数
     strstr  find（初始位置从0开始，找不到是－1）
     strcmp  compare(const basic_string& str)函数（等于时返回0，不等时是－1什么的）
     strncmp  compare(size_type p0, size_type n0, const basic_string& str) （把p0开始拿出n0个元素和str比）
  compare(size_type p0, size_type n0, const basic_string& str, size_type pos, size_type n) （str也只取从pos开始n个元素出来比）
16 已经用过的库，后来不用了，除了把include等去掉外，还要重新完全编译，不然还是报找不到xxx.lib
17 即使÷是赋值语句，也不是原子的，因为RMW的原因（从内存总线读过来，cpu修改，写回内存，针对内存读完就解锁的结构）
18 c++中堆分配比栈分配慢，因为要查找可用的"洞"。而java全部都是堆分配，但它采用"传送带"的形式，总是往后分配，所以堆分配也比较快。最简单的GC垃圾回收方案是引用计数。
19 由于map和set是用排序结构存储的（红黑树），所以不再提供sort功能，如果你想这样做，必须将他们拷入vector之类进行。
20 强指针 ＝ 智能指针 auto_ptr(c++标准规定的，但是很差，不能放进容器等，因此建议使用boost或loki中的智能指针)
     弱指针 ＝ 传统的指针
21 malloc/free 和 new/delete的主要区别： 函数/运算符    new/delete会调用构造和析构（malloc/free不会）
malloc出来的内存指针是void*，因此要自己转换，它只能用于内部数据类型
22 进线程同步方法
临界区：一次只能有一个线程进入代码段。主要是为实现线程之间同步的，但是使用的时候注意，一定要在用此临界对象同步的线程之外建立该对象（一般在主线程中建立临界对象）。 
互斥体：可以跨进程同步，并且可以给个字符串的命名
信号量：特殊的互斥体，但所管资源数可以大于1
23 extern关键字
为了实现C和C++的混合编程，C++提供了C连接交换指定符号extern "C"来解决名字匹配问题，函数声明前加上extern "C"后，则编译器就会按照C语言的方式将该函数编译为_foo，这样C语言中就可以调用C++的函数了。
The extern keyword declares a variable or function and specifies that it has external linkage (its name is visible from files other than the one in which it's defined). When modifying a variable, extern specifies that the variable has static duration (it is allocated when the program begins and deallocated when the program ends). The variable or function may be defined in another source file, or later in the same file. In C++, when used with a string, extern specifies that the linkage conventions of another language are being used for the declarator(s).
"C" is the only other language specifier currently supported by Microsoft C/C++. 
声明外部链接函数的例子：
extern "C" int printf( const char *, ... );
extern "C"
{
   int getchar( void );
   int putchar( int );
}
24 const放在函数后面
表示这个函数在函数体内不能改变类成员变量
25 函数内部定义的static变量，也是在编译时就确定地址并初始化了的（也就是说，应该是存储在全局静态存储区）
26 4G内存在win32中布局
0 － 4M 保留给dos
4M －2G程序私有
2G － 3G程序间共享
3G - 4G系统区
27 标准c/c++库没有itoa函数，可以用boost中的string转数字的lexical_cast
using std::string;
const double d = 123.12;
string s = boost::lexical_cast<string>(d); //出错是抛出异常的方式
18 软件复用
1）dll级别的复用由于调用着记忆了要调用函数的位置等，导致dll必须版本匹配，直接导致dll hell的发生（dll本身带着独立的函数和数据，这样具有相当的独立性，程序集成后容易实现错误隔离），"无法定位程序输入点xxx于xxx.dll上"
2）而com级复用时，com是个二进制的标准，外部只访问com的公共内存结构，所以可以忽略不同编程语言，应用环境之间的区别，解决了重新编译发行的问题
3）组件级复用
4）框架复用：设计和分析的复用
19 如果两个类之间的继承关系为私有，编译器一般不会将派生类对象（如Student）转换成基类对象（如Person）;如果使类D私有继承于类B，这样做是因为你想利用类B中已经存在的某些代码，而不是因为类型B的对象和类型D的对象之间有什么概念上的关系。
20 volatile 告诉编译器这个变量可能被非语句更改的，不要在优化时对诸如连续写同样的值这样的操作做任何假设（即不要想当然地优化它们）；
int volatile nVint;
又比如指向设备寄存器的指针volatile unsigned short* pP2LTCH = (unsigned short *) 0X7200005E;不要理解为这个指针是易失的，而要理解为这个指针指向的寄存器值是易失的。
21 ActiveX相对于其它COM组件而言最舒服的地方是属性和事件可以直接在“属性”面版中调整，一般是ocx控件，而一般com组件是以dll的形式提供
22 面向策略编程，主要是把大的类分成很多单一功能的类（某一方面？）
A policy defines a class interface or a class template interface.The interface consists of one or all of the following: inner type definitions, member functions, and member variables.
23 ANSI C定义的几个（6个）基本宏：
__TIMESTAMP__    __FILE__  __LINE__等 (每个项目或模块应该有统一的调测开关或打印函数，比如都带有文件名和行号)
24 DLL相关
"D L L中函数的代码创建的任何对象均由调用线程所拥有，而D L L本身从来不拥有任何东西"
"D L L可以包含对话框模板、字符串、图标和位图等资源，在D L L中通常没有用来处理消息循环或创建窗口的支持代码"
DLL的两种连接方式：
1) 隐式连接：
隐式是常用的方式（链接时要引用lib文件，让应用程序的源代码只引用D L L中包含的符号）
"如果链接程序发现D L L的源代码模块至少输出了一个函数或变量，那么链接程序也生成 一个. l i b文件。这个. l i b文件很小，因为它不包含任何函数或变量。它只是列出所有已输出函数 和变量的符号名。"
2) 显示连接：
"DLL的lib文件是不需要的，因为并不直接引用输出符号。 .exe 文件不包含输入表。"
"一个线程调用LoadLibrary (Ex)函数，将DLL加载到进程的地址空间这时线程可以调用GetProcAddress以便间接引用DLL的输出符号。"
25 Fundamental to COM are these concepts: 
Interfaces: the mechanism through which an object exposes its functionality.
IUnknown: the basic interface on which all others are based. It implements the reference counting and interface querying mechanisms running through COM.
Reference counting: the technique by which an object (or, strictly, an interface) decides when it is no longer being used and is therefore free to remove itself.
QueryInterface: the method used to query an object for a given interface.
Marshaling: the mechanism that enables objects to be used across thread, process, and network boundaries, allowing for location independence.
Aggregation: a way in which one object can make use of another. 
26 ATL is the Active Template Library, a set of template-based C++ classes with which you can easily create small, fast Component Object Model (COM) objects. It has special support for key COM features including: stock implementations of IUnknown, IClassFactory, IClassFactory2 and IDispatch; dual interfaces; standard COM enumerator interfaces; connection points; tear-off interfaces; and ActiveX controls.
ATL是在VC++环境下实现COM/ActiveX的一套C++库（以模板的方式提供的）。
27 ActiveX :
A set of technologies that enables software components to interact with one another in a networked environment, regardless of the language in which they were created. ActiveX is built on the Component Object Model (COM). 
28 为防止SendMessage函数导致发送者永远挂起（比如接收者处理有误），可以使用SendMessageTimeout
29 数据对齐并不是操作系统的内存结构的一部分，而是C P U结构的一部分。
当C P U访问正确对齐的数据时，它的运行效率最高。当数据大小的数据模数的内存地址是0时，数据是对齐的。例如， W O R D值应该总是从被2除尽的地址开始，而D W O R D值应该总是从被4除尽的地址开始，如此等等。
#pragma pack(2)
....
#pragma pack() //恢复默认
28 必须使用初始式而不是构造函数内初始化的地方
1）如果类存在继承关系，派生类必须在其初始化表里调用基类的构造函数。
2）类的const常量只能在初始化表里被初始化，因为它不能在函数体内用赋值的方式来初始化
29 com的双接口IUnknown和IDispatch(IUnknown是必须实现的)
Methods in Vtable Order
IUnknown Methods:
QueryInterface Returns pointers to supported interfaces. 
AddRef Increments the reference count. 
Release Decrements the reference count. 
IDispatch Methods:
GetTypeInfoCount Retrieves the number of type information interfaces. 
GetTypeInfo Retrieves the type information for an object. 
GetIDsOfNames Maps a single member and an optional set of argument names to a corresponding set of integer DISPIDs. 
Invoke Provides access to properties and methods exposed by an object. 
30 当c++类有对象成员时，不再提供默认的拷贝构造函数，编译会报错。可改成指针，并在构造和析构里创建和释放。
31 Each new thread receives its own stack space, consisting of both committed and reserved memory. By default, each thread uses 1 MB of reserved memory, and one page of committed memory.
32 ++前后置效果
    aa=0;
    j = (aa++) + aa;//j=0,aa=1
    aa =0;
    j = (aa++) + aa++;//j=0,aa=2
    aa =0;
    j = (aa++) + (++aa);//j=2,aa=2
33 数组和指针的比较
数组要么在静态存储区（用于存全局和static变量等）被创建（如全局数组），要么在栈上被创建。数组名对应着（而不是指向）一块内存，其地址与容量在生命期内保持不变，只有数组的内容可以改变。
char a[] = "hello";//注意sizeof(a)==6 (实际长度加1)，字符数组的sizeof和指针的sizeof是不同的, char *p; sizeof(p)==4；单纯的char aaa[];通不过编译
a[0] ='X';
cout << a << endl;
char b[10];
strcpy(b, a); // 不能用 b = a; 这是通不过编译的，报告"cannot convert from 'char [6]' to 'char [10]'"
char *p = "world"; // 注意p指向常量字符串
p[0] = ‘X’; // 编译器不能发现该错误，运行时才报错，另外sizeof(a)=6 而 sizeof(p)=4
34 Windows 和 linux
创建过程：CreateProcess 和 fork
线程：CreateThread 和 pthread_create
线程同步：WaitForSingleObject() and WaitForMultipleObjects() 和 pthread_join() 
35 关于protected关键字
protected keyword specifies that those members are accessible only from member functions and friends of the class and its derived classes. This applies to all members declared up to the next access specifier or the end of the class.
When preceding the name of a base class, the protected keyword specifies that the public and protected members of the base class are protected members of the derived class.
Default access of members in a class is private. Default access of members in a structure or union is public.
36 完成端口（windows 2000或xp以上才支持）
CreateIoCompletionPort（创建一个完成端口）  GetQueuedCompletionStatus（在一个完成端口上取一个事件）  PostQueuedCompletionStatus（主动向某个完成端口发一个事件）
异步IO在两种平台上的实现：
Windows： The Windows ReadFile() and WriteFile() system functions can either perform synchronous I/O or initiate an overlapped I/O operation.
POSIX.：The POSIX aio_read() and aio_write() functions initiate asynchronous read and write operations, respectively. These functions are separate from the read() and write() (and Sockets recv() and send()) functions that are used in ACE's IPC wrapper facade classes (see Chapter 3 in C++NPv1).
先了解什么是信号/非信号态,什么是等待函数,什么是成功等待的副作用,什么是线程挂起等？
37 apache源码中的apachemonitor可以轻易转换成别的服务的windows图形管理台，xml分析模块也可以直接嵌进别的应用使用
注意：编译apache时要把sdk的include目录放到前面（vc option那里）
38 函数内的字符常量在栈内存，不是全局存储区
char *GetMemory(void)
{
    char p[]="hello";//注意如果是char *p="hello";则p指向的内存跑到全局常量区（该区内存不能改写）去了，函数返回后全局区的hello还在（即使调用其他函数导致堆栈变化）
    int i = sizeof(p);//一样＝6
    return p;
}
void CMsgbox2Dlg::OnButton11() 
{
    char * p = GetMemory();
    char pp[10];
    int i=0;
    i++;
    memcpy (pp,p,6); //只要这里调用了其他函数，p的内容就变乱码了，说明char p[]的形式p的内存在堆栈区
    AfxMessageBox(pp);
}
39 宏的副作用
#define MIN(A,B) ((A) <= (B) ? (A) : (B))
int a=2,b=3;
int fff = MIN(a++,b);
//a=4,fff=3
int a=4,b=3;
int fff = MIN(a++,b);
//a=5,fff=3
40 引用和指针
本来引用必须指向一个对象的，但如果有人这样写，可能产生不可预知的结果。
char *pc = 0;          // set pointer to null
char& rc = *pc;        // make reference refer to
                       // dereferenced null pointer
41 不管是int a[10]; 还是 int *a;  a = new int[7];
*(a+7)始终相当于a[7]
42 为什么拷贝函数的参数必须是引用的
因为不这样的话，参数本身也会是个拷贝（而且还是位拷贝）
43 c++返回类对象没有很好的办法（返回指针，实例，引用都有各自的缺陷），有些编译器提出了变通的办法。
44 CObject is the principal base class for the Microsoft Foundation Class Library. It serves as the root not only for library classes such as CFile and CObList, but also for the classes that you write. CObject provides basic services, including 
1)Serialization support
2)Run-time class information
3)Object diagnostic output
4)Compatibility with collection classes 
Note that CObject does not support multiple inheritance.
另外也有一些MFC类不是从CObject继承的，比如一些工具类：CString，CTime等
引用:http://blog.csdn.net/stephenxu111/archive/2008/05/09/2424520.aspx
