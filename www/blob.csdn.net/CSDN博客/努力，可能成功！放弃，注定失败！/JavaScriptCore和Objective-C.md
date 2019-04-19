# JavaScriptCore和Objective-C - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年05月11日 16:57:49[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：7475
在iOS开发中，因为H5页面的一些先天优势，原生界面里面掺杂着H5页面是一种很常见的方案。公司应用最近因为业务需要一下子接入了大量H5界面，另外还要求：原生界面使用的是友盟统计分析，为了统计数据能在平台连续、集中的展示出来，希望H5页面的统计事件和原生界面的统计事件都上报到同一个后台。为了满足这个要求，就需要H5页面使用友盟统计的iOS SDK来上报用户事件，也就是说，H5页面需要与原生应用进行交互。
本来想从头到尾把了解的方方面面都写一下，但是后来在网上发现有很多优秀的博客，所以就没必要了，这里简单做一个归纳。
但是这些博客也存在一个共同的问题，就是几乎对交互过程中存在的问题和限制鲜有描述，所以我想本文略有价值的地方在于第二部分。
#### 一、JavaScript和Objective-C的交互
交互实际上就是方法的互相调用，所以分两部分。
##### (一)、JS调用OC代码
###### 1、拦截协议
JS调用OC代码可以通过拦截NSRequest请求来调用原生方法进行交互。
UIWebView的代理方法
`- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType;`
UIWebView每次加载请求内容之前，都会调用这个方法，该方法返回YES/NO来决定UIWebView是否加载request请求。所以我们可以通过URL的协议头甚至URL字符串来区别正常的URL请求和本地方法的调用请求。JS传递给OC的参数可以通过URL带过来，如果参数内容过长可以通过post请求来传递，本地在拦截request后，可以将HTTPBody中的请求内容解析出来。
iOS6及以前，拦截协议是JS调用OC方法唯一的出路，即使出现了一些第三方框架（比如WebViewJavaScriptBridge），也是基于拦截协议进行的封装。
iOS7及之后，拦截协议的方法仍然可用，但是苹果给我提供了更友好、完善的方案。
###### 2、使用JavaScriptCore
JavaScriptSore是苹果在iOS7之后提供的一套框架，它让JS与OC的交互更加简单方便。
要使用JavaScriptCore首先我们需要引入它的头文件#import <JavaScriptCore/JavaScriptCore.h>
重要对象：
```
#import "JSContext.h"
#import "JSValue.h"
#import "JSManagedValue.h"
#import "JSVirtualMachine.h"
#import "JSExport.h"
```
- JSContext是JavaScript的运行环境，他主要作用是执行JS代码和注册OC方法接口，相当于HTML中< JavaScript ></JavaScript >之间的内容。
- JSValue是JSContext的返回结果，他对数据类型进行了封装，并且为JS和OC的数据类型之间的转换提供了方法。
- JSManagedValue是JSValue的封装，用它可以解决JS和原生代码之间循环引用的问题。
- JSVirtualMachine 管理JS运行时和管理JS暴露的OC对象的内存。
- JSExport是一个协议，通过实现它可以把一个OC对象暴漏给JS，这样JS就可以调用这个对象暴露的方法。
发现一个写得很好的博客，做一次大自然的搬运工，更详细的内容请参考 [[iOS JavaScriptCore使用]](http://liuyanwei.jumppo.com/2016/04/03/iOS-JavaScriptCore.html)
##### (二)、OC调用JS代码
###### 1、使用UIWebView的方法
`- (nullable NSString *)stringByEvaluatingJavaScriptFromString:(NSString *)script`
###### 2、 使用JavaScriptCore中JSContext的方法
```
- (JSValue *)evaluateScript:(NSString *)script;
- (JSValue *)evaluateScript:(NSString *)script withSourceURL:(NSURL *)sourceURL
```
具体使用可参考 [[iOS JavaScriptCore使用]](http://liuyanwei.jumppo.com/2016/04/03/iOS-JavaScriptCore.html)
#### 二、使用JavaScriptCore遇到的坑
###### 1、内存泄漏问题
当使用JSExport协议的方式来实现交互时，我们可能会在我们的交互对象中声明了一个JSContext属性用来保存JS上下文，代码可能通常这样
```
//声明属性
@property (nonatomic,strong) JSContext * context;
```
```
//使用
self.context = [webView valueForKeyPath:@"documentView.webView.mainFrame.javaScriptContext"];
//或
self.context = [[JSContext alloc] init];
```
在注入JS的时候当前JSContext上下文引用了当前交互对象self，从而造成循环引用。
###### 解决方法
- 使用Block进行，不使用JSExport协议。
- 在交互对象与JSContext之间加一层代理。(处理过NSTimer循环引用问题的同学应该熟悉这个方案)
###### 2、UIWebView加载第一个页面JS调用本地方法正常，但是页面发生了跳转后，JS调用本地方法就失效了
我们在代码中注入JS代码可能像这样
```
//在- (void)viewDidLoad中注入
- (void)viewDidLoad {
    [super viewDidLoad];
    NSURL *url = [NSURL URLWithString:urlString];
    NSURLRequest *request = [NSURLRequest requestWithURL:url];
    [self.webView loadRequest:request];
    //1、使用block注入
    JSContext *context = [self.webView valueForKeyPath:@"documentView.webView.mainFrame.javaScriptContext"];
    context[@"stat"] = ^(NSString *event){
    };
    //2、使用JSExport协议的方式注入一个对象
    Myobj *obj = [[Myobj alloc] init];
    self.jsContext[@"obj"] = obj;
}
```
或者这样
```
//在- (void)webViewDidFinishLoad:(UIWebView *)webView中注入
- (void)webViewDidFinishLoad:(UIWebView *)webView
{
     //1、使用block注入
    JSContext *context = [self.webView valueForKeyPath:@"documentView.webView.mainFrame.javaScriptContext"];
    context[@"stat"] = ^(NSString *event){
    };
    //2、使用JSExport协议的方式注入一个对象
    Myobj *obj = [[Myobj alloc] init];
    self.jsContext[@"obj"] = obj;
```
这里我们要讨论的是注入时机的问题。
当我们在`- (void)viewDidLoad`中注入JS代码之后，如果页面发生了重定向，此时web页面的JS已经发生了变化，而`- (void)viewDidLoad`方法只会执行一次，所以不再是之前我们注入过的那些JS了，此时再调用本地方法自然就失效了。
如果我们在`- (void)webViewDidFinishLoad:(UIWebView *)webView`方法中注入JS，看起来貌似可以解决重定向之后调用失效的问题，因为webView每次加载完成后都会回调`- (void)webViewDidFinishLoad:(UIWebView *)webView`，也就是说每次重定向之后，只要页面加载完成，JS代码就会重新被注入。如果JS调用OC方法的时机是在页面加载完成之后，比如点击web界面上的按钮或者由用户手动触发一个事件调用OC代码，这种情况一定是web页面加载完成之后才会发生的，而此时我们已经重新注入了JS，这样一点问题都没有。但是，如果JS调用OC方法的时机刚好发生在页面加载过程中呢？比如web界面加载过程中自动执行一些操作需要调用OC代码，而此时
```
-
 (void)webViewDidFinishLoad:(UIWebView *)webView
```
还没有回调，所以我们的JS代码并没有重新注入，这里仍然会造成失效的问题。至于解决方案，可以看这里
[Why use JavaScriptCore in iOS7 if it can't access a UIWebView's runtime?](http://stackoverflow.com/questions/18920536/why-use-javascriptcore-in-ios7-if-it-cant-access-a-uiwebviews-runtime)，貌似使用了私有API,有被拒的风险啊~！
我们的应用在统计H5页面路径的时候就是属于需要JS自动调用OC方法的情况，当用户进入页面后需要让JS调用OC方法上报一个统计事件，上报这个事件时，仅仅是表示用户进入了这个界面，并不跟用户产生其他任何交互，所以明显不能通过点击一个按钮来触发。为了避开被拒的风险，我是这样做的
```
- (void)webViewDidFinishLoad:(UIWebView *)webView
{
    JSContext *context = [webView valueForKeyPath:@"documentView.webView.mainFrame.javaScriptContext"];
    context[@"stat"] = ^(NSString *event){
        [MobClick event:event];
        DDLogInfo(@"UMAnalytics...%@",event);
    };
    //页面加载完成后，手动触发页面跟踪的统计事件
    [context evaluateScript:[NSString stringWithFormat:@"ios.start()"]];
    };
```
我将注入时机放在了`- (void)webViewDidFinishLoad:(UIWebView *)webView`中，并与前端约定好上报H5页面路径的统计事件不再让JS主动调用OC方法，而是改为由我在页面加载完成后被动触发，见上面最后一行代码。之所以这样，一是避免web页面重定向导致方法失效的问题，二是页面路径的统计事件本来就应该在界面显示完成后再上报，三是只需要知道状态，不需要与用户交互。这里我在本地触发JS调用之后，最终JS还是会调用由我注入的stat()方法，虽然饶了一个弯，但是H5页面统计事件的埋点及其他逻辑就不再在OC中实现了，而是由H5自己去处理，做到让H5像原生界面一样上报统计事件。
iOS8引入了WKWebView代替了UIKit中的UIWebView，至于WKWebView与JavaScript的交互，玩法有比较大的变化，本文就先这样。
文／BobooO（简书作者）
原文链接：http://www.jianshu.com/p/ca5456fb3983
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
