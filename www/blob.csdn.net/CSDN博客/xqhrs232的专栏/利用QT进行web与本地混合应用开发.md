# 利用QT进行web与本地混合应用开发 - xqhrs232的专栏 - CSDN博客
2018年09月03日 10:22:21[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：267
原文地址::[https://blog.csdn.net/talking12391239/article/details/16857179](https://blog.csdn.net/talking12391239/article/details/16857179)
相关文章
1、[用Qt开发Web和本地混合的应用](https://www.cnblogs.com/findumars/p/5313445.html)----[https://www.cnblogs.com/findumars/p/5313445.html](https://www.cnblogs.com/findumars/p/5313445.html)
2、QTWebEngine的使用----[https://blog.csdn.net/chenhaifeng2016/article/details/54983024/](https://blog.csdn.net/chenhaifeng2016/article/details/54983024/)
原文参见 [http://www.qtsoftware.com/forms/whitepapers/reg-whitepaper-hybrid](http://www.qtsoftware.com/forms/whitepapers/reg-whitepaper-hybrid).
Qt提供了本地 C++对象与JavaScript的无缝集成，是进行本地与web混合应用开发的理想平台。
(一）Qt Webkit 集成
　 　利用Qt的Webkit 集成与QtNetwork模块，你完全可以进行本地桌面与web混合应用开发，你可以自由地混合JavaScript，样式表，Web内容和Qt组件。Webkit是一个非常成熟的web浏览引擎。Qt中集成了这个大名鼎鼎的引擎，通过ＱtWebkit，你可以在Ｃ++ 中执行JavaScript，或者在网页中集成Ｃ++对象，并且通过JavaScript和这些对象进行交互。
　　一个现代的ＨＴＭＬ渲染引擎只 是混合开发的一半，另一半就是本地应用和渲染对象的交互。ＱＴ的Webkit 集成提供了这种解决方案：
　　１.使用object标签嵌入Qt Widgets组件。这可以让使用Ｃ++代码的Qt组件包含在网页中，作为网页的部分外观。
　　２.在JavaScript中访问Ｃ++对象。你 可以在JavaScript环境中插入Ｃ++对象，让网页脚本直接访问你的数据结构。
　　３.在Qt中执行JavaScript。你可以在Ｃ++ 调用网页环境中的JavaScript函数，触发网页事件。
　　４.共享客户端存储。在JavaScript和Ｃ++中你都具有访问数据库的能 力，这样当下线时也能共享大量数据。
(二）与嵌入的Qt对象交互
　　使用 QWebView 组件，有两种方法可以在网页中嵌入C++对象。你可以在网页的JavaScript中添加C++对象，或者也可以创建一个插件，然后在网页中使用 object标签嵌入。
　　第二种方法更容易入手。当在网页中放入 Widget组件时，它的所有public slots就像普通函数一样被网页中的JavaScript函数访问。
    要在网页中添加一个Widget，首先要告诉你的QWebPage对象,该Widget可用,这个通过子类化QWebPlugFactory完成,你需要 重新实现两个方法：plugs和create。plugs方法通知网页该Widget可用，create方法根据请求创建widget。
　　在 HTML网页中，widgets使用object标签创建。比如，下面这个标签试图创建一个 application/x-qt-colorlabel　组件。
<object type="application/x-qt-colorlabel" width="50px" height="20px" id="label" />
　　要利用这种创建，必须要允许使用插件并且要告诉QWebpage插件的工厂类。在下面的代码中，ColorLabelFactory将 会根据application/x-qt-colorlabel的请求创建相应实例。
QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, true); 
webView->page()->setPluginFactory(new 　ColorLabelFactory(this));
　　ColorLabel有一个公开的slot: chagneColor(),这个对于网页中的JavaScript自动可用。在网页中插入一个指向该元素的链接，可以以一种简单的方式激活C++函数。
<a href='javascript:document.getElementById("label").changeColor();'>Change color!</a> 
　　要反方向推进事件，必须要使你的对象在JavaScript文档上下文中可用。要对QWebPage的每一个 QWebFrame，调用addToJavaScriptWindowObject方法。这个方法允许你根据名字把一个对象添加到JavaScipt上下 文中。
webView->page()->mainFrame()->addToJavaScriptWindowObject( "eventSource", new eventSource( this ) );
　　要连接刚添加对象eventSource的信号，要加上一段JavaScript代码，使用evaluateJavaScript方法 完成。下面的代码将把eventSource对象的signalName信号连接到一个JavaScript函数destFunction。
![](http://images.cnblogs.com/cnblogs_com/baizx/082710_0845_QTweb1.gif)webView->page()->mainFrame()->evaluateJavaScript( "eventSource.signalName.connect(destFunction);" );
　　如果你把一个对象添加到一个以标准浏览器查看的JavaScript页面中，有一个信号需要知道。每一次JavaScript 内容被清除，Frame都会释放 javaScriptWindowObjectCleared 信号。为了使你的Qt对象一直可用，你需要连接这个信号，并且在这里调用 addToJavaScriptWindowObject函数。
（二）使用客户端存储共享数据
       随着HTML5，WEB标准日益靠近于桌面，同样，桌面也开始集成WEB。这方面一个最大的变化就是客户端存储。这在客户机上给了每一方（比如每一个页 面）一个能利用SQL的数据库引擎，可以缓存本地数据，减少流量，使页面能脱机使用。还可以用来存储大量的结构化的，可搜索的数据。
      客户端存储可以在JavaScript中使用，从JavaScript代码中搜索数据库，然后从搜索结果中生成页面。这要使用 openDatabase和transaction函数。
假设现在有个数据库, 代码如下所示：
db = openDatabase("TestDb ", "1.0", "Client side storage test", 200000);
db.transaction(function(tx) {
      tx.executeSql("SELECT id, text FROM Texts", [], function(tx, result) {
          for (var i = 0; i < result.rows.length; ++i) {
              var row = result.rows.item(i);
               processText( row['id'], row['text'] );
         }
    },    function(tx, error) {
         alert('Failed to retrieve texts from the database - ' + error.message);
         return;
    });
});
     使用QtWebkit，你可以使用QtSql模块访问同一个数据库。这在混合开发中是一个非常有用的特征。比如，你的应用中的web页面在和本地部分共享 数据时，可以用同样的机制来保存数据。
     为了避免安全问题，客户端的数据库只能由具有正确权限的一方在JavaScript中访问。本地的C++代码也可以通过静态的QWebSecurityOrigin::allOrigins 方法，或者通过QWebFrame::securityOrigin 来访问所有安全对象。
       通过databases方法，可以访问一个 QWebDatabaew方法列表，每一个web数据库对象有一个filename属性，可以用来在本地代码中访问数据库。
QWebDatabase webdb = mySecurityOrigin.databases()[index];
QSqlDatabase sqldb = QSqlDatabase::addDatabase("QSQLITE", "webconnection");
sqldb.setDatabaseName(webdb.fileName());
if (sqldb.open()) {
    QStringList tables = sqldb.tables();
![](http://images.cnblogs.com/cnblogs_com/baizx/082710_0845_QTweb2.gif)
}
         在WEB和本地应用连接事件的能力，再加上共享数据的机制，更易于模糊WEB和桌面之间的界限。
（三）WEB转换
       很多通过WEB得到的数据并不适合直接显示。比如网络新闻, 地理数据，以及其它特定数据格式的应用。QT的网络模块可以以一种很简单的方式下载这样的数据，然后解析数据，把它转换成能被自己的代码处理的合适的格 式。也可以通过QtXmlPatterns模块处理，当输出格式是XML或者想在XHMTL网页中显示时，这样更加方便。
      我们通过一个小例子来大致了解一下这个有意思的部分。我们下载一个新闻源，用XSLT把它从XML转换成XHTML，然后通过QWebPage把它显示出 来。（图略）
      QNetworkAccessManager 类让你很容易处理客户端和WEB服务器之间的交互。它帮你处理诸如代理，保存设置，COOKIE以及SSL会话这些细节。总之，它不但让你在象上传下载这 些常见情况更加容易，还可以帮你处理登录，认证这些复杂会话。
    为了下载我们例子中的新闻源，我们所有要做的就是创建一个QNetworkAccessManager 然后调用它的get方法。结果通过 finished(QNetworkReply*) 信号返回。
{
![](http://images.cnblogs.com/cnblogs_com/baizx/082710_0845_QTweb3.gif)
QNetworkAccessManager *manager = new QNetworkAccessManager( this );
connect( manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleReply(QNetworkReply*)) );
connect( manager, SIGNAL(finished(QNetworkReply*)), m_progressBar, SLOT(hide()) );
QNetworkReply *reply = manager->get( QNetworkRequest( QUrl( feedUrl ) ) );
connect( reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(updateProgress(qint64,qint64)) );
}
附上原文：
# [Qt Features for Hybrid Web/Native Application Development-转载](http://www.cnblogs.com/baizx/archive/2010/08/27/1810255.html)
White Paper
Today's computer users live and work in an interconnected world. They always have a network at
hand and expect their data to be available at all times wherever they are. The boundary between
desktop applications and the web is closing and hybrid applications, mixing the best of both worlds,
are starting to emerge.
With the Qt WebKit Integration and the QtNetwork module, you have all the tools that you need to
create your own hybrid applications by mixing JavaScript, style sheets, web content and Qt compo-
nents freely. Combined with the QtXmlPatterns module, you can download, transform and query XML
and present it in a hybrid web environment. This paper presents the features provided in the Qt
WebKit Integration, and discusses how you can use them to develop hybrid applications.
Qt Features for Hybrid Web/Native
Application Development
Qt Features for Hybrid Web/Native Application Development
Today's computer users live and work in an interconnected world. They always have a network at
hand and expect their data to be available at all times wherever they are. The boundary between
desktop applications and the web is closing and hybrid applications, mixing the best of both worlds,
are starting to emerge.
With the Qt WebKit Integration and the QtNetwork module, you have all the tools that you need to
create your own hybrid applications by mixing JavaScript, style sheets, web content and Qt
components freely. Combined with the QtXmlPatterns module, you can download, transform and
query XML and present it in a hybrid web environment.
Features of the Qt WebKit Integration
One of the key components when mixing web and native applications is the web contents rendering
engine – QtWebKit. This module makes it easy to integrate the wide-spread WebKit engine into
your applications. The integration includes triggering JavaScript from C++, as well as integrating
C++ objects into a web page and interacting with those objects through JavaScript.
The WebKit engine can be seen as a fully fledged web browser engine. The highlights of this engine
are its modern rendering features:
z ACID3 compliance gives you proper standards-compliant rendering.
z CSS-based transformations makes it possible to scale, rotate, skew and translate page
elements through CSS.
z CSS-based animations let you make smooth transitions between different states. For
instance, elements can be made to fade in and out as the mouse cursor hovers over them.
z Support for the video-tag allows for embedding video contents easily. The video player uses
codecs available from the Phonon Multimedia framework (also a part of Qt).
z Full page zooming makes it possible to scale not only the font size, but the entire page
including images.
z NPAPI support to embed standard web browser plugins enabling third party media and
more.
z The SquirrelFish JavaScript engine offers one of the fastest JavaScript experiences
available.
A modern, capable HTML rendering engine is only one half of a hybrid application. The other half
is the interaction between the native application and the rendered contents. The Qt WebKit
integration offers all the pieces of such a puzzle.
z Embed Qt widgets into a web page using the object-tag. This lets your web page contain Qt
widgets along with native C++ code as part of the visual appearance of the page.
z Access Qt objects from the JavaScript. This allows you to insert C++ objects into the
JavaScript context, letting the web page's script interact directly with your data structures.
z Trigger JavaScript from Qt. You can call JavaScript functions in their web page context
from your C++ code and trigger events in the web page.
z Shared client side storage. This gives you access to a database accessible both from
JavaScript and C++, making it possible to share large amounts of data easily even when
offline.
The Qt WebKit integration forms the base of all hybrid applications, but the key factor is getting the
native code and web contents to interact. Qt offers a number of techniques to interconnect the two
worlds, sharing events and data while sharing the screen as one homogeneous interface to the user.
Interacting With Embedded Qt Objects
There are two ways to embed Qt objects into a web page illustrated using the QWebView widget.
You can either add your objects to the JavaScript context of a page, or you can create a plugin that
makes it possible to place Qt widgets inside a web page using the object tag.
The latter is an easy way to start creating a hybrid application: simply put your widgets inside a web
page. When the widgets are in the page, their public slots will be exposed to JavaScript functions of
the page as ordinary functions.
To add a widget to a page, you need to tell your QWebPage object that the widget is available. This is
done by subclassing the QWebPluginFactory class and reimplementing the methods plugins and
create. The plugins method informs the web page of which plugins it makes available, while the
create method creates widgets on demand.
From the HTML making up the web page in question, the widgets are created using the object tag.
For instance, the following tag will attempt to create an instance of an application/x-qt-colorlabel
widget.
<object type="application/x-qt-colorlabel" width="50px" height="20px" id="label"
/>
To facilitate the creation of such a widget, we must enable plugins and tell QWebPage about the
plugin factory class. In the code below, the ColorLabelFactory creates instances of ColorLabel
widgets in in response to application/x-qt-colorlabel requests.
{
...
QWebSettings::globalSettings()->
setAttribute( QWebSettings::PluginsEnabled, true );
webView->page()->setPluginFactory( new ColorLabelFactory( this ) );
...
}
The ColorLabel widget exposes a public slot called changeColor(). This is automatically made
available to JavaScript in the web page. Adding a link to the HTML referring to the element makes
it possible to activate C++ functions in a simple way.
<a href='javascript:document.getElementById("label").changeColor();'>Change color!</a>
To make it possible to push events in the other direction, you need to make your objects available
through the JavaScript document contexts. Using the addToJavaScriptWindowObject method
available from each QWebFrame of your QWebPage. This method allows you to add an object to the
JavaScript context with a name:
webView->page()->mainFrame()->
addToJavaScriptWindowObject( "eventSource", new eventSource( this ) );
To connect a signal from the object you have just added, you need to inject a piece of JavaScript
into the web page context. This is done using the evaluateJavaScript method. The code below
connects the signal signalName from the eventSource object to the JavaScript function
destFunction. You can even pass arguments from the signal to the connected JavaScript function.
webView->page()->mainFrame()->
evaluateJavaScript( "eventSource.signalName.connect(destFunction);" );
If you want to add a Qt object to the JavaScript contents of pages viewed through a standard
browser, there is a signal to be aware of. Each time the JavaScript context is cleared, the frame
emits the javaScriptWindowObjectCleared signal. To ensure that your Qt object is available at
all times, you need to connect to this signal and make your addToJavaScriptWindowObject calls
there.
The Qt WebKit integration lets you create applications where the native C++ interacts with the
JavaScript context in a seamless manner. This is the foundation of powerful hybrid applications, and
Qt makes it easy.
Using the Client Side Storage to Share Data
With HTML 5, the web standard moves closer to the desktop in the same way as the desktop has
started to integrate the web. One of the bigger changes in this direction is the introduction of client
side storage. This gives each origin (i.e. each web page context) a SQL capable database engine on
the client machine. This can be used to cache data locally, to reduce traffic and make the page
available off-line. It can also be used to store large quantities of data in a structured, searchable way.
The client side storage is available from JavaScript. The idea is to search the database from your
JavaScript code and generate the HTML from the search result. This is done using the
openDatabase and transaction functions. Assuming the database is present and filled, the
following snippet shows the idea.
db = openDatabase("TestDb ", "1.0", "Client side storage test", 200000);
db.transaction(function(tx) {
tx.executeSql("SELECT id, text FROM Texts",
[], function(tx, result) {
for (var i = 0; i < result.rows.length; ++i) {
var row = result.rows.item(i);
processText( row['id'], row['text'] );
}
}, function(tx, error) {
alert('Failed to retrieve texts from the database - ' + error.message);
return;
});
});
Using the Qt WebKit integration, you can access the same databases through Qt's SQL module. This
can be a very useful feature in a hybrid application. For example, the web page half of you
application can use exactly the same mechanism for keeping data as when sharing data with your
native application half.
To avoid security breaches, the client side databases are only accessible through JavaScript from the
right security origin. From your native C++ code you can access all security origins through the
static QWebSecurityOrigin::allOrigins method, or via the QWebFrame::securityOrigin
method.
Given an origin object, you can get access to a list of QWebDatabase objects through the databases
method. Each web database object has a fileName property that can be used to open the database
for access from the native code.
QWebDatabase webdb = mySecurityOrigin.databases()[index];
QSqlDatabase sqldb = QSqlDatabase::addDatabase("QSQLITE", "webconnection");
sqldb.setDatabaseName(webdb.fileName());
if (sqldb.open()) {
QStringList tables = sqldb.tables();
...
}
Combining this data sharing mechanism with the ability to connect events between the web and
your native application, makes it easy to blur the border between web and desktop.
Transforming the Web
Much of the data available through the web is not suitable for direct presentation. Examples are
news feeds, geo data and other application specific data formats. Qt's networking module makes it
possible to download such data in an easy manner. Parsing the data and converting it into a suitable
format can then be handled by your custom code or, for instance, by the QtXmlPatterns module.
The latter is handy when the output format is expected to be XML, or if you want to display it in a
web page, XHTML.
In this section we will walk through the interesting parts of a small example downloading a news
feed, converting it from XML to XHTML using an XSL transform, before presenting it through a
QWebView as shown in figure 1.
Figure 1: A news feed from ArsTechnica shown as
XHTML using a QWebView.
The QNetworkAccessManager class allows you to easily handle the interaction between a client
and a web server. It helps you handle details such as proxy and cache settings, but also cookies and
SSL/TSL sessions. All in all, it makes it easy for the average case – to upload or download data --
but allows you to handle complex sessions with logins, certificates, etc..
To download the news feed for our example, all we need to do is create a QNetworkAccessManager
and call its get method. The result is returned through the finished(QNetworkReply*) signal.
However, as you can tell from the source code below, the reply object is available from the moment
you make the request. This allows us to monitor the progress of the request by listening to the
downloadProgress(qint64,qint64) signal.
{
...
QNetworkAccessManager *manager = new QNetworkAccessManager( this );
connect( manager, SIGNAL(finished(QNetworkReply*)),
this, SLOT(handleReply(QNetworkReply*)) );
connect( manager, SIGNAL(finished(QNetworkReply*)),
m_progressBar, SLOT(hide()) );
QNetworkReply *reply = manager->get( QNetworkRequest( QUrl( feedUrl ) ) );
connect( reply, SIGNAL(downloadProgress(qint64, qint64)),
this, SLOT(updateProgress(qint64,qint64)) );
}
When the request is finished, the QNetworkReply object holds the entire reply. This reply needs
further processing before it can be presented to users. In the case of an XML news feed such as RSS
or ATOM, the processing can be done using an XSL transformation. In the case of this
transformation, we use convert each item-node into a XHTML table row, populating the columns
with the contents of the tags pubDate and title. This XSL template is part of a larger file containing
more rules and the rest of the XHTML tags surrounding the table data.
<xsl:template match="item">
<tr>
<td><xsl:value-of select="pubDate"/></td>
<td><xsl:value-of select="title"/></td>
</tr>
</xsl:template>
The XSL query is combined with the reply given from the QNetworkReply object using a
QXmlQuery object. This objects lets us perform XQueries on our XML data, transforming it from
the news feed into XHTML. In the code below, the replyBuffer contains the reply from the
QNetworkReply. The queryFile is a QFile object with the XSL query and the resultBuffer is
a QIODevice wrapping the QByteArray result into an interface suitable for QXmlQuery. The
result is then displayed through the QWebView m_webView using the setHtml method.
{
...
QXmlQuery qry( QXmlQuery::XSLT20 );
qry.setFocus( &replyBuffer );
qry.setQuery( &queryFile );
qry.evaluateTo( &resultBuffer );
m_webView->setHtml( QString(result) );
}
As you can see, Qt gives you all the tools that you need to easily download, process and display
data from the Web. The combination of QNetworkAccessManager, QXmlQuery and QWebView
forms a flexible flow from unformatted XML to data displayed on the screen. If your data is not
XML, you can still use the QXmlQuery by providing a QAbstractXmlNodeModel, modeling non-
XML data into XML nodes.
Summary
By providing the means for seamless integration between native C++ objects and JavaScript, Qt is
the ideal platform for hybrid application development. The interaction between these two worlds is
not limited to sharing data. Just like data, events can be passed both ways too.
Hybrid applications have a number of attractive features. For instance, the user knows the web
metaphor and is used to working with it. As parts of the application is deployed over the web, it can
be updated without having to install anything on each user's machine. Another benefit is the
separation of style from content using style sheets. Creating hybrid applications using Qt takes this
one step further – parts of your application are scripted, other parts consist of native code. The user
interface can comprise of code, generated widgets, HTML or CSS.
The combination of the Qt WebKit integration, QtNetwork and QtXmlPatterns lets you harness the
power of native code and the flexibility of HTML, CSS and JavaScript. These are the tools for
building the next generation of applications.
For more information on the Qt WebKit Integration, visit http://qt.nokia.com./products
or browse the Qt technical documentation at http://doc.trolltech.com/qtwebkit.html.
