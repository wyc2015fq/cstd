# 处理ipv6和内购(IAP)及掉单问题的正确姿势 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年06月02日 16:35:47[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：2618
> 
本文来自简书，原文地址:[http://www.jianshu.com/p/b7195675ffdd](http://www.jianshu.com/p/b7195675ffdd)
最近开发一个项目涉及到内购, 也遇到过一些问题. 这里拿出来分享一下, 避免一些人走弯路.
开头先聊一聊最近苹果关于2017年新的审核机制和沸沸扬扬的微信和苹果的撕逼
### 1. 2017新的审核机制:
- 
ipv6: 使用国内阿里云的app上架, 大都会遇到ipv6被拒的邮件:
解决方案:
方案1. 服务端解决: [配置阿里云ECS支持IPv6, 添加AAAA解析](https://blog.6ag.cn/1859.html)
方案2. 客户端解决: 手机端配置ipv6环境测试, 录制APP内的操作视频, 上传到`YouTobe`,
 将网址发送给审核人员即可通过审核 (ps: 录制时候一定要录制APP所在的网络环境: 设置中->无线网络->DNS: 2001:2:0:aab1: :: 1 ,DNS为这种格式则为ipv6)
- 
内购:
说一说这个项目内购有趣的事情:
a. 首先做这个项目的时候, 我们充值虚拟币方案定的是: 后台做一个开关, app在审核期间走苹果内购, 在上线后, 走微信和支付宝支付, 并向低版本兼容. 达到绕过苹果审核的目的. 结果被拒了, 邮件中提到了支付宝, 当时很懵逼, 就留下了老大的联系方式和苹果沟通, 第二天苹果打来电话: 说内购的同时不可以使用第三方支付. 由此看来: 第三方支付的相关相关代码或SDK被扫描到了. 遂移除掉, 只使用内购方式
b. 审核期间, 苹果发来一封邮件大概意思是问: `你们确定内购的最高价格是你们期望的吗? 回复以后才可以继续审核`,
 这里我的理解是: 我们的内购的最高价格定得很高`149美元`的那一档, 所以苹果要确认一下, 经过回复邮件说明了一下
```
这个最高价格确定是我们自己定的最高价格,
 没有错误
```
, 第二天苹果又恢复了审核, 变成了审核中...
c. app被拒后, 内购项目变成了`需要开发人员操作`, 盗图一张:
![](http://upload-images.jianshu.io/upload_images/1829781-8da184fb823ca653.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
需要开发人员操作
这时候一般只需要进入`需要开发人员操作`的内购项目中, 修改一下`描述`,
 重新提交即可, 然后重新提交app. (ps: 一般这里我只是将描述中添加或删除空格, 就可以重新提交了)
d. 关于项目中: app内购商品返回列表为空, 返回的都是无效产品
即: [response.products count]始终为0, [response.invalidProductIdentifiers] 有值
这个的原因是: 协议、税务和银行业务中必须通过才可以(盗图一张):
![](http://upload-images.jianshu.io/upload_images/1829781-997e401119254839.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
协议、税务和银行业务
### 2. 谈一谈微信和苹果的撕逼
新的审核协议将打赏列为了内购
我的观点和这个[仁兄](https://www.zhihu.com/question/60141705/answer/173822367)一样
### 3. 闲话扯完了, 看一下怎么做内购并处理掉单问题:
[苹果官方提供的内购的正确姿势](https://developer.apple.com/library/content/technotes/tn2387/_index.html#//apple_ref/doc/uid/DTS40014795)
苹果这一文中说明两点:
a. 在appdelegate中添加观察者, 在购买成功后提交给自己的服务器, 由自己服务器提交凭证到苹果服务器验证正确后, 返回给客户端之后, 这笔交易才完成, 这时候再`queue.finishTransaction(transaction)`,
 如果这期间苹果的服务器还没返回结果 或者 购买成功了,我们提交凭证给自己服务器的时候网断掉了(钱空了, 但是虚拟物品没有到账, 丢单了), 则这笔交易都没有完成, 方法`queue.finishTransaction(transaction)`都没有调用,
 所以再次打开app的时候, 因为appdelegate中添加了观察者, 就会再次调用
```
func paymentQueue(_ queue: SKPaymentQueue, updatedTransactions
 transactions: [SKPaymentTransaction])
```
方法
b. 苹果推荐进入内购项目表单页面的时候先请求appstore,根据返回的可销售商品来进行展示(但是很多app的做法都是调用自己的接口取得商品价格列表进行展示, 但是我们不能确定我们自己的服务器返回的和苹果返回的不同), 这里非常抱歉的说明一下: 我们的app也是按照自己服务器的api返回的数据展示的商品价格列表, 哈哈哈
c. 关于内购和服务端的接口参数, 我们设置为:
- 此次交易的用户的唯一标示符(accountID):
- 交易成功的凭证
- 此次交易的订单号
- 服务端也要处理重复请求该接口的情况(不要每次请求成功都给用户加钱..)
说明: 用户的唯一标示符的作用: 如果用户购买成功, 但是将凭证给自己服务端的时候断掉了, 然后自己切换了账号, 下次打开app的时候检测, 我们需要这个表示符知道谁买的..不要将虚拟货币充错用户
> 
ios7 苹果增加了一个属性applicationusername，SKMutablepayment的属性，所以用户在发起支付的时候可以指定用户的username及自己生成的订单，这样用户再下次得到回调的时候就知道，此交易是哪个订单发起的了进而完成交易。回调中获取username。
上代码: (内购工具类)
```
import Foundation
import StoreKit
enum InpurchaseError: Error {
    /// 没有内购许可
    case noPermission
    /// 不存在该商品: 商品未在appstore中\商品已经下架
    case noExist
    /// 交易结果未成功
    case failTransactions
    /// 交易成功但未找到成功的凭证
    case noReceipt
}
typealias Order = (productIdentifiers: String, applicationUsername: String)
class Inpurchase: NSObject, SKPaymentTransactionObserver, SKProductsRequestDelegate {
    static let `default` = Inpurchase()
    /// 掉单/未完成的订单回调 (凭证, 交易, 交易队列)
    var unFinishedTransaction: ((String, SKPaymentTransaction, SKPaymentQueue) -> ())?
    private var sandBoxURLString = "https://sandbox.itunes.apple.com/verifyReceipt"
    private var buyURLString = "https://buy.itunes.apple.com/verifyReceipt"
    private var isComplete: Bool = true
    private var products: [SKProduct] = []
    private var failBlock: ((InpurchaseError) -> ())?
    /// 交易完成的回调 (凭证, 交易, 交易队列)
    private var receiptBlock: ((String, SKPaymentTransaction, SKPaymentQueue) -> ())?
    private var successBlock: (() -> Order)?
    private override init() {
        super.init()
        SKPaymentQueue.default().add(self)
    }
    deinit {
        SKPaymentQueue.default().remove(self)
    }
    /// 开始向Apple Store请求产品列表数据，并购买指定的产品，得到Apple Store的Receipt，失败回调
    ///
    /// - Parameters:
    ///   - productIdentifiers: 请求指定产品
    ///   - successBlock: 请求产品成功回调，这个时候可以返回需要购买的产品ID和用户的唯一标识，默认为不购买
    ///   - receiptBlock: 得到Apple Store的Receipt和transactionIdentifier，这个时候可以将数据传回后台或者自己去post到Apple Store
    ///   - failBlock: 失败回调
    func start(productIdentifiers: Set<String>,
               successBlock: (() -> Order)? = nil,
               receiptBlock: ((String, SKPaymentTransaction, SKPaymentQueue) -> ())? = nil,
               failBlock: ((InpurchaseError) -> ())? = nil) {
        guard isComplete else { return }
        defer { isComplete = false }
        let request = SKProductsRequest(productIdentifiers: productIdentifiers)
        request.delegate = self
        request.start()
        self.successBlock = successBlock
        self.receiptBlock = receiptBlock
        self.failBlock = failBlock
    }
    //MARK: - SKProductsRequestDelegate
    func productsRequest(_ request: SKProductsRequest, didReceive response: SKProductsResponse) {
        products = response.products
        guard let order = successBlock?() else { return }
        buy(order)
    }
    /// 购买给定的order的产品
    private func buy(_ order: Order) {
        let p = products.first { $0.productIdentifier == order.productIdentifiers }
        guard let product = p else { failBlock?(.noExist); return }
        guard SKPaymentQueue.canMakePayments() else { failBlock?(.noPermission); return }
        let payment = SKMutablePayment(product: product)
        /// 发起支付时候指定用户的username, 在掉单时候验证防止切换账号导致充值错误
        payment.applicationUsername = order.applicationUsername
        SKPaymentQueue.default().add(payment)
    }
    //MARK: - SKPaymentTransactionObserver
    func paymentQueue(_ queue: SKPaymentQueue, updatedTransactions transactions: [SKPaymentTransaction]) {
        for transaction in transactions {
            switch transaction.transactionState {
            case .purchased:
                // appStoreReceiptURL iOS7.0增加的，购买交易完成后，会将凭据存放在该地址
                guard let receiptUrl = Bundle.main.appStoreReceiptURL,
                      let receiptData = NSData(contentsOf: receiptUrl) else { failBlock?(.noReceipt);return }
                let receiptString = receiptData.base64EncodedString(options: NSData.Base64EncodingOptions(rawValue: 0))
                if let receiptBlock = receiptBlock {
                    receiptBlock(receiptString, transaction, queue)
                }else{ // app启动时恢复购买记录
                    unFinishedTransaction?(receiptString, transaction, queue)
                }
                isComplete = true
            case .failed:
                failBlock?(.failTransactions)
                queue.finishTransaction(transaction)
                isComplete = true
            case .restored: // 购买过 对于购买过的商品, 回复购买的逻辑
                queue.finishTransaction(transaction)
                isComplete = true
            default:
                break
            }
        }
    }
}
```
appdelegate中的监听使用方式:
```
appdelegate中: 
func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplicationLaunchOptionsKey: Any]?) -> Bool {
        Inpurchase.default.unFinishedTransaction = {(receipt, transaction, queue) in
            // 如果存在掉单情况就会走这里
            let data = InpurchaseAPIData(accountID: transaction.payment.applicationUsername, //用户唯一标示
                                         transactionID: transaction.transactionIdentifier, //交易流水
                                         receiptData: receipt)// 凭证
            LPNetworkManager.request(Router.verifyReceipt(data)).showToast().loading(in: self.view).success {[weak self] in
                showToast("恢复购买成功")
                // 记住一定要请求自己的服务器成功之后, 再移除此次交易
                queue.finishTransaction(transaction)
                }.fail {
                    print("向服务器发送凭证失败")
            }
        }
        return true
    }
```
点击购买的代码:
```
// 点击购买
        let productIdentifiers: Set<String> = ["a", "b", "c"]
        Inpurchase.default.start(productIdentifiers: productIdentifiers, successBlock: { () -> Order in
            return (productIdentifiers: "a", applicationUsername: "该用户的id或改用户的唯一标识符")
        }, receiptBlock: { (receipt, transaction, queue) in
            //交易成功返回了凭证
            let data = InpurchaseAPIData(accountID: transaction.payment.applicationUsername,
                                         transactionID: transaction.transactionIdentifier,
                                         receiptData: receipt)
            LPNetworkManager.request(Router.verifyReceipt(data)).showToast().loading(in: self.view).success {[weak self] in
                showToast("购买成功")
                // 记住一定要请求自己的服务器成功之后, 再移除此次交易
                queue.finishTransaction(transaction)
                }.fail {
                    print("向服务器发送凭证失败")
            }
        }, failBlock: { (error) in
            print(error)
        })
```
[demo地址](https://github.com/gaoyuexit/GYInpurchase) 能点个star也是极好的, 打不打赏无所谓, 能帮到你就好
还有一种实践方式, 个人并不推荐, 因为太繁琐了:
思路: 购买成功后在本地将订单的用户, 凭证等信息存储到本地(UserDefaults, 数据库,keyChain等), 将凭证发送给自己服务器成功之后再移除此条交易记录, 每次打开app的时候, 在本地扫描是否有未完成的订单, 循环发送给自己的服务器进行二次验证
补充:
- 
关于上线:
错误做法: 上线审核的时候使用沙箱测试地址, 审核通过后, 手动发布上线, 上线后让服务器切换到苹果的正式测试地址
> 
说明: 这种做法第一次上架可以使用, 但是到第二次迭代审核的时候, 苹果测试员使用的是沙盒环境, 但是我们服务器是正式环境, 会导致报错误码: 21007
正确的做法: 判断苹果正式验证服务器的返回code，如果是21007 `表示环境不对`，则再一次连接测试服务器进行验证即可..
 (这一步骤即: 先判断苹果的环境, 根据苹果环境切换沙盒地址还是正式地址)
- 
关于苹果二次验证返回的参数:
服务端\客户端对苹果发送请求进行验证有时会返回多个交易记录
> 
说明: 苹果验证会返回: 一个未完成交易的数组(一般只有一个, 就是当前操作购买的这个), 如果有多个为完成的交易,就会返回多个 (这种情况一般是代码写的不对造成的), 服务端根据`transactionIdentifier`找到当前购买的交易或者取最后一个也是当前购买的交易来做判断和验证....经过测试发现如果在当前手机请求发现出现多个未完成的交易,
 则换另外一部手机和账号等, 仍然会返回那些未完成的交易, 看来每次对商品进行购买, 苹果会把所有未完成的交易都返回(不管这个商品是其他用户的还是其他手机的)
