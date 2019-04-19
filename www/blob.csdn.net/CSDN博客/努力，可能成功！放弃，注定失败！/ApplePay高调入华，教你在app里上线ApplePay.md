# ApplePay高调入华，教你在app里上线ApplePay - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年03月22日 14:23:55[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：785

![](http://upload-images.jianshu.io/upload_images/1391549-d765260ea7e0e42c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
ApplePay在中国上线后，就有许多线上app前后脚加入了对其的接入支持，个人比较喜欢的ENJOY也抢在首批接入了ApplePay应用内支付。本文将分享作者的接入经验。
ApplePay是苹果公司推出的一种线上/线下的便捷支付方式，根据TouchId来验证支付卡持卡人身份，ApplePay并不参与资金流动，最终还是让银行完成扣款处理，目前据Apple介绍只配备在iphone6以上和新款的ipad Air2和ipad mini3上，中国地区最低系统要求为IOS9.2，对于一种新支付体验，这门槛的确有点高了。苹果开发文档对ApplePay工作方式的介绍.
![](https://raw.githubusercontent.com/liuzhiyi1992/MyStore/master/ApplePay/itunesApplePay%E5%88%97%E8%A1%A8.png)
线下支付使用NFC POST机只需要与银联联系签约租用即可，我们来介绍线上支付的流程和app应用内接入的方法。 
目前的接入方式有两种，一是使用第三方提供商的SDK接入，另一种是让PassKit Framework直接与银联的接口对接，当然网络上还有一些自己使用PassKit PaymentRequest自己生成订单组织信息，直接与Apple对接的Demo，因为我们不可能每家银行跑去签约，大陆的银行也不会给我们开放特许，因此这种方式仅仅能用于测试ApplePay的功能和API尝鲜，并不适用于生产中。
[ApplePay官网](https://developer.apple.com/apple-pay/)上有列出中国目前支持并提供SDK的第三方提供商
使用第三方SDK接入的优点是开发成本比较低，并且各自都应该有高度定制的Payment Sheet(因订单信息完整度的不同)，可供定制更详细的商品信息展示，缺点就是要钱。
若我们选择后者，相对开发成本会高，移动端不仅需要对支持性进行验证，协商银联接口数据对Payment Sheet的展示关闭进行控制，对各种异常进行捕抓和处理，同时后台需要自己实现对银联接口的认证交互，对交易数据的标准封装，订单状态的处理，支付信息的解密等。
我们选择使用银联SDK接入这种折中的方式，免费，工作量可以接受。 
使用银联SDK接入的实现方式如图所示，主要工作是商户后台利用现成的API与银联的交互：
![](https://raw.githubusercontent.com/liuzhiyi1992/MyStore/master/ApplePay/%E9%93%B6%E8%81%94ApplePay%E6%94%AF%E4%BB%98%E5%AE%9E%E7%8E%B0%E6%96%B9%E5%BC%8F_2.png)
## 申请MerchantID并更新你的证书：
向苹果申请我们独有的商户ID，这里我借用Yasin朋友在[简书](http://www.jianshu.com/p/2e5e45afc246)上分享的详细步骤，真的很详细易懂。
获得MerchantId后，更新你的证书，并且于项目->targets->Capabilities中打开ApplePay权限，选择正确的MerchantID，让三个steps成为tick状态完全权限的配置。
![](https://raw.githubusercontent.com/liuzhiyi1992/MyStore/master/ApplePay/applePay_2.png)
ok开始与银联碰头，于银联商家技术服务中心找到ApplePay入口，然后在"技术文档"选项卡中可以找到相应的SDK和后台文档。
![](https://raw.githubusercontent.com/liuzhiyi1992/MyStore/master/ApplePay/apply%E9%93%B6%E8%81%94%E6%8A%80%E6%9C%AF%E6%96%87%E6%A1%A3%E6%88%AA%E5%9B%BE_2.png)
这里我使用PHP后台，在下载的SDK压缩包里找到了PHP Version SDK，忽略掉这个外壳，我们需要将子文件夹upacp_demo_app部署到我们的服务器中。
![](https://raw.githubusercontent.com/liuzhiyi1992/MyStore/master/ApplePay/%E5%90%8E%E5%8F%B0%E5%BC%80%E5%8F%91%E5%8C%85_3.png)
这里我将upacp_demo_app部署到我mac的PHP环境中，打开upacp_demo_app/demo/api_05_app可以看见各种对订单处理的接口。
![](https://raw.githubusercontent.com/liuzhiyi1992/MyStore/master/ApplePay/%E5%90%8E%E5%8F%B0php%E6%8E%A5%E5%8F%A3_2.png)
订单的创建，取消，查询，退款等，现在我们测试下创建订单接口 
### 订单参数
下图是创建订单的请求参数，目前银联创建ApplePay订单仅支持图中所示的参数字段，基本满足使用但可定制性比较低，最基本的订单参数merId商户号，orderId订单号，txnTime订单发送时间，txnAmt订单金额，目前测试阶段我们可以通过直接调用此接口post传参，也可以直接写死在php文件中
- 注意：这里的merId是银联商户号，而非苹果分配的MerchantID。
![](https://raw.githubusercontent.com/liuzhiyi1992/MyStore/master/ApplePay/appConsume_3.png)
### 测试证书
了解到这里后还没有具备生成订单的条件，然后我们需要配置配置签名证书和验签证书的路径，这些证书在SDK下载包里面已经有附带，我们只需要在upacp_demo_app/sdk/SDKConfig.php文件中配置好他们的路径即可
![](https://raw.githubusercontent.com/liuzhiyi1992/MyStore/master/ApplePay/SDKConfig2_2.png)
这里需要配置主机的绝对路径，不能使用项目相对路径，完成SDK_SIGN_CERT_PATH，SDK_ENCRYPT_CERT_PATH，SDK_VERIFY_CERT_DIR路径的配置
### 成功获取tn
我们请求下Form_6_2_AppConsume.php接口，即可创建订单，并且返回对应的`Trade Name`，在App端我们就是拿这个Trade Name来对相应的订单进行付款。
![](https://raw.githubusercontent.com/liuzhiyi1992/MyStore/master/ApplePay/%E5%BA%94%E7%AD%94%E6%8A%A5%E6%96%87%E9%AA%8C%E7%AD%BE%E6%88%90%E5%8A%9F_2.png)
拿到订单tn后，我们已经可以使用银联SDK调起支付，在这之前还要做的就是对设备和平台的支持性检测，通过，present出Payment Sheet即可。
## 工程配置:
1.添加SDK包：
将下载SDK解压包中找到applePaySDK文件夹，加入到需要接入ApplePay的项目中。
![](https://raw.githubusercontent.com/liuzhiyi1992/MyStore/master/ApplePay/%E9%A1%B9%E7%9B%AE%E7%9B%AE%E5%BD%95%E6%96%87%E4%BB%B6_2.png)
**这里注意，项目内如果同时支持银联普通支付的话，因为两个库引用重复，最好的办法就是升级银联SDK至最新版本(3.3.3)，当然可以使用添加Linker Flags的方式解决，或者可以自己剥离掉重复部分，我是懒人，我选择升级哈哈。
2.为工程引入必须的framework： 
- CFNetwork.framework 
- PassKit.framework 
- SystemConfiguration/framework 
- libUPAPayPlugin.a 
- libz.1.2.5.tbd
3.银联SDK使用http请求，在ios9以后需在工程plist文件中添加NSAppTransportSecurity来支持Http继续使用。 
**---注意---**
- 因为银联SDK内部分代码是由C/C++组织，这里必须添加libz.1.2.5.tbd，并且将引用到UPAPayPlugin.h的源文件的后缀改为.mm
- 同时检查Target->Build Settings->Search Paths->Library Search Paths中自定义库libUPAPayPlugin.a的路径是否正确
4.最后可以开始写代码了，在需要调起ApplePay支付控件的文件中引入头文件UPAPayPlugin.h(记得文件名后缀需要改成.mm)，PassKit/PassKit.h
```
- (void)verifiesApplePayAbility {
    if (![PKPaymentAuthorizationViewController class]) {
        //检查系统版本支持性
        PopMessage(@"当前系统版本不支持ApplePay，最低支持：iphone6, ios9.0以上");
        return;
    } else if (![PKPaymentAuthorizationViewController canMakePayments]) {
        //检查设备支持性
        PopMessage(@"当前设备不支持ApplePay，最低支持：iphone6, ios9.0以上");
        return;
    } else {
        //检查卡片支持性
        NSArray *supportedNetwork = @[PKPaymentNetworkVisa, PKPaymentNetworkMasterCard, PKPaymentNetworkChinaUnionPay, PKPaymentNetworkAmex, PKPaymentNetworkDiscover];
        if (![PKPaymentAuthorizationViewController canMakePaymentsUsingNetworks:supportedNetwork]) {
            PopMessage(@"没有绑定支持的卡片");
            return;
        }
    }
    //调起苹果支付控件
    [self presentPaymentSheet];
}
- (void)presentPaymentSheet {
    WaitingMessage(@"正在呼出ApplePay支付控件");
    [UPAPayPlugin startPay:yourPayTn mode:self.applePayMode viewController:self delegate:self andAPMechantID:kAppleMechantId];
//-startPay 第一个参数是后台向银联请求创建订单获得的商品tn
//mode是字符串，00为正式环境，01为测试环境
//此处的MechantID传的是从苹果那获取的mechantId
}
```
如果需要对借记卡/信用卡作限制，在检查卡片支持性的步骤可以这样写: 
```
//检查卡片支持性
NSArray *supportedNetwork = @[PKPaymentNetworkVisa, PKPaymentNetworkMasterCard, PKPaymentNetworkChinaUnionPay, PKPaymentNetworkAmex, PKPaymentNetworkDiscover];
PKMerchantCapability capabilities = PKMerchantCapabilityEMV | PKMerchantCapability3DS | PKMerchantCapabilityDebit;
if (![PKPaymentAuthorizationViewController canMakePaymentsUsingNetworks:supportedNetwork capabilities:capabilities]) {
    PopMessage(@"没有绑定支持的卡片，本支付仅支持使用借记卡支付");
    return;
}
```
最后是银联ApplePay的支付回调:
返回的UPPayResult对象中有各种支付状态，同时我们还应该检查他的otherInfo属性，里面包含银联的优惠活动信息，如果有则应该在支付成功页中告知客户。 
```
//实现UPAPayPluginDelegate
- (void)UPAPayPluginResult:(UPPayResult *)payResult {
    //do something
    //检查是否有银联优惠信息，告知客户
}
```
otherInfo中包含优惠信息的格式为:
```
otherInfo = "currency=元&order_amt=20.00&pay_amt=15.00"
//currency       币种
//order_amt      订单金额
//pay_amt        实付金额
```
> 
！！！目前不支持商户自定义优惠活动！！！
博主自己研究了php API很久后无果，咨询银联客服，然后彻底死心。。 
## 展示
以下左图是通过银联SDK接入，因为可定制参数比较少，Payment Sheet比较简短，右图是使用苹果API直接生成的订单。
![](https://raw.githubusercontent.com/liuzhiyi1992/MyStore/master/ApplePay/ApplePay%E6%8E%A5%E5%85%A5%E6%96%B9%E5%BC%8F%E5%AF%B9%E6%AF%94.jpg)
相信美团的ApplePay也是跟银联对接的，心血来潮买了个流量顺便作下对比
![](https://raw.githubusercontent.com/liuzhiyi1992/MyStore/master/ApplePay/%E4%B8%8E%E7%BE%8E%E5%9B%A2applePay%E5%AF%B9%E6%AF%94.jpg)
## 生产环境:
**csr文件**
若需要在生产环境中使用银联SDK接入ApplePay，首先需向银联申请开通ApplePay服务(联系下签约服务)，并从银联商户服务平台生成ApplePay专用的CSR文件，重新去苹果开发者网站签署证书。
![](https://raw.githubusercontent.com/liuzhiyi1992/MyStore/master/ApplePay/%E9%93%B6%E8%81%94%E5%85%A8%E6%B8%A0%E9%81%93%E5%95%86%E6%88%B7%E6%9C%8D%E5%8A%A1_2.png)
**公钥，私钥**
公钥在SDK下载包里面有，私钥在cfca入网通知邮件里面附带也可以自行下载，一个商户号唯一一份私钥和授权码，跟银联普通支付私钥是同一份。 
**配置**
修改生产环境配置文件中的签名证书，密码，后台url地址
更换正式环境商户号(也可以是同一个)
app前端startPay方法的mode参数改为"00"
## 注意事项:
- 银联常规支付SDK升级最新版本3.3.3
- 项目内是否有引入libz.1.2.5.tbd
- 将引用到UPAPayPlugin.h的源文件的后缀改为.mm
- 检查Target->Build Settings->Search Paths->Library Search Paths中自定义库libUPAPayPlugin.a的路径是否正确
- ApplePay的按钮展示必须使用[官方指定](https://developer.apple.com/library/ios/documentation/UserExperience/Conceptual/MobileHIG/ApplePay.html)，否则过不了[审核](https://developer.apple.com/apple-pay/Apple-Pay-Identity-Guidelines.pdf)
- [](https://developer.apple.com/apple-pay/)
###### Apple Pay - Apple Developer
— 苹果官方ApplePay介绍主页 
[zhiyi](/users/e2ed12011d4f)・
[developer.apple.com →](https://developer.apple.com/apple-pay/)
- [](http://www.jianshu.com/p/2e5e45afc246)
###### Apple Pay准备工作-申请MerchantID及对应证书详细图文教程 - 简书
— 申请ApplePay商户ID教程 
[zhiyi](/users/e2ed12011d4f)・
[www.jianshu.com →](http://www.jianshu.com/p/2e5e45afc246)
- [](https://open.unionpay.com/ajweb/index)
###### 商家技术服务
— 银联商户服务中心 
[zhiyi](/users/e2ed12011d4f)・
[open.unionpay.com →](https://open.unionpay.com/ajweb/index)
- [](https://developer.apple.com/library/ios/documentation/UserExperience/Conceptual/MobileHIG/ApplePay.html)
###### iOS Human Interface Guidelines: Apple Pay
— 苹果官方ApplePay审核规范要求 
[zhiyi](/users/e2ed12011d4f)・
[developer.apple.com →](https://developer.apple.com/library/ios/documentation/UserExperience/Conceptual/MobileHIG/ApplePay.html)
- [](http://zyden.vicp.cc/applepay/)
###### 个人博客原文
[zhiyi](/users/e2ed12011d4f)・
[zyden.vicp.cc →](http://zyden.vicp.cc/applepay/)
