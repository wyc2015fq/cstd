# iOS开发 应用启动速度的优化 - 月若无涯的专栏 - CSDN博客
2013年04月18日 14:42:25[月若无涯](https://me.csdn.net/u010124617)阅读数：3997
对于一个应用来说，如果你启动的速度太慢，需要用户等待很长时间，那么可能用户的耐心也在这等待中慢慢消失。
那么如何提升你的应用启动速度呢？
首先，我们要搞清楚从点击图标，直到进入界面期间，你的应用发生了什么，主要的事情有：
1.链接并加载所有的framework和静态库
2.UIKit初始化，包括加载字体、状态栏等等
3.应用程序的callback
所以要做的事情有哪些？
1.不需要的Framework和静态库不要添加
2.初始化UIKit时，UserDefaults、main xib会被初始化，所以，保持你的main xib尽可能小，同时在UserDefaults中不要写入类似图片这种过大的数据。
3.应用程序回调，进入application:willFinishLaunchingWithOptions:以及application:didFinishLaunchingWithOptions:这两个方法后，不要在这里进行过多的初始化及其他耗时的操作，因为你会发现这显著的影响了你程序的启动速度。
对于第3点可以采用的优化方案是使用一个初始化界面，内容精简，可以和启动页类似，这样既可以较快的完成应用的加载进入，又可以把之前需要在application:didFinishLaunchingWithOptions这种地方要做的耗时的操作放到这个界面的viewDidAppear中，之后再进行其他的跳转操作之类。而且可以自由控制进入真正的主界面的时间。
我的上一个项目，没经过优化之前的启动速度是3s，优化后启动速度是0.9s。提示还是很明显的。
ps：如何检查自己的启动时间：
使用NSLog的方法就可以了。
在AppDelegate.h中：CFAbsoluteTime StartTime;
在main.m中：
int
 main(int argc, char **argv) {
StartTime = CFAbsoluteTimeGetCurrent();
// ...
}
在AppDelegate.m的回调中
- (void)applicationDidFinishLaunching:(UIApplication *)app {
dispatch_async(dispatch_get_main_queue(), ^{
NSLog(@"App Launched in %f sec", CFAbsoluteTimeGetCurrent() - StartTime);
});
//
 ...
}
参考文章：http://cache.baiducontent.com/c?m=9f65cb4a8c8507ed4fece763105392230e54f73561838d4438958448e435061e5a71e2cf3a6157539283273a5db2495be8e4732f77552fecc6899f4badace25c38fb54230019913212c419dedc3654d656914d99db0e90cbe733e3b9a4d2c82757dd25046df1f19c5b7003bb6be76037f4d69d5f662b07bb9d27148f4e0122885344a136f3&p=8b2a954e898111a058ec9721444a9f24&newp=882a9544d5891dc312be9b7e0664a5755c5bc4387ebad414789ecf24d9&user=baidu&fm=sc&query=ios%BF%AA%B7%A2+%BC%D3%BF%EC%D3%A6%D3%C3%C6%F4%B6%AF%CB%D9%B6%C8&qid=&p1=5
