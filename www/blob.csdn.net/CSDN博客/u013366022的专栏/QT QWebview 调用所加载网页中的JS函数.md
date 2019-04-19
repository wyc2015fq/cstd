# QT QWebview 调用所加载网页中的JS函数 - u013366022的专栏 - CSDN博客
2015年03月30日 09:48:36[slitaz](https://me.csdn.net/u013366022)阅读数：3173
you should be able to execute your script using [QWebFrame::evaluateJavaScript](http://doc.qt.nokia.com/latest/qwebframe.html#evaluateJavaScript) method.
 See if an example below would work for you:
initializing webview:
```cpp
```
QWebView *view = new QWebView(this->centralWidget());
view->load(QUrl("file:///home//test.html"));
connect(view, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)));
```
```
loadFinished signal handler:
```cpp
```
void MyTestWindow::loadFinished(bool)
{
    QVariant f1result = ((QWebView*)sender())->page()->mainFrame()->evaluateJavaScript("f1('test param')");
    qDebug() << f1result.toString();
}
```
```
test.html:
```cpp
```
<head>
    <script LANGUAGE="JavaScript">
        function f1 (s) 
        {
            alert (s) 
            return "f1 result"
        }
    </script>
</head>
<body>
    test html
</body>
```
```
