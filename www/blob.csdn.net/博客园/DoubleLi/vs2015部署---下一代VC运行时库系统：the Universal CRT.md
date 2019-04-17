# vs2015部署---下一代VC运行时库系统：the Universal CRT - DoubleLi - 博客园









### 前言

其实the Universal CRT（通用C运行时库）已经不能算是“下一代”，因为它已经在前两年伴随着Visual Studio 2015和Windows10发布。但是由于之前使用VS2015开发的应用还较少，关注的人也少，相关的文章更少。所以笔者想干脆用一篇文章，深入浅出的介绍一下Universal CRT。

### 什么是CRT？

CRT，也就是“C Runtime”，中文译为：C运行时。我们一直称为C Runtime Library（C运行时库）。它主要负责实现C程序在运行时的各种抽象功能实现。不同的C编译器会有不同的具体实现程序，比如Windows下常见的微软的VC编译器的msvcrt.dll；GCC/Glibc的crt0.o、crti.o等。具体到二进制表现形式，可以是动态库，也可以是静态库。

CRT一般会至少负责以下功能：
- 实现C库函数。比如：printf、fopen等
- 初始化程序启动所需要的一些功能
- 异常处理
- 多线程处理

CRT也可以看成是一个操作系统抽象层。大家都知道，每一种操作系统都有自己的API或者是系统调用。像C语言之所以能够跨平台（代码级），就是因为我只需要用C库函数写程序，CRT会转化为相应平台的具体实现的API来处理。

![](http://www.qingpingshan.com/uploads/allimg/170302/1016015063-0.png)

下面我们要着重讨论的是Windows下的开发工具Visual C++配套的CRT，以下将统称为“VC运行时库”。

### VC运行时库的历史

![](http://www.qingpingshan.com/uploads/allimg/170302/1016015947-1.png)

（注：上表中的划代是作者自己的划代方式）

#### 第一代

我记得我最早学C语言还是上大二（2002年）的时候，那时可是VC的黄金年代。额~~应该说是钻石年代。大家都在学习VC++6.0，一个堪称经典的版本。VC++6.0是为了配合Windows98的应用软件开发而发布的，发布的年代也差不多就是98年前后，所以内部有些文件又称之为VC98。

到这个时候，VC运行时库文件只有一个叫msvcrt.dll的，一般在c:\windows\system32下，大家发行软件产品的时候，基本上不需要考虑是否需要带个什么库，因为默认操作系统都会自带的。

逐渐的，大家发现一个非常棘手的问题，那就是随着功能的增多，不同版本的msvcrt.dll支持的功能和函数不同，版本的识别和兼容成了难题。当时很多安装包为了自己的需要，会不停的覆盖系统的msvcrt.dll，导致运行不稳定。如果把WinXP系统的msvcrt.dll覆盖了Win98的，那会导致WinXP不稳定。所以后来就发展除了第二代的CRT。

#### 第二代

自从微软发布Visual Studio.Net 2002/2003开始，为了解决日益增长的功能和DLL版本兼容性问题，微软给现有的运行时库加上了版本号，并且把运行时库拆分为多个文件，比较常见的是msvcrXXX.dll和msvcpXXX.dll。其中XXX是版本号。


版本号的定义和文件命名如下表所示：
![](http://www.qingpingshan.com/uploads/allimg/170302/1016012925-2.png)

其中msvcrXXX.dll（注意不是msvcrtXXX.dll）一般负责实现C语言的基础特性，如程序启动、异常处理、库函数等功能。msvcpXXX.dll负责标准C++库的相关实现。大家可以看看其中的导出函数就能明白。

在这一阶段中，开发者想要发布软件产品，就必须确保目标计算机上必须已经安装了相应版本的运行时库。一般情况下，微软推荐大家使用运行时库安装包来进行安装。对于大型软件为了预防万一，都会附带vcredist.exe安装包。但是这种方法很不受待见。因为现在大多数软件都是通过互联网进行分发，对安装包大小很敏感。每次打包附带一个运行时库包，既增加了用户的下载时间，又增加了推广成本。

现在大部分软件的做法是将这两个dll放在软件目录下发行。但是问题又来了，光是VC2008的msvcr90.dll的就有N个版本，不同语言有不同版本，随着update增加的修正版本也很多。大部分情况应该不会有问题，但笔者就曾经遇到过加载错误版本的运行时库而程序崩溃又查不出问题的经历。这个时候微软要求必须使用manifest文件来指定加载的VC运行时库版本。直到VC2010，由于编译器内置了manifest，所以就不需要额外提供。

#### 第三代

自Visual Studio 2010开始，微软大力改进了很多C++特性，陆续在2012、2013、2015版本中增加了对C99、C++11、C++14、C++17等标准的支持，使得C++库的功能成倍增加。这种小步快跑的更新模式，使得如何有效的让VC运行时库向前和向后兼容而不破坏现有的软件组件的问题变得异常突出。再加上让VC运行时库能够更好的支持Win8/10提倡的PC和移动设备并举的理念，微软团队决定在Visual Studio 2015对VC运行时库进行重构。然后“the Universal CRT”就应运而生了。

### the Universal CRT

the Universal CRT（以下简称UCRT），顾名思义，意思为“通用C运行时库”。关键就在“通用”这两个字上。早期的设计理念就是要把相对通用的功能独立出来。这个概念最早在Visual Studio 14 （即vs2015）的CTP1 [1] 发布的时候提出来 [2] 。VS很神奇的跳过了13.0这个版本，直接从12.0（vs2013）跳到了14.0（vs2015），估计是因为欧美人把13这个数字认为是不吉利的有关。尽管UCRT的版本号称是1.0，但真实的VCRuntime还是14.0。

#### 早期的设计

当vs2015还在CTP阶段时，微软的设想是将VC运行时库拆分成三部分。

vcruntime140.dll 包含运行期需要处理的功能，如：进程启动、异常处理、以及耦合到相关编译器的功能。

appcrt140.dll包含所有平台上都可用的所有功能，且以后保持这部分CRT的向后兼容性。包括：堆、数学库、stdio库、locale库、大多数字符串操作函数、时间库和一些其他功能等。

desktopcrt140.dll包含所有只能由桌面应用程序使用的功能，且以后保持这部分CRT的向后兼容性。包括：处理多字节字符串、exec和spawn进程管理函数、direct-to-console I/O函数的功能等等。

![](http://www.qingpingshan.com/uploads/allimg/170302/101601O08-3.png)

#### 正式版本

在最终发布正式版的时候，微软将appcrt140.dll和desktopcrt140.dll合并为一个不带版本号的程序库：ucrtbase.dll。它对应的Debug版本的命名是ucrtbased.dll。这个后来被正式命名为“the Universal CRT”。

![](http://www.qingpingshan.com/uploads/allimg/170302/10160154Q-4.png)

#### API Sets for Universal Windows Platform (UWP) apps

令很多人吐槽的是，UCRT并不只是一个DLL，它还附带了一堆以“api-ms-”开头的DLL程序文件，且有40个之多！可以看到，这些DLL导出了几乎所有的win32api。这其实是微软在Windows10中大力推动的“Universal Windows Platform (UWP) apps”即“通用Windows平台应用”的api接口 [3] 。这些dll有些默认为“delay load”，也就会是被延迟加载。一般基于UCRT编译的程序，不是直接调用ucrtbase.dll，而是调用VCRuntime140.dll和UWP apis来间接调用。

![](http://www.qingpingshan.com/uploads/allimg/170302/1016012253-5.png)

### 基于UCRT程序的部署方法

#### 什么程序是基于UCRT的？

如果你是用Visual Studio 2015和2017来编写C或C++程序，那么就已经是基于UCRT的。

#### 需要关注的程序模块

VCRuntime140.dll 这是VC运行时库和编译器相关的必备模块，必须存在。

msvcp140.dll 如果你写的程序含有C++标准库的代码，那就必须存在。

ucrtbase.dll和api-ms-**.dll 必须存在。

#### 部署方法

第一种

微软强烈推荐使用vcredist.exe来给目标机安装相应的文件。它会安装所有对的UCRT文件和必备组件。这是最省事儿便捷的方法。

但是vcredist_x86.exe和vcredist_x64.exe就各有近14MB的体积！大型程序发布的时候可能无所谓，而很多很多基于互联网发布的程序，却不可能这么干。互联网程序对安装包的大小很敏感，这直接影响最终用户终端的到达率和推广成本。

可参考PHP7.1的Windows版的下载页面和安装包。

第二种

程序自带VCRuntime140.dll和msvcp140.dll,再给系统打基于msu的KB2999226补丁。KB2999226补丁会给系统安装UCRT。一般情况下，Windows10已经自带了UCRT，不需要额外打补丁。

这种方法不适用于WinXP系统。而在中国WinXP系统还有很大保有量，大家都不会轻易放弃这个庞大的用户群的。

可参考Python 3.5的Windows安装包。

第三种

现在互联网程序大多使用的是app-local的部署模式，意思就是把依赖库放在自己程序目录下，既不会跟别的应用软件冲突，又方便了软件分发。

起初，微软并没有打算针对UCRT程序继续这样的部署模式。但是后来大家反响比较强烈，所以在Windows 10 SDK发布的时候，把UCRT和UMP的相关dlls都一起发布了。这个目录一般是“C:\Program Files (x86)\Windows Kits\10\Redist\ucrt”。

你也可以在Visual Studio 2015的安装目录下找到VCRuntime140.dll和msvcp140.dll。这个目录一般是“C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\redist\x86\Microsoft.VC140.CRT”。

有些开发者一开始可能会被ucrt目录下的四十几个文件吓到，不过还好都不大，打包压缩以后都很小。

可参考Visual Studio 2015配套的Remote Debugger 调试工具。这个工具由于要求是“standalone”的，所以就是用此方法部署的。可以在这里找到：“C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\IDE\Remote Debugger\x86”。

### UCRT展望

UCRT的确给我们带来了很多全新的概念，也给我们带来了少许的不适应。但这毕竟是未来的发展方向。基于UCRT的Visual Studio 2015给我们带来的众多新的C、C++语言标准的支持，我相信以后C++程序会变得更加强大。按照网上的一些说法，未来不排除会对VCRuntime模块再次优化重构的可能性，这个我们只能拭目以待了。Visual Studio 2017即将发布，我们也将继续跟进UCRT的发展方向。

[1] CTP 即Community Technology Preview，译为社区技术预览版，一般是微软开发软件的早期对内测试版

[2] 参考 [https://blogs.msdn.microsoft.com/vcblog/2015/03/03/introducing-the-universal-crt/](https://blogs.msdn.microsoft.com/vcblog/2015/03/03/introducing-the-universal-crt/)

[3] 参考： [https://msdn.microsoft.com/zh-cn/library/mt186421.aspx](https://msdn.microsoft.com/zh-cn/library/mt186421.aspx)

from：http://www.qingpingshan.com/m/view.php?aid=223329










