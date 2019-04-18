# C++ 智能指针 - gauss的专栏 - CSDN博客
2013年01月04日 22:17:30[gauss](https://me.csdn.net/mathlmx)阅读数：220
**[智能指针](http://en.wikipedia.org/wiki/Smart_pointer)**（Smart Pointer），是一个来用存储指向动态分配（堆）对象指针的类。简单的说，它本身是一个类，这个类是用来存储对象指针。
**一、智能指针的介绍**
智能指针，就是具备指针功能同时提供安全内存回收的一个类。它是用一个对象来对指针进行建模，使之具有指针的特性，跟指针具有相同含义的
 -> 和 * 操作．并且通过对象的构造函数（获取资源），析构资源（释放资源）来对资源进行管理，从而减少程序员对通过new操作获取到的对象的生命周期进行管理的负担。
除了能够在适当的时间自动删除指向的对象外，他们的工作机制很像C++的内置指针。智能指针在面对异常的时候格外有用，因为他们能够确保正确的销毁动态分配的对象，他们也可以用于跟踪被多用户共享的动态分配对象。
事实上，智能指针能够做的还有很多事情，例如处理线程安全，提供写时复制，确保协议，并且提供远程交互服务。有能够为这些ESP (Extremely Smart Pointers)创建一般智能指针的方法，但是并没有涵盖进来。智能指针的大部分使用是用于生存期控制，阶段控制。它们使用operator->和operator*来生成原始指针，这样智能指针看上去就像一个普通指针。
**二、智能指针的原理**
,当类中有指针成员时，一般有两种方式来管理指针成员：
1）采用值型的方式管理，每个类对象都保留一份指针指向的对象的拷贝；
2）更优雅的方式是使用智能指针，从而实现指针指向的对象的共享。智能指针的一种通用实现技术是使用引用计数(reference count)。智能指针类将一个计数器与类指向的对象相关联，引用计数跟踪该类有多少个对象共享同一指针，具体实现步骤如下：1）每次创建类的新对象时，初始化指针并将引用计数置为1；
2）当对象作为另一对象的副本而创建时，拷贝构造函数拷贝指针并增加与之相应的引用计数（加1）；
3）对一个对象进行赋值时，赋值操作符减少左操作数所指对象的引用计数（减1，如果引用计数为减至0，则删除对象），并增加右操作数所指对象的引用计数（加1）；
4）调用析构函数时，析构函数减少引用计数（减1，如果引用计数减至0，则删除基础对象）。
**三、 智能指针的特点**
指针可以分为泛型指针，原生指针、智能指针，它们的含义和区别如下：
1. 泛型指针
泛型指针，是指数据类型不确定的指针，它有多种含义：
(1) 指void*指针，可以指向任意数据类型，因此具有“泛型”含义。
(2) 指具有指针特性的泛型数据结构，包含泛型的迭代器、智能指针等。
广义的迭代器是一种不透明指针，能够实现遍历访问操作。通常所说的迭代器是指狭义的迭代器，即基于C++的STL中基于泛型的iterator_traits实现的类的实例。
总体来说，泛型指针和迭代器是两个不同的概念，其中的交集则是通常提到的迭代器类。
2. 原生指针
原生指针，就是普通指针，与它相对的是使用起来行为上像指针，但却不是指针。
说“原生”是指“最简朴最基本的那一种”。因为现在很多东西都抽象化理论化了，所以“以前的那种最简朴最基本的指针”只是一个抽象概念（比如iterator）的表现形式之一。
3. 智能指针
智能指针，是C++里面的概念，由于 C++ 语言没有自动内存回收机制，程序员每次得自己处理内存相关问题，但用智能指针便可以有效缓解这类问题，引入智能指针可以防止出现悬垂指针的情况。
一般是把指针封装到一个称之为智能指针类中，这个类中另外还封装了一个使用计数器，对指针的复制等操作将导致该计数器的值加1，对指针的delete操作则会减1，值为0时，释放指针，并指针为NULL

**四、智能指针的实现**
根据《Moden C++ Design》，我们可以构造具有很多正交特性的智能指针。
1）C++中的智能指针与JAVA中的对象
前段时间跟朋友聊了些有关JAVA的东西，感觉上Java中的对象就是C++中的智能指针，但具有不同的资源释放方式。
在JAVA中，创建实例时，不能像C++中运用" A a;" 语句声明得到一个类(A)的实例a，而必须通过语句 Ａa = new A() 来获得；在释放a时，应用必需通知GC（垃圾收集功能）来释放该实例所占用的资源。
当然，JAVA中的对象有一小点同C++中的职能不同，因为在C++中指针不具有"."操作符，故智能指针一般也不提供"."操作符，但在Java中都是通过"."操作符对对象进行操作的，不过我们可以把C++中智能指针的"->"操作符与Java中的"."操作符进行类比**。**
2）引用计数型智能指针
C++中有一种常用的智能指针，是引用计数型智能指针：RCSmartPtr。 它的实现基理如下：
首先，存在RCObject，即存在一个对象，该对象提供引用计数接口。
其次，要存在指向RCObject的RCSmartPtr对象，在RCSmartPtr对象的构造过程中，把指向RCObject的指针作为参数传入RCSmartPtr中。因此每增加一个RCSmartPtr对象，就多了一个指向RCObject的指针。RCSmartPtr可以通过调用RCObject的引用计数接口，增加RCObject的引用计数。同样的道理可以在RCSmartPtr对象的析构函数中调用RCObject的引用记数接口来减少RCObject的引用记数。
最后，在对RCObject的引用计数进行操作时对引用计数进行检查，如果引用计数为0,则RCObject将摧毁本身，从而释放该对象所占用的资源。
通过这种方式，我们就可以把对资源的管理交给机器来管理，解除了对人工的倚赖。

**五、C++智能指针的应用分析**
前段时间，在查控件的内存泄露时，最终找出一个错误：
在使用XMLDom(COM)时，由于重复使用某接口指针前未释放Dispatch指针(Release)，而导致内存泄露，而此类错误(如同BSTR类型的泄漏)，VC的调试器和Bondcheck均无能为力。解决办法，似乎只有细心一途，但只要稍稍仔细看看，就可发现，实际上如果正确使用VC提供的智能指针，是可以避免此问题的。
另外，一直为Java程序员津津乐道的内存使用无需管理的优势，一直知道用C++的智能指针可以模拟。但一直没实际动手做过，趁此分析之机，用C++简单包装了一个。反正粗看之下，可以达到与Java类似的效果，当然，C++的对象更高效且节省内存。
就以上所提到的，简单罗列几点：
**1）关于纠错，MFC和ATL中智能指针的应用**
1. 在Windows中如何方便的查看当前进程使用的内存。
虽然代码简单，但对纠错时有大用处，不用不停的通过切换任务管理器来查看内存使用。代码如下：
UINT C_BaseUtil::getProcessMemoryUsed()
{
　UINT uiTotal = 0L;
　HANDLE hProcess = ::GetCurrentProcess();
　PROCESS_MEMORY_COUNTERS pmc;
　if(::GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
　　uiTotal = pmc.WorkingSetSize;
　return uiTotal;
}
注意：由于内存使用会是一个不稳定的过程，所以，需要在程序稳定时进行调用，才能准确。
**2）在使用Com的Dispatch指针时，如果不使用COM智能指针，容易出现的错误**
2.1. 忘记在所有出口释放指针。
IXMLDOMDocument *pDoc = NULL;
CoCreateInstance(...)
……
pDoc->Release();
错误：如果中间代码发生异常，则pDoc未能正常释放，造成内存泄露。
2.2. 重复使用同一指针变量，导致中间生成的Dispatch指针未能释放。
IXMLDOMNode *pNode = NULL;
if(FAILED(pDoc->selectSingleNode(_bstr_t("Workbook"), &pNode)) || pNode==NULL)
throw(_T("selectSingleNode failed!"));
if(FAILED(pDoc->selectSingleNode(_bstr_t("Workbook"), &pNode)) || pNode==NULL)
throw(_T("selectSingleNode failed!"));
错误：pNode未释放就开始第二次调用，造成内存泄露。或者类似pNode = pNode2的这种写法，也随手就出问题了。
必须调用if(pNode) {pNode->Release();pNode=NULL;}
**3）使用MFC提供的Com智能指针解决上述问题**
注意：可通过查看源码，看到#import生成的智能指针的原型是_com_ptr_t。
3.1
IXMLDOMDocumentPtr docPtr = NULL;
docPtr.CreateInstance(...)
……
这下不会有问题了，因为docPtr在析构时会有正确的释放处理。
3.2
IXMLDOMNodePtr nodePtr = NULL;
if(FAILED(pDoc->selectSingleNode(_bstr_t("Workbook"), &nodePtr)) || nodePtr==NULL)
throw(_T("selectSingleNode failed!"));
if(FAILED(pDoc->selectSingleNode(_bstr_t("Workbook"), &nodePtr)) || nodePtr==NULL)
throw(_T("selectSingleNode failed!"));
不会出错了，因为_com_ptr_t重载了&操作符，在取指针时，有如下操作：
Interface** operator&() throw()
{
　_Release();
　m_pInterface = NULL;
　return &m_pInterface;
}
3.3
nodePtr = nodePrt2 ，也不会有问题：
仔细查看源码，在=操作符中会调用Attach，而Attach的做法是，会先调用_Release();
3.4
再看看值传递，拷贝构造函数如下
template<> _com_ptr_t(const _com_ptr_t& cp) throw()
: m_pInterface(cp.m_pInterface)
{ 
　_AddRef(); 
}
嗯，也不会有问题。
3.5
最后，我们也总结一下使用COM智能指针时的注意事项：
不要在Com智能指针的生命期如果在::CoUninitailize之后，那请在调用::CoUninitailize之前，强制调用MyComPtr = NULL;达到强制释放的目的，否则会出错。
不要混用智能指针和普通Dispatch指针，不要调用MyComPtr->Release()，这违背智能指针的原意，会在析构时报错。
**4）使用ATL提供智能指针：CComPtr或是CComQIPtr**
如果不使用MFC框架，要自已包装IDispatch，生成智能指针，还可以使用ATL提供的智能指针。查看源码，并参照《深入解析ATL》一书，发现实现与_com_ptr_t大同小异，效果一致。
**六、C++智能指针的标准实现1）标准C++的智能指针**
说到智能指针，我们一定要看看标准C++提供的auto_ptr。而auto_ptr的使用是有很多限制的，我们一条一条来细数：
1.1 auto_ptr要求一个对象只能有一个拥有者，严禁一物二主。
比如以下用法是错误的。
classA *pA = new classA;
auto_ptr<classA> ptr1(pA);
auto_ptr<classA> ptr2(pA);
1.2 auto_ptr是不能以传值方式进行传递的。
因为所有权的转移，会导致传入的智能指针失去对指针的所有权。如果要传递，可以采用引用方式，利用const引用方式还可以避免程序内其它方式的所有权的转移。就其所有权转移的做法：可以查看auto_ptr的拷贝构造和=操作符的源码，此处略。
1.3 其它注意事项：
a、不支持数组。
b、注意其Release语意，它没有引用计数，与com提供的智能指针不同。Release是指释放出指针，即交出指针的所有权。
c、auto_ptr在拷贝构造和=操作符时的特珠含义，决定它不能做为STL标准容器的成员。
好了，看了上面的注意事项，特别是第三条，基本上可以得出结论：在实际应用场合，auto_ptr基本没什么应用价值的。
**2）如何得到支持容器的智能指针**
我们利用auto_ptr的原型，制作一个引用计数的智能指针，则时让它支持STL容器的标准。实现代码很简单，参照了《C++标准程序库》中的代码，关键代码如下：
**[cpp]**[view plain](http://blog.csdn.net/sunboy_2050/article/details/6198096#)[copy](http://blog.csdn.net/sunboy_2050/article/details/6198096#)[print](http://blog.csdn.net/sunboy_2050/article/details/6198096#)[?](http://blog.csdn.net/sunboy_2050/article/details/6198096#)
- template<class T>  
- class CountedPtr {  
- private:  
- 　T *ptr = NULL;  
- long *counter = NULL;  
- 
- public:  
- explicit CountedPtr(T *p = NULL) : ptr(p),count(newlong(1){}     // 构造
- 　　~CountedPtr() {Release();}　　                                      // 析构
- 
- 　　CountedPtr(cont CountedPtr<T> &p) : ptr(p.ptr),count(p.count) {++*counter;} 　　// 拷贝构造
- 
- // = 操作符
- 　　CountedPtr<T>& operator= (const CountedPtr<T>& p) {  
- if(this!=&p) {  
- 　　　 Release();   
- 　　　　ptr = p.ptr;  
- 　　　　counter = p.counter;  
-         ++*counter;  
- 　　　}  
- return *this;  
- 　　}  
- 
- // 其它略 
- // ....
- 
- private:  
- void Release() {  
- if(--*counter == 0) {  
- delete counter;  
- delete ptr;  
-         counter = NULL;  
-         ptr = NULL;  
- 　　　}   
- 　　}  
- }  
好了，这样，当复制智能指针时，原指针与新指针副本都是有效的，这样就可以应用于容器了。
现在，通过CountedPtr包装的C++对象，是不是和Java的对象类似了呢，呵呵。只要再加上一些必要的操作符，它就可以作为容器中的共享资源来使用了。
**参考推荐：**
[智能指针](http://baike.baidu.com/view/1391603.htm)（百度百科）
[Smart pointer](http://en.wikipedia.org/wiki/Smart_pointer)（wiki）
[C++中的智能指针](http://www.cppblog.com/martin/archive/2009/03/03/martin_yahoo.html)
[C++ 智能指针详解](http://blog.csdn.net/xt_xiaotian/article/details/5714477)
[Visual C++及C++中的智能指针应用分析](http://www.bccn.net/Article/kfyy/vc/jszl/200608/4310_2.html)
[android sp wp](http://hi.baidu.com/sunboy_2050/blog/item/76843e288c6266245243c155.html?timeStamp=1336265256563)（推荐）
[Android 智能指针原理](http://blog.csdn.net/sunboy_2050/article/details/7538546)
