# VS2008中影响exe大小和速度的编译选项 - 三少GG - CSDN博客
2013年03月08日 03:14:06[三少GG](https://me.csdn.net/scut1135)阅读数：2088

完全脱离编程环境的C/C++学习方法，不是好的方法，现在所谓的环境中立理论就是“什么都不学”理论，VC、GCC，主流的就两个，精通其中一个就能吃遍天下，教材里就应该选择一个大讲特讲！
作为VC的代表，今天我给大家全面介绍VC中的编译器选项，全面介绍不需要，MSDN里从头到尾都介绍完了，今天我只讲对生成的exe文件大小和速度有影响的。
用VC就得用IDE，我也以IDE的工程设置里面的排列顺序介绍，某些选项需要自己手动添加的最后介绍，我后面说的默认值是release的，debug版本一般不需要调选项。
**项目 - 属性 -配置属性 - C/C++，这是编译器选项。**
优化：
通常，**算法程序选择最大化速度(/O2)，**界面程序选择最小化大小(/O1)，可以获得最佳的效果。
优选大小或速度，只有在使用完全优化(/Ox)时才有效，完全优化一般不推荐使用，用处就是可以生成速度与/O2基本相当，但是体积更小的代码（选速度优先的话）。
其他几个选项实际上已包含在/O1、/O2之中，具体请看MSDN。
代码生成：
启用字符串池(/GF)，会将相同的字符串合并，当然可以减小空间占用，虽然本项目默认没有打开，但是默认的/Zi选项会自动打开/GF，这里打不打开一样。
启用C++异常：该项默认打开，在C++项目中（比如MFC中），会大大增加程序体积，增加约30%，关闭并不代表try不能用了，但会一定程度上降低健壮性，对于空间要求较高的程序，建议关闭。对于正式项目，请参见MSDN，看看会不会造成不利影响。
运行库：默认多线程DLL(/MD)，体积最优的方案，如果对方没有VS运行时库，选择/MT会将C/C++运行库静态编译，体积增加不少，因此，我的选择一般是程序与redist包一起发布，也就几M，而且以后永远可以接受/MD版本了。
缓冲区安全检查：关闭的话，减少0.5K~1K体积（默认情况，VC的段长度512字节，因此程序体积变化的最小单位是0.5K）。
**启用增强指令集：VS2008只支持到SSE2.**真想用SSE3的话去用Intel
 C++.如果安装了Intel C++ 11，可集成与VS2008，同样的地方选择SSE3效果超群。
浮点模型：精确还是快速理论上肯定对速度有影响，但是我极少使用浮点编程，我的方向是系统、安全和密码，都是整数的天下。
高级：
编译为C还是C++影响不大，这充分说明了C++简单面向对象特性和C效率差不多（如重载，默认情况下，编译器会检查扩展名决定目标代码类型，对于cpp文件，所有的函数都会编译为可重载的类型，但是对效率几乎没有影响）。
**项目 - 属性 -配置属性 -链接器，这是链接器选项。**
输入：
忽略库只有在库冲突时候才有用，VC绝对不会连接没有调用到的库，哪怕你明确指定了。
清单文件：
完全使用API编程可以不生成清单。减少约1K体积。
一般情况下，关闭UAC的那一项，可减少0.5K。
调试：
关闭“生成调试信息(/DEBUG)”，根据程序规模，可减少1K~几十K。
优化：
    release模式，默认情况下已经该组已经最优了，/OPT:REF和/OPT:ICF已经打开，注意，VS2005、VS2008中Windows 98优化那一项没用，不像VC6取消Windows
 98优化可以大大减小体积。因为VS2005、VS2008中段大小已经是512字节，VC6默认4K。
高级：
指定入口点，可以大大减小程序体积，但是不调用CRT的入口无法自动处理参数，可用GetCommandLine和CommandLineToArgvW这两个API来处理参数。
随机基址：默认模式启用映像随机化(/DYNAMICBASE)，会大大增加程序体积，因为这是个增加程序防反编译、防破解能力的选项。如无需求，请选择禁用映像随机化(/DYNAMICBASE:NO)，文件越大，体积缩小越明显，至少30%。
命令行：
小程序，可以指定段大小/ALIGN，/O1编译的化最小可以使用/ALIGN:4，这个选项不推荐，第一有点规模的程序就不能用太小的段，/O2优化的也不能用小段，而且默认的512字节段可以使用UPX压缩，再小就不能了，除非咱们编译那种600字节的Hello
 World，这个选项意义不大，因此微软才没有给他一个图形选项。
同样，编译600字节hello world还需要/merge合并段选项，同样不推荐使用。
有些选项VS2005和VS2003没有，VS2003还包括几个VS2008废除的选项，实际上VC里面程序优化效率最高的个人感觉是VS2003。VC6的界面差别比较大，选项有一定差异，但毕竟都是微软的产品，差别不大，甚至于MASM这个汇编编译器，连接选项大都与VC相同……
再说一点，VS2008SP1的MFC工程会自动生成巨大的256*256真彩图标，因此默认的MFC对话框程序都有近100K，建议删除多余的图标，配合上述选项能减到10多K
-----------------------------------------------------------------------------------
### [vs2008编译时出现问题解决方法：MSVCRTD.lib(crtexew.obj) : error LNK2019: unresolved external symbol _WinMain@16 referenced in function](http://blog.csdn.net/tyronewj/article/details/5042945)

今天，用VS2008编译一个C语言的工程，修改完所有错误后，出现了如下的错误：
MSVCRTD.lib(crtexew.obj) : error LNK2019: unresolved external symbol[_WinMain@16](mailto:_WinMain@16)referenced in function ___tmainCRTStartup
在网上搜了一下，做如下修改：
Project -> xxxx Properties -> Configuration Properties -> Linker -> System ->Console ，原来是/subsystem:windows，这里改为(/SUBSYSTEM:CONSOLE)。
修改后， 该问题解决。
这个工程的入口函数是main。还有一个详细的描述：
[http://blog.csai.cn/user1/16781/archives/2006/6412.html](http://blog.csai.cn/user1/16781/archives/2006/6412.html)
#### 一，问题描述
error LNK2001: unresolved external symbol _WinMain@16
debug/main.exe:fatal error LNK 1120:1 unresolved externals
error executing link.exe;
#### 二，产生这个问题可能的原因
1， 你用vc建了一个控制台程序，它的入口函数应该是main, 而你使用了WinMain.
2. 你用vc打开了一个.c/.cpp 文件，然后直接编译这个文件，这个文件中使用了WinMian而不是main作为入口函数。vc这时的默认设置是针对控制台程序的。
#### 三， 解决方法
**如果入口函数是WinMain，**
**1.进入project->setting->c/c++, 在category中选择preprocessor,在processor definitions中删除_CONSOLE, 添加_WINDOWS**
**2.进入project->setting->Link, 在Project options中将 /subsystem:console改为/subsystem:windows.**
**3.保存设置，Rebuild All.**
**如果入口函数是main，做反向的修改。**
**四，VS2005中的设置**
1.菜单中选择 Project->Properties, 弹出Property Pages窗口
2.在左边栏中依次选择：Configuration Properties->C/C++->Preprocessor,然后在右边栏的Preprocessor Definitions对应的项中删除_CONSOLE, 添加_WINDOWS.
3.在左边栏中依次选择：Configuration Properties->Linker->System,然后在右边栏的SubSystem对应的项改为Windows(/SUBSYSTEM:WINDOWS)
4.Rebuild All. Ok 。

