# iOS开发实战 - fastlane一键打包并上传至Appstore/蒲公英 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年04月04日 13:50:30[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：190

##### fastlane 介绍
> 
[fastlane](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Ffastlane%2Ffastlane) 是一款为 iOS 和 Android 开发者提供的自动化构建工具，它可以帮助开发者将 App 打包、签名、测试、发布、信息整理、提交 App Store 等工作完整的连接起来，实现完全自动化的工作流，如果使用得当，可以显著的提高开发者的开发效率。
依赖环境:
Xcode7 +
macOS or Linux with Ruby 2.0.0 +
###### 废话不多说，一起来看高清大图 ...
##### 1. 安装fastlane
$ sudo gem install fastlane
```php
```php
如果报错：ERROR: While executing gem ... (Errno::EPERM) Operation not permitted - /usr/bin/commander 
使用: sudo gem install -n /usr/local/bin fastlane
```
```
![](https://upload-images.jianshu.io/upload_images/1859695-6e5dda51d328026e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
mac自带的ruby，需要sudo权限
###### 检查一下是否安装成功
$ fastlane --version
##### 2. fastlane 初始化 （cd到你的工程目录）
![](https://upload-images.jianshu.io/upload_images/1859695-97181222aef3bafa.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
##### 3. 初始化选项：这里我选择 “3” - 发布到Appstore
![](https://upload-images.jianshu.io/upload_images/1859695-e8ce75af6f83cc45.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
输入开发者账号和密码
![](https://upload-images.jianshu.io/upload_images/1859695-1fc7a5e19f2fc9a8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
允许管理你的App’s matadata
![](https://upload-images.jianshu.io/upload_images/1859695-c09ca447028e2896.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
完成
![](https://upload-images.jianshu.io/upload_images/1859695-98083745fa5d4897.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
目录更新
> 
补充：
1.fastlane 初始化默认会创建三个文件：Fastfile、Appfile、Deliverfile；两个文件夹：metadata、screenshots
2.Fastfile : 核心文件，主要用于 命令行调用和处理具体的流程，lane相对于一个方法或者函数
3.Appfile : 存储有关开发者账号相关信息
4.Deliverfile: deliver工具的配置文件
如果Deliverfile、screenshots和metadata没有自动生成，通过deliver init 重新初始化
##### 4. 一键打包并上传至Appstore
###### 4.1 工程版本配置
![](https://upload-images.jianshu.io/upload_images/1859695-bf74f7e33192e6a5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)

![](https://upload-images.jianshu.io/upload_images/1859695-335e2abbae8df5a9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
和上图的Version对应
###### 4.2 初始化 fastlane match
$ fastlane match init
![](https://upload-images.jianshu.io/upload_images/1859695-89bbda642029e6dc.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
###### 4.3  fastfile 文件 (后期会更新更多详细用法)
![](https://upload-images.jianshu.io/upload_images/1859695-63dffa7bcb771970.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
默认配置：因为我们在初始化fastlane的时候选择了发布到Appstore increment_build_number 会自动增加app的版本号
###### 4.3 一键发布
![](https://upload-images.jianshu.io/upload_images/1859695-5e624409cecfea31.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
耐心等待...
![](https://upload-images.jianshu.io/upload_images/1859695-5aa593ce872ea501.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
在这之前，浏览器会弹出一个html页面展示App信息，输入 y即可
![](https://upload-images.jianshu.io/upload_images/1859695-397d9c5b252cd647.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
上传完成，只需要10分钟额
![](https://upload-images.jianshu.io/upload_images/1859695-11ff4511a520c496.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
完成，稍等十几分钟，选择构建的包，修改本次发布的内容就可以开布了
##### 5. 一键打包并上传至蒲公英
[参考文档](https://link.jianshu.com?t=https%3A%2F%2Fwww.pgyer.com%2Fdoc%2Fview%2Ffastlane)
###### 5.1 安装蒲公英的 fastlane 插件
$ fastlane add_plugin pgyer
![](https://upload-images.jianshu.io/upload_images/1859695-707b1d62067e170e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
###### 5.2 修改 fastfile 文件
![](https://upload-images.jianshu.io/upload_images/1859695-80967ed3da2eb5d8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
fastfile文件
![](https://upload-images.jianshu.io/upload_images/1859695-2618ed94fc867f53.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
获取 API Key 和 User Key
###### 5.3 一键发布
![](https://upload-images.jianshu.io/upload_images/1859695-56bbe88634bf32d4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
然后就可以去看抖音啦。。。
![](https://upload-images.jianshu.io/upload_images/1859695-8cf8163df9d669c7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
上传至蒲公英中...
![](https://upload-images.jianshu.io/upload_images/1859695-845219a22126a4a8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)

![](https://upload-images.jianshu.io/upload_images/1859695-418dac59b9accee1.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/375)
短信提醒
###### 6.容易出错的点：
> 
1.生产证书缺少私钥，你可能是从开发者中心自行下载的或者本地钥匙串里没有，你需要你的小伙伴发给你生产证书的p12文件生成生产证书（例如：换了一台新的设备）；
2.描述文件不存在，或者过期的情况；
3.或者是你不存在Ad Hoc的发布证书，你需要登陆开发者账号去创建即可;
! ! ! 不是上述几种错误的也不要慌，看终端里面的错误信息，对症下药
作者：Mint丶bin
链接：https://www.jianshu.com/p/d951473098eb
來源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
