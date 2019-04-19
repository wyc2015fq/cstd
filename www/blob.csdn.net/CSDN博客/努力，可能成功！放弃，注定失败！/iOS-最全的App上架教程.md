# iOS-最全的App上架教程 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年07月01日 09:12:14[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1412
## App上架教程
### 准备
- 开发者账号
- 完工的项目
### 上架步骤
- 一、创建App ID
- 二、创建证书请求文件 （CSR）
- 三、创建发布证书 （CER）
- 四、创建Provisioning Profiles配置文件 （PP文件）
- 五、在App Store创建应用
- 六、打包上架
### 一、创建App ID
- 
1.打开[苹果开发者网](https://developer.apple.com/)，点击“Account”登录会员中心。
- 
2.填写信息创建app ID
![](http://upload-images.jianshu.io/upload_images/1418424-20092b058e4afdd1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
点击+创建ID
![](http://upload-images.jianshu.io/upload_images/1418424-10bd21bcc5eeaec2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/1418424-a59c6f4aae94472f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/1418424-63314c4b0171b10b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/1418424-f4b95ccec7bc40f2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
### **二、创建证书请求文件（CSR文件）**
> 
CRS文件主要用于 绑定你的电脑的
- 
1 **点开LaunchPad，在其他中找到打开钥匙串访问**
![](http://upload-images.jianshu.io/upload_images/1418424-cf7caf84cdcda17c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![Uploading 3_933887.png . . .]
- 
2 **点击电脑左上角的钥匙串访问–证书助理–从证书颁发机构请求证书**
![](http://upload-images.jianshu.io/upload_images/1418424-66e0c26bf8a6f299.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
- 3 **出现如下界面，选择存储到磁盘，点击继续**
![](http://upload-images.jianshu.io/upload_images/1418424-d049f3766f21bd9a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
- 
4 **选择存储到桌面，存储**
![](http://upload-images.jianshu.io/upload_images/1418424-367a065f76c30178.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
- 
5 **点击完成**
![](http://upload-images.jianshu.io/upload_images/1418424-2173b31cbb877cb1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
- 
6 **在桌面上看到下面的文件，证书请求文件完成**
![](http://upload-images.jianshu.io/upload_images/1418424-39d033f9499fdddf.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
### 三、创建发布证书 (CER文件)
- 
1 **找到Provisioning Profiles ，点击All，然后点击右上角 + 号**
![](http://upload-images.jianshu.io/upload_images/1418424-9ead3933fb3fcfc9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
- 
2 **点击App Store and Ad Hoc**
![](http://upload-images.jianshu.io/upload_images/1418424-48423bb0a59a4245.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
` 发布证书和开发者证书需要分别创建，操作两次，开发者证书用于真机调试，发布证书用于提交到AppStore。`
- 
3.** 点击Continue **
![](http://upload-images.jianshu.io/upload_images/1418424-5a5b3415e16d2361.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
- 
4.** 点击Continue **
![](http://upload-images.jianshu.io/upload_images/1418424-77f40f2502100787.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
- 
5.** 点击choose File.. 选择创建好的证书请求文件：CertificateSigningRequest.certSigningRequest 文件，点击Generate**
![](http://upload-images.jianshu.io/upload_images/1418424-99eaa653aaa320ff.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
- 
6.** 点击Download下载创建好的发布证书（cer后缀的文件），然后点击Done，你创建的发布证书就会存储在帐号中。 **
![](http://upload-images.jianshu.io/upload_images/1418424-f623c8edaa0d748a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
- 
7.** 双击安装。如果安装不上，可以直接将证书文件拖拽到钥匙串访问的列表中 **
> 
`重点`: 一般一个开发者帐号创建一个发布证书就够了，如果以后需要在其他电脑上上架App，只需要在钥匙串访问中创建p12文件，把p12文件安装到其他电脑上。这相当于给予了其他电脑发布App的权限。
## 四、创建Provisioning Profiles文件
- - **找到Provisioning Profiles ，点击All，然后点击右上角 + 号**
![](http://upload-images.jianshu.io/upload_images/1418424-8db5446f31900685.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
- 
2.** 选择App Store，点击Continue**
![](http://upload-images.jianshu.io/upload_images/1418424-d9221799ef0efd40.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
> 
该流程也需要进行两次，分别创建开发用的PP证书和发布的PP证书。
- 
3.**在App ID 这个选项栏里面找到你刚刚创建的：App IDs（Bundle ID） 类型的套装，点击Continue**
![](http://upload-images.jianshu.io/upload_images/1418424-0791ffc70db51dd9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
- 
4.**选择你刚创建的发布证书（或者生成p12文件的那个发布证书），点击Continue**
![](http://upload-images.jianshu.io/upload_images/1418424-69b1cf1411299c02.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
- 
5.**在Profile Name栏里输入一个名字（这个是PP文件的名字，可随便输入，在这里我用工程名字，便于分别），然后点击Generate**
![](http://upload-images.jianshu.io/upload_images/1418424-75051d80b80b745d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
> 
注意：wildCard格式的证书没有推送，PassCard等服务的应用，慎重选择。因为PP证书的开发者证书需要真机调试，所以我们需要绑定真机，这里因为之前添加过一些设备，所以这里就可以直接全选添加，如果没有的话，需要将真机的udid复制出来在此添加。在发布PP文件中，是没有这一步的。
- 6.**Download生成的PP文件，然后点击Done**
![](http://upload-images.jianshu.io/upload_images/1418424-b745137046d3a4b0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
> 
双击就添加到Xcode中，这样在真机调试或者发布时，就可以分别有不同的PP证书与其对应。其实可以不用下载保存
五、在App Store创建应用
**1、回到Account，点击iTunes Connect**
**2、点击我的App**
![](http://upload-images.jianshu.io/upload_images/1418424-7deb556aa1835f8c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
**3、点击新建 iOSApp**
![](http://upload-images.jianshu.io/upload_images/1418424-5a2cbd976f9f0230.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
**4、依次按提示填入对应信息，然后点击创建**
![](http://upload-images.jianshu.io/upload_images/1418424-6ed8eae5f74d82d7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
**5、依次把不同尺寸的App截图拉入到对应的里面**
![](http://upload-images.jianshu.io/upload_images/1418424-ca495bcbde3b7f6e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
**6、填入App简介**
![](http://upload-images.jianshu.io/upload_images/1418424-816987f3d7f21825.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
**7、按提示依次输入**
![](http://upload-images.jianshu.io/upload_images/1418424-56f1d721e7c87430.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
> 
此时这个构建版本还没有生成，我们先把基本信息填写完毕，然后再进入Xcode中把项目打包发送到过来。注意：填写完一定要点击右上角的保存。
![](http://upload-images.jianshu.io/upload_images/1418424-4689db026a0a8e80.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
> 
不要忘记填写测试账号，否则会被拒的，而且一定要跟服务器同事说好，不要删除测试账号，否则同样被拒（联系号码 一定要+ 86 如：+86 15167896568
### 六、打包上架
> 
在Xcode中打包工程找到你刚刚下载的发布证书（后缀为.cer）或者p12文件，和PP文件，双击，看起来没反应，但是他们已经加入到你的钥匙串中。如果之前步骤已操作过，可省略此步。
**1、打开Xcode，配置项目环境，点击+可以选择Add Apple ID；点击View Details可以查看该Apple Id下的Certificates和Provisioning Profile证书文件，在这里你可以点击下载。在项目Targets下的Identity中，Team选择对应的Apple ID 即可。**
> 
特别注意： 这里填写的Apple ID 不是你自己手机上创建的Apple ID 一定要是 开发者账号的 账号和密码 （~QAQ~ 我就在这里被坑过）
![](http://upload-images.jianshu.io/upload_images/1418424-2af21333307eb3a9.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
**2、选择模拟器为iOS Device，按照下图提示操作**
![](http://upload-images.jianshu.io/upload_images/1418424-7bc615fccc2b5ab3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/1418424-c19d21a098b1861c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/1418424-65f93462cc10487f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
**3、修改.plist文件，两个.plist文件都要修改**
![](http://upload-images.jianshu.io/upload_images/1418424-b5bbbaa6ef02f373.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
**4、Archive在线打包，在真机状态下选择Product——>Archive，如果不是真机状态下，Archive会是灰色不可用的)。**
![](http://upload-images.jianshu.io/upload_images/1418424-cfc9ec1565889aae.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
**5、打包之后会生成一个 ipa文件 ，然后返回我的App~~在构建版本处，点击Application Loader 就会将其下载下来，然后通过该 软件把ipa文件上传到 appstore上**。
![](http://upload-images.jianshu.io/upload_images/1418424-db8de837edf24ad8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/1418424-e3dc777ee4d906f7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/1418424-a4cab58537691c6e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/1418424-1464e747698acfef.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
**打包过程中 会出现的问题 **
![](http://upload-images.jianshu.io/upload_images/1418424-712e7be97a55c0d6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
解决方案：[Your build settings specify a provisioning profile with the UUID “”, however, no such provisioning profile was found.解决方案](http://www.jianshu.com/p/1cd967ade901)
**application Loader 上传出现的错误 **
[application loader上传报90158错误](http://www.cnblogs.com/codeforcode/p/4995026.html)
![](http://upload-images.jianshu.io/upload_images/1418424-453475bf1c8a3d10.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
**6.发送成功后返回到我的App，刷新页面，在构建版本处就会有个 + 号，点击 + 号把发送过来的程序添加上去就行了**
**7.提交审核**
## 感觉不错的话 请粉一个 谢谢！！！！
文／随梦而飞飞（简书作者）
原文链接：http://www.jianshu.com/p/cea762105f7c
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
