# 使用Google Analytics跟踪捕获JavaScript，AngularJS，jQuery的在线错误和异常 - 深之JohnChen的专栏 - CSDN博客

2015年08月08日 12:08:21[byxdaz](https://me.csdn.net/byxdaz)阅读数：2411



  原文  [http://ourjs.com/detail/54f6b500232227083e00003c](http://ourjs.com/detail/54f6b500232227083e00003c?utm_source=tuicool)

谷歌Analytics（分析）是最流行的网站统计服务。它有多种用途：跟踪访问者的会话活动和事件。

（注* 尽管谷歌在中国无法访问，但它的分析服务中可用的，访问分析管理界面需要借助代理。）

它能够跟踪任何事件。因此我们可以通过谷歌Analytics（分析）发送和跟踪任何数据。其中最重要的一个应用场景就是，为Web开发人员分析网页上可能产生或未发现的错误和异常。

到目前为止，谷歌提出了2种方式跟踪事件方式：
-  经典的ga.js与 以 _gaq.push（...） 接口发送
-  新的通用分析analytics.js，通过 ga（'send'，...） 语法发送。

我们来看一看JavaScript，AngularJS和jQuery的异常跟踪。

### 经典的ga.js

当你正在使用谷歌分析时，它会以下面的方式进行工作：

```java
var _gaq = _gaq || [];
_gaq.push(['_setAccount', 'UA-XXXXXXXX-X']);
_gaq.push(['_trackPageview']);
(function () {
    var ga = document.createElement('script');
    ga.type = 'text/javascript';
    ga.async = true;
    ga.src = ('https:' == document.location.protocol ? '[https://ssl](https://ssl)' : '[http://www']()) + '.google-analytics.com/ga.js';
    var s = document.getElementsByTagName('script')[0];
    s.parentNode.insertBefore(ga, s);
})();
```

### JavaScript错误error

下面的代码将会把所有JS抛出的错误（错误信息和发生错误的时间）信息发送给GA； navigator.userAgent 提供用户浏览器的信息，将有助于简化分析异常发生的原因。

```java
//纯JavaScript错误处理程序
window.addEventListener('error', function (err) {
    var lineAndColumnInfo = err.colno ? ' line:' + err.lineno +', column:'+ err.colno : ' line:' + err.lineno;
    _gaq.push([
        '_trackEvent',
        'JavaScript Error',
        err.message,
        err.filename + lineAndColumnInfo + ' -> ' +  navigator.userAgent,
        0,
        true
    ]);
});
```

### AngularJS异常处理程序

AngularJS通过exceptionHandler捕捉所有未捕获的异常。

```php
// AngularJS错误处理程序
angular.module('loggerApp')
        .config(function ($provide) {
            $provide.decorator("$exceptionHandler", function ($delegate) {
                return function (exception, cause) {
                    $delegate(exception, cause);
                    _gaq.push([
                        '_trackEvent',
                        'AngularJS error',
                        exception.message,
                        exception.stack,
                        0,
                        true
                    ]);
                };
            });
        });
```

### jQuery异常

jQuery提供.error（）方法，它将帮助开发者捕获异常。

```java
// jQuery的错误处理程序（jQuery的API）
jQuery.error = function (message) {
    _gaq.push([
        '_trackEvent',
        'jQuery Error',
        message,
        navigator.userAgent,
        0,
        true
    ]);
}
```

### jQuery的AJAX错误处理程序

每当用一个Ajax请求发生错误时，jQuery就会触发ajaxError事件。

```java
// jQuery的AJAX错误处理程序（jQuery的API）
$(document).ajaxError(function (event, request, settings) {
    _gaq.push([
        '_trackEvent',
        'jQuery Ajax Error',
        settings.url,
        JSON.stringify({
            result: event.result,
            status: request.status,
            statusText: request.statusText,
            crossDomain: settings.crossDomain,
            dataType: settings.dataType
        }),
        0,
        true
    ]);
});
```

### 新版analytics.js

如果您使用的是新牌的API，它可能像下面这样：
(function(i,s,o,g,r,a,m){i['GoogleAnalyticsObject']=r;i[r]=i[r]||function(){
    (i[r].q=i[r].q||[]).push(arguments)},i[r].l=1*new Date();a=s.createElement(o),
        m=s.getElementsByTagName(o)[0];a.async=1;a.src=g;m.parentNode.insertBefore(a,m)
})(window,document,'script','//www.google-analytics.com/analytics.js','ga');

ga('create', 'UA-XXXXXXXX-X', 'auto');
ga('send', 'pageview');

下面是在新版analytics.js代码上，追踪错误异常。

### JavaScript错误

```java
window.addEventListener('error', function (err) {
    var lineAndColumnInfo = err.colno ? ' line:' + err.lineno +', column:'+ err.colno : ' line:' + err.lineno;
    ga(
        'send',
        'event',
        'JavaScript Error',
        err.message,
        err.filename + lineAndColumnInfo + ' -> ' +  navigator.userAgent,
        0,
        true
    );
});
```

### AngularJS异常处理程序

```php
// AngularJS错误处理程序
angular.module('loggerApp')
        .config(function ($provide) {
            $provide.decorator("$exceptionHandler", function ($delegate) {
                return function (exception, cause) {
                    $delegate(exception, cause);
                    ga(
                        'send',
                        'event',
                        'AngularJS error',
                        exception.message,
                        exception.stack,
                        0,
                        true
                    );
                };
            });
        });
```

### jQuery的错误

```java
// jQuery的错误处理程序（jQuery的API）
jQuery.error = function (message) {
    ga(
        'send',
        'event',
        'jQuery Error',
        message,
        navigator.userAgent,
        0,
        true
    );
}
```

### jQuery的AJAX错误处理程序

```java
// jQuery的AJAX错误处理程序（jQuery的API）
$(document).ajaxError(function (event, request, settings) {
    ga(
        'send',
        'event',
        'jQuery Ajax Error',
        settings.url,
        JSON.stringify({
            result: event.result,
            status: request.status,
            statusText: request.statusText,
            crossDomain: settings.crossDomain,
            dataType: settings.dataType
        }),
        0,
        true
    );
});
```

完整的示例如下：

经典版 [http://codepen.io/malyw/pen/myLPeJ](http://codepen.io/malyw/pen/myLPeJ)

最新版 [http://codepen.io/malyw/pen/MYGwzz](http://codepen.io/malyw/pen/MYGwzz)

### 在GA中检验数据

在GA中你可以找到两种主要的事件报告

实时在线事件
![](http://img0.tuicool.com/FN7NJn.png)
普通事件报告
![](http://img1.tuicool.com/2EviEzI.gif)
**原文地址：点此 **

