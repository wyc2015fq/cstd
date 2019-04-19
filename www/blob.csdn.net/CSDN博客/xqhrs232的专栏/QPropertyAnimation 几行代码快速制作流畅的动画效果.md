# QPropertyAnimation 几行代码快速制作流畅的动画效果 - xqhrs232的专栏 - CSDN博客
2018年01月08日 10:30:04[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：337
原文地址::[http://blog.csdn.net/csnd_ayo/article/details/53790217](http://blog.csdn.net/csnd_ayo/article/details/53790217)
相关文章
1、Qt 动画类 - QPropertyAnimation Class （翻译）----[http://blog.csdn.net/csnd_ayo/article/details/53770611](http://blog.csdn.net/csnd_ayo/article/details/53770611)
2、Qt简介----[http://blog.csdn.net/csnd_ayo/article/details/70170882](http://blog.csdn.net/csnd_ayo/article/details/70170882)
- QPropertyAnimation Class 官方英文文档【[点击前往](http://doc.qt.io/qt-5/QPropertyAnimation.html)】
- QPropertyAnimation Class 中文译文文档【[点击前往](http://blog.csdn.net/csnd_ayo/article/details/53770611)】
# 简介
QPropertyAnimation Class 是一个控制动画效果的类，诞生自 Qt
 4.6 版本。 该类继承自 QVarianAnimation，并支持其它基类相同的动画类，例如：QAnimationGroup 动画组类，该类仅支持继承自 QObject 类的窗口部件。
# 以例代劳
用例子来讲述各个功能，直观，立竿见影。
头文件
[cpp][view
 plain](http://blog.csdn.net/csnd_ayo/article/details/53790217#)[copy](http://blog.csdn.net/csnd_ayo/article/details/53790217#)
- #ifndef MAINWINDOW_H
- #define MAINWINDOW_H
- #include <QMainWindow>
- namespace Ui {  
- class MainWindow;  
- }  
- 
- class MainWindow : public QMainWindow  
- {  
-     Q_OBJECT  
- public:  
- explicit MainWindow(QWidget *parent = 0);  
-     ~MainWindow();  
- private:  
-     Ui::MainWindow *ui;  
- };  
- #endif // MAINWINDOW_H
cpp文件
[cpp][view
 plain](http://blog.csdn.net/csnd_ayo/article/details/53790217#)[copy](http://blog.csdn.net/csnd_ayo/article/details/53790217#)
- #include <QPropertyAnimation>
- #include "mainwindow.h"
- #include "ui_mainwindow.h"
- 
- MainWindow::MainWindow(QWidget *parent) :  
-     QMainWindow(parent),  
-     ui(new Ui::MainWindow)  
- {  
-     ui->setupUi(this);  
- 
- /*  声明动画类，并将控制对象 this (this一定是继承自QObject的窗口部件)  以及属性名 "geometry" 传入构造函数  */
-     QPropertyAnimation* animation = new QPropertyAnimation(this, "geometry");  
- /*  设置动画持续时长为 2 秒钟  */
-     animation->setDuration(2000);  
- /*  设置动画的起始状态 起始点 (1,2)  起始大小 (3,4)  */
-     animation->setStartValue(QRect(1, 2, 3, 4));  
- /*  设置动画的结束状态 结束点 (100,200)  结束大小 (300,400)  */
-     animation->setEndValue(QRect(100, 200, 300, 400));  
- /*  设置动画效果  */
-     animation->setEasingCurve(QEasingCurve::OutInExpo);  
- /*  开始执行动画 QAbstractAnimation::DeleteWhenStopped 动画结束后进行自清理(效果就好像智能指针里的自动delete animation) */
-     animation->start(QAbstractAnimation::DeleteWhenStopped);  
- }  
- 
- MainWindow::~MainWindow()  
- {  
- delete ui;  
- }  
QPropertyAnimation 声明的时候
可以传入的属性分别有  pos(位置)、windowOpacity(透明度)
位置示例
[cpp][view
 plain](http://blog.csdn.net/csnd_ayo/article/details/53790217#)[copy](http://blog.csdn.net/csnd_ayo/article/details/53790217#)
- void OEasyWebNotice::onShow() {  
-     QRect rect = QApplication::desktop()->availableGeometry();  
- constint &endy = rect.height() - height();  
-     QPropertyAnimation *animation= new QPropertyAnimation(this,"pos");  
-     animation->setDuration(2000);  
-     animation->setStartValue(QPoint(rect.width() - width(), rect.height()));  
-     animation->setEndValue(QPoint(rect.width() - width(), endy));  
-     animation->setEasingCurve(QEasingCurve::OutCubic);  
- 
-     connect(animation, SIGNAL(finished()),  
- this, SLOT(animationFinished()));  
-     show();  
-     animation->start(QAbstractAnimation::DeleteWhenStopped);  
- }  
透明度示例
[cpp][view
 plain](http://blog.csdn.net/csnd_ayo/article/details/53790217#)[copy](http://blog.csdn.net/csnd_ayo/article/details/53790217#)
- void OEasyWebNotice::onClose(void) {  
-     disconnect(closeButton_.get(),SIGNAL(clicked()),  
- this, SLOT(onClose()));  
- 
-     QPropertyAnimation* animation = new QPropertyAnimation(this, "windowOpacity");  
-     animation->setDuration(1000);  
-     animation->setStartValue(1);  
-     animation->setEndValue(0);  
-     animation->setEasingCurve(QEasingCurve::InCirc);  
-     connect(animation, SIGNAL(finished()),  
- this, SLOT(deleteLater()));  
-     show();  
-     animation->start(QAbstractAnimation::DeleteWhenStopped);  
- }  
关于 QPropertyAnimation 我为大家推荐一个我写的项目
[国内开源社区](http://git.oschina.net/Mr_ChenLuYong/oeasymessage)[国际开源社区](https://github.com/chenluyong/OEasyMessage)
版权声明：分享得以延续，交流方知彼此。作者：_OE_ 博客地址：http://blog.csdn.net/csnd_ayo
- 
本文已收录于以下专栏：
- [《Qter
 必知必会》](http://blog.csdn.net/column/details/15254.html)

