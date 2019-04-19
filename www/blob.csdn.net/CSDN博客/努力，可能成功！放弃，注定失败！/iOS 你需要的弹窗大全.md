# iOS 你需要的弹窗大全 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年09月29日 09:23:08[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：185
在我们的实际开发项目中，弹窗是必不可少的，很多时候我们用的是系统的AlertViewController，但是实际情况中，并不能满足我们的开发需求，这个时候我们需要的就是自定义自己的弹窗效果。接下来我会写一些自己的所封装的弹窗效果。包括代理delegate回调，block 回调，xib新建view来创建我们需要的弹窗效果。
### 官方思路
1.在我们自己动手之前一定要先看看官方是怎么封装的，这样我们写出来的代码才接近苹果语言，看起来高大上。好的代码一定是见名知意的，别人一看这个方法就知道大概我们通过这个方法可以得到什么样的效果。
```
// ios8.0 之后
    UIAlertController *alertController = [UIAlertController alertControllerWithTitle:@"提示" message:@"message" preferredStyle:UIAlertControllerStyleAlert];
    
    UIAlertAction *cancelAction = [UIAlertAction actionWithTitle:@"取消" style:UIAlertActionStyleCancel handler:nil];
    UIAlertAction *okAction = [UIAlertAction actionWithTitle:@"确定" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        NSLog(@"确定");
    }];
    
    [alertController addAction:cancelAction];
    [alertController addAction:okAction];
    [self presentViewController:alertController animated:YES completion:nil];
// ios8.0 之前
UIAlertView * alertView = [[UIAlertView alloc] initWithTitle:@"Tittle" message:@"This is message" delegate:self 
cancelButtonTitle:@"cancel" otherButtonTitles:nil, nil];
[alertView show];
```
因为在代码量风格上，我还是比较喜欢老版本的弹窗，毕竟代码上啊，一句话调用美滋滋。所以接下来我们封装也是模仿官方开始.....
# delegate
我们可以看到在苹果官方中，我们需要通过识别用户点击某个按钮来确定需要进一步的操作事件，这个时候是通过代理来实现的。代理的话，我们在熟悉不过了。
- 首先申明协议
```
#pragma mark - 协议
@class HLAlertView;
@protocol HLAlertViewDelegate<NSObject>
- (void)alertViewDidClickButtonWithIndex:(NSInteger)index;
@end
```
- 在viewController中遵循代理，设置代理 ， 实现方法即可
```
<HLAlertViewDelegate>
self.delegate = self;
#pragma mark --- HLAlertViewDelegate
-(void)alertViewDidClickButtonWithIndex:(NSInteger)index{
    if (index == AlertSureButtonClick) {
        [self alertSureButtonClick];
    }else{
        [self alertCauseButtonClick];
    }
}
```
- 接下来就是实现我们封装类的.h文件方法申明，以及.m的实现方法
```
//.h 文件
#import <UIKit/UIKit.h>
typedef enum : NSUInteger {
    AlertCauseButtonClick = 0,
    AlertSureButtonClick
} AlertButtonClickIndex;
#pragma mark - 协议
@class HLAlertView;
@protocol HLAlertViewDelegate<NSObject>
- (void)alertViewDidClickButtonWithIndex:(NSInteger)index;
@end
@interface HLAlertView : UIView
@property(nonatomic, weak) id <HLAlertViewDelegate> delegate;
- (instancetype)initWithTittle:(NSString *)tittle message:(NSString *)message sureButton:(NSString *)sureBtn;
- (void)show;
@end
```
```
@interface HLAlertView()
/** 弹窗主内容view */
@property (nonatomic,strong) UIView   *contentView;
/** 弹窗标题 */
@property (nonatomic,copy)   NSString *title;
/** message */
@property (nonatomic,copy)   NSString *message;
/** 确认按钮 */
@property (nonatomic,copy)   UIButton *sureButton;
@end
@implementation HLAlertView
- (instancetype)initWithTittle:(NSString *)tittle message:(NSString *)message sureButton:(NSString *)sureBtn{
    
    if (self = [super init]) {
        self.title = tittle;
        self.message = message;
        
        [self sutUpView];
    }
    return self;
}
- (void)sutUpView{
    self.frame = [UIScreen mainScreen].bounds;
    self.backgroundColor = [UIColor colorWithWhite:0.5 alpha:0.85];
    [UIView animateWithDuration:0.5 animations:^{
        self.alpha = 1;
    }];
    
    //------- 弹窗主内容 -------//
    self.contentView = [[UIView alloc]init];
    self.contentView.frame = CGRectMake(0, 0, SCREEN_WIDTH - 80, 150);
    self.contentView.center = self.center;
    self.contentView.backgroundColor = [UIColor whiteColor];
    self.contentView.layer.cornerRadius = 6;
    [self addSubview:self.contentView];
    
    // 标题
    UILabel *titleLabel = [[UILabel alloc]initWithFrame:CGRectMake(0, 10, self.contentView.width, 22)];
    titleLabel.font = [UIFont boldSystemFontOfSize:20];
    titleLabel.textAlignment = NSTextAlignmentCenter;
    titleLabel.text = self.title;
    [self.contentView addSubview:titleLabel];
    
    // message
    UILabel *messageLable = [[UILabel alloc]initWithFrame:CGRectMake(0, 50, self.contentView.width, 22)];
    messageLable.font = [UIFont boldSystemFontOfSize:17];
    messageLable.textAlignment = NSTextAlignmentCenter;
    messageLable.text = self.message;
    [self.contentView addSubview:messageLable];
    
    
    // 取消按钮
    UIButton * causeBtn = [UIButton buttonWithType:UIButtonTypeCustom];
    causeBtn.frame = CGRectMake(0, self.contentView.height - 40, self.contentView.width/2, 40);
    causeBtn.backgroundColor = [UIColor grayColor];
    [causeBtn setTitle:@"取消" forState:UIControlStateNormal];
    [causeBtn addTarget:self action:@selector(causeBtn:) forControlEvents:UIControlEventTouchUpInside];
    [self.contentView addSubview:causeBtn];
    
    // 确认按钮
    UIButton * sureButton = [UIButton buttonWithType:UIButtonTypeCustom];
    sureButton.frame = CGRectMake(causeBtn.width, causeBtn.y, causeBtn.width, 40);
    sureButton.backgroundColor = [UIColor redColor];
    [sureButton setTitle:@"确定" forState:UIControlStateNormal];
    [sureButton addTarget:self action:@selector(processSure:) forControlEvents:UIControlEventTouchUpInside];
    
    [self.contentView addSubview:sureButton];
    
}
- (void)show{
    UIWindow *keyWindow = [UIApplication sharedApplication].keyWindow;
    [keyWindow addSubview:self];
}
- (void)processSure:(UIButton *)sender{
    if ([self.delegate respondsToSelector:@selector(alertViewDidClickButtonWithIndex:)]) {
        [self.delegate alertViewDidClickButtonWithIndex:AlertSureButtonClick];
    }
    [self dismiss];
}
- (void)causeBtn:(UIButton *)sender{
    if ([self.delegate respondsToSelector:@selector(alertViewDidClickButtonWithIndex:)]) {
        [self.delegate alertViewDidClickButtonWithIndex:AlertCauseButtonClick];
    }
    [self dismiss];
}
#pragma mark - 移除此弹窗
/** 移除此弹窗 */
- (void)dismiss{
    [self removeFromSuperview];
}
```
通过代理的方式我们就完成了我们自己页面的封装了。
## block弹窗
先看一下封装之后我们的调用方式吧：
```
HLAlertViewBlock * alertView = [[HLAlertViewBlock alloc] initWithTittle:@"提示" message:@"通过Block弹窗回调的弹窗" block:^(NSInteger index) {
        if (index == AlertSureButtonClick) {
            [self alertSureButtonClick];
        }else{
            [self alertCauseButtonClick];
        }
    }];
    [alertView show];
```
相比代理的方式的话，我们还行喜欢这种block回调的，简大气接地气啊。当然在我们需要处理逻辑多的时候，还是代理会比较好一点，具体环境下具体使用。
封装成block的好处就是在我们构造方法的时候就可以实现我们将来的点击方法，所以在自定义弹窗类的.h文件中，我们要申明block属性。代码
```
//.h
@interface HLAlertViewBlock : UIView
@property(nonatomic, copy) void (^buttonBlock) (NSInteger index);
- (instancetype)initWithTittle:(NSString *)tittle message:(NSString *)message block:(void (^) (NSInteger index))block;
- (void)show;
@end
```
```
//.m
@interface HLAlertViewBlock()
/** 弹窗主内容view */
@property (nonatomic,strong) UIView   *contentView;
/** 弹窗标题 */
@property (nonatomic,copy)   NSString *title;
/** message */
@property (nonatomic,copy)   NSString *message;
/** 确认按钮 */
@property (nonatomic,copy)   UIButton *sureButton;
@end
@implementation HLAlertViewBlock
- (instancetype)initWithTittle:(NSString *)tittle message:(NSString *)message block:(void (^)(NSInteger))block{
    if (self = [super init]) {
        self.title = tittle;
        self.message = message;
        self.buttonBlock = block;
        [self sutUpView];
    }
    return self;
}
```
到此为止，我们的block弹窗申明方法也搞定了。
## xib的封装弹窗
![](https://upload-images.jianshu.io/upload_images/3132503-25e6a4e4ffee2471.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
图片.png
好处就是不用写界面代码了。
## 殊途同归
还有一种实现弹窗效果的方法，不通过新建view而是Controller来实现的，就是新建一个透明的控制器。代码如下
```
PopViewController * popVC = [[PopViewController alloc] init];
    UIColor * color = [UIColor blackColor];
    popVC.view.backgroundColor = [color colorWithAlphaComponent:0.85];
    popVC.modalPresentationStyle = UIModalPresentationOverCurrentContext;
    [self presentViewController:popVC animated:NO completion:nil];
```
更加简单，逻辑也更加好处理一些。
最后附上demo地址：gibHub地址：[https://github.com/MrBMask](https://github.com/MrBMask)
作者：MrBMask
链接：https://www.jianshu.com/p/817c8e71f1f8
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
