# TT--TTOtherPayManager(iOS微信和支付宝支付) - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年05月16日 14:50:28[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：628标签：[iOS微信和支付宝																[iOS 资源大全																[iOS学习资料																[ios开发](https://so.csdn.net/so/search/s.do?q=ios开发&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS学习资料&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS 资源大全&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS微信和支付宝&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
![](http://upload-images.jianshu.io/upload_images/2363764-20bb901e95d2a6f8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
## 本文来自简书，原文地址:[http://www.jianshu.com/p/8cbe1a5b6704](http://www.jianshu.com/p/8cbe1a5b6704)
## 前言
针对iOS的支付,在之前,TTFrameWork仅仅写了关于苹果的内购[AppPurchaseManager](https://github.com/zhangfurun/TT/tree/master/TTFrameWork/Common/Tools),这次的更新,我融入了关于第三方(支付宝和微信)支付,采用最简单的调用方式,一站式解决方案.
## 提醒
根据微信支付和阿里(支付宝)支付的官方SDK的说明,进行支付的签名等一些信息,尽量放到 服务端 去做,特别是阿里支付需要涉及到 私钥(yuè) ,这里,为了方便开发者更容易理解,将签名的过程和内容放到客户端进行处理.
## 用途
针对苹果的审核理念,虚拟货币必须使用苹果内购,比如会员的充值等等,但是,限制于苹果服务器的不稳地以及高延迟,很多国内的开发者就按耐不住躁动的心,开始网项目中融入国内的微信和阿里支付,当然,这个也是需要技巧的,不如审核隐藏什么的,这个具体的方案就不多说了,毕竟存在风险,而且已经举报,查实就会被下架,特别是知名的APP,比如QQ购买会员依然是内购,所以,这个就看你们是怎么选择了.
## 正传
下面针对更新进行说明:
#### 1. 导入库,关于相关的库的导入,请参考微信和支付宝sdk,那里比较详细
[微信iOS接入说明](https://pay.weixin.qq.com/wiki/doc/api/app/app.php?chapter=8_5)
[微信SDK](https://pay.weixin.qq.com/wiki/doc/api/app/app.php?chapter=11_1)
[阿里支付iOS接入说明](https://doc.open.alipay.com/docs/doc.htm?spm=a219a.7629140.0.0.Wb47Y3&treeId=204&articleId=105295&docType=1)
[阿里支付SDK](https://doc.open.alipay.com/doc2/detail.htm?treeId=54&articleId=104509&docType=1)
ps:导入库会出现很多问题,后续,我会在我的简书,推出相关的解决方案
#### 2. 设置相关的注册的信息:
在TTConst中进行设置
```cpp
```cpp
// 微信
#define WECHATAPPIDKEY              @"" // appiid
#define WECHATAPPSECRETKEY          @"" // SECRETKEY
#define WECHATPARTNERID             @"" // 商户号
#define WECHATPARTNERKEY            @"" // 密钥
// 阿里
#define ALIPAYAPPID                 @"" // appid
#define ALIPAYPRIVATEKEY            @"" // 私钥(这个对于阿里支付的私钥,建议放到服务器端去做)
```
```
#### 3. UIAppleDelegate中的设置
- 
##### 注册微信和阿里支付的相关SDK
```
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
 return [TTPAY_MANAGER registerApp];
}
```
- 
##### 微信/支付宝APP对支付结果进行回调
这里需要执行协议,也就是微信和支付宝在客户端支付完后,会进行APP进行跳转的回调,相关的信息会通过协议进行表达.
> 
这里,我们对两种支付方式的回调进行了分装,通过Url.host进行区分,这里具体如实现可以查看实现方式.
iOS 9.0 之前
```
- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation {
   return [TTPAY_MANAGER handleUrl:url];
}
```
iOS 9.0 之后
```
- (BOOL)application:(UIApplication *)app openURL:(NSURL *)url options:(NSDictionary<NSString*, id> *)options {
   return [TTPAY_MANAGER handleUrl:url];
}
```
##### 4. 使用
- 
##### 相关Model进行赋值
创建微信和支付宝对应的数据Model
![](http://upload-images.jianshu.io/upload_images/2363764-427207acc5f0df14.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
阿里支付Model
![](http://upload-images.jianshu.io/upload_images/2363764-579f16ed0964492b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
微信支付Model
记住, 数据均不为空 ,因为这里的数据是获取的支付的关键参数,如果需要设置其他的参数,可以参考上面的SDK内容.如果为空,可能导致不跳转或者支付失败.
如果考虑服务器返回数据为空的情况,可以在model的属性的get方法中,进行重写,防止为空的情况,因为为空的话,会导致不跳转等等意外情况
- 
##### 举例使用
下面拿阿里(支付宝)支付进行说明:
- 
创建Model,进行赋值:
```
AliPayModel *model = [AliPayModel new];
model.notifyUrl = item.aliPayNotifyUrl; // 这里的回调地址是服务器给的
model.price = self.orderItem.originalPrice; 
model.orderNo = item.orderCode; // 这里是服务器定义的商品的标识码
model.payTitle = @"title";
model.body = @"description";
```
- 
调用payWithOrderMessage,传入model就行,接下来会有相关的状态进行回调
```
[TTPAY_MANAGER payWithOrderMessage:model successBlock:^{
NSLog(@"----成功了");
} cancelBlock:^{
NSLog(@"----取消了");
} failureBlock:^(NSString *errStr) {
NSLog(@"----失败了");
}];
```
最后,可以通过回调进行相应的处理,从而完成支付.
## 补充:
整套的支付开发流程,大家可以去查看一下对应的SDK,需要补充的是,毕竟这个不是自家的东西,很多东西局限性在于延迟或者数据对接问题,因此,你需要跟服务器对接好,比如服务器验证回调信息等等情况,以及相关的已经扣款,但是服务器无响应的情况,这个需要根据具体的产品需求进行相关的设置.
## 总结:
在开始的时候,看SDK的说明文档很懵逼,因为,参数太多,看不过来,而且也不知道哪些是需要填写的,哪些是选择填写的,而且,涉及到公司产品的需求,许多东西需要动态化处理.当时查看了很多的Demo,发现都是采用的官网给给出的Demo实例,针对签名部分,并没有给出很清楚的实例,如微信的二次签名,以及支付宝的私钥签名,这个其实是接入支付SDK的难点,因为,决定雷锋一把,写了这个傻瓜式的支付Tool,这个你可以直接拿过去用,拖入自己的工程,会报一些错误,可以从网上查找一下答案,这个网上都会有相关的内容,我就不一一列举了.
## 项目
这个第三方支付的Manager是我的TTFrameWork的一个分支,作为一个模块,已经上传到Github了,欢迎大家多多下载.
总项目地址:[TTFrameWork](https://github.com/zhangfurun/TT)
支付项目地址:[TTOtherPayManager](https://github.com/zhangfurun/TT/tree/master/TTFrameWork/TTOtherPayManager)
TTFrameWork的简书地址:[TTFrameWork简书](http://www.jianshu.com/p/0bd0ec075409)
