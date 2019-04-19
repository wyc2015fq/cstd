# QT之QWebEngineView - xqhrs232的专栏 - CSDN博客
2018年09月05日 14:28:32[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：158
原文地址::[https://blog.csdn.net/GG_SiMiDa/article/details/77067687](https://blog.csdn.net/GG_SiMiDa/article/details/77067687)
相关文章
1、Qt之QWebEngineView用法总结----[https://blog.csdn.net/asas714627366/article/details/75257863](https://blog.csdn.net/asas714627366/article/details/75257863)
2、Windows下QtCreator使用QWebEngineView----[https://blog.csdn.net/m32692529/article/details/78788337](https://blog.csdn.net/m32692529/article/details/78788337)
3、QWebEngineView的简单使用Demo----[https://blog.csdn.net/u014597198/article/details/52805425](https://blog.csdn.net/u014597198/article/details/52805425)
4、Qt5 QWebEngineView 一些心得----[https://blog.csdn.net/qq_40720736/article/details/79188304](https://blog.csdn.net/qq_40720736/article/details/79188304)
## 从webkit到webengine
自从Qt5.6后引入了QWebEngine，摒弃了原先的QWebkit，这是一款基于chrome浏览器内核引擎，Qt webenginewidgets模块中提供了QWebEngineView这个视图控件来很方便的加载和显示网页，仅需如下几行：
```
QWebEngineView* webview = new QWebEngineView;
webview->load(QUrl("https://www.baidu.com/"));
webview->show();
```
- 1
- 2
- 3
## QWebEngineView跳转问题
这段代码很容易运行起来，但是别高兴早了，接下来你会发现点页面上的链接或者百度到其它页面时无法跳转，这是为什么呢？
因为我们只是一个浏览器引擎，不是像firefox、chrome那样的浏览器，浏览器已经提供了tab页去显示一个新的链接，所以这里我们需要自己决定如何去显示新打开的链接。
有两种方法可以实现 
1、我们发现点击链接时时会收到urlChanged信号的，它附带的参数就是url地址，所以如果你想在本webview视图上显示这个页面就直接在绑定的槽函数里调用load（url）就ok了，如果你想实现向浏览器用另一tab来显示也是可以的，只需另外创建一个QWebEngineView，加载显示就好了，当然你也可以跳出一个对话框来显示，就想新打开一个浏览器窗口一样；
2、第二种方法就是重写createWindow方法，默认返回的是NULL，所以我们看到的效果是无法显示新的链接，因为你没有提供一个QWebEngineView给它嘛，在这个方法里有一个WebWindowType类型参数，
```
enum WebWindowType {
        WebBrowserWindow,
        WebBrowserTab,
        WebDialog
    };
```
- 1
- 2
- 3
- 4
- 5
可以看到就是第一种方法中我们提到的三种显示，本浏览器窗口视图显示，另一tab页显示，跳出新的对话框显示，分别给你想实现的效果就好啦，简单而灵活。实际上js里面的window.open就会触发这个动作，调用到这个方法。
示例代码：
```
class HWebView : public QWebEngineView
{
     Q_OBJECT
public:
    HWebView::HWebView(QWidget *parent)
        : QWebEngineView(parent){
    }
protected:
    virtual QWebEngineView *createWindow(QWebEnginePage::WebWindowType type){
        HWebView* view = new HWebView;
        view->setAttribute(Qt::WA_DeleteOnClose);
        view->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        return view;
    }
};
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
## c++和JS交互
因为Qt自带的元对象模型Meta Object Model，信号与槽机制，使得c++和JS的交互异常简单而优雅，可以说比微软的com组件更加好用。 
这里隆重登场的就是QWebChannel了，它使用了QWebSocket了，在C++端开了一个服务器端口，在JS客户端去连接这个端口，然后利用qt的元对象模式，信号与槽机制进行交换，说的很复杂，实际请看代码，很简单
```
class HWebContext : public QObject{
    Q_OBJECT
signals：
    void sigClicked();
public slots:
    void setAction(int action);
};
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
```
QWebChannel* webchannel = new QWebChannel(m_webview);
m_webview->page()->setWebChannel(webchannel);
m_webContext = new HWebContext;
webchannel->registerObject(QStringLiteral("content"), (QObject*)m_webContext);
```
- 1
- 2
- 3
- 4
首先我们定义一个c++和JS进行交互的对象类，这个类需要继承自QObject，加上Q_OBJECT宏，定义一些信号和槽，这样就可以使用qt的信号与槽机制了.
然后new一个QWebChannel，与QWebEngineView里面的QWebEnginePage绑定起来，用registerObject去注册我们定义的c++和JS进行交互的对象。
这样c++端的工作就完成了，c++端实际上开了一个端口去监听，提供了一个对象供js去使用。
在JS端需要做的工作如下：
```
<script type="text/javascript" src="./qwebchannel.js"></script>
var g_context;
function onClicked(){
}
window.onload=function(){
    new QWebChannel(qt.webChannelTransport, function(channel) {
    g_context= channel.objects.content;
    g_context.sigClicked.connect(onClicked())
    }); 
}
g_context.setAction(0);
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
加载qwebchannel.js，这个文件在qt安装目录下搜索可以找到，然后在窗口加载时利用QWebChannel，实际上是链接到了c++服务器端，获取到我们在c++端注册的对象，content是c++端注册时为对象设置的字符串标识，为这个对象的信号连接一个槽函数（js函数），这样c++端发出信号时就会去调用这个js函数，而js端调用c++槽函数更加简单，直接类似g_context.setAction(0)就可以了。
## 清理缓存、浏览记录、cookie
```
QWebEngineView view;
    view.page()->profile()->setPersistentCookiesPolicy(QWebEngineProfile::NoPersistentCookies);
    view.page()->profile()->clearHttpCache(); // 清理缓存
    view.page()->profile()->clearAllVisitedLinks(); // 清理浏览记录
    QWebEngineCookieStore* pCookie = view.page()->profile()->cookieStore();
    pCookie->deleteAllCookies(); // 清理cookie
    pCookie->deleteSessionCookies(); // 清理会话cookie
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
## 部署
使用了webenginewidgets模块的，除了需要所需的库lib目录外，还有一些目录也需要一并拷贝过来，分别是libexec qml resources translations
此外需要在程序运行目录下写上配置文件qt.conf 
[Paths] 
