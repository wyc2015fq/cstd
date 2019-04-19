# Visual Studio下Debug和Release区别 - 三少GG - CSDN博客
2013年03月09日 10:48:48[三少GG](https://me.csdn.net/scut1135)阅读数：4664
# [Debug和Release区别](http://www.cnblogs.com/JemBai/archive/2009/01/13/1374805.html)
最近写代码过程中，发现 Debug 下运行正常，Release 下就会出现问题，百思不得其解，而Release 下又无法进行调试，于是只能采用printf方式逐步定位到问题所在处，**才发现原来是给定的一个数组未初始化，导致后面处理异常。**网上查找了些资料，在这
 罗列汇总下，做为备忘～ 
一、Debug 和 Release 的区别 
        Debug 通常称为调试版本，它包含调试信息，并且不作任何优化，便于程序员调试程序。Release 称为发布版本，它往往是进行了各种优化，使得程序在代码大小和运行速度上都是最优的，以便用户很好地使用。 
**Debug 和 Release 的真正区别，在于一组编译选项。 **
Debug 版本   
参数       含义   
/MDd /MLd 或 /MTd 使用 Debug runtime library(调试版本的运行时刻函数库)   
/Od 关闭优化开关   
/D "_DEBUG" 相当于 #define _DEBUG,打开编译调试代码开关(主要针对assert函数)   
/ZI   
创建 Edit and continue(编辑继续)数据库，这样在调试过程中如果修改了源代码不需重新编译   
GZ 可以帮助捕获内存错误  
Release 版本 参数含义   
/MD /ML 或 /MT 使用发布版本的运行时刻函数库   
/O1 或 /O2 优化开关，使程序最小或最快   
/D "NDEBUG" 关闭条件编译调试代码开关(即不编译assert函数)   
/GF 合并重复的字符串，并将字符串常量放到只读内存，防止被修改  
Debug 和 Release 并没有本质的界限，他们只是一组编译选项的集合，编译器只是按照预定的选项行动。 
相关经验： 转自[http://dev.csdn.net/article/17/17068.shtm](http://dev.csdn.net/article/17/17068.shtm)
1. 变量。 
大家都知道，**debug跟release在初始化变量时所做的操作是不同的**，debug是将每个字节位都赋成0xcc(注1)，而release的赋值近 似于随机(我想是直接从内存中分配的，没有初始化过)。这样就明确了，如果你的程序中的某个变量没被初始化就被引用，就很有可能出现异常：用作控制变量将
 导致流程导向不一致；用作数组下标将会使程序崩溃；更加可能是造成其他变量的不准确而引起其他的错误。所**以在声明变量后马上对其初始化一个默认的值是最简 单有效的办法，否则项目大了你找都没地方找。**代码存在错误在debug方式下可能会忽略而不被察觉到，如debug方式下数组越界也大多不会出错，在 release中就暴露出来了，这个找起来就比较难了:(
 还是自己多加注意吧 呵呵，就是我犯的问题～～ 
2. 自定义消息的消息参数。 
MFC为我们提供了很好的消息机制，更增加了自定义消息，好处我就不用多说了。这也存在debug跟release的问题吗？答案是肯定的。在自定义消息 的函数体声明时，时常会看到这样的写法：afx_msg LRESULT OnMessageOwn(); Debug情况下一般不会有任何问题，而当你在Release下且多线程或进程间使用了消息传递时就会导致无效句柄之类的错误。导致这个错误直接原因是消 息体的参数没有添加，即应该写成：afx_msg LRESULT
 OnMessageOwn(WPARAM wparam, LPARAM lparam); (注2) 
3. release模式下不出错，但debug模式下报错。 
这种情况下大多也是因为代码书写不正确引起的，查看MFC的源码，可以发现好多ASSERT的语句(断言)，这个宏只是在debug模式下才有效，那么就 清楚了，**release版不报错是忽略了错误而不是没有错误，这可能存在很大的隐患，**因为是Debug模式下，比较方便调试，好好的检查自己的代码，再此
 就不多说了。 
4. ASSERT, VERIFY, TRACE..........调试宏 
这种情况很容易解释。举个例子：请在VC下输入ASSERT然后选中按F12跳到宏定义的地方，这里你就能够发现Debug中ASSERT要执行 AfxAssertFailedLine，而Release下的宏定义却为"#define ASSERT(f) ((void)0)"。所以注意在这些调试宏的语句不要用程序相关变量如i++写操作的语句。VERIFY是个例外，"#define VERIFY(f) ((void)(f))"，即执行，这里的作用就不多追究了，有兴趣可自己研究:)。
总结： 
Debug与Release不同的问题在刚开始编写代码时会经常发生，99%是因为你的代码书写错误而导致的，所以不要动不动就说系统问题或编译器问题， 努力找找自己的原因才是根本。我从前就常常遇到这情况，经历过一次次的教训后我就开始注意了，现在我所写过的代码我已经好久没遇到这种问题了。下面是几个 避免的方面，即使没有这种问题也应注意一下： 
1. 注意变量的初始化，尤其是指针变量，数组变量的初始化(很大的情况下另作考虑了)。 
2. 自定义消息及其他声明的标准写法 
3. 使用调试宏时使用后最好注释掉 
4. 尽量使用try - catch(...) 
5. 尽量使用模块，不但表达清楚而且方便调试。 
注1： 
debug版初始化成0xcc是因为0xcc在x86下是一条int 3单步中断指令，这样程序如果跑飞了遇到0xcc就会停下来，这和单片机编程时一般将没用的代码空间填入jmp 0000语句是一样地转贴于：计算机二级考试_考试大【责编:drfcy 纠错】
[VC]DEBUG和RELEASE2007年08月26日 星期日 下午 04:33    I.    内存分配问题   
   1.    变量未初始化。下面的程序在debug中运行的很好。   
   thing    *    search(thing    *    something)   
   BOOL    found;   
   for(int    i    =    0;    i    <    whatever.GetSize();    i++)   
   {   
   if(whatever[i]->field    ==    something->field)   
   {    /*    found    it    */   
   found    =    TRUE;   
   break;   
   }    /*    found    it    */   
   }   
   if(found)   
   return    whatever[i];   
   else   
   return    NULL;   
   而在release中却不行，因为debug中会自动给变量初始化found=FALSE,而在release版中则不会。所以尽可能的给变量、类或结构初始化。   
   2.    数据溢出的问题   
   如：char    buffer[10];   
   int    counter;   
   lstrcpy(buffer,    "abcdefghik");   
   在debug版中buffer的NULL覆盖了counter的高位，但是除非counter>16M,什么问题也没有。但是在release版 中，counter可能被放在寄存器中，这样NULL就覆盖了buffer下面的空间，可能就是函数的返回地址，这将导致ACCESS    ERROR。   
   3.    DEBUG版和RELEASE版的内存分配方式是不同的    。如果你在DEBUG版中申请    ele    为    6*sizeof(DWORD)=24bytes,实际上分配给你的是32bytes（debug版以32bytes为单位分配），    而在release版，分配给你的就是24bytes（release版以8bytes为单位），所以在debug版中如果你写ele[6],可能不会有 什么问题，而在release版中，就有ACCESS   
 VIOLATE。   
   II.    ASSERT和VERIFY   
   1.    ASSERT在Release版本中是不会被编译的。   
   ASSERT宏是这样定义的   
   #ifdef    _DEBUG   
   #define    ASSERT(x)    if(    (x)    ==    0)    report_assert_failure()   
   #else   
   #define    ASSERT(x)   
   #endif   
   实际上复杂一些，但无关紧要。假如你在这些语句中加了程序中必须要有的代码   
   比如   
   ASSERT(pNewObj    =    new    CMyClass);   
   pNewObj->MyFunction();   
   这种时候Release版本中的pNewObj不会分配到空间   
   所以执行到下一个语句的时候程序会报该程序执行了非法操作的错误。这时可以用VERIFY    ：   
   #ifdef    _DEBUG   
   #define    VERIFY(x)    if(    (x)    ==    0)    report_assert_failure()   
   #else   
   #define    VERIFY(x)    (x)   
   #endif   
   这样的话，代码在release版中就可以执行了。   
   III.    参数问题：   
   自定义消息的处理函数，必须定义如下：   
   afx_msg    LRESULT    OnMyMessage(WPARAM,    LPARAM);   
   返回值必须是HRESULT型，否则Debug会过，而Release出错   
   IV.    内存分配   
   保证数据创建和清除的统一性：如果一个DLL提供一个能够创建数据的函数，那么这个DLL同时应该提供一个函数销毁这些数据。数据的创建和清除应该在同一个层次上。   
   V.    DLL的灾难   
   人们将不同版本DLL混合造成的不一致性形象的称为    “动态连接库的地狱“(DLL    Hell)    ，甚至微软自己也这么说[http://msdn.microsoft.com/library/techart/dlldanger1.htm](http://msdn.microsoft.com/library/techart/dlldanger1.htm))。   
   如果你的程序使用你自己的DLL时请注意：   
   1.    **不能将debug和release版的DLL混合在一起使用。debug都是debug版，release版都是release版。  **
   解决办法是将debug和release的程序分别放在主程序的debug和release目录下   
   2.    千万不要以为静态连接库会解决问题，那只会使情况更糟糕。   
   VI.    RELEASE板中的调试    ：   
   1.    将ASSERT()    改为    VERIFY()    。找出定义在"#ifdef    _DEBUG"中的代码，如果在RELEASE版本中需要这些代码请将他们移到定义外。查找TRACE(...)中代码，因为这些代码在RELEASE中 也不被编译。    请认真检查那些在RELEASE中需要的代码是否并没有被便宜。   
   2.    变量的初始化所带来的不同，在不同的系统，或是在DEBUG/RELEASE版本间都存在这样的差异，所以请对变量进行初始化。   
   3.    是否在编译时已经有了警告?请将警告级别设置为3或4,然后保证在编译时没有警告出现.   
   VII.    将Project    Settings"    中    "C++/C    "    项目下优化选项改为Disbale（Debug）。编译器的优化可能导致许多意想不到的错误，请参[http://www.pgh.net/~newcomer/debug_release.htm](http://www.pgh.net/~newcomer/debug_release.htm)
   1.    此外对RELEASE版本的软件也可以进行调试，请做如下改动：   
   在"Project    Settings"    中    "C++/C    "    项目下设置    "category"    为    "General"    并且将"Debug    Info"设置为    "Program    Database"。   
   在"Link"项目下选中"Generate    Debug    Info"检查框。   
   "Rebuild    All"   
   如此做法会产生的一些限制：   
   无法获得在MFC    DLL中的变量的值。   
   必须对该软件所使用的所有DLL工程都进行改动。   
   另：   
   MS    BUG：MS的一份技术文档中表明，在VC5中对于DLL的"Maximize    Speed"优化选项并未被完全支持，因此这将会引起内存错误并导致程序崩溃。   
   2.    [http://www.sysinternals.com/](http://www.sysinternals.com/)有 一个程序DebugView，用来捕捉OutputDebugString的输出，运行起来后（估计是自设为system   
 debugger）就可以观看所有程序的OutputDebugString的输出。此后，你可以脱离VC来运行你的程序并观看调试信息。   
   3.    有一个叫Gimpel    Lint的静态代码检查工具，据说比较好用[http://www.gimpel.com/](http://www.gimpel.com/)    不过要化$的。
