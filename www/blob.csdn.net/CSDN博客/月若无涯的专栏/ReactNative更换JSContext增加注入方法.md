# ReactNative更换JSContext增加注入方法 - 月若无涯的专栏 - CSDN博客
2016年03月23日 16:42:53[月若无涯](https://me.csdn.net/u010124617)阅读数：2787
对于ReactNative中JS端调用iOS的原生代码，其本质上也是利用JSContext来进行处理的，这点在看起源码后可以得到验证。 
使用ReactNative调用原生方法，基本都是属于原生代码加个方法，桥接后调用起来的思路。 
而本人比较喜欢直接通过JSContext注入方法调用，省去不断添加方法，并且扩展起来容易些。
20160608更新： 
目前最新的ReactNative版本是0.27，方法已经经过了变动，之前的方案也有些需要调整，现在有种更好的方式获取到JSContext： 
阅读新版源码后发现，在RCTJSCExecutor的setUp执行之后可以进行JSContext的获取（开发模式下有个携带JSContext的通知RCTJavaScriptContextCreatedNotification也是从这里发出）。
尝试了两种获的JSContext的方法： 
方法一：不用创建新类，但要使用AOP和KVC技术，AOP用的Aspects库，iOS8及以上可用 
注意：最后发现该方法在iOS7.x有内存上面的问题，当RCTJSCExecutor释放时JSContext会被额外释放一次导致闪退，因此若最低版本支持iOS7请使用方法二
```
[RCTJSCExecutor aspect_hookSelector:@selector(setUp) withOptions:AspectPositionAfter usingBlock:^(id<AspectInfo> aspectInfo){
    [aspectInfo.instance executeBlockOnJavaScriptQueue:^{//所有对JSContext的操作要在JS执行线程中
        JSContext *context = [aspectInfo.instance valueForKeyPath:@"context.context"];
        //这里对context进行其他注入处理,以log为例
        context[@"log"] = ^() {
            NSArray *args = [JSContext currentArguments];
            for (id obj in args) {
                DLog(@"%@",obj);
            }
        };
        [context evaluateScript:@"local.log = log"];
        ...
    }];
} error:nil];
```
方法二：需要2步 
1.生成RCTJSCExecutor的子类（便于使用其原始的各种处理），我这里叫RNJSContextExecutor 
这子类要有一些特殊的初始化处理，如下：
```
@implementation RNJSContextExecutor
RCT_EXPORT_MODULE();
- (void)setUp{//重载setUp方法
    [super setUp];//别忘记父类调用
    __weak typeof(self) selfWeak = self;
    [self executeBlockOnJavaScriptQueue:^(){//所有对JSContext的操作要在JS执行线程中
        JSContext *context = [selfWeak valueForKeyPath:@"context.context"];
        //这里对context进行其他注入处理,以log为例
        context[@"log"] = ^() {
            NSArray *args = [JSContext currentArguments];
            for (id obj in args) {
                DLog(@"%@",obj);
            }
        };
        [context evaluateScript:@"local.log = log"];
        ...
    }];
}
@end
```
2.替换ReactNative执行页面中使用的executorClass
```
RCTRootView *rootView = [[RCTRootView alloc] initWithBridge:bridge moduleName:@"ReactNativeProject" initialProperties:nil];
[rootView.bridge setExecutorClass:[RNJSContextExecutor class]];//更换JS执行器
[rootView.bridge reload];//重新加载，是新的JS执行器更新
```
最后就可以在RN端的JS中使用在客户端注入的JS方法 
在js页面中，直接使用local.log(“test”,”hello world”);就可以调用到第一步中注入的方法。
关于JSContext的一些使用，可以参考我之前的博客：[用JavaScriptCore做android和iOS都兼容的JS-NativeSDK](http://blog.csdn.net/u010124617/article/details/50053069)
另：本文需求的原因也是由于想要使用ReactNative和之前已经基于JSContext做好的部分对接上，避免二次开发。欢迎留言交流。
