# iOS内购(最新)-内含遇到的"坑" - weixin_33985507的博客 - CSDN博客
2017年10月08日 22:22:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：14
内购--- iOS 客户端进行虚拟商品的支付方式
官方中文内购教程文档：[http://help.apple.com/itunes-connect/developer/#/devb57be10e7](http://help.apple.com/itunes-connect/developer/#/devb57be10e7)
需要实现类似下方这样的效果;项目里面同时有内购和微信/支付宝支付;需要了解微信/支付宝支付的请看这里:[http://www.jianshu.com/p/10dabfed0e1d?utm_campaign=hugo&utm_medium=reader_share&utm_content=note&utm_source=weixin-friends](https://www.jianshu.com/p/10dabfed0e1d?utm_campaign=hugo&utm_medium=reader_share&utm_content=note&utm_source=weixin-friends)
![2279162-2ee04dc3ae770c23.png](https://upload-images.jianshu.io/upload_images/2279162-2ee04dc3ae770c23.png)
内购.png
废话不多说,直接来看下具体流程,,,在最后我会分享内购测试过程中需要注意的点.
--------------------- 1.首先是协议部分--------------------------------
使用开发者账号登录之后在下图这里注册一系列信息
![2279162-1156cb96179f989e.png](https://upload-images.jianshu.io/upload_images/2279162-1156cb96179f989e.png)
Snip20171008_2.png
接下来如果是第一次做的话是下面第一张图
![2279162-9ee9ee261c3f28f2.jpg](https://upload-images.jianshu.io/upload_images/2279162-9ee9ee261c3f28f2.jpg)
014C11BB7E5DE352024D939523F4E5CB.jpg
如果是之前已经做过的是下图这样的
![2279162-b3edc438805e1d45.png](https://upload-images.jianshu.io/upload_images/2279162-b3edc438805e1d45.png)
Snip20171008_3.png
那么在这里如果是之前没做过的就接着下面的步骤一步一步来;如果做过了在第二张图里面点击 Edit 进行里面信息的新增和修改
![2279162-77d28a5b0bc04d86.jpg](https://upload-images.jianshu.io/upload_images/2279162-77d28a5b0bc04d86.jpg)
点击 Add New Contact .jpg
![2279162-f239abf7a6f7058f.jpg](https://upload-images.jianshu.io/upload_images/2279162-f239abf7a6f7058f.jpg)
这里写 CEO 的信息就好.jpg
填好点击右下方 Save 之后回到上一个图中就可以选择刚刚创建的新信息了,选好之后点击右下角 Done
![2279162-9195552d0c6c97fe.jpg](https://upload-images.jianshu.io/upload_images/2279162-9195552d0c6c97fe.jpg)
银行信息.jpg
![2279162-77e9990589afc0df.jpg](https://upload-images.jianshu.io/upload_images/2279162-77e9990589afc0df.jpg)
A51844806C21A5E667BF740C48478B7C.jpg
![2279162-3134981884c93370.jpg](https://upload-images.jianshu.io/upload_images/2279162-3134981884c93370.jpg)
DFE77D69CF44E04137CAEF00E69DA930.jpg
这里的 CNAPS Code 也可以在这里查询:[https://e.czbank.com/CORPORBANK/QYUK](https://e.czbank.com/CORPORBANK/QYUK)
![2279162-3656727e2751bb29.jpg](https://upload-images.jianshu.io/upload_images/2279162-3656727e2751bb29.jpg)
61207ABE0EE3B3D699333FE0F23F69A9.jpg
![2279162-8cd84b7458081d82.jpg](https://upload-images.jianshu.io/upload_images/2279162-8cd84b7458081d82.jpg)
549EC13E77199D61931E7833EFA7C19B.jpg
![2279162-0d7d637f00631bd8.jpg](https://upload-images.jianshu.io/upload_images/2279162-0d7d637f00631bd8.jpg)
税务信息jpg
![2279162-4647f5239ecebce4.jpg](https://upload-images.jianshu.io/upload_images/2279162-4647f5239ecebce4.jpg)
这是最后一页信息,往下拉,将下面的都填写完毕就 OK 了.jpg
![2279162-d1ff1fa80e0a4ffe.png](https://upload-images.jianshu.io/upload_images/2279162-d1ff1fa80e0a4ffe.png)
这里是收尾.png
-------------2.可以开始在 iTunes Connects 里面为应用创建内购了--------------------
此时如果你没创建过 APP 的话先创建一个自己的 APP, 然后在点击功能按钮就到了下面这里
![2279162-5fa150b1a68a3160.png](https://upload-images.jianshu.io/upload_images/2279162-5fa150b1a68a3160.png)
点击 + 来添加内购项目
![2279162-b00711c88437c18a.png](https://upload-images.jianshu.io/upload_images/2279162-b00711c88437c18a.png)
BCED61AAA040A0755154C8CBF6EA0505.png
![2279162-0bdfb2995d16d4de.png](https://upload-images.jianshu.io/upload_images/2279162-0bdfb2995d16d4de.png)
Snip20171008_5.png
![2279162-3db0433f815e5e90.png](https://upload-images.jianshu.io/upload_images/2279162-3db0433f815e5e90.png)
Snip20171008_6.png
![2279162-59723a0dbee637a2.png](https://upload-images.jianshu.io/upload_images/2279162-59723a0dbee637a2.png)
最新的这里是必须有图才可以
填写好之后点击右上方的存储 提交审核,就会出现下方这样子,当然我这是通过审核之后的
![2279162-bf2bb432c740a8d9.png](https://upload-images.jianshu.io/upload_images/2279162-bf2bb432c740a8d9.png)
这就是按照项目需求创建你想要的几个内购
-------------3.添加内购测试用户--------------------
这个沙盒 Apple ID 账号不注册添加的话是无法进行内购沙盒测试的
![2279162-2b7a4b3540e613fb.png](https://upload-images.jianshu.io/upload_images/2279162-2b7a4b3540e613fb.png)
Snip20171008_10.png
![2279162-22028f73a749952b.png](https://upload-images.jianshu.io/upload_images/2279162-22028f73a749952b.png)
Snip20171008_11.png
![2279162-1158248b610ba81d.png](https://upload-images.jianshu.io/upload_images/2279162-1158248b610ba81d.png)
填写好之后点击右上方的存储
-------------4.重点来了:直接上代码--------------------
HYInpurchesObserver 是我自建的一个内购类
在需要内购的代码块中调用
- (HYInpurchesObserver *)shareObserver;初始哈之后在调用
- (void)startBuy:(NSArray *)products;传入之前在 iTunes Connects 里面创建的产品 ID
- (void)addProductToPaymentQueue:(int)ProductType;这个是将选择购买产品传进去(如用户选择了充值 1元)
```
#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>
@interface HYInpurchesObserver : NSObject <SKPaymentTransactionObserver, SKProductsRequestDelegate>
+ (HYInpurchesObserver *)shareObserver;
- (void)beginObserver;
- (void)cancleObserver;
- (void)startBuy:(NSArray *)products;
- (void)addProductToPaymentQueue:(int)ProductType;
-(void)PurchasedTransaction: (SKPaymentTransaction *)transaction;
-(void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions;
@end
```
```
#import "HYInpurchesObserver.h"
#define ITMS_SANDBOX_VERIFY_RECEIPT_URL @"https://sandbox.itunes.apple.com/verifyReceipt"//沙河验证
#define ITMS_PRODUCT_VERIFY_RECEIPT_URL @"https://buy.itunes.apple.com/verifyReceipt"//生产环境
@interface HYInpurchesObserver ()
@property (nonatomic, strong) NSArray *products;
@end
@implementation HYInpurchesObserver
static HYInpurchesObserver *purchesObserver = nil;
+ (HYInpurchesObserver *)shareObserver{
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        if (!purchesObserver) {
            purchesObserver = [[HYInpurchesObserver alloc] init];
        }
    });
    return purchesObserver;
}
//开始监听购买结果
- (void)beginObserver{
    [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
}
//移除监听
- (void)cancleObserver{
    [[SKPaymentQueue defaultQueue] removeTransactionObserver:self];
}
//开始请求商品信息
- (void)startBuy:(NSArray *)products{
    if ([self canMakePay]) {
        NSLog(@"允许进行内购");
        [self requestProduct:products];
    }else{
        NSLog(@"不允许进行内购");
    }
}
-(BOOL)canMakePay{
    return [SKPaymentQueue canMakePayments];
}
- (void)requestProduct:(NSArray *)products{
    self.products = products;
    NSSet *nsset = [NSSet setWithArray:products];
    SKProductsRequest *request=[[SKProductsRequest alloc] initWithProductIdentifiers: nsset];
    request.delegate = self;
    [request start];
}
//添加商品到购买渠道
-(void)addProductToPaymentQueue:(int)ProductType{
    SKPayment *payment = [SKPayment paymentWithProductIdentifier:self.products[ProductType]];
    [[SKPaymentQueue defaultQueue] addPayment:payment];
}
#pragma mark - SKProductsRequestDelegate 收到商品反馈
- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response{
    NSArray *products = response.products;
    if (products.count == 0) {
        NSLog(@"无法获取商品信息");
    }
    NSLog(@"无效的产品%@", response.invalidProductIdentifiers);
    for (SKProduct *product in products) {
        NSLog(@"SKProduct 描述信息%@", [product description]);
                NSLog(@"产品标题 %@" , product.localizedTitle);
        NSLog(@"产品描述信息: %@" , product.localizedDescription);
        NSLog(@"价格: %@" , product.price);
        NSLog(@"Product id: %@" , product.productIdentifier);
    }
}
-(void) PurchasedTransaction: (SKPaymentTransaction *)transaction
{
    NSLog(@"-----PurchasedTransaction----");
    NSArray *transactions =[[NSArray alloc] initWithObjects:transaction, nil];
    [self paymentQueue:[SKPaymentQueue defaultQueue] updatedTransactions:transactions];
    // [transactions release];
}
//#pragma mark - SKPaymentTransactionObserver 监听购买结果
- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions{
    for (SKPaymentTransaction *transaction in transactions) {
        switch (transaction.transactionState) {
            case SKPaymentTransactionStatePurchasing:
                NSLog(@"-----商品添加进列表 --------");
                break;
            case SKPaymentTransactionStateDeferred:
                NSLog(@"-----交易延期—－－－－");
                break;
            case SKPaymentTransactionStatePurchased://交易完成
                [[NSNotificationCenter defaultCenter] postNotificationName:@"TRANSACTIONSUCCESS" object:nil];
                [self completeTransaction:transaction];
                NSLog(@"-----交易完成 --------");
                break;
            case SKPaymentTransactionStateFailed://交易失败
                [self failedTransaction:transaction];
                NSLog(@"-----交易失败 --------");
                break;
            case SKPaymentTransactionStateRestored://已经购买过该商品
                [self restoreTransaction:transaction];
                NSLog(@"-----已经购买过该商品 --------");
                
            default:
                break;
        }
    }
}
- (void)completeTransaction:(SKPaymentTransaction *)transaction{
    NSLog(@"交易结束");
    NSString *productID =  transaction.payment.productIdentifier;
    //验证购买结果
    if (productID.length > 0) {
        //向自己的服务器验证购买凭证
        //最好将返回的数据转换成 base64再传给后台,后台再转换回来;以防返回字符串中有特字符传给后台显示空
        NSString *result=[[NSString alloc]initWithData:transaction.transactionReceipt encoding:NSUTF8StringEncoding];
        NSString *environment = [self environmentForReceipt:result];
        // appStoreReceiptURL iOS7.0增加的，购买交易完成后，会将凭据存放在该地址
        NSURL *receiptURL = [[NSBundle mainBundle] appStoreReceiptURL];
        // 从沙盒中获取到购买凭据
        NSData *receiptData = [NSData dataWithContentsOfURL:receiptURL];
        NSString *encodeStr = [receiptData base64EncodedStringWithOptions:NSDataBase64EncodingEndLineWithLineFeed];
        
        NSString *sendString = [NSString stringWithFormat:@"{\"receipt-data\" : \"%@\"}", encodeStr];
        NSLog(@"_____%@",sendString);
        NSURL *StoreURL=nil;
        if ([environment isEqualToString:@"environment=Sandbox"]) {
            StoreURL= [[NSURL alloc] initWithString: ITMS_SANDBOX_VERIFY_RECEIPT_URL];
        }
        else{
            StoreURL= [[NSURL alloc] initWithString: ITMS_PRODUCT_VERIFY_RECEIPT_URL];
        }
        //******这个二进制数据由服务器进行验证*****************************
        NSData *postData = [NSData dataWithBytes:[sendString UTF8String] length:[sendString length]];
        
        //结束交易
        [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
        
    }
}
- (void) failedTransaction: (SKPaymentTransaction *)transaction
{
    
    NSLog(@"交易失败2");
    if (transaction.error.code == SKErrorPaymentCancelled)
    {
        UIAlertView * alert=[[UIAlertView alloc]initWithTitle:@"提 示" message:@"你已取消购买"  delegate:nil cancelButtonTitle:nil otherButtonTitles:@"确定", nil];
        [alert show];
    }
    else if(transaction.error.code==SKErrorPaymentInvalid)
    {
        UIAlertView * alert=[[UIAlertView alloc]initWithTitle:@"提 示" message:@"支付无效"  delegate:nil cancelButtonTitle:nil otherButtonTitles:@"确定", nil];
        [alert show];
        
    }
    else if(transaction.error.code==SKErrorPaymentNotAllowed)
    {
        UIAlertView * alert=[[UIAlertView alloc]initWithTitle:@"提 示" message:@"不允许支付"  delegate:nil cancelButtonTitle:nil otherButtonTitles:@"确定", nil];
        [alert show];
    }
    else if(transaction.error.code==SKErrorStoreProductNotAvailable)
    {
        UIAlertView * alert=[[UIAlertView alloc]initWithTitle:@"提 示" message:@"产品无效"  delegate:nil cancelButtonTitle:nil otherButtonTitles:@"确定", nil];
        [alert show];
    }
    else if(transaction.error.code==SKErrorClientInvalid)
    {
        UIAlertView * alert=[[UIAlertView alloc]initWithTitle:@"提 示" message:@"客服端无效"  delegate:nil cancelButtonTitle:nil otherButtonTitles:@"确定", nil];
        [alert show];
    }
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
  
}
//收据的环境判断；
-(NSString * )environmentForReceipt:(NSString * )str
{
    str= [str stringByReplacingOccurrencesOfString:@"\r\n" withString:@""];
    str = [str stringByReplacingOccurrencesOfString:@"\n" withString:@""];
    str = [str stringByReplacingOccurrencesOfString:@"\t" withString:@""];
    str=[str stringByReplacingOccurrencesOfString:@" " withString:@""];
    str=[str stringByReplacingOccurrencesOfString:@"\"" withString:@""];
    NSArray * arr=[str componentsSeparatedByString:@";"];
    //存储收据环境的变量
    NSString * environment=arr[2];
    return environment;
}
@end
```
到此:内购流程就到一段落了;但是在内购中还会遇到一些问题.先集中解决如下:
-------------5.内购中容易遇到的坑[注意:要用真机测试]--------------------
```
1.Q:如果提示--找不到对应的商品---- 
   A1:检查项目 Bundle id - 证书 - 产品 ID 是否对应
   A2:上面一开始填写的银行信息和税务信息是否正确
   A3:在 iTunes Connect 里面创建的产品是否已通过审核,通过才可以的
```
```
2.Q:如果提示 --无法连接到 iTunes Store---类似下图
  A1:上面有没有注册沙箱测试人员,必须注册之后才可以使用
 A2:将手机上的 App Store 里面的账号退出;再将设置里面的个人信息账号退出;以及设置里面 iTunes Store 与 App Store 里的账号也退出
 A3:如果执行上面两步之后还不行,此时需要在设置里面 iTunes Store 与 App Store 里登录你在沙箱注册的测试账号以后再进行
 A4:如果执行了上面的4步任然不行,那就需要等明天再试.(原因:苹果服务器在中国不稳定),这是经过苹果客服和大多数人的经验总结出来的.
```
![2279162-97330b322a9dd6e9.png](https://upload-images.jianshu.io/upload_images/2279162-97330b322a9dd6e9.png)
Snip20171008_14.png
如果可以测试成功就是下面截图流程
![2279162-7635510adfc0a88e.jpg](https://upload-images.jianshu.io/upload_images/2279162-7635510adfc0a88e.jpg)
点击使用现有的 AppleID
![2279162-849cf2d9c056a9bd.jpg](https://upload-images.jianshu.io/upload_images/2279162-849cf2d9c056a9bd.jpg)
由于是沙盒测试,所以会显示[Envirionment:Sandbox]
![2279162-90d70e054c24c970.jpg](https://upload-images.jianshu.io/upload_images/2279162-90d70e054c24c970.jpg)
411507471733_.pic.jpg
这里是官方内购参考文档:[https://developer.apple.com/library/content/technotes/tn2387/_index.html](https://developer.apple.com/library/content/technotes/tn2387/_index.html)
后记
关于支付成功之后的验证有两种:内置模式--和服务器模式;具体可以看这篇文章的详细讲解:[http://www.jianshu.com/p/e7722bc578c0](https://www.jianshu.com/p/e7722bc578c0)
以后这个帖子会一直更新;如果能帮到你是我最大的荣幸;
如果有你在开发过程中有遇到问题,欢迎在下方提问....
ps：喜欢我的朋友多多支持，用你的小手点个喜欢或关注，就是对我最大的鼓励。
