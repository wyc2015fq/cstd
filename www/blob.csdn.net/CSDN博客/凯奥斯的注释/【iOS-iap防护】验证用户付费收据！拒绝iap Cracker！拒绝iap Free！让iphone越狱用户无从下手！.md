# 【iOS-iap防护】验证用户付费收据！拒绝iap Cracker！拒绝iap Free！让iphone越狱用户无从下手！ - 凯奥斯的注释 - CSDN博客





2013年11月28日 17:55:30[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：1886








# 【iOS-iap防护】验证用户付费收据！拒绝iap Cracker！拒绝iap Free！让iphone越狱用户无从下手！【2012年5月2日更新防护iap Free的方法】


本站文章均为[李华明Himi](http://www.himigame.com/about-himi)原创,转载务必在明显处注明：（作者新浪微博：[@李华明Himi](http://weibo.com/317426208)) 
转载自[【黑米GameDev街区】](http://www.himigame.com/) 原文链接: [http://www.himigame.com/iphone-cocos2d/673.html](http://www.himigame.com/iphone-cocos2d/673.html)

[☞ 点击订阅 ☜](http://feed.feedsky.com/himigame) 本博客最新动态!及时将最新博文通知您！






分享











对于iOS的应用安全这块主要有两块是我们开发者可以避免的，一个是存储数据加密，这个在上一篇文章Himi介绍了base64加密算法；另外一个就是付费产品防护！那么本篇Himi来分享如何防护越狱用户的iap Cracker！

对于iap Cracker这个插件，Himi简单介绍下！

iap Cracker可以说是iOS越狱用户的终极利器阿，当今app Store的所有内置收费的游戏，基本使用此插件进行秒购买无压力！（对于那些收费下载的游戏，对于越狱用户来说，安装个XX助手<你懂得～>就可以免费体验app store的所有游戏，不管你下载收费还是内置收费！）

iap Cracker能绕过appstore的付费流程，其方式是当用户点击付费产品进行购买后，iap Cracker模拟返回一个购买成功的消息（无需联网，说白了，连post 数据给App store都没有！），然后我们应用中收到这个“假的”交易成功的消息直接给用户加钱，加装备，加各种….

OK，对于iap Cracker就不再多介绍了，下面Himi来分享如何防护iap Cracker吧；

对于越狱用户使用付费破解插件进行付费这个问题，其实Apple并没有不管，而是已经在文档中清晰的说明，只是很多童鞋并没有发现，如下截图：

![](http://www.himigame.com/wp-content/uploads/2012/03/222.png)

apple提示开发者付费要进行验证付费收据！ 原文apple dev官方文档连接：

[https://developer.apple.com/library/ios/#documentation/NetworkingInternet/Conceptual/StoreKitGuide…](https://developer.apple.com/library/ios/#documentation/NetworkingInternet/Conceptual/StoreKitGuide/VerifyingStoreReceipts/VerifyingStoreReceipts.html#//apple_ref/doc/uid/TP40008267-CH104-SW1)

  下面Himi就详细讲解如何在我们付费流程中加入iap防护，步骤如下：

1. 首先将 json类库和NSData+Base64类导入你的项目中，下载：

    “json_base.zip” 下载地址：  [http://vdisk.weibo.com/s/hq1Qk](http://vdisk.weibo.com/s/hq1Qk)

2. 然后将Himi封装的如下函数拷贝到你付费代码所在的类中：



```
```cpp
.h中：
```



```cpp
-(
```

```cpp
BOOL
```

```cpp
)putStringToItunes:(NSData*)iapData;
```





```cpp
.m中：
```





```cpp
#import "NSData+Base64.h"
```



```cpp
#import "NSString+SBJSON.h"
```



```cpp
#import "JSON.h"
```





```cpp
-(
```

```cpp
BOOL
```

```cpp
)putStringToItunes:(NSData*)iapData{
```

```cpp
//用户购成功的transactionReceipt
```





```cpp

```

```cpp
NSString*encodingStr = [iapData base64EncodedString];
```





```cpp

```

```cpp
NSString *URL=@
```

```cpp
"[https://sandbox.itunes.apple.com/verifyReceipt](https://sandbox.itunes.apple.com/verifyReceipt)"
```

```cpp
;
```



```cpp

```

```cpp
//[https://buy.itunes.apple.com/verifyReceipt](https://buy.itunes.apple.com/verifyReceipt)
```



```cpp

```

```cpp
NSMutableURLRequest *request = [[NSMutableURLRequest alloc] init];
```

```cpp
// autorelease];
```



```cpp

```

```cpp
[request setURL:[NSURL URLWithString:URL]];
```



```cpp

```

```cpp
[request setHTTPMethod:@
```

```cpp
"POST"
```

```cpp
];
```



```cpp

```

```cpp
//设置contentType
```



```cpp

```

```cpp
[request addValue:@
```

```cpp
"application/json"
```

```cpp
forHTTPHeaderField:@
```

```cpp
"Content-Type"
```

```cpp
];
```



```cpp

```

```cpp
//设置Content-Length
```



```cpp

```

```cpp
[request setValue:[NSString stringWithFormat:@
```

```cpp
"%d"
```

```cpp
, [encodingStr length]] forHTTPHeaderField:@
```

```cpp
"Content-Length"
```

```cpp
];
```





```cpp

```

```cpp
NSDictionary* body = [NSDictionary dictionaryWithObjectsAndKeys:encodingStr, @
```

```cpp
"receipt-data"
```

```cpp
, nil];
```



```cpp

```

```cpp
SBJsonWriter *writer = [SBJsonWriter
```

```cpp
new
```

```cpp
];
```



```cpp

```

```cpp
[request setHTTPBody:[[writer stringWithObject:body] dataUsingEncoding:NSASCIIStringEncoding allowLossyConversion:YES]];
```



```cpp

```

```cpp
NSHTTPURLResponse *urlResponse=nil;
```



```cpp

```

```cpp
NSError *errorr=nil;
```



```cpp

```

```cpp
NSData *receivedData = [NSURLConnection sendSynchronousRequest:request
```



```cpp

```

```cpp
returningResponse:&urlResponse
```



```cpp

```

```cpp
error:&errorr];
```





```cpp

```

```cpp
//解析
```



```cpp

```

```cpp
NSString *results=[[NSString alloc]initWithBytes:[receivedData bytes] length:[receivedData length] encoding:NSUTF8StringEncoding];
```



```cpp

```

```cpp
CCLOG(@
```

```cpp
"-Himi-  %@"
```

```cpp
,results);
```



```cpp

```

```cpp
NSDictionary*dic = [results JSONValue];
```



```cpp

```

```cpp
if
```

```cpp
([[dic objectForKey:@
```

```cpp
"status"
```

```cpp
] intValue]==0){
```

```cpp
//注意，status=@"0" 是验证收据成功
```



```cpp

```

```cpp
return
```

```cpp
true
```

```cpp
;
```



```cpp

```

```cpp
}
```



```cpp

```

```cpp
return
```

```cpp
false
```

```cpp
;
```



```cpp
}
```
```



接着说下此方法的使用，一般付费代码中，童鞋们肯定会有如下函数：



```
```cpp
- (
```

```cpp
void
```

```cpp
)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
```

```cpp
//交易结果
```



```cpp
{
```



```cpp

```

```cpp
for
```

```cpp
(SKPaymentTransaction *transaction in transactions)
```



```cpp

```

```cpp
{
```



```cpp

```

```cpp
switch
```

```cpp
(transaction.transactionState)
```



```cpp

```

```cpp
{
```



```cpp

```

```cpp
case
```

```cpp
SKPaymentTransactionStatePurchased:
```

```cpp
//交易完成
```



```cpp

```

```cpp
if
```

```cpp
([self putStringToItunes:transaction.transactionReceipt]){
```



```cpp

```

```cpp
//这里给用户添加钱阿，装备阿什么的
```



```cpp

```

```cpp
}
```



```cpp

```

```cpp
break
```

```cpp
;
```



```cpp

```

```cpp
......代码省略
```



```cpp

```

```cpp
}
```



```cpp

```

```cpp
}
```



```cpp
}
```
```



上面这个函数当获取交易成功的消息都会进入到SKPaymentTransactionStatePurchased这个case中（不管是iap cracker模拟的还是appstore真的反馈的消息）， 那么我们一般不做iap防护情况下，会直接在此case中给用户添加金币阿，什么的！ 但是如上所说因为iap cracker也会模拟返回交易成功的消息，也会进入到这个case中，因此我们在此与appstore再次进行一次收据验证！

另外说一点当交易完成时appstore传回来的transaction（SKPaymentTransaction）类中的transactionReceipt属性里包含AppStore返回经过签名的收据信息！OK，我们要的就是这个收据并将此收据post给appstore 的server进行收据验证，所以在SKPaymentTransactionStatePurchased这个交易成功的case中再调用Himi封装的函数if([self putStringToItunes:transaction.transactionReceipt]){} 进行再次确认下购买是否付费流程正确！

那么下面详细说下Himi封装的这个putStringToItunes函数：

此函数中，首先我们将传入的收据data类型变量进行base64转换成string类型，然后将此收据以json的形式发送给appstore进行验证！这里注意！一定要以json形式发送，否则appstore server端不识别!

最后再次利用json对appstore server返回的字段（json数据）进行解析，我们只需要解析出 status 这个key的value即可！

当appstore验证收据正确时我们解析出来的 status 这个key的value值为0(零)！

下面是appstore返回json数据的两种形式：

1. 收据无效的情况：



```
```cpp
{
```

```cpp
"status"
```

```cpp
:21002,
```

```cpp
"exception"
```

```cpp
:
```

```cpp
"java.lang.NullPointerException"
```

```cpp
}
```
```



2.收据正确的情况，如下图（点击放大）：

![](http://www.himigame.com/wp-content/uploads/2012/05/11112.png)[](http://www.himigame.com/wp-content/uploads/2012/03/11112.png)

最后大家需要注意的一点是，Himi封装的函数中post的地址这里要记得发布的时候修改！

      因为当你沙盒测试的时候地址是：https://sandbox.itunes.apple.com/verifyReceipt

      但是正式发布后post的地址应该是：  https://buy.itunes.apple.com/verifyReceipt

千万不要发布应用的时候别忘记修改这里！

OK，本篇就介绍到这里，希望对还没有做iap防护的童鞋有所帮助！



—————2012年5月2日更新：

对于iap 的防护，现在除了iap cracker之外，那么最主要的就是国人的iap free这个插件了，那么对于iap free的防护，如果我们仅仅只是跟apple的服务器进行验证收据，那么iap free照样能破解我们的游戏/软件！

我这里先大概说下iap free：

iap free的功能与iap cracker功能类似，只是更加强大的iap free能在你与apple服务器进行验证收据的步骤中进行截取，并返回一个模仿apple返回的假收据！这么一说大家就很清楚了，我们上面说的iap 验证收据变得毫无意义，但是不要着急，这里Himi将iap free的假收据形式打印了出来，大家对比看下就知道该如何来做iap free的防护了：

![](http://www.himigame.com/wp-content/uploads/2012/05/111.jpg)

上图就是iap free制作的假收据啦，那么至于如何继续做防护，我想这里不需要再多解释了，毕竟有政策就有对策；大家发挥吧～

另外说一点，当用户在无网的情况下购买任何产品，肯定只有两种情况，1.越狱机器想破解。2.忘记；

那么我们可以使用下面这段代码判断当前ios设备是否联网了：



```
```cpp
-(
```

```cpp
BOOL
```

```cpp
)isNetworkOK{
```



```cpp

```

```cpp
struct
```

```cpp
sockaddr_in zeroAddress;
```



```cpp

```

```cpp
bzero(&zeroAddress,
```

```cpp
sizeof
```

```cpp
(zeroAddress));
```



```cpp

```

```cpp
zeroAddress.sin_len =
```

```cpp
sizeof
```

```cpp
(zeroAddress);
```



```cpp

```

```cpp
zeroAddress.sin_family = AF_INET;
```



```cpp

```

```cpp
SCNetworkReachabilityRef defaultRouteReachability = SCNetworkReachabilityCreateWithAddress(NULL, (
```

```cpp
struct
```

```cpp
sockaddr *)&zeroAddress);
```



```cpp

```

```cpp
SCNetworkReachabilityFlags flags;
```





```cpp

```

```cpp
BOOL
```

```cpp
didRetrieveFlags = SCNetworkReachabilityGetFlags(defaultRouteReachability, &flags);
```



```cpp

```

```cpp
CFRelease(defaultRouteReachability);
```





```cpp

```

```cpp
if
```

```cpp
(!didRetrieveFlags)
```



```cpp

```

```cpp
{
```



```cpp

```

```cpp
NSLog(@
```

```cpp
"Error. Could not recover network reachability flags"
```

```cpp
);
```



```cpp

```

```cpp
return
```

```cpp
NO;
```



```cpp

```

```cpp
}
```





```cpp

```

```cpp
BOOL
```

```cpp
isReachable = flags & kSCNetworkFlagsReachable;
```



```cpp

```

```cpp
BOOL
```

```cpp
needsConnection = flags & kSCNetworkFlagsConnectionRequired;
```



```cpp

```

```cpp
return
```

```cpp
(isReachable && !needsConnection) ? YES : NO;
```



```cpp
}
```
```



项目中添加    SystemConfiguration.framework

然后在导入 ：

  #import <SystemConfiguration/SCNetworkReachability.h>

      #import <netinet/in.h>




