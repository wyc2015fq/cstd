# 探索嵌入式应用框架（EAF） - 我相信...... - CSDN博客





2018年03月18日 00:00:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1026
所属专栏：[IoT与智能硬件](https://blog.csdn.net/column/details/19754.html)










EAF是Embedded Application Framework 的缩写，即嵌入式应用框架。嵌入式应用框架是 Application framework的一种， 是在嵌入式领域的应用框架。

Application Framework——应用框架，是一种软件框架，软件开发人员用应用框架作为标准结构，以便实现应用软件。

那什么是软件框架呢？

## 关于软件框架

软件框架是一个抽象的概念, 是提供了通用的软件功能，可以通过用户编写代码有选择地改变, 从而提供特定的软件应用。 软件框架提供了构建和部署应用程序的标准方式。 软件框架是一个通用的、可重用的软件环境, 它提供特定的功能, 作为大型软件平台的一部分, 以促进软件应用程序、产品和解决方案的开发。 软件框架可能包括辅助程序、编译器、代码库、工具集和API , 汇集了所有不同的组件, 便于一个项目或系统的开发。

那些我们用过或者常见的软件框架太多了：
- 
windows上MFC

- 
.net Framework

- 
spring framework以及衍生的SSH等等

- 
Entity Framework

- 
Robot Framework

- 
play framework

- 
Zend framework

- 
Oracle Application Development Framework

- 
Cactus Framework 

- 
Eclipse XXX framework

- 
AWS Lambda Framework

- 
FrondEnd framework：Vue.js,Angular.js,React等等

- 
......


尤其是Web 应用框架更加丰富多彩，基于各自语言实现的Web Application Framework 不胜枚举。

进一步， 很多的系统架构也引入Framework 的概念，例如，Enterprise architecture framework等等，甚至在研发管理上也同样涌现了Framework，例如 Project Management Framework，Risk Management Framework等，外延在不断在融合放大。

关于软件应用框架的开发著述也有很多，例如侧重API设计的《软件框架设计的艺术》等等。

## 嵌入式系统的应用框架

与一般的电脑相比, 嵌入式设备有着自己的特性, 例如耗电量低、体积小、操作范围粗糙、单位成本低。 这是以有限的加工资源为代价的, 这使得程序和交互的难度大大增加。 然而, 通过在硬件之上建立智能机制, 利用可能存在的传感器和存在一个嵌入式单元网络, 既可以在单位和网络一级对现有资源进行最佳管理, 并提供远远超出现有可用功能的增强功能。 这或许就是嵌入式应用的必然性。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1Gjczp4MnVIpj9B5QODjadJ9a3ISzMtpZaSeNR1B3ITjCEibkOibhFrbDccVdTDqaXA79mzXB8HY7iclND3A/640?wx_fmt=jpeg)

EAF 是面向嵌入式系统的软件应用框架。嵌入式系统是一个在较大的机械或电气系统中具有专用功能的计算机系统, 通常具有实时计算约束。嵌入式系统从便携式设备, 如数字手表和 MP3播放器, 到大型的固定设备, 如交通灯, 工厂控制器, 以及大部分复杂的系统, 如混合动力车、核磁共振成像和航空电子设备等等。 复杂性从低到单一的微控制器芯片, 到非常高的多个单位等等。

鉴于如此的复杂，很难有一个统一的应用开发框架。

但是，参考Java 面向资源的分类（J2EE，J2SE，J2ME，Java Card等），我们可以对嵌入式系统的应用框架进行尝试分析。

## 手机上的应用框架

由于手机的功能越来越强大，很多时候被认为超出了嵌入式系统的范畴。但是，了解手机上的应用框架，对于EAF而言，还是大有裨益的。

手机上的应用开发框架一般也被称为 Mobile Development Framework。近些年，随着智能手机的普及，面向手机开发的应用框架已被人们所熟知，例如 ios development framework，Android development framework，还有基于HTML5的混合编程框架PhoneGap等等。

尤其是Android，如果资源允许，基于android的嵌入式设备可以轻松使用android的软件应用框架。google 还面向嵌入式设备推出了android wearable 等一系列方案，当然iOS 也是如此。

下图就是那张经典的android 架构层次图：

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1Gjczp4MnVIpj9B5QODjadJ9a3Ivle3HKrHibp5ibWHFZmz8JleygEib16ac0TI6yNOuAyTcjFtLKuxVxiacA/640?wx_fmt=jpeg)

Android应用程序框架分为应用层、应用框架层、系统运行库层和Linux内核层，在开发应用时就是在这个框架上进行扩展。Android应用框架的部分功能如下：
- 
android.app：提供高层的程序模型和基本的运行环境。

- 
android.content：包含对各种设备上的数据进行访问和发布。

- 
android.database：通过内容提供者浏览和操作数据库。

- 
android.graphics：底层的图形库,包含画布,颜色过滤,点,矩形，可以直接绘制屏幕上。

- 
android.location：定位和相关服务的类。

- 
android.media：提供一些类管理多种音频、视频的媒体接口。

- 
android.net：提供帮助网络访问的类，超过通常的java.net.*接口。

- 
android.os：提供了系统服务、消息传输和IPC机制。

- 
android.opengl：提供OpenGL的工具。

- 
android.provider：提供访问Android内容提供者的类。

- 
android.telephony：提供与拨打电话相关的API交互。

- 
android.view：提供基础的用户界面接口框架。

- 
android.util：涉及工具性的方法，例如时间日期的操作。

- 
android.webkit：默认浏览器操作接口。

- 
android.widget：包含各种UI元素（大部分是可见的）在应用程序的布局中。


智能手机的资源还是相当丰富的，在智能手机之前的Feature Phone 同样有着自己的应用开发框架，只是不如Android 和iOS 那么普及，已经逐渐被人们所遗忘。 但是，这些应用框架的设计思想和实现方式同样有着重要的参考性。例如，Qualcomm的BREW（翻开了历史的回忆）。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1Gjczp4MnVIpj9B5QODjadJ9a3ItSYJGqiaQaGemFicW8IJBic6N2ZGflvwZic2S36JoqHCH8ow48rqofEUEQ/640?wx_fmt=png)

BREW 指Binary Runtime Environment for Wireless 的缩写，从基本的层面而言，BREW 平台就是手持设备上嵌入式芯片操作系统的接口或抽象层。
- 
BINARY 是指二进制。BREW的编程接口是一套二进制的函数库。所有基于BREW的应用和扩展类被编译和联接成二进制代码，在本地执行。

- 
RUNTIME是指运行时间。所有基于BREW的应用和扩展类志在运行时被发现和调用，这一点很像动态连接库，事实上，BREW的应用和扩展类的模拟器版本就是一个DLL。

- 
ENVIRONMENT是指环境。BREW是一个开放而且灵活的环境，提供了大量的编程接口，并可以管理丰富的业务。

- 
WIRELESS是指无线。BREW 可以充分无限设备的特性，快速有效的运行与低 RAM/FLASH 的环境中，使有限的无线网络资源得到有效的使用。


BREW基本上遵从COM这一组件构架的。组件架构的一个优点就是应用可以随时间的流逝而发展进化，除此之外，使用组件还有一些可以使对以有应用的升级更加方便和灵活的优点，如应用的定制，组件库以及分布式组件等。

## M2M的应用框架

鉴于 M2M 技术的特点, 系统设计者可能不得不从头开始构建整个 M2M 体系结构。其核心是, M2M 技术包括增加一个装置或设备的智能服务, 并将该设备与可以监控或控制该设备的后端基础设施连接起来。 为了实现这一目标, 一个 M2M 设备使用了两个基本元素: 与后端通信的基础设施(无线调制解调器或模块)和运行服务的软件。

通过提供一种将 M2M 服务直接嵌入通信模块的方法, 以及预先安装装的软件模块、连接能力和处理资源方式，一般的M2M 应用框架如下：

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1Gjczp4MnVIpj9B5QODjadJ9a3I4VbPSMqQuJ0TdfYVQNRk4ngYHa0HHtcCTcWfqCJFGk5uyzVnUzHY4g/640?wx_fmt=jpeg)

EAF一般包括下列组成部分:

### 优化的轻量级操作系统

虽然一些 M2M 应用程序需要更强大的 RTOS, 但大多数都不需要。 操作系统的设计能够提供 API 来控制语音, 数据调用, 短信, 以及 TCP/IP 连接。 应该经过优化, 以充分利用直接访问协议栈的能力。 为了提供对连接应用程序的全面支持, 操作系统还应提供一个核心功能集, 其中包括:
- 
实时性, 包括保证对外部或内部中断的响应时间, 不论其状态如何。

- 
灵活安排任务的优先顺序。

- 
多任务能力, 以定义和同步服务所需的任务。

- 
在处理速度和功率选择方面的灵活性, 以优化电池寿命。

- 
内存、固件和软件保护功能。

- 
能够使用API访问蜂音频等媒体和数据路径。


### 软件库

为了简化开发过程和市场的速度时间, EAF 应该包括各种软件库和API, 提供设备或服务可能需要的各种功能。 这包括定位、全面的互联网连接协议、无线和互联网安全等服务。 EAF还应支持为目标市场具体需求开发的第三方软件库。 理想情况下, EAF 不仅应得到通信模块供应商的支持, 还应该得到合作伙伴和开发者的支持。

### 开发工具

EAF 还应该包含一个开发工具包, 以便于编码、调试和监视 M2M 应用程序, 这些工具应该是开源的, 可以免费使用。 最终, EAF 应该提供开发 M2M 应用程序并将其嵌入模块所需的一切。

### 云连接

最后, EAF 应该提供工具来简化连接设备的云管理, 包括一个完全实现的系统来处理设备监控和软件/固件升级。 该系统应该允许开发者监控设备的健康状况, 并识别潜在的问题。 它还应该包括已验证的操作系统远程升级工具, 以及使用补丁机制的热修复工具。

## 面向脚本的EAF

对于为嵌入式系统创建软件的开发者而言，对脚本的编写并不陌生。 选择脚本是往往是解决问题的最快方法。一般地，脚本被用来自动构建和运行验证测试。作为全栈必备的Javascript被广泛应用于网页、 web 服务器和移动应用程序中，在嵌入式系统中也将占据一席之地。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1Gjczp4MnVIpj9B5QODjadJ9a3IbKYm9ehzPQo4WK97dictjPffibwPWfzicicEl3tVxhdT5Bpn7iafzCE7ZUQ/640?wx_fmt=jpeg)

出于对性能的担心，可以通过一些技术手段提升JavaScript的运行性能：

1.充分利用内置函数和对象。 Javascript 语言拥有支持数组、 JSON、正则表达式和其他字符串运算的复杂内置对象。 这些实现通常在 JavaScript 引擎中得到了很好的优化。

2.小心编码。由于脚本比本地代码慢, 代码优化在性能重要的地方是至关重要的。 Javascript 的动态特性意味着 JavaScript 引擎通常不能像 c 编译器那样有效地优化代码。

3.混合编程。没有一种语言适用于所有情况, 所以要为工作选择最好的语言, 而不是100% 纯粹地使用。 每一个 JavaScript 引擎都提供了一种从脚本调用本地代码的方法。 如果不确定性能, 首先在 JavaScript 中实现, 因为它更容易执行。 如果存在性能瓶颈, 请在 c 中重新实现函数。

在 Mozilla 的一个项目 asm.js 中出现了一个可以替代使用 c 的方法。 定义了一个严格的 JavaScript 子集, 它更容易被翻译成本地代码或 c 源代码。 利用 c 和 JavaScript 之间的语法相似性, 使翻译变得简单明了。 嵌入式开发人员可以在 asm.js JavaScript 子集中编写性能关键代码, 并将其编译为本地代码, 作为构建过程的一部分, 而不是依赖 JIT。

对于内存的使用而言，从积极的一面来看, JavaScript 使用了一个垃圾收集器, 从而消除了显式释放内存的需要。 通过这种简化, 嵌入式开发人员有时间专注于其他方面的开发。

目前，已经有多个开发系统开始支持嵌入式的JavaScript：
- 
Espruino是一个单片机开发板, 被称为微控制器的 JavaScript。 它运行开源的 Espruino JavaScript 引擎, 它支持 JavaScript 标准的子集。

- 
Tessel 是一个集成了Wi-Fi的JavaScript 微处理器，并提供一个兼容Node.js的 API，开发人员可以使用Node.js进行开发。

- 
Marvell Semiconductor 的 Kinoma Create 是一个运行在 Marvell 半导体上的"JavaScript 物联网构建工具包", 它支持完整的 JavaScript 5.0.


在国內，也有类似的开发系统，例如老友周爱民老师所在的ruff.io。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1Gjczp4MnVIpj9B5QODjadJ9a3ISUwVpIAEh0CpJgAjhEiafGS2aOecgxIibJC1Q5ibm37wfPO08CNnQGoQA/640?wx_fmt=jpeg)

总之，Javascript 在嵌入式设备上的潜力很大, 可以简化嵌入式程序员的开发工作, 并为客户提供更可靠、更可定制的产品。







综上所述，嵌入式应用框架（EAF）对嵌入式系统的成功有着重要的意义。特别是对于嵌入式开放平台而言，能够提高开发者的开发效率，促进生态系统的繁荣。然而，IoT中还有很多超轻量级的设备，或许根本没有OS，被称为“智能颗粒”，笑称“智霾”，可能不需要EAF，也可能是一种另外形态的EAF，只是我不知道而已。




***参考：***
- 
Jaroslav Tulach， 《软件框架的设计艺术》，人民邮电出版社，2011

- 
金泰延 宋亨周 朴知勋 李白 林起永， 《Android 框架揭秘》，人民邮电出版社，2012

- 
曹洪伟，《BREW 进阶与精通》，电子工业出版社，2009

- 
http://www.embedded-computing.com/hardware/embedded-application-frameworks-simplifying-the-development-of-m2m-devices

- 
http://www.embedded-computing.com/embedded-computing-design/javascript-for-embedded-devices

- 
http://www.embedded-computing.com/embedded-computing-design/speeding-embedded-systems-time-to-market-using-node-js








