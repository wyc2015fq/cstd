# iOS实现OTA无线安装App - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年09月07日 09:32:46[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：272
![](https://upload-images.jianshu.io/upload_images/1519620-bea14ff4dff290b1.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/816/format/webp)
> 
引用Apple说明：
您可以使用 Apple Developer Enterprise Program（Apple 开发者企> 业级计划）开发 iOS 和 macOS 应用，对于开发、测试应用以及将> > 其分发给用户，此计划提供了完整的流程。您可以通过将应用托管> > 在您创建的安全网站服务器上来分发应用，或通过使用移动设备管> > 理 (MDM) 解决方案或其他应用管理解决方案来进行分发。
参考链接： [Apple - 创建企业内部应用](https://help.apple.com/deployment/ios/#/apd29c0f45f1)
## 一、概念介绍
![](https://upload-images.jianshu.io/upload_images/1519620-f153dd9d69eaedd4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/375/format/webp)
1、什么是ipa文件？
> 
IPA 既 `iPhone application archive` ，是 iOS 系统的软件包，ipa 文件都是使用苹果的 [FairPlay](https://zh.wikipedia.org/w/index.php?title=FairPlay&action=edit&redlink=1)[DRM](https://zh.wikipedia.org/wiki/%E6%95%B0%E5%AD%97%E7%89%88%E6%9D%83%E7%AE%A1%E7%90%86) 技术进行加密保护的。
每个 ipa 文件都是 ARM 架构的可执行文件以及该应用的资源文件的打包文件，只能安装在 iPhone，iPod Touch 或 iPad 上。
ipa 文件可以通过修改后缀名为 zip 后，进行解压缩，查看其软件包中的内容。
2、什么是OTA？
> 
OTA 即 `Over-the-Air`，是 Apple 在 iOS4 中新加的一项技术，目的是让开发者能够脱离 Appstore，实现从服务器下载并安装 iOS 应用。
用户只需要在 iOS 设备的浏览器中，打开`itms-services://`协议链接，就可以直接安装App。（注意：此处的安装是指，个人账号需要注册设备，企业账号无需注册设备）
3、如何通过OTA安装？
> 
OTA方式安装，是通过 WebKit 解析链接中的`itms-services://`来实现的。
例如： `<a href="itms-services://?action=download-manifest&url=https://coding.net/u/richluisx/p/talent-shop/git/raw/master/manifest.plist">点击安装</a>`
iOS 浏览器会去读取 manifest.plist 中的信息，如：iOS应用的名称、版本、安装地址等。
4、IPA打包方式
|打包方式|开发者账号类型|发布App Store|安装设备|证书|
|----|----|----|----|----|
|App Store|公司账号、个人账号|YES|无限制|发布证书|
|Ad Hoc|公司账号、个人账号|NO|100|发布证书|
|Enterprise|企业账号|NO|无限制|发布证书|
|Development|公司账号、个人账号|NO|100|开发证书|
5、Apple开发者账号类型
|账号类型|价格|发布AppStore|安装设备数量|证书类型|
|----|----|----|----|----|
|个人账号|$99|YES|100|Ad Hoc, App Store|
|公司账号|$99|YES|100|Ad Hoc, App Store|
|企业账号|$299|NO|无限制|Ad Hoc, In House|
|教育账号|$0|YES|100|Ad Hoc, App Store|
## 二、分发App准备
![](https://upload-images.jianshu.io/upload_images/1519620-ffa44acb21d8591f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/450/format/webp)
|项目|说明|
|----|----|
|支持HTTPS的服务器|本文选择coding托管，也可以放到自己的服务器，原理相同|
|ipa文件|本文中使用企业签名的.ipa文件，[蒲公英企业签名服务](http://www.pgyer.com/app/signature)|
|manifest.plist文件|包含.ipa的下载地址、图片地址|
|icon文件|（2张）尺寸：512x512、57x57|
![](https://upload-images.jianshu.io/upload_images/1519620-d26ccd7c3ca1cb11.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/906/format/webp)
## 三、生成manifest.plist
1、打开Xcode，Targets选中Generic iOS Device，然后选择Product，选择Archive打包。
2、进入Archives，选择Export，下边三种方法都可以选。（区别参考1.4）
![](https://upload-images.jianshu.io/upload_images/1519620-ade6323a2fc79d89.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
3、勾选，包含 manifest.plist 文件用于 OTA 安装。
![](https://upload-images.jianshu.io/upload_images/1519620-937d0262bfa644d2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
4、填写相关信息，生成 manifest.plist 文件。（随便填写，后面可以修改）
![](https://upload-images.jianshu.io/upload_images/1519620-1849e3b8e778ea09.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
5、选择，自动管理签名，然后导出。
![](https://upload-images.jianshu.io/upload_images/1519620-e6f2f429b7f88e24.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
6、获取到 manifest.plist 文件。
![](https://upload-images.jianshu.io/upload_images/1519620-51e3563ef24e948f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
## 四、上传服务器
![](https://upload-images.jianshu.io/upload_images/1519620-d84ea854029f0510.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/530/format/webp)
> 
本文使用 [coding](https://coding.net/) 服务器来演示，也可以放到自己的服务器中，原理相同。
1、将文件推送到coding仓库，目录结构如下
![](https://upload-images.jianshu.io/upload_images/1519620-7cf6d66d87af691d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/460/format/webp)
2、获取.ipa文件下载链接
![](https://upload-images.jianshu.io/upload_images/1519620-f4892b6c3b564316.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
3、获取57x75、512x512图片链接
![](https://upload-images.jianshu.io/upload_images/1519620-dc15deab4a17554f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
4、获取manifest.plist链接，先选择“原始数据”，然后“复制浏览器中的链接”
![](https://upload-images.jianshu.io/upload_images/1519620-d1e14e9e9c22781c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
![](https://upload-images.jianshu.io/upload_images/1519620-3bb77842b5511404.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
## 五、修改manifest.plist
1、将第四章的获取的链接，以及App信息等数据，替换到`【xxxxx】`标记的对应内容中，然后重新推送到coding远程仓库。
```
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>items</key>
    <array>
        <dict>
            <key>assets</key>
            <array>
                <dict>
                    <key>kind</key>
                    <string>software-package</string>
                    <key>url</key>
                    <string>【xxxxx】</string>
                </dict>
                <dict>
                    <key>kind</key>
                    <string>display-image</string>
                    <key>url</key>
                    <string>【xxxxx】</string>
                </dict>
                <dict>
                    <key>kind</key>
                    <string>full-size-image</string>
                    <key>url</key>
                    <string>【xxxxx】</string>
                </dict>
            </array>
            <key>metadata</key>
            <dict>
                <key>bundle-identifier</key>
                <string>【xxxxx】</string>
                <key>bundle-version</key>
                <string>【xxxxx】</string>
                <key>kind</key>
                <string>software</string>
                <key>title</key>
                <string>【xxxxx】</string>
            </dict>
        </dict>
    </array>
</dict>
</plist>
```
**字段参考：**
|字段|值|说明|
|----|----|----|
|software-package|`https://coding.net/u/richluisx/p/talent-shop/git/raw/master/talent-shop.ipa`|.ipa 安装包地址|
|display-image|`https://coding.net/u/richluisx/p/talent-shop/git/raw/master/talent-logo.512x512.png`|.57x57 图片地址|
|full-size-image|`https://coding.net/u/richluisx/p/talent-shop/git/raw/master/talent-logo.57x57.png`|.512x512 图片地址|
|bundle-identifier|com.xxx.xxx|包标识|
|bundle-version|1.2.1|版本号|
|title|达人店|应用名称|
## 六、生成下载App的html
![](https://upload-images.jianshu.io/upload_images/1519620-d20de48e1a8dbacb.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/634/format/webp)
1、通过`<a>`标签，跳转`itms-services`协议链接的方式，来下载安装App。
**OTA安装协议参考：**
```
itms-services://?action=download-manifest&url=【替换为 manifest.plist 文件地址】
```
**示例代码：**
```
<!DOCTYPE html>
<html lang="en">
<head>
   <meta charset="UTF-8">
   <title>达人店App下载</title>
</head>
<body>
<a href="itms-services://?action=download-manifest&url=https://coding.net/u/richluisx/p/talent-shop/git/raw/master/manifest.plist">点击安装</a>
</body>
</html>
```
## 七、安装App
![](https://upload-images.jianshu.io/upload_images/1519620-321e4670f4bcfc81.jpeg?imageMogr2/auto-orient/strip%7CimageView2/2/w/360/format/webp)
更多文章            
