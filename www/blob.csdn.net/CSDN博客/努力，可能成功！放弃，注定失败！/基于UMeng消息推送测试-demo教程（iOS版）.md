# 基于UMeng消息推送测试-demo教程（iOS版） - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年12月07日 10:46:24[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：112
基于UMeng消息推送测试-demo教程（iOS版）
眼看着就要过年了（2017本命年终于快过去了临来崭新的2018大发年），每个人的心里就像飞了一样，这个时候能够在办公间待下来的，每个人都是精英，突然想起了那首张含韵的歌《[放假了](https://link.jianshu.com?t=http%3A%2F%2Fwww.kuwo.cn%2Fyinyue%2F112848)》。
今天是2017年腊月25距离2018年仅有5天，依旧在外拼搏着，马路俩边的商铺陆陆续续关门（有钱没钱回家过年！！！）
好了，进入正题～～～
前年写个一篇《[基于UMeng第三方分享（新浪）测试-demo教程](https://www.jianshu.com/p/63ebb3da6a73)》教程，感兴趣的可以看一下，由于时间比较老旧可能有些操作流程更新了，但是不要紧，大体流程依旧在。
由于要做带有推送的app， 最终商量决定运用[UMeng](https://link.jianshu.com?t=http%3A%2F%2Fmobile.umeng.com%2Fpush%3Fspm%3Da211g2.181323.0.0.5c633604fV9moo)的SDK推送，这里的需求就是我们app自己的服务器通过umeng-sdk去推消息。
而不是通过Umeng的官网后台去编辑消息进行推送。这里需要安卓，iOS，以及我们的app java server 都需要进行umeng-sdk的集成。而今天这篇教程是通过Umeng的官网后台去编辑消息进行推送，后续再陆续进行更新以上需求教程
![](https://upload-images.jianshu.io/upload_images/1378846-8fb3c9fd6212b964.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
通过Umeng的官网后台去编辑消息进行推送到app
# 1.集成sdk
1.1.下载iOS端的[umeng-sdk](https://link.jianshu.com?t=http%3A%2F%2Fmobile.umeng.com%2Fcustom_sdk)
![](https://upload-images.jianshu.io/upload_images/1378846-4d42804938707f91.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
Push SDK 3.0.1
下载后的文件是如下这样的：
![](https://upload-images.jianshu.io/upload_images/1378846-5536ccfddef8b4e6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/844/format/webp)
Push SDK 3.0.1里的内容
其中：Common  // 公共组件是所有友盟产品的基础组件，必选
push    // Push组件
1.2.添加依赖库
基础依赖库
CoreTelephony.framework 获取运营商标识
    libz.tbd  数据压缩
    libsqlite.tbd  数据缓存
    SystemConfiguration.framework  判断网络状态
可选依赖库
  SecurityEnvSDK.framework 金融风控组件 
  UTDID.framework 设备标识ID组件
上述的可选依赖库在thirdparties文件夹里
![](https://upload-images.jianshu.io/upload_images/1378846-724057fc457ae9e0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/631/format/webp)
SecurityEnvSDK.framework 金融风控组件   UTDID.framework 设备标识ID组件
笔者在添加可选依赖库后，运行报错，我就把它给删除了（不添加可选依赖库）
1.3.选择SDK功能组件并下载，解压.zip文件得到相应组件包(例如：UMCommon.framework， UMPush.framework这来个库必须添加进来)。
XcodeFile —> Add Files to "Your Project"，在弹出Panel选中所下载组件包－>Add。（注：选中“Copy items if needed”）
![](https://upload-images.jianshu.io/upload_images/1378846-1b07b89f0cdf350c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/717/format/webp)
UMCommon.framework， UMPush.framework这来个库必须添加进来
添加依赖库，在项目设置target -> 选项卡General ->Linked Frameworks and Libraries 如下以及效果：
![](https://upload-images.jianshu.io/upload_images/1378846-9eb4c89d82bb7b00.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/864/format/webp)
整体效果
1.4.打开推送开关
点击项目---->TARGET---->Capabilities，将这里的Push Notification的开关打开，效果如图所示：
![](https://upload-images.jianshu.io/upload_images/1378846-9592178cabff60b3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/687/format/webp)
Push Notification的开关打开
同时点击项目---->TARGET---->Capabilities-->Background Modes--->Remote notifications也要勾选上，如下：
![](https://upload-images.jianshu.io/upload_images/1378846-35f454f02b54412c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/844/format/webp)
TARGET---->Capabilities-->Background Modes--->Remote notifications也要勾选上
至此工程的配置已经完成了，接下来就是代码的编写，但是这之前还需要进行证书的配置。
# 2.iOS证书配置指南
是为了在umeng官网上传自己的p12证书，注意需要测试证书和生产证书，都需要上传，已经你导出p12时设置的密码
**使用消息推送功能前，需要严格按照此文档配置相关证书。否则将会影响消息推送组件的正常使用。**
**本文档包括：**
[创建APP ID](https://link.jianshu.com?t=http%3A%2F%2Fdev.umeng.com%2Fpush%2Fios%2Flicense-configuration-guide%232)        // 类似于身份证标示
[创建并配置CSR文件](https://link.jianshu.com?t=http%3A%2F%2Fdev.umeng.com%2Fpush%2Fios%2Flicense-configuration-guide%233)         // **CSR是Cerificate Signing Request的英文缩写，即证书请求文件，在申请数字证书时由CSP(加密服务提供者)在生成私钥的同时也生成证书请求文件**
[创建并配置开发调试证书](https://link.jianshu.com?t=http%3A%2F%2Fdev.umeng.com%2Fpush%2Fios%2Flicense-configuration-guide%234)           // Certificates
[添加测试设备](https://link.jianshu.com?t=http%3A%2F%2Fdev.umeng.com%2Fpush%2Fios%2Flicense-configuration-guide%235)
[创建并配置Provisioning Profiles](https://link.jianshu.com?t=http%3A%2F%2Fdev.umeng.com%2Fpush%2Fios%2Flicense-configuration-guide%236)   // 配置概要文件
[创建并配置推送证书（开发环境&生产环境）](https://link.jianshu.com?t=http%3A%2F%2Fdev.umeng.com%2Fpush%2Fios%2Flicense-configuration-guide%237)
[导出推送证书的P12文件](https://link.jianshu.com?t=http%3A%2F%2Fdev.umeng.com%2Fpush%2Fios%2Flicense-configuration-guide%238)
[在U-Push后台上传P12推送证书](https://link.jianshu.com?t=http%3A%2F%2Fdev.umeng.com%2Fpush%2Fios%2Flicense-configuration-guide%239)
**[*以Ad Hoc方式打包应用](https://link.jianshu.com?t=http%3A%2F%2Fdev.umeng.com%2Fpush%2Fios%2Flicense-configuration-guide%2310)**
[iOS 10证书配置教程](https://link.jianshu.com?t=http%3A%2F%2Fdev.umeng.com%2Fpush%2Fios%2Flicense-configuration-guide%2311)
2.1创建APP ID
登录Apple Member Center ：[https://developer.apple.com/membercenter](https://link.jianshu.com?t=https%3A%2F%2Fdeveloper.apple.com%2Fmembercenter)，选择**certificates，identifiers&profiles **选项，如下图所示：
![](https://upload-images.jianshu.io/upload_images/1378846-754da4edd8275963.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/701/format/webp)
选择certificates，identifiers&profiles 选项
选择identity中的APP IDs，如下图所示：
![](https://upload-images.jianshu.io/upload_images/1378846-4dcaad0df686d90a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/704/format/webp)
Identifiers也就是一个标示相当于身份证号码
**注意：**创建Explicit App ID时，要注意此时设置的Bundle ID要与APP以后的Bundle ID一致,如下图所示：
![](https://upload-images.jianshu.io/upload_images/1378846-85cc081032c954ad.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/709/format/webp)
注意此时设置的Bundle ID要与APP以后的Bundle ID一致
![](https://upload-images.jianshu.io/upload_images/1378846-4cfbeb793da17a32.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
![](https://upload-images.jianshu.io/upload_images/1378846-9647a81cf422295b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
选择**Push Notification **来开通Push功能，如下图所示：
![](https://upload-images.jianshu.io/upload_images/1378846-45dffb9b2424be79.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/710/format/webp)
Push Notification 来开通Push功能
点击下方的**register **按钮，几秒钟之后下方会变成一个**Done **，表示APP ID已经创建完成，如下图所示：
![](https://upload-images.jianshu.io/upload_images/1378846-6bfe6994371e5280.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/706/format/webp)
![](https://upload-images.jianshu.io/upload_images/1378846-32cd877ebec64838.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/683/format/webp)
![](https://upload-images.jianshu.io/upload_images/1378846-095551b2a2bf2618.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/648/format/webp)
**注意：** 创建的APP ID要与自己项目的Bundle ID保持一致。如果已经有APP ID，请确保自己的APP ID已经开通了Push功能，如果没有开通，可以点击APP ID，在其底部有一个**Edit**按钮，点击后选中Push服务并创建，如下图所示：
![](https://upload-images.jianshu.io/upload_images/1378846-67fb086308e523d5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/642/format/webp)
**至此，APP ID创建完毕**
**2.2.创建CSR文件**
***如果你已经有CSR文件，请继续进行下一步的设置。（跳过2.2步骤的操作）***
**CSR是Cerificate Signing Request的英文缩写，即证书请求文件。证书申请者在申请数字证书时由CSP(加密服务提供者)在生成私钥的同时也生成证书请求文件。证书申请者只要把CSR文件提交给证书颁发机构后，证书颁发机构使用其根证书私钥签名就生成了证书公钥文件，也就是颁发给用户的证书。**
**在电脑中找到钥匙串----->钥匙串访问------>证书管理------>从证书颁发机构请求证书，如下图所示：**
![](https://upload-images.jianshu.io/upload_images/1378846-39aeb481807ef2a7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/663/format/webp)
![](https://upload-images.jianshu.io/upload_images/1378846-402f674994ac0129.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/701/format/webp)
设置完证书信息后，点击“继续”，然后选择存储位置(保存到桌面)，点击存储，**至此，CSR文件创建成功。**
**文件如下：PushTestCertificateSigningRequest.certSigningRequest**
![](https://upload-images.jianshu.io/upload_images/1378846-07d52731f2bc2fac.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/265/format/webp)
PushTestCertificateSigningRequest.certSigningRequest
**2.3.创建开发调试证书**
***如果你已经有开发调试证书，请继续进行下一步的设置。（跳过2.3步骤的操作）***
***选择Development（如果是发布上线，需要选择Distribution），如下图所示：***
![](https://upload-images.jianshu.io/upload_images/1378846-4996c7a6f09afe30.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/703/format/webp)
在这里我们先选测试证书（开发证书），后文中也会进行生产证书操作（Distribution），应为umeng官网后台同时需要开发证书（也就是测试证书）和生产证书
**之后，你将看到如下四个证书。这一步，我们选择iOS App Development证书，如下图所示：**
![](https://upload-images.jianshu.io/upload_images/1378846-9458f7f5e13bcd34.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/706/format/webp)
![](https://upload-images.jianshu.io/upload_images/1378846-65e1c4eb4a49f116.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/675/format/webp)
![](https://upload-images.jianshu.io/upload_images/1378846-186c1801b07d1751.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/686/format/webp)
![](https://upload-images.jianshu.io/upload_images/1378846-aed74abd1fb856c0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/694/format/webp)
在choose file...中选择本地的CertificateSigningRequest（CSR）文件，然后点击继续，如下图所示：
在**choose file...**中选择本地的**CertificateSigningRequest（CSR）**文件，然后点击继续，如下图所示：
![](https://upload-images.jianshu.io/upload_images/1378846-c857edbe5d32b635.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/694/format/webp)
![](https://upload-images.jianshu.io/upload_images/1378846-53d985ad8a837e1b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/676/format/webp)
点击Download，将证书下载到本地，双击下载的开发调试证书（文件名为：iOS_development.cer）
# 点击**Download**，将证书下载到本地，*双击*下载的开发调试证书（文件名为：**iOS_development.cer**），就可以在钥匙串访问中的我的证书中找到。（这里必须要注意的是：必须要双击下，否则，后面的推送会出现状态：失败 
# 消息：device-token全部失败
# 的情况自己不注意的坑）
![](https://upload-images.jianshu.io/upload_images/1378846-adb26157689dc70a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/564/format/webp)
造成的失败，笔者就失败在这里了
**下载后的文件如下：**
![](https://upload-images.jianshu.io/upload_images/1378846-d56cd9cc7ad8c84e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/146/format/webp)
ios_development.cer
**至此，开发调试证书设置完毕**
**2.4. 添加测试设备**
**测试推送需要添加测试设备，添加方法如下所示：**
![](https://upload-images.jianshu.io/upload_images/1378846-064fe858875b76e3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/684/format/webp)
![](https://upload-images.jianshu.io/upload_images/1378846-ee3b4ac2fc715db7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/695/format/webp)
![](https://upload-images.jianshu.io/upload_images/1378846-edcf918899743149.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/685/format/webp)
![](https://upload-images.jianshu.io/upload_images/1378846-066b0b6d86f4c780.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/687/format/webp)
**2.5.创建Provisioning Profiles文件 （配置概要文件）**
**选择Development，创建开发环境的Provisioning Profiles（如果是发布上线，需要选择Distribution），如下图所示：**
![](https://upload-images.jianshu.io/upload_images/1378846-05b91be1a54de053.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/721/format/webp)
开发的配置概要文件
![](https://upload-images.jianshu.io/upload_images/1378846-0b78991f8597749b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/680/format/webp)
开发的配置概要文件
创建**新的Provisioning Profiles**，并选择之前生成好的App ID。如下图所示：
![](https://upload-images.jianshu.io/upload_images/1378846-e1191629b535af56.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/693/format/webp)
选择之前生成好的App ID
**选中创建的证书：**
![](https://upload-images.jianshu.io/upload_images/1378846-af7faef694e323d2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/649/format/webp)
选中创建的证书：
**选中测试设备：**
![](https://upload-images.jianshu.io/upload_images/1378846-62237f50765a7c9c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/678/format/webp)
选中测试设备：
![](https://upload-images.jianshu.io/upload_images/1378846-82475a3588568dab.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/714/format/webp)
把创建的**Provisioning Profiles**,**调试证书**，**推送证书**下载下来（文件名：umengpushtestdemo.mobileprovision）以后双击证书
![](https://upload-images.jianshu.io/upload_images/1378846-49faa9a8a5d779ae.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/644/format/webp)
文件如下：umengtestdemo.mobileprovision
![](https://upload-images.jianshu.io/upload_images/1378846-77e69f6c0ad21aa7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/184/format/webp)
umengtestdemo.mobileprovision
2.6.创建推送证书
选择**Apple Push Notification service SSL（Sandbox）**
![](https://upload-images.jianshu.io/upload_images/1378846-c0087dd3fab4c8b3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/716/format/webp)
这里笔者先选开发的推送证书
**注意：推送证书分为开发环境和生产环境，如果需要生成生产环境的推送证书请选择： Apple Push Notification service SSL （Sandbox&Production），如下图所示：**
![](https://upload-images.jianshu.io/upload_images/1378846-e375f28f5c78fed1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/646/format/webp)
Apple Push Notification service SSL （Sandbox&Production）
选中APP ID后，点击**continue**，直到上传CRS文件，如下图所示：
![](https://upload-images.jianshu.io/upload_images/1378846-12bf15a153f8b612.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/653/format/webp)
![](https://upload-images.jianshu.io/upload_images/1378846-308264435ef43313.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/685/format/webp)
![](https://upload-images.jianshu.io/upload_images/1378846-eec669a332394d6e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/697/format/webp)
选择上方的PushTestCertificateSigningRequest.certSigningRequest文件
![](https://upload-images.jianshu.io/upload_images/1378846-d6804f6dce33a914.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/623/format/webp)
# 点击下载，将证书下载到本地后，双击下载的开发环境推送证书（文件名为：aps_development.cer），就可以在钥匙串访问中的我的证书中找到。
文件如下：aps_development.cer
![](https://upload-images.jianshu.io/upload_images/1378846-204fc47c13a1fc08.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/205/format/webp)
aps_development.cer
2.6。导出推送证书的P12文件
**注意：在钥匙串访问（钥匙圈）的我的证书中选择刚刚导入的证书，选择导出，注意不要选中私钥，并设置相应密码即可。**
![](https://upload-images.jianshu.io/upload_images/1378846-d507906428716002.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/762/format/webp)
![](https://upload-images.jianshu.io/upload_images/1378846-5a2423559f77b9cd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/706/format/webp)
正确的导出方式：
![](https://upload-images.jianshu.io/upload_images/1378846-5cdeb50d1e1a491b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/726/format/webp)
**错误的导出方式如下：**
![](https://upload-images.jianshu.io/upload_images/1378846-22600ad02ffb6485.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/626/format/webp)
**注意：此步骤所设置的密码将在之后在U-Push后台上传推送证书时所使用，且密码只能包含大小写（az，AZ），数字（0-9），特殊字符（。-_） ，其他元素将会被过滤。**
![](https://upload-images.jianshu.io/upload_images/1378846-3f4f4be6fb3689aa.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/712/format/webp)
生产环境的推送证书也是以同样的方式导出。
**至此，P12证书导出完毕。**
**2.7。在U型推后台上传P12推送证书**
**点击应用进入应用信息选项，即可上传已被导出的P12证书，如下图所示：**
![](https://upload-images.jianshu.io/upload_images/1378846-1252f3ea9fbb8a63.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/671/format/webp)
然后以同样的上述方式，配置成产的证书，并导出P12文件设置密码，上传到umeng官网后台的生产证书项里
# 3.工程代码的编写
![](https://upload-images.jianshu.io/upload_images/1378846-f41cbf683b2aa1b7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/765/format/webp)
![](https://upload-images.jianshu.io/upload_images/1378846-b8ab8393b8a30932.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/862/format/webp)
![](https://upload-images.jianshu.io/upload_images/1378846-b1c12e9ff014a62c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/845/format/webp)
4.在umeng官网后台的操作
，上述步骤中 - （void）application：（UIApplication *）application didRegisterForRemoteNotificationsWithDeviceToken：（NSData *）deviceToken {里会打印设备令牌的值64位的字符，添加到umeng官网后台的测试设备里
![](https://upload-images.jianshu.io/upload_images/1378846-68ad2296a42b1b56.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
然后就是官网后台发送推送的操作，可以用单播的形式：
![](https://upload-images.jianshu.io/upload_images/1378846-a653f291b8b6a240.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
最终实现的效果图：
![](https://upload-images.jianshu.io/upload_images/1378846-4ad70ac795a51f75.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/640/format/webp)
![](https://upload-images.jianshu.io/upload_images/1378846-c8ec551a5ca67678.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/640/format/webp)
# 下面说一下关于umeng推送过程中遇到的坑：如下图：
![](https://upload-images.jianshu.io/upload_images/1378846-9fa49533d4d999e0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/882/format/webp)
设备令牌全部失败
![](https://upload-images.jianshu.io/upload_images/1378846-021e672c385f9c98.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/761/format/webp)
设备令牌全部失败
![](https://upload-images.jianshu.io/upload_images/1378846-811ca78e4eadccf0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
DeviceTokenNotForTopic
这里一系列的原因：umeng官方并没有给出具体的解决方案，联系客户又是那么费时费力
## 解决方案1：
添加测试机设备的DeviceToken无效或者失败，报红色或者黄色都是不行的，
![](https://upload-images.jianshu.io/upload_images/1378846-9687df49420bd932.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
# 解决方案2：
配置证书时选的appID必须和x-code里的Bundle Identifier相同，这是最大的一个坑，详细参见：[http](https://link.jianshu.com?t=http%3A%2F%2Fbbs.umeng.com%2Fthread-16274-1-2.html)：//bbs.umeng.com/thread-16274-1-2.html
![](https://upload-images.jianshu.io/upload_images/1378846-b026d34834e34f06.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/715/format/webp)
# 演示地址：
# [https://github.com/mrzhao12/UMengPushTest](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fmrzhao12%2FUMengPushTest)
作者：ttdiOS 
链接：HTTPS：//www.jianshu.com/p/b5078166d9f6 
来源：书繁简
繁简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
