# QT webkit 各个类之间关系--QWebView-QWebPage - L_Andy的专栏 - CSDN博客

2017年01月18日 11:51:45[卡哥](https://me.csdn.net/L_Andy)阅读数：636


一、QT webkit简介

1.Qt

**Qt**（发音同cute）是一个[跨平台](http://zh.wikipedia.org/zh-hans/%E8%B7%A8%E5%B9%B3%E5%8F%B0)的[C++](http://zh.wikipedia.org/zh-hans/C%2B%2B)应用程式开发框架，有时又被称为[C++](http://zh.wikipedia.org/zh-hans/C%2B%2B)[部件工具箱](http://zh.wikipedia.org/zh-hans/%E9%83%A8%E4%BB%B6%E5%B7%A5%E5%85%B7%E7%AE%B1)。Qt被用在[KDE](http://zh.wikipedia.org/zh-hans/KDE)桌面环境、[Opera](http://zh.wikipedia.org/zh-hans/Opera)、[Google
 Earth](http://zh.wikipedia.org/zh-hans/Google_Earth)、[Skype](http://zh.wikipedia.org/zh-hans/Skype)、[Adobe
 Photoshop Album](http://zh.wikipedia.org/w/index.php?title=Adobe_Photoshop_Album&act%3Cwbr%3Eion=edit&redlink=1)和[VirtualBox](http://zh.wikipedia.org/zh-hans/VirtualBox)的开发中。它是[**挪威**](http://zh.wikipedia.org/zh-hans/%E6%8C%AA%E5%A8%81)[**Qt Software**](http://zh.wikipedia.org/zh-hans/Qt_Software)的产品，2008年6月17日被[芬兰](http://zh.wikipedia.org/zh-hans/%E8%8A%AC%E5%85%B0)的[诺基亚](http://zh.wikipedia.org/zh-hans/%E8%AF%BA%E5%9F%BA%E4%BA%9A)（Nokia**）公司收购**，以增强该公司在[跨平台](http://zh.wikipedia.org/zh-hans/%E8%B7%A8%E5%B9%B3%E5%8F%B0)[软件](http://zh.wikipedia.org/zh-hans/%E8%BD%AF%E4%BB%B6)研发方面的实力。

2.Webkit

**WebKit**是[Mac
 OS X](http://zh.wikipedia.org/zh-cn/Mac_OS_X)v10.3及以上版本所包含的[软件框架](http://zh.wikipedia.org/w/index.php?title=%E8%BD%AF%E4%BB%B6%E6%A1%86%E6%9E%B6&act%3Cwbr%3Eion=edit&redlink=1)（对v10.2.7及以上版本也可通过[软件更新](http://zh.wikipedia.org/w/index.php?title=%E8%BD%AF%E4%BB%B6%E6%9B%B4%E6%96%B0&act%3Cwbr%3Eion=edit&redlink=1)获取）。同时，WebKit也是[Mac
 OS X](http://zh.wikipedia.org/zh-cn/Mac_OS_X)的[Safari](http://zh.wikipedia.org/zh-cn/Safari)[网页浏览器](http://zh.wikipedia.org/zh-cn/%E7%BD%91%E9%A1%B5%E6%B5%8F%E8%A7%88%E5%99%A8)的基础。WebKit是一个[开源](http://zh.wikipedia.org/zh-cn/%E5%BC%80%E6%BA%90)项目，主要由[KDE](http://zh.wikipedia.org/zh-cn/KDE)的[KHTML](http://zh.wikipedia.org/zh-cn/KHTML)修改而来并且包含了一些来自[苹果公司](http://zh.wikipedia.org/zh-cn/%E8%8B%B9%E6%9E%9C%E5%85%AC%E5%8F%B8)的一些组件。

传统上，WebKit包含一个网页引擎WebCore和一个脚本引擎JavaScriptCore，它们分别对应的是KDE的[KHTML](http://zh.wikipedia.org/zh-cn/KHTML)和[KJS](http://zh.wikipedia.org/zh-cn/KJS)。不过，随着cript
 style="COLOR: rgb(0,102,153); TEXT-DECORATION: none" href="http://zh.wikipedia.org/zh-cn/JavaScript">JavaScript引擎的独立性越来越强，现在WebKit和WebCore已经基本上混用不分（例如[Google
 Chrome](http://zh.wikipedia.org/zh-cn/Google_Chrome)和[Maxthon 3](http://zh.wikipedia.org/zh-cn/Maxthon)采用[V8](http://zh.wikipedia.org/zh-cn/V8)引擎，却仍然宣称自己是WebKit内核）。

3.Qt webkit

简单的说，Qt webkit就是Qt对Apple公司webkit的支持而开发的库，主要包括以下几个类
|**[QWebDatabase](http://jesserei.blog.163.com/qwebdatabase.html)**|Access to HTML 5 databases created with JavaScript|
|----|----|
|**[QWebFrame](http://jesserei.blog.163.com/qwebframe.html)**|Represents a frame in a web page|
|**[QWebHistory](http://jesserei.blog.163.com/qwebhistory.html)**|Represents the history of a QWebPage|
|**[QWebHistoryInterface](http://jesserei.blog.163.com/qwebhistoryinterface.html)**|Interface to implement link history|
|**[QWebHistoryItem](http://jesserei.blog.163.com/qwebhistoryitem.html)**|Represents one item in the history of a QWebPage|
|**[QWebHitTestResult](http://jesserei.blog.163.com/qwebhittestresult.html)**|Information about the web page content after a hit test|
|**[QWebPage](http://jesserei.blog.163.com/qwebpage.html)**|Object to view and edit web documents|
|**[QWebPluginFactory](http://jesserei.blog.163.com/qwebpluginfactory.html)**|Creates plugins to be embedded into web pages|
|**[QWebSecurityOrigin](http://jesserei.blog.163.com/qwebsecurityorigin.html)**|Defines a security boundary for web sites|
|**[QWebSettings](http://jesserei.blog.163.com/qwebsettings.html)**|Object to store the settings used by QWebPage and QWebFrame|
|**[QWebView](http://jesserei.blog.163.com/qwebview.html)**|Widget that is used to view and edit web documents|

二、QWebView Class介绍

**QWebView****类是Qt webkit模块中最常用到的类之一**。它用来创建一个可以view以及edit文档的widget。也就是说，我们可以**通过QWebView来查看和编辑网页**（**编辑网页视具体情况实现**）。同时，QWebView可以包含QWebPage和QWebFrame的对象，它们三者的关系可以表示为：

![](https://img-blog.csdn.net/20170118115359110?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

QWebPage和QWebFrame将会在后面介绍。

三、QWebPage
 Class介绍

QWebPage类和QWebView**类一样，都用于查看和编辑网页**，不同的是，从两者的包含关系上我们可以知道，QWebView可以用于打开多个网页，而里面具体的网页对象就是QWebPage。QWebPage通过mainframe（）方法可以得到More [QWebView](http://jesserei.blog.163.com/qwebview.html)-like
 functions，如**load(), setUrl()和setHtml()。**

四、QWebFrame
 Class介绍

**QWebFrame****可以算是QWebPage的元对象了**，每一个QWebPage至少有一个QWebFrame，它被称作**QWebPage的mainframe**，通过QWebPage::
 mainframe()方法得到。换言之，QWebFrame依附QWebPage存在。通过调**用QWebFrame的page()方法返回它所在的QWebPage对象**。而**zoomFactor()方法则实现了网页内容的缩放。**

# QWebView为何有些网页显示不了呢 官方示例也不行

QWebView为什么有些网页显示不了呢 官方示例也不行
        QWebView *view = new QWebView(this);
        view->load(QUrl(url));
        view->show();
网页是https协议的，使用SSL加密连接了。你的Qt库集成了openssl模块吗？如果没有，要-openssl开关重编QtNetwork库。如果有，连接webView->page()->networkAccessManager()的sslErrors信号，调用QNetworkReply的ignoreSslErrors函数，然后把libeay32.dll和ssleay32.dll两个文件和程序放到一起。


