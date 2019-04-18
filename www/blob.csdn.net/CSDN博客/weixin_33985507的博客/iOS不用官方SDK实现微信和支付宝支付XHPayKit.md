# iOS不用官方SDK实现微信和支付宝支付XHPayKit - weixin_33985507的博客 - CSDN博客
2018年02月24日 11:51:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：11
![2229730-5b68c6e7af0f19be.png](https://upload-images.jianshu.io/upload_images/2229730-5b68c6e7af0f19be.png)
微信、支付宝支付
## 前言
前段时间由于项目需求,移除了项目中的微信支付SDK和支付宝支付SDK,这种情况下需要自己搞定支付,第一时间考虑到的是使用openshare,但是当下载openshare下来后发现,openshare的支付接口,并不能直接替换官方SDK支付接口,
官方SDK的实现逻辑是后台订单签名完成后,客服端传签名信息及参数给支付SDK,支付SDK生成协议url,然后拉起第三方支付APP,
而openshare的支付接口是直接传拉起支付的协议url地址,将生成协议url地址的操作也交给后台来生成,
这样的话后台需要修改代码,
能否不修改后台代码,弄一个支付接口直接替换官方SDK支付接口,实现无缝对接呢,于是就研究了下微信和支付宝支付时APP间通讯关系,最后封装成[XHPayKit](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2FCoderZhuXH%2FXHPayKit).
## 特性:
1.[XHPayKit](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2FCoderZhuXH%2FXHPayKit)拥有和官方SDK类似接口,可以直接替换官方SDK支付接口,如果你使用过官方SDK,那么转换为本库你只需花费极短时间.
2.[XHPayKit](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2FCoderZhuXH%2FXHPayKit)只有10kb大小,不用导入任何依赖库,便可实现微信支付、支付宝支付,如果你想为项目瘦身或由于某种原因,不想使用官方SDK实现支付功能,此库将是一个不错的选择.
3.[XHPayKit](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2FCoderZhuXH%2FXHPayKit)使用时,不需要配置微信等平台appid等信息,服务端配置就可以了,因为后台签名订单时会返回appid等信息给客户端.
## 注意:
1.先在微信、支付宝开放平台注册你的应用,并获得支付能力
2.导入此库,并请将 weixin 、 alipay 字段添加到info.plist白名单
3.添加自己APP URL Schemes,和微信回调URL Schemes,详见README文档
## 使用方法:
### 1.微信支付
```
//微信支付参数,下面7个参数,由后台签名订单后生成,并返回给客服端(与官方SDK一致)
 //注意:请将下面参数设置为你自己真实订单签名后服务器返回参数,便可进行实际支付
XHPayWxReq *req = [[XHPayWxReq alloc] init];
req.openID = @"";
req.partnerId = @"";
req.prepayId = @"";
req.nonceStr = @"";
req.timeStamp = 1518156229;
req.package = @"";
req.sign = @"";
        
//传入订单模型,拉起微信支付
[[XHPayKit defaultManager] wxpayOrder:req completed:^(NSDictionary *resultDict) {
          NSLog(@"支付结果:\n%@",resultDict);
          NSInteger code = [resultDict[@"errCode"] integerValue];
          if(code == 0){//支付成功
                
          }
}];
```
### 2.支付宝支付
```
//支付宝订单签名,此签名由后台签名订单后生成,并返回给客户端(与官方SDK一致)
//注意:请将下面值设置为你自己真实订单签名,便可进行实际支付
NSString *orderSign = @"很长的一串支付宝订单签名";
        
//传入支付宝订单签名 和 自己App URL Scheme,拉起支付宝支付
[[XHPayKit defaultManager] alipayOrder:orderSign fromScheme:@"XHPayKitExample" completed:^(NSDictionary *resultDict) {
    NSLog(@"支付结果:\n%@",resultDict);
    NSInteger status = [resultDict[@"ResultStatus"] integerValue];
    if(status == 9000){//支付成功
                
    }
}];
```
### 3.在Appdelegate中添加以下代码 - 处理第三方支付跳回商户app携带的支付结果Url
```
#if __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_9_0
/** iOS9及以后 */
- (BOOL)application:(UIApplication *)app openURL:(NSURL *)url options:(NSDictionary<UIApplicationOpenURLOptionsKey, id> *)options
{
    BOOL result = [[XHPayKit defaultManager] handleOpenURL:url];
    if (!result) {//这里处理其他SDK(例如QQ登录,微博登录等)
        
    }
    return result;
}
#endif
/** iOS9以下 */
- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation
{
    BOOL result = [[XHPayKit defaultManager] handleOpenURL:url];
    if (!result) {//这里处理其他SDK(例如QQ登录,微博登录等)
        
    }
    return result;
}
```
## 其它接口:
```
/**
 是否安装微信
 @return 已安装YES,未安装NO
 */
+(BOOL)isWxAppInstalled;
/**
 是否安装支付宝
 @return 已安装YES,未安装NO
 */
+(BOOL)isAliAppInstalled;
```
## 支付结果resultDict释义:
### 微信
```
{
    "errCode":0,
    "errStr":"成功"
}
//以下状态码含义与官方SDK一致
errCode = 0,成功
errCode = -1,普通错误类型
errCode = -2,用户点击取消并返回
errCode = -3,发送失败
errCode = -4,授权失败 
errCode = -5,微信不支持
```
### 支付宝
```
{
    "result":"",
    "resultStatus":"9000",
    "memo":"支付成功"
}
//以下状态码含义与官方SDK一致
resultStatus = 9000,支付成功
resultStatus = 8000,正在处理中，支付结果未知（有可能已经支付成功）,请查询商户订单列表中订单的支付状态
resultStatus = 4000,支付失败
resultStatus = 5000,重复请求
resultStatus = 6001,用户中途取消
resultStatus = 6002,网络连接出错
resultStatus = 6004,支付结果未知（有可能已经支付成功），请查询商户订单列表中订单的支付状态
```
## 小结:
XHPayKit的实现非常简单,感兴趣的同学可以下载下来研究下支付时APP间通讯.
代码地址:[https://github.com/CoderZhuXH/XHPayKit](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2FCoderZhuXH%2FXHPayKit)
