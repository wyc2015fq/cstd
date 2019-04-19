# 关于Xcode8 iOS10下模拟器NSLog不输出的问题 - 月若无涯的专栏 - CSDN博客
2016年06月24日 09:56:32[月若无涯](https://me.csdn.net/u010124617)阅读数：12674
昨天升级了Xcode8beta版，兴高采烈的打开工程启动模拟器后发现自己的NSLog输出在console中看不到了，查阅[Xcode8 release note](https://developer.apple.com/download/)后发现官方的中有这么一段
> 
When debugging an app running on Simulator, logs may not be visible in the console. 
  Workaround: Use command + / in Simulator.app to open the system log in the Console app to view NSLogs. (26457535)
那么官方的方案是推荐我们使用系统的控制台来看日志：开着模拟器，按下“Command”和“/”键调出来，混杂着各种乱七八糟的日志，阅读起来很不爽。。。
有没有办法恢复以前console中显示我们日志呢？ 
好消息是printf()函数经过测试还是可以用的，那么我们可以通过宏定义来替换原来的NSLog
`#define NSLog(...) printf("%f %s\n",[[NSDate date]timeIntervalSince1970],[[NSString stringWithFormat:__VA_ARGS__]UTF8String]);`
如果希望该修改仅针对开发模式生效，可以添加
```
#ifndef __OPTIMIZE__
#define NSLog(...) printf("%f %s\n",[[NSDate date]timeIntervalSince1970],[[NSString stringWithFormat:__VA_ARGS__]UTF8String]);
#endif
```
好了，把这个宏丢到你的项目中比如Debug.h文件中，在全局引用中或各个文件自己引用即可
