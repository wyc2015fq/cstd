# iOS IAP应用内购详细步骤和问题总结指南 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年12月21日 13:43:21[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：261
> 
最近公司在做APP内购会员功能 遇到了很多问题 总结记录一下 首先一定要区分Apple pay 和IAP内购的区别
可以先去看一下[官方文档地址](https://help.apple.com/app-store-connect/#/devae49fb316) 有每个步骤的详细解释
**本篇文章分为：1、 内购支付流程；2、开发集成步骤；3、问题（遇坑）记录解决方式**
**之前没看官方文档走了很多弯路 网上博客并不系统 强烈建议先过一遍官方文档**
### 先看一下IAP内购支付流程（官方）
![](https://upload-images.jianshu.io/upload_images/2145905-29ab01d34e9ba0f2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/547/format/webp)
官方流程图
> - 程序向服务器发送请求，获得一份产品列表。
- 服务器返回包含产品标识符的列表。
- 程序向App Store发送请求，得到产品的信息。
- App Store返回产品信息。
- 程序把返回的产品信息显示给用户（App的store界面）
- 用户选择某个产品
- 程序向App Store发送支付请求
- App Store处理支付请求并返回交易完成信息。
- 程序从信息中获得数据，并发送至服务器。
- 服务器纪录数据，并进行审(我们的)查。
- 服务器将数据发给App Store来验证该交易的有效性。
- App Store对收到的数据进行解析，返回该数据和说明其是否有效的标识。
- 服务器读取返回的数据，确定用户购买的内容。
- 服务器将购买的内容传递给程序。
第一步：内购账户税务协议、银行卡绑定相关
> 
一般都是运营或者产品经理处理这步 这篇文章图文步骤比较详细 处理税务银行相关设置 [IAP,In App Purchases-在APP内部支付](https://blog.csdn.net/j_av_a/article/details/74407814)
第二步：Xcode设置相关
> 
打开In-App Purchase开关 对应在开发者证书中心的项目证书中显示应该也是可用状态
![](https://upload-images.jianshu.io/upload_images/2145905-8ad45bc812884f9b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
屏幕快照 2018-08-22 下午6.00.11.png
![](https://upload-images.jianshu.io/upload_images/2145905-227ccb3dc7c50f81.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
屏幕快照 2018-08-22 下午6.01.35.png
第三步：在App Store Content -> 我的APP 添加内购项目商品
> - 在[首页](https://appstoreconnect.apple.com/)上，点按“我的 App”，然后选择与该 App 内购买项目相关联的 App。
- 在工具栏中，点按“功能”，然后在左列中点按“App 内购买项目”。
- 若要添加 App 内购买项目，请前往“App 内购买项目”，并点按“添加”按钮（+）。
![](https://upload-images.jianshu.io/upload_images/2145905-15c2c08d99514d61.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
屏幕快照 2018-08-23 上午10.06.23.png
**选择功能 添加内购项目商品**
![](https://upload-images.jianshu.io/upload_images/2145905-f4f33d114e22daec.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
选择功能Tab
> 
内购商品对应四种类型 消耗型、非消耗型、自动续订订阅型、非续订订阅型
[官方文档](https://help.apple.com/app-store-connect/#/devae49fb316)
- 选择“消耗型项目”、“非消耗型项目”或“非续订订阅”，并点按“创建”。有关自动续订订阅的信息，请参见[创建自动续期订阅](https://help.apple.com/app-store-connect/#/dev06f89ce98)。
- 添加参考名称、产品 ID 和本地化显示名称。
- 点按“存储”或“提交以供审核”。
```
您可以在创建您的 App 内购买项目时输入所有的元数据，或稍后输入您的 App 内购买项目信息。
```
![](https://upload-images.jianshu.io/upload_images/2145905-164f5b60114dbe94.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
屏幕快照 2018-08-23 上午10.09.34.png
> 
添加一个测试商品 其他属性都可以随意填写 产品ID一定要认真填写 项目中需要根据ID获取商品信息 价格有不同的等级可以选 最低备用等级1 == 1元
填写完成之后储存 就完成了一个内购商品的添加
![](https://upload-images.jianshu.io/upload_images/2145905-cb742fe4e2b9509e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
屏幕快照 2018-08-23 上午10.16.31.png
第四步：沙盒环境测试账号
> 
因为涉及到钱相关 总不能直接用money去支付吧 所以需要你去添加一个沙盒技术测试人员的账号 （这个账号是虚拟的） 付款不会扣你
**看第三步那张图 在App Store Content 选择用户和职能 进入下面页面 选择沙箱技术测试员 添加测试账号**
![](https://upload-images.jianshu.io/upload_images/2145905-cd25276aa18890b4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
屏幕快照 2018-08-23 上午11.02.26.png
![](https://upload-images.jianshu.io/upload_images/2145905-e3b5d6014296c99d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
屏幕快照 2018-08-23 上午11.05.28.png
**Tips：Q：为什么添加沙箱技术测试员 注册不成功 Unknown Email xxxxxx**
**首先这里有个坑 邮箱只要符合格式就可以 虚假邮箱也可以 但密码必须符合正式的要求要有大小写和字符 复杂就好 例如:Lh123456***
第五步：代码实现（初步，未进行优化 有什么问题可以在评论中跟我沟通）
**.h文件**
```
typedef void(^XSProductStatusBlock)(BOOL isStatus);
@interface XSApplePayManager : NSObject
+ (instancetype)shareManager;
/** 检测客户端与服务器漏单情况处理*/
+ (void)checkOrderStatus;
/**
  根据商品ID请求支付信息
 @param orderId 订单号
 @param productId 商品号
 @param statusBlock 回掉block
 */
- (void)requestProductWithOrderId:(NSString *)orderId
                        productId:(NSString *)productId
                      statusBlock:(XSProductStatusBlock)statusBlock;
```
**.m文件**
```
#import <StoreKit/StoreKit.h>
#import "APIManager.h"
#import "UIAlertView+AABlock.h"
@interface XSApplePayManager ()<SKProductsRequestDelegate,SKPaymentTransactionObserver>
@property (nonatomic, copy) NSString *orderId;
@property (nonatomic, copy) XSProductStatusBlock statusBlcok;
@end
@implementation XSApplePayManager
+ (instancetype)shareManager
{
    static dispatch_once_t onceToken;
    static XSApplePayManager *manager = nil;
    dispatch_once(&onceToken, ^{
        manager = [[XSApplePayManager alloc]init];
    });
    return manager;
}
/** 检测客户端与服务器漏单情况处理*/
+ (void)checkOrderStatus
{
    NSDictionary *orderInfo = [XSApplePayManager getReceiptData];
    if (orderInfo != nil) {
        
        NSString *orderId = orderInfo[@"orderId"];
        NSString *receipt = orderInfo[@"receipt"];
        
        [[XSApplePayManager shareManager] verifyPurchaseForServiceWithOrderId:orderId receipt:receipt];
    }
}
#pragma mark -- 结束上次未完成的交易
-(void)removeAllUncompleteTransactionsBeforeNewPurchase{
    
    NSArray* transactions = [SKPaymentQueue defaultQueue].transactions;
    
    if (transactions.count >= 1) {
        
        for (SKPaymentTransaction* transaction in transactions) {
            if (transaction.transactionState == SKPaymentTransactionStatePurchased ||
                transaction.transactionState == SKPaymentTransactionStateRestored) {
                [[SKPaymentQueue defaultQueue]finishTransaction:transaction];
            }
        }
        
    }else{
        NSLog(@"没有历史未消耗订单");
    }
}
/** 检测权限 添加支付监测 开始支付流程*/
- (void)requestProductWithOrderId:(NSString *)orderId
                        productId:(NSString *)productId
                      statusBlock:(XSProductStatusBlock)statusBlock
{
    
    if (orderId == nil || productId == nil) {
        [AAProgressManager showFinishWithStatus:@"订单号/商品号有误"];
        return;
    }
    
    if ([[XZDeviceManager didRoot] isEqualToString:@"didRoot"]) {//写自己的越狱判断方法
        [AAProgressManager showFinishWithStatus:@"越狱手机不支持内购"];
        return;
    }
    
    
    if([SKPaymentQueue canMakePayments]){
        
        [self removeAllUncompleteTransactionsBeforeNewPurchase];
        
        [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
        self.orderId = orderId;
        self.statusBlcok = statusBlock;
        [self requestProductData:productId];
        
    }else{
        [AAProgressManager showFinishWithStatus:L(@"请打开应用内支付功能")];
    }
}
/** 去Apple IAP Service 根据商品ID请求商品信息*/
- (void)requestProductData:(NSString *)type{
    
    [AAProgressManager showWithStatus:@"正在请求..."];
    NSArray *product = [[NSArray alloc] initWithObjects:type,nil];
    
    NSSet *nsset = [NSSet setWithArray:product];
    SKProductsRequest *request = [[SKProductsRequest alloc] initWithProductIdentifiers:nsset];
    request.delegate = self;
    [request start];
}
#pragma mark -- SKProductsRequestDelegate
//收到产品返回信息
- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response{
    
    NSArray *product = response.products;
    if([product count] == 0){
        [AAProgressManager showFinishWithStatus:L(@"无法获取商品信息，请重新尝试购买")];
        return;
    }
    
    NSLog(@"产品付费数量:%ld",product.count);
    
    SKProduct *p = product.firstObject;
    
    SKMutablePayment *payment = [SKMutablePayment paymentWithProduct:p];
    payment.quantity = (NSInteger)p.price;//购买次数=价钱
    if (payment.quantity == 0) {
        payment.quantity = 1;
    }
    payment.applicationUsername = self.orderId;//[NSString stringWithFormat:@"%@",[[AAUserManager shareManager] getUID]];
    [[SKPaymentQueue defaultQueue] addPayment:payment];
}
//请求失败
- (void)request:(SKRequest *)request didFailWithError:(NSError *)error{
    NSLog(@"------------------错误-----------------:%@", error);
    if (self.statusBlcok) {
        self.statusBlcok(NO);
    }
    [AAProgressManager showFinishWithStatus:L(@"从Apple获取商品信息失败")];
}
- (void)requestDidFinish:(SKRequest *)request{
    NSLog(@"------------反馈信息结束-----------------%@",request);
}
#pragma mark -- 监听AppStore支付状态
- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transaction{
    
    NSLog(@"监听AppStore支付状态");
    dispatch_async(dispatch_get_main_queue(), ^{
        for(SKPaymentTransaction *tran in transaction){
            switch (tran.transactionState) {
                case SKPaymentTransactionStatePurchased:
                {
                    // 发送到苹果服务器验证凭证
                    [self verifyPurchaseWithPaymentTransaction];
                    [[SKPaymentQueue defaultQueue] finishTransaction:tran];
                }
                    break;
                case SKPaymentTransactionStatePurchasing:
                    NSLog(@"商品添加进列表");
                    break;
                case SKPaymentTransactionStateRestored:
                {
                    [AAProgressManager showFinishWithStatus:L(@"已经购买过商品")];
                    [[SKPaymentQueue defaultQueue] finishTransaction:tran];
                }
                    break;
                case SKPaymentTransactionStateFailed:
                {
                    if (self.statusBlcok) {
                        self.statusBlcok(NO);
                    }
                    NSLog(@"交易失败");
                    [[SKPaymentQueue defaultQueue] finishTransaction:tran];
                }
                    break;
                case SKPaymentTransactionStateDeferred:
                {
                    [AAProgressManager showFinishWithStatus:L(@"最终状态未确定")];
                }
                    break;
                default:
                    break;
            }
        }
    });
    
}
#pragma mark -- 验证
/**验证购买，避免越狱软件模拟苹果请求达到非法购买问题*/
-(void)verifyPurchaseWithPaymentTransaction{
    
    //从沙盒中获取交易凭证并且拼接成请求体数据
    NSURL *receiptUrl = [[NSBundle mainBundle] appStoreReceiptURL];
    NSData *receiptData = [NSData dataWithContentsOfURL:receiptUrl];
    NSString *receiptString = [receiptData base64EncodedStringWithOptions:NSDataBase64EncodingEndLineWithLineFeed];
    [self saveReceiptData:@{@"receipt":receiptString,
                            @"orderId":self.orderId}];
    
  
    [self verifyPurchaseForServiceWithOrderId:self.orderId
                                      receipt:receiptString];
}
- (void)verifyPurchaseForServiceWithOrderId:(NSString *)orderId
                                    receipt:(NSString *)receiptString
{
    if (orderId == nil && receiptString == nil) {
        if (self.statusBlcok) {
            self.statusBlcok(NO);
        }
        [AAProgressManager showFinishWithStatus:@"订单号/凭证无效"];
        return;
    }
    
    [self removeTransaction];
    [AAProgressManager showWithStatus:@"正在验证服务器..."];
    
    WS(weakSelf);
    [[APIManager sharedInstance] verifyPurchaseWithOrderID:orderId
                                                    params:@{@"ceceipt-data":receiptString}
                                                   success:^(id response)
     {
         dispatch_async(dispatch_get_main_queue(), ^{
             [AAProgressManager dismiss];
             [AAProgressManager showFinishWithStatus:L(@"交易完成")];
             [weakSelf removeLocReceiptData];
             if (weakSelf.statusBlcok) {
                 weakSelf.statusBlcok(YES);
             }
         });
         
     } failure:^(NSError *error) {
         dispatch_async(dispatch_get_main_queue(), ^{
             
             [CommonFunction showError:error];
             [weakSelf verifyPurchaseFail];
         });
     }];
}
- (void)verifyPurchaseFail
{
    WS(weakSelf);
    UIAlertView *altert =[UIAlertView alertViewWithTitle:@"服务器验证失败"
                                                 message:@"账单在验证服务器过程中出现错误，\n请检查网络环境是否可以再次验证\n如果取消可在网络环境良好的情况下重新启动行者可再次继续验证支付"
                                       cancelButtonTitle:L(@"取消")
                                       otherButtonTitles:@[L(@"再次验证")]
                                               onDismiss:^(NSInteger buttonIndex)
                          {
                              dispatch_async(dispatch_get_main_queue(), ^
                                             {
                                                 [XSApplePayManager checkOrderStatus];
                                             });           ;
                              
                          } onCancel:^{
                              dispatch_async(dispatch_get_main_queue(), ^{
                                  
                                  if (weakSelf.statusBlcok) {
                                      weakSelf.statusBlcok(NO);
                                  }
                                  [PromptInfo showWithText:@"可在网络环境良好的情况下重新启动行者可再次继续验证支付"];
                                  
                              });
                          }];
    [altert show];
}
//交易结束
- (void)completeTransaction:(SKPaymentTransaction *)transaction
{
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
}
- (void)removeTransaction
{
    [[SKPaymentQueue defaultQueue] removeTransactionObserver:self];
}
#pragma mark -- 本地保存一次支付凭证
static NSString *const kSaveReceiptData = @"kSaveReceiptData";
- (void)saveReceiptData:(NSDictionary *)receiptData
{
    [[NSUserDefaults standardUserDefaults] setValue:receiptData forKey:kSaveReceiptData];
    [[NSUserDefaults standardUserDefaults]synchronize];
}
+ (NSDictionary *)getReceiptData
{
    return [[NSUserDefaults standardUserDefaults] valueForKey:kSaveReceiptData];
}
- (void)removeLocReceiptData
{
    [[NSUserDefaults standardUserDefaults] removeObjectForKey:kSaveReceiptData];
    [[NSUserDefaults standardUserDefaults] synchronize];
}
```
第六步：IAP支付流程 & 服务器验证流程
> 
整个支付流程如下：
1.客户端向Appstore请求购买产品（假设产品信息已经取得），Appstore验证产品成功后，从用户的Apple账户余额中扣费。
2.Appstore向客户端返回一段receipt-data，里面记录了本次交易的证书和签名信息。
3.客户端向我们可以信任的服务器提供receipt-data
4.服务器对receipt-data进行一次base64编码
5.服务器把编码后的receipt-data发往itunes.appstore进行验证
6.itunes.appstore返回验证结果给服务器
7.服务器对商品购买状态以及商品类型，向客户端发放相应的道具与推送数据更新通知
漏单处理 确保receipt-data的成功提交与异常处理
> 
建立在IAP Server Model的基础上，并且我们知道手机网络是不稳定的，在付款成功后不能确保把receipt-data一定提交到服务器。如果出现了这样的情况，那就意味着玩家被appstore扣费了，却没收到服务器发放的道具。
**漏单处理：**
解决这个问题的方法是在客户端提交receipt-data给我们的服务器，让我们的服务器向苹果服务器发送验证请求，验证这个receipt-data账单的有效性. 在没有收到回复之前，客户端必须要把receipt-data保存好，并且定期或在合理的UI界面触发向服务端发起请求，直至收到服务端的回复后删除客户端的receipt账单记录。
如果是客户端没成功提交receipt-data，那怎么办？就是玩家被扣费了，也收到appstore的消费收据了，却依然没收到游戏道具，于是投诉到游戏客服处。
> 
这种情况在以往的经验中也会出现，常见的玩家和游戏运营商发生的纠纷。游戏客服向玩家索要游戏账号和appstore的收据单号，通过查询itunes-connect看是否确有这笔订单。如果订单存在，则要联系研发方去查询游戏服务器，看订单号与玩家名是否对应，并且是否已经被使用了，做这一点检查的目的是 为了防止恶意玩家利用已经使用过了的订单号进行欺骗(已验证的账单是可以再次请求验证的,曾经为了测试,将账单手动发给服务器处理并成功)，谎称自己没收到商品。这就是上面一节IAP Server Model中红字所提到的安全逻辑的目的。当然了，如果查不到这个订单号，就意味着这个订单确实还没使用过，手动给玩家补发商品即可。
*更多可以查看这篇博文[苹果IAP安全支付与防范 receipt收据验证](https://blog.csdn.net/teng_ontheway/article/details/47023119)*
### 遇到的坑
Q：21004 你提供的共享密钥和账户的共享密钥不一致 什么是共享密钥？ 共享密钥从哪里获取？
> 
**A：**先看一下官方文档怎么说[生成收据验证代码](https://help.apple.com/app-store-connect/#/devf341c0f01)
为了在验证自动续期订阅时提高您的 App 与 Apple 服务器交易的安全性，您可以在收据中包含一个 32 位随机生成的字母数字字符串，作为共享密钥。
在 App Store Connect 中生成共享密钥。您可以生成一个主共享密钥，作为您所有 App 的单一代码，或作为针对单个 App 的 App 专用共享密钥。您也可以针对您的部分 App 使用主共享密钥，其他 App 使用 App 专用共享密钥。
点击下面展开就可以看到共享密钥生成的方式
Q：沙箱技术测试人员添加不成功 总是提示邮箱错误
> 
**A：** 沙箱技术测试账号用于付款测试 任意未创建过Apple ID 的邮箱都可以 假的邮箱也可以 重要的是密码格式一定要包含大小写 跟正式账号注册规则一样 （例如：Lh123456*）
Q：自己服务器向苹果服务器验证收据/凭证参数是什么？向status code 验证apple iap sever的状态码代表什么意思？
> 
**A:**21002、21003、21004、21005、21006、21007... 具体可以查看这篇文档[用App Store验证收据](https://developer.apple.com/library/archive/releasenotes/General/ValidateAppStoreReceipt/Chapters/ValidateRemotely.html#//apple_ref/doc/uid/TP40010573-CH104-SW1)
Q：Apple 和IAP的区别
> 
**A:**IAP是链接App store的内购服务 一般是虚拟商品需要走的通道（比如会员功能）
Apple Pay是苹果跟各大银行合作的卡包形式的类似于刷卡支付服务 一般用于现实场景
这两个一定别搞混了
Q:怎么通过itunes-connect查看具体订单，itunes-connect中无法直接看到订单信息，可以用以下方法来查询
> 
1.可以通过账单向苹果发送账单验证，有效可以手动补发
2 .用自己的服务器的记录账单列表对比
3.利用第三方的TalkingData等交易函数，会自动记录账单数据
还有一些问题可以借鉴一下这篇博文[iOS之你一定要看的内购破解-越狱篇](https://www.jianshu.com/p/5cf686e92924) 他遇到的实际问题比较多 按需借鉴
觉得有帮助可以关注我 后续继续补充....
作者：小怪兽饲养猿
链接：https://www.jianshu.com/p/307a3b7cfd6c
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
