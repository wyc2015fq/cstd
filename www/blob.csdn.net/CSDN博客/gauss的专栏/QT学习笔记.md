# QT学习笔记 - gauss的专栏 - CSDN博客
2013年01月03日 18:04:52[gauss](https://me.csdn.net/mathlmx)阅读数：407
个人分类：[客户端编程](https://blog.csdn.net/mathlmx/article/category/944323)
                
 QT使创建独立于平台的数据库应用成为可能。
QT的内建驱动支持Oracle,Microsoft SQL Server,Sybase Adaptive Server,IBM DB2,PostgreSQL,MySQL,BorlandInterbase,SQLite,以及各种ODBC的数据库。QT的数据库功能完全集成到了QT设计器中，能提供数据库的生动的预览。QT包括专门的数据库组件，并且任何内建的或自定义的组件都可以数据相关；
QT普遍使用Unicode并且有良好的国际化支持。QT包括QT Linguist等工具可用来协助翻译。应用程序可以很容易地使用和混合使用用阿拉伯语，汉语，英语，希伯来语，日语，俄语以及其他Unicode所支持的语言所写成的文本；
QT包括许多专用领域的类。比如，QT拥有包括SAX、DOM语法分析器的XML模块。使用QT的STL兼容的集合类能够把对象储存在内存里面。QT的I/O类和网络类提供使用标准协议来处理本地和远程文件的能力
窗口部件是QWidget或其子类的实例，自定义的则通过子类化得来
使用QValidator及其子类可以约束或验证QComboBox，QLineEdit和QSpinBox。可以使用正则表达式来进行验证
在GUI应用程序中含有QMainWindow，QMenuBar和QToolBar。在对话框中含有
QMessageBox，QFileDialog，QWizard和其他对话框。布局中含有QSplitter，2D/3D图形中含有QCanvas和QGLWidget
![](https://p-blog.csdn.net/images/p_blog_csdn_net/kw123/qt1.JPG)
BankAccount x,y;
connect(&x,SIGNAL(balanceChanged(int)),&y,SLOT(setBalance(int)));
x.setBalance(2450);
当x的balance被设置为2450，balanceChanged()信号被发射。这个信号被y的setBalance
()槽接收，将y的balance也设置为2450
/*
如果一个Quit按钮的clicked()信号被连接到一个程序的quit()槽，用户就可以点击Quit来结束这个程序。代码可以这样写
**connect(button.SIGNAL(clicked()),qApp,SLOT(quit()));**
在QT程序执行期间，连接可以随时增加或撤消。
*/
菜单：PoP
每个菜单项有一个图标、一个复选框和一个快捷键。菜单项通常会响应动作（比如保存）。分隔符项目显示为一条线，用来可视化地分组相关的动作。
这是一个创建了New，Open，Exit菜单项的文件菜单的例子：
QPopupMenu *fileMenu=new QPopupMenu(this);
fileMenu->insertItem("&New",this,SLOT(newFile()),CTRL+Key_N);
fileMenu->insertItem("&Open...",this,SLOT(open()),CTRL+Key_O);
fileMenu->insertSeparator();
fileMenu->insertItem("E&xit",qApp,SLOT(quit()),CTRL+Key_Q);
MenuBar:
QmenuBar实现了一个菜单栏。它自动布局在其父部件（如一个QMainWindow）的顶端，如果
父窗口不够宽就会自动地分割而跨越多行。QT内置的布局管理器会考虑各种菜单栏。在Macintosh上面，菜单栏会如期地出现在屏幕的顶部。
这里展示了怎样创建一个含有File，Edit和Help菜单的菜单栏：
QMenuBar *bar=new QMenuBar(this);
bar->insertItem("&File",fileMenu);
bar->insertItem("&Edit",editMenu);
bar->insertItem("&Help",helpMenu);
QT的菜单系统非常灵活。菜单项能够动态地启用，禁用，增加或删除。通过子类化
QCustomMenuItem，可以创建自定义外观和行为菜单项。
下面的代码实现了一个“Save”菜单项，一个“Save”工具栏按钮和一个“Save”快捷键，并且均有气球式帮助：
QAction *saveAct=new QAction("Save",saveIcon,"&Save",CTRL+Key_S,this);
connect(saveAct,SIGNAL(activated()),this,SLOT(save()));
saveAct->setWhatsThis("Saves the current file.");
saveAct->addTo(fileMenu);
saveAct->addTo(toolbar);
设置:
使用QSettings类可以很容易地将用户设置和其他程序设置保存到磁盘上。在Windows中，QSettings利用系统注册表保存设置；在其他平台上，设置以文本文件形式保存。
可以利用一个键来保存设置。例如，/SoftwareInc/Zoomer/RecentFiles键可能包括一个最近使用文件的列表。布尔值，数字，Unicode字符串以及Unicode字符串列表都可以保存
多线程：
GUI程序通常使用多线程：一个线程保持用户界面的响应，其他一个或多个线程做着耗时的活动比如读取大文件或进行复杂的运算。QT可以配置为支持多线程并且提供了六个线程相关类：QThread,,QThreadStorage,QMutex,QMutexLocker,QSemaphore和QWaitCondition
2D
1.图象
Qimage类提供了几种格式的图象的输入、输出和处理，包括BMP、GIF、JPEG、MNG、
PNG、PNM、XBM和XPM
2.图标按钮 
许多QT内建的窗口部件能够显示图象，比如按钮、标签、菜单项等。这里是怎样在按钮上显示一个图标的方法：
QPushButton *button=new QPushButton("&Find Address",parent);
button->setIconSet(QIconSet(QImage("find.bmp")));
![](https://p-blog.csdn.net/images/p_blog_csdn_net/kw123/qt2.JPG)
QImage支持1位，8位和32位色深的图象。程序员可以处理象素和调色板数据，应用转换(比如旋转与裁剪)，如果需要的话也可降低色深。为了达到某些目的(比如透明度和alpha混合)，程序可以在QImage中将“alpha通道”和颜色数据保存在一起
绘画
QPainter为绘图部件提供了一个独立于平台的API。它提供各种原始的或高级的功能比如转换和剪切。所有QT内建部件使用QPainter绘制它们自身。程序员在实现自定义部件的时候也常常使用
QPainter。
Qpainter提供了一些标准函数来绘制点，直线，多边形，椭圆，弧线和贝塞尔曲线等。下面的命令绘制了一个120×60的矩形，左上角位于(25，15)，有着2个象素宽的红色虚线轮廓：
painter.setPen(QPen(red,2,DashLine));
painter.drawRect(25,15,120,60);
QPainter支持使用矩形，多边形，椭圆和位图组成的区域的剪切。复杂区域的产生可以通过相并，相交，相减和简单区域的异或产生。剪切可以用来减小重绘时的抖动。
QColor类存储了一种用RGB/HSV三基色或一个名字(如“skyblue”)表示的颜色。QT程序员能
够描述任意一个24位颜色；QT从系统调色板中自动地为程序分配所要求的颜色，在颜色受限的显示器上则为相似的颜色。
绘图设备
Qpainter可以在任意“绘图设备”上操作。在任何被支持的设备上绘画所需的代码是相同的，设备无关的。QT支持下列绘画设备：
* 一个QPixmap本质上是个“屏幕外的部件”。图形可以先画在一个QPixmap上，然后块传送到Qwidget以减小抖动。这种技术就叫“双缓冲”。
* 一个QPicture是一个可以完美地缩放，旋转和裁剪的矢量图。QPictrue类存储的图象是一系列绘图命令而不是一些象素的数据。它支持SVG(W3C's Scalable Vector Graphics)XML格式的输入与输出。
* 一个QPrinter表现为一台物理打印机。在Windows中，绘画命令被送到Windows打印引擎来使用已安装的驱动。在Unix中，输出PostScript并送往打印守护进程。
* 一个QWidget也是一个绘画设备，如同前面见过的条形图。
画布
QCanvas提供一个2D图形的高级接口。它能够处理大量的画布项目来描述直线，矩形，椭圆，文本，位图及动画精灵等。画布项目很容易做成交互式的。
画布项目是QCanvasItem子类的实例。它们比窗口部件轻巧得多，能很快的移动，隐藏和显示。QCanvas有效地支持冲突检测，还能罗列出指定区域中的所有画布项目。QCanvasItem可以被子类化，用以提供自定义的项目类型或扩充已有类型的功能。
Qcanvas对象使用QCanvasView类渲染。许多QCanvasView对象可以显示同一个QCanvas，但转化，缩放，旋转和裁剪各不相同
3D图形
OpenGL是一个用来渲染3D图形的标准API。QT开发者可以使用OpenGL在他们的程序中绘制3D图形。这写操作是通过子类化QGLWidget－－QWidget的一个子类来完成的，使用标准的OpenGL函数而不是QPainter。Qt的OpenGL模块在Windows、X11和Macintosh上是可用的，使用的是系统的OpenGL库(或Mesa)。Qt开发者能够设置一个OpenGL渲染的显示格式：单缓冲还是双缓冲、缓冲深度、RGBA模式还是颜色索引模式、alpha通道、装饰层等。在颜色索引模式下，他们还可以手动设置颜色映射表。当使用QT时，开发者是在使用纯粹的OpenGL。QT提供两个方便的函数，qglClearColor()和qglColor()，能接受一个QColor参数并在任意模式下工作。
数据库
QT的SQL模块简化了各种平台上GUI数据库程序的创建。程序员可以很容易地执行SQL语句，使用专用的数据库部件，或者创建数据相关的部件。Qt的SQL模块提供了一个访问数据库的跨平台接口。QT包括一些驱动，可以支持Oracle、Microsoft SQL Server、Sybase Adaptive Server、IBM DB2、PostgreSQL、MySQL、Borland Interbase、SQLite和ODBC。这些驱动可以在所有QT所支持的平台上工作并且库都是可用的。程序能够同时使用多种驱动访问多种数据库。
程序员可以很容易地执行SQL语句。QT也提供一个程序员可以用来自动产生合适的SQL语句的高级C＋＋接口。
任何QT部件(预定义的或自定义的)都可以做成数据相关的。QT也包括一些便利的数据库专用部件，使得以窗体或表格形式显示数据记录的对话框或窗口创建起来更加简单。数据相关的部件自动地支持浏览，更新和删除记录。大多数数据库设计要求新纪录具有唯一的QT猜测不到的关键字，所以插入操作通常需要书写一小段代码。程序员很容易就可以迫使用户确认一些操作，比如删除。QT的SQL模块完全集成到了QT设计器中，提供了一些模板和向导使得数据库的创建尽可能地快。向导能够使用导航按钮来创建和窗体，并且带有更新，插入和删除按钮。基于QT SQL模块的便利性，创建能够使用外键查看，能显示概要－细节关系并且支持数据挖掘的数据库程序变得非常简捷。
数据相关的窗口部件
QDataTable是一个显示QSqlCursor结果集记录的QTable。和QTable一样，QDataTable支持在适当的地方编辑。通过设置QDataTable的确认属性，程序员可以迫使用户确认所有或已选择的改变(比如删除)。为每种数据类型选择编辑器时要依赖于具体的数据类型。例如，QLineEdit适用于CHAR域，而QSpinBox适用于INTEGER域。通过对基本表创建一个属性映射，程序员可以覆盖其默认属性，使其可以与程序员喜欢的编辑器部件类型的域(列)相配。
不必书写任何代码就可以更新或删除记录。插入操作需要一些代码是由于大多数数据库设计要求将要创建的新纪录有唯一的码。通过在一个与QDataTable::beforeInsert()信号相连的槽中生成码，可以很容易完成这件事。
风格与主题
QT自动使用本机的风格作为程序外观。QT程序会尊重用户在颜色、字体和声音等方面的喜好。而QT程序员可以自由地使用各种提供的风格，也可以覆盖任意首选项。使用QT强大的风格引擎程序员可以修改已有的风格或实现自己的风格。
一种风格实现了一个特定平台上用户界面的外观。一种风格是一个实现了诸如“绘制框架”、“绘制按钮”等函数的QStyle子类。QT以最大的速度和灵活性来完成窗口部件本身的绘制
布局
布局提供了一个强大而灵活的选择来使用固定的位置和大小。布局把程序员从程序执行的大小和位置的计算中解放出来，并且提供了自动调整的能力以适合用户的屏幕、语言和字体。
QT提供了布局管理器来组织父部件区域中的子部件。它们自动调整子部件的大小和位置，最小大小以及顶级部件的尺寸，并且在内容或字体改变时重新定位。QT通过布局管理器对部件布局进行了优化。
内建布局管理器
QT的内建布局管理器有QHBoxLayout,QVBoxLayout和QGridLayout。
QHBoxLayout将其管理的部件组织在一个从左到右的水平行上。
QVBoxLayout将其管理的部件组织在一个从上到下的垂直列上。
QGridLayout将其管理的部件组织在一些网格单元中；不见可以横跨多个单元。
自定义布局
通过子类化QLayout开发者可以定义自己的布局管理器。和QT一起提供的customlayout样例展示了三个自定义布局管理器：BorderLayout、CardLayout和SimpleFlow，程序员可以使用并修改它们。
QT还包括QSplitter，一个最终用户可以操纵的分离器。某些情况下，QSplitter可能比布局管理器更为可取。
为了完全的控制，通过重新实现每个子部件的QWidget::resizeEvent()并调用QWidget::setGeometry()，就可以在一个部件中手动实现布局
事件
应用程序对象以QT事件的方式接收系统消息。应用程序能够监视、过滤并响应不同颗粒度的事件。在QT中，一个事件是一个继承QEvent的对象。时间被传递到QObject对象以便响应它们。程序员能够在应用程序水平和对象水平上监视、过滤事件
事件的产生
多数事件是窗口系统和通知部件产生的，例如，一个键被按下，一个鼠标按钮被点击，或者程序窗口被改变大小。可以计划性地向对象发送模拟事件。存在超过50种的事件，最常用到的有MouseButtonPress、MouseButtonRelease、MouseButtonDblClick、Wheel、 KeyPress、KeyRelease、Paint、Resize和Close。开发者可以增加与内建类型行为类似的事件类型。
只知道一个键被按下或一个鼠标按钮被释放往往是不够的。接收者还必须知道诸如哪个键被按下，哪个鼠标按钮被释放，鼠标的位置在哪里等信息。这些附加信息在QEvent的子类中是可用的，比如QMouseEvent、QKeyEvent、QPaintEvent、QResizeEvent和QCloseEvent
事件的传递
QT通过调用虚函数QObject::event()传递事件。为求方便，QWidget::event()把一些最常见的事件传递给了精巧的处理器，比如QWidget::mouseReleaseEvent()和QWidget::keyPressEvent()。在写自己的部件或特殊化已有的部件时，开发者可以很容易重新实现这些处理器
文件输入/输出
QT为多种平台提供了类来完成高级的输入/输出。QT的QTextStream类有和标准类相似的接口，并且支持QTextCodec提供的编码。QDataStream类用于将基础C＋＋类型和许多QT的类型串行化微独立于平台的二进制格式。
XML
QT的XML模块提供了SAX和DOM解析器，它们都可以读取合乎语法的XML并且是无验证的。
SAX(Simple API for XML)的实现模仿了SAX2 Java实现的设计并具有相适应的命名规则。第二级DOM(Document Object Model)的实现遵照了W3C的建议并且支持命名空间。
许多QT程序使用XML格式来保存它们的永久数据。SAX解析器用来增量式地读取数据，对于那
些解析需求较低或者包含在超大型文件中的应用程序非常合适。DOM解析器读取整个文件并在内存中生成一棵可随意遍历树
进程间通信
QProcess用来启动一个外部程序并且用一种平台无关的方式来与它们通信。通信是通过写入外部程序的标准输入和读取它们的标准输出或标准错误来完成的。
QProcess以异步方式工作，以发射信号的方式来报告数据的可用性。QT程序连接到这些信号来取回并处理数据，也可以发送数据到这些外部程序来表示响应
网络
QT提供了多种平台的接口来编写TCP/IP客户端和服务器，支持IPv4和IPv6。
QSocket类提供了一个异步带缓冲的TCP连接。QSocket是一个QIODevice，使得在一个套接
字上面使用QTextStream和QDataStream变得非常容易
集合类
集合类用来在内存中存储项目组。QT提供了一系列与标准模板库(STL)兼容的类，无论编译器是否支持STL都可以工作。
应用程序常常需要管理内存中的项目，例如成组的图象、部件或自定义的部件。许多编译器支持STL，STL提供了现成的数据结构来存储项目。QT提供了线性表、栈、队列和STL语法的字典。QT的集合类能与STL和非STL编译器协同使用。
QT丰富而方便的集合类(“容器”)和相关的迭代器在内部被大量使用，并且成为了QT API的一部分。QT的容器使用“私有类”和“隐含共享”两种技术来优化速度和内存效率。程序员也可以在支持的平台上使用STL容器，代价是是去了QT的优化。
模板类常常会显著增加可执行文件的大小，因为编译器会为各种特殊类型生成相同的代码。QT的模板集合类减少了代码的膨胀，因为它们只是非模板私有类上的一个薄层
插件和动态链接库
QT能够访问动态链接库中与平台无关的函数。QT还支持插件，插件允许开发者以独立组件方式创建、分发编解码器、数据库驱动、图象格式转换器、风格和自定义窗口部件
1．插件
将QT便解码器、图象格式转换器、风格或者自定义窗口部件转换为插件是通过子类化合适的插件基类，实现一些函数并增加一个宏来完成的
2．动态链接库
QLibrary类提供了各种平台的动态库装入，这是一种比拘束得多的构建时连接强大得多的机制
            
