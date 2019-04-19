# iOS开发实用技术_Touch ID - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年05月04日 09:20:50[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：430
TouchID 开发基础知识
支持系统和机型:最低机型iPhone 5s,最低系统iOS8.0
依赖框架: #import <LocalAuthentication/LocalAuthentication.h>
注意事项:做iOS8.0以下版本适配时,需要进行API验证
备注: 模拟器->Hardware->Touch ID Enrolled
#### 代码演示
![](http://upload-images.jianshu.io/upload_images/1956558-89985460a87d58c9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Simulator Screen Shot 2016年5月2日 下午8.55.17.png
### ViewController.h
#import <UIKit/UIKit.h>
@interface ViewController : UIViewController
@end
### ViewController.m
#import "ViewController.h"
#import <LocalAuthentication/LocalAuthentication.h>
@interface ViewController ()
@end
@implementation ViewController
- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor whiteColor];
}
- (IBAction)use_TouchID:(id)sender {
        // 判断系统
    if (!([UIDevice currentDevice].systemVersion.doubleValue >= 8.0)) {
        return;
    }
        // 调用Touch ID
        [self userTouchID];
}
- (void)userTouchID{
    // 创建指纹验证对象
    LAContext  *context = [LAContext new];
    NSError *errorLA = nil;
    if ([context canEvaluatePolicy:LAPolicyDeviceOwnerAuthenticationWithBiometrics error:&errorLA])
    {
        // 弹出验证
        [context evaluatePolicy:LAPolicyDeviceOwnerAuthenticationWithBiometrics localizedReason:@"请验证已有指纹" reply:^(BOOL success, NSError * _Nullable error) {
            if (success) {
                // 验证成功
                // 进行操作
            } else {
                NSLog(@"%@", error.localizedDescription);
                switch (error.code) {
                    case LAErrorSystemCancel:
                        NSLog(@"切换到其他APP, 系统取消验证TouchID");
                        break;
                    case LAErrorUserCancel:
                        NSLog(@"用户取消验证TouchID");
                        break;
                    case LAErrorUserFallback:
                        NSLog(@"用户选择输入密码,切换到主线程进行处理");
                        [[NSOperationQueue mainQueue] addOperationWithBlock:^{
                        }];
                        break;
                    default:
                        [[NSOperationQueue mainQueue] addOperationWithBlock:^{
                            // 其他情况,切换到主线程进行处理
                        }];
                        break;
                }
            }
        }];
        
    } else {
        switch (errorLA.code) {
            case LAErrorTouchIDNotEnrolled:
                NSLog(@"LAErrorTouchIDNotEnrolled");
                break;
            case LAErrorPasscodeNotSet:
                NSLog(@"LAErrorPasscodeNotSet");
                break;
            default:
                NSLog(@"Touch ID is unaviliable");
                break;
        }
    }
}
// API中的相关枚举解释
/*
 typedef NS_ENUM(NSInteger, LAError)
 {
 // 授权失败
 LAErrorAuthenticationFailed = kLAErrorAuthenticationFailed,
 // 用户取消Touch ID授权
 LAErrorUserCancel           = kLAErrorUserCancel,
 // 用户选择输入密码
 LAErrorUserFallback         = kLAErrorUserFallback,
 // 系统取消授权
 LAErrorSystemCancel         = kLAErrorSystemCancel,
 // 系统未设置密码
 LAErrorPasscodeNotSet       = kLAErrorPasscodeNotSet,
 // 设备Touch ID不可用,如未打开
 LAErrorTouchIDNotAvailable  = kLAErrorTouchIDNotAvailable,
 // 设备Touch ID不可用,如用户未录入
 LAErrorTouchIDNotEnrolled   = kLAErrorTouchIDNotEnrolled,
 } NS_ENUM_AVAILABLE(10_10, 8_0);
*/
@end
文／Lawliet_（简书作者）
原文链接：http://www.jianshu.com/p/472b219989b5
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
