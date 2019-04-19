# 集成支付宝钱包支付iOS SDK的方法与经验 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年03月14日 14:37:00[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1104
没想到，支付宝的SDK是我目前用过的所有第三方SDK中最难用的一个了。
## 下载
首先，你要想找到这个SDK，都得费点功夫。现在的SDK改名叫移动支付集成开发包了，下载页面在 [这里](https://b.alipay.com/order/productDetail.htm?productId=2013080604609654&tabId=4#ps-tabinfo-hash) 的 “[请点此下载集成开发包](http://download.alipay.com/public/api/base/WS_MOBILE_PAY_SDK_BASE.zip)”
> 
Baidu和Googlep排在前面的支付宝开放平台，里面的SDK已经是2年前的版本了，而且还不支持64位架构。
## 文档
压缩包里有两个相关文档 ：
《支付宝钱包支付接口开发包2.0标准版.pdf》
《支付宝钱包支付接口开发包2.0标准版接入与使用规则.pdf》
iOS相关内容可以主要看第一个文档，第二个文档名字和里面写的不一样，内容其实是个附录；文档里面多个平台都涉及到了，内容有些杂乱。下面先解释下整体SDK的流程和要做的事，就好对症下药找文档内相应的内容了。
## 流程
摘自第一个文档《支付宝钱包支付接口开发包2.0标准版.pdf》
![业务流程图](http://upload-images.jianshu.io/upload_images/185792-ec4290951fa39551.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
业务流程图
图中的“商户客户端”就是我们的iOS客户端需要做的事情：
- 调用支付宝支付接口
- 处理支付宝返回的支付结果
在调用支付宝支付接口前，我们还需要先生成一个订单，文档中描述时，是将这步也放在客户端来做了，但也可以在服务器端生成这个订单（图中支付宝会在支付成功后通知服务器端，所以在服务器端生成订单的话，你可以掌握所有订单，而且也会更安全）：
- 生成订单（可以在iOS客户端内生成，也可以在服务器端生成）
- 调用支付宝支付接口，发送订单
- 处理支付宝返回的支付结果
其实对于业务来说，这些步骤已经够了，但是有一个安全性问题，你肯定不希望你接收到的支付结果被截获修改，所以，这就需要在生成订单和处理支付结果的时候做一个安全性校验：
生成订单时对数据签名，收到支付结果时对数据进行签名验证，以检验数据是否被篡改过。
支付宝目前只支持采用RSA加密方式做签名验证。
> 
[RSA加密算法](http://zh.wikipedia.org/wiki/RSA%E5%8A%A0%E5%AF%86%E6%BC%94%E7%AE%97%E6%B3%95) 除了可加解密外，还可用来作签名校验。
简单的说，RSA会生成一个私钥和一个公钥，私钥你应该独自保管，公钥你可以分发出去。
做签名验证时，你可以用私钥对需要传输的数据做签名加密，生成一个签名值，之后分发数据，接收方通过公钥对签名值做校验，如果一致则认为数据无篡改。
具体到支付宝使用RSA做签名验证，就是在生产订单时，需要使用私钥生成签名值；在处理返回的支付结果时，需要使用公钥验证返回结果是否被篡改了。
具体需要对哪些值，怎样生成签名，对哪些值最签名验证，可以在第一个文档中找找，后面我会简单提一下，但还是以文档或实践为准吧。
## 集成
清楚了流程后，就好理解怎么集成了。
### 支付SDK
如果只需要发送订单和处理支付返回结果，只需要添加`AlipaySDK.bundle`和`AlipaySDK.framework`就行了。
这里再吐槽下，之前用的旧版本，和现在的版本相比，还不光是把类名字给改了，原先是用的类方法，现在新版又给改成了单例了。。还真是任性啊，这要是哪家小厂的SDK，估计早被弃用了把。。
发送订单的方法：
```
- (void)payOrder:(NSString *)orderStr
      fromScheme:(NSString *)schemeStr
        callback:(CompletionBlock)completionBlock;
```
- 如果手机内没安装支付宝的app，会直接展现支付宝web支付界面，通过callback返回支付结果；
- 如果手机内安装了支付宝的app，会跳转到支付宝的app支付，然后通过openURL的回调返回支付结果。
> 
支付宝的SDK只给了一个处理返回结果的方法，而不像其他第三方的SDK提供一个处理openURL的方法，所以你需要通过DEMO或者在第二个文档里找到处理openURL的方式：
```
if ([url.host isEqualToString:@"safepay"]) {
[[AlipaySDK defaultService] processOrderWithPaymentResult:url
standbyCallback:^(NSDictionary *resultDic) {
          NSLog(@"result = %@",resultDic);
}]; }
```
SDK也提供了一个处理openURL返回结果的方法
```
```
- (void)processOrderWithPaymentResult:(NSURL *)resultUrl
                      standbyCallback:(CompletionBlock)completionBlock;
```
```
两个回调block都统一定义为`typedef void(^CompletionBlock)(NSDictionary *resultDic);`，
返回了一个字典，但是SDK里完全没有提示有哪些key。。
你可以在文档里找到，或者自己实际试一下，返回的信息如下：
- resultStatus，状态码，SDK里没对应信息，第一个文档里有提到：- 9000 订单支付成功
- 8000 正在处理中
- 4000 订单支付失败
- 6001 用户中途取消
- 6002 网络连接出错
- memo， 提示信息，比如状态码为6001时，memo就是“用户中途取消”。**但千万别完全依赖这个信息**，如果未安装支付宝app，采用网页支付时，取消时状态码是6001，但这个memo是空的。。（当我发现这个问题的时候，我就决定，对于这么不靠谱的SDK，还是尽量靠自己吧。。）
- result，订单信息，以及签名验证信息。如果你不想做签名验证，那这个字段可以忽略了。。
如果你对支付的安全性不那么在意或重视的话，到这里就可以完成支付宝的集成了。
如果想更加安全，还是需要增加下面的签名验证的。
### 签名验证
首先，RSA只是一种算法，所以你可以使用任何一种开源的、或者自己去实现这个算法来实现签名和验证的目的。
在整个流程当中，因为涉及到了RSA公钥、私钥的生产，RSA的签名、验证签名，SHA1值的计算，base64和URL编码，所以支付宝用了一个开源的代码来统一解决这些问题，就是[openssl](http://www.openssl.org/)（顺便再吐槽下，这DEMO里一放`openssl`，不知道又会引来多少公司的产品里使用`openssl`了，估计阿里自己也没少用，什么时候都能跟老罗、华为一样去[赞助](http://www.openssl.org/support/acknowledgments.html)点呢。。）
如果你想省事，也用`openssl`，那你需要把这些东西都加入到项目中：DEMO中的`openssl目录`头文件，两个库文件`libcrypto.a libssl.a`，DEMO里支付宝自己写的`Util目录`
#### 订单签名
上面说了，订单签名应该用私钥，但是把私钥放到app里其实本身就不安全，因为你的app是分发到用户手里的，私钥应该放在自己的手里，分发出去的应该是公钥。
所以私钥最好是放在自己的服务器上，订单加密这个工作放在服务器端来做，服务器将包含签名的订单信息返回给app，app再通过SDK发送给支付宝，这样会更安全些；而且服务器也能掌握所有的订单状况。
如果你非要将私钥集成到app里，那可以参考SDK的DEMO，因为这个DEMO就是在app本地通过私钥做的订单签名。。
#### 支付结果签名验证
上面的回调block提到了返回的内容，返回的支付结果中的`result`字段里是带有订单信息和签名信息的，所以签名验证就是需要这个字段的值。
文档中有一个这个字段的例子，实际结果没有换行，我换一下行便于阅读：
```bash
partner="2088101568358171"&seller_id="xxx@alipay.com"&out_trade_no="0819145412-6177"&subject="测试"&body="测试测试"&total_fee="0.01"¬ify_url="http://notify.msp.hk/notify.htm"&service="mobile.securitypay.pay"&payment_type="1"&_input_charset="utf-8"&it_b_pay="30m"&success="true"
&sign_type="RSA"
&sign="hkFZr+zE9499nuqDNLZEF7W75RFFPsly876QuRSeN8WMaUgcdR00IKy5ZyBJ4eldhoJ/2zghqrD4E2G2mNjs3aE+HCLiBXrPDNdLKCZ gSOIqmv46TfPTEqopYfhs+o5fZzXxt34fwdrzN4mX6S13cr3UwmEV4L3Ffir/02RBVtU="
```
总共分为三个部分
- 第一部分是订单信息，每个字段的具体含义可以在文档里找；
- 中间`sign_type`是签名用的算法，文档里说了，目前只支持RSA；
- 最后的`sign`就是签名值。
验证的步骤如下：
- 首先把订单信息和签名值分别提取出来（SDK居然都不给处理好。。）- 订单信息就是`sign_type`的连字符`&`之前的所有字符串
- 签名值是`sign`后面双引号内的内容，注意签名的结尾也是`=`，所以不要用split字符串的方式提取
- 如果你想简单，可以直接使用`Util目录`下的`DataVerifier`来作签名验证- `- (BOOL)verifyString:(NSString *)string withSign:(NSString *)signString;`
- 第一个参数就是订单信息，第二个参数就是签名值。
> 
其实不使用`openssl`，用其他第三方RSA的开源代码也是可以的。可以看下DEMO里`openssl_wrapper`的源码和SDK的文档。
- 对于订单信息，先做一个base64编码（DEMO中这个还要调`openssl`来实现。。），再计算SHA1的值（这个也可以完全不用`openssl`，苹果的库中都有的。。），然后再签名比对。
- 对于公钥，如果使用其他第三方代码，需要注意格式问题。支付宝的DEMO实现中，是把这个公钥又转回成`openssl`生成的本地文件格式，然后再写入本地文件，再让`openssl`读取出来使用。。
以上，就是支付宝 iOS SDK的一些介绍。
总体来说，我觉得能靠自己处理的地方还是尽量不要依赖这个不太靠谱的SDK了。
