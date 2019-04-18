# PyQt5教程——介绍（1） - YZXnuaa的博客 - CSDN博客
2018年04月27日 13:37:17[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：315
# PyQt5介绍
这是一份PyQt5的教程。教程的目的是让你开始学会使用PyQt5工具包。这个教程的内容在Linux上创建并测试。
## PyQt5相关
PyQt5是一套来自Digia的Qt5应用框架和Python的粘合剂。支持Python2.x和Python3.x版本。本教程使用Pyhton 3。Qt库是最强大的GUI支持库的一种。PyQt5的官方主页是[www.riverbankcomputing.co.uk/news](http://www.riverbankcomputing.co.uk/news)。是Riverbank Computing开发了PyQt5。
PyQt5以一套Python模块的形式来实现功能。它包含了超过620个类，600个方法和函数。它是一个多平台的工具套件，它可以运行在所有的主流操作系统中，包含Unix，Windows和Mac OS。PyQt5采用双重许可模式。开发者可以在GPL和社区授权之间选择。
PyQt5的类被划分在几个模块中，下面列出了这些模块：
- QtCore
- QtGui
- QtWidgets
- QtMultimedia
- QtBluetooth
- QtNetwork
- QtPositioning
- Enginio
- QtWebSockets
- QtWebKit
- QtWebKitWidgets
- QtXml
- QtSvg
- QtSql
- QtTest
`QtCore` 模块包含了非GUI的功能设计。这个模块被用来实现时间，文件和目录，不同数据类型，流，URL，mime类型，线程和进程。`QtGui` 模块包含的类用于窗口化的系统结构，事件处理，2D绘图，基本图形，字体和文本。`QtWidgets` 模块包含的类提供了一套UI元素来创建经典桌面风格用户界面。`QtMultimedia` 模块包含的类用于处理多媒体内容和链接摄像头和无线电功能的API。`QtBluetooth` 模块包含的类用于扫描蓝牙设备，并且和他们建立连接互动。`QtNetwork` 模块包含的类用于网络编程，这些类使TCP/IP和UDP客户端/服务端编程更加容易和轻便。`QtPositioning` 模块包含的类用于多种可获得资源的位置限定，包含卫星定位，Wi-Fi，或一个文本文件。`Enginio` 模块用于解决客户端访问Qt云服务托管。 `QtWebSockets` 模块包含的类用于解决WebSocket通信协议。 `QtWebKit` 包含的关于浏览器的类用于解决基于WebKit2的支持库。 `QtWebKitWidgets` 模块包含的关于WebKit1的类基本解决浏览器使用基于QtWidgets应用问题。 `QtXml` 模块包含的类用于解析XML文件。这个模块提供SAX和DOM API解决方法。 `QtSvg` 模块提供类用于显示SVG文件内容。Scalable Vector Graphics (SVG) 是一种语言，用XML来描述二维图形和图形应用程序。 `QtSql模块提供类驱动数据库工作。 `QtTest` 模块包含了方法提供PyQt5应用的单元测试。`
## PyQt4 和 PyQt5 的不同之处
The PyQt5 is not backward compatible with PyQt4; there are several significant changes in PyQt5. However, it is not very difficult to adjust older code to the new library. The differences are, among others, the following:
PyQt5不向后兼容PyQt4；这是一些在PyQt5中的重要改变。然而，将旧代码迁移到新的版本中并不是非常困难。不同点如下：
- Python 模块已经被改写. 一些模块被舍弃 (`QtScript`), 部分的模块被分割成子模块 (`QtGui`, `QtWebKit`).
- 新的模块被引进, 包含 `QtBluetooth`, `QtPositioning`, 和 `Enginio`.
- PyQt5 只支持最新风格的信号和槽的写法. SIGNAL()和SLOT()的调用将不会被长时间支持.
- PyQt5 不支持任何在Qt 5.0版本中弃用或取消的API.
