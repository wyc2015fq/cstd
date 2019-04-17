# 用 Eclipse 平台进行 C/C++ 开发 - 长歌行 - CSDN博客





2012年02月15日 14:20:11[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：602标签：[eclipse																[平台																[websphere																[makefile																[工具																[windows](https://so.csdn.net/so/search/s.do?q=windows&t=blog)
个人分类：[C、C++](https://blog.csdn.net/iuhsihsow/article/category/1072760)







**简介：** 我们将概述如何在 C/C++ 开发项目中使用 Eclipse 平台。尽管 Eclipse 主要是一个 Java 开发环境，但其体系结构确保了对其它编程语言的支持。在本文中，您将学习如何使用 C/C++ 开发工具箱（C/C++ Development Toolkit，CDT），它是可用于 Eclipse 的最佳 C/C++ 工具箱。


**本文的标签：**c, cpp, eclipse, ecllipse,linux,用eclipse平台进行c_c__开发



C 和 C++ 语言都是世界上最流行且使用最普遍的编程语言，因此 Eclipse 平台（Eclipse Platform）提供对 C/C++ 开发的支持一点都不足为奇。因为 Eclipse 平台只是用于开发者工具的一个框架，它不直接支持 C/C++；它使用外部插件来提供支持。本文将向您演示如何使用 CDT — 用于 C/C++ 开发的一组插件。CDT 项目（有关链接，请参阅本文后面的[参考资料](https://www.ibm.com/developerworks/cn/linux/opensource/os-ecc/#resources)一节）致力于为
 Eclipse 平台提供功能完全的 C/C++ 集成开发环境（Integrated Development Environment，IDE）。虽然该项目的重点是 Linux，但它在可使用 GNU 开发者工具的所有环境（包括 Win32（Win 95/98/Me/NT/2000/XP）、QNX Neutrino 和 Solaris 平台）中都能工作。

CDT 是完全用 Java 实现的开放源码项目（根据 Common Public License 特许的），它作为 Eclipse SDK 平台的一组插件。这些插件将 C/C++ 透视图添加到 Eclipse 工作台（Workbench）中，现在后者可以用许多视图和向导以及高级编辑和调试支持来支持 C/C++ 开发。

由于其复杂性，CDT 被分成几个组件，它们都采用独立插件的形式。每个组件都作为一个独立自主的项目进行运作，有它自己的一组提交者、错误类别和邮件列表。但是，所有插件都是 CDT 正常工作所必需的。下面是 CDT 插件／组件的完整列表：
- **主 CDT 插件（Primary CDT plug-in）**是“框架”CDT 插件。 
- **CDT 功能 Eclipse（CDT Feature Eclipse）**是 CDT 功能组件（Feature Component）。
- **CDT 核心（CDT Core）**提供了核心模型（Core Model）、CDOM和核心组件（Core Component）。
- **CDT UI**是核心 UI、视图、编辑器和向导。 
- **CDT 启动（CDT Launch）**为诸如编译器和调试器之类的外部工具提供了启动机制。 
- **CDT 调试核心（CDT Debug Core）**提供了调试功能。 
- **CDT 调试 UI（CDT Debug UI）**为 CDT 调试编辑器、视图和向导提供了用户界面。 
- **CDT 调试 MI（CDT Debug MI）**是用于与 MI 兼容的调试器的应用程序连接器。 

现在，让我们研究一下如何在实际应用程序中使用这些组件。图 1 显示了 Eclipse 中的 C/C++ 项目：

**图 1. 在带有 CDT 插件的 Eclipse 中编辑 C/C++ 项目**
![图 1. 在带有 CDT 插件的 Eclipse 中编辑 C/C++ 项目](https://www.ibm.com/developerworks/cn/linux/opensource/os-ecc/workspace.jpg)

安装和运行 CDT

在下载和安装 CDT 之前，首先必需确保 GNU C 编译器（GNU C compiler，GCC）以及所有附带的工具（make、binutil 和 GDB）都是可用的。如果正在运行*Linux*，只要通过使用适用于您分发版的软件包管理器来安装开发软件包。在*Windows*平台上，将需要安装 Cygwin 工具箱（请参阅[参考资料](https://www.ibm.com/developerworks/cn/linux/opensource/os-ecc/#resources)以获得链接）。Cygwin是用于
 Windows 的类 UNIX 环境，它包括 GCC 移植以及所有必需的开发工具，包括 automake 和 GNU 调试器（GNU Debugger，GDB）。Cygwin是在`cygwin1.dll` 库基础上构建的。Cygwin 的备用解决方案是 Minimalist GNU for Windows（MinGW）（请参阅[参考资料](https://www.ibm.com/developerworks/cn/linux/opensource/os-ecc/#resources)以获得链接）。该工具是一组可免费获取、自由分发的特定于
 Windows 的头文件和导入库，这些头文件和导入库与 GNU 工具集（它们允许您生成不依赖于任何第三方 DLL 的本机 Windows 程序）结合在一起。如果您想要创建与 POSIX 兼容的 Windows 应用程序，那么 MinGW 是最佳选择。MinGW 甚至可以在 Cygwin 安装之上工作。*Solaris*和*QNX*要求您从因特网下载并安装其特定的 GCC、GNU Make binutils 和 GDB 移植（请参阅[参考资料](https://www.ibm.com/developerworks/cn/linux/opensource/os-ecc/#resources)以获得链接）。

假设您安装了适当的 Java SDK/JRE 和 Eclipse 平台 SDK，并且它们都正常运行。CDT 以两种“方式”可用：稳定的发行版和试运行版（nightly build）。试运行版未经完全测试，但它们提供了更多的功能并改正了当前错误。安装之前，请检查磁盘上是否存在先前版本的 CDT，如果存在，请确保完全除去它。因为 CDT 没有可用的卸载程序，所以需要手工除去它。为了检查先前版本是否存在，转至 CDT 插件所驻留的目录：`eclipse/plugins` 。接着，除去所有以`org.eclipse.cdt`
 名称开头的目录。需要做的最后一件事情是从`workspace/.metadata/.plugins` 和`features` 除去 CDT 元数据目录
`or.eclipse.cdt.*` 。 

下一步是下载 CDT 二进制文件。注意：请下载适合于您操作系统的正确的 CDT。遗憾的是，即使 CDT 是用 Java 编写的，它也不是与平台无关的。接着，将归档文件解压到临时目录中，从临时目录将所有插件目录内容都移到 Eclipse`plugins` 子目录。还需要将`features` 目录内容移到 Eclipse
`features` 子目录中。现在，重新启动 Eclipse。Eclipse 再次启动之后，更新管理器将告诉您它发现了更改并询问您是否确认这些更改。现在您将能够看到两个可用的新项目：C 和 C++。



[回页首](https://www.ibm.com/developerworks/cn/linux/opensource/os-ecc/#ibm-pcon)

创建新项目

在 Eclipse 中安装 CDT 之后，浏览至 **File => New => Project**，在那里，您将发现三个新的可用项目类型：C（“Standard C Make Project”）、C++（“Standard C++ Make Project”）和“Convert to C or C++ Projects”。从“Standard Make C++ Project”开始，为您的项目创建源代码文件。在 C/C++ Projects 视图中，单击鼠标右键，然后选择**New => Simple => File**。命名您的文件并保存它。您可能会用这种方法创建许多头文件以及 C/C++ 实现代码文件。最后当然是 Makefile，GNU Make 将使用它来构建二进制文件。对该 Makefile 使用常见的 GNU make 语法（请参阅[参考资料](https://www.ibm.com/developerworks/cn/linux/opensource/os-ecc/#resources)）。请记住：Makefile 要求您使用 Tab
 字符而不是空格来产生缩进行。 

您通常会将现有的源代码导入 Eclipse（请参阅图 2）。CDT 为执行这一操作提供了一种便捷的方法：即使用 Import 向导，将文件从文件系统目录复制到工作台。转至主菜单栏，选择**File => Import => File System**。单击**Next**，打开源目录，选择您想要添加文件的目录。单击**Select All** 以选择目录中的所有资源，然后从头到尾检查，取消选择您不打算添加的那些资源。指定将作为导入目标的工作台项目或文件夹。还可以通过从文件系统拖动文件夹和文件并将它们放入
 Navigator 视图中，或者通过复制和粘贴来导入文件夹和文件。

**图 2. 将现有的源代码导入 CDT 项目**
![图 2. 将现有的源代码导入 CDT 项目](https://www.ibm.com/developerworks/cn/linux/opensource/os-ecc/importing.jpg)



[回页首](https://www.ibm.com/developerworks/cn/linux/opensource/os-ecc/#ibm-pcon)

关键的 CDT IDE 功能

CDT IDE是在 CDT UI 插件所提供的通用可扩展编辑器基础上构建的。然而，该模块仍处于开发阶段，所以它仍缺少一些重要的实用程序，如类浏览器或语言文档浏览器。CDT IDE 的主要功能是：
- **语法突出显示**：CDT IDE识别 C/C++ 语法，并为语法突出显示提供了完全可配置的代码着色以及代码格式化功能： 

**图 3. 不成功的编译之后突出显示的语法错误标记**
![图 3. 不成功的编译之后突出显示的语法错误标记](https://www.ibm.com/developerworks/cn/linux/opensource/os-ecc/error.jpg)
- **提纲**：Outline 窗口模块提供了有关出现在源代码中的过程、变量、声明以及函数的快速视图。利用 outline，您可以方便地找到源代码中的适当引用，或者甚至搜索所有项目源代码。
- **代码辅助**：这个代码完成功能类似于可在 Borland C++ Builder 或 MS Visual Studio 中找到的功能。它使用了*代码模板*，并且只有助于避免愚蠢的语法错误：

**图 4. 有助于使用正确的语言语法的代码辅助功能**
![图 4. 有助于使用正确的语言语法的代码辅助功能](https://www.ibm.com/developerworks/cn/linux/opensource/os-ecc/assist.jpg)
- **代码模板**：由代码辅助功能使用的代码模板是标准 C/C++ 语言语法结构的定义。您也可以定义自己的代码模板来扩展您自己的快捷键，如用于`author` 或`date` 关键字的快捷键。在
**Window => Preferences => C/C++ => Code Templates** 中，可以添加新模板并查看完整的模板列表。也可以将模板作为 XML 文件导出和导入。

**图 5. 预定义的 C/C++ 代码模板**
![图 5. 预定义的 C/C++ 代码模板](https://www.ibm.com/developerworks/cn/linux/opensource/os-ecc/templates.jpg)
- **代码历史记录**：即使您没有使用 CVS 或其它源代码版本管理软件，也可以跟踪项目源代码中的本地更改。在选择的文件上单击鼠标右键，从上下文菜单选择**Compare With => Local History...**：

**图 6. 用 Local History 功能检查源代码中的更改**
![图 6. 用 Local History 功能检查源代码中的更改](https://www.ibm.com/developerworks/cn/linux/opensource/os-ecc/local-history.jpg)



[回页首](https://www.ibm.com/developerworks/cn/linux/opensource/os-ecc/#ibm-pcon)

构建并运行项目

CDT 提供了一种设置项目构建选项的简单方法。CDT 依赖于三个 GNU 工具：GCC、GDB 和 Make。因此，对用于调试的 GDB 或用于编译的 GCC 和 Make 的依赖要求这些应用程序可用于用户想要使用的平台。大多数 Linux（通常和类 POSIX）源代码软件包使用 autoconf 脚本来检查构建环境，所以您必需运行`configure` 命令，该命令在编译之前创建“Makefile”。CDT 没有提供编辑 autoconf 脚本的方法，所以必需手工编写它们；然而，您可以配置构建选项以在编译之前调用`configure`
 命令。 

如果通过调用 `make` 命令来构建项目，那么缺省设置没问题，但是，如果使用一种更复杂的方法进行构建，则必需在 Build Command 文本框中输入适当的命令（例如，`make -f make_it_all` ）。接下来，在 C/C++ Projects 视图中，选择**C/C++ project**，然后单击鼠标右键并选择**Rebuild Project**。所有来自 make、编译器和链接程序的编译消息都被重定向到控制台窗口：

**图 7. 带编译器输出的控制台窗口**
![图 7. 带编译器输出的控制台窗口](https://www.ibm.com/developerworks/cn/linux/opensource/os-ecc/building.jpg)

编译成功之后，您或许想要运行您的应用程序。所有用于运行和调试的选项都位于主 Eclipse 菜单的 Run 菜单下。然而，必须在早期定义用于运行项目的选项。可以通过转至主菜单（在那里，有用于运行应用程序的不同概要文件）中的**Run...** 选项来完成这一步；例如，可以将一个概要文件用于测试目的，而将另一个概要文件用于运行最终版本。另外，可以定义希望要传递给应用程序的参数，或者可以设置环境变量。其它选项用于设置调试选项，例如使用哪个调试器（GNU GDB 或 Cygwin GDB）。图
 8 显示了正在为项目创建运行概要文件（run profile）。

**图 8. 为项目创建运行概要文件**
![图 8. 为项目创建运行概要文件](https://www.ibm.com/developerworks/cn/linux/opensource/os-ecc/running.jpg)

当进入 C/C++ Projects 视图，选择您的项目，单击鼠标右键并在 Build Settings 选项卡上选择 **Properties** 之后，就可以使用更多的用于构建项目的常用选项。这些选项主要影响因遇到编译错误而使构建停止时所发生的情况。



[回页首](https://www.ibm.com/developerworks/cn/linux/opensource/os-ecc/#ibm-pcon)

调试 C/C++ 项目

CDT 扩展了标准的 Eclipse Debug 视图，使之具备了调试 C/C++ 代码的功能。Debug 视图允许您在工作台中管理程序的调试或运行。要开始调试当前项目，只要切换到 Debug 视图，您将能够在代码中设置（并在执行过程中随时更改）断点／监测点并跟踪变量和寄存器。Debug 视图显示正在调试的每个目标的暂挂线程的堆栈框架。程序中的每个线程都作为树中的一个节点出现，Debug 视图显示正在运行的每个目标的进程。

Eclipse 通过 CDT 调试 MI（CDT Debug MI）插件（其组件之一）支持与机器接口（Machine Interface，MI）兼容的调试器。但 MI 调试器究竟是什么呢？通常情况下，象 ddd 和 xxgdb（请参阅[参考资料](https://www.ibm.com/developerworks/cn/linux/opensource/os-ecc/#resources)以获得链接）之类的第三方 GUI 调试器在实现调试功能时都依赖于 GDB 的命令行接口（Command
 Line Interface，CLI）。遗憾的是，经过证实该接口非常不可靠。GDB/MI 提供了一种新的面向机器的接口，它非常适合于想要直接解析 GDB 输出的程序。



[回页首](https://www.ibm.com/developerworks/cn/linux/opensource/os-ecc/#ibm-pcon)

结束语

C/C++ 开发工具箱（CDT）提供了一组功能强大的插件，这些插件可以帮助您用 Eclipse 开发 C/C++ 应用程序。虽然 CDT 仍处于开发阶段，但您可以利用其现有的诸多功能。


参考资料 
- 您可以参阅本文在 developerWorks 全球站点上的 [英文原文](http://www.ibm.com/developerworks/library/os-ecc/index.html?S_TACT=105AGX52&S_CMP=cn-a-l). 


- 请加入 Eclipse 平台社区，并从 [eclipse.org](http://www.eclipse.org/)下载该平台。Eclipse 平台源代码是根据 Common Public License 特许的。在 eclipse.org 上，您还将找到有关 Eclipse 项目的术语词汇表和描述，以及技术文章和新闻组。Eclipse 平台白皮书详细描述了 Eclipse 的主要组件和功能。




- 从 eclipse.org 下载 [CDT 项目](http://dev.eclipse.org/viewcvs/index.cgi/~checkout~/cdt-home/downloads/main.html?cvsroot=Tools_Project)的组件。 




- 请访问 eclipse.org 上的 [新闻组](http://www.eclipse.org/newsgroups/index.html)，以获取与 C/C++ 插件相关的技术讨论和问题。




- 开发人员可以通过为该项目中每个组件所创建的 [开发人员邮件列表（developer mailing lists）](http://www.eclipse.org/mail/index.html)来参与和 C/C++ IDE 实现相关的讨论。




- CDT 项目使用 bugzilla 作为其错误和功能跟踪系统。在 Eclipse [bugzilla 页](http://dev.eclipse.org/bugs/)上输入错误／功能报告与填写 Web 表单一样简单。 




- 有关 Eclipse 平台及其工作原理的入门文章，请参阅由 Greg Adams 和 Marc Erickson 合著的 *developerWorks*文章“[Working the Eclipse Platform](http://www.ibm.com/developerworks/library/os-plat/?S_TACT=105AGX52&S_CMP=cn-a-l)”。




- 要开始使用 Eclipse 平台开发应用程序，请参阅由 David Gallardo 撰写的 *developerWorks*文章“ [Getting started with the Eclipse Platform](http://www.ibm.com/developerworks/library/os-ecov/?S_TACT=105AGX52&S_CMP=cn-a-l)”。 




- 如果您有兴趣创建自己的 Eclipse 插件，请参阅由 David Gallardo 撰写的 *developerWorks*文章“ [开发 Eclipse 插件](https://www.ibm.com/developerworks/cn/java/os-ecplug/index.html)”。 




- 从 Red Hat 下载 [Cygwin 工具箱](http://www.redhat.com/software/cygwin/)。




- 从 MinGW.org 下载 [MinGW 工具箱](http://www.mingw.org/)。 




- 在 gnu.org 上学习有关 [GNU make 语法](http://www.gnu.org/software/make/make.html)的更多知识。




- 从 gnu.org 下载 [ddd](http://www.gnu.org/software/ddd/)GUI 调试器。 




- 下载 [xxgdb](http://www.ee.ryerson.ca:8080/~elf/xapps/Q-IV.html)GUI 调试器。




- 请访问 [WebSphere Studio Plug-in Central](http://www.ibm.com/developerworks/cgi-bin/click.cgi?S_TACT=105AGX52&S_CMP=cn-a-l&url=http://www7b.software.ibm.com/wsdd/downloads/plugin/&origin=l)，以获取已被验证为“Ready for WebSphere Studio Software”的插件的目录。




- 学习 IBM WebSphere Studio 工具的一个扩展，它提供了有助于您使用这些工具的信息和工具。由于所有 WebSphere Studio 工具都基于 WebSphere Studio 工作台（WebSphere Studio Workbench），而工作台基于 Eclipse 框架，所以很容易通过使用 Eclipse 帮助扩展功能来扩展产品文档。请参阅文章“[Adding
 Self-Help Support to WebSphere Studio Tools by Extending their Eclipse Help Systems](http://www.ibm.com/developerworks/cgi-bin/click.cgi?S_TACT=105AGX52&S_CMP=cn-a-l&url=http://www7b.software.ibm.com/wsdd/library/techarticles/0301_mckay/mckay.html&origin=l)”。




- 请在 *developerWorks*上查找更多的 [Eclipse 用户](http://www.ibm.com/developerworks/oss/?S_TACT=105AGX52&S_CMP=cn-a-l)参考资料。 





关于作者



Pawel Leszek 是 [Studio B](http://www.studiob.com/) 的一位作家，他是一位专长于 Linux/Win/Mac OS 系统体系结构和管理的独立软件顾问和作家。他具有许多操作系统、编程语言和网络协议方面的经验，尤其是 Lotus Domino 和 DB2 方面的经验。Pawel 还是*LinuxWorld*上一系列文章的作者，以及*PC World*波兰版的 Linux
 专栏作家。Pawel 和他的妻子以及可爱的小女儿住在华沙。欢迎提问并提出意见；您可以直接给作者发电子邮件（[pawel.leszek@ipgate.pl](mailto:pawel.leszek@ipgate.pl?cc=)）。](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=makefile&t=blog)](https://so.csdn.net/so/search/s.do?q=websphere&t=blog)](https://so.csdn.net/so/search/s.do?q=平台&t=blog)](https://so.csdn.net/so/search/s.do?q=eclipse&t=blog)




