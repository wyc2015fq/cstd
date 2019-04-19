# 苹果推送(APNs)ios push小结 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年08月30日 09:47:49[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：725
个人分类：[未分类](https://blog.csdn.net/elesos/article/category/6361263)
## 简介
推送服务APNs（Apple Push Notification services）
苹果的推送服务APNs基本原理简单来说就是：苹果利用自己专门的推送服务器（APNs）接收来自我们自己的应用服务器的需要被推送的信息，然后推送到指定的iOS设备上，然后由设备通知到我们的应用程序，设备以通知或者声音的形式通知用户有新的消息。推送的前提是装有我们应用的设备需要向APNs服务器注册，注册成功后APNs服务器会返给我们一个device_token，拿到这个token后我们将这个token发给我们自己的应用服务器，当有需要被推送的消息时，我们的应用服务器会将消息按指定的格式打包，然后结合设备的device_token一并发给APNs服务器，由于我们的应用和APNs维持一个基于TCP的长连接，APNs将新消息推送到我们设备上，然后在屏幕上显示出新消息来。
1.Device连接APNs服务器并携带设备序列号
2.连接成功，APNs产生device_token并返回给Device
3.Device携带获取的device_token向我们自己的应用服务器注册,一定要将device_token发送给我们的应用服务器，因为在推送过程中，首先是由我们的应用服务器将需要推送的消息结合device_token按指定格式（后面会提到）打包然后发送给APNS服务器，然后由APNS服务器推送给我们的设备。
推送消息是个JSON结构体，最大值是 256 字节。alert标签的内容就是会显示在用户手机上的推送信息，badge显示的数量（注意是整型）是会在应用Icon右上角显示的数量，提示有多少条未读消息等，sound就是当推送信息送达是手机播放的声音，传defalut就标明使用系统默认声音，如果传比如“beep.wav”就会播放在我们应用工程目录下名称为beep.wav的音频文件。
要在这个结构中新增自定义数据，请加在aps空间之外。比如后台推送消息给应用同时要求应用打开某个页面：
{"aps":{"alert":"张三:国庆回家吗？","sound":"default"},"msg":"这里是额外消息，不显示在通知界面上。"}
## 具体步骤
打开“钥匙串”程序，（证书助理->从证书颁发机构请求证书），只填邮箱和常用名称，ca不用填，然后保存.certSigningRequest文件到磁盘。
在[iOS Dev Center](https://developer.apple.com/devcenter/ios/index.action)点击App IDs进入App ID列表。
为 App 开启 Push Notification 功能。(推送证书分为两个版本，一个是Development版，一个是Production版，分别对应开发证书和发布证书。)
上传刚才生成的.certSigningRequest文件，生成aps_development.cer推送证书，双击安装。
打开“钥匙串”程序，（选择登录与我的证书选项后）找到IOS Push Services那条。
右键导出，存储为cert.p12,(输入密码P*d)，然后将该证书的折叠打开，导出“专用密钥”。存储为key.p12
接下来打开终端生成cert.pem
openssl pkcs12 -clcerts-nokeys-out cert.pem -in cert.p12
生成key.pem（先输入导出key.p12时设置的密码P*****d, 然后设置一个新的密码,如pushpwd,这个密码后面php推送时要设置）
openssl pkcs12 -nocerts-out key.pem -in key.p12
合并成ck.pem
**cat** cert.pem key.pem **>** ck.pem
将这个ck.pem放在服务器上，与发送脚本放在同一个目录下。 测试这个key
openssl s_client -connect gateway.sandbox.push.apple.com:2195-cert cert.pem -key ck.pem
## iOS程序处理
应用如何获取deviceToken。应用必须先注册使用push功能。
-(BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary*)launchOptions
{*//Let teh device know we want to receive push notifications*[[UIApplication sharedApplication] registerForRemoteNotificationTypes:(UIRemoteNotificationTypeBadge | UIRemoteNotificationTypeSound | UIRemoteNotificationTypeAlert)];
    returnYES;
}-(void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData*)deviceToken
{
    NSLog(@"My token is:%@", deviceToken);
}-(void)application:(UIApplication *)application didFailToRegisterForRemoteNotificationsWithError:(NSError*)error
{
    NSLog(@"Failed tp get token, error:%@", error);
}
客户端接收push消息
如果应用已经启动，将调用AppDelegate的方法：
 -(void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary*)userInfo {*// userInfo 就是push消息的Payload*}
 
如果应用还没有启动，通过push冷启动后，仍然能在启动后获取Payload：
 -(BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary*)launchOptions {NSDictionary* userInfo =[launchOptions objectForKey:UIApplicationLaunchOptionsRemoteNotificationKey];
      NSLog(@"Payload: %@", userInfo);
  }
## php推送程序
**<?php***/* ini_set("display_errors", "On");error_reporting(E_ALL | E_STRICT); */**// Put your device token here (without spaces):*$deviceToken='af93167751347b0909a05b97fefa1c7525dbbad0608cd63d19b11cbeb4a7878c';*// Put your private key's passwd here:*$passphrase='这时填你生成key.pem时设置的密码';*// Put your alert message here:*$message='Welcome to www.elesos.com';*////////////////////////////////////////////////////////////////////////////////*$ctx=stream_context_create();stream_context_set_option($ctx,'ssl','local_cert','client.pem');stream_context_set_option($ctx,'ssl','passphrase',$passphrase);*// Open a connection to the APNS server*$fp=stream_socket_client('ssl://gateway.sandbox.push.apple.com:2195',$err,$errstr,60, STREAM_CLIENT_CONNECT|STREAM_CLIENT_PERSISTENT,$ctx);if(!$fp)exit("Failed to connect: **$err****$errstr**". PHP_EOL);echo'Connected to APNS'. PHP_EOL;*// Create the payload body*$body['aps']=array('alert'=>$message,'badge'=>1,'sound'=>'default');*// Encode the payload as JSON*$payload=json_encode($body);*// Build the binary notification*$msg=chr(0).pack('n',32).pack('H*',$deviceToken).pack('n',strlen($payload)).$payload;*// Send it to the server*$result=fwrite($fp,$msg,strlen($msg));if(!$result)echo'Message not delivered'. PHP_EOL;elseecho'Message successfully delivered'. PHP_EOL;*// Close the connection to the server*fclose($fp);
## 参考
[https://developer.apple.com/library/IOs/documentation/NetworkingInternet/Conceptual/RemoteNotificationsPG/Chapters/ApplePushService.html](https://developer.apple.com/library/IOs/documentation/NetworkingInternet/Conceptual/RemoteNotificationsPG/Chapters/ApplePushService.html)
[一步一步实现iOS应用PUSH功能](http://tanqisen.github.io/blog/2013/02/27/ios-push-apns/)
[http://www.dapps.net/dev/code/apple-push-notification-services-tutorial-part-1.html](http://www.dapps.net/dev/code/apple-push-notification-services-tutorial-part-1.html)
[一步一步教你做ios推送](http://blog.csdn.net/showhilllee/article/details/8631734)
[http://www.cnblogs.com/gpwzw/archive/2012/03/31/apple_push_notification_services_tutorial_part_1-2.html](http://www.cnblogs.com/gpwzw/archive/2012/03/31/apple_push_notification_services_tutorial_part_1-2.html)
[http://www.cnblogs.com/gugupluto/p/3534118.html](http://www.cnblogs.com/gugupluto/p/3534118.html)
[http://blog.csdn.net/kylinbl/article/details/6729369](http://blog.csdn.net/kylinbl/article/details/6729369)
[苹果推送机制APNs（一）](http://blog.csdn.net/ryantang03/article/details/8482259)
[苹果推送机制APNs（二）](http://blog.csdn.net/ryantang03/article/details/8540362)
[http://docs.jpush.cn/pages/viewpage.action?pageId=1343727](http://docs.jpush.cn/pages/viewpage.action?pageId=1343727)
[http://saeapns.sinaapp.com/doc.html](http://saeapns.sinaapp.com/doc.html)
