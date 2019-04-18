# QT技巧总结 - gauss的专栏 - CSDN博客
2013年01月03日 18:09:04[gauss](https://me.csdn.net/mathlmx)阅读数：319
个人分类：[客户端编程](https://blog.csdn.net/mathlmx/article/category/944323)
1． 如何在Qt程序中加入OpenGL支持。
在QT程序中加入OpenGL支持很简单，只需要在Kdevelop连接的库中加入“-lGL -lGLU”即可，如果需要glut支持，还可以加入“-lglut”。具体操作是在kdevelop集成编译环境中按下”F7”，在弹出的对话框中选择 “Linker”一项，在输入栏输入你想添加的库即可，写法与gcc/g++一致。
一般在类QGLWidget中使用OpenGL,调用此类的头文件是qgl.h,具体写法请参考qt例程中的gear,texture,box等程序(在RedHat7.2中,它们在/usr/lib/qt-2.3.1/doc/examples下).
2． 检验linux/Unix环境是否支持OpenGL.
Qt中的QGLFormat类可以帮助我们轻易检验系统是否支持OpenGL，载入头文件（#include <qgl.h>）后，我们就可以使用QGLFormat的静态函数hasOpenGL来检验，具体写法如下例：
if (!QGLFormat::hasOpenGL()) //Test OpenGL Environment
{
qWarning( "This system has no OpenGL support. Exiting." );//弹出警告对话框
return -1;
}
3.获得屏幕的高和宽.
一般我们可以通过QT的Qapplication类来获得系统的一些信息,载入头文件(#include <qapplication.h>)我们就可以调用它,下例是使主程序充满整个屏幕的代码:
Gui_MainForm gui_mainform;
a.setMainWidget( &gui_mainform );
gui_mainform.resize( QApplication::desktop()->width(), QApplication::desktop()->height() ); gui_mainform.show();
4.关于信号和槽.
信号和槽机制是QT库的重要特性,可以说不了解它就不了解Qt.此机制能在各类间建立方便快捷的通信联系,只要类中加载了Q_OBJECT宏并用 connect函数正确连接在一起即可,具体写法这里就不赘述了.但本人在使用过程中发现使用此机制容易破坏程序的结构性和封装性,速度也不是很让人满 意,尤其是在跨多类调用时.鄙人的一孔之见是: 信号和槽机制不可不用,但不可多用.
5.QT程序中界面的设计.
尽管Kdevelop是一个优秀的集成编译环境,可遗憾的是它不是一个可视化的编译环境,好在有Qdesigner来帮助我们完成界面设计,该程序的使用 很简单,使用过VB,VC和Delphi的程序员能很快其操作方式,操作完成后存盘会生成一个扩展名为”ui”的文件,你接下来的任务就是把它解析成 cpp和h文件,假设文件名为myform.ui,解析方法如下:
$uic myform.ui –I myform.h –o myform..cpp //这句生成cpp文件
$uic myform.ui –o myform.h //这句生成h文件.
6.由pro文件生成Makefile.
对于Linux/Unix程序员来说编写Makefile文件是一项令人烦恼的任务,而qt程序员就没有这样的烦恼,一句$qmake –o Makefile myprogram.pro就可以轻松愉快的完成任务,而pro文件的编写也很容易,其核心是h和cpp文件的简单列表.具体写法请参考一下qt自带的样 例和教程吧(在RedHat7.2中,它在/usr/lib/qt-2.3.1/doc/examples下),相对Makefile文件简直没有什么难 度.
7.主组件的选择.
一般我们在编程是使用继承Qwidget类的类作为主组件,这当然未可厚非.但在制作典型的多文档和单文档程序时我们有更好的选择— QmainWindow类,它可以方便的管理其中的菜单工具条主窗口和状态条等,在窗体几何属性发生变化时也能完美的实现内部组件缩放,这比用传统的几何 布局类来管理要方便得多,而且不用写什么代码.关于它的具体细节请查阅QT的帮组文档,这里就不赘述了.
8.菜单项中加入Checked项.
在QT中,菜单项中加入Checked有点麻烦,具体写法如下:
1> 定义int型成员变量,并在创建菜单项中写:
displayGeometryMode=new QPopupMenu(this); //这里创建弹出菜单组displayGeometryMode
m_menuIDWire=displayGeometryMode->insertItem("Wire",this,SLOT(slt_Change2WireMode()));.//创建弹出菜单子项
displayGeometryMode->setItemChecked(m_ menuIDWire,true);//设定此子项为选择状态
2> 再在槽函数中写:
displayGeometryMode->setItemChecked(m_menuIDWire,false);//这里设定此子项为非选择状态
9.截获程序即将退出的信号.
有些时候我们需要在程序即将退出时进行一些处理,如保存文件等等.如何截获程序退出的信号呢?还是要用到Qapplication类的aboutToQuit()信号,程序中可以这样写:
connect(qApp,SIGNAL(aboutToQuit()),this,SLOT(Slot_SaveActions()));
在槽函数Slot_SaveActions()就可以进行相关处理了,注意,使用全局对象qApp需要加载头文件(#include <qapplication.h>).
10.弹出标准文件对话框.
在程序中弹出文件对话框是很容易处理的,举例如下:
QString filter="Txt files(*.txt)\n" //设置文件过滤,缺省显示文本文件
"All files(*)" ; //可选择显示所有文件
QString Filepathname=QFileDialog::getOpenFileName(" ",filter,this);//弹出对话框,这句需要加载头文件(#include < qfiledialog.h >)
11.将当前日期时间转化为标准Qstring.
QDateTime currentdatetime =QDateTime::currentDateTime();//需要加载头文件(#include < qdatetime.h >)
QString strDateTime=currentdatetime.toString();
12.设置定时器
所有Qobject的子类在设置定时器时都不必加载一个Qtimer对象,因为这样造成了资源浪费且需要书写多余的函数,很不方便.最好的办法是重载timerEvent函数,具体写法如下:
class Gui_DlgViewCtrlDatum : public QDialog
{
Q_OBJECT
public:
Gui_DlgViewCtrlDatum( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
~Gui_DlgViewCtrlDatum();
protected:
void timerEvent( QTimerEvent * );
};
void Gui_DlgViewCtrlDatum::timerEvent( QTimerEvent *e )
{
//statements
}
再在Gui_DlgViewCtrlDatum的构造函数中设置时间间隔:
startTimer(50);//单位为毫秒
这样,每隔50毫秒,函数timerEvent便会被调用一次.
13.最方便的几何布局类QGridLayout
在QT的几何布局类中,笔者认为QgridLayout使用最为方便,举例如下:
QGridLayout* layout=new QGridLayout(this,10,10);//创建一个10*10的QgridLayout实例
layout->addMultiCellWidget(gui_dlgslab_glwnd,1,8,0,7);//将OpenGL窗口固定在QgridLayout中的(1,0)单元格到(8,7)单元格中
layout->addMultiCellWidget(Slider1,0,9,8,8);//将一个slider固定在单元格(0,8)到(9,8)中
layout->addWidget(UpLimitLbl,1,9);//将一个label(UpLimitLbl)固定在单元格(1,9)中
这样,无论窗体大小如何改变,它们的布局方式都不会发生改变,这比反复使用QvboxLayout和QhboxLayout要方便快捷许多.
注:使用几何布局类需要调用头文件(#include <qlayout.h>)
14.字符串类Qstring和字符串链表类QstringList.
Qstring是Qt中标准字符串类,下面列出它的一些常用函数:
toInt():将字符串转化成int类型.
ToFloat():将字符串转化成float类型.
ToDouble():将字符串转化成double类型.
Left(n):从左起取n个字符
Right(n):从右起取n个字符
SetNum(n):将实数n(包括int,float,double等)转化为Qsting型.
QstringList是大家比较少使用的类,它可以看成Qstring组成的链表(QT中标准链表类Qlist的函数对它都适用,它的单个节点是Qstring类型的),特别适合与处理文本,下面一段代码就可见其方便快捷:
Qstring strtmp=”abc|b|c|d”;
QstringList strlsttmp;
Strlsttmp =QStringList::split("|", strtmp);
For(unsigned int I=0;I< Strlsttmp.count();I++)
{
cout<< Strlsttmp.at(I);
}
结果输出为:abc b c d,也就是说,通过一个函数split,一行文本就被符号”|”自动分割成了单个字符串.这在文本处理时特别省力.(请参考c语言大全第四版中用”strtok”函数分割文本的例程,将双方比较一下)
15. QGLWidget类如何加入鼠标支持.
QGLWidget类加入鼠标支持需要重载以下函数:
void mousePressEvent(QMouseEvent*);
void mouseMoveEvent(QMouseEvent*);
void mouseReleaseEvent(QMouseEvent*);
请具体看一个实例:
class Gui_WgtMain_GLWnd : public QGLWidget {
Q_OBJECT
public:
Gui_WgtMain_GLWnd(QWidget *parent=0, const char *name=0);
~Gui_WgtMain_GLWnd();
protected:
void initializeGL();
void paintGL();
void resizeGL( int w, int h );
void mousePressEvent(QMouseEvent*);
void mouseMoveEvent(QMouseEvent*);
void mouseReleaseEvent(QMouseEvent*);
private:
int m_nCnt;
};
void Gui_WgtMain_GLWnd::mousePressEvent(QMouseEvent* e)
{
//statements
}
void Gui_WgtMain_GLWnd:: mouseMoveEvent (QMouseEvent* e)
{
//statements
}
void Gui_WgtMain_GLWnd:: mouseReleaseEvent (QMouseEvent* e)
{
//statements
}
其中, e->x();e->y();可以获得鼠标的位置, e->button()可以取得鼠标按键的状态(左中右键以及ctrl,alt,shift等组合键),灵活使用他们就可以在用鼠标操作OpenGL画面了.
16.由ui文件生成.h和.cpp文件
生成.cpp文件
$uic myform.ui -i myform.h -o myform.cpp
生成.h文件
$uic myform.ui -o myform.h 
