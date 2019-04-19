# 引向 Windows 8 的道路 - 文章 - 伯乐在线
来源：[忙碌命](http://blog.csdn.net/laiboy/article/details/7096017)
**作者前言：**我本来计划在2012年1月MSDN杂志发表我的第一篇关于 Windows8 文章。然而，微软已经禁止在 MSDN 杂志上发表任何关于 Windows 8 的文章。我不允许在我的 MSDN 杂志专栏上发表任何关于 Windows 8 的文章，直至另行通知。对于我来说，这没有什么。其他出版商似乎没有任何此类限制。为什么冷落 MSDN 呢？好消息是，你现在能读到我的第一篇关于 Windows 8 的文章！具有讽刺意味的是，因为它意味着为后续关于 Windows 8 文章定下了基调。
（**译者注：**读完此文后让人拍案而起，我在翻译过程中加入了一些个人译注，为了更好的把这篇文章分享给对 Windows 开发情有独钟的程序员朋友们。）
Windows 8 是微软送给原生C++ (native C++)开发者的礼物。经过多年的推动，.NET 框架已经被视为“一个正确的道路(one true path)”，微软正以开创性的姿态回归大卫.卡特勒(David Cutler)发布的1993年的Windows NT。大卫.卡特勒(David Cutler)将 Windows 从一个名不见经传的，被Unix开发者嘲笑的操作系统变成了一个桌面操作系统的主导者，服务器市场操作系统的强大竞争对手。G.帕斯卡尔.扎卡里(G. Pascal Zachary)写的观止(Showstopper)这本书，讲述了比尔.盖茨(Bill Gates)聘请大卫.卡特勒(David Cutler)设计和开发 Windows NT 的引人入胜的故事,和他们在开发部分基础软件中所遇到的挑战和斗争，这本书已经不再出版。
**从COM 到 Common Language Runtime(CLR)**
自从 Sun 证明托管代码(managed code )的确是一个可行的选择，微软在开发托管代码的领域里走了一些弯路后（译注，可能是Sun与MS的Java标准之争），微软在 2000年发布了.net Framework 的beta版。在接下来的十年，.net Framework 更是深远影响微软的开发部门甚至整个公司。该信息是明确的：托管代码在许多方面优于本地代码。它是更安全，更可靠，甚至可以更快！这是理论。在实践中，结果被证明是有所不同。
微软把 Windows Form 吹捧为一个开发 Windows 应用程序为更简单的方法。然后微软又开发出Windows Presentation Foundation（WPF）来解决在Windows Form的一些限制。跟着又开发 Silverlight 作为更轻、 更快的 WPF。同样地，微软又开发了 ASP.NET 来简化 web 服务开发，跟着推出被认为是更好的通信方式的 Windows Communication Foundation (WCF)(译注，开发者都被累死了)。然而，这些技术从来没有完全辜负炒作（never quite lived up to the hype）(注：这是反语)。在这之前我已在本专栏中写过原生(native)  Windows Web Services (WWS) API分析[比较文章](http://msdn.microsoft.com/zh-cn/magazine/ee335693.aspx)，其中流量(throughput)和操作数(working set)的比较就完全让WCF激动万分。和原生(native)的 Direct2D API [http://msdn.microsoft.com/zh-cn/magazine/dd861344.aspx]表现出令人印象深刻的性能提升。
自从开发者在2000年接触 .NET Framework beta后，有很多关于微软的技术发展方向的疑问。直至微软提出把COM作为Windows软件组件的首选技术。微软已出版的Kraig Brockschmidt的巨大影响力的<Inside OLE 2 >这一本书，Don Box把在 Essential COM<COM本质论>这一本书中把 .NET Framework 称为更好更简洁的 COM。
微软技术研究员布赖恩.哈利(Brian Harry)，当时为公共语言运行时（ Common Language Runtime,CLR）的开发经理,发出了现在著名的电子邮件,.NET开发邮件列表。这是亲切的称为“布莱恩的小邮件(Brian’s little email)”,长度超过五千字。这封邮件有助于澄清一些理由背后的一个棘手的问题，.NET framework 对于COM的最根本的转变，就是远离资源管理. COM，其核心是通过一个C + +虚函数表现的基于侵入性引用计数模型(invasive reference-counting)。这就是IUnknown接口,COM的引用计数模型的两个问题被认为是不可克服的。首先是引用计数本身的成本。每次调用AddRef和Release通常在一个环环相扣的指令分配一个相对昂贵的参考简单的行为（Each call to AddRef and Release typically resulted in an interlocked instruction making the simple act of assigning a reference relatively costly)（译注：即每次要判断接口引用是否到正确的COM对象）。公平地说,是专为使用COM组件的边界,目的是为了设计一个通用的对象模型,这个组件可以被应用程序中的所有对象使用。应该指出的是,CLR的大部分的初始设计是为了与 Visual Basic兼容,C＃中还不存在。第二个问题是引用计数周期的问题。通过垃圾收集器跟踪，周期不是一个问题。同样，也有其他方式来解决这个问题，例如使用使用标准模板库(Standard Template Library)的弱指针(weak pointers)。
**从.NET 到 Windows Runtime**
不是在建基于CLR，Windows 团队决定是该回到原生代码的这一证实可靠性和性能设计要点上,COM的本质也是设计一下代伟大的Windows API的指导原则。Windows Runtime (WinRT)就是是植根于IUnknown接口的，并以把COM作一个新的方向。
抛弃中间语言代码编译需要的”just-in-time”(.net的托管代码转为处理器的指令的过程)。为什么还要强迫用户再编译一次代码,你可以积极优化，提前将你的程序编译，确保用户第一时间获得最快的体验和获得每一个支持的处理器上的所有时间？抛弃垃圾收集的运行时（garbage-collected runtimes）的不确定性资源管理。处理或不处理，这就是问题。抛弃于复杂和昂贵的托管安全模型。 Windows NT引入了一个完美的应用程序隔离和安全性模型，经得起时间的考验。
当然，在过去十几年也有许多教训，也会应用于COM和现在的WinRT。在他们之前的天真的.net开发人员和 Java 和 UNIX 开发人员还想模拟 Windows 注册表，并提供良好的旧文本文件作为更好的选择。WinRT继续使用注册表,因为它是一种快速、可靠的,甚至简单的方法来管理设置。但是使一个更简单、 更易于管理的环境的确需要一些与CLR 与相关的应用程序封装方法。WinRT也借CLR元数据格式的先验(priori knowledge)组件功能的来增强COM的动态转换(cast)，旧有的QueryInterface!
.NET 开发人员哪儿去了？与 .NET Framework 初次发布时 C + + 开发人员时收到的冷淡对待不同。微软保证.net开发人将会继续获得原生代码一流的体验，虽然性能由于托管代码的性质稍微打点折扣。由于WinRT是基于COM接口,CLR 可以容易地使用其令人印象深刻的 COM 互操作( COM interop)扩展WinCRT。Visual Studio IDE可以继续为.net 开发人员提供继续提供更好的开发体验。与它在最近几年所做的那样，但是C ++开发人员仍然有真正的问题，就且是一个一流的编译器和一个毫不妥协的Windows API(译注：微软需要开发标准的C++编译器同时要支持WinRT特点，即自动为COM对象插入引用计数调用的代码,原来的Windows开发人员会排斥新的Windows API)。
**从C++/CLI 到 C++/CX**
那么，是否意味着C++开发者再次使用 ATL 和 CComPtr 吗？啊，说不准。虽然您可以使用 ATL，但是你有更好的选择。2003 年，Visual C+ + 团队遇到身份危机(译注：他们认为C++也会是CLR上的成员，一分子，不再是独立的了。)。“托管代码”似乎接管世界。也许他们认为需要加快步伐做出点成绩出来。因此，该团队决心重新设计一套语言扩展，即后来的C++/CLI。Visual C+ + 团队著名成员 Herb Sutter 和 Brandon Bray 孜孜不倦地设计出来了一套漂亮的，一流的，最强大的 .net 编程语言。我记得我是最早写了关于 C + + / CLI 的[文章](http://msdn.microsoft.com/en-  us/library/ms379617%28VS.80%29.aspx)。由于我没有编译器，我只是简单写了这些文章，内容的主要信息来源是来自我与Bray 和 Sutter的讨论。
新的语言让人激动万分，但却很短暂。很快 C++/CLI 的遭遇让 Visual C + +团队从他们的身份危机中恢复过来，他们意识到原生(native)代码是非常需要(great need )的，并不是所有人都要用.NET Framework.他们意识到，他们应该为注重本地代码，库和工具的开发者社区提供最佳服务。我们也已经看到他们对这一决定的支持所做出的成果，今天Visual C++已经很好地支持了 C++11 的标准。
跟着，令人惊喜的是，各种关于 C++/CLI 的发展建设会议在9月卷土重来。实际上 C++/CLI 已经与 .NET没有关系了，只是看似有关系。其中最显著的区别在于使用”ref new”上下文关键字来创建对象的而不是 C + + / CLI 的 gcnew 关键字。

C++
```
; html-script: false ]auto w = ref new Widget;
```
ref new 究竟返回什么呢？嗯，我可以更明确了:

C++
```
; html-script: false ]Widget ^ w = ref new Widget;
```
在 C + + / CLI ^ 运算符( ^ operator ) 声明该变量为 CLR 引用类型的 handle。然而,这并不CLR,那么它一定还有别的东西。此外，引用的对象 w 超出作用范围时会发生什么呢？嗯，事实证明，C++/CX 已引入了智能指针( smart pointer)的语义到语言中。Widget ^ 的对象相当于带有别出心载的语法糖的 ComPtr<Widget> 的对象(译注:即 Syntactic sugar 原文是 with some fancy sugar coating,指计算机语言中添加的某种语法，这种语法对语言的功能并没有影响，但是更方便程序员使用 )，ref new 关键字返回的是一个 COM 对象指针，或更确切地说，是一个WinRT对象，这个 handle 负责一个引用计数。一旦该 handle 超出范围，便释放引用。这也许是在代码中更容易理解:

C++
```
void scope()
{
auto a = ref new Widget; // RoActivateInstance
auto b = a; // IUnknown::AddRef
a = nullptr; // IUnknown::Release for a
}   // IUnknown::Release for b
```
RoActivateInstance 在WinRT中相当于传统的COM的CoCreateInstance的功能。你调用RoActivateInstance时就直接会返回一个对象的引用计数已经增加的指针。当 a 被分配到 b 时编译器可以确保 AddRef 被正确调用。同样，每次当一个空指针值被分配到 a 和 b 超出范围时编译器确保 Release 会被调用。
这里关键的是,这不是有趣的新的管理环境。这是良好的原生(native)代码。再者,一些代码帮助说明这个事实:

C++
```
auto a = ref new Widget;
auto raw = reinterpret_cast<IUnknown *>(a);
auto x = raw->AddRef();
 assert(2 == x);
auto y = raw->Release();
 assert(1 == y);
```
没有什么说的, reinterpret_cast 使用的原生(native)代码 ！由于底层的IUnknown接口指针，你自然可以调用其所有成员，包括QueryInterface(译注:COM的操作都是必须通过IUnknown 接口查询)。使用 AddRef 和 Release 只是向你证明这是原生（native）代码，因为这些代码已经证明 a是使用了引用计数的 COM 对象。
特定类型和使用普通C++的智能指针来管理由此产生的引用计数(reference-counted)的接口指针。但是,有很多好的理由这样做。正如我上面显示的，如果你需要一些算法或优化一个位在你的代码更细粒度的控制，你发现你可以轻松地抛开语法糖(译注:即 Syntactic sugar 原文是  sugar coating,指计算机语言中添加的某种语法，这种语法对语言的功能并没有影响，但是更方便程序员使用 )，
在本月的专栏中，我需要花些时间在 历史上（译注:Windows 由 .net 转变到 WinRT 的历史）,对于我们理解 Windows 8 至关重要。下个月请与我一起开始探索 Windows 8 的细节。
肯尼.克尔(Kenny Kerr),是一个对原生 Windows 开发富有激情的软件工匠。你可以在  [http://kennykerr.ca](http://kennykerr.ca/) 了解他。
原文地址  [http://kennykerr.ca/2011/10/18/the-road-to-windows-8/](http://kennykerr.ca/2011/10/18/the-road-to-windows-8/)       编译：忙碌命 [laiboy](http://blog.csdn.net/laiboy/article/details/7096017)
