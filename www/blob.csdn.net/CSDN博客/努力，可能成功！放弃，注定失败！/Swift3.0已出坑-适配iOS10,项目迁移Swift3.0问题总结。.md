# Swift3.0已出坑-适配iOS10,项目迁移Swift3.0问题总结。 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年10月31日 16:55:06[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：5213
# 前言
> 
swift项目,为适配iOS10,无奈只能更新Xcode 8 ,可是发现一入3.0深似海,从此幸福是路人.于是边摸索边修改,在节前的完成代码迁移.节后在完成手头工作后,整理思路,把swift3迁移的心得分享大家.
废话不多说分享下心得:
## 1、代码篇:(一定要做好备份!!!)
#### 1>升级Xcode8正式版(8A218a).
![](http://upload-images.jianshu.io/upload_images/1666327-98d84a6ab3a12740.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Xcode 8 
#### 2>使用系统自带的桥接工具.(原装的就是好,直接打开2.2项目也会提醒->步骤:3)
###### Edit > Convert > To Current Swift Syntax..
![](http://upload-images.jianshu.io/upload_images/1666327-fec7c1aada9529b8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
桥接选项
#### 3>不用说就是为3.0来的,选择Swift3.
![](http://upload-images.jianshu.io/upload_images/1666327-84d13f5bf197baa0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Swift3.0
#### 4>上面的框架就不用选了,只选择`.app`的.(这个是检测代码,不然你会哭的)
![](http://upload-images.jianshu.io/upload_images/1666327-4e3fc24ecf949ac7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
.app
#### 5>检测完后.
在左边的选择页中选择好要转移的代码,左边的代码是swift3.0代码,右侧代码时swift2.2代码->点`Save`.
![](http://upload-images.jianshu.io/upload_images/1666327-bea954aa25e31670.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
6.png
## 2、第三方框架篇:(使用的cocoapods管理的第三方框架)
#### 1>发现框架报错.(不要怕,按照错误一样一样来)
- 
###### 问题1:--.第三方框架版本问题,"Use Legacy Swift Language Version"
“Use Legacy Swift Language Version” (SWIFT_VERSION) is required to be configured correctly for targets which use Swift. Use the [Edit > Convert > To Current Swift Syntax…] menu to choose a Swift version or use the Build Settings editor to configure the build
 setting directly.
![](http://upload-images.jianshu.io/upload_images/1666327-3146e35e3b1dff2f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第三方框架版本问题
在pod的配置文件里面的Use Legacy Swift Language Version选择`YES`错误就消除了
`Pods`->`TARGETS->指定框架` -> `Build Settings` -> 
```
Use Legacy Swift Language Version
```
![](http://upload-images.jianshu.io/upload_images/1666327-ced879460f44ea3c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
pod的配置
- 
###### 问题2:--.lipo:can't open input file:xxx(No such file or directory)
![](http://upload-images.jianshu.io/upload_images/1666327-bb07e2570e751087.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
lipo:
`PROJRCT`-> `Build Settings` -> `Build Active Architecture only`->`YES`
![](http://upload-images.jianshu.io/upload_images/1666327-9bc9b5e892190e90.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
lipo
其他问题后续收集再整理...
#### 2>发现框架过时,需要更新框架.
由于3.0的更新,swift各个主流框架都已经及时的推出了新的版本,例如:Alamofire 4.0.0 , SnapKit 3.0.0 , Kingfisher 3.1.0 , SwiftyJSON 也已经更新了专供Swift3的分支.
所以,要更新框架以供使用.
![](http://upload-images.jianshu.io/upload_images/1666327-d67e896f28e9c548.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
更新主流第三方框架
```php
```php
pod 'SnapKit', '~> 3.0.0'
pod 'SwiftyJSON', :git => 'https://github.com/acegreen/SwiftyJSON.git', :branch => 'swift3'
pod 'Alamofire','~> 4.0.0'
pod 'Kingfisher','~> 3.1.0'
```
```
可能会出现以下问题:
- 
###### SnapKit框架问题:
![](http://upload-images.jianshu.io/upload_images/1666327-b66d211f0bbcc0f6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
SnapKit
看到200多个框架错误后凌乱了.都是3.0语法了,为什么还会报错,不在框架,在自己安装的cocoapods环境有关系.
解决方案:
需要更新最新环境到`cocoapods 1.1.0.rc.2`
``sudo gem install cocoapods --pre``
可以参照SnapKit的官方issues:
[https://github.com/SnapKit/SnapKit/issues/300](https://github.com/SnapKit/SnapKit/issues/300)
如果没有安装 cocoapods可以参照之前写的博客.
[安装 cocoapods 1.1.0心得体会:](http://www.jianshu.com/p/b98b8ac7d22d)
- 
###### Alamofire框架问题:
![](http://upload-images.jianshu.io/upload_images/1666327-14c391721f02140b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Alamofire
Alamofire/Source/ServerTrustPolicy.swift:243:41: Use of unresolved identifier 'kSecTrustResultInvalid'
Alamofire的官方issues:
[https://github.com/Alamofire/Alamofire/issues/1541](https://github.com/Alamofire/Alamofire/issues/1541)
解决方案:
上面也提到了,更新最新的Alamofire框架.
```bash
pod 'Alamofire','~> 4.0.0'
```
可能还遇到的问题:
```
[!] Unable to satisfy the following requirements:
- `Alamofire (~> 4.0.0)` required by `Podfile`
Specs satisfying the `Alamofire (~> 4.0.0)` dependency were found, but they required a higher minimum deployment target.
```
解决方法:
Alamofire 4.0.0 最低要求系统版本9.0,已经将系统升高到9.0还是无法安装.
目前的没有找到有效的解决方法,只有一个方案就是重新立项先集成框架,在把转接完的代码迁移到新的项目中去.
10月24日,更新解决方法.
之前提示Alamofire 4.0.0, Podfile报错.
需要在Podfile文件中指定最低系统9.0项目版本.`platform :ios,'9.0'`
Alamofire的官方issues:
[https://github.com/Alamofire/Alamofire/issues/1639](https://github.com/Alamofire/Alamofire/issues/1639)
![](http://upload-images.jianshu.io/upload_images/1666327-2638ba907c545a40.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
指定9.0
- 
###### 可能还会有其他框架问题,目前还没有遇到.欢迎讨论.
## 3.其他问题总结:
- 
###### 关于重建工程来迁移到swift3.0解决方案.
前面也提到了,发现有个别问题无法解决,一直解决也是没有成效,于是使用目前来说最简洁的方法就重新建立工程.
先把新建的工程集成好最新支持swift3.0的第三方框架,测试程序可不可以跑通后,再把之前转换完的代码一个分类一个分类迁移到新的工程里面.
至于手工拖入的第三方依赖库,应用跳转,按照之前集成好的照搬.或者重新集成.
- 
###### swift3.0语法的学习.
1.官方已经给出文档以及转变说明.
Apple Swift 3.0官方文档:[The Swift Programming Language
 (Swift 3)](https://developer.apple.com/library/prerelease/content/documentation/Swift/Conceptual/Swift_Programming_Language/TheBasics.html#//apple_ref/doc/uid/TP40014097-CH5-ID309)
![](http://upload-images.jianshu.io/upload_images/1666327-0b6b3aaf1d67bed3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Apple Swift 3.0官方文档
2.Apple Swift 3.0官方演变:[apple/swift-evolution/releases/swift-3_0.md](https://github.com/apple/swift-evolution/blob/master/releases/swift-3_0.md)
![](http://upload-images.jianshu.io/upload_images/1666327-0e3bcf5083b6d4e0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Swift 3.0官方演变
其主要是废除了一些API,简化代码的书写,例如取出OC中的NS类前缀,C语言的风格,等等.
3.国外raywenderlich的Swift 3.0迁移介绍:
[https://www.raywenderlich.com/135655/whats-new-swift-3](https://www.raywenderlich.com/135655/whats-new-swift-3)
![](http://upload-images.jianshu.io/upload_images/1666327-0cb70ac2167f7a10.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
raywenderlich的Swift 3.0
- 
###### 后续持续补充.
## 后记
- 对swift3的书写方式还需要进一步的加强,虽然和之前的写法有着轻微的改动,使整体变的更加的简洁、严谨,但是稍不注意还是出现莫名其妙的报错.
- 展望swift4,让暴风雨来的更猛烈些吧.
- 欢迎讨论、批评、指正.
文／RamboLu（简书作者）
原文链接：http://www.jianshu.com/p/27fd2a2b32e4
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
