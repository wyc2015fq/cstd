# iOS 指纹识别 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年11月23日 09:12:39[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：2441
# 简介
苹果从iPhone5S开始，具有指纹识别技术，从iOS8.0之后苹果允许第三方 App 使用 Touch ID进行身份验证。指纹识别Touch ID提供3+2共5次指纹识别机会（3次识别失败后，弹出的指纹验证框会消失，同时会报错code = -1，然后点击指纹会再次弹框可验证两次），如果五次指纹识别全部错误，就需要手动输入数字密码，数字密码可以输入6次，如果6次输入的数字密码都错误，系统会停止验证，一定的间隔后才能再次输入密码验证，而且间隔会随着输入的次数增长。
# 使用方法
1.首先导入框架LocalAuthentication
2.判断系统版本，最低iOS 8.0
3.创建验证对象上下文LAContext
4.判断指纹识别技术是否可用canEvaluatePolicy
5.如果可用，开始调用方法开始使用指纹识别
`#import <LocalAuthentication/LocalAuthentication.h>`
```
//指纹按钮
- (void)showFingerprintTouch
{
    //系统支持，最低iOS 8.0
    if ([UIDevice currentDevice].systemVersion.floatValue >= 8.0)
    {
        LAContext * context = [[LAContext alloc] init];
        NSError * error;
        if ([context canEvaluatePolicy:LAPolicyDeviceOwnerAuthenticationWithBiometrics error:&error])
        {
            //localizedReason: 指纹识别出现时的提示文字
            [context evaluatePolicy:LAPolicyDeviceOwnerAuthenticationWithBiometrics localizedReason:@"指纹解锁" reply:^(BOOL success, NSError * _Nullable error) {
                if (success)
                {
                    //识别成功
                    dispatch_async(dispatch_get_main_queue(), ^{
                            //在主线程中，处理 ......
                    });
                }
                else if (error)
                {
                    NSLog(@"LAPolicyDeviceOwnerAuthenticationWithBiometrics -- %@",error);
                }
            }];
        }
        else if([context canEvaluatePolicy:LAPolicyDeviceOwnerAuthentication error:nil])
        {
            [context evaluatePolicy:LAPolicyDeviceOwnerAuthentication localizedReason:@"密码解锁" reply:^(BOOL success, NSError * _Nullable error){
                NSLog(@"LAPolicyDeviceOwnerAuthentication -- %@", error);
            }];
        }
        NSLog(@" --- %@ ", error);
    }
}
```
# 代码解析
##### 指纹识别的策略
`[context canEvaluatePolicy:LAPolicyDeviceOwnerAuthenticationWithBiometrics error:nil][context canEvaluatePolicy:LAPolicyDeviceOwnerAuthentication error:nil]`
![](http://upload-images.jianshu.io/upload_images/288548-374a476fded738b0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
- LAPolicyDeviceOwnerAuthenticationWithBiometrics ，指纹授权使用， 当设备不具有Touch ID的功能，或者在系统设置中没有设置开启指纹，授权将会失败。当指纹验证3+2次都没有通过的时候指纹验证就会被锁定，就需要先进行数字密码的解锁才能继续使用指纹密码。
- LAPolicyDeviceOwnerAuthentication，指纹和数字密码的授权使用，当指纹可用且没有被锁定，授权后会进入指纹密码验证。不然的话会进入数字密码验证的页面。当系统数字密码没有设置不可用的时候，授权失败。如果数字密码输入不真确，连续6次输入数字密码都不真确后，会停用鉴定过一定的间隔后才能使用，间隔时间依次增长，如图：
![](http://upload-images.jianshu.io/upload_images/288548-a8bc62e6d130fc1a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
- 弹出的指纹验证框的两个按钮的标题，可以通过下面的方法修改：
`context.localizedCancelTitle = @"取消";context.localizedFallbackTitle = @"输入密码";`
- localizedReason:是用来设置弹出框的提示内容的，一般写的是是用原因，如图：
![](http://upload-images.jianshu.io/upload_images/288548-7f4b5c3db69080ee.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/288548-437981f5060a58a7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
##### 错误情况
官方文档中给出的错误情况，有以下几种，如图：
![](http://upload-images.jianshu.io/upload_images/288548-f4de21e92dbbff7c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/288548-1836fd4aef54d5bb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
简单介绍几种常出现的错误
> 
3次验证失败后报错
Error Domain=com.apple.LocalAuthentication Code=-1 "Application retry limit exceeded." UserInfo={NSLocalizedDescription=Application retry limit exceeded
> 
用户取消
Error Domain=com.apple.LocalAuthentication Code=-2
> 
用户在弹出的指纹验证框中，点击输入密码
Error Domain=com.apple.LocalAuthentication Code=-3 "Fallback authentication mechanism selected."
可以判断这种错误 error.code == -3，进行后面的操作
> 
设备没有设置指纹报错
Error Domain=com.apple.LocalAuthentication Code=-7 "No fingers are enrolled with Touch ID."
> 
3+2次指纹验证都失败后报错，这种情况指纹解锁会被锁定，输入数字密码真确后才能再次进行指纹验证
Error Domain=com.apple.LocalAuthentication Code=-8 "Biometry is locked out."
# 注意事项
- 
识别成功后在主线程上处理
识别成功后，不是在主线程上，如果不切换到主线程上，后面的不会及时处理。
- 
如果不写后面canEvaluatePolicy:LAPolicyDeviceOwnerAuthentication判断，在iOS 10 上会遇到一个问题就是，在你验证了3+2次指纹都没成功之后，系统不会弹出输入密码的验证页面，点击指纹验证按钮也没有反应。但是在iOS9中5次验证失败之后点击再次验证，系统会弹出输入数字密码的页面。
# 最后
附上带有指纹、手势和数字密码解锁的例子。传送门 [GitHub](https://github.com/MA806P/PasscodeLock)
文／MA806P（简书作者）
原文链接：http://www.jianshu.com/p/aef5a506311b
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
