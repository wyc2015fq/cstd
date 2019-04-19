# qt5信号与槽新语法 - baidu_33570760的博客 - CSDN博客
2017年05月03日 16:03:19[carman_风](https://me.csdn.net/baidu_33570760)阅读数：319
    转自 http://blog.csdn.net/dbzhang800/article/details/6547196
在 Qt5 的 qtbase-staging 代码仓库的 qobject_connect_ptr 分支中已经引入了一种全新的信号与槽的语法。
参看 [New Signal Slot Syntax Possibly Coming in Qt 5](http://developer.qt.nokia.com/wiki/New_Signal_Slot_Syntax)一文。
## 新老语法
如果我们将一个QSlider对象的valueChanged信号链接到一个QSpinBox对象的setValue槽，使用传统方式：
connect(slider, SIGNAL(valueChanged(int)), spinbox, SLOT(setValue(int)));
而使用新式语法，这个样子：
connect(slider, &QSlider::valueChanged, spinbox, &QSpinBox::setValue);
使用新式语法：
- 编译期：检查信号与槽是否存在，参数类型检查，Q_OBJECT是否存在
- 信号可以和普通的函数、类的普通成员函数、lambda函数连接（而不再局限于信号函数和槽函数）
- 参数可以是 typedef 的或使用不同的namespace specifier
- 可以允许一些自动的类型转换（即信号和槽参数类型不必完全匹配）
这一切都太吸引人了，我们稍候逐一查看
## 例子
例子很简单，考虑到大家应该没安装该版本的Qt，故稍微罗嗦一下。
(注意：现在是2011年6月15日，或许等你感兴趣想安装时，仓库已经有了较大变化。- dbzhang800)
### 准备工作
仓库地址：[https://qt.gitorious.org/+qt-developers/qt/qtbase-staging](https://qt.gitorious.org/+qt-developers/qt/qtbase-staging)
- 你可以直接通过git克隆该仓库
git clone git://gitorious.org/+qt-developers/qt/qtbase-staging.git
然后checkout出 qobject_connect_ptr 这个分支
- 
你可以直接下载打包后的源码[qobject_connect_ptr压缩包](https://qt.gitorious.org/qt/qtbase-staging/archive-tarball/qobject_connect_ptr)
剩下的工作就不用说了，configure、make、make install
### 代码
看个完整的程序代码，除了两个connect是新的，其他的应该都无须解释。
#include <QtGui/QApplication>
#include <QtGui/QHBoxLayout>
#include <QtGui/QSlider>
#include <QtGui/QSpinBox>
class Widget:public QWidget
{
public:
    Widget(QWidget *parent=0);
};
Widget::Widget(QWidget *parent):
    QWidget(parent)
{
    QHBoxLayout * layout = new QHBoxLayout(this);
    QSlider * slider = new QSlider(Qt::Horizontal);
    QSpinBox * spinbox = new QSpinBox;
    layout->addWidget(spinbox);
    layout->addWidget(slider);
    connect(slider, &QSlider::valueChanged, spinbox, &QSpinBox::setValue);
    connect(spinbox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), slider, &QSlider::setValue);
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
恩，如你所想，程序一切正常。
## 老语法的不足
我们知道老式语法connect中接收的是两个字符串,
bool QObject::connect ( const QObject * sender, const char * signal, const QObject * receiver, const char * method, Qt::ConnectionType type = Qt::AutoConnection ) [static]
比如：
connect(slider, SIGNAL(valueChanged(int)), spinbox, SLOT(setValue(int)));
编译预处理以后就是：
connect(slider, "2valueChanged(int)", spinbox, "1setValue(int)");
这有什么问题呢？
- 即使信号和槽不存在，编译不会出问题。只有运行时会给出警告并返回false，可是大部分用户并不检查返回值。
- 参数必须匹配，比如信号参数是 int，槽参数是 double，语法将会 connect 失败
- 参数类型必须字面上一样，比如说都是int，但是其中一个typedef了一下：
typedef int myInt;
connect(a, SIGNAL(sig(int)), b, SLOT(slt(myInt)));
或者namespace修饰不一样
using namespace std;
connect(a, SIGNAL(sig(std::string)), b, SLOT(slt(string)));
都会导致连接失败。
我们在[Qt信号和槽，与const char* 的故事](http://hi.baidu.com/cyclone/blog/item/004db93814c19fcdd4622560.html)一文中详细地讨论过这些问题。
新式的信号槽写法完全避免了这些问题。
## 编译期检查
新式语法是使用模板来实现的。由于模板的实例化是编译期完成的，所以如果有问题编译时直接就可以暴露出来，这比老式用法(问题要在运行时才能反应出来)是的巨大的改进。
### 信号或槽不存在
注意看connect的写法
connect(a, &Widget::sig1, b, &Widget::slt2);
都是用的函数的地址
- 如果相应的函数不存在，编译器将直接告知：
../newconnect/main.cpp:26:20: error: ‘sig1’ is not a member of ‘Widget’
../newconnect/main.cpp:26:41: error: ‘slt2’ is not a member of ‘Widget’- 如果使用Widget的信号，而Widget中没有添加Q_OBJECT宏，编译器将直接告知
src/gui/kernel/qwidget.h: In member function ‘void QWidget::qt_check_for_QOBJECT_macro(const T&) const [with T = Widget]’:
......................
../../qt-labs/qtbase-newsignal-build/include/QtGui/../../../qtbase/src/gui/kernel/qwidget.h:149:5: error: void value not ignored as it ought to be
模板一出错，给的东西总是这麽多，只好中间大部分都省略了。
### 参数不匹配
- 如果信号参数是int，槽参数是double。或者信号参数是QString，槽参数是QVariant。将不再有问题。
- 如果参数不能隐式cast，将会直接报错。比如信号参数是 int，槽参数是QString：
src/corelib/kernel/qobject.h: In static member function ‘static void QtPrivate::FunctionPointer<Ret (Obj::*)(Arg1)>::call(Ret (Obj::*)(Arg1), Obj*, void**) [with Args = QtPrivate::List<int, void>, Obj = Widget, Ret = void, Arg1 = QString, Ret (Obj::*)(Arg1) = void (Widget::*)(QString)]’...
同样很的错误风格。没办法，模板总是这样子
### 重载的函数怎么办？
注意看我们一开始给出的例子中给出的两个 connect 语句
connect(slider, &QSlider::valueChanged, spinbox, &QSpinBox::setValue);
connect(spinbox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), slider, &QSlider::setValue);
前一个很简洁，可是后一个？什么情况！！！
呵呵，没办法啊，QSpinbox的valueChanged信号是重载的：
void valueChanged ( int i )
void valueChanged ( const QString & text )
只好显示调用static_cast了。
## lambda函数
[C++](http://lib.csdn.net/base/cplusplus)0x标准引入了lambda函数，这个东西配合新式的connect使用似乎是很有意思。
比如：当QSlider的值改变时，通过qDebug输出该值，我们只需要
connect(slider, &QSlider::valueChanged, [](int v){qDebug()<<"slider value: "<<v;});
如果在以前，我们只能先定义一个槽函数，然后connect到该槽函数。
### C++0x启用
- 如果你使用的 MSVC 2010, 直接用，不需要任何设置
- 如果你使用的GCC，在pro文件内添加：
QMAKE_CXXFLAGS += -std=c++0x
### 异步操作
lambda配合新式connect，使得异步操作变得更简单了。
- 打开一个创建在heap中的对话框，调用open() 不阻塞程序运行。
- 连接其finished信号到一个lambda函数
- ...
static void outputSelectedFileName()
{
    QFileDialog *dlg = new QFileDialog();
    dlg->open();
    QObject::connect(dlg, &QDialog::finished, [dlg, this](int result) {
        if (result) {
            QString name = dlg->selectedFiles().first();
            qDebug()<<name;
            // ...
        }
        dlg->deleteLater();
    });
}
