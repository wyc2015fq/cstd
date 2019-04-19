# Qt 基础知识 - 三少GG - CSDN博客
2013年01月03日 23:32:00[三少GG](https://me.csdn.net/scut1135)阅读数：1025
**优秀的学习资源 !!!**[](http://blog.51cto.com/zt/20/1)
[http://blog.51cto.com/zt/20/1](http://blog.51cto.com/zt/20/1)
[http://blog.csdn.net/iamdbl/article/category/275004](http://blog.csdn.net/iamdbl/article/category/275004)
事件是视窗系统或者Qt本身在各种不同的情况下产生的。当用户点击或者释放鼠标，键盘时，一个鼠标事件或者键盘事件就产生了。当窗口第一次显示时，一个绘制事件会产生告诉新可见的窗口绘制自己。很多事件是为了相应用户动作产生的，也有一些事件是由 系统独立产生的。
在用Qt编程时，我们很少要考虑事件，当一些事件发生时，Qt控件会发出相应的信号。只有当实现用户控件或者需要修改现有控件的行为时，我们才需要考虑事件。
事件不能和信号混淆。一般来讲，在使用控件时需要处理的是信号，在实现一个控件时需要处理事件。例如，**我们使用QPushButton时，我们只要clicked()信号就可以了，而不用管鼠标点击事件。但是如果我们实现一个像QPushButton这样的类，我们就需要处理鼠标或者键盘事件，发出clicked()信号。**
前面说过，**Qt使用的是自己的预编译器，它提供了对C++的一种扩展。利用Qt的信号槽机制，就可以把彼此独立的模块相互连接起来，不需要实现知道模块的任何细节。**
为了达到这个目的，Qt提出了一个Meta-Object系统。它提供了两个关键的作用：信号槽和内省。
面向对象程序设计里面会讲到Smalltalk语言有一个元类系统。所谓元类，就是这里所说的Meta-Class。如果写过HTML，会知道HTML标签里面也有一个<meta>，这是用于说明页面的某些属性的。同样，Qt的Meta-Object系统也是类似的作用。内省又称为反射，允许程序在运行时获得类的相关信息，也就是meta-information。什么是meta-information呢？举例来说，像这个类叫什么名字？它有什么属性？有什么方法？它的信号列表？它的槽列表？等等这些信息，就是这个类的meta-information，也就是“元信息”。这个机制还提供了对国际化的支持，是QSA(Qt
 Script for Application)的基础。
标准C++并没有Qt的meta-information所需要的动态meta-information。所以，Qt提供了一个独立的工具，moc，通过定义Q_OBJECT宏实现到标准C++函数的转变。moc使用纯C++实现的，因此可以在任何编译器中使用。
这种机制工作过程是：
首先，Q_OBJECT宏声明了一些QObject子类必须实现的内省的函数，如metaObject()，tr(),qt_metacall()等；
第二，Qt的moc工具实现Q_OBJECT宏声明的函数和所有信号；
第三，QObject成员函数connect()和disconnect()使用这些内省函数实现信号槽的连接。
以上这些过程是qmake，moc和QObject自动处理的，你不需要去考虑它们。如果实现好奇的话，可以通过查看QMetaObject的文档和moc的源代码来一睹芳容。
## qt uic命令介绍
windows的qt里有个uic.exe
qt4以下
uic xxx.ui -o xxx.h
uic xxx.ui -i xxx.h -o xxx.cpp
qt4
uic xxx.ui -o xxx.h把UIC.exe和你要转换的xxx.ui文件拷贝到同一目录。
开始菜单，运行CMD
进入uic.exe和xxx.ui的目录，运行以下命令：
uic xxx.ui -o xxx.h 生成.h文件
uic xxx.ui -i xxx.h -o xxx.cpp 生成.cpp文件
### Qt uic使用
1、用Qt Designer创建mainwindow.ui
2、生的.h类命令：uic -o mainwindow.h mainwindow.ui
3、使用ui的方法（3种）：
a、直接使用法
包含头文件：#include "mainwindow.h"
#include <QtGui/QApplication>
#include "mainwindow.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);   
   QDialog dlg;//创建Dialog
 MainWindow ui;
 ui.setupUi(&dlg);//把ui加载到Dialog中
 ui.show();
    return a.exec();
}
b.单一继承法
#include "mainwindow.h"
#include <QDialog>
class MyClass:public QDialog
{
  public:
  MainWindow ui;
  MyClass()
  {
 ui.setupUi(*this);
  }
}
#include <QtGui/QApplication>
#include "myclass.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);   
 MyClass dlg;//创建Dialog 
 dlg.show();
    return a.exec();
}
c.多重继承法
#include "mainwindow.h"
#include <QDialog>
class MyClass:public QDialog，public MainWindow
{
  public:
  MyClass()
  {
 setupUi(*this);//已经继承了MainWindow的setupUi方法
  }
}
#include <QtGui/QApplication>
#include "myclass.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);   
 MyClass dlg;//创建Dialog 
 dlg.show();
    return a.exec();
}
# [在IDE（VC++）下开发Qt程序](http://www.cnblogs.com/codespy/archive/2008/08/16/1269429.html)
以MS visual studio为例。在项目的源码目录下建立一个GeneratedFiles文件夹，然后在visual studio中为项目新建一个筛选器Generated Files。选择需要使用Qt元对象编译器（moc.exe）进行编译的一个头文件（假设这个文件为mainwindow.h），然后打开它的属性对话框。选择属性中的“自定义生成步骤|常规”，
在”命令行“中输入："$(QTDIR)\bin\moc.exe"  -I".\GeneratedFiles\." -I".\GeneratedFiles\$(ConfigurationName)\." ".\mainwindow.h" -o ".\GeneratedFiles\$(ConfigurationName)\moc_mainwindow.cpp"
在“输出”中输入：".\GeneratedFiles\$(ConfigurationName)\moc_mainwindow.cpp"
在“附加依赖项中输入”："$(QTDIR)\bin\moc.exe";.\mainwindow.h
使用VS的IDE宏进行设置会更方便一些。
然后编译mainwindow.h，将编译出的moc_mainwindow.cpp添加到筛选器中。再编译整个项目。
![mainwindow_h](http://images.cnblogs.com/cnblogs_com/codespy/WindowsLiveWriter/IDEQt_DA29/mainwindow_h_thumb.jpg)
此外，Qt程序还会用到窗体文件（*.ui）和资源文件(*.qrc)。
窗体文件（mainwindow.ui）:
命令行 ："$(QTDIR)\bin\uic.exe" -o ".\GeneratedFiles\ui_mainwindow.h" "$(InputPath)"
输出：".\GeneratedFiles\ui_mainwindow.h"
附加依赖项：$(QTDIR)\bin\uic.exe
资源文件（main.qrc，schematic_view.jpg为资源）:
命令行："$(QTDIR)\bin\rcc.exe" -name "main" -no-compress ".\main.qrc" -o .\GeneratedFiles\qrc_main.cpp
输出：.\GeneratedFiles\qrc_main.cpp
附加依赖项：.\main.qrc;.\schematic_view.jpg
另外，用Qt designer生成的资源文件，prefix没有'/'，如<qresource prefix="main" >，应改为<qresource prefix="/main" >
这样就能以此格式使用资源
setWindowIcon( QIcon(":/main/schematic_view.jpg") );
对于其他IDE，设置上应该差不多。如果你安装了Qt for visual studio的插件，那么这些东西会自动设置。
如果你的IDE没有类似于“自定义生成步骤”这样的选项，那么就直接把它删除。
**信号与槽的使用，需要用到moc(即meta object compiler)。**    这是因为：当要在GUI中用到信号与槽，就需在.h文件中的类里写入Q_OBJECT宏。而任何含有Q_Object的类都必须使用Qt的moc工具生成对应的cpp文件，然后在项目里面包含这个cpp，编译才能成功，否则会出错链接错误
