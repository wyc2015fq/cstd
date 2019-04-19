# 用JavaScriptCore做android和iOS都兼容的JS-NativeSDK - 月若无涯的专栏 - CSDN博客
置顶2015年11月26日 11:01:11[月若无涯](https://me.csdn.net/u010124617)阅读数：9518
最近在给公司做一个JS-Native的SDK，就是用于JS和原生之间的交互。 
使用场景上主要还是webView，那么原先的url拦截的方式已经不再考虑，我们使用了iOS7之后的JavaScriptCore.framework。
总体来说，苹果的JavaScriptCore的API是非常易用的，主要操作步骤如下： 
1.获取一个JSContext。
```bash
_jsContext = [_webView valueForKeyPath:
@"documentView.webView.mainFrame.javaScriptContext"];
```
2.处理某个JS端调起的方法，比如log
```
_jsContext[@"log"] = ^() {
        NSArray *args = [JSContext currentArguments];
        for (id obj in args) {
            //需要注意这里的obj都还是JSValue
            NSLog(@"%@",obj);
        }
    };
```
3.调用JS端执行某个JS方法
```
[_jsContext 
evaluateScript:@"log('arg1')"];
[_jsContext 
evaluateScript:@"logCallback('arg1')"];
```
4.重定义某个JS端的方法(其实和3是一种，不过特殊了点)
```
[_jsContext evaluateScript:@"checkAPI = function(){\
     return [\
     'selectImage',\
     'startRecord',\
     'login',\
     ];\
     }"];
```
掌握了以上4步，基本上在iOS端和JS的交互就完全OK了。 
接着我们开始和android端h5端进行联调。 
到这里，我们发现了一个问题： 
android端JS方法调用原生方法时，需要指定interfaceName，所以他们的JS调用方式是这样的：
`xxname.log('arg1');`
前面多出来了xxname，这种方式下，我们直接使用JSContext进行监听处理是不行的
```
//这个不能被执行到
_jsContext[@"xxname.log"] = ^() {
        NSArray *args = [JSContext currentArguments];
        for (id obj in args) {
            //需要注意这里的obj都还是JSValue
            NSLog(@"%@",obj);
        }
    };
```
那么怎么办呢？胜利在望的时候卡住了？难道要让h5那边调用的时候按平台区分么？如果是iOS平台就调用log(‘xx’)，android就调用xxname.log(‘xx’)??
还好，我们这么犀利怎么会解决不了这样的问题。 
这里我给大家介绍两种方法解决：
**方案一：**
使用JSExport，达到和android类似处理效果 
步骤如下： 
1.定义WebViewJSHelperProtocol协议实线JSExport协议 
并在协议中定义出所有JS端要调用的方法
```
@protocol WebViewJSHelperProtocol<JSExport>
-(void)log:(NSString*)arg;
@end
```
2.定义类实现该协议
```
@interface WebViewJSHelper()<WebViewJSHelperProtocol>
@end
@implementation WebViewJSHelper
-(void)log:(NSString*)arg{
    NSLog(@"%@",arg);
}
@end
```
3.注册该类的实例到JSContext
```bash
WebViewJSHelper *_webViewJSHelper = [[WebViewJSHelper alloc]init];
_jsContext[@"xxname"] = _webViewJSHelper;
```
到此，JS调用xxname.log方法时，会进入我们iOS端定义的log方法中执行。 
关于JSExport还有一些定义别名之类的，感兴趣的同学可以自行看API或网上文档。 
主要注意的是，这里不要出现_jsContext[@”xxname”] = self;这样的代码，因为会导致self实例被JS端持有而内存不释放的问题。
**方案二：**
这个方案的操作方式就简单多了，对比方案一，无需定义实现的原生方法，操作如下： 
1.随便定义一个类加上JSExport协议
```
@interface WebViewJSExport : NSObject<JSExport>
@end
@implementation WebViewJSExport
@end
```
2.注册给JSContext
`_jsContext[@"xxname"] = [WebViewJSExport new];`
3.处理某个JS端调起的方法
```
//定义不含xxname的正常js方法
_jsContext[@"log"] = ^() {
        NSArray *args = [JSContext currentArguments];
        for (id obj in args) {
            //需要注意这里的obj都还是JSValue
            NSLog(@"%@",obj);
        }
    };
//重定义xxname.log方法
[_jsContext evaluateScript:@"xxname.log = log"];
```
好了，现在整套功能都可以使用了。
上面方案二的方式个人觉得优点更多一些，首先更贴近JS习惯，其次不需要定义那么多的方法，而且对于js端调用来说，不定参数个数也更容易些。
这篇文章的最后，讲一点小的注意事项： 
1. 使用 NSArray *args = [JSContext currentArguments];得到的args，内部都是JSValue，千万注意这一点，如果想要使用，最好转成相应的类型，比如[obj toString]之类的，具体可以看JSValue的头文件 
2. 要注意循环引用的问题，不要写出 _jsContext[@”xxname”] = self; 这样的代码来，其他的就是block中的了，老生常谈不再细说。 
3. 要注意JS端和原生端生命周期的问题 
两端的内存管理方式不一样，JS端是GC，我们是引用计数，一般情况不会有生命周期的问题，但是必要时，需要使用JSVirtualMachine包装一下，这块我也还在学习，希望大家注意。
