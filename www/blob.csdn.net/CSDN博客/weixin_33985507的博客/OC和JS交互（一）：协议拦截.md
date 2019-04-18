# OC和JS交互（一）：协议拦截 - weixin_33985507的博客 - CSDN博客
2018年01月02日 11:07:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：11
# 协议拦截
通俗点理解，就是定义好一个协议，JS使用该协议来调用OC/Swift的方法，并且传值给OC/Swift。
大概的格式如下：
> 
协议名称://方法名称?参数1=数值1&参数2=数值2
例如
```
wxx://sendTitle?title=标题&name=测试
```
其中`wxx`为约定好的协议名称，`sendTitle`为约定好的`OC/Swift`的方法名称，`title=标题&name=测试`为参数列表。当然，参数列表可以是不同的格式，对应着`OC/Swift`那边的解析方式也就不同。
协议例如的基本格式如下：
```
wxx://sendTitle?title=标题&name=测试
```
或者JSON格式：
```
wxx://sendTitle?{'title':'标题','name':'测试'}
```
当然如果参数很多，可以使用对象转JSON，再拼接进协议：
```
var titleObj = new Object();
titleObj.title = '苹果'
titleObj.name = '标题'
...
var jsonStr = JSON.stringify(titleObj);
"wxx://sendTitle?".concat(jsonStr)
```
既然有了协议，我们就可以使用协议。
假如以上拼接好的协议是叫`protocol`
可以这样使用：
```
location.href = protocol;
```
或者：
```
window.location.href = protocol;
```
还有就是这种写法（不知道跟前两者有何区别，或者有何优势，对`JavaScript`不太了解，后续再学习下。）：
```
function loadURL(url) {
        var iFrame;
        iFrame = document.createElement("iframe");
        iFrame.setAttribute("src", url);
        iFrame.setAttribute("style", "display:none;");
        iFrame.setAttribute("height", "0px");
        iFrame.setAttribute("width", "0px");
        iFrame.setAttribute("frameborder", "0");
        document.body.appendChild(iFrame);
        // 发起请求后这个iFrame就没用了，所以把它从dom上移除掉
        iFrame.parentNode.removeChild(iFrame);
        iFrame = null;
    }
```
`url`就是传入的协议：
```
loadURL(protocol)
```
# OC端的处理
对于协议的拦截，`UIWebView`和`WKWebView`需要在对应的回调中去拦截：
> 
UIWebView
```
#pragma mark -UIWebViewDelegate
-(BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType{
    NSURL *url = request.URL;
    NSString *scheme = [url scheme];
    NSLog(@"%@",scheme);
    if ([scheme isEqualToString:@"wxx"]) {
        [self handleCustomAction: url];
        return NO;
    }
    return YES;
}
```
> 
WKWebView
```
#pragma mark -WKNavigationDelegate
- (void)webView:(WKWebView *)webView decidePolicyForNavigationAction:(WKNavigationAction *)navigationAction decisionHandler:(void (^)(WKNavigationActionPolicy))decisionHandler{
    
    NSURL *url = navigationAction.request.URL;
    NSString *scheme = [url scheme];
    //拦截wxx协议
    if ([scheme isEqualToString:@"wxx"]) {
        [self handleCustomAction:url];
        decisionHandler(WKNavigationActionPolicyCancel);
        return;
    }
    decisionHandler(WKNavigationActionPolicyAllow);
}
```
两者在写法上有些差异，但是道理是一样的。其中`request.URL`和`navigationAction.request.URL`就是我们在JS中定义的完整协议（包括协议名称，方法名称以及参数，如果有参数的话）。而`[url scheme]`就是协议名称，我们根据协议名称进行拦截：`if ([scheme isEqualToString:@"wxx"])`。而在`handleCustomAction:`方法中，我们可以根据方法名，来分别调用OC的不同方法：
```
- (void)handleCustomAction:(NSURL *)url{
    NSString *host = [url host];
    
    if ([host isEqualToString:@"sendTitle"]) {
        [self sendTitle:url];
    }
}
```
其中`[url host]`可以获取方法名称。最终我们在通过`[url query]`取出参数，做具体解析。
从上面我们可以总结，OC端获取协议、协议名称、方法名称和参数分别如下：
- request.URL（UIWebView）
navigationAction.request.URL（WKWebView）
- [url scheme]
- [url host]
- [url query]
大致如图所示：
![1237662-4c8eb776be1290a3.png](https://upload-images.jianshu.io/upload_images/1237662-4c8eb776be1290a3.png)
协议基本格式.png
> 
**特别注意：协议名称中如果含有大写字母，[url scheme]获取的协议名称都会被转换为小写**
# JS传给OC/Swift含有中文的乱码问题
一般来说，JS调用OC/Swift的方法，基本有以下两种情况：
- 调用OC/Swift中原生的系统功能，如相机，相册。
- 传值给OC/Swift使用。
通过[url query]取出参数，却发现，含有中文的时候，打印出来的string是乱码的，例如这样我的参数是
```
{"title":"标题"}
```
```
NSString *query = url.query;
//转为UTF-8格式的字符串
query = [query stringByRemovingPercentEncoding];
```
而OC端打印出来是
> 
{"title":"æ ‡é¢˜"}
这个问题肯定是转码的问题，既然OC端使用这个`stringByRemovingPercentEncoding`是没错的。那么就是H5页面的问题了，最终发现，需要在`<head>`中增加这样一句，以此指定好使用的字符集：
```
<meta http-equiv="Content-Type" content="text/html;charset=utf-8">
```
`content-Type`用于设定页面显示的字符集。
[有关iOS的解码](https://www.jianshu.com/p/206c2bdcaa04)
下一篇：[OC和JS交互（二）：WKWebView之MessageHandler](https://www.jianshu.com/p/ccc4304ca83a)
