# iOS 应用内评分 - weixin_33985507的博客 - CSDN博客
2018年04月10日 17:02:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
支持 iOS 版本：iOS10.3 以上
限制条件：
1.每年最多使用 3 次弹窗来请求用户评分。
2.只能评星，不能写评论。
3.只能调用方法，无法判断是否响应，无法获取评分信息（完全是 App Store 管理）
4.开发模式时调用此方法，可以一直弹出评分视图，但提交是灰色不可点击；使用 TestFlight 分发的应用程序中（内测）调用此方法时，此方法无效；
使用：
引入头文件：#import <StoreKit/StoreKit.h> （StoreKit 框架）
```
if([SKStoreReviewController respondsToSelector:@selector(requestReview)]) {
    // iOS 10.3 以后
    [SKStoreReviewController requestReview];
} else {
    // iOS 10.3 之前
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:APPSTORE_URL]];
}
```
注意事项：
因为无法知道用户评几分，为了获得高的评分，对流程需要做一下优化。
比如：先询问用户是否对应用满意，如果满意则去评分，如果不满意则跳转到意见反馈页面，避免了打低分。
另外如果用户点了满意，调用评分，也不能确定是否会弹框（可能是离上次弹框时间比较短，苹果没有说明是否会有间隔时间（没有找到有关信息）；也可能用户已经弹过三次了）。
可以在下个版本加上，暂时不对用户开放，用来测试（比如在意见反馈输入框输入“评分”，弹出评分，看触发几率、间隔时间）
经测试：评分弹框出现前1秒会有卡住的感觉
在合适的时机调用：
对于我们的应用，在使用优惠券投资成功，提现，回款等时机对用户对咱们评分比较有利。另外对比较活跃的用户（一天登录10几次，每次停留时间较长）
直接跳到 AppStore app 评论页面：
对产品 Appstore 的 URL 添加参数 action=write-review ，可以自动打开 App 可以在 App Store 中撰写评论的页面
评论回复：
在 iOS 10.3 后，开发者回复可以 App Store 评论。
在 App 的开发者管理后台 iTunes Connect 中，开发者在我的 App —— 活动 —— 评分与评论里能看到用户给 App 留下的评论，并可以选择某一条评论进行回复。
