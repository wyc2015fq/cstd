# Qt核心知识归类及相关资料 - fanyun的博客 - CSDN博客
2016年09月30日 08:58:20[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：3150
### Qt是什么 ?
[Qt](http://dev.10086.cn/cmdn/wiki/index.php?doc-view-1975)是综合性跨平台的C++应用开发框架，它包括：一个存有400多种图形化用户界面的宏大C++类库、数据库、内核、XML、网络功能、OpenGL、多线程及众多用于以下目的的高级跨平台开发工具：
• GUI版面和表单设计
• 国际化
• 文档
### Qt库：
Qt 类库拥有构建强健高端应用所需的全部函数
被分为几个模块，Qt的直属API包括：
核心类
GUI类
SQL数据库类
XML类
网络类
OpenGL® 3D图像类
其它还有更多
下面介绍常用的类：
QApplication 应用程序类              //管理图形用户界面应用程序的控制流和主要设置
QLabel 标签类                                //提供文本或者图像的显示
QPushButton 按钮类                     // 提供了命令按钮 按钮的一种
QButtonGroup 按钮组合类           // 按钮组 相关按钮的组合
QGroupBox 群组类                        // 一个有标题的组合框
QDateTimeEdit 日期时间编辑框类  
QLineEdit 行编辑框类                   // 单行文本编辑器
QTextEdit 文本编辑框类                 // 单页面多信息编辑器对象
QComboBox 组合框类
QProgressBar 进度条类
QLCDNumber 数字显示框类
QScrollBar 滚动条类
QSpinBox 微调框类
QSlider 滑动条类
QIconView 图标视图类
QListView 列表视图类
QListBox 列表框类
QTable 表格类
QValidator 有效性检查类
QImage 图像类
QMainWindow 主窗口类
QPopupMenu 弹出性菜单类
QMenuBar 菜单栏类
QToolButton 工具按钮类
QToolTip 提示类
QWhatsThis 这是什么类
QAction 动作类
QHBoxLayout 水平布局类
QVBoxLayout 垂直布局类
QGridLayout 表格布局类
QT对话框类
QMessageBox 消息对话框类
QProgressDialog 进度条对话框类
QWizard 向导对话框类
QFileDialog 文件对话框类
QColorDialog 颜色对话框类
QFontDialog 字体对话框类
QPrintDialog 打印对话框类
要系统学习QT 还需要看看QT的slot系统,QT库类接口等
### Qt开发工具：
Qt Creator • 用于Qt开发的轻量级跨平台集成开发环境
Qt Designer •强大的拖曳式图形化用户界面排版和设计工具
Qt Linguist • 一整套工具，支持对Qt应用作快捷无误的翻译
Qt Assistant • 可定制可重发布的帮助文件和文档阅读器
qmake • 跨平台makefile生成器
Qt Creator
Qt Creator在所有开发工具中最年轻，但其性能却令Qt开发伙伴们爱不释手。你可以在Qt Creator产品专页中看到这方面的信息。
Qt Designer
Qt Designer是强大的拖曳式图形化用户界面排版和表单构建器
功能 支持表单和对话框的创建，可即时预览
与Qt版面系统集成
宏大的标准widgets集
支持客户定制的widgets和对话框
与Microsoft® Visual Studio .NET无缝集成
优势 大大加快了界面的设计过程
支持所有平台上的本地外观感觉
开发者能在自行选择的工作环境内充分发挥其现有技能
![（图）Qt一览(Qt for Symbian)](http://dev.10086.cn/cmdn/wiki/uploads/201002/s_1267062962nQVfNqjQ.jpg)
Qt Linguist
Qt Linguist是一组能理顺国际化工作流的工具。
![（图）Qt一览(Qt for Symbian)](http://dev.10086.cn/cmdn/wiki/uploads/201002/s_1267062999rh1pZJmX.jpg)
功能 采集所有的用户界面文本并以一个简洁的窗口将其展现给人工译者
支持所有语言
从单一应用的二进制程序内部提供同时多语言支持及同时多写入系统
优势 大大加快了翻译/本地化进程
与Qt的语言敏感排版引擎协同，以创建与语言不相关的简洁一致的界面
轻松应对国际市场
Qt Assistant
Qt Assistant是完全可定制、可重新发行的帮助文件/文档浏览器
![（图）Qt一览(Qt for Symbian)](http://dev.10086.cn/cmdn/wiki/uploads/201002/s_1267063136GhA3qPB5.jpg)
功能 简单明快的[web](http://dev.10086.cn/cmdn/wiki/index.php?doc-view-1500)浏览器般导航、书签和文档文件连接
支持富文本HTML
全文本关键词查阅
可定制并随Qt供应
优势 无需再从头开始构建帮助系统
充分利用现有的HTML技能
以方便搜寻和导航的格式向最终用户提供文档
qmake
跨平台应用build工具
功能 读取工程源码，生成依赖关系树，生成平台相关工程和makefiles
与Visual Studio及Xcode集成
优势 无需担忧跨平台编译
降低对makefile手工构建的需求度
### Qt术语：
Widgets：[UI](http://dev.10086.cn/cmdn/wiki/index.php?doc-view-1788)组件（按钮、消息框、应用视窗）
版面管理器：对各种子widgets的自动定位及自动改变大小
信号与槽：对象间通信
事件：系统事件（鼠标点击、键盘，等等）
动作：如，在工具条和菜单中的保存动作
### Qt架构：
Qt使用本地样式绘制用户界面
精准模拟外观感觉的Widgets也可被开发者改编
构建于平台的底层APIs之上
薄封装的MFC, Motif, Layered工具包。较低的性能表现，较低的灵活性
跨平台
面向多平台的单一源码，仅需重新编译
### Qt图形化架构：
![（图）Qt一览(Qt for Symbian)](http://dev.10086.cn/cmdn/wiki/uploads/201002/s_1267063202UoOH9ZES.jpg)
### Qt模块：
模块    说明 
QtCore  供其他模块使用的非图形化核心类 
QtGui  图形化用户界面(GUI)组件 
QtNetwork 用于网络编程的类 
QtOpenGL OpenGL支持类 
QtScript 用于评估Qt脚本的类 
QtSql 使用SQL对数据库进行整合的类 
QtSvg 用于显示SVG文件内容的类 
QtWebKit 用于显示和编辑web内容的类 
QtXml 用于处理XML的类 
QtXmlPatterns 用于XML和定制数据模型的XQuery及 XPath引擎 
Phonon 多媒体框架类 
Qt3Support Qt 3兼容类 
QtDesigner 用于扩展Qt Designer的类 
QtUiTools 运行时从 ui-files创建用户界面 
QtHelp 用于在线帮助的类 
QtAssistant 在线帮助支持 
QtTest 用于单元测试的工具类 
### Qt的优点：
一套源码面向多个平台
开发时间少– 进入市场早
降低维护开销
避免开发集体中的操作系统小集团
真正的平*立性
用几周时间就能应付一个新平台，而非几月
快速响应不断变化的市场需求
与平台变化隔绝
Qt得到了积极的维护和开发，以支持所有最新的主流OS款型
将开发力量集中于增值创新
Qt能提供真正持久的竞争优势
Qt使C++编程加快了，容易了，而且更直观了，因而提高了开发者的效率
Qt部署工具减少了开发过程中通常具有的瓶颈：
图形化用户界面设计和布局- Qt Designer
翻译/本地化- Qt Linguist
文档编制- Qt Assistant
跨平台build系统– qmake
Qt提供了真正意义上的平台自由度– 应付一个新平台按日或周计算，而不是按月或年计算一套代码基意味着更少的维护时间和开销 – 成倍提高开发产出.
Qt官网信息及软件下载：
Qt官网：[https://www.qt.io](https://www.qt.io/)
Qt下载：[http://www.qt.io/download](http://www.qt.io/download)
Qt所有下载：[http://download.qt.io/archive/qt](http://download.qt.io/archive/qt)
Qt官方发布下载：[http://download.qt.io/official_releases/qt](http://download.qt.io/official_releases/qt)
Open Source下载：[http://www.qt.io/download-open-source/#section-2](http://www.qt.io/download-open-source/#section-2)
Qt WiKi：[https://wiki.qt.io/Main_Page](https://wiki.qt.io/Main_Page)
 编码风格推荐：
Qt Coding Style：
[http://wiki.qt.io/Qt_Coding_Style](http://wiki.qt.io/Qt_Coding_Style)（low-level）
[http://wiki.qt.io/Coding_Conventions](http://wiki.qt.io/Coding_Conventions)（higher-level ）
Google开源项目风格指南 ：
[http://zh-google-styleguide.readthedocs.io/en/latest](http://zh-google-styleguide.readthedocs.io/en/latest)
里面包含五份（C++ 、Objective-C、[Python](http://lib.csdn.net/base/11) 、JSON、Shell ）中文版的风格指南。
C coding style guidelines ：
[http://www.quinapalus.com/coding.html](http://www.quinapalus.com/coding.html)
C++ Coding Standard ：
[http://www.possibility.com/Cpp/CppCodingStandard.html](http://www.possibility.com/Cpp/CppCodingStandard.html)
GitHub & Third-Party
 QtProject ：
[https://github.com/qtproject](https://github.com/qtproject)
各种强大的Qt项目及丰富的Qt库。
Awesome Qt ：
一系列强大的C/C++框架、库、资源和其它好东西。
[https://insideqt.github.io/awesome-qt](https://insideqt.github.io/awesome-qt)
[https://github.com/fffaraz/awesome-cpp](https://github.com/fffaraz/awesome-cpp)
[http://fffaraz.github.io/awesome-cpp](http://fffaraz.github.io/awesome-cpp)
inqlude ：
[https://inqlude.org](https://inqlude.org/)
Qt库存档 - 为Qt应用程序开发人员提供了所有现有的库。
free-programming-books-zh_CN ：
[https://github.com/justjavac/free-programming-books-zh_CN](https://github.com/justjavac/free-programming-books-zh_CN)
国外论坛：
Qt-Centre： 
[http://www.qtcentre.org](http://www.qtcentre.org/)
一个非常全面的外国网站，有forum、wiki、docs、blogs等。
Qt-forum ：
[http://www.qtforum.org](http://www.qtforum.org/)
比较有名的国际Qt编程论坛。
Qt-Prop ：
[http://qt-prop.org](http://qt-prop.org/)
可以找到很多专属的Qt应用。
Qt-Apps ：
[http://qt-apps.org](http://qt-apps.org/)
可以找到很多免费的Qt应用，获得源码来学习、研究，使用时请遵守相关开源协议。
KDE-Apps ：
[http://kde-apps.org](http://kde-apps.org/)
用过Linux/KDE的应该比较熟悉，里面有许多KDE-Desktop相关的应用。
Qt Software ：
[http://www.qtsoftware.com](http://www.qtsoftware.com/)
提供越来越多的第三方商业软件和开源软件的Qt用户社区。
QUIt Coding： 
[http://quitcoding.com](http://quitcoding.com/)
一群享受前沿技术开发的人才，Qt官方大使项目的成员。
国内论坛：
CSDN Qt论坛 ：
[http://bbs.csdn.net/forums/Qt](http://bbs.csdn.net/forums/Qt)
作为中国最大的IT社区和服务平台，CSDN也在持续的关注Qt的发展，Qt技术社区也已经上线很久了，可以在里面进行知识传播 - 提问、分享自己的一些学习心得、资料等。
QTCN开发网： 
[http://www.qtcn.org](http://www.qtcn.org/)
国内最早、最活跃的Qt中文社区，内容丰富、覆盖面广、在线人数众多，上面有很多热心、无私的Qt爱好者，他们会帮助初学者尽快的入门。
Qter开源社区： 
[http://www.qter.org](http://www.qter.org/)
致力于Qt普及工作！里面富含Qter们开发的实用开源项目和作品，以及一系列优秀的原创教程 - 图文并茂、简单易学。
** 1.相关学习资料下载：**
**[http://download.csdn.net/detail/fanyun_01/9637022](http://download.csdn.net/detail/fanyun_01/9637022)**
