# VS2017中添加QWebEngineView模块 - xqhrs232的专栏 - CSDN博客
2018年09月05日 11:58:32[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：107
原文地址::[https://blog.csdn.net/qing666888/article/details/76796011](https://blog.csdn.net/qing666888/article/details/76796011)
开发环境: Qt 5.9.0 VS2017
VS2017 在生成新的工程时，会默认带了一些模块。
如果添加新的模块如何操作呢？
目前就添加 [QWebEngineView] 模块为例。
在生成新的工程时默认带的模块，里面没有QWebEngineWidget模块，所以需要自己手动配置：
图一
![](https://img-blog.csdn.net/20170806233532675?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcWluZzY2Njg4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
第一步:
代码中添加 [QWebEngineView] 代码 然后编译
代码1
**[cpp]**[view plain](http://blog.csdn.net/chinahaerbin/article/details/52537546#)[copy](http://blog.csdn.net/chinahaerbin/article/details/52537546#)
- #include <QtWidgets/QMainWindow>  
- #include "ui_qttest001.h"  
- #include <QWebEngineView>  
- 
- class QtTest001 : public QMainWindow  
- {  
-     Q_OBJECT  
- 
- public:  
-     QtTest001(QWidget *parent = 0);  
-     ~QtTest001();  
- 
-     void resizeEvent(QResizeEvent*);  
- 
- private:  
-     Ui::QtTest001Class ui;  
-     QWebEngineView* view;  
- };  
- 
- #endif // QTTEST001_H  
代码2
**[cpp]**[view plain](http://blog.csdn.net/chinahaerbin/article/details/52537546#)[copy](http://blog.csdn.net/chinahaerbin/article/details/52537546#)
- #include "qttest001.h"  
- 
- QtTest001::QtTest001(QWidget *parent)  
-     : QMainWindow(parent)  
- {  
-     ui.setupUi(this);  
- 
-     view = new QWebEngineView(this);  
-     view->load(QUrl("https://www.baidu.com/"));  
-     view->show();  
- }  
- 
- QtTest001::~QtTest001()  
- {  
- 
- }  
- 
- void QtTest001::resizeEvent(QResizeEvent*)  
- {  
-     view->resize(this->size());  
- }  
当编译出现错误:
如图2
![](https://img-blog.csdn.net/20160914140840531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
此问题是没添加 WebEngineView 文件路径
$(QTDIR)\include\QtWebEngineWidgets
按照下图的步骤添加WebEngineView 就可以啦
图3
![](https://img-blog.csdn.net/20160914140845844?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
图4
![](https://img-blog.csdn.net/20160914140851613?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
第二步: 添加lib 库文件
Qt5WebEngined.lib
Qt5WebEngineWidgetsd.lib
图5
![](https://img-blog.csdn.net/20160914140856422?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后编译运行。
![](https://img-blog.csdn.net/20160914140902379?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
