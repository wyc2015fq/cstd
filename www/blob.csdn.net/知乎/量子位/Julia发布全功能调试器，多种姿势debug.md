# Julia发布全功能调试器，多种姿势debug - 知乎
# 



> 铜灵 编译整理
量子位 出品 | 公众号 QbitAI




Julia这门语言，因为集合了C语言的速度、Ruby的灵活、Python的通用于一身，获得了万千程序员的喜爱。

福利来了~Julia团队又发布了新的**全功能**调试器，可以以多种姿势调试代码，节省你的工作量。

有了这个新调试器的加持，你可以用Julia完成此前达不到的惊奇效果，包括：
- 直接进入函数并手动运行代码，同时检查它的状态
- 设置断点（breakpoint）、捕捉error，检测到底是哪里出现了错误
- 交互式地更新和替换现有代码，并且还能无需重新启动快速修复bug
- 用Juno中的全功能IDE，在易于使用的图形界面中将这些功能捆在一起

是不是有点跃跃欲试了？
![](https://pic4.zhimg.com/v2-27754e6e7eb148ca3867fc0866650533_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='450'></svg>)
## **核心角色**

这个全功能调试器为何如此秀，Julia官网解释说，这是因为里面有一个核心角色：

一个强大的编译器。

官方解释说，这个编译器可以运行Julia代码，同时支持各种前端控件控制具体执行。

每一个前端都是自己的包（package）：Juno IDE中就结合了调试器，Rebugger提供了一种REPL文本的UI， Debugger负责提供一般的step/next/continue命令行接口。

所有这些调试功能都可以与Revise无缝集成，所以，你可以在单独的session中连续分析和修改代码。

## **调试器新功能×2**

下面一起来看新功能具体介绍~

**Juno**

Juno为编译器提供了丰富的接口，支持设置断点，还可以在源代码中直接执行。调试器REPL可以在本地环境中执行任意代码，Workspace可以帮助你检查局部变量。

下面这张图就展示的是**gcd**调试环节：



![](https://pic2.zhimg.com/v2-e2a2deae1c903dd4280c09702a0a84e1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='610'></svg>)



Juno.@run宏指令负责解释你的代码，如果遇到断点，则Juno.@enter支持你从第一行重新开始。

**Debugger和Rebugger**

这是两个REPL接口，Debugger提供类似gdb的 “step, next, continue”接口，而Rebugger提供的是类似IDE一样的控制接口。

Debugger能提供一些其他接口没有的功能，比如对stepping的精细控制，比如执行生成函数的生成器，所以很多情况下Debugger可以成为你的首选。



![](https://pic4.zhimg.com/v2-8862662591a07c974d749c041aaa39fb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='909' height='591'></svg>)



## **神奇的包**

全能新调试器不仅带来了全新的功能，还为一些已经发布的Julia包赋予了新功能。

它们可能已经不是你熟悉的包了，Julia中已经形成了新的包“生态系统”，由四组大将组成，分别是JuliaInterpreter、LoweredCodeUtils、CodeTracking、Revise和Rebugger。

**JuliaInterpreter**

JuliaInterpreter是整个堆栈的关键角色，包含各种评估和检查Julia代码运行情况的各种逻辑。

现在，JuliaInterpreter已经在Julia 1.0的基础上扩展了很多新功能，包括：
- 默认递归：通过编译器运行几乎所有代码，实现断点和捕捉error更加直接。
- 性能增强：现在通过代码运行step-wise的速度是原始速度的50倍。
- 新获得了编译“顶级代码”的能力：现在，连定义包和创造测试套件的代码，也可以用它来编译了。
- 支持断点：你可以把它当作编译器自身另一种形式的控制流。现在可以支持在特定源代码行或特定方法的入口处设置断点，还能自动识别错误。

**LoweredCodeUtils**

LoweredCodeUtils可以用于建立多种协作方法间的联系，从而起到桥梁的作用。

不过，LoweredCodeUtils是最新包中最深入最难懂的，让不少人头大。

举个例子，下面这个看似简单定义，实际上创建了五种方法，分别是一种body method，两种定位参数方法和两种关键字函数法。
![](https://pic3.zhimg.com/v2-36c96de15893e5102185b81cda2ae442_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='93'></svg>)
**CodeTracking**

CodeTracking是一个简单、轻量级的查询API，可以用于从Revise中进行检索。

一般的工作场景是LoweredCodeUtils执行分析，Revise掌管随时间推移出现的各种变化，而CodeTracking负责将这些变化通知到其他组件。

三者分工明确，方便好用。

**Revise和Rebugger**

这个包最大的变化莫过于一眼看过去，可以发现编译的界面产生了变化。此外，Rebugger还获得了一些新的能力，比如在REPL中定义处理方法。

从长远来看，它对JuliaInterpreter和LoweredCodeUtils的深入分析可能会实现更多以前无法实现的功能。

## **传送门**
[https://julialang.org/blog/2019/03/debuggers​julialang.org](https://link.zhihu.com/?target=https%3A//julialang.org/blog/2019/03/debuggers)



— **完** —

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态

戳右上角「+关注」获取最新资讯↗↗

如果喜欢，请分享or点赞吧~比心❤


