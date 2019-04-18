# iOS开发如何实现消息推送机制 - weixin_33985507的博客 - CSDN博客
2013年10月28日 12:12:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3

转自：http://blog.csdn.net/r8hzgemq/article/details/7874914
**Push的原理：**
　　Push 的工作机制可以简单的概括为下图
![](http://dl.iteye.com/upload/attachment/617605/322782e2-f9a1-3836-9ffe-9257e2fca12c.jpg)
　　图中，Provider是指某个iPhone软件的Push服务器，这篇文章我将使用.net作为Provider。
　　APNS 是Apple Push Notification Service(Apple Push服务器)的缩写，是苹果的服务器。
**上图可以分为三个阶段。**
　　第一阶段：.net应用程序把要发送的消息、目的iPhone的标识打包，发给APNS。
　　第二阶段：APNS在自身的已注册Push服务的iPhone列表中，查找有相应标识的iPhone，并把消息发到iPhone。
　　第三阶段：iPhone把发来的消息传递给相应的应用程序， 并且按照设定弹出Push通知。
转播到腾讯微博
![iOS开发如何实现消息推送机制](http://img1.gtimg.com/hb/pics/hv1/64/234/914/59492584.jpg)
　　从上图我们可以看到。
　　1、首先是应用程序注册消息推送。
　　2、 IOS跟APNS Server要deviceToken。应用程序接受deviceToken。
　　3、应用程序将deviceToken发送给PUSH服务端程序。
　　4、 服务端程序向APNS服务发送消息。
　　5、APNS服务将消息发送给iPhone应用程序。
　　无论是iPhone客户端跟APNS,还是Provider和APNS都需要通过证书进行连接的。下面我介绍一下几种用到的证书。
**几种证书：**
　　一、*.certSigningRequest文件
　　1、生成Certificate Signing Request (CSR)：
转播到腾讯微博
![iOS开发如何实现消息推送机制](http://img1.gtimg.com/hb/pics/hv1/65/234/914/59492585.jpg)
2、填写你的邮箱和Common Name，这里填写为PushChat。选择保存到硬盘。
转播到腾讯微博
![iOS开发如何实现消息推送机制](http://img1.gtimg.com/hb/pics/hv1/66/234/914/59492586.jpg)
　　这样就在本地生成了一个PushChat.certSigningRequest文件。
**二、生成*.p12文件**
　　1、导出密钥，并输入你的密码。
转播到腾讯微博
![iOS开发如何实现消息推送机制](http://img1.gtimg.com/hb/pics/hv1/67/234/914/59492587.jpg)
输入你的密码：
转播到腾讯微博
![iOS开发如何实现消息推送机制](http://img1.gtimg.com/hb/pics/hv1/68/234/914/59492588.jpg)
　　这样就生成了一个PushChatKey.p12文件。
**三、新建一个App ID 和SSL certificate文件**
　　1、用你的付过费的apple帐号登录到iOS Provisioning Portal。新建一个App ID。
　　Description:中输入PushChat
　　Bundle Seed ID:默认选择Generate New
　　Bundle Identifier：输入com.mysoft.PushChat
　　点击提交
转播到腾讯微博
![iOS开发如何实现消息推送机制](http://img1.gtimg.com/hb/pics/hv1/69/234/914/59492589.jpg)
　　这样就会生成下面这条记录：
　　点击配置：
转播到腾讯微博
![iOS开发如何实现消息推送机制](http://img1.gtimg.com/hb/pics/hv1/70/234/914/59492590.jpg)
出现下面界面，点击继续：
转播到腾讯微博
![iOS开发如何实现消息推送机制](http://img1.gtimg.com/hb/pics/hv1/71/234/914/59492591.jpg)
　　这里我们选择前面生成好的PushChat.certSigningRequest文件，点击生成。

转播到腾讯微博
![iOS开发如何实现消息推送机制](http://img1.gtimg.com/hb/pics/hv1/72/234/914/59492592.jpg)
正在生成
转播到腾讯微博
![iOS开发如何实现消息推送机制](http://img1.gtimg.com/hb/pics/hv1/73/234/914/59492593.jpg)
　　生成完毕，我们把它下载下来。命名为aps_developer_identity.cer。

转播到腾讯微博
![iOS开发如何实现消息推送机制](http://img1.gtimg.com/hb/pics/hv1/74/234/914/59492594.jpg)
　　点击完成，你会发现状态变成Enabled。
　　到现在为止，我们已经生成了3个文件。
　　1、PushChat.certSigningRequest
　　2、PushChatKey.p12
　　3、aps_developer_identity.cer
　　现在我们创建一个简单的iPhone应用程序。
　　1、打开Xcode，选择创建一个View-based Application。命名如下图：
转播到腾讯微博
![iOS开发如何实现消息推送机制](http://img1.gtimg.com/hb/pics/hv1/75/234/914/59492595.jpg)
　　2、在PushChatAppDelegate中的didFinishLaunchingWithOptions方法中加入下面代码：
　　通过registerForRemoteNotificationTypes方法，告诉应用程序，能接受push来的通知。
　　3、在xcode中运行，会弹出下面的提示框：
转播到腾讯微博
![iOS开发如何实现消息推送机制](http://img1.gtimg.com/hb/pics/hv1/76/234/914/59492596.jpg)
　　选择OK。表示此应用程序开启消息通知服务。
　　在 PushChatAppDelegate.m代码中添加下面方法获取deviceToken ：
　　- (void)application:(UIApplication*)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData*)deviceToken{NSLog(@My token is: %@, deviceToken);}- (void)application:(UIApplication*)application didFailToRegisterForRemoteNotificationsWithError:(NSError*)error{NSLog(@Failed to get token, error: %@, error);}
　　获取到的deviceToken，我们可以通过webservice服务提交给.net应用程序，这里我简单处理，直接打印出来，拷贝到.net应用环境中使用。
　　发送通知的.net应用程序出来需要知道deviceToken之外，还需要一个与APNS连接的证书。
　　这个证书可以通过我们前面生成的两个文件中得到。
**使用OpenSSL**
　　1、将aps_developer_identity.cer转换成 aps_developer_identity.pem格式。
　　openssl x509 -in aps_developer_identity.cer -inform DER -out aps_developer_identity.pem -outform PEM
　　2、将p12格式的私钥转换成pem，需要设置4次密码，密码都设置为：abc123。
　　openssl pkcs12 -nocerts -out PushChat_Noenc.pem -in PushChat.p12
　　3、用certificate和the key 创建PKCS#12格式的文件。
　　openssl pkcs12 -export -in aps_developer_identity.pem -inkey PushChat_Noenc.pem -certfile PushChat.certSigningRequest -name aps_developer_identity -out aps_developer_identity.p12
　　这样我们就得到了在.net应用程序中使用的证书文件：aps_developer_identity.p12。
　　在.net应用程序中发送通知。
　　有个开源的类库：apns-sharp。
　　地址是：http://code.google.com/p/apns-sharp/。
　　我们下载源代码，对里面的JdSoft.Apple.Apns.Notifications做相应的调整就能用了。
　　我们根据DeviceToken和p12File对JdSoft.Apple.Apns.Notifications.Test做相应的调整，如下图。
转播到腾讯微博
![iOS开发如何实现消息推送机制](http://img1.gtimg.com/hb/pics/hv1/77/234/914/59492597.jpg)
　　这样就OK了。
　　效果：
　　通知的代码：
　　for (int i = 1; i
　　用手机拍的ipad上面的显示：
转播到腾讯微博
![iOS开发如何实现消息推送机制](http://img1.gtimg.com/hb/pics/hv1/78/234/914/59492598.jpg)

转播到腾讯微博
![iOS开发如何实现消息推送机制](http://img1.gtimg.com/hb/pics/hv1/79/234/914/59492599.jpg)
　　总结：这篇文章主要是详细的讲述了ios消息推送机制的实现，如何通过.net应用程序发送消息给ios应用程序。
