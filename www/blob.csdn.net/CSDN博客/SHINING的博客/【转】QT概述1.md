# 【转】QT概述1 - SHINING的博客 - CSDN博客
2010年03月30日 10:21:00[snsn1984](https://me.csdn.net/snsn1984)阅读数：1295
来源：百度百科。　
Qt是一个跨平台的[C++](http://baike.baidu.com/view/824.htm)图形[用户界面](http://baike.baidu.com/view/362528.htm)应用程序框架。它提供给应用程序开发者建立艺术级的图形用户界面所需的所用功能。Qt是完全[面向对象](http://baike.baidu.com/view/125370.htm)的，很容易扩展，并且允许真正地组件[编程](http://baike.baidu.com/view/3281.htm)。
　　自从1996年早些时候，Qt进入商业领域，它已经成为全世界范围内数千种成功的应用程序的基础。Qt也是流行的Linux桌面环境KDE 的基础，[KDE](http://baike.baidu.com/view/281881.htm)是所有主要的Linux发行版的一个标准组件。
Qt支持下述[平台](http://baike.baidu.com/view/58664.htm)：
[MS](http://baike.baidu.com/view/17855.htm)/[Windows](http://baike.baidu.com/view/4821.htm) - 95、98、NT 4.0、ME、2000、XP 和 Vista
[Unix](http://baike.baidu.com/view/8095.htm)/X11 - [Linux](http://baike.baidu.com/view/1634.htm)、[Sun](http://baike.baidu.com/view/24856.htm)[Solaris](http://baike.baidu.com/view/329359.htm)、HP-UX、Compaq Tru64 UNIX、IBM AIX、SGI IRIX和其它很多X11平台
Macintosh - Mac OS X 
Embedded - 有帧缓冲(framebuffer)支持的Linux平台，Windows CE。
Symbian/S60 - 目前已经可以提供技术预览版本
Qt是诺基亚公司的一个产品。
Qt被按不同的版本发行：
Qt商业版提供给商业软件开发。它们提供传统商业软件发行版并且提供在协议有效期内的免费升级和技术支持服务。如果要获得最新信息，请浏览诺基亚公司Qt Software网站页面，或者与sales@trolltech.com联系。商业版多一些扩展模块。
Qt开源版是Qt仅仅为了开发自由和开放源码软件，提供了和商业版本同样的功能。GNU通用公共许可证下，它是免费的。最新的版本可以在这里下载。
　　从2009年3月发布的Qt 4.5 起，诺基亚将为Qt增添新的开源LGPL授权选择。
　　关于如何正确的选择这三种授权协议，可以阅读[1]。
　　从2009年5月11日起，诺基亚Qt Software 宣布 Qt源代码库面向公众开放，Qt 开发人员可通过为 Qt 以及与Qt 相关的项目贡献代码、翻译、示例以及其他内容，协助引导和塑造 Qt未来的发展。为了便于这些内容的管理，Qt Software 启用了基于 Git 和 Gitorious 开源项目的Web 源代码管理系统[2]。
在推出开放式 Qt 代码库的同时，Qt Software 在其官方网站发布了其产品规划（Roadmap)。其中概述了研发项目中的最新功能，展现了现阶段对 Qt 未来发展方向的观点，以期鼓励社区提供反馈和贡献代码，共同引导和塑造 Qt 的未来。
**Qt 4.6****版本**
2009年12月1日，诺基亚发布了Qt 4.6——最新版的跨平台应用程序和用户界面框架。Qt 4.6 对全新平台提供支持，具有强大的全新图形处理能力并支持多点触摸和手势输入，让高级应用程序和设备的开发过程变得更加轻松和快乐。
　　更多平台
Qt 4.6 首次包含了对Symbian 平台的支持，并在其所支持的平台中新增了 Windows 7、Apple Mac OS 10.6 (雪豹) 和即将上市的Maemo 6。此外还为实时操作系统QNX 和 VxWorks提供了网络社区支持。对Maemo 5 的支持目前也已进入研发阶段，其技术预览版第二版已于2009年12月1日发布。
Qt 对 Symbian 和 Maemo 的支持首次为开发人员创造了使用同一代码库同时针对这两个平台进行开发的机会。能够跨平台分享代码就意味着开发人员所开发的应用程序上市的速度将更快，影响的设备用户群将更广。
Qt Mobility项目下的全新Qt API技术预览版也于今日同时发布。这些跨平台的API为开发人员提供了多种功能，如导航类设备的定位功能，以及信息发送和接收、联系人和网络连接管理等。
　　外观更炫
Qt 4.6是在4.5图形增强功能的基础上创建的，进一步强化了这些图形功能，添加了全新的动画框架（Animation Framework）——包括直观的状态机功能——以及全新的高级图形功能，如不透明、阴影、闪光和滤镜等。
　　使用手指
Qt4.6最令人兴奋的新功能之一就是可以创建多点触摸应用程序。这种全新的基于多触点和手势的输入法（如轻触和动力滚动）使Qt4.6框架的功能更为强大，用户可以通过触觉以动态的方式与应用程序和设备进行互动。
　　性能更强
　　传统软件开发观点认为高级用户界面功能的获得通常是以性能严重受损作为代价的。Qt 4.6 对此观点提出了挑战，为此投入大量工作，既要获得最佳性能，又要能让开发人员所开发的产品达到用户所要求的“令人惊叹”的效果。
　　性能提升主要包括：一个经过高度优化、重新编写的 Qt GraphicsView 渲染算法、一个全新的 OpenGL 绘图引擎、WebKit、使用 OpenVG 对 2D 矢量图形的支持，以及对全新 DirectFB 的支持。
　　最新工具
　　在发布 Qt 4.6 的同时，作为 Qt 开发跨平台 IDE 的 Qt Creator 也发布了更新版本。Qt Creator 1.3 和 Qt 4.6共同构成的 Qt SDK，包含了开发跨平台应用程序所需的全部功能。
**Qt Creator**
　　开发人员目前已经可以下载Qt Creator的1.3版本，这是一个用于Qt开发的轻量级跨平台集成开发环境。Qt Creator可带来两大关键益处：提供首个专为支持跨平台开发而设计的集成开发环境 (IDE)，并确保首次接触Qt框架的开发人员能迅速上手和操作。
Qt Creator包含了一套用于创建和测试基于Qt应用程序的高效工具，包括：
　　一个高级的C++代码编辑器上下文感知帮助系统可视化调试器源代码管理项目和构建管理工具
Qt Creator在LGPL 2.1版本授权下有效，并且接受代码贡献。目前的Qt Creator只支持桌面平台（Windows、Linux、Mac操作系统）的Qt开发，但随后数月就能支持嵌入式平台的开发。
**Qt****软件开发工具包**** (Qt SDK)**
　　全新的Qt软件开发工具包，可从www.qtsoftware.com下载。这个综合套装的设计主旨，是为开发人员提供从一个易于安装的二进制文件包入手，开始跨平台Qt开发这一过程中所需的一切。
Qt SDK包括了Qt库、Qt Creator IDE和Qt工具，这些都集成在一个易于安装的文件包里。
Qt SDK也同样也支持Linux/X11, Windows, Mac三种主流桌面操作系统。
**Qt****的优势**
Qt 同 X Window 上的 Motif，Openwin，GTK 等图形界面库和 Windows 平台上的 MFC，OWL，VCL，ATL 是同类型的东西，但是 Qt 具有下列优点:
　　优良的跨平台特性: 
Qt支持下列操作系统: Microsoft Windows 95/98， Microsoft Windows NT， Linux，Solaris， SunOS， HP-UX， Digital UNIX (OSF/1， Tru64)， Irix， FreeBSD，BSD/OS， SCO， AIX， OS390，QNX 等等。
　　面向对象
Qt 的良好封装机制使得 Qt 的模块化程度非常高，可重用性较好，对于用户开发来说是非常方便的。 Qt 提供了一种称为 signals/slots 的安全类型来替代 callback，这使得各个元件之间的协同工作变得十分简单。
　　丰富的 API 
Qt包括多达 250 个以上的 C++ 类，还提供基于模板的 collections， serialization， file， I/Odevice， directory management， date/time 类。甚至还包括正则表达式的处理功能。
　　支持 2D/3D 图形渲染，支持 OpenGL
　　大量的开发文档
XML 支持
Webkit 引擎的集成，可以实现本地界面与Web内容的无缝集成
　　但是真正使得 Qt 在自由软件界的众多 Widgets (如 Lesstif，Gtk，EZWGL，Xforms，fltk 等等)中脱颖而出的还是基于 Qt 的重量级软件 KDE 。
**Qt ****的资源**
Qt官方主页（英文）
http://www.qtsoftware.com/
Qt中文网论坛（Qt中文网论坛）（简体中文）
http://www.qtcn.org/
参考资料：
1.Qt GPL, LGPL & Commercial License
[阅读](http://www.qteverywhere.com/archives/157)
2.Qt 源代码库
[qt git](http://qt.gitorious.org/)
扩展阅读：
1.如需了解最新的Qt信息，请访问Qt中文博客：[www.qteverywhere.com](http://www.qteverywhere.com/)
