# Qt中Ui名字空间以及setupUi函数的原理和实现 - xqhrs232的专栏 - CSDN博客
2016年10月14日 23:18:12[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：174
个人分类：[X11/Qt/Qt quick/Qxml界面技术](https://blog.csdn.net/xqhrs232/article/category/6455019)
原文地址::[http://blog.csdn.net/desert187/article/details/39672371](http://blog.csdn.net/desert187/article/details/39672371)
相关文章
1、[在Qt中如何使用QtDesigner创建的UI文件（一）](http://blog.csdn.net/desert187/article/details/39672039)----[http://blog.csdn.net/desert187/article/details/39672039](http://blog.csdn.net/desert187/article/details/39672039)
2、[在Qt中如何使用QtDesigner创建的UI文件（二）](http://blog.csdn.net/desert187/article/details/39672061)----[http://blog.csdn.net/desert187/article/details/39672061](http://blog.csdn.net/desert187/article/details/39672061)
用最新的QtCreator选择GUI的应用会产生含有如下文件的工程
![](http://images.cnblogs.com/cnblogs_com/pingf/207761/o_%E8%87%AA%E5%8A%A8%E7%94%9F%E6%88%90.png)
下面就简单分析下各部分的功能。
.pro文件是供qmake使用的文件，不是本文的重点【不过其实也很简单的】，在此不多赘述。
所以呢，还是从main开始，
**[cpp]**[view
 plain](http://blog.csdn.net/desert187/article/details/39672371#)[copy](http://blog.csdn.net/desert187/article/details/39672371#)
- #include <QtGui/QApplication>
- #include "mainwindow.h"
- int main(int argc, char *argv[])  
- {  
- QApplication a(argc, argv);  
- MainWindow w;  
- w.show();  
- return a.exec();  
- }  
很简单的样子
QApplication a(argc, argv)和a.exec()可以理解为载入了Qt的[架构](http://lib.csdn.net/base/architecture)，跑Qt的程序都要有此部，就不多说了。
其中调用了个MainWindow并把它show了出来，具体分析下
下面是mainwindow.h中的内容
**[cpp]**[view
 plain](http://blog.csdn.net/desert187/article/details/39672371#)[copy](http://blog.csdn.net/desert187/article/details/39672371#)
- #ifndef MAINWINDOW_H
- #define MAINWINDOW_H
- #include <QtGui/QMainWindow>
- namespace Ui  
- {  
- class MainWindow;  
- }  
- class MainWindow : public QMainWindow  
- {  
- Q_OBJECT  
- public:  
- MainWindow(QWidget *parent = 0);  
- ~MainWindow();  
- private:  
- Ui::MainWindow *ui;  
- };  
- #endif // MAINWINDOW_H
开始的namespace Ui可能让人有点摸不着头脑，这是因为qt把ui相关的类单独独立了出来，但类名相同，禁用namespace区别【但是就目前的使用来说，感觉这样做不怎么好，后面我会解释原因】
**声明namespace Ui是因为要调用Ui中的MainWindow，此MainWindow非彼MainWindow，后面涉及的*ui指针会调用它！**
关于Q_OBJECT就不说了，Qt中与signal和slot相关的类都要这么声明下。
仔细看出了构造，析构就没啥了，只有那么个*ui！不过现在如果运行下，也只会生成个窗体而已。
下面来看构造函数和析构函数，其实也就是mainwindow.c
**[cpp]**[view
 plain](http://blog.csdn.net/desert187/article/details/39672371#)[copy](http://blog.csdn.net/desert187/article/details/39672371#)
- #include "mainwindow.h"
- #include "ui_mainwindow.h"
- MainWindow::MainWindow(QWidget *parent)  
- : QMainWindow(parent), ui(new Ui::MainWindow)  
- {  
- ui->setupUi(this);  
- }  
- MainWindow::~MainWindow()  
- {  
- delete ui;  
- }  
构造时在堆上new了个Ui域中的MainWindow，并调用setupUi，析构仅仅是将其delete了，还是很简单！
正如前面所述Qt很好的把ui分离了出去，前面图中的那个.ui文件就是让QtDesigner使的布局用文件！
现在运行下，会生成ui_mainwindow.h，这个里面会涉及到真正布局用的函数，也就是那个Ui域中的MainWindow.下面具体看一下，
**[cpp]**[view
 plain](http://blog.csdn.net/desert187/article/details/39672371#)[copy](http://blog.csdn.net/desert187/article/details/39672371#)
- #ifndef UI_MAINWINDOW_H
- #define UI_MAINWINDOW_H
- #include <QtCore/QVariant>
- #include <QtGui/QAction>
- #include <QtGui/QApplication>
- #include <QtGui/QButtonGroup>
- #include <QtGui/QHeaderView>
- #include <QtGui/QMainWindow>
- #include <QtGui/QMenuBar>
- #include <QtGui/QStatusBar>
- #include <QtGui/QToolBar>
- #include <QtGui/QWidget>
- QT_BEGIN_NAMESPACE  
- class Ui_MainWindow  
- {  
- public:  
- QMenuBar *menuBar;  
- QToolBar *mainToolBar;  
- QWidget *centralWidget;  
- QStatusBar *statusBar;  
- void setupUi(QMainWindow *MainWindow)  
- {  
- if (MainWindow->objectName().isEmpty())  
- MainWindow->setObjectName(QString::fromUtf8("MainWindow"));  
- MainWindow->resize(600, 400);  
- menuBar = new QMenuBar(MainWindow);  
- menuBar->setObjectName(QString::fromUtf8("menuBar"));  
- MainWindow->setMenuBar(menuBar);  
- mainToolBar = new QToolBar(MainWindow);  
- mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));  
- MainWindow->addToolBar(mainToolBar);  
- centralWidget = new QWidget(MainWindow);  
- centralWidget->setObjectName(QString::fromUtf8("centralWidget"));  
- MainWindow->setCentralWidget(centralWidget);  
- statusBar = new QStatusBar(MainWindow);  
- statusBar->setObjectName(QString::fromUtf8("statusBar"));  
- MainWindow->setStatusBar(statusBar);  
- retranslateUi(MainWindow);  
- QMetaObject::connectSlotsByName(MainWindow);  
- } // setupUi
- void retranslateUi(QMainWindow *MainWindow)  
- {  
- MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));  
- Q_UNUSED(MainWindow);  
- } // retranslateUi
- };  
- namespace Ui {  
- class MainWindow: public Ui_MainWindow {};  
- } // namespace Ui
- QT_END_NAMESPACE  
- #endif // UI_MAINWINDOW_H
吼吼，一下子多了不少，但其实还是很容易的。Ui_MainWindow声明了几个构件，具体我就不说了，因为也没啥可说的，它实现了setupUi函式，也就是前面那个MainWindow中调用的setupUi。
**但是要说明的是QMetaObject::connectSlotsByName函式会自动连接相应名 称的信号与槽，但要注意它连接的是传入的MainWindow及其子构件【不是子类】，注意前边ui->setupUi(this)中传入的 this，也就是非ui域中的MainWindow，所以如果要声明signal和slot时还是要在非ui域的MainWindow中来声明，然后通过 ui->xxx的形式来与GUI产生交互！如果我们在QtDesiner中拖放一个按钮然后点击[Go](http://lib.csdn.net/base/go) to slot就很容易印证这一点。**
retranslateUi则会为ui中的构件命名，具体也不在此多说。
最后还是看看这段代码
**[cpp]**[view
 plain](http://blog.csdn.net/desert187/article/details/39672371#)[copy](http://blog.csdn.net/desert187/article/details/39672371#)
- namespace Ui {  
- 
- class MainWindow: public Ui_MainWindow {};  
- 
- } // namespace Ui
**前面非Ui域中的MainWindow的*ui指向的是Ui域中的MainWindow，而Ui域中的MainWindow出了继承了Ui_MainWindow之外，内部一贫如洗！【有点绕口了】**
**来张图片，再复习下**![](http://images.cnblogs.com/cnblogs_com/pingf/207761/o_mainwindow.png)
最后要说明的有两点，个人感觉是QtCreator的BUG，
其一是如果自己定制控件，并且想在内置的designer中载入，win下用mingw是不可行的，因为sdk套件中的 designer是用微软的编译器编译的，当然也有个比较方便的解决的办法，就是把qtcreator的源码下来，用现有的creator再编译一遍，然 后覆盖过去就行了。
其二也是前面提到的，两个同名的MainWindow仅用Ui域来区分，虽然感觉这样做从设计上来说是很美的，但调试时却会有些许的问题，总之在creator中调试不能识别正确的域，具体见下图例
![](http://images.cnblogs.com/cnblogs_com/pingf/207761/o_debugproblem.png)
像上面这张图this实际上应该指向的是**非**Ui域中的MainWindow 【this其实指向的是MainWindow，它并不知是哪个域的MainWindow，再往下展开就错误的指向了Ui域】，但调试的数据区指向了Ui域 中的MainWindow，当然也不是没有解决的办法，你可以手工将Ui域中的MainWindow改下名就可以获得正确的调试信息了，只是这样做稍显麻 烦，而且再度运行qmake后可能还要重新修改。
来源： <[http://blog.csdn.net/songjinshi/article/details/7333119](http://blog.csdn.net/songjinshi/article/details/7333119)>
