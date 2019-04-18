# QT介绍 - 深之JohnChen的专栏 - CSDN博客

2019年03月21日 23:13:39[byxdaz](https://me.csdn.net/byxdaz)阅读数：370
个人分类：[QT](https://blog.csdn.net/byxdaz/article/category/8858602)



一、Qt介绍

Qt，坦白来说，并不只是一个界面库，他是C++编程思想的集大成者。它是得到完善的C++应用程序框架。使用Qt，在一定程度上你获得的是一个“一站式”、“全方位”的解决方案，STL。string、XML、数据库、网络这些零散的功能都包含在Qt中，并且得到了封装，一共开发者使用。

跨平台GUI通常有三种实现策略，分别是：

API 映射：API 映射是说，界面库使用同一套 API，将其映射到不同的底层平台上面。大体相当于将不同平台的 API 提取公共部分。比如说，将 Windows 平台上的按钮控件和 Mac OS 上的按钮组件都取名为 Button。当你使用 Button 时，如果在 Windows 平台上，则编译成按钮控件；如果在 Mac OS 上，则编译成按钮组件。这么做的好处是，所有组件都是原始平台自有的，外观和原生平台一致；缺点是，编写库代码的时候需要大量工作用于适配不同平台，并且，只能提取相同部分的 API。比如 Mac OS 的文本框自带拼写检测，但是 Windows 上面没有，则不能提供该功能。这种策略的典型代表是 wxWidgets。这也是一个标准的 C++ 库，和 Qt 一样庞大。它的语法看上去和 MFC 类似，有大量的宏。据说，一个 MFC 程序员可以很容易的转换到 wxWidgets 上面来。

API 模拟：前面提到，API 映射会“缺失”不同平台的特定功能，而 API 模拟则是解决这一问题。不同平台的有差异 API，将使用工具库自己的代码用于模拟出来。按照前面的例子，Mac OS 上的文本框有拼写检测，但是 Windows 的没有。那么，工具库自己提供一个拼写检测算法，让 Windows 的文本框也有相同的功能。API 模拟的典型代表是 wine ——一个 Linux 上面的 Windows 模拟器。它将大部分 Win32 API 在 Linux 上面模拟了出来，让 Linux 可以通过 wine 运行 Windows 程序。由此可以看出，API 模拟最大优点是，应用程序无需重新编译，即可运行到特定平台上。另外一个例子是微软提供的 DirectX，这个开发库将屏蔽掉不同显卡硬件所提供的具体功能。使用这个库，你无需担心硬件之间的差异，如果有的显卡没有提供该种功能，SDK 会使用软件的方式加以实现。

GUI 模拟：任何平台都提供了图形绘制函数，例如画点、画线、画面等。有些工具库利用这些基本函数，再不同绘制出自己的组件，这就是 GUI 模拟。GUI 模拟的工作量无疑是很大的，因为需要使用最基本的绘图函数将所有组件画出来；并且这种绘制很难保证和原生组件一模一样。但是，这一代价带来的优势是，可以很方便的修改组件的外观——只要修改组件绘制函数即可。很多跨平台的 GUI 库都是使用的这种策略，例如 gtk+（这是一个 C 语言的图形界面库。使用 C 语言很优雅地实现了面向对象程序设计。不过，这也同样带来了一个问题——使用大量的类型转换的宏来模拟多态，并且它的函数名一般都比较长，使用下划线分割单词，看上去和 Linux 如出一辙。gtk+ 并不是模拟的原生界面，而有它自己的风格，所以有时候就会和操作系统的界面格格不入。），Swing 以及我们的 Qt。

Qt 和 wxWidgets 一样，也是一个标准的 C++ 库。但是它的语法类似于 Java 的 Swing，十分清晰，而且使用信号槽（signal/slot）机制，让程序看起来很明白——这也是很多人优先选择 Qt 的一个很重要的原因。不过，所谓“成也萧何，败也萧何”。这种机制虽然很清楚，但是它所带来的后果是你需要使用 Qt 的 moc 对程序进行预处理，才能够再使用标准的 make 或者 nmake 进行正常的编译，并且信号槽的调用要比普通的函数调用慢大约一个数量级（Qt 4 文档中说明该数据，但 Qt 5 尚未有官方说明）。Qt 的界面也不是原生风格的，尽管 Qt 使用 style 机制十分巧妙地模拟了原生界面。另外值得一提的是，Qt 不仅仅能够运行在桌面环境中，还可以运行在嵌入式平台以及手机平台。

二、Qt安装与配置

## ******Qt的安装******

![](https://img-blog.csdnimg.cn/20190322171728458.png)

上面是三种Qt，第一种是使用Qt Creator+mingw编译和调试开发的Qt，第二种是使用Qt Creator+msvc调试和编译的Qt，第三种是使用vs+Qt插件。开始学习的建议安装第一种，自带gcc编译器和gdb调试器。安装过程就不说了，一直下一步，安装完之后的状态如下图。

![](https://img-blog.csdnimg.cn/20190322171749335.png)

## ******2.2Qt的项目创建******
- 
1、文件->新建文件或项目，弹出一个对话框，我们选择Application项目中的Qt Widgets Application，如下图所示：

![图解Qt Creator创建新项目](https://imgsa.baidu.com/exp/w=500/sign=73f86c8aeacd7b89e96c3a833f254291/b21bb051f8198618918583c446ed2e738ad4e6b4.jpg)

- 
2、输入项目名称：helloworld，然后点击“浏览”按钮来选择源码存放路径，比如我这里是E:\codeTest\qtCreator_2，如果选中了“设为默认的项目路径”，那么今后创建的项目都将存放在这里。单击“下一步”，如下图所示：

注意：项目所在路径中不能有中文，否则程序运行报错，例如：C:\你好\qtCreator_2。

![图解Qt Creator创建新项目](https://imgsa.baidu.com/exp/w=500/sign=a8aec99f7ac6a7efb926a826cdfbafe9/a71ea8d3fd1f4134d8b5693e291f95cad0c85eeb.jpg)

- 
3、选择构建套件：DeskTop Qt 5.6.7.1 MinGw 32bit，点击详情按钮可以看到  默认为Debug版本和Release版本分别设置了不同的目录，Debug即为调试版本，Release为可发布版本。都保持默认即可，单击“下一步”，如下图所示：

![图解Qt Creator创建新项目](https://imgsa.baidu.com/exp/w=500/sign=53599a8df0039245a1b5e10fb795a4a8/4afbfbedab64034fe867ae46a3c379310b551dc3.jpg)

- 
4、自定义类名：设定类名为 HelloDialog，基类选择 QDialog ，单击”下一步“。这表明该类继承自 QDialog类，使用这个类可以生成一个对话框界面，Qt框架中所有的类都是以‘Q’开头的，如下图所示：

![图解Qt Creator创建新项目](https://imgsa.baidu.com/exp/w=500/sign=31f984b45dda81cb4ee683cd6267d0a4/42a98226cffc1e171615e0714690f603728de9cc.jpg)

- 
5、设置项目管理：在这里可以看到这个项目的汇总信息，还可以使用版本控制系统，这里不会涉及，直接单击”完成“即可，如下图所示：

![图解Qt Creator创建新项目](https://imgsa.baidu.com/exp/w=500/sign=c69ae541506034a829e2b881fb1249d9/9e3df8dcd100baa192ae2bfb4b10b912c9fc2eeb.jpg)

- 
6、右击项目文件列表中的任一文件，点击”在Explorer中打开“，就可以转到项目文件夹，让我们来看看项目目录中的各个文件说明，如下图所示：

![图解Qt Creator创建新项目](https://imgsa.baidu.com/exp/w=500/sign=728eb2b4bb4543a9f51bfacc2e168a7b/7af40ad162d9f2d396bb48afa5ec8a136227cccc.jpg)

- 
7、以上就是Qt Creator创建新项目的方法和步骤。

## ****** 2.3 Qt的配置******

这里说一下mingw版本的配置，此外msvc版本的Qt的编译器和调试器都是msvc

![](https://images2015.cnblogs.com/blog/666950/201511/666950-20151104184758508-839035436.png)

三、常见概念

****3********.1　信号和槽机制（Signal & Slot）****

  Qt提供了信号和槽机制用于完成界面操作的响应，是完成任意两个Ｑt对象之间的通信机制。其中，信号会在某个特定的情况或动作下被触发，槽是等同于接收并处理信号的函数。

　　每个Ｑt对象都包含若干个预定义的信号和若干个预定义的槽，当某一个特定的事件发生时，一个信号被发送，与信号相关的槽则会响应信号并完成相应的处理。当一个类被继承时，该类的信号和槽也同时被继承，也可以根据需要自定义信号和槽。

****１. 信号与槽的连接方式****

　　　　１）一个信号可以与另一个信号相连　　　　　　

connect(Object1,SIGNAL(signal1),Object2,SLOT(signal2));

　　　　　表示Object1的信号１可以触发Object2的信号2的发送。

　　　２）一个信号可以与多个槽相连　

connect(Object1,SIGNAL(signal1),Object2,SLOT(slot2));

connect(Object1,SIGNAL(signal1),Object3,SLOT(slot3));

3）一个槽可以响应多个信号

connect(Object1,SIGNAL(signal1),Object2,SLOT(slot2));

connect(Object3,SIGNAL(signal3),Object2,SLOT(slot2));

　　　常用的连接方式为

connect(Object1,SIGNAL(signal),Object2,SLOT(slot));

　　　　其中，signal为Object1的信号，slot为Object2的槽
SIGNAL()和SLOT()是Ｑt定义的两个宏，它们返回其参数的Ｃ语言风格的字符串(const char*)。如下所示，两者等同。

connect(Object1,SIGNAL(signal),Object2,SLOT(slot));

connect(Object1,“signal”,Object2,“slot”);
****２.信号与槽机制的优点****

****１）类型安全。****需要关联的信号和槽的签名必须是等同的，即信号的参数类型和参数个数与接收该信号的槽的参数类型和参数个数相同。

　　　　　　但是槽的参数个数可以少于信号的参数个数，但缺少的参数必须是信号参数的最后一个或几个参数。如信号和槽的参数签名不符，编译器就会报错。
****２）松散耦合。****信号与槽机制减弱了Ｑt对象的耦合度。

　　　　　　激发信号的Ｑt对象无须知道是哪个对象的槽接收它发出的信号，它只需在适当的时候发送适当的信号即可，它不需要关心它发出的信号有没有被接收到，以及哪个对象的哪个槽接收到该信号。

　　　　　　对象的槽也不需要知道哪些信号关联了自己，而一旦关联信号和槽，Ｑt就保证了适合的槽得到了调用。即使关联的对象在运行时被删除程序也不会崩溃。

　　　　注意：一个类若要支持信号和槽，就必须从QObject或QObject的子类继承。Ｑt的信号和槽机制不支持对模板的使用。
****３）信号与槽机制的效率****

　　　　　　信号与槽机制增强了对象间通信的灵活性，然而损失发一些性能。同回调函数相比，信号和槽机制运行速度有些慢。通常，传递一个信号来调用槽函数将会比直接调用非虚函数运行速度慢10倍。原因如下：

a. 需要定位接收信号的对象。

b. 安全地遍历所有的关联（如一个信号关联多个槽的情况）。

c. 编组（marshal）／解组（unmarshal）传递的参数。

d. 多线程的时候，信号可能需要排队等待。

　　　　　然而，与创建对象的new操作和删除对象的delete操作相比，信号和槽的运行代价只是它们很少的一部分。信号和槽机制导致的这点性能损耗，对实时应用程序是可以忽略的；同信号和槽提供的灵活性和简便性相比，这点性能损耗是值得的。

****3********.２　Qt5元对象系统****

　　Ｑt 元对象系统提供了对象间的通信机制（信号和槽）、运行时类型信息和动态属性系统的支持，是标准Ｃ＋＋的一个，它使Ｑt能够更好地实现ＧＵＩ图形用户界面编程。
Ｑt的元对象系统不支持Ｃ＋＋模板，尽管模板扩展了标准Ｃ＋＋的功能，但是元对象系统提供了模板无法提供的一些特性。
****Ｑt元对象系统基于以下三个事实。****
****１）基类QObject:**** 任何需要使用元对象系统功能的类必须继承自QObject。
****２）Q_OBJECT宏：****Q_OBJECT宏必须出现在类的私有声明区，用于启动元对象的特性。
****３）元对象编译器　（Ｍeta-Object Complier, moc):**** 为QObject子类实现元对象特性提供必要的代码实现。

****3********.3 布局管理器****

　　在设计较复杂的ＧＵＩ用户界面时，仅通过指定窗口部件的父子关系以期达到加载和排列窗口部件的方法是行不通的，最好的办法是****使用Ｑt提供的布局管理器。****

    QGridLayout *mainLayout = new QGridLayout(this); //(a)

    mainLayout->addWidget(label1,0,0);             //(b)

    mainLayout->addWidget(lineEdit,0,1);

    mainLayout->addWidget(label2,1,0);

    mainLayout->addWidget(button,1,1);    

    setLayout(mainLayout);     //(c)

　　其中：

(a) QGridLayout *mainLayout = new QGridLayout(this) : 创建一个网格布局管理器对象　mainLayout, 并且 this 指出父窗口。

(b) mainLayout->addWidget(...) : 分别将控件label1等放置在该布局管理器中，还可以在创建布局管理器对象时不必指明父窗口。

(c) QWidget::setLayout(...) : 将布局管理器添加到对应的窗口部件对象中。因为这里的主窗口就是父窗口，所以直接调用　setLayout(mainLayout)即可。

****发布Qt应用****

发布Qt 编写的软件需要以"release" 配置编译源代码，Qt Creator左下角的运行工具栏中可以更改配置，Vs在[project/项目]->[属性/properties]中进行配置。

将编译得到的二进制可执行文件与必要的支持库打包。

支持库由.pro文件中设置的模块决定，一般包含core和gui模块的Qt应用需要以下支持库文件：
- Qt库文件[Qt\lib]- Qt5Core.dll
- Qt5Gui.dll
- Qt5Widgets.dll

- Qt Platform插件[Qt\plugins\platforms]

复制文件夹并删除其中的.pdb文件，只保留.dll文件。

qwindows.dll】同名.dll文件中删除带有d后缀(debug模式所需)的.dll文件，如删除"qwindowsd.dll"，保留"qwindows.dll"。

Desktop平台下所需的文件：

- qoffscreen.dll

- qminimal.dll
- ICU库[Qt\bin]

ICU库用于提供对Unicode字符编码的支持

-  icudt53.dll

- icuin53.dll

- icuuc53.dll
- C++ Runtime Library- msvcp120.dll
- msvcr120.dll

- OpenGl支持库- libEGL.dll

- Windows平台相关- gpsvc.dll
- kernel32.dll

****Qt5如何添加信号和槽****

转载：[https://blog.csdn.net/xzq413520903/article/details/79280705](https://blog.csdn.net/xzq413520903/article/details/79280705)

信号和槽的创建方式有两种，这里暂且称他们为静态创建和动态创建。

 何为静态创建和动态创建呢？这里我们以按钮pushButton为例。假设我们已经创建好了Dialog窗口，

 然后我们在窗口上面放置按钮，如下图所示。此时我们可以看到在文件ui_dialog.h中会自动添加如下

![](https://img-blog.csdn.net/20180207153903464?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHpxNDEzNTIwOTAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

按键创建的代码。

![](https://img-blog.csdn.net/20180207154424242?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHpxNDEzNTIwOTAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

此时，我们只要在转到设计的ui界面上，将鼠标放置于pushButton上面，点击右键，选项“转到槽...”，

此时会出现如下窗口，若是选择按钮单击有效则选择click()，然后点击OK。此时将会自动生成如下函数并

跳转至该函数

![](https://img-blog.csdn.net/20180207154950635?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHpxNDEzNTIwOTAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

 void Dialog::on_MyButton_clicked()

{

//添加你所想要的操作

}

若是想要动态创建按钮，则不用先在窗口上面放置按钮控件，而是直接在代码中添加创建。如下所示，

我们现在Dialog.h文件中定义一个按钮指针以及一个槽，然后在Dialog.cpp初始化构造函数中添加按钮的

 创建、位置的设置以及槽和信号的设置。

![](https://img-blog.csdn.net/20180207155743413?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHpxNDEzNTIwOTAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20180207155818709?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHpxNDEzNTIwOTAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

最后再自己在Dialog.cpp文件中实现槽函数void on_MyButton_clicked()。

四、windows开Qt开发

方案一：Qt Creator

Qt Creator+MinGW 或者 Qt Creator+VC++ Compiler

方案二：QT4.8.5 for Windows(VS2010+VS插件)

1、必须确保电脑已安装VS2010。

2、安装Qt for vs2010的库文件，请运行qt-win-opensource-4.8.5-vs2010.exe。

QT默认的安装路径是C:\Qt\4.8.5，这里建议修改为C:\Qt\4.8.5_VS2010，此举是为了与其他QT版本库区分开。避免把已安装的其他QT版本覆盖并破坏掉。(注：支持windows环境的QT库有mingw，vs2010，vs2008等版本)

Windows7 QT5.11.2+VS 2015环境搭建

[https://blog.csdn.net/HW140701/article/details/82917808](https://blog.csdn.net/HW140701/article/details/82917808)

# **五、Linux Qt5 开发环境搭建**

[https://blog.csdn.net/u013934107/article/details/80712418](https://blog.csdn.net/u013934107/article/details/80712418)
- 实例

有两种创建图形化程序界面的方法，一、通过系统自动生成的.ui文件，二、通过代码实现。

　　方法一可通过鼠标拖拉布局界面，较方便，但在设计较复杂的界面时最好还是使用Ｑt提供的布局管理器。

****ＱＴ项目框架分析****

****6********.1**** 每一个工程都有一个执行的入口函数，此项目中的main.cpp中的 main()函数就是此工程的入口。

main.cpp

#include "dialog.h"

#include <QApplication>

int main(int argc, char *argv[])

{

    QApplication a(argc, argv);

    Dialog w;

    w.show();

return a.exec();

}

　　如上所示，其中：

　　１) #include "dialog.h"　包含了程序中要完成功能的Dialog类的定义，在Dialog类中封装完成所需要的功能。

　　　　注意：程序中使用哪个类，就要引用包含该类定义的头文件。如　dialog.h中使用QLabel、QLineEdit、QPushButton必须包含头文件

#include <QLabel> #include <QLineEdit> #include <QPushButton>

２) #include <QApplication>：Application类的定义。

 ３)  int main(int argc, char *argv[]):　应用程序的入口，几乎在所有使用Ｑt的情况下，main()函数只需要在将控制权交给Ｑt库之前执行初始化，然后Ｑt 库通过事件向程序告知用户的行为。所有的Ｑt程序中都　必须有且只有一个main()函数。

 ４) QApplication a(argc, argv): a 是这个程序的　QApplication对象。

**　　　　在每一个使用Ｑt图形化应用程序中都必须使用一个QApplication 对象。ＱApplication管理了各种和样的图形化应用程序的广泛资源、基本设置、控制流及事件处理等。**

在任何的窗口部件被使用之前必须创建QApplication 对象。它在这里被创建并且处理这些命令行变量。所有被Qt识别的命令行参数都将从argv中被移去（并且 argc 也因此而减少）。

　　５) w.show()：当创建一个窗口部件的时候，默认它是不可见的，必须调用show()函数使它变为可见。、

　　６) return a.exec()：程序进入消息循环，等待可能的输入进行响应。这里就是main()函数将控制权交给Qt, Ｑt完成事件处理工作，当应用程序退出的时候，exec()函数的值就会返回。在exec()函数中，Qt接收并处理用户和系统的事件并且将它们传递给适当的窗口部件。

****6********.2  dialog.h 头文件****

在类Dialog中的定义中，Q_OBJECT宏的作用是启动Ｑt元对象系统的一些特性（如支持信号和槽等），它必须放到类定义的私有区。

private slots:　表示下面的函数为槽函数。

dialog.h

#ifndef DIALOG_H

#define DIALOG_H

#include <QDialog>

#include <QLabel>

#include <QLineEdit>

#include <QPushButton>

#include <QGridLayout>

conststaticdouble PI = 3.1416;

class Dialog : public QDialog

{

Q_OBJECT

public:

    Dialog(QWidget *parent = 0);

    ~Dialog();

private:

    QLabel *label1, *label2;

    QLineEdit *lineEdit;

    QPushButton *button;

private slots:

void showArea();

};

#endif// DIALOG_H

****6********.3 dialog.cpp****

　　在该文件中初始化部件，创建布局管理器，将部件加到布局管理器中，并且连接

#include "dialog.h"

Dialog::Dialog(QWidget *parent)

    : QDialog(parent)

{
//初始化部件

    label1 = new QLabel(this);

    label1->setText(tr("请输入圆的半径："));

    lineEdit = new QLineEdit(this);

    label2 = new QLabel(this);

    button = new QPushButton(this);

    button->setText(tr("显示对应的圆的面积"));
//创建布局管理器，将部件加到布局管理器中

    QGridLayout *mainLayout = new QGridLayout(this);

    mainLayout->addWidget(label1,0,0);

    mainLayout->addWidget(lineEdit,0,1);

    mainLayout->addWidget(label2,1,0);

    mainLayout->addWidget(button,1,1);
// 连接信号与槽
//分别是 点击和文本内容改变信号

    connect(button,SIGNAL(clicked()),this,SLOT(showArea()));

    connect(lineEdit,SIGNAL(textChanged(QString)),this,SLOT(showArea()));

}

Dialog::~Dialog()

{

delete label1,label2,lineEdit,button;

}

//定义槽函数

void Dialog::showArea()

{

bool ok;

    QString tempStr;

    QString valueStr = lineEdit->text();

int valueInt = valueStr.toInt(&ok);

double area = valueInt * valueInt * PI;

    label2->setText(tempStr.setNum(area));

}

****6********.4**** 上面三部分为这个工程代码的分析，介绍了主程序中完成图形化界面程序所需的编写的代码，即窗口部件的初始化，以及窗口的使用。dialog.h dialog.cpp主要告诉如何自定义一个对话框Ｄialog类（继承自ＱDialog)即定义窗口部件，以及如何将窗口部件的信号与处理事件的槽函数进行绑定。

　　通过上面的分析我们可以创建一个简单的计算圆面积的图形应用程序。

# **七、QT调用MFC DLL**

方式一：隐式调用

int nSum = TestAddFunction(1, 2);

qDebug() << nSum;

测试成功！

注意：需要.dll文件、.lib文件、.h文件，

在.pro文件中添加lib库路径和文件

LIBS+= -L../***/***/***/lib -l***

-L后面跟的是lib库路径

-l后面跟的是lib文件名，不要后缀.lib

因为库函数是以C来编译和链接的，所以在包含头文件的时候注意要加extern "C"，以C语言来编译和链接。

extern "C"

{

#include "InterfaceFunction.h"

}

所有用到的dll放在exe同目录下。

方式二：显示调用

QLibrary lib("DUHardware_D.dll");

if (lib.load()) {

    qDebug() << "load ok!";

} else {

    qDebug() << "load failed!";

}

typedef int (*TestAddFunction)(int, int);

TestAddFunction fpAdd = (TestAddFunction)lib.resolve("TestAddFunction");

int nSum = fpAdd(1, 2);

qDebug() << nSum;

测试成功！

注意：只需要dll文件，如果dll用到了其他dll，所有dll都需要放在exe同目录下，不然lib.load()失败。

# **八、Qt中使用MFC**

[https://blog.csdn.net/sichuanpb/article/details/69375626](https://blog.csdn.net/sichuanpb/article/details/69375626)

开发环境：

win8 + [vs2008](https://www.baidu.com/s?wd=vs2008&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd) + qt-[windows](https://www.baidu.com/s?wd=windows&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)-opensource-4.8.0

****A：********需求：********在QT中添加MFC的应用;****

****过程：****

1：用vs创建QT的ui工程；

2：引入MFC头文件，这个可以直接在MFC工程中的stdafx.h中直接复制；

![](https://img-blog.csdn.net/20131112174118281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hpbmFiaW5sYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

3：修改一些工程配置；

![](https://img-blog.csdn.net/20131112174152406?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hpbmFiaW5sYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20131112174207750?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hpbmFiaW5sYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

应用示例:

![](https://img-blog.csdn.net/20131112174545578?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hpbmFiaW5sYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

方法2：上述方法虽然可行，但是设置麻烦，这里有个更好的设置方法；

通过[visualstudio](https://www.baidu.com/s?wd=visualstudio&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)创建Qt程序后，添加添加相关都文件，和上面一致；

然后直接在工程属性中设置“在共享DLL中应用MFC”；

这样就可以在在Qt工程中用MFC了；

![](https://img-blog.csdn.net/20141112152619200?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hpbmFiaW5sYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

****B：QT中添加windows函数库；****

如果不想用ＭＦＣ库，觉得添加方法有点麻烦，可以直接用windwos sdk编程，这个添加比较简单：

（1）： 添加 #include <Windows.h>，这样就可以在Qt中用windows sdk了；

（2）：在Qt中获取窗口句柄:

              Qt的Widget类提供了一个很方便的接口winId()，这个接口在windows系统上获取窗口句柄；

             如  在窗口上绘制一条直线：

        void slot3()
{
HWND hwnd = HWND(winId());
HDC hDC = ::GetDC(hwnd);

POINT pt;
MoveToEx(hDC, 0,0, &pt);
LineTo(hDC,1000,100);

::ReleaseDC(hwnd, hDC );

}

八、MFC调用Qt中的类

[https://blog.csdn.net/lmhuanying1012/article/details/76168295](https://blog.csdn.net/lmhuanying1012/article/details/76168295)

包含相关目录

在MFC工程的属性页中添加包含目录(include)及库目录(lib)

这里的包含目录是：E:\Qt\5.9.1\msvc2015\include

              库目录是：E:\Qt\5.9.1\msvc2015\lib


配置Qt环境

引用头文件

#include <QtWidgets\QApplication>

其中QtCore是包含目录(include)下面的目录。


添加全局代码或静态局部代码：

int iArgc = 0;

static QApplication qtApp(iArgc, NULL);

引用相关类头文件

在工程中，引用要使用类的相关头文件

#include <QtCore\QString>

其中QtCore是包含目录(include)下面的目录。

MFC 下调用Qt编写的含界面的dll

[https://blog.csdn.net/libin88211/article/details/38183791](https://blog.csdn.net/libin88211/article/details/38183791)

在VC下调用Qt编译的带界面的DLL。

首先强调一下环境：

操作系统：                     win8

VS   版本：                     VS2010

Qt    版本：                     qt-opensource-windows-x86-mingw482_opengl-5.3.0.exe

qtwinmingrate版本：   qt-solutions-ov3r1oads-qt-solutions 此版本较为新，2.8 在Qt5下用不了。

                                         下载地址：https://qt.gitorious.org/qt-solutions/ov3r1oads-qt-solutions/source/80592b0e7145fb876ea0e84a6e3dadfd5f7481b6:

参考：1，http://blog.csdn.net/normallife/article/details/5397980

            2， http://www.rebaoxian.com/28649.html

            3，http://qimo601.iteye.com/blog/1675575

参考十分有限而且粗略，不适合新手来做，没有例子，没有详细的代码。且所用Qt版本老旧



我的工作

1 首先安装好Qt5.3， 这个不用多说。

2 下载下来qt-solutions-ov3r1oads-qt-solutions 解压后进入D:\qt-solutions-ov3r1oads-qt-solutions\qtwinmigrate\examples\qtdll 目录，该目录下面有qtdll.pro工程文件 用       qt5.3打开该文件。

3 测试是否可以进行编译，上述文件后在Qtcreater 中构建，或者编译该项目，编译通过后会弹出下图所示自定义执行档对话框，此种情况很正常，因为是dll 项目无程序入口qt让指定入口,关闭即可。第一次编译出现此界面，随后就不会出了，关闭后找到dll生成目录我的机子上为D:\qt-solutions-ov3r1oads-qt-solutions\qtwinmigrate\examples\build-qtdll-Desktop_Qt_5_3_0_MinGW_32bit-Debug\debug 如果成功该目录下会有我们期待已久的qtdialog.dll。   

  4  上述步骤顺利的话说明你的可以往下进行了。我开始的理解是这个qtdialog.dll 即使我所需要的，将它拷贝出来就可以用了，可实践证明我想多了。用VS2010建了一个MFC工程进行测试，测试例子很简单就一个基于对话框的界面上面放一个按钮，按钮事件是调用该dll.  

HMODULE  mod = LoadLibrary( "qtdialog.dll" );

DWORD  err= GetLastError();

 if ( mod ) { typedef BOOL(*pShowDialog)(HWND parent);

 pShowDialog showDialog = (pShowDialog)GetProcAddress( mod, "showDialog" );

 if ( showDialog )

 showDialog( theApp.m_pMainWnd->m_hWnd );

 FreeLibrary( mod );

 } else {

 CAboutDlg aboutDlg;

 aboutDlg.DoModal();

 }



    接下来就是一个痛苦的过程了，

        问题1：无论如何也不能加载qtdialog.dll ，mod 的值始终都是0， 工程目录下明明有该dll文件。

            解决办法： 调用   LoadLibrary();后立马调用GetLastError()得到错误码，分析错误码。

            错误码为126 意思是无法加载相应模块。这可怎么办呢？后来想想是不是dll依赖的一些qt的库没有复制过来导致的，遂使用Depends.exe查看该dll的依赖项，果不其然依赖挺多的后将各个依赖项在qt的安装目录下通过搜索找到放到MFC工程目录下。找到这些后还会提示有依赖项，总之把所有的依赖项添加完毕我一共添加了下图那么多可能有些是多余的。当然这是debug版本下的都带了个d.因为这个依赖项的问题搞了整整一天！


        问题2：依赖问题解决了也能载入dll了而且mod不为空，而且能够得到showDialog（）函数的地址 但是一直出这样的错误，太坑爹了，不知道什么原因呀！！


                    解决办法：首先尝试了各种方法，下载旧版本的VC运行时，修复dll ............总之就差重装系统了，没有解决，最后想想是不是这种调用方法有问题？

                                        按照参考2 中的方法试一下。

                                        打开qtdll工程，打开源文件中的main.cpp 备用，然后右键单击qtdll工程添加新文件“qt界面设计师类”继承与mainwindow 类当然也可以是其他的界面类，

                                        在界面上随意放置两个组件，如button，label等

                                        然后将main函数中的下面一段：                           

//extern "C" __declspec(dllexport) bool showDialog( HWND parent )

//{

//    QWinWidget win( parent );

//    win.showCentered();

//    QMessageBox::about( &win, "About QtMfc", "QtMfc Version 1.0\nCopyright (C) 2003" );

//    return TRUE;

//}

                    换成

extern "C" __declspec(dllexport)int main(int argc, char *argv[])

{

 QApplication a(argc, argv);

 MyJieMian w;

 w.show();

 return a.exec();

}

     重新编译通过。重新将生成的dll文件拷贝到MFC测试工程中

5   测试成功         

    将MFC按钮响应程序改为下述代码

void CMFCDLLDlg::OnBnClickedButton1()

{

// TODO: Add your control notification handler code here

HMODULE  mod = LoadLibrary( "qtdialog.dll" );

DWORD  err= GetLastError();

 if ( mod ) {

 typedef int (*Mymain)(int argc, char *argv[]);

 Mymain mmain= (Mymain)GetProcAddress(mod,"main");

char* b="abc";

char**a=&b;

 mmain(0,a);

 FreeLibrary( mod );

 } else {

 CAboutDlg aboutDlg;

 aboutDlg.DoModal();

 }

}

按下MFC中的按钮奇迹出现了，虽然丑了点但是还是出现了Qt下设计好的界面了。

相关链接

官网：[https://www.qt.io/](https://www.qt.io/)

官方下载链接：[http://download.qt.io/](http://download.qt.io/)

Qt下载链接：[http://download.qt.io/archive/qt/](http://download.qt.io/archive/qt/)

Qt Creator下载链接：[http://download.qt.io/archive/qtcreator/](http://download.qt.io/archive/qtcreator/)

Qt VS开发插件下载链接：[http://download.qt.io/archive/vsaddin/](http://download.qt.io/archive/vsaddin/)

Qt官方文档链接：[http://doc.qt.io/](http://doc.qt.io/)

Qt API在线参考文档：[http://www.kuqin.com/qtdocument/index.html](http://www.kuqin.com/qtdocument/index.html)

Qt中调用MFC对话框实例：[https://download.csdn.net/download/byxdaz/11052330](https://download.csdn.net/download/byxdaz/11052330)

