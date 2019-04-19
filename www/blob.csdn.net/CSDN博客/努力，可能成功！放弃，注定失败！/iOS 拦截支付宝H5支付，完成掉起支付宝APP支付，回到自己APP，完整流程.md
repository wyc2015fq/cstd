# iOS 拦截支付宝H5支付，完成掉起支付宝APP支付，回到自己APP，完整流程 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2019年03月20日 10:51:32[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：182
先说一下这个想法的来源，然后有相关需求的朋友也可以这样来操作
公司产品用到了支付宝支付，但是避免上架审核问题（不能接入支付宝SDK，害怕检测），采用了支付宝的H5支付，作为产品的支付渠道；APP内部，采用了WKWebView作为主体来加载H5内容。
本来是没有什么问题的，但是每次看到支付宝H5支付页面的提示，就感觉有点别扭，而且支付的话，是在H5中完成支付，需要输入验证码、支付密码这些。如图：
[图一]
![](https://upload-images.jianshu.io/upload_images/1817180-03f9db93eefb541b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/828/format/webp)
Simulator Screen Shot - iPhone XR - 2018-11-09 at 13.50.52.png
![](https://upload-images.jianshu.io/upload_images/1817180-b123a59b331396e7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/828/format/webp)
Simulator Screen Shot - iPhone XR - 2018-11-09 at 13.54.01.png
看到“尝试打开支付宝客户端”，就猜到，这个应该是会去唤起支付宝APP完成支付，然后本着一个程序员的心态，开始了踩坑之路。
首先，回顾一下支付宝SDK的支付流程：
- 通过后台接口，获取支付串（个人这样称呼哈），然后交给支付宝SDK
```
[[AlipaySDK defaultService] payOrder:@"支付串" fromScheme:@"本APP的Scheme" callback:^(NSDictionary * resultDic) {
         //  回调流程
  }];
```
- 当执行上面代码以后，就看到能掉起支付宝APP了（自己设置好白名单和相关SDK初始化工作）；然后正常执行支付流程
- 完成支付以后，支付宝第一次会提示打开“***”APP，点击完成，即可回到自己的APP
- APP在Appdelegate的OpenURL方法里处理好相关操作后，即可在第一部的回调流程中，获取到支付结果，即可完成整个支付流程
```
- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation {
   if ([url.host isEqualToString:@"safepay"])
         [[AlipaySDK defaultService] processOrderWithPaymentResult:url       standbyCallback:^(NSDictionary *resultDic) {
                
          }];
   }
  return YES;
}
```
然后，我们回头看下这个支付流程，和H5流程对比，我们想要做到的，就是1.掉起支付宝APP；2.支付完成后，支付宝APP要返回到我们的APP里面来
关键点就在这个“URLScheme”里面，废话不多说，直接说我这边的处理方法
在WKWebView进如“图一”页面的时候，看下WKWebView的代理方法
- 1.代理方法如下面代码
```
- (void)webView:(WKWebView *)webView decidePolicyForNavigationAction:(WKNavigationAction *)navigationAction decisionHandler:(void (^)(WKNavigationActionPolicy))decisionHandler
    //  这里解释一下我这边要用到的东西
    /*
         H5既然能掉起APP，猜测只能是URLScheme和Universallink
         然后这个代理方法的“ navigationAction”，就我们要研究的东西
    */
      NSLog(@"urlScheme:%@",navigationAction.request.URL.scheme);
      NSLog(@"urlStr:%@",navigationAction.request.URL.absoluteString);
      decisionHandler(WKNavigationActionPolicyAllow);
}
```
- 看到控制台打印的东西，就找到一个熟悉的东西 alipay://alipayclient
```
/*
进入页面的时候可能会多次执行
*/  
控制台打印了一个 
“ urlScheme:alipay”
和
“alipay://alipayclient/?%7B%22requestType%22%3A%22SafePay%22%2C%22fromAppUrlScheme%22%3A%22alipays%22%2C%22dataString%22%3A%22h5_route_token%3D%5C%22RZ32UMjYcMJ3b4fVFd5P6OLqPRiX9mmobilecashierRZ32%5C%22%26is_h5_route%3D%5C%22true%5C%22%22%7D”
```
- 机智的发现，这个其实就是支付宝APP的URLScheme地址，那么后面这一堆是啥呢？机智的朋友其实已经发现，这个是一个URLEncode后的String，所以，我们来解码一下看看
```
//  OC URLDecode方法
- (NSString*)URLDecodedString:(NSString*)str {
    NSString *decodedString=(__bridge_transfer NSString *)CFURLCreateStringByReplacingPercentEscapesUsingEncoding(NULL, (__bridge CFStringRef)str, CFSTR(""), CFStringConvertNSStringEncodingToEncoding(NSUTF8StringEncoding));
    
    return decodedString;
}
```
- 
解码后的字符串是
alipay://alipayclient/?{"requestType":"SafePay","fromAppUrlScheme":"alipays","dataString":"h5_route_token="RZ32UMjYcMJ3b4fVFd5P6OLqPRiX9mmobilecashierRZ32"&is_h5_route="true""}
- 
然后就看到一个很可爱的字符“ fromAppUrlScheme”，这个其实就是我们要修改的东西，也就支付完成后，他默认要跳转的UrlScheme地址，默认他是“ alipays”（已经成功了一半，继续往下看）
- 
接下来，我们要修改成我们自己的，我写一个我这边比较偷懒的方法，大家可以自行操作了哈
```
- (void)webView:(WKWebView *)webView decidePolicyForNavigationAction:(WKNavigationAction *)navigationAction decisionHandler:(void (^)(WKNavigationActionPolicy))decisionHandler
    
  //  先判断一下，找到需要跳转的再做处理
  if ([navigationAction.request.URL.scheme isEqualToString:@"alipay"]) {
        //  1.以？号来切割字符串
        NSArray * urlBaseArr = [navigationAction.request.URL.absoluteString componentsSeparatedByString:@"?"];
        NSString * urlBaseStr = urlBaseArr.firstObject;
        NSString * urlNeedDecode = urlBaseArr.lastObject;
        //  2.将截取以后的Str，做一下URLDecode，方便我们处理数据
        NSMutableString * afterDecodeStr = [NSMutableString stringWithString:[self URLDecodedString:urlNeedDecode]];
        //  3.替换里面的默认Scheme为自己的Scheme
        NSString * afterHandleStr = [jjj stringByReplacingOccurrencesOfString:@"alipays" withString:@"自己APP的Scheme"];
        //  4.然后把处理后的，和最开始切割的做下拼接，就得到了最终的字符串
        NSString * finalStr = [NSString stringWithFormat:@"%@?%@",urlBaseStr, [self  URLEncodeString:kkk]];
        
        
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.5 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
            //  判断一下，是否安装了支付宝APP（也就是看看能不能打开这个URL）
            if ([[UIApplication sharedApplication] canOpenURL:[NSURL URLWithString:finalStr]]) {
                [[UIApplication sharedApplication] openURL:[NSURL URLWithString:finalStr]];
            }
        });
        
        //  2.这里告诉页面不走了 -_-
        decisionHandler(WKNavigationActionPolicyCancel);
    }
    decisionHandler(WKNavigationActionPolicyAllow);
}
//  OC 做URLEncode的方法
-  (NSString *)URLEncodeString:(NSString*)str {
    NSString *unencodedString = str;
    
    NSString *encodedString = (NSString *)
    
    CFBridgingRelease(CFURLCreateStringByAddingPercentEscapes(kCFAllocatorDefault,
                                                              
                                                              (CFStringRef)unencodedString,
                                                              
                                                              NULL,
                                                              
                                                              (CFStringRef)@"!*'();:@&=+$,/?%#[]",
                                                              
                                                              kCFStringEncodingUTF8));
    
    return encodedString;
}
- (NSString*)URLDecodedString:(NSString*)str {
    NSString *decodedString=(__bridge_transfer NSString *)CFURLCreateStringByReplacingPercentEscapesUsingEncoding(NULL, (__bridge CFStringRef)str, CFSTR(""), CFStringConvertNSStringEncodingToEncoding(NSUTF8StringEncoding));
    
    return decodedString;
}
```
- 然后，就会发现，APP可以正常的唤起支付宝SDK支付了，并且，支付完成后，可以正常回到自己的APP
其实这里正常来说就已经实现了我们的需求了，当唤起支付宝APP支付后，当前的WKWebView其实也在等待支付结果，因为当你从支付宝APP返回到你自己的APP后，WKWebView的页面流程也正常执行了（页面会跳转到事先设置的一个returnUrl）
这里猜测，应该是当前的页面做了长链接（Scoket），当支付成功后，scoket检测到了支付结果，然后自动跳转了，如果我们需要自己处理支付结果应该怎么做呢？如果有这方面需求的朋友，可以继续看
- 回到Appdelegate的代理方法
```
//  不同的iOS版本，可能方法不同，不过大概能知道是哪个方法哈
-(BOOL)application:(UIApplication *)app openURL:(NSURL *)url options:(NSDictionary<NSString *,id> *)options{ 
  
}
```
- 当支付完成，返回APP的时候，Appdelegate的这个方法将会收到数据
```
-(BOOL)application:(UIApplication *)app openURL:(NSURL *)url options:(NSDictionary<NSString *,id> *)options{ 
  
  //  判断一下这个host，safepay就是支付宝的
  if ([url.host isEqualToString:@"safepay"]) {
        //  拿到这个链接的数据，数据大概是这样的
        /*
        mushRoomBox://safepay/?%7B%22memo%22:%7B%22result%22:%22%7B%5C%22alipay_trade_wap_pay_response%5C%22:%7B%5C%22code%5C%22:%5C%2210000%5C%22,%5C%22msg%5C%22:%5C%22Success%5C%22,%5C%22app_id%5C%22:%5C%222017020505522491%5C%22,%5C%22auth_app_id%5C%22:%5C%222017020505522491%5C%22,%5C%22charset%5C%22:%5C%22GBK%5C%22,%5C%22timestamp%5C%22:%5C%222018-11-09%2011:43:06%5C%22,%5C%22out_trade_no%5C%22:%5C%222018100001_simple_48f033a4a00ba19a0526ddbd4f96b036%5C%22,%5C%22total_amount%5C%22:%5C%220.01%5C%22,%5C%22trade_no%5C%22:%5C%222018110922001438371006963978%5C%22,%5C%22seller_id%5C%22:%5C%222088221175674834%5C%22%7D,%5C%22sign%5C%22:%5C%22T6jn7QSsvjjG%5C/HxknSl2lOdHj6zf7jebWEKkeGhEJaXCYFbRHMTsml7TNRhRACupmwDYUADQOynKNR%5C/QBXHgBxJRhYEE9SXVz4mp7sqSGPItzV4bVmZw5AG5UJzsuIh9O6QC%5C/QHc+FcA2i1zw46VDMUnbCCKat5q4ME9d3AUSMhYg31xO+1TZnraDfm0I%5C/OwAnA5iCTOhq%5C/fkm%5C/r4hsujdv9fTvmszgIJLMhN7yQhoGaOWs0vIQ6hoJoXmCXVapfY7ANsoKaR6hGJogH4GKGQi0vLk17W8s+LjcOUT%5C/zB6K6gN+fyCefm8FOJ+8g4GUAreuZDcW0MuGE1OOmaxDrXg==%5C%22,%5C%22sign_type%5C%22:%5C%22RSA2%5C%22%7D%22,%22ResultStatus%22:%229000%22,%22memo%22:%22%22%7D,%22requestType%22:%22safepay%22%7D
        */
        //  然后，一样的套路方法
        NSString * urlNeedJsonStr = url.absoluteString;
        NSArray * afterComStr = [urlNeedJsonStr componentsSeparatedByString:@"?"];
        //  这个decode方法，在上面找哈
        NSString * lastStr = [self URLDecodedString:kkstr.lastObject];
        //  这个lastStr，其实是一个jsonStr，转一下，就看到了数据
        NSDictionary * dict = [self  dictionaryWithJsonString:lastStr];
        //  dict的结构差不多是这样
        /*
          "memo": {
                  "result":"订单相关信息，如订单号，支付金额等等";
                   "ResultStatus":"9000";
           },
          ******
        */
        
        //  和支付宝SDK的返回结果一次，这个ResultStatus，就是我们要的数据
        //  9000 ：支付成功
        //  8000 ：订单处理中
        //  4000 ：订单支付失败
        //  6001 ：用户中途取消
        //  6002 ：网络连接出错
        //  这里的话，就可以根据状态，去处理自己的业务了
    }
    return YES;
}
-  (NSDictionary *)dictionaryWithJsonString:(NSString *)jsonString
{
    if (jsonString == nil) {
        return nil;
    }
    
    NSData *jsonData = [jsonString dataUsingEncoding:NSUTF8StringEncoding];
    NSError *err;
    NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:jsonData
                                                        options:NSJSONReadingMutableContainers
                                                          error:&err];
    if(err)
    {
        NSLog(@"json解析失败：%@",err);
        return nil;
    }
    return dic;
}
```
- 处理方法，就贴到代码里面了，需要的可以看下
然后，需求和其他可能需求，也就处理完成了，如果这些方法有帮到你的话，点个赞吧
作者：JiYis
链接：https://www.jianshu.com/p/d0d43e3d3446
来源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
