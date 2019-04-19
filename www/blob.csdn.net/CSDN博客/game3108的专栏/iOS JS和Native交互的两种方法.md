# iOS: JS和Native交互的两种方法 - game3108的专栏 - CSDN博客
2015年05月15日 14:45:56[game3108](https://me.csdn.net/game3108)阅读数：474
个人分类：[iOS](https://blog.csdn.net/game3108/article/category/2844789)
现在用webbridge做js与native交互，但也要了解下基础原理。
原地址：http://www.hackbase.com/tech/2014-10-09/69336.html
背景:UIWebView: iOS 用来展示 web 端内容的控件。1. 核心方法:- (NSString*)stringByEvaluatingJavaScriptFromString:(NSString *)script;script 就是 JS 代码,返回结果为 js 执行结果。 比如一个 JS function ...
背景:
UIWebView: iOS 用来展示 web 端内容的控件。
1. 核心方法:
```
- (NSString*)stringByEvaluatingJavaScriptFromString:(NSString *)script;
```
script 就是 JS 代码,返回结果为 js 执行结果。 比如一个 JS function 为
```
function testFunction(abc){
  return abc;
};
```
webview 调用此 JS 代码如下:
```
NSString *js = @"testFunction('abc')";
NSString *result = [webView stringByEvaluatingJavaScriptFromString:js];
```
2. 重要回调:
```
- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType;
```
webview 每当需要去加载一个 request 就先回调这个方法,让上层决定是否 加载。一般在这里截获,进行本地的处理。
Native 调用 JS:
本质就一个方法,通过 stringByEvaluatingJavaScriptFromString,都是同步。
下面重点说说JS怎么回调Native：
1.通常方法：js修通过改doucument的loaction或者新建一个看不见的iFrame,修改它的 src,就会触发回调 webView 的 shouldStartLoadWithRequest,参数 request 的 url 就是新赋值的 location 或者 url,上层截获这个 url 的参数,对此分发即可。 这个都是异步调用的。
如 JS function：
```
![复制代码](http://www.hackbase.com/uploadfile/tech/uploadfile/201410/20141009025059392.gif)
    var messagingIframe;
    messagingIframe = document.createElement('iframe');
    messagingIframe.style.display = 'none';
    document.documentElement.appendChild(messagingIframe);
function TestIOSJS(){
        messagingIframe.src = "ios/test/click";
    };
![复制代码](http://www.hackbase.com/uploadfile/tech/uploadfile/201410/20141009025059392.gif)
```
当触发上面的JS时，webview会收到下面的回调：
```
![复制代码](http://www.hackbase.com/uploadfile/tech/uploadfile/201410/20141009025059392.gif)
- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType
{
    NSString *url = request.URL.absoluteString;
    if([url hasSuffix:@"ios/test/click"]){
        //do something you want
        return NO;
    }
    return YES;
}
![复制代码](http://www.hackbase.com/uploadfile/tech/uploadfile/201410/20141009025059392.gif)
```
通过截获这个request的参数就可以做native需要做的事情。
有个开源的代码挺不错的，大家可以看看：https://github.com/marcuswestin/WebViewJavascriptBridge
2.通过XMLHttpRequest:
　　(1) Native子类化一个NSURLProtocol类,并通过[NSURLProtocol registerClass:self];把自己注册。
　　(2) JS function 创建一个 XMLHttpRequest 对象,然后可以设置携带的参数,设置同步或者异步,然后通过 send 发送请求。
```
![复制代码](http://www.hackbase.com/uploadfile/tech/uploadfile/201410/20141009025059392.gif)
    function iOSExec(){
        var execXhr = new XMLHttpRequest();
        execXhr.open('HEAD', "/!test_exec?" + (+new Date()), true); //设置scheme
        var vcHeaderValue = /.*\((.*)\)/.exec(navigator.userAgent)[1];
        execXhr.setRequestHeader('vc', vcHeaderValue);//设置参数等
        execXhr.setRequestHeader('rc', 1);
        // 发起请求
        execXhr.send(null);
    };
![复制代码](http://www.hackbase.com/uploadfile/tech/uploadfile/201410/20141009025059392.gif)
```
　　(3) 因为步骤1已经把自己注册,所以每个客户端的网络请求都会请求这个类 的+(BOOL)canInitWithRequest:(NSURLRequest *)request,让此决定是否需要生成这个request。
```
![复制代码](http://www.hackbase.com/uploadfile/tech/uploadfile/201410/20141009025059392.gif)
@implementation TestURLProtocol
+(void)initProtocol
{
    [NSURLProtocol registerClass:self];
}
+(BOOL)canInitWithRequest:(NSURLRequest *)request{
    NSString *url = request.URL.absoluteString;
    if([url containsString:@"!test_exec"]){
        //do something
    }
    return NO;
}
![复制代码](http://www.hackbase.com/uploadfile/tech/uploadfile/201410/20141009025059392.gif)
```
　　(4) 通过获取这个request的参数,上层可以进行拦截,然后进行本地的相 关操作。 
这个方法比较少用，不过能解决JS同步回调Native的方法。
这里也有一个开源库，大家可以看一下：https://github.com/apache/cordova-ios/tree/master/CordovaLib

