# 玩转swift(二) - 三少GG - CSDN博客
2016年01月22日 17:26:34[三少GG](https://me.csdn.net/scut1135)阅读数：318
工具篇
# 用CocoaPods做iOS程序的依赖管理
http://blog.devtang.com/blog/2014/05/25/use-cocoapod-to-manage-ios-lib-dependency/
### 使用 CocoaPods
使用时需要新建一个名为 Podfile 的文件，以如下格式，将依赖的库名字依次列在文件中即可
```
```bash
platform :ios
pod 'JSONKit',       '~> 1.4'
pod 'Reachability',  '~> 3.0.0'
pod 'ASIHTTPRequest'
pod 'RegexKitLite'
```
```
然后你将编辑好的 Podfile 文件放到你的项目根目录中，执行如下命令即可：
```
```bash
cd "your project home"
pod install
```
```
现在，你的所有第三方库都已经下载完成并且设置好了编译参数和依赖，你只需要记住如下 2 点即可：
- 
使用 CocoaPods 生成的 .xcworkspace 文件来打开工程，而不是以前的 .xcodeproj 文件。
- 
每次更改了 Podfile 文件，你需要重新执行一次
```
pod
 update
```
命令。
