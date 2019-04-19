# QT基础(二)----鼠标、键盘事件处理机制、信息拦截机制 - xqhrs232的专栏 - CSDN博客
2017年06月07日 10:29:29[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：207
个人分类：[X11/Qt/Qt quick/Qxml界面技术](https://blog.csdn.net/xqhrs232/article/category/6455019)
原文地址::[http://blog.csdn.net/qianqin_2014/article/details/51234752](http://blog.csdn.net/qianqin_2014/article/details/51234752)
相关文章
1、[Qt中的键盘事件，以及焦点的设置（比较详细）](http://www.cnblogs.com/findumars/p/6006070.html)----[http://www.cnblogs.com/findumars/p/6006070.html](http://www.cnblogs.com/findumars/p/6006070.html)
2、[QT5的软键盘输入法实现](http://blog.csdn.net/tracing/article/details/50617571)----[http://blog.csdn.net/tracing/article/details/50617571](http://blog.csdn.net/tracing/article/details/50617571)
# 一 鼠标事件处理机制 QEvent
新建文件或项目----其它选项----空项目----输入项目名称-----在**.pro**文件中添加代码**QT += widgets gui**----添加新文件----C++
 Class-----输入类名mouseWidget----**继承自QWidget**
.pro文件
**[cpp]**[view
 plain](http://blog.csdn.net/qianqin_2014/article/details/51234752#)[copy](http://blog.csdn.net/qianqin_2014/article/details/51234752#)
- HEADERS += \  
-     mousewidget.h  
- 
- SOURCES += \  
-     mousewidget.cpp  
- 
- QT += widgets gui  
头文件mousewidget.h
**[cpp]**[view
 plain](http://blog.csdn.net/qianqin_2014/article/details/51234752#)[copy](http://blog.csdn.net/qianqin_2014/article/details/51234752#)
- #ifndef MOUSEWIDGET_H
- #define MOUSEWIDGET_H
- 
- #include <QWidget>
- #include<QEvent>
- #include<QMouseEvent>
- 
- class mouseWidget : public QWidget  
- {  
-     Q_OBJECT  
- public:  
- explicit mouseWidget(QWidget *parent = 0);  
- 
- //消息处理函数是一个虚函数，当我们的类需要进行消息处理时，需要重载该虚函数才能为自己所用
- //所有窗口都可以用到Event函数，因为所有的消息传递过程都需要讲过该Event
- bool event(QEvent *);  
- 
- signals:  
- 
- public slots:  
- };  
- 
- #endif // MOUSEWIDGET_H
源文件mousewidget.cpp
**[cpp]**[view
 plain](http://blog.csdn.net/qianqin_2014/article/details/51234752#)[copy](http://blog.csdn.net/qianqin_2014/article/details/51234752#)
- #include "mousewidget.h"
- #include<QApplication>
- #include<QDebug>
- 
- mouseWidget::mouseWidget(QWidget *parent) : QWidget(parent)  
- {  
- 
- }  
- 
- bool mouseWidget::event(QEvent *ev)  
- {  
- if(ev->type() == QEvent::MouseButtonPress)  
-     {  
- //一般情况下我们部队鼠标按键直接通过QEvent函数处理，而是经过其子类QMouseEvent类来处理
-         QMouseEvent *ev1 = (QMouseEvent *)ev;  
- if(ev1->type() == QMouseEvent::MouseButtonPress)  
-         {  
-             qDebug()<<"mouse pressed!";  
-         }  
-     }  
- 
- //注意，函数最后必须返回原来的event函数，即返回父类的event
- return QWidget::event(ev);  
- }  
- 
- int main(int argc, char *argv[])  
- {  
-     QApplication app(argc, argv);  
- 
-     mouseWidget w;  
-     w.show();  
- 
-     app.exec();  
- }  
运行结果：在输出的窗口中用鼠标任意点击，调试框会输出"mouse pressed!"
注意：重载的虚函数Event在实现时千万不要忘记返回父类的Event函数
# 二 事件的截断机制 eventFilter函数
头文件mousewidget.h
**[cpp]**[view
 plain](http://blog.csdn.net/qianqin_2014/article/details/51234752#)[copy](http://blog.csdn.net/qianqin_2014/article/details/51234752#)
- #ifndef MOUSEWIDGET_H
- #define MOUSEWIDGET_H
- 
- #include <QWidget>
- #include<QEvent>
- #include<QMouseEvent>
- #include<QPushButton>
- 
- class mouseWidget : public QWidget  
- {  
-     Q_OBJECT  
- public:  
- explicit mouseWidget(QWidget *parent = 0);  
- 
-     QPushButton *_button;  
- //重载消息过滤机制
- bool eventFilter(QObject *, QEvent *);  
- 
- signals:  
- 
- public slots:  
- };  
- 
- #endif // MOUSEWIDGET_H
源文件mousewidget.cpp
**[cpp]**[view
 plain](http://blog.csdn.net/qianqin_2014/article/details/51234752#)[copy](http://blog.csdn.net/qianqin_2014/article/details/51234752#)
- #include "mousewidget.h"
- #include<QApplication>
- #include<QDebug>
- 
- mouseWidget::mouseWidget(QWidget *parent) : QWidget(parent)  
- {  
-     QPushButton *button = new QPushButton("OK", this);  
-     _button = button;  
- //将button按钮设置为该窗口的默认选中项目
-     button->setDefault(this);  
- //button为自己安装了一个消息过滤器，经过button的消息，都需要调用过滤器eventFilter函数
-     button->installEventFilter(this);  
- 
-     connect(button, &QPushButton::clicked, [this](){  
- this->close();  
-     });  
- }  
- 
- bool mouseWidget::eventFilter(QObject *o, QEvent *e)  
- {  
- if(o == (QObject *)_button && (e->type() == QEvent::MouseButtonPress ||  
-                                    e->type() == QEvent::MouseButtonRelease ||  
-                                    e->type() == QEvent::MouseButtonDblClick))  
-     {  
- returntrue;                //截断按钮的功能
-     }  
- 
- return QWidget::eventFilter(o, e);  
- }  
- 
- int main(int argc, char *argv[])  
- {  
-     QApplication app(argc, argv);  
- 
-     mouseWidget w;  
-     w.show();  
- 
-     app.exec();  
- }  
运行结果：未加**eventFilter**函数之前，点击OK按钮，窗口就会自动关闭，当加上时间过滤函数之后，点击OK按钮便没有了反应。因为我们为按钮安装了一个消息过滤器，按钮要处理的消息首先要经过该事件过滤器，当时间过滤器返回的是**true**的时候，就把按按钮将要指向的指令给拦截了。
#  三 更精准的鼠标事件处理机制
头文件mouseWidget.h
**[cpp]**[view
 plain](http://blog.csdn.net/qianqin_2014/article/details/51234752#)[copy](http://blog.csdn.net/qianqin_2014/article/details/51234752#)
- #ifndef MOUSEWIDGET_H
- #define MOUSEWIDGET_H
- 
- #include <QWidget>
- #include<QMouseEvent>
- #include<QEvent>
- #include<QDebug>
- 
- class mouseWidget : public QWidget  
- {  
-     Q_OBJECT  
- public:  
- explicit mouseWidget(QWidget *parent = 0);  
- 
- //更精准的鼠标事件梳理函数
- void mouseMoveEvent(QMouseEvent *);  
- void mousePressEvent(QMouseEvent *);  
- void mouseReleaseEvent(QMouseEvent *);  
- //不建议使用，因为该函数是调用两次mousePressEvent来说海鲜的
- void mouseDoubleClickEvent(QMouseEvent *);  
- signals:  
- 
- public slots:  
- };  
- 
- #endif // MOUSEWIDGET_H
源文件mousewidget.cpp
**[cpp]**[view
 plain](http://blog.csdn.net/qianqin_2014/article/details/51234752#)[copy](http://blog.csdn.net/qianqin_2014/article/details/51234752#)
- #include "mousewidget.h"
- #include<QApplication>
- 
- mouseWidget::mouseWidget(QWidget *parent) : QWidget(parent)  
- {  
- //鼠标不需要点下去，当鼠标在窗口内移动时就会打印消息
- this->setMouseTracking(true);  
- }  
- 
- void mouseWidget::mousePressEvent(QMouseEvent *ev)  
- {  
- //获取鼠标点击的位置
-     QPoint point = ev->pos();  
-     qDebug()<<point;  
- 
- //鼠标点击时做出响应
- //特别注意QEvent::MouseButtonPress返回的类型和Qt::LeftButton返回的类型以及Qt::ShiftModifier返回类型的不同之处
- if(ev->type() == QEvent::MouseButtonPress)  
-     {  
- if(ev->button() == Qt::LeftButton)  
-         {  
- if(ev->modifiers() == Qt::ShiftModifier)  
-                 qDebug()<<"shift + LeftButton pressed!";  
- elseif(ev->modifiers() == Qt::ControlModifier)  
-                 qDebug()<<"control + LeftButton pressed!";  
- else
-                 qDebug()<<"LeftButton pressed!";  
-         }  
- else
-         {  
- if(ev->modifiers() == Qt::ShiftModifier)  
-                 qDebug()<<"shift + RightButton pressed!";  
- elseif(ev->modifiers() == Qt::ControlModifier)  
-                 qDebug()<<"control + RightButton pressed!";  
- else
-                 qDebug()<<"RightButton pressed!";  
-         }  
-     }  
- }  
- 
- void mouseWidget::mouseMoveEvent(QMouseEvent *ev)  
- {  
- //类中加了this.setMouseTracking(true);只要鼠标在窗口内移动，就会打印下列消息
- //否则只有拖着鼠标移动时才会打印消息
-     qDebug()<<"mouse is moving...";  
- }  
- 
- void mouseWidget::mouseReleaseEvent(QMouseEvent *ev)  
- {  
- if(ev->type() == QEvent::MouseButtonRelease)  
-         qDebug()<<"released!";  
- }  
- 
- void mouseWidget::mouseDoubleClickEvent(QMouseEvent *ev){}  
- 
- int main(int argc, char *argv[])  
- {  
-     QApplication app(argc, argv);  
- 
-     mouseWidget w;  
-     w.show();  
- 
-     app.exec();  
- }  
**运行结果：在窗口上移动时会不断打印mouse is moving...，用不同的鼠标点击会输出不同的结果。**
**注意：**
- 特别注意QEvent::MouseButtonPress返回的类型和Qt::LeftButton返回的类型以及Qt::ShiftModifier返回类型的不同之处
- 在头文件中声明的函数，在源文件中一定要实现，不然会出现编译出错的现象！！！
- 
# 四 键盘事件处理机制及其输入框enter键截取内容机制
头文件keywidgets.h
**[cpp]**[view
 plain](http://blog.csdn.net/qianqin_2014/article/details/51234752#)[copy](http://blog.csdn.net/qianqin_2014/article/details/51234752#)
- #ifndef KEYWIDGETS_H
- #define KEYWIDGETS_H
- 
- #include <QWidget>
- #include<QKeyEvent>
- #include<QLineEdit>
- 
- class keywidgets : public QWidget  
- {  
-     Q_OBJECT  
- public:  
- explicit keywidgets(QWidget *parent = 0);  
- 
- void keyPressEvent(QKeyEvent *ev);  
- void keyReleaseEvent(QKeyEvent *ev);  
- 
-     QLineEdit *edit = new QLineEdit(this);  
- signals:  
- 
- public slots:  
- };  
- 
- #endif // KEYWIDGETS_H
源文件：
**[cpp]**[view
 plain](http://blog.csdn.net/qianqin_2014/article/details/51234752#)[copy](http://blog.csdn.net/qianqin_2014/article/details/51234752#)
- #include "keywidgets.h"
- #include<QApplication>
- #include<QDebug>
- 
- keywidgets::keywidgets(QWidget *parent) : QWidget(parent)  
- {  
-     edit->setPlaceholderText("Input:");  
- //截取enter
-     connect(edit, &QLineEdit::returnPressed, [this](){  
-         qDebug()<<this->edit->text();  
-         edit->setText("");<span style="white-space:pre">   </span>//之后将输入框内容置为空
-     });  
- 
- }  
- 
- void keywidgets::keyPressEvent(QKeyEvent *ev)  
- {  
- //打印键值
- int n = ev->key();  
-     qDebug()<<n;<span style="white-space:pre">      </span>//打印按键对应的数字
-     qDebug()<<(char)n;<span style="white-space:pre">        </span>//打印ASCII
- }  
- void keywidgets::keyReleaseEvent(QKeyEvent *ev)  
- {}  
- 
- int main(int argc, char *argv[])  
- {  
-     QApplication app(argc, argv);  
- 
-     keywidgets w;  
-     w.show();  
- 
-     app.exec();  
- }  
# 五 消息处理机制与QApplication中的notify通知功能
头文件MyWidget.h
**[cpp]**[view
 plain](http://blog.csdn.net/qianqin_2014/article/details/51234752#)[copy](http://blog.csdn.net/qianqin_2014/article/details/51234752#)
- #ifndef MYWIDGET_H
- #define MYWIDGET_H
- 
- #include <QWidget>
- 
- class MyWidget : public QWidget  
- {  
-     Q_OBJECT  
- public:  
- explicit MyWidget(QWidget *parent = 0);  
- 
- bool event(QEvent *);  
- 
- signals:  
- 
- public slots:  
- 
- };  
- 
- #endif // MYWIDGET_H
头文件MyApplication.h
**[cpp]**[view
 plain](http://blog.csdn.net/qianqin_2014/article/details/51234752#)[copy](http://blog.csdn.net/qianqin_2014/article/details/51234752#)
- #ifndef MYAPPLICATION_H
- #define MYAPPLICATION_H
- 
- #include <QApplication>
- 
- class MyApplication : public QApplication  
- {  
-     Q_OBJECT  
- public:  
- 
-     MyApplication(int argc, char*argv[]):QApplication(argc, argv)  
-     {}  
- bool notify(QObject *, QEvent *);  
- 
- signals:  
- 
- public slots:  
- 
- };  
- 
- #endif // MYAPPLICATION_H
源文件MyWidget.cpp
**[cpp]**[view
 plain](http://blog.csdn.net/qianqin_2014/article/details/51234752#)[copy](http://blog.csdn.net/qianqin_2014/article/details/51234752#)
- #include "MyWidget.h"
- #include <QPushButton>
- #include <QEvent>
- #include "MyApplication.h"
- #include <QDebug>
- #include <QApplication>
- MyWidget::MyWidget(QWidget *parent) :  
-     QWidget(parent)  
- {  
- 
- }  
- 
- 
- 
- bool MyWidget::event(QEvent *e)  
- {  
- if(e->type() == QEvent::User)  
-     {  
-         qDebug() << "User event is comming";  
-     }  
- return QWidget::event(e);  
- }  
- 
- 
- int main(int argc, char* argv[])  
- {  
-     MyApplication app(argc, argv);  
- 
-     MyWidget w;  
-     w.show();  
- 
- // 发送一个Event给MyWidget
-     qDebug() << "begin send";  
- //通过postEvent发送的消息需要等待处理
-     app.postEvent(&w, new QEvent(QEvent::User));  
-     qDebug() << "end send";  
- //通过sendEvent发送的消息需要及时处理，不建议使用
- // app.sendEvent(&w, )
- 
- return app.exec();  
- }  
源文件MyApplication.cpp
**[cpp]**[view
 plain](http://blog.csdn.net/qianqin_2014/article/details/51234752#)[copy](http://blog.csdn.net/qianqin_2014/article/details/51234752#)
- #include "MyApplication.h"
- #include <QEvent>
- 
- #include <QDebug>
- bool MyApplication::notify(QObject *o, QEvent *e)  
- {  
**[cpp]**[view
 plain](http://blog.csdn.net/qianqin_2014/article/details/51234752#)[copy](http://blog.csdn.net/qianqin_2014/article/details/51234752#)
- //获取最上层窗口
- if(this->topLevelWidgets().count()>0)<span style="white-space:pre"> </span>//不加判断会造成程序崩溃，因为即使退出也会运行
-     {  
-         QWidget* mainWnd = this->topLevelWidgets().at(0);  
- if(o==(QObject*)mainWnd && e->type() == QEvent::MouseButtonPress)  
-         {  
- // do ...
-             qDebug() << "mainwnd is clicked";  
-         }  
-     }  
- 
- return QApplication::notify(o, e);  
- }  
运行结果：
**[cpp]**[view
 plain](http://blog.csdn.net/qianqin_2014/article/details/51234752#)[copy](http://blog.csdn.net/qianqin_2014/article/details/51234752#)
- begin send  
- end send  
- User event is comming  
- mainwnd is clicked  
- mainwnd is clicked  
- mainwnd is clicked  
- mainwnd is clicked  
首先，打印消息处理信息
然后，点击主页面会打印mainwnd is clicked
# 六 窗口关闭前处理函数和重绘函数
头文件mousewidget.h
**[cpp]**[view
 plain](http://blog.csdn.net/qianqin_2014/article/details/51234752#)[copy](http://blog.csdn.net/qianqin_2014/article/details/51234752#)
- #ifndef MOUSEWIDGET_H
- #define MOUSEWIDGET_H
- 
- #include <QWidget>
- #include<QEvent>
- #include<QPainter>
- 
- class mouseWidget : public QWidget  
- {  
-     Q_OBJECT  
- public:  
- explicit mouseWidget(QWidget *parent = 0);  
- 
- //关闭窗口前要处理的函数
- void closeEvent(QCloseEvent *e);  
- 
- //重绘消息，当窗口需要重新画的时候进行重绘
- void paintEvent(QPaintEvent *);  
- 
- signals:  
- 
- public slots:  
- };  
- 
- #endif // MOUSEWIDGET_H
源文件mousewidget.cpp
**[cpp]**[view
 plain](http://blog.csdn.net/qianqin_2014/article/details/51234752#)[copy](http://blog.csdn.net/qianqin_2014/article/details/51234752#)
- #include "mousewidget.h"
- #include<QApplication>
- #include<QDebug>
- 
- mouseWidget::mouseWidget(QWidget *parent) : QWidget(parent)  
- {  
- 
- }  
- 
- //关闭窗口时要处理的阐述
- void mouseWidget::closeEvent(QCloseEvent *e)  
- {  
-     qDebug()<<"before closeWindow, what you should do?";  
- }  
- 
- void mouseWidget::paintEvent(QPaintEvent *e)  
- {  
-     QPainter p(this);  
-     p.drawLine(QPoint(0, 0), QPoint(100, 100));  
- }  
- 
- int main(int argc, char *argv[])  
- {  
-     QApplication app(argc, argv);  
- 
-     mouseWidget w;  
-     w.show();  
- 
-     app.exec();  
- }  
![](https://img-blog.csdn.net/20160424215741689)

