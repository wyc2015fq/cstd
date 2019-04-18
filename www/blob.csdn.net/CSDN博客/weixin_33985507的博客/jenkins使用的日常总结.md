# jenkins使用的日常总结 - weixin_33985507的博客 - CSDN博客
2017年01月09日 15:44:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：12
# 安装
**推荐使用 brew 去安装,如果直接下载安装包,会遇到各种坑;**我们使用 jenkins 打包的目的就是能够简单快速的打包;如果安装过程中要自己去填这样那样的坑是我们所不愿意的;
```
安装
1.使用命令行安装
$ brew install jenkins
如果没有安装java环境的话会出现以下错误，需要先安装jenkins需要安装java环境， [JDK下载地址](http://www.oracle.com/technetwork/java/javase/downloads/index.html)
jenkins: Java 1.7+ is required to install this formula. You can install with Homebrew Cask: brew install Caskroom/cask/java You can download from: http://www.oracle.com/technetwork/java/javase/downloads/index.html Error: An unsatisfied requirement failed this build.
2.当在终端中看到`Jenkins is fully up and running`表示安装成功了,在浏览器中输入以下地址访问：
http://localhost:8080/
3.往后要使用 jenkins, 直接在终端中输入
$ jenkins
看到Jenkins is fully up and running,浏览器中输入以下地址访问：
http://localhost:8080/
```
# 配置
- 
参数化构建
![874748-9ef642637f22a66c.png](https://upload-images.jianshu.io/upload_images/874748-9ef642637f22a66c.png)
参数化构建
由于 jenkins 打包提供的时间没有时分秒,只有年月日;我们可以自己来获取打包时间
![874748-9892b78caaac6721.png](https://upload-images.jianshu.io/upload_images/874748-9892b78caaac6721.png)
配置打包时间
![874748-74011304c902e4a8.png](https://upload-images.jianshu.io/upload_images/874748-74011304c902e4a8.png)
不同 scheme 打包
- 源码管理
我们是选用 github 代码仓库作为实例,所以要安装 github plugin 配置源码
![874748-3a8d7f8b396fc832.png](https://upload-images.jianshu.io/upload_images/874748-3a8d7f8b396fc832.png)
- 
构建
本文用 xcode plugin 插件实现,你也可以可以使用 shell( 感觉比较麻烦)
![874748-c66c80d2c025bc4e.png](https://upload-images.jianshu.io/upload_images/874748-c66c80d2c025bc4e.png)

![874748-f1e9ef938461d67f.png](https://upload-images.jianshu.io/upload_images/874748-f1e9ef938461d67f.png)
配置构建参数,输出目录, ipa 包名
签名钥匙串先要安装[Keychains and Provisioning Profiles Management](https://link.jianshu.com?t=http://wiki.jenkins-ci.org/display/JENKINS/Keychains+and+Provisioning+Profiles+Plugin)
![874748-17a1e92c7ddabf64.png](https://upload-images.jianshu.io/upload_images/874748-17a1e92c7ddabf64.png)
安装插件
login.keychain 地址`/Users/管理员用户名/Library/keychains/login.keychain`
![874748-62d7b7dbae8a3d94.png](https://upload-images.jianshu.io/upload_images/874748-62d7b7dbae8a3d94.png)
![874748-c567cfce69d02fd8.png](https://upload-images.jianshu.io/upload_images/874748-c567cfce69d02fd8.png)
上传 login.keychain
![874748-0997f9885c0deb1e.png](https://upload-images.jianshu.io/upload_images/874748-0997f9885c0deb1e.png)
签名的钥匙串
![874748-29ff6a14d7101688.png](https://upload-images.jianshu.io/upload_images/874748-29ff6a14d7101688.png)
Paste_Image.png
# configuration 和 scheme
![874748-c711a62ce178e72e.png](https://upload-images.jianshu.io/upload_images/874748-c711a62ce178e72e.png)
Xcode 设置不同的环境 provison
- configuration
一般用来设置多种环境,例如 debug adhoc appstore
![110555-c8db153a43013d73.png](https://upload-images.jianshu.io/upload_images/110555-c8db153a43013d73.png)
设置新的configuration
[Xcode多种Build Configuration配置使用](https://www.jianshu.com/p/51a2bbe877aa)
[Xcode定义Preprocessor Macros，编译宏。](https://link.jianshu.com?t=http://blog.csdn.net/jlstmacblog/article/details/21019479)
- scheme配置(Target)
scheme 一般是用来对应不同的终端,例如 iphone 和 ipad 一般用两个 target
![874748-df758771779cae91.png](https://upload-images.jianshu.io/upload_images/874748-df758771779cae91.png)
配置不同的 scheme
![874748-22eb14aca35d322e.png](https://upload-images.jianshu.io/upload_images/874748-22eb14aca35d322e.png)
iphone 和 ipad 按不同 scheme构建
- 
sheme 和 configuration 混合使用
假如工程中，有iphone和iPad两个相同target。要实现在iPhone和iPad的同一个程序块，不同的逻辑。这里就可以用到Preprocessor Macros了。只要把你的宏加到Preprocessor Macros里就行了。
![874748-64a73179d7c99298](https://upload-images.jianshu.io/upload_images/874748-64a73179d7c99298)
使用方法：
```
#ifdef HD_VERSION
    ...;
#endif
```
# 邮件
配置邮件的时候,qq 邮箱设置不了 stmp,我用的是 163邮箱
更多细节请看链接,我就不再重复写了[Jenkins邮件配置](https://link.jianshu.com?t=http://www.cnblogs.com/TestWorld/p/5603792.html)
配置不成功可能出现的问题:
- 没有去163设置 smtp 的密码
- 没有配置 jenkins 系统管理员邮箱地址
![874748-4252c631648c1780.png](https://upload-images.jianshu.io/upload_images/874748-4252c631648c1780.png)
设置 smtp 服务
![874748-99d1fe2ceafeca71.png](https://upload-images.jianshu.io/upload_images/874748-99d1fe2ceafeca71.png)
配置 jenkins 系统管理员邮箱地址
# 扫码下载
目前有两种方式:
- 上传到三方网站,例如蒲公英
- 自己搭建网站提供扫码下载
参考链接
[手把手教你利用Jenkins持续集成iOS项目](https://www.jianshu.com/p/41ecb06ae95f)
[使用Jenkins搭建iOS/Android持续集成打包平台](https://link.jianshu.com?t=http://debugtalk.com/post/iOS-Android-Packing-with-Jenkins/)
[苹果App部署HTTPS进行在线下载安装](https://link.jianshu.com?t=http://doc.wex5.com/ipa-deploy-https/)
