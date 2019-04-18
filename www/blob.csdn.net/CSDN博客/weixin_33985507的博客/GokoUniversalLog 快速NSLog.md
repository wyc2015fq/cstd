# GokoUniversalLog 快速NSLog - weixin_33985507的博客 - CSDN博客
2017年12月01日 16:29:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
# GokoUniversalLog
- 作为iOS/Mac 开发，你是否在使用NSLog的时候，对于他的繁琐使用感到麻烦呢，如果你有这个困扰，那么`GokoUniversalLog`将很适合你。
## 如何使用
- [下载 GokoUniversalLog](https://link.jianshu.com?t=https://github.com/Gokotx/GokoUniversalLog/archive/master.zip) 查看里面的Demo使用方式。
- 查看 `GokoUniversalLog.h`  有哪些APIs 可以用。
## 集成方式
### From CocoaPods
- 安装方法，支持CocoaPods ，只需要在podfile中添加如下内容
```
pod 'GokoUniversalLog'
```
或者使用如下方式，指定总是使用最新版
```
pod 'GokoUniversalLog', :git => 'https://github.com/Gokotx/GokoUniversalLog.git'
```
- 在工程目录下执行如下命令
```
pod install
```
### 手动集成
- 直接拖动 `GokoUniversalLog/GokoUniversalLog`  目录下两个文件到你的项目中
## 使用方法
### 全局设置是否启用Log
- 建议添加到  `-(BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions` ， 中来配置是否启用，配置代码如下：
```
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    GokoLogEnable(YES);
    return YES;
}
```
### 开始使用
- 简单Log 输出，同NSLog 输出内容
```
__attribute__((overloadable))  void GokoLog(id firstParam, ...) NS_REQUIRES_NIL_TERMINATION;
```
通常用起来就像下面这样子 (see [demo](https://link.jianshu.com?t=https://github.com/Gokotx/GokoUniversalLog/archive/master.zip) for more):
```
@Class Foo
NSString * fooo = @"Fooo";
NSLog(@"%@%@",fooo,[Foo new]);
//一句话输出,任何类型都直接支持，不需要再转化
GokoLog(fooo,[Foo new], nil);
GokoLog(1.4);
GokoLog(@1,nil);
GokoLog(16);
```
- 如果要查看一个对象的完整输出(包括属性，属性如果是类的话，继续递归输出)
```
__attribute__((overloadable)) void GokoDescriptionLog(id firstParam, ...) NS_REQUIRES_NIL_TERMINATION;
```
通常用起来就像下面这样子 ( see [demo](https://link.jianshu.com?t=https://github.com/Gokotx/GokoUniversalLog/archive/master.zip) for more) :
```
@Class Foo
NSString * fooo = @"Fooo";
GokoDescriptionLog(fooo,[Foo new], nil);
```
## 如果看不明白，下载 [Demo](https://link.jianshu.com?t=https://github.com/Gokotx/GokoUniversalLog/archive/master.zip) 查看使用方法
- 如果你看到这里，那么给你推荐一款流量统计的应用吧 [ijumper](https://link.jianshu.com?t=https://itunes.apple.com/cn/app/ijumper/id1298439690?mt=8) ,欢迎下载使用。
