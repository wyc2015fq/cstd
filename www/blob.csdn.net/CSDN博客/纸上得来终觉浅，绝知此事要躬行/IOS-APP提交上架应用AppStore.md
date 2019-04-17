# IOS-APP提交上架应用AppStore - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年06月27日 17:48:48[boonya](https://me.csdn.net/boonya)阅读数：999








原文地址：[http://rdcqii.hundsun.com/portal/article/457.html](http://rdcqii.hundsun.com/portal/article/457.html)


做运营多年，经常遇到以下场景：

![](http://rdcqii.hundsun.com/portal/data/upload/201605/f_d8f9ce86918501705280010b32e500e8.jpg)

为了让我们friendship更加长久，我决定献出我积累多年的干货——iOS-APP提交上架流程详解，包你看完此文，再也不愁。




**1.证书的导出**

**1.前期准备**

首先你需要有一个苹果的开发者帐号，一个Mac系统。如果没有帐号可以在打开http://developer.apple.com/申请加入苹果的开发者计划。申请开发者可以参考百度经验“[苹果开发者帐号免费申请](http://jingyan.baidu.com/article/c85b7a64024ad0003aac956d.html)，打开“[苹果开发者网站](https://developer.apple.com/)”，并登录到苹果会员中心（MemberCenter），登录以后可以看到下面这个界面，列出了你开发需要的一些工具，见下图：

![](http://rdcqii.hundsun.com/portal/data/upload/201605/f_630073afef11b2b6f884fb1f710d1ce8.png)

 选择Certificates, Identifiers & Profiles，申请证书、App ID、设备等配置都在此处，见下图：

![](http://rdcqii.hundsun.com/portal/data/upload/201605/f_346282d8a9e90b74e3449ac1505c1f03.png)

**2.申请App ID 2**

选择App IDs -->点击+创建一个新的App ID

![](http://rdcqii.hundsun.com/portal/data/upload/201605/f_5331bfccbba3f64b274c9cf70f638609.png)

其中有两项需要你自己填：

第一项Name，用来描述你的App ID，这个随便填，没有什么限制，最好是项目名称，这样方便自己辨识（不允许中文哦）。

第二项Bundle ID (App ID Suffix)，这是你App ID的后缀，需要仔细填写。因为这个内容和你的程序直接相关，后面很多地方要用到，最好是com.yourcompany.yourappname的格式，我用的是项目中的Bundle ID。

第三项App Services，默认会选择2项，不能修改，只需要选择上自己需要的服务即可，不知道在哪里看到说是必须要选上推送，否则后面就没有办法修改了，但是我自己测试的是可以修改的，然后点击Continue确认,没有错的话直接点击Submit后点击Done即完成App ID的创建。

![](http://rdcqii.hundsun.com/portal/data/upload/201605/f_6fdd1bc3bbc93cf499f424b5b4911550.png)




## 3.创建证书

3.1 先创建一个证书请求文件

这儿需要一个Mac系统，以下内容以OS X Yosemite系统为例。首先打开应用程序-实用工具-钥匙串访问（KEY CHAIN）,在证书助理中，选择"从证书颁发机构求证书"，如下图：

![](http://rdcqii.hundsun.com/portal/data/upload/201605/f_488bc2d854b9de2040aba764ac647b61.jpg)

在下图所示的界面，你的电子邮件地址：填你申请idp的电子邮件地址，常用名称，默认就好，CA空，选择存储到磁盘，点击"继续"：

![](http://rdcqii.hundsun.com/portal/data/upload/201605/f_aa9c10dcf9bbece47ff4f901604cfe06.jpg)

选择保存的位置，比如选择桌面。下一步点击完成，你就可以看到你的桌面多了一个CertificateSigningRequest.certSigningRequest的证书请求文件。

注：CSR文件尽量每个证书都制作一次，将常用名称区分开来，因为该常用名称是证书中的密钥的名字。




3.2 制作Provisioning Profile证书文件

开发者证书的制作：登录到开发者中心，找到证书配置的版块，选择，点击右上角的加号：

![](http://rdcqii.hundsun.com/portal/data/upload/201605/f_a5dcf2f417e434c3cf861896942aafa7.jpg)

发布证书和开发者证书需要操作两次，分别创建，开发者证书用于真机调试，发布证书用于提交到AppStore。

![](http://rdcqii.hundsun.com/portal/data/upload/201605/f_12cfe7f13afb837309050746392ced4a.jpg)




![](http://rdcqii.hundsun.com/portal/data/upload/201605/f_cb58ff02500924ef8b489c671493bfe5.jpg)

直接选择我们上一步保存在桌面的CertificateSigningRequest.certSigningRequest的证书请求文件即可。提交上去后就会生成一个cer证书，如图所示，有效期为一年。

![](http://rdcqii.hundsun.com/portal/data/upload/201605/f_47c296a72aca9af978cd884584719cc0.png)

点击DownLoad，双击安装。（如果安装不上，可以直接将证书文件拖拽到钥匙串访问的列表中）

Provisioning Profile证书制作：到这一步证书还没凑全，想提交AppStore还需要PP证书。点击Provisioning Profiles，然后点击右上角的加号。

![](http://rdcqii.hundsun.com/portal/data/upload/201605/f_6a5a1904f4509badba1fca28e1542f13.jpg)

该流程也需要进行两次，分别创建开发用的PP证书和发布的PP证书。

![](http://rdcqii.hundsun.com/portal/data/upload/201605/f_5260d4d453b1ebd650af4b1e050fa333.jpg)

下一步的选择项里会自动检测匹配你的AppID，另外下拉项中还可以选择wildCard格式。

注意：wildCard格式的证书没有推送，PassCard等服务的应用，慎重选择。

这里有一个区别，因为PP证书的开发者证书需要真机调试，所以我们需要绑定真机，这里因为之前我添加过一些设备，所以这里就可以直接全选添加，如果没有的话，需要将真机的uuid复制出来在此添加，在发布PP文件中，是没有这一步的。

![](http://rdcqii.hundsun.com/portal/data/upload/201605/f_c25c1db39584e6361c8f83ccc96e5656.png)

之后就是输入一个PP证书的名字了，然后生成，下载保存，双击就添加到Xcode中，这样在真机调试或者发布时，就可以分别有不同的PP证书与其对应。




3.3 配置项目环境

3.3.1    Account的配置

Xcode——>Preferences 就会出现Account的配置界面，如图：

![](http://rdcqii.hundsun.com/portal/data/upload/201605/f_87320a60ed1d54efa486bc2e3550345c.jpg)

点击+可以选择Add AppleID；点击View Details可以查看该Apple ID下的Certificates和Provisioning Profile证书文件，在这里你可以点击下载。

3.3.2    项目的配置

在项目Targets下的Identity中，Team选择对应的Apple ID 即可。在XCode的项目设置里，Provisioning Profile就可以选择对应的证书。这样前期的准备工作就全部算完成了。

![](http://rdcqii.hundsun.com/portal/data/upload/201605/f_016093dad628ea93add8b4cae85abee0.jpg)



**2.ipa打包**

**1.Archive****在线打包**

在真机状态下选择Product——>Archive(如果不是真机状态下，Archive会是灰色不可用的)，成功会弹出如下图：

![](http://rdcqii.hundsun.com/portal/data/upload/201605/f_81c3df399fc540da1dadd1280ad086a5.jpg)




**3.****ipa提交App Store**



**1.在itunes中创建程序**

登录开发者中心，选择Itunes Connect，进入后选择“我的App”如下图：

![](http://rdcqii.hundsun.com/portal/data/upload/201605/f_c2bab325b4cf58df8eebc22e36fc8e89.png)

点击左上角的+选择“新建App”，根据自己的app然后填写相应的信息即可

![](http://rdcqii.hundsun.com/portal/data/upload/201605/f_28c03ba4bcb35013a67f4427186c6856.png)

△选择语言的时候，简体中文是Simplified
 Chinese

△ 套装ID与SKU主要是app的唯一标识吧，我是用的项目中Bundle Identifier的内容，即com.company.projectname。（App信息以及价格与定价根据app不同自己填写就可以了）

![](http://rdcqii.hundsun.com/portal/data/upload/201605/f_014782240b2dc70df44c075d6ff20a2e.jpg)

△上传App Icon的时候，注意那个问号哦，需要上传1024*1024的，而且不能有圆角效果哦。

△ 这里需要不同屏幕的截图，可以直接用***运行后截图。待模拟器运行开始的时候，按住cmd+s, 模拟器的屏幕截图就直接保存在桌面上了。

△ 上传的时候会提示“无法载入文件”的问题。原因是：截图保存的文件名有中文，修改下截图名称即可。

△ 屏幕尺寸大家都可以查到，为了自己方便，简单备注下。

△ipa包的上传：我使用的是Applicaton
 Loader。点击选取，选择导出的ipa的文件，然后选择对应的信息，成功之后，30分钟~1小时差不多久可以在“构建版本”。首次的话会直接提交app，如果有多个版本的话，需要    在这里选择对应需要审核的版本即可。

△电话号码中需要加中国的区号+86，如：+86151678xxxxx



