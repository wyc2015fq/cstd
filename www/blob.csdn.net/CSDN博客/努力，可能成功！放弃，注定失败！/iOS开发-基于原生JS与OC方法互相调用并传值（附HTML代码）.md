# iOS开发-基于原生JS与OC方法互相调用并传值（附HTML代码） - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年06月14日 19:54:39[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：15121
最近项目里面有有个商品活动界面，要与web端传值，将用户在网页点击的商品id 传给客户端，也就是js交互，其实再说明白一点就是方法的互相调用而已。
本文叙述下如何进行原生的JavaScript交互
本文包括**JS调用OC方法并传值，OC调用JS方法并传值**
本来想把html放进服务器里面，然后访问，但是觉得如果html在本地加载更有助于理解，特把html放进项目里
## HTML代码
```xml
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
</head>
<body>
<div style="margin-top: 20px">
<h2>JS与OC交互</h2>
<input type="button" value="唤起本地方法(call)" onclick="tianbai.call()">
</div>
<div>
<input type="button" value="唤起getCall:(NSString *)callString传值" onclick="call()">
</div>
<script>
var call = function()
{
    var callInfo = JSON.stringify({"jianshu": "http://www.jianshu.com/users/55c8fdc3c6e7/latest_articles"});
        tianbai.getCall(callInfo);
}
var Callback = function(str)
{
    alert(str);
}
var alerCallback = function()
{
    alert('成功');
}
</script>
</body>
</html>
```
上面html的代码：建立了两个button
第一个button绑定了 `tianbai.call()` 方法，这里 `tianbai` 是一个对象，这个对象的作用下面OC代码中会说明，
`tianbai.call()` 代表 `tianbai` 对象调用 `call()` 方法
第二个button绑定了 `call()` 的方法，调用的是下面JavaScript中的 `call()` 方法，在 JavaScript 的
`call()` 里面，定义一个 `callInfo` 参数，方法中 `tianbai.getCall(callInfo)` 代表
`tianbai` 对象调用 `getCall` 方法并传参数 `callInfo` ，下面两个方法是OC调用JavaScript方法，其中Callback传回str，alerCallback为OC仅调用JavaScript方法！
## OC代码
demo采用原生的JavaScriptCore类
引入三个名词：
- JSContext：给JavaScript提供运行的上下文环境
- JSValue：JavaScript和Objective-C数据和方法的桥梁
- JSExport：这是一个协议，如果采用协议的方法交互，自己定义的协议必须遵守此协议
ViewController.h中的代码(代码过长，方法说明都在注释里)
```
#import <UIKit/UIKit.h>
//导入头文件
#import <JavaScriptCore/JavaScriptCore.h>
@protocol JSObjcDelegate <JSExport>
//tianbai对象调用的JavaScript方法，必须声明！！！
- (void)call;
- (void)getCall:(NSString *)callString;
@end
@interface ViewController : UIViewController<UIWebViewDelegate,JSObjcDelegate>
@property (nonatomic, strong) JSContext *jsContext;
@property (strong, nonatomic)  UIWebView *webView;
@end
```
ViewController.m中的代码(代码过长，方法说明都在注释里)
JavaScriptCore中web页面调用原生应用的方法可以用Delegate或Block两种方法，此文以按Delegate讲解。
设置webView
```
self.webView = [[UIWebView alloc]initWithFrame:CGRectMake(0, 20, [UIScreen mainScreen].bounds.size.width, [UIScreen mainScreen].bounds.size.height)];
    self.webView.delegate = self;
    //从本地加载html文件
    NSString* path = [[NSBundle mainBundle] pathForResource:@"index" ofType:@"html"];
    NSURL* url = [NSURL fileURLWithPath:path];
    NSURLRequest* request = [NSURLRequest requestWithURL:url] ;
    [self.webView loadRequest:request];
    [self.view addSubview:self.webView];
```
JavaScript的tianbai是一个对象，充当原生应用和web页面之间的一个桥梁。用来调用方法
webview加载完成调用代理
```
- (void)webViewDidFinishLoad:(UIWebView *)webView {
      // 设置javaScriptContext上下文
    self.jsContext = [webView valueForKeyPath:@"documentView.webView.mainFrame.javaScriptContext"];
    //将tianbai对象指向自身
    self.jsContext[@"tianbai"] = self;
    self.jsContext.exceptionHandler = ^(JSContext *context, JSValue *exceptionValue) {
        context.exception = exceptionValue;
        NSLog(@"异常信息：%@", exceptionValue);
    };
}
```
将对象指向自身后，如果调用 `tianbai.call()` 会响应下面的方法，OC方法中调用js中的Callback方法，并传值
```
- (void)call{
    NSLog(@"call");
    // 之后在回调JavaScript的方法Callback把内容传出去
    JSValue *Callback = self.jsContext[@"Callback"];
    //传值给web端
    [Callback callWithArguments:@[@"唤起本地OC回调完成"]];
}
```
将对象指向自身后，如果调用 `tianbai.getCall(callInfo)` 会响应下面的方法，OC方法中仅调用JavaScript中的alerCallback方法
```
- (void)getCall:(NSString *)callString{
    NSLog(@"Get:%@", callString);
    // 成功回调JavaScript的方法Callback
    JSValue *Callback = self.jsContext[@"alerCallback"];
    [Callback callWithArguments:nil];
}
```
将对象指向自身后，还可以向html注入js
```
- (void)alert{
    // 直接添加提示框
    NSString *str = @"alert('OC添加JS提示成功')";
    [self.jsContext evaluateScript:str];
}
```
Demo地址：[点击下载](https://github.com/QuTianbai/JavaScript)
文／TianBai（简书作者）
原文链接：http://www.jianshu.com/p/fd378c6d70c0
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
