# 如何诊断Windows CE的应用程序崩溃(转载) - xqhrs232的专栏 - CSDN博客
2009年11月12日 17:57:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：858
 无论你是一个单纯的电脑用户还是一名高级软件工程师，都一定对程序崩溃不陌生。做为一名Windows CE应用程序开发者，你也一定遇到过下图这种场景：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/singlerace/EntryImages/20080715/FAE.JPG)
这个对话框告诉你，有一个叫installer.exe的程序在地址00019320处崩溃了。如果这个程序归你负责，那么你的问题就来了：怎么找出这个BUG？这篇文章我想谈谈我在这方面的一些经验。
Windows CE的崩溃界面给出的信息十分的少，其中最有用的无疑是崩溃地址，如果你能从崩溃地址定位到源代码去，这个问题可以说就解决了一半。
从地址定位到源代码的方法有几种。一种是利用MAP文件：你可以在BUILD程序时生成MAP文件。MAP文件是一个文本文件，其中主要记录了各个函数入口对应的地址信息。比如这个例子中，崩溃地址对应的入口是：
0001:000082f4 [??1?$CComPtr@UIImage@@@ATL@@QAA@XZ](mailto:??1?$CComPtr@UIImage@@@ATL@@QAA@XZ) 000192f4 f i ImageViewer.objMAP文件的好处是它是文本文件，可以人工阅读，缺点是信息不够多，只能定位到函数级别，而且要看懂MAP文件你需要有足够的经验，比如其中那串长长的貌似乱码的字符串是C++函数经过name mangling处理后的结果，没有一定的经验你根本没法还原出实际的函数。
另一种方法是利用PDB文件，PDB文件收集了应用程序的调试符号。PDB文件提供的信息很全，不过你得需要一定的工具才能解读它。如果你是一个经验丰富的Windows桌面平台的应用程序开发者，你可能听说过MSJ（Microsoft System Journal）这本杂志。如果你曾经看过这本杂志，应该对Bugslayer不陌生。在这篇文章中，Bugslayer介绍了他做的一个工具：CrashFinder。CrashFinder可以从崩溃地址通过查询相应的PDB文件直接定位到导致崩溃的源代码行。幸运的是，由于Windows
 CE可执行程序及其PDB文件的格式和Windows桌面系统上的是一样的，因此CrashFinder也可以用来定位Windows CE程序的崩溃地址。下面是CrashFinder显示的结果：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/singlerace/EntryImages/20080715/crashfinder.JPG)
CrashFinder提供的信息十分有用，但是不够直观。为此我在Remote Process Explorer提供了一个更方便的界面，它可以直接显示源代码，并把导致崩溃的那一行highlight出来：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/singlerace/EntryImages/20080715/crackaddress.JPG)
PDB文件里包含着大量的调试信息可以帮助你诊断应用程序错误，因此一般来说即使是正式release的版本，你也应该生成并维护好这些PDB文件。使用PDB文件的关键是崩溃的应用程序和PDB文件一定要匹配，否则它不但不能帮到你，反而会误导你。下图是VS2005中设置生成PDB文件和MAP文件的地方：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/singlerace/EntryImages/20080715/dbginfo.JPG)
前面说过，Windows CE的崩溃界面给出的信息十分少，很多时候我们还需要更多的信息才能定位问题，另外有些Windows CE设备可能根本没有显示器。为解决这一问题，Windows CE在应用程序崩溃时还同时往外（一般是串口）输出相关的崩溃信息。比如这个例子中，如果你正好接着调试串口，开着HyperTerminal，那么在程序崩溃的时候你会看到这样的信息：
Data Abort: Thread=8d661000 Proc=81a477c0 'installer.exe'
AKY=00000401 PC=00019320(installer.exe+0x00009320) RA=00019094(installer.exe+0x0
0009094) BVA=16080100 FSR=00000007我相信Windows CE开发者一定也对这几行信息很熟悉。怎么利用这些信息诊断程序的问题？这里面最关键的信息是PC和RA给出的地址信息。PC就是上面提到的崩溃地址，根据这个地址用CrashFinder或者我的Remote Process Explorer里的Crack Address界面可以定位到导致崩溃的源代码行；RA是PC的返回地址(Return Address），根据这个地址可以找到导致崩溃的上一级函数，这个信息也很重要，因为很多时候崩溃的原因往往是上层函数往底层函数传递了非法参数导致的，比如你的应用程序用一个非法的窗口类调用MFC函数，崩溃地址在MFC函数里面，但是出问题的地方在你的调用代码里。下图是RA地址对应的源代码：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/singlerace/EntryImages/20080715/crackaddress2.JPG)
除了PC和RA，其他信息也可以提供一些参考作用：BVA是ARM中的Fault Address Register（FAR），是引起Data Abort的虚拟地址，比如说你的程序试图访问一个非法地址里的内容，那么Data Abort时BVA就是这个非法地址；FSR是Fault Status Register，指明导致异常的原因，FSR的解释可以看这里。要注意的是Thread和Proc给出的不是Thread Id/Thread Handle或者Process Id/Process Handle，它们给出的是该Thread或Proc对应的内核对象的指针，类似于Window
 NT平台的TEB和PEB的概念。由于你看到崩溃信息时线程已经退出了，因此根据这个信息在事后你无法知道是哪个线程出的错。以后我将介绍一种系统级的logging机制，这种机制可以把每条log的Thread Id、TEB等信息同时记录下来，这样在崩溃时就可以根据Data Abort的TEB信息和先前log中出现的TEB，找到出错的线程。这样，你不但可以定位到错误的源代码，还能找到运行错误代码的线程，将大大提高解决问题的效率。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/singlerace/archive/2008/07/15/2655154.aspx](http://blog.csdn.net/singlerace/archive/2008/07/15/2655154.aspx)
