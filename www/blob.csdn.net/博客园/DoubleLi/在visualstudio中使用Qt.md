# 在visualstudio中使用Qt - DoubleLi - 博客园







# 1.  说明

在此说明一下IDE跟封装的之间的关系，他们之间本质上来说没有关系，是可以多对对的关系。



Qt开发是个比较泛的概念，Qt是由很多一系列类组成的整体，就像boost里面也有很多的类，而boost是一个整体的概念。同样，MFC也是一个比较泛的概念，里面有很多类。

这三种都是所谓的编程或封装的类。



visual studio是VC开发的一个IDE，eclipse似乎也可以作为VC开发的IDE。记事本也是个IDE，只是比较简陋。QtCreator也是个IDE。



visual studio这个IDE也可以来开发**“使用Qt库”**的程序。只是站在程序编译的角度，你要告诉IDE你这个文件该怎么编译。



维护商业版Qt的公司开发了一个“**方便编译Qt程序”**的IDE，叫QtCreator，这个工具跨平台，只是相比visualstudio，这个工具的功能不太强大，想尝试的童鞋可以试试。



你可以在任何工程中使用Qt的库，比如使用Qt的字符处理类QString、QFile。需要注意的是，如果你的程序中使用了消息响应相关的内容，那主程序就得是QtCoreApplication。不过很多时候你不需要关注这种细节。

Qt的消息响应并不局限在界面程序，不带界面的程序里面也会有消息处理。

当然，MFC的界面类和Qt的界面类是可以一起出现在程序里面的，不过这个只有高手才会，入门者一般不推荐这样做。

# 2.  开发

## 1.1.  基础知识

### 部署开发环境;

将qt安装在d盘根目录下,新建以下两个环境变量：QMAKESPEC=win32-msvc2008、QTDIR=D:\Qt\4.6.2

往Path路径加入”%QTDIR%\bin”

### VC工程设置

设置头文件include目录，如下图：



设置链接库目录：



设置依赖的库库文件（注意Debug和Release版本文件不要用错）：





### Qt基础知识

学习《C++ GUI Programming with Qt4》章节Chapter 1；

理解signal、slot；

(可参考帮助文档(Home/The QtObject Model/signals and slots)章节)

### 参考资料：

C++ GUI Programming with Qt4

Qt Assistant帮助文档

## 1.2.  Qt GUI编程

### 对话框QDialog：

使用对话框作为信息的录入界面；

学习《C++ GUI Programming with Qt4》Chapter2

### 掌握程序主窗口、菜单、工具栏、Action响应处理：

学习《C++ GUI Programming with Qt4》Chapter3

D:\Qt\4.6.2\examples\mainwindows\application;

### 构建程序界面的方法：

方法1、使用designer设计界面，界面直观，适用于复杂的界面布局；

（ui文件的使用见“编程杂项\编译UI文件”）

方法2、在界面简单且后续很少改动的情况下直接使用代码手工编写界面（使用Layout）；

### 列表控件QTreeWidget：

使用QTreeWidget显示具有层次结构的数据；

示例代码：

Example\TreeWidget;

## 1.3.  数据库操作

连接oracle数据库，掌握QSqlDatabase；

使用QSqlQuery实现增、删、查、改等基本数据库操作；

学习《C++ GUI Programming with Qt4》Chapter 13.

示例代码：Example\Database;

## 1.4.  2D图形绘制

学习《C++ GUI Programming with Qt4》Chapter8

# 3.  编程杂项

## 中文显示

设置编码，在构造Application时调用：

QTextCodec * pCodeC =QTextCodec::codecForName(“GB18030”);

QTextCodec::setCodecForCStrings(pCodeC);l

QTextCodec::setCodecForLocal(pCodeC);

QTextCodec::setCodecForTr(pCodeC);

构造中文内容的QString

QString strContent =QString::fromLocal8Bit(“中文内容”);



QString、CString、const char *间的转换：

const char * szTest = “测试文字”;

CString strC = CString(szTest);

QString strQ =QString::fromLocal8Bit(szTest)；

strC = QString::fromLocal8Bit((const char*)strQ.toLocal8Bit());



## 编译UI文件（界面文件）



设置ui文件的编译选项：

Command Line:

%qtdir%\bin\uic.exe $(InputPath) -o$(InputDir)ui_$(InputName).h

Description:

Uic'ing $(InputName).ui ...

Outputs:

$(InputDir)ui_$(InputName).h

使用举例：

**DMSCreatePlanDlg.ui通过uic命令生成了ui_DMSCreatePlanDlg.h**

**DMSCreatePlanDlg.h文件:**

#include “ui_DMSCreatePlanDlg.h”

class DMSCreatePlanDlg : public QDialog

{

Q_OBJECT

public:

       DMSCreatePlanDlg(QWidget* pParent);

private:

       Ui::DMDCreatePlanDlgui;

}

DMSCreatePlanDlg.cpp文件：

DMSCreatePlanDlg::DMSCreatePlanDlg(QWidget* pParent)

: QDialog(pParent)

{

       ui.setupUi(this);

       ……

}

## 编译带moc信息头文件

设置需要生成moc信息头文件的编译选项：

Command Line:

%qtdir%\bin\moc.exe$(InputDir)$(InputName).h

-o $( ProjectDir)moc_$(InputName).cpp

Description:

Moc'ing $(InputName).h ...

Outputs:

$( ProjectDir)moc_$(InputName).cpp

每个qt工程一般包含一个autogen.cppx文件，用来include生成的moc_.cpp文件。



元对象编译器moc (the Meta Object Complier)：

moc 分析C＋＋源文件，如果它发现在一个头文件(header file)中包含Q_OBJECT宏定义，然后动态的生成另外一个C++源文件，这个新的源文件包含 Q_OBJECT的实现代码，这个新的 C++ 源文件也会被编译、链接到这个类的二进制代码中去，因为它也是这个类的完整的一部分。通常，这个新的C++ 源文件会在以前的C++ 源文件名前面加上 moc_ 作为新文件的文件名。



## QWidget和QDialog的选择

QWidget是窗口元件，可当做控件，一般用于嵌入在其他QWidget或对话框中；

QDialog是独立的窗口，用于独立显示；

## 使用[Visual Studio Add-in 1.2.3 for Qt5](http://download.qt-project.org/official_releases/vsaddin/qt-vs-addin-1.2.3-opensource.exe)

使用此工具可以免除添加编译选项的麻烦

下载地址Qt相关程序的地址

[http://qt-project.org/downloads](http://qt-project.org/downloads)



![](http://img.blog.csdn.net/20140903140217505?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHJlYW1lcnNm/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



在安装Qt的开发插件之后，visual studio的IDE是长这样的



![](http://img.blog.csdn.net/20140903140322386?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHJlYW1lcnNm/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](http://img.blog.csdn.net/20140903140344974?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHJlYW1lcnNm/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



在工程上右键多了这么几个选项：



![](http://img.blog.csdn.net/20140903140147765?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHJlYW1lcnNm/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




- 顶
0- 踩










