# Xcode自动打包（傻瓜式教程） - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年04月02日 16:26:52[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1791

![](https://upload-images.jianshu.io/upload_images/2356976-8832c3bea8ee42ad.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/465)
logo.jpg
### 目录
- 常用打包方式
- 自动打包流程
- 过程图解
- xcodebuild使用
- 脚本文件解读
一个普通的项目，从打包到上传（或导出ipa），耗时基本在10-30分钟或者更长时间，且中间得有人工看守（其实就是Next step），如果通过脚本执行自动打包，方便不方便的先放一边，逼格是不是一下子就提上来了，下面这篇文章将从傻瓜式打包讲起，让你先顺利的完成自动打包的一个小流程，当然，如果有兴趣，可以向下翻一番，看看原理。
### 一、常用打包方式：
Product ——>Archive ——>Upload to AppStore / Export
### 二、自动打包流程
目前xcode9已经可以自己创建管理证书了，所以在流程中省略了证书及描述文件配置。
- 
[下载shell脚本](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2FtonyLZQ%2FArchiveScript)
- 
将Archive文件夹中的文件拖入项目的根目录下
- 
修改脚本中的一下基本配置（项目名称、debug模式等）
- 
cd到工程根目录，通过./shell.sh 执行脚本
- 
end
### 三、过程图解
下面用导出ipa包的过程举个例子，appstore和fir大同小异。
![](https://upload-images.jianshu.io/upload_images/2356976-902786b53bd8c376.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
下载脚本及配置文件.png
![](https://upload-images.jianshu.io/upload_images/2356976-5bc78a4a866e8c43.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
项目根目录对比.png
打开shell.sh脚本文件，修改相关配置。
![](https://upload-images.jianshu.io/upload_images/2356976-511fe8e509a57216.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/641)
修改脚本配置-1.png
![](https://upload-images.jianshu.io/upload_images/2356976-502184a61b18617d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
修改脚本配置-2.png
![](https://upload-images.jianshu.io/upload_images/2356976-19d0044d45462b20.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
修改脚本配置-3（非必配）.png
![](https://upload-images.jianshu.io/upload_images/2356976-8c09e2b3a502a354.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
执行脚本打包.png
![](https://upload-images.jianshu.io/upload_images/2356976-09db6d6a2ad7f5c4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
自动打包过程.png
![](https://upload-images.jianshu.io/upload_images/2356976-40451d5b45b466b9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/613)
ipa路径.png
⚠️：当我们在窗口中对xcode进行操作时，Xcode内部是执行了xcodebuild命令，脚本打包原理其实就是将xcodebuild命令统一放到一个脚本文件中，运行这个脚本文件，执行一系列操作。
### 四、xcodebuild初步使用
##### 4.1、xcodebuild常用命令：
```cpp
```cpp
// 1、查看xcodebuild版本
$ xcodebuild -version
// 2、查看当前系统的SDK及其版本
$ xcodebuild -showsdks
// 3、 查看工程中的所有目标（target）
$ xcodebuild -list
//  4、编译工程 (默认编译Release版本)
$ xcodebuild -target AddTest
// 5、 编译Debug版本：
$ xcodebuild -target AddTest -configuration Debug
//  6、clean清理工程
$ xcodebuild clean
```
```
##### 4.2、xcodebuild复杂命令的使用
- Product ——>Archive
```
```
$ xcodebuild archive -workspace 项目工作路径 -scheme 项目名称
$ xcodebuild archive -workspace  \
/Users/zeamo/Desktop/ArchiveTest/ArchiveTest.xcworkspace \
-scheme ArchiveTest
```
```
![](https://upload-images.jianshu.io/upload_images/2356976-107eb616d3cfa9fb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
xcodebuild命令-1.png
![](https://upload-images.jianshu.io/upload_images/2356976-dacac1cf0ddd39e5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/677)
xcodebuild archive执行结果.png
- Archive ——> ipa
```bash
xcodebuild -exportArchive -archivePath \
${build_path}/${project_name}.xcarchive \
-configuration ${development_mode} \
-exportPath ${exportIpaPath} \
-exportOptionsPlist ${exportOptionsPlistPath} \
```
### 四、脚本文件解读
脚本文件原自 [kepuna github](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fkepuna%2FarchiveScript)
![](https://upload-images.jianshu.io/upload_images/2356976-79797af86c0504ea.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
脚本文件解读-1.png
![](https://upload-images.jianshu.io/upload_images/2356976-cab0d3f6e10238e2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
脚本文件解读-2.png
![](https://upload-images.jianshu.io/upload_images/2356976-c101932f48334fb3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
脚本文件解读-3.png
![](https://upload-images.jianshu.io/upload_images/2356976-896b44265eadc635.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
脚本文件解读-4.png
![](https://upload-images.jianshu.io/upload_images/2356976-9a9a999e10e24953.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
脚本文件解读-5.png
参考文章：
[https://blog.csdn.net/qq981378640/article/details/53923973](https://link.jianshu.com?t=https%3A%2F%2Fblog.csdn.net%2Fqq981378640%2Farticle%2Fdetails%2F53923973)
[https://blog.csdn.net/u014205965/article/details/78083686](https://link.jianshu.com?t=https%3A%2F%2Fblog.csdn.net%2Fu014205965%2Farticle%2Fdetails%2F78083686)
[https://www.jianshu.com/p/97c97c2ec1ca](https://www.jianshu.com/p/97c97c2ec1ca)
[https://www.jianshu.com/p/05dc9f925467](https://www.jianshu.com/p/05dc9f925467)
作者：强子ly
链接：https://www.jianshu.com/p/121fe6fdc4e9
來源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
