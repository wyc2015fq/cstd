# C++ 异常处理 catch(...) - L_Andy的专栏 - CSDN博客

2013年07月25日 17:24:01[卡哥](https://me.csdn.net/L_Andy)阅读数：1247
个人分类：[C/C++编程](https://blog.csdn.net/L_Andy/article/category/1925955)



                
如果要想使一个catch block能抓获多种数据类型的异常对象的话，怎么办?C++标准中定义了一种特殊的catch用法，那就是” catch(…)”。

**　感性认识**

　　1、catch(…)到底是一个什么样的东东，先来个感性认识吧!看例子先：

　int main() 

　　{ 

　　try 

　　{ 

　　cout << "在 try block 中, 准备抛出一个异常." << endl; 

　　//这里抛出一个异常(其中异常对象的数据类型是int，值为1) 

　　throw 1; 

　　} 

　　//catch( int& value ) 

　　//注意这里catch语句 

　　catch( …) 

　　{ 

　　cout << "在 catch(…) block 中, 抛出的int类型的异常对象被处理" << endl; 

　　} 

　　}
　　2、哈哈!int类型的异常被catch(…)抓获了，再来另一个例子：

　int main() 

　　{ 

　　try 

　　{ 

　　cout << "在 try block 中, 准备抛出一个异常." << endl; 

　　//这里抛出一个异常(其中异常对象的数据类型是double，值为0.5) 

　　throw 0.5; 

　　} 

　　//catch( double& value ) 

　　//注意这里catch语句 

　　catch( …) 

　　{ 

　　cout << "在 catch(…) block 中, double类型的异常对象也被处理" << endl; 

　　} 

　　}
　　3、同样，double类型的异常对象也被catch(…)块抓获了。是的，catch(..)能匹配成功所有的数据类型的异常对象，包括C++语言提 供所有的原生数据类型的异常对象，如int、double，还有char*、int*这样的指针类型，另外还有数组类型的异常对象。同时也包括所有自定义 的抽象数据类型。例程如下：

　int main() 

　　{ 

　　try 

　　{ 

　　cout << "在 try block 中, 准备抛出一个异常." << endl; 

　　//这里抛出一个异常(其中异常对象的数据类型是char*) 

　　char* p=0; 

　　throw p; 

　　} 

　　//catch( char* value ) 

　　//注意这里catch语句 

　　catch( …) 

　　{ 

　　cout << "在 catch(…) block 中, char*类型的异常对象也被处理" << endl; 

　　} 

　　} 

　　int main() 

　　{ 

　　try 

　　{ 

　　cout << "在 try block 中, 准备抛出一个异常." << endl; 

　　//这里抛出一个异常(其中异常对象的数据类型是int[]) 

　　int a[4]; 

　　throw a; 

　　} 

　　//catch( int value[] ) 

　　//注意这里catch语句 

　　catch( …) 

　　{ 

　　cout << "在 catch(…) block 中, int[]类型的异常对象也被处理" << endl; 

　　} 

　　}
　　4、对于抽象数据类型的异常对象。catch(…)同样有效，例程如下：

　class MyException 

　　{ 

　　public: 

　　protected: 

　　int code; 

　　}; 

　　int main() 

　　{ 

　　try 

　　{ 

　　cout << "在 try block 中, 准备抛出一个异常." << endl; 

　　//这里抛出一个异常(其中异常对象的数据类型是MyException) 

　　throw MyException(); 

　　} 

　　//catch(MyException& value ) 

　　//注意这里catch语句 

　　catch( …) 

　　{ 

　　cout << "在catch(…) block中, MyException类型的异常对象被处理" << endl; 

　　} 

　　}
**　对catch(…)有点迷糊?**

　　1、究竟对catch(…)有什么迷糊呢?还是看例子先吧!

　void main() 

　　{ 

　　int* p = 0; 

　　try 

　　{ 

　　// 注意：下面这条语句虽然不是throw语句，但它在执行时会导致系统 

　　// 出现一个存储保护错误的异常(access violation exception) 

　　*p = 13; // causes an access violation exception; 

　　} 

　　catch(...) 

　　{ 

　　//catch(…)能抓获住上面的access violation exception异常吗? 

　　cout << "在catch(…) block中" << endl; 

　　} 

　　}
　　请问上面的程序运行时会出现什么结果吗?catch(…)能抓获住系统中出现的access violation exception异常吗?朋友们!和我们的主人公阿愚一样，自己动手去测试一把!

　　结果又如何呢?实际上它有两种不同的运行结果，在window2000系统下用VC来测试运行这个小程序时，发现程序能输出"在catch(…) block中"的语句在屏幕上，也即catch(…) 能成功抓获住系统中出现的access violation exception异常，很厉害吧!但如果这个同样的程序在linux下用gcc编译后运行时，程序将会出现崩溃，并在屏幕上输出”segment fault”的错误信息。

　　主人公阿愚有点急了，也开始有点迷糊了，为什么?为什么?为什么同样一个程序在两种不同的系统上有不同的表现呢?其原因就是：对于这种由于硬件或操作 系统出现的系统异常(例如说被零除、内存存储控制异常、页错误等等)时，window2000系统有一个叫做结构化异常处理(Structured Exception Handling，SEH)的机制，这个东东太厉害了，它能和VC中的C++异常处理模型很好的结合上(实际上VC实现的C++异常处理模型很大程度上建 立在SEH机制之上的，或者说它是SEH的扩展，后面文章中会详细阐述并分析这个久富盛名的SEH，看看catch(…)是如何神奇接管住这种系统异常出
 现后的程序控制流的，不过这都是后话)。而在linux系统下，系统异常是由信号处理编程方法来控制的(信号处理编程，signal processing progamming。在介绍unix和linux下如何编程的书籍中，都会有对信号处理编程详细的介绍，当然执着的主人公阿愚肯定对它也不会放过，会深 入到unix沿袭下来的信号处理编程内部的实现机制，并尝试完善改进它，使它也能够较好地和C++异常处理模型结合上)。

　　那么C++标准中对于这种同一个程序有不同的运行结果有何解释呢?这里需要注意的是，window2000系统下catch(…)能捕获住系统异常， 这完全是它自己的扩展。在C++标准中并没有要求到这一点，它只规定catch(…)必须能捕获程序中所有通过throw语句抛出的异常。因此上面的这个 程序在linux系统下的运行结果也完全是符合C++标准的。虽然大家也必须承认window2000系统下对C++异常处理模型的这种扩展确实是一个很 不错的完善，极大得提高了程序的安全性。

**　　为什么要用catch(…)这个东东?**

　　程序员朋友们也许会说，这还有问吗?这篇文章的一开始不就讲到了吗?catch(…)能够捕获多种数据类型的异常对象，所以它提供给程序员一种对异常 对象更好的控制手段，使开发的软件系统有很好的可靠性。因此一个比较有经验的程序员通常会这样组织编写它的代码模块，如下：


　void Func() 

　　{ 

　　try 

　　{ 

　　// 这里的程序代码完成真正复杂的计算工作，这些代码在执行过程中 

　　// 有可能抛出DataType1、DataType2和DataType3类型的异常对象。 

　　} 

　　catch(DataType1& d1) 

　　{ 

　　} 

　　catch(DataType2& d2) 

　　{ 

　　} 

　　catch(DataType3& d3) 

　　{ 

　　} 

　　// 注意上面try block中可能抛出的DataType1、DataType2和DataType3三 

　　// 种类型的异常对象在前面都已经有对应的catch block来处理。但为什么 

　　// 还要在最后再定义一个catch(…) block呢?这就是为了有更好的安全性和 

　　// 可靠性，避免上面的try block抛出了其它未考虑到的异常对象时导致的程 

　　// 序出现意外崩溃的严重后果，而且这在用VC开发的系统上更特别有效，因 

　　// 为catch(…)能捕获系统出现的异常，而系统异常往往令程序员头痛了，现 

　　// 在系统一般都比较复杂，而且由很多人共同开发，一不小心就会导致一个 

　　// 指针变量指向了其它非法区域，结果意外灾难不幸发生了。catch(…)为这种 

　　// 潜在的隐患提供了一种有效的补救措施。 

　　catch(…) 

　　{ 

　　} 

　　}
　　还有，特别是VC程序员为了使开发的系统有更好的可靠性，往往在应用程序的入口函数中(如MFC框架的开发环境下 CXXXApp::InitInstance())和工作线程的入口函数中加上一个顶层的trycatch块，并且使用catch(…)来捕获一切所有的 异常，如下：

　BOOL CXXXApp::InitInstance() 

　　{ 

　　if (!AfxSocketInit()) 

　　{ 

　　AfxMessageBox(IDP_SOCKETS_INIT_FAILED); 

　　return FALSE; 

　　} 

　　AfxEnableControlContainer(); 

　　// Standard initialization 

　　// If you are not using these features and wish to reduce the size 

　　// of your final executable, you should remove from the following 

　　// the specific initialization routines you do not need. 

　　#ifdef _AFXDLL 

　　Enable3dControls(); // Call this when using MFC in a shared DLL 

　　#else 

　　Enable3dControlsStatic(); // Call this when linking to MFC statically 

　　#endif 

　　// 注意这里有一个顶层的trycatch块，并且使用catch(…)来捕获一切所有的异常 

　　try 

　　{ 

　　CXXXDlg dlg; 

　　m_pMainWnd = &dlg; 

　　int nResponse = dlg.DoModal(); 

　　if (nResponse == IDOK) 

　　{ 

　　// TODO: Place code here to handle when the dialog is 

　　// dismissed with OK 

　　} 

　　else if (nResponse == IDCANCEL) 

　　{ 

　　// TODO: Place code here to handle when the dialog is 

　　// dismissed with Cancel 

　　} 

　　} 

　　catch(…) 

　　{ 

　　// dump出系统的一些重要信息，并通知管理员查找出现意外异常的原因。 

　　// 同时想办法恢复系统，例如说重新启动应用程序等 

　　} 

　　// Since the dialog has been closed, return FALSE so that we exit the 

　　// application, rather than start the application's message pump. 

　　return FALSE; 

　　}
　　通过上面的例程和分析可以得出，由于catch(…)能够捕获所有数据类型的异常对象，所以在恰当的地方使用catch(…)确实可以使软件系统有着更 好的可靠性。这确实是大家使用catch(…)这个东东最好的理由。但不要误会的是，在C++异常处理模型中，不只有catch(…)方法能够捕获几乎所 有类型的异常对象.

