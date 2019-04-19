# Apple Pay接入详细教程 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年03月01日 13:37:10[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：2363

Apple Pay运行环境：iPhone6以上设备，操作系统最低iOS9.0以上，部分信息设置需要iOS9.2以上。目前还不支持企业证书添加。
环境搭建好后可以在模拟器上面运行，xcode7.2.1+iPhone6SP9.2系统下，系统会绑定几种虚拟的银行卡，和几个联系人，方便调试，支付也不会发生真实的付款，真的很方便。
[TCO]
#### 准备工作
在接入Apple Pay之前，首先要申请MerchantID及对应证书。
请移步我写的[申请MerchantID及对应证书详细图文教程](http://www.jianshu.com/p/2e5e45afc246)
#### 工程设置
bundleID设置
![](http://upload-images.jianshu.io/upload_images/1024259-16fff81828406887.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Applepay Setting1.png
Capability中启用Apple Pay权限，并选择merchantID。
![](http://upload-images.jianshu.io/upload_images/1024259-21c046559eb3d59a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Applepay Setting2.png
之后项目会多一个Applepay的配置文件ApplePayYasin.entitlements
![](http://upload-images.jianshu.io/upload_images/1024259-35739a7c9011a540.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Applepay Setting3.png
#### 需要引用的库
> 
Xcode7.0以上不需要再手动添加需要引用的库了，只需要导入头文件就可以了
```
#import <PassKit/PassKit.h>                                 //用户绑定的银行卡信息
#import <PassKit/PKPaymentAuthorizationViewController.h>    //Apple pay的展示控件
#import <AddressBook/AddressBook.h>                         //用户联系信息相关
```
#### 设备Applepay权限检测
```
if (![PKPaymentAuthorizationViewController class]) {
        //PKPaymentAuthorizationViewController需iOS8.0以上支持
        NSLog(@"操作系统不支持ApplePay，请升级至9.0以上版本，且iPhone6以上设备才支持");
        return;
    }
    //检查当前设备是否可以支付
    if (![PKPaymentAuthorizationViewController canMakePayments]) {
        //支付需iOS9.0以上支持
        NSLog(@"设备不支持ApplePay，请升级至9.0以上版本，且iPhone6以上设备才支持");
        return;
    }
    //检查用户是否可进行某种卡的支付，是否支持Amex、MasterCard、Visa与银联四种卡，根据自己项目的需要进行检测
    NSArray *supportedNetworks = @[PKPaymentNetworkAmex, PKPaymentNetworkMasterCard,PKPaymentNetworkVisa,PKPaymentNetworkChinaUnionPay];
    if (![PKPaymentAuthorizationViewController canMakePaymentsUsingNetworks:supportedNetworks]) {
        NSLog(@"没有绑定支付卡");
        return;
    }
```
#### 创建支付请求PKPaymentRequest
- 初始化PKPaymentRequest
这里需要注意RMB的币种代码是CNY
```
//设置币种、国家码及merchant标识符等基本信息
  PKPaymentRequest *payRequest = [[PKPaymentRequest alloc]init];
  payRequest.countryCode = @"CN";     //国家代码
  payRequest.currencyCode = @"CNY";       //RMB的币种代码
  payRequest.merchantIdentifier = @"merchant.ApplePayDemoYasin";  //申请的merchantID
  payRequest.supportedNetworks = supportedNetworks;   //用户可进行支付的银行卡
  payRequest.merchantCapabilities = PKMerchantCapability3DS|PKMerchantCapabilityEMV;      //设置支持的交易处理协议，3DS必须支持，EMV为可选，目前国内的话还是使用两者吧
```
- 设置发票配送信息和货物配送地址信息,用户设置后可以通过代理回调代理获取信息的更新
```
//    payRequest.requiredBillingAddressFields = PKAddressFieldEmail;   
//如果需要邮寄账单可以选择进行设置，默认PKAddressFieldNone(不邮寄账单)
//楼主感觉账单邮寄地址可以事先让用户选择是否需要，否则会增加客户的输入麻烦度，体验不好，
  payRequest.requiredShippingAddressFields = PKAddressFieldPostalAddress|PKAddressFieldPhone|PKAddressFieldName;
  //送货地址信息，这里设置需要地址和联系方式和姓名，如果需要进行设置，默认PKAddressFieldNone(没有送货地址)
```
![](http://upload-images.jianshu.io/upload_images/1024259-43c63c7a57cc2a08.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
送货信息页面展示
- 
设置货物的配送方式，不需要不配置
```
//设置两种配送方式
  PKShippingMethod *freeShipping = [PKShippingMethod summaryItemWithLabel:@"包邮" amount:[NSDecimalNumber zero]];
  freeShipping.identifier = @"freeshipping";
  freeShipping.detail = @"6-8 天 送达";
  PKShippingMethod *expressShipping = [PKShippingMethod summaryItemWithLabel:@"极速送达" amount:[NSDecimalNumber decimalNumberWithString:@"10.00"]];
  expressShipping.identifier = @"expressshipping";
  expressShipping.detail = @"2-3 小时 送达";
  payRequest.shippingMethods = @[freeShipping, expressShipping];
```
![](http://upload-images.jianshu.io/upload_images/1024259-c8a390d491efc6ef.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
ApplePayiPhone2.png
![](http://upload-images.jianshu.io/upload_images/1024259-ff33c2d6e59a960b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
ApplePayiPhone3.png
- 
账单信息的设置
- 每条账单的设置
账单列表使用`PKPaymentSummaryItem`添加描述和价格，价格使用`NSDecimalNumber`。
PKPaymentSummaryItem初始化：
label为商品名字或者是描述，amount为商品价格，折扣为负数，type为该条账单为最终价格还是估算价格(比如出租车价格预估)
`+ (instancetype)summaryItemWithLabel:(NSString *)label amount:(NSDecimalNumber *)amount;`
`+ (instancetype)summaryItemWithLabel:(NSString *)label amount:(NSDecimalNumber *)amount type:(PKPaymentSummaryItemType)type NS_AVAILABLE(NA, 9_0);`
- NSDecimalNumber初始化：
NSDecimalNumber可以使用数字初始化，也可以使用字符串。
使用方法请移步我写的[NSDecimalNumber--十进制数](http://www.jianshu.com/p/4703d704c953)
- 
添加账单列表：
```
NSDecimalNumber *subtotalAmount = [NSDecimalNumber decimalNumberWithMantissa:1275 exponent:-2 isNegative:NO];   //12.75
PKPaymentSummaryItem *subtotal = [PKPaymentSummaryItem summaryItemWithLabel:@"商品价格" amount:subtotalAmount];
NSDecimalNumber *discountAmount = [NSDecimalNumber decimalNumberWithString:@"-12.74"];      //-12.74
PKPaymentSummaryItem *discount = [PKPaymentSummaryItem summaryItemWithLabel:@"优惠折扣" amount:discountAmount];
NSDecimalNumber *methodsAmount = [NSDecimalNumber zero];
PKPaymentSummaryItem *methods = [PKPaymentSummaryItem summaryItemWithLabel:@"包邮" amount:methodsAmount];
NSDecimalNumber *totalAmount = [NSDecimalNumber zero];
totalAmount = [totalAmount decimalNumberByAdding:subtotalAmount];
totalAmount = [totalAmount decimalNumberByAdding:discountAmount];
totalAmount = [totalAmount decimalNumberByAdding:methodsAmount];
PKPaymentSummaryItem *total = [PKPaymentSummaryItem summaryItemWithLabel:@"Yasin" amount:totalAmount];  //最后这个是支付给谁。哈哈，快支付给我
summaryItems = [NSMutableArray arrayWithArray:@[subtotal, discount, methods, total]];
//summaryItems为账单列表，类型是 NSMutableArray，这里设置成成员变量，在后续的代理回调中可以进行支付金额的调整。
payRequest.paymentSummaryItems = summaryItems;
```
#### 显示购物信息并进行支付
```
//ApplePay控件
    PKPaymentAuthorizationViewController *view = [[PKPaymentAuthorizationViewController alloc]initWithPaymentRequest:payRequest];
    view.delegate = self;
    [self presentViewController:view animated:YES completion:nil];
```
#### PKPaymentAuthorizationViewControllerDelegate代理
- 这里还有两个类要介绍- PKPayment 支付成功信息
```
PKPaymentToken *payToken = payment.token;
//支付凭据，发给服务端进行验证支付是否真实有效
PKContact *billingContact = payment.billingContact;     //账单信息
PKContact *shippingContact = payment.shippingContact;   //送货信息
PKContact *shippingMethod = payment.shippingMethod;     //送货方式
```
- PKContact 联系人信息
```
NSPersonNameComponents *name = contact.name;                //联系人姓名
CNPostalAddress *postalAddress = contact.postalAddress;     //联系人地址
NSString *emailAddress = contact.emailAddress;              //联系人邮箱
CNPhoneNumber *phoneNumber = contact.phoneNumber;           //联系人手机
NSString *supplementarySubLocality = contact.supplementarySubLocality;  //补充信息，地址详细描述，其他备注等等,iOS9.2及以上才有
```
- 
代理说明
送货地址回调
```
-(void)paymentAuthorizationViewController:(PKPaymentAuthorizationViewController *)controller
                didSelectShippingContact:(PKContact *)contact
                              completion:(void (^)(PKPaymentAuthorizationStatus, NSArray<PKShippingMethod *> * _Nonnull, NSArray<PKPaymentSummaryItem *> * _Nonnull))completion{
  //contact送货地址信息，PKContact类型
  //送货信息选择回调，如果需要根据送货地址调整送货方式，比如普通地区包邮+极速配送，偏远地区只有付费普通配送，进行支付金额重新计算，可以实现该代理，返回给系统：shippingMethods配送方式，summaryItems账单列表，如果不支持该送货信息返回想要的PKPaymentAuthorizationStatus
  completion(PKPaymentAuthorizationStatusSuccess, shippingMethods, summaryItems);
}
```
送货方式回调
```cpp
```cpp
-(void)paymentAuthorizationViewController:(PKPaymentAuthorizationViewController *)controller
                 didSelectShippingMethod:(PKShippingMethod *)shippingMethod
                              completion:(void (^)(PKPaymentAuthorizationStatus, NSArray<PKPaymentSummaryItem *> * _Nonnull))completion{
  //配送方式回调，如果需要根据不同的送货方式进行支付金额的调整，比如包邮和付费加速配送，可以实现该代理
  PKShippingMethod *oldShippingMethod = [summaryItems objectAtIndex:2];
  PKPaymentSummaryItem *total = [summaryItems lastObject];
  total.amount = [total.amount decimalNumberBySubtracting:oldShippingMethod.amount];
  total.amount = [total.amount decimalNumberByAdding:shippingMethod.amount];
  [summaryItems replaceObjectAtIndex:2 withObject:shippingMethod];
  [summaryItems replaceObjectAtIndex:3 withObject:total];
  completion(PKPaymentAuthorizationStatusSuccess, summaryItems);
}
```
```
支付卡选择回调
```
-(void)paymentAuthorizationViewController:(PKPaymentAuthorizationViewController *)controller didSelectPaymentMethod:(PKPaymentMethod *)paymentMethod completion:(void (^)(NSArray<PKPaymentSummaryItem *> * _Nonnull))completion{
  //支付银行卡回调，如果需要根据不同的银行调整付费金额，可以实现该代理
  completion(summaryItems);
}
```
送货地址回调，已弃用
```
-(void)paymentAuthorizationViewController:(PKPaymentAuthorizationViewController *)controller didSelectShippingAddress:(ABRecordRef)address completion:(void (^)(PKPaymentAuthorizationStatus, NSArray<PKShippingMethod *> * _Nonnull, NSArray<PKPaymentSummaryItem *> * _Nonnull))completion{
  //送货地址回调，已弃用
}
```
付款成功苹果服务器返回信息回调，做服务器验证
```cpp
```cpp
-(void)paymentAuthorizationViewController:(PKPaymentAuthorizationViewController *)controller
                     didAuthorizePayment:(PKPayment *)payment
                              completion:(void (^)(PKPaymentAuthorizationStatus status))completion {
  PKPaymentToken *payToken = payment.token;
  //支付凭据，发给服务端进行验证支付是否真实有效
  PKContact *billingContact = payment.billingContact;     //账单信息
  PKContact *shippingContact = payment.shippingContact;   //送货信息
  PKContact *shippingMethod = payment.shippingMethod;     //送货方式
  //等待服务器返回结果后再进行系统block调用
  dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(3 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
      //模拟服务器通信
      completion(PKPaymentAuthorizationStatusSuccess);
  });
}
```
```
支付完成回调
```
-(void)paymentAuthorizationViewControllerDidFinish:(PKPaymentAuthorizationViewController *)controller{
  [controller dismissViewControllerAnimated:YES completion:nil];
}
```
demo的话因为证书问题可能会报错，不过大家可以看看代码。
[demo下载](https://github.com/SummertimSadness/ApplePayDemo)
