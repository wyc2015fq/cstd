# ios自定义AlertView - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年05月06日 10:23:10[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：388
## 先上图,弹框的背景色,按钮背景色,提示的消息的字体颜色都可以改变
![](http://upload-images.jianshu.io/upload_images/1412973-7585718936d7b289.gif?imageMogr2/auto-orient/strip)
- 利用单例实现丰富的自定义接口
```
//
//  PBAlertController.h
//  PBAlertDemo
//
//  Created by 裴波波 on 16/4/20.
//  Copyright © 2016年 裴波波. All rights reserved.
//
#import <UIKit/UIKit.h>
typedef void(^PBBlock)();
@interface PBAlertController : UIViewController
/** 设置alertView背景色 */
@property (nonatomic, copy) UIColor *alertBackgroundColor;
/** 设置确定按钮背景色 */
@property (nonatomic, copy) UIColor *btnConfirmBackgroundColor;
/** 设置取消按钮背景色 */
@property (nonatomic, copy) UIColor *btnCancelBackgroundColor;
/** 设置message字体颜色 */
@property (nonatomic, copy) UIColor *messageColor;
/** 创建单例 */
+(instancetype)shareAlertController;
/** 弹出alertView以及点击确定回调的block */
-(void)alertViewControllerWithMessage:(NSString *)message andBlock:(PBBlock) block;
@end
```
- .m文件中初始化控件以及对alertView的控件的属性进行懒加载,确定初始的颜色.
```
//
//  PBAlertController.m
//  PBAlertDemo
//
//  Created by 裴波波 on 16/4/20.
//  Copyright © 2016年 裴波波. All rights reserved.
//
#import "PBAlertController.h"
/** 屏幕尺寸 */
#define kMainScreenBounds [UIScreen mainScreen].bounds
@interface PBAlertController ()
/** 蒙版 */
@property (nonatomic, strong) UIView *coverView;
/** 弹框 */
@property (nonatomic, strong) UIView *alertView;
/** 点击确定回调的block */
@property (nonatomic, copy) PBBlock block;
@end
@implementation PBAlertController
- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor whiteColor];
}
-(void)alertViewControllerWithMessage:(NSString *)message andBlock:(PBBlock) block{
    self.block = block;
    //创建蒙版
    UIView * coverView = [[UIView alloc] initWithFrame:kMainScreenBounds];
    self.coverView = coverView;
    [self.view addSubview:coverView];
    coverView.backgroundColor = [UIColor blackColor];
    coverView.alpha = 0.7;
    //创建提示框view
    UIView * alertView = [[UIView alloc] init];
    alertView.backgroundColor = self.alertBackgroundColor;
    //设置圆角半径
    alertView.layer.cornerRadius = 6.0;
    self.alertView = alertView;
    [self.view addSubview:alertView];
    alertView.center = coverView.center;
    alertView.bounds = CGRectMake(0, 0, kMainScreenBounds.size.width * 0.75, kMainScreenBounds.size.width * 0.75 * 1.5/ 3);
    //创建操作提示 label
    UILabel * label = [[UILabel alloc] init];
    [alertView addSubview:label];
    label.text = @"操作提示";
    label.font = [UIFont systemFontOfSize:19];
    label.textAlignment = NSTextAlignmentCenter;
    CGFloat lblWidth = alertView.bounds.size.width;
    CGFloat lblHigth = 22;
    label.frame = CGRectMake(0, 0, lblWidth, lblHigth);
    //创建中间灰色分割线
    UIView * separateLine = [[UIView alloc] init];
    separateLine.backgroundColor = [UIColor grayColor];
    [alertView addSubview:separateLine];
    separateLine.frame = CGRectMake(0, lblHigth + 1, alertView.bounds.size.width, 1);
    //创建message label
    UILabel * lblMessage = [[UILabel alloc] init];
    lblMessage.textColor = self.messageColor;
    [alertView addSubview:lblMessage];
    lblMessage.text = message;
    lblMessage.textAlignment = NSTextAlignmentCenter;
    lblMessage.numberOfLines = 2; //最多显示两行Message
    CGFloat margin = 5;
    CGFloat msgX = margin;
    CGFloat msgY = lblHigth + 3;
    CGFloat msgW = alertView.bounds.size.width - 2 * margin;
    CGFloat msgH = 44;
    lblMessage.frame = CGRectMake(msgX, msgY, msgW, msgH);
    //创建确定 取消按钮
    CGFloat buttonWidth = (alertView.bounds.size.width - 4 * margin) * 0.5;
    CGFloat buttonHigth = 25;
    UIButton * btnCancel = [[UIButton alloc] init];
    [alertView addSubview:btnCancel];
    [btnCancel setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [btnCancel setTitle:@"取消" forState:UIControlStateNormal];
    [btnCancel setBackgroundColor:self.btnCancelBackgroundColor];
    btnCancel.frame = CGRectMake(margin, alertView.bounds.size.height - margin - buttonHigth, buttonWidth, buttonHigth);
    btnCancel.tag = 0;
    [btnCancel addTarget:self action:@selector(didClickBtnConfirm:) forControlEvents:UIControlEventTouchUpInside];
    //确定按钮
    UIButton * btnConfirm = [[UIButton alloc] init];
    btnConfirm.tag = 1;
    [alertView addSubview:btnConfirm];
    [btnConfirm setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [btnConfirm setTitle:@"确定" forState:UIControlStateNormal];
    [btnConfirm setBackgroundColor:self.btnConfirmBackgroundColor];
    btnConfirm.frame = CGRectMake(alertView.bounds.size.width - margin - buttonWidth, alertView.bounds.size.height - margin - buttonHigth, buttonWidth, buttonHigth);
    [btnConfirm addTarget:self action:@selector(didClickBtnConfirm:) forControlEvents:UIControlEventTouchUpInside];
}
/** 点击确定 or 取消触发事件 */
-(void)didClickBtnConfirm:(UIButton *)sender{
    if (sender.tag == 0) {
        [self dismissViewControllerAnimated:YES completion:nil];
        return;
    }
    self.block();
    [self dismissViewControllerAnimated:YES completion:nil];
}
- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
}
static PBAlertController * instance = nil;
+(instancetype)shareAlertController{
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [[PBAlertController alloc] init];
    });
    return instance;
}
-(UIColor *)alertBackgroundColor{
    if (_alertBackgroundColor == nil) {
        _alertBackgroundColor = [UIColor colorWithRed:249/255.0 green:249/255.0 blue:249/255.0 alpha:1];
    }
    return _alertBackgroundColor;
}
/** 确定按钮背景色 */
-(UIColor *)btnConfirmBackgroundColor{
    if (_btnConfirmBackgroundColor == nil) {
        _btnConfirmBackgroundColor = [UIColor orangeColor];
    }
    return _btnConfirmBackgroundColor;
}
/** 取消按钮背景色 */
-(UIColor *)btnCancelBackgroundColor{
    if (_btnCancelBackgroundColor == nil) {
        _btnCancelBackgroundColor = [UIColor blueColor];
    }
    return _btnCancelBackgroundColor;
}
/** message字体颜色 */
-(UIColor *)messageColor{
    if (_messageColor == nil) {
        _messageColor = [UIColor blackColor];
    }
   return _messageColor;
}
@end
```
- 在需要调用的地方进行调用
```
//
//  ViewController.m
//  PBAlertDemo
//
//  Created by 裴波波 on 16/4/20.
//  Copyright © 2016年 裴波波. All rights reserved.
//
#import "ViewController.h"
#import "PBAlertController.h"
@interface ViewController ()
@end
@implementation ViewController
//点击按钮弹出提示框
- (IBAction)clickShowAlertBtn:(id)sender {
    PBAlertController * alertVc = [PBAlertController shareAlertController];
    alertVc.messageColor = [UIColor redColor];
    [alertVc alertViewControllerWithMessage:@"这是一message沙哈" andBlock:^{
        NSLog(@"点击确定后执行的方法");
    }];
    alertVc.modalTransitionStyle = UIModalTransitionStyleCrossDissolve;
    [self presentModalViewController:alertVc animated:YES];
}
@end
```
- 源代码demo下载地址,以及使用方法
[https://git.oschina.net/alexpei/AlertViewController.git](https://git.oschina.net/alexpei/AlertViewController.git)
文／AlexPei（简书作者）
原文链接：http://www.jianshu.com/p/683ddedded9d
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
