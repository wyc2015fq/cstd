# iOS开发 解决同时使用友盟三方登录 和 微信支付 不走回调的问题 - weixin_33985507的博客 - CSDN博客
2017年11月22日 12:40:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
在项目中有可能会使用到友盟的三方登录 和 微信支付
微信支付 会出现不走回调的问题 下面说一下如何解决
//url Schemes回调
- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url
{
return [self handleUrl:url];
}
- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation
{
return [self handleUrl:url];
}
- (BOOL)handleUrl:(NSURL *)url
{
//判断如果是支付的话 不走友盟方法 直接走微信和QQ的方法
if([[url absoluteString] rangeOfString:@"这里是你的微信key://pay"].location == 0)
return [WXApi handleOpenURL:url delegate:self],[TencentOAuth HandleOpenURL:url];
else
return [[UMSocialManager defaultManager] handleOpenURL:url];
}
以上就是iOS开发 解决同时使用友盟三方登录 和 微信支付 不走回调的问题的全文介绍,希望对您学习和使用ios应用开发有所帮助.
