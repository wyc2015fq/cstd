# __declspec 使用说明 - ljx0305的专栏 - CSDN博客
2009年02月06日 13:08:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：873
**__declspec**
__cdecl和__stdcall都是函数调用规范(还有一个__fastcall),规定了参数出入栈的顺序和方法,
如果只用VC编程的话可以不用关心,但是要在C++和Pascal等其他语言通信的时候就要注意了,只有用相同的方法才能够调用成功.
另外,像printf这样接受可变个数参数的函数只有用cdecl才能够实现.   
__declspec主要是用于说明DLL的引出函数的,在某些情况下用__declspec(dllexport)在DLL中生命引出函数,比用传统的DEF文件方便一些.在普通程序中也可以用__declspec(dllimport)说明函数是位于另一个DLL中的导出函数.
例子不太好举啊,其实就是在函数声明的时候多加一个关键字,比如很多API函数就是象这样声明的:   
  int   WINAPI   MessageBoxA(HWND,LPCSTR,LPSTR,UINT);   
  而WINAPI实际上就是__stdcall.   
  大多数API都采用__stdcall调用规范,这是因为几乎所有的语言都支持__stdcall调用.相比之下,__cdecl只有在C语言中才能用.但是__cdecl调用有一个特点,就是能够实现可变参数的函数调用,比如printf,这用__stdcall调用是不可能的.   
  __fastcall这种调用规范比较少见,但是在Borland   C++   Builder中比较多的采用了这种调用方式.   
  如果有共享代码的需要,比如写DLL,推荐的方法是用__stdcall调用,因为这样适用范围最广.如果是C++语言写的代码供Delphi这样的语言调用就必须声明为__stdcall,因为Pascal不支持cdecl调用(或许Delphi的最新版本能够支持也说不定,这个我不太清楚).在其他一些地方,比如写COM组件,几乎都用的是stdcall调用.在VC或Delphi或C++Builder里面都可以从项目设置中更改默认的函数调用规范,当然你也可以在函数声明的时候加入__stdcall,__cdecl,__fastcall关键字来明确的指示本函数用哪种调用规范.   
  __declspec一般都是用来声明DLL中的导出函数.这个关键字也有一些其他的用法,不过非常罕见.
关于DLL的函数：   
  动态链接库中定义有两种函数：导出函数(export   function)和内部函数(internal   function)。   
  导出函数可以被其它模块调用，内部函数在定义它们的DLL程序内部使用。   
  输出函数的方法有以下几种：   
  1、传统的方法   
  在模块定义文件的EXPORT部分指定要输入的函数或者变量。语法格式如下：   
  entryname[=internalname]   [@ordinal[NONAME]]   [DATA]   [PRIVATE]   
  其中：   
  entryname是输出的函数或者数据被引用的名称；   
  internalname同entryname;   
  @ordinal表示在输出表中的顺序号(index)；   
  NONAME仅仅在按顺序号输出时被使用（不使用entryname）；   
  DATA表示输出的是数据项，使用DLL输出数据的程序必须声明该数据项为_declspec(dllimport)。   
  上述各项中，只有entryname项是必须的，其他可以省略。   
  对于“C”函数来说，entryname可以等同于函数名；但是对“C++”函数（成员函数、非成员函数）   
  来说，entryname是修饰名。可以从.map映像文件中得到要输出函数的修饰名，或者使用   
  DUMPBIN   /SYMBOLS得到，然后把它们写在.def文件的输出模块。DUMPBIN是VC提供的一个工具。   
  如果要输出一个“C++”类，则把要输出的数据和成员的修饰名都写入.def模块定义文件。   
  2、在命令行输出   
  对链接程序LINK指定/EXPORT命令行参数，输出有关函数。   
  3、使用MFC提供的修饰符号_declspec(dllexport)   
  在要输出的函数、类、数据的声明前加上_declspec(dllexport)的修饰符，表示输出。__declspec   
  (dllexport)在C调用约定、C编译情况下可以去掉输出函数名的下划线前缀。extern   "C"使得在C++中   
  使用C编译方式成为可能。在“C++”下定义“C”函数，需要加extern   “C”关键词。用extern   "C"来   
  指明该函数使用C编译方式。输出的“C”函数可以从“C”代码里调用。   
  例如，在一个C++文件中，有如下函数：   
  extern   "C"   {void   __declspec(dllexport)   __cdecl   Test(int   var);}   
  其输出函数名为：Test     
  MFC提供了一些宏，就有这样的作用。   
  AFX_CLASS_IMPORT：__declspec(dllexport)   
  AFX_API_IMPORT：__declspec(dllexport)   
  AFX_DATA_IMPORT：__declspec(dllexport)   
  AFX_CLASS_EXPORT：__declspec(dllexport)   
  AFX_API_EXPORT：__declspec(dllexport)   
  AFX_DATA_EXPORT：__declspec(dllexport)   
  AFX_EXT_CLASS：   #ifdef   _AFXEXT     
  AFX_CLASS_EXPORT   
  #else   
  AFX_CLASS_IMPORT   
  AFX_EXT_API：#ifdef   _AFXEXT   
  AFX_API_EXPORT   
  #else   
  AFX_API_IMPORT   
  AFX_EXT_DATA：#ifdef   _AFXEXT   
  AFX_DATA_EXPORT   
  #else   
  AFX_DATA_IMPORT   
  像AFX_EXT_CLASS这样的宏，如果用于DLL应用程序的实现中，则表示输出（因为_AFX_EXT被定义，通   
  常是在编译器的标识参数中指定该选项/D_AFX_EXT）；如果用于使用DLL的应用程序中，则表示输入   
  （_AFX_EXT没有定义）。   
  要输出整个的类，对类使用_declspec(_dllexpot)；要输出类的成员函数，则对该函数使用   
  _declspec(_dllexport)。如：   
  class   AFX_EXT_CLASS   CTextDoc   :   public   CDocument   
  {   
  …   
  }   
  extern   "C"   AFX_EXT_API   void   WINAPI   InitMYDLL();   
  这几种方法中，最好采用第三种，方便好用；其次是第一种，如果按顺序号输出，调用效率会高些；   
  最次是第二种。     
  六、模块定义文件(.DEF)   
  模块定义文件(.DEF)是一个或多个用于描述DLL属性的模块语句组成的文本文件，每个DEF文件至少必   
  须包含以下模块定义语句：   
  *   第一个语句必须是LIBRARY语句，指出DLL的名字；   
  *   EXPORTS语句列出被导出函数的名字；将要输出的函数修饰名罗列在EXPORTS之下，这个名字必须与   
  定义函数的名字完全一致，如此就得到一个没有任何修饰的函数名了。   
  *   可以使用DESCRIPTION语句描述DLL的用途(此句可选)；   
  *   ";"对一行进行注释(可选)。   
  七、DLL程序和调用其输出函数的程序的关系   
  1、dll与进程、线程之间的关系   
  DLL模块被映射到调用它的进程的虚拟地址空间。   
  DLL使用的内存从调用进程的虚拟地址空间分配，只能被该进程的线程所访问。   
  DLL的句柄可以被调用进程使用；调用进程的句柄可以被DLL使用。   
  DLL使用调用进程的栈。   
  2、关于共享数据段   
  DLL定义的全局变量可以被调用进程访问；DLL可以访问调用进程的全局数据。使用同一DLL的每一个   
  进程都有自己的DLL全局变量实例。如果多个线程并发访问同一变量，则需要使用同步机制；对一个   
  DLL的变量，如果希望每个使用DLL的线程都有自己的值，则应该使用线程局部存储(TLS，Thread     
  Local   Strorage)。   
  在程序里加入预编译指令，或在开发环境的项目设置里也可以达到设置数据段属性的目的.必须给   
  这些变量赋初值,否则编译器会把没有赋初始值的变量放在一个叫未被初始化的数据段中。   
  extern   "C"   指示编译器用C语言方法给函数命名。   
  在制作DLL导出函数时由于C++存在函数重载，因此   
  __declspec(dllexport)   function(int,int)   在DLL会被decorate，例如被decorate成为   function_int_int，而且不同的编译器decorate的方法不同，造成了在用GetProcAddress取得function地址时的不便，使用extern   "C"时，上述的decorate不会发生，因为C没有函数重载，但如此一来被extern"C"修饰的函数，就不具备重载能力，可以说extern   和   extern   "C"不是以回事。
__declspec(dllexport)   声明一个导出函数，是说这个函数要从本DLL导出。我要给别人用。一般用于dll中
__declspec(dllimport)   声明一个导入函数，是说这个函数是从别的DLL导入。我要用。一般用于使用某个dll的exe中  
_declspec(thread)可以降低程序员的负担，又能做到线程局部存储的要求。VC++允许一个变量或结构被声明为“具有线程局部性”。例如，下面的声明，如果放在一个DLL之中，将产生出一个全局变量，对每一个进程而言独一无二：   
  DWORD   gProgressCounter；   
  但是如果这样声明，它就是对每一个线程独一无二：   
  _declspec(thread)     DWORD   gProgressCounter;   
  每一个以这种方式声明对象的EXE和DLL，将在可执行文件中有一个特殊的节区(section),内含所有的线程局部变量。当EXE或DLL被载入时，操作系统会认识这个节区并适当的处理之。这个节区会被操作系统社定为“对每一个线程具有局部性”。   
  当一个EXE被载入时，操作系统扫描可执行文件以及所有静态链接(implicity   linked)的DLLs，以便找出所有的线程局部节区。所有节区的大小被加总在一起以求出每个线程启动时应该配置的内存数量。
1. __declspec(align(16)) struct SS{ int a,b; };
 它与#pragma pack()是一对兄弟，前者规定了对齐的最小值，后者规定了对齐的最大值。同时出现时，前者优先级高。 __declspec(align())的一个特点是，它仅仅规定了数据对齐的位置，而没有规定数据实际占用的内存长度，当指定的数据被放置在确定的位置之后，其后的数据填充仍然是按照#pragma pack规定的方式填充的，这时候类/结构的实际大小和内存格局的规则是这样的：在__declspec(align())之前，数据按照#pragma pack规定的方式填充，如前所述。当遇到__declspec(align())的时候，首先寻找距离当前偏移向后最近的对齐点（满足对齐长度为max(数据自身长度,指定值))，然后把被指定的数据类型从这个点开始填充，其后的数据类型从它的后面开始，仍然按照#pragma pack填充，直到遇到下一个__declspec(align())。当所有数据填充完毕，把结构的整体对齐数值和__declspec(align())规定的值做比较，取其中较大的作为整个结构的对齐长度。 特别的，当__declspec(align())指定的数值比对应类型长度小的时候，这个指定不起作用。
2. #pragma section("segname",read)
    / __declspec(allocate("segname")) int i = 0;
    / int main(){ return 1;};
 此关键词必须跟随code_seg,const_seg,data_seg,init_seg,section关键字之后使用，以上例子使用了section关键字。使用此关键字将告知编译器，其后的变量间被分配在那个数据段。
3. __declspec(deprecated(MY_TEXT)) void func(int) {}
 与pragma deprecated()相同。此声明后，如果在同一作用域中使用func(int)函数，将被提醒c4996警告。
4. __declspec( dllimport ) declarator
   & __declspec( dllexport ) declarator
 无须多说，此二关键字用于导入导出外接元素。
5. __declspec(jitintrinsic)
 用于标记一个函数或元素为64位公共语言运行时。具体用法未见到。
6. __declspec( naked ) int func( formal_parameters ) {}
 此关键字仅用于x86系统，多用于硬件驱动。此关键字可以使编译器在生成代码时不包含任何注释或标记。仅可以对函数的定义使用，不能用于数据声明、定义，或者函数的声明。
7. __declspec(restrict) float * init(int m, int n) {};
   & __declspec(noalias) void multiply(float * a, float * b, float * c) {}；// 优化必用！
 __declspec(restrict)仅适用于返回指针的函数声明，如 __declspec(restrict) void *malloc(size_t size);restrict declspec 适用于返回非别名指针的函数。此关键字用于 malloc 的 C 运行时库实现，因为它决不会返回已经在当前程序中使用的指针值（除非您执行某个非法操作，如在内存已被释放之后使用它）。restrict declspec 为编译器提供执行编译器优化的更多信息。对于编译器来说，最大的困难之一是确定哪些指针会与其他指针混淆，而使用这些信息对编译器很有帮助。有必要指出，这是对编译器的一个承诺，编译器并不对其进行验证。如果您的程序不恰当地使用 restrict declspec，则该程序的行为会不正确。 __declspec(noalias)也是仅适用于函数，它指出该函数是半纯粹的函数。半纯粹的函数是指仅引用或修改局部变量、参数和第一层间接参数。此 declspec 是对编译器的一个承诺，如果该函数引用全局变量或第二层间接指针参数，则编译器会生成将中断应用程序的代码。
8. class X {
   / __declspec(noinline) int mbrfunc() { return 0; /* will not inline*/ };
 在类中声明一个函数不需要内联。
9. __declspec(noreturn) extern void fatal () {}
 不需要返回值。
10. void __declspec(nothrow) __stdcall f2();
 不存在异常抛出。
11. struct __declspec(novtable) X { virtual void mf(); };
    / struct Y : public X {void mf() {printf_s("In Y/n");}};
 此关键字标记的类或结构不能直接实例化，否则将引发AV错误(access violation)。此关键字的声明将阻止编译器对构造和析构函数的vfptr的初始化。可优化编译后代码大小。
12. struct S {   int i;
    / void putprop(int j) {  i = j; }
    / int getprop() { return i; }
    / __declspec(property(get = getprop, put = putprop)) int the_prop;};
 此关键字与C#中get & set属性相同，可定义实现针对一个字段的可读或可写。以上例子，可以使用(如果实例化S为ss)如：ss.the_prop = 156;(此时，ss.i == 156)接着如果：cout<< s.the_prop;(此时将调用getprop，使返回156)。
13. __declspec(selectany)（转）
 在MFC，ATL的源代码中充斥着__declspec(selectany)的声明。selectany可以让我们在.h文件中初始化一个全局变量而不是只能放在.cpp中。比如有一个类，其中有一个静态变量，那么我们可以在.h中通过类似__declspec(selectany) type class::variable = value; 这样的代码来初始化这个全局变量。既是该.h被多次include，链接器也会为我们剔除多重定义的错误。对于template的编程会有很多便利。
14. __declspec(thread) int in_One_Thread;
 声明in_One_Thread为线程局部变量并具有线程存储时限，以便链接器安排在创建线程时自动分配的存储。
15. struct __declspec(uuid("00000000-0000-0000-c000-000000000046")) IUnknown;
 将具有唯一表示符号的已注册内容声明为一个变量，可使用__uuidof()调用。
引用:http://blog.chinaunix.net/u2/61797/showart_1003918.html
