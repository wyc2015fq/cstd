# Qt Webengine Widget常见问题 - xqhrs232的专栏 - CSDN博客
2018年09月03日 11:42:38[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：635
原文地址::[https://blog.csdn.net/lee353086/article/details/48734379](https://blog.csdn.net/lee353086/article/details/48734379)
相关文章
1、Qt5.x 编译QWebenginewidgets出现错误Project ERROR: Unknown module(s) in QT: webenginewidgets----[https://blog.csdn.net/lisen9lisen/article/details/79796255](https://blog.csdn.net/lisen9lisen/article/details/79796255)
2、初学QT，Project ERROR: Unknown module(s) in QT: webkit， 这个编译错误怎么解决? ----[https://bbs.csdn.net/topics/391935955](https://bbs.csdn.net/topics/391935955)
t Webengine Widget常见问题
Author: kagula
LastUpdate: 2015-09-25
Environment:
 [1]Qt5.4、Qt5.5
 [2]Visual studio 2013 Update5
 [3]QT Plugin 1.2.4
注意：
 [1]发现和中文输入法有兼容性问题，感觉需要修改render_widget_host_view_qt_delegate_widget.cpp文件来解决。
** [2]若碰到Cannot open include file: 'QtWebEngineWidgets'的问题**
 则把“C:\Qt\5.4\msvc2013\include\QtWebEngineWidgets”加入到头文件搜索路径中。
 添加下面两个lib文件的链接。
 Qt5WebEngined.lib
 Qt5WebEngineWidgetsd.lib
 这里假设你用的是Qt5.4。
** [3]最简单的调用“Webegine”的示例代码段如下:**
#include <QtWebEngineWidgets>
#include <QSizePolicy>
Erp2013::Erp2013(QWidget *parent)
: QMainWindow(parent)
{
ui.setupUi(this);
view = new QWebEngineView(this);
view->load(QUrl("http://www.baidu.com"));
//setCentralWidget(view);
//view->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
ui.verticalLayout->addWidget(view);
}
** [4]html/js如何同C++代码交互，参考资料[4]**
   但是必须注意以下几点事项：
   [4-1]
   假设我把index.html重命名为index2.html，"qwebchannel.js"文件放在了D:/nginx-1.8.0/html/目录下面。
   如果找不到“qwebchannel.js”文件可以从下面地址下载：http://code.qt.io/cgit/qt/qtwebchannel.git/tree/src/webchannel/qwebchannel.js
   [4-2]修改standalone例子main.cpp的"fromLocalFile"那行代码为下面的样子。
   QUrl url = QUrl::fromLocalFile("D:/nginx-1.8.0/html/index2.html");
   [4-3]下面这段代码是Qt调用系统默认浏览器打开上面的超级链接
    QUrl url = QUrl::fromLocalFile("D:/nginx-1.8.0/html/index2.html");
    url.setQuery(QStringLiteral("webChannelBaseUrl=") + server.serverUrl().toString());//奇怪这行代码没起作用
    QDesktopServices::openUrl(url);
   [4-4]程序跑起来后，在Web浏览器中，你会发现“webChannelBaseUrl”参数没有正确传递。
   你需要把下面的字符串在浏览器中再打一次：
   "file:///D:/nginx-1.8.0/html/index2.html?webChannelBaseUrl=ws://127.0.0.1:12345"
   [4-5]现在你浏览器中的页面应该和你standalone程序相互通讯了。
   [4-6]查看js代码，我把QWebChannel注册时的“dialog”改为“Erp2013”，下面html中的代码
   由
   window.dialog = channel.objects.dialog;
   相应改为了如下：
   window.Erp2013 = channel.objects.Erp2013;
   类似，把html中dialog名改为了Erp2013.
** [5]如何调用js函数?**
  QString code = QString::fromLocal8Bit("alert('调用Java function的测试')");
  view->page()->runJavaScript(code);
**参考资料**
[1]《Qt WebEngine Widgets》
http://doc.qt.io/qt-5/qtwebenginewidgets-index.html
[2]《Qt下载》
http://www.qt.io/download-open-source/
[3]《Qt WebEngine Widgets C++ Classes》
http://doc.qt.io/qt-5/qtwebenginewidgets-module.html
[4]《standalone例子》
http://doc.qt.io/qt-5/qtwebchannel-standalone-example.html
