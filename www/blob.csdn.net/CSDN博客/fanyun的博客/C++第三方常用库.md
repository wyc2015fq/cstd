# C++第三方常用库 - fanyun的博客 - CSDN博客
2016年09月30日 09:27:45[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：6103标签：[C++](https://so.csdn.net/so/search/s.do?q=C++&t=blog)
个人分类：[库](https://blog.csdn.net/fanyun_01/article/category/6838780)
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
在C++中，库的地位是非常高的。C++之父
   Bjarne  Stroustrup先生多次表示了设计库来扩充功能要好过设计更多的语法的言论。现实中，C++的库门类繁多，解决的问题也是极其广泛，库从轻量级到重量级的都有。不少都是让人眼界大开，亦或是望而生叹的思维杰作。
在程序开发过程中，经常会引用第三方已经开发好的库：
(一)  **尽量使用开源的第三方库**
这样就可以拿到第三方库的源码，然后自己编译，根据自己的需求。
msdn 上有这样一段话： 
警告 不要混合使用运行时库的静态版本和动态版本。在一个进程中有多个运行时库副本会导致问题，因为副本中的静态数据不与其他副本共享。链接器禁止在 .exe 文件内部既使用静态版本又使用动态版本链接，但您仍可以使用运行时库的两个（或更多）副本。例如，当与用动态 (DLL) 版本的运行时库链接的 .exe 文件一起使用时，用静态（非DLL）版本的运行时库链接的动态链接库可能导致问题。（还应该避免在一个进程中混合使用这些库的调试版本和非调试版本）。
(二)  **直接使用第三库，需要弄清依赖关系**
如果直接使用一些比较成熟库，我建议使用一些工具，比如说使用depends查看一下这个第三方库还依赖于哪些库。 
比如，有的库还是依赖于msvcp等几个dll，这样对于没有安装过VS等编译器的计算机就无法运行您的程序。
另外，附上一些C++开源库： 
1. Boost  C++标准委员会库工作组成员发起，在C++社区中影响甚大，其成员已近2000人。
   Boost库为我们带来了最新、最酷、最实用的技术，是不折不扣的“准”标准库。Boost中比较有名气的有这么几个库：
  Regex   
  正则表达式库   
  Spirit   
  LL   parser   framework，用C++代码直接表达EBNF   
  Graph   
  图组件和算法   
  Lambda   
  在调用的地方定义短小匿名的函数对象，很实用的functional功能   
  concept   check   
  检查泛型编程中的concept   
  Mpl   
  用模板实现的元编程框架   
  Thread   
  可移植的C++多线程库   
  Python   
  把C++类和函数映射到Python之中   
  Pool   
  内存池管理   
  smart_ptr   
2.pthread windows下的posix线程实现 
3.libcurl 一个有名的开源网络爬虫库 阿里旺旺中使用到了 
4.libeay32 OpenSSL Library 
5.libtidy 一个专门解析htm的库 
6.zlib 这个鬼都知道 
7.freetype c接口的type2字体处理库 
8.libmad 一个编解码mp3的库 
9.libogg,等 一个编解码ogg音频格式的库 
10.libsnd 一个开源的编解码十多种音频格式的库 
11.ffmpeg 一个关于音频视频处理的库 
12.Freeimage,Cximage,Devil 这3个都是用来处理图形的库 
13.libpng,libjpeg,….基本同上 
14.angelscript 一个类似lua的脚本引擎 其脚本风格类似于标准c语言 
15.flac/flac++一个编解码flac音频格式的库 
16.tinyxml,rapidxml,libxml 都是关于xml解析方面的 
17.luaplus,luabind都是涉及绑定lua和c++的库 
18.ode,bullet 开源的物理引擎库 
19.timidity一个可以把mid音频格式转化为wav格式的库 
20.vlc一个类似ffmeg的库 
21.zthread一个类型boost-thread,pthread的c++风格的多线程库 
22.sigc++,sigslot信号插槽库 类型的有boost中的signal 
23.SDL 简单的音频视频库 
24.hge一个简单的使用ddraw的2维游戏小引擎 
25.opencv一个开源的处理图形的库 
26.mygui,cegui 都是游戏上使用的GUI系统 
27.鬼火游戏引擎,Orge,都是开源的游戏中间件 
28.Wxwidget一个开源的跨平台,类似MFC 
29.QT  Qt是Trolltech公司的一个多平台的C++图形用户界面应用程序框架。它提供给应用程序开发者建立艺术级的图形用户界面所需的所用功能。Qt是完全面向对象的很容易扩展，并且允许真正地组件编程。自从1996年早些时候，Qt进入商业领域，它已经成为全世界范围内数千种成功的应用程序的基础。Qt也是流行的Linux桌面环境KDE
   的基础，同时它还支持Windows、Macintosh、Unix/X11等多种平台。
30.loki一个实验性质的c++库 
31.ace一个网络通信库 
32.fmod一个有点名气的游戏音效引擎 
33.sqlite 一个开源的桌面数据库
34.大名鼎鼎的微软基础类库（Microsoft   Foundation   Class）。大凡学过VC++的人都应该知道这个库。虽然从技术角度讲，MFC是不大漂亮的，但是它构建于Windows
   API   之上，能够使程序员的工作更容易,编程效率高，减少了大量在建立   Windows 程序时必须编写的代码，同时它还提供了所有一般
   C++  编程的优点，例如继承和封装。
35.WTL  基于ATL的一个库。因为使用了大量ATL的轻量级手法，模板等技术，在代码尺寸，以及速度优化方面做得非常到位。主要面向的使用群体是开发COM轻量级供网络下载的可视化控件的开发者。
36.GTK GTK是一个大名鼎鼎的C的开源GUI库。在Linux世界中有Gnome这样的杀手应用。而GTK就是这个库的C++封装版本。
37.网络通信ACE库 C++库的代表，超重量级的网络通信开发框架。ACE自适配通信环境（Adaptive
  Communication   Environment）是可以自由使用、开放源代码的面向对象框架，在其中实现了许多用于并发通信软件的核心模式。ACE提供了一组丰富的可复用C++包装外观（Wrapper
   Facade）和框架组件，可跨越多种平台完成通用的通信软件任务，其中包括：事件多路分离和事件处理器分派、信号处理、服务初始化、进程间通信、共享内存管理、消息路由、分布式服务动态（重）配置、并发执行和同步，等等。
38. StreamModule  设计用于简化编写分布式程序的库。尝试着使得编写处理异步行为的程序更容易，而不是用同步的外壳包起异步的本质。
39. SimpleSocket 这个类库让编写基于socket的客户/服务器程序更加容易。
40. Blitz++是一个高效率的数值计算函数库，它的设计目的是希望建立一套既具像C++  一样方便，同时又比Fortran速度更快的数值计算环境。通常，用C++所写出的数值程序，比
   Fortran慢20%左右，因此Blitz++正是要改掉这个缺点。方法是利用C++的template技术，程序执行甚至可以比Fortran更快。Blitz++目前仍在发展中，对于常见的SVD，FFTs，QMRES等常见的线性代数方法并不提供，不过使用者可以很容易地利用Blitz++所提供的函数来构建。
