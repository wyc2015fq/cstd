# 基于MBProgressHUD的封装 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年03月30日 15:24:03[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：4318
# 1、前言
在ios开发中，最经典也是最常用的提示框就是MBProgressHUD了，用于在执行一些任务时的提示效果，但它也有一定的弊端，就是封装的不是很好，每次我们使用的时候就要费很大的劲写一堆大量的重复的代码，这对于我们天生有着强迫症的程序猿来说是绝对不能容忍的，而且还有一点不能容忍的就是，比如说去后台请求数据时，如果数据量比较大，网速又比较慢，那么那个loading就一直在那转啊转，想取消都木得可能！这怎么受得了，必须得改，对于有着代码洁癖的我来说又不想在每次请求的地方都去写一堆判断什么时候该取消，于是就诞生了这篇文章，创建了MBManager的类，一句代码搞定提示框的问题，另外我在里面添加了手势功能，只要点击屏幕“loading”就再也不见了，瞬间心情舒畅了很多，有木有！
- **特性**
1、对MBProgressHUD就行封装，调用只需要一句代码
2、添加手势功能，触摸屏幕就可以取消提示框
3、demo经历了项目的考验。目前暂没有发现bug，请放心使用
- **效果**
![](http://upload-images.jianshu.io/upload_images/1350207-b8a531cf05310d4d.gif?imageMogr2/auto-orient/strip)
MBManager.gif
- **demo地址：[MBManager下载地址，请戳](https://github.com/hungryBoy/alertView)**
# 2、demo示例
大家可以先下载下来demo运行一下看看效果，和这个gif图不太一样，这个图片是我没有修改之前录制的效果，但总体是差不多的。
- **头文件里面有这么些内容，很简单，不做过多介绍，大家看注释就行了，很清晰：**
（1）定义预设提示语和显示时间，可根据需要在这直接修改
```
static NSString *const kLoadingMessage = @"加载中"; //定义网络加载时显示的提示语，可以在此直接修改成想要的提示，达到定义一次，全局有一样
static CGFloat const   kShowTime  = 2.0f;//定义自动消失的提示语显示的时间，可直接修改
```
（2）蒙层属性，默认为YES
```
//是否显示变淡效果，默认为YES，  PS：只为 showPermanentAlert:(NSString *) alert 和 showLoading 方法添加
@property (nonatomic, assign) BOOL isShowGloomy;
```
（3）最常用的三种方法，加载提示框到当前的window上
```
/**
*  显示“加载中”，待圈圈，若要修改直接修改kLoadingMessage的值即可
*/
+(void) showLoading;
/**
*  一直显示自定义提示语，不带圈圈
*
*  @param alert 提示信息
*/
+(void) showPermanentAlert:(NSString *) alert;
/**
*  显示简短的提示语，默认2秒钟，时间可直接修改kShowTime
*
*  @param alert 提示信息
*/
+(void) showBriefAlert:(NSString *) alert;
/**
*  隐藏alert
*/
+(void)hideAlert;
```
（4）添加提示框到特定的view上，一般用不到，除非要添加提示框到特定的view上，这一点在demo里面也有演示，大家可以[下载](https://github.com/hungryBoy/alertView)下来看一下效果。
```
/***************************************
*                                     *
*  以下方法根据情况可选择使用，一般使用不到  *
*                                     *
***************************************
*/ 
/**
*  显示简短提示语到view上
*
*  @param message 提示语
*  @param view    要添加到的view
*/
+(void) showBriefMessage:(NSString *) message InView:(UIView *) view; 
/**
*  显示长久的（只要不用手触摸屏幕或者调用hideAlert方法就会一直显示）提示语到view上
*
*  @param message 提示语
*  @param view    要添加到的view
*/
+(void) showPermanentMessage:(NSString *)message InView:(UIView *) view; 
/**
*  显示网络加载到view上
*
*  @param view 要添加到的view
*/
+(void) showLoadingInView:(UIView *) view;
```
# 3、demo详解
**（1）创建单例**
保证整个工程只有一个MBManager实例
```
//用GCD来创建单例
+(instancetype )shareManager{
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        hudManager = [[self alloc] init];
    });
    return hudManager;
}
```
**（2）初始化，重写init方法**
```
-(instancetype)init{
    if (self = [super init]) {
        [self initBackView];//创建窗口蒙层，出现北京变淡的效果
        self.isShowGloomy = YES;//默认打开蒙层，如果不想要这个效果，可以直接修改为NO
    }
    return self;
}
```
**（3）创建蒙层**
```
-(void)initBackView{
    //创建蒙层，默认初始大小为整个屏幕，设置背景色为黑色，透明度为0.5，隐藏属性为YES，当需要的时候再设其为NO
    bottomView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, kScreen_width, kScreen_height)];
    bottomView.backgroundColor = [UIColor blackColor];
    bottomView.alpha = 0.5;
    bottomView.hidden = YES;
}
```
**（4）停留的提示框**
```
//调用此方法产生的提示框会长时间的停留在界面，直到用手触摸屏幕或者调用hideAlert方法
+ (void) showPermanentMessage:(NSString *)message InView:(UIView *) view{
    hudAddedView = view;//记录当前提示框添加到的view
    [self shareManager];//创建Manager
    if (view == nil) {
        view = [[UIApplication sharedApplication] windows].lastObject;//如果view为nil，则把当前的window赋值为view，把提示框添加到当前的window上
    }
    MBProgressHUD *hud = [MBProgressHUD showHUDAddedTo:view animated:YES];//创建hud
    hud.labelText = message;//要显示的提示语
    hud.removeFromSuperViewOnHide = YES;//hud隐藏时同时从父视图上移除
    hud.mode = MBProgressHUDModeCustomView;//设置hud的样式
    if (hudManager.isShowGloomy) {//根据isShowGloomy的值来判断要不要显示蒙层效果
        //如果添加了view则将botomView的frame修改与view一样大
        if (hudAddedView) {//如果设置了要把提示框添加到的view则把蒙层的frame设置成和要添加的view的frame一样大
            bottomView.frame = CGRectMake(0, 0, hudAddedView.frame.size.width, hudAddedView.frame.size.height);
        }
        [view addSubview:bottomView];//添加蒙层到view上
        [hudManager showBackView];//此时蒙层并没有显示，因为botomView的hide属性为YES，需调用此方法蒙层效果才会出现
    }
    [view bringSubviewToFront:hud];//把hud移到最上面，否则hud的效果有点淡，因为被botomView遮挡了
    [hudManager addGestureInView:view];//添加手势
}
```
**（5）网络加载用，统一显示预设的提示**
显示转圈圈，一般用于网络请求数据或者处理较大量的数据时用
```
+ (void) showLoadingInView:(UIView *) view{
    hudAddedView = view;
    [self shareManager];
    if (view == nil) {
        view = [[UIApplication sharedApplication] windows].lastObject;
    }
    MBProgressHUD *hud = [[MBProgressHUD alloc] initWithView:view];
    hud.labelText = kLoadingMessage;//预设的提示语，直接修改其值
    hud.removeFromSuperViewOnHide = YES;
    if (hudManager.isShowGloomy) {
        //如果添加了view则将botomView的frame修改与view一样大
        if (hudAddedView) {
            bottomView.frame = CGRectMake(0, 0, hudAddedView.frame.size.width, hudAddedView.frame.size.height);
        }
        [view addSubview:bottomView];
        [hudManager showBackView];
    }
    [view addSubview:hud];
    [hud show:YES];//调用此方法后才会显示
    [hudManager addGestureInView:view];
}
```
**（6）显示简短、简捷的提示语**
这种效果就是很窄的那种一句话提示，默认2秒后自动消失，当然也是支持手势的，具体效果可以直接看demo
```
+ (void) showBriefMessage:(NSString *) message InView:(UIView *) view{
    hudAddedView = view;
    [self shareManager];
    if (view == nil) {
        view = [[UIApplication sharedApplication] windows].lastObject;
    }
    MBProgressHUD *hud = [MBProgressHUD showHUDAddedTo:view animated:YES];
    hud.labelText = message;
    hud.mode = MBProgressHUDModeText;
    hud.margin = 10.f;
    //HUD.yOffset = 200;
    hud.removeFromSuperViewOnHide = YES;
    [hud hide:YES afterDelay:kShowTime];//要显示的时间可以在这修改
    [hudManager addGestureInView:view];
}
```
**（7）最常用的三种方法**
上面那几个方法很少用的，最常用的就是下面这三个，不需要传要添加的view，只需要输入我们要显示的提示语就可以了，甚至于连提示语都不需要输入，想想都很酷的样子
```
//很简单，不再作过多介绍，只需要调上面对应的方法，传参为nil就可以了
+(void)showLoading{
    [self showLoadingInView:nil];
}
+(void)showBriefAlert:(NSString *)alert{
    [self showBriefMessage:alert InView:nil];
}
+(void)showPermanentAlert:(NSString *)alert{
    [self showPermanentMessage:alert InView:nil];
}
```
**（8）有提示就得有隐藏**
```
+(void)hideAlert{
    [hudManager hideBackView];
    UIView *view ;//找到提示框添加到的view
    if (hudAddedView) {
        view = hudAddedView;
    }else{
        view = [[UIApplication sharedApplication].windows lastObject];
    }
    [self hideHUDForView:view];//将hud从view上移除
}
+ (void)hideHUDForView:(UIView *)view
{
    [self hideHUDForView:view animated:YES];
}
+ (BOOL)hideHUDForView:(UIView *)view animated:(BOOL)animated {
    MBProgressHUD *hud = [self HUDForView:view];
    if (hud != nil) {
        hud.removeFromSuperViewOnHide = YES;
        [hud hide:animated];
        return YES;
    }
    return NO;
}
+ (MBProgressHUD *)HUDForView:(UIView *)view {
    NSEnumerator *subviewsEnum = [view.subviews reverseObjectEnumerator];
    for (UIView *subview in subviewsEnum) {
        if ([subview isKindOfClass:[MBProgressHUD class]]) {
            return (MBProgressHUD *)subview;
        }
    }
    return nil;
}
```
**（9）有关蒙层的两个方法**
```
//也很简单，就是控制hidden属性和frame，以及手势的移除
-(void)showBackView{
    bottomView.hidden = NO;
}
-(void)hideBackView{
    bottomView.hidden = YES;
    [tap removeTarget:nil action:nil];
    bottomView.frame = CGRectMake(0, 0, kScreen_width, kScreen_height);
}
```
**（10）点击屏幕**
```
//去除蒙层，移除手势，隐藏hud
-(void)tapTheScreen{
    NSLog(@"点击屏幕");
    [hudManager hideBackView];
    [tap removeTarget:nil action:nil];
    [MBManager hideAlert];
}
```
**（11）解决手势冲突**
```
//主要解决手势与button和tableView的冲突，在项目中有出现了与自定义的segment冲突，导致segment点击无效，于是添加此方法予以解决
-(BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldReceiveTouch:(UITouch *)touch {
    if ([NSStringFromClass([touch.view class]) isEqualToString:@"PKProductMainListTableViewCellContentView"]) {
        return NO;
    }
    if ([touch.view isKindOfClass:[UITableViewCell class]]) {
        return NO;
    }
    if ([touch.view isKindOfClass:[UIButton class]]) {
        return NO;
    }
    return YES;
}
-(BOOL)gestureRecognizerShouldBegin:(UIGestureRecognizer *)gestureRecognizer{
    return YES;
}
```
# 4、使用
使用方法很简单，只需要导入MBManager和MBProgressHUD库（如果你的项目里面已经包含MBProgressHUD不要重复导入），然后在需要的地方import MBManager，然后就可直接调用了，建议在pch里面直接导入MBManager，这样在全工程都可以用了：
```
- (IBAction)showTextOnly:(UIButton *)sender {
    [MBManager showBriefMessage:@"提示语" InView:self.view];
}
- (IBAction)showStill:(UIButton *)sender {
#if 0
    [MBManager showPermanentAlert:@"一直显示"];
#else
    [MBManager showPermanentMessage:@"一直显示" InView:self.backView];
#endif
}
- (IBAction)showDelay:(UIButton *)sender {
#if 1
    [MBManager showLoading];
#else
    [MBManager hideAlert];
    [MBManager showBriefAlert:@"hello"];
#endif
}
```
# 总结
我本人也是刚开始在简书上写些东西，以供记录自己学习的点滴，同时也希望能给大家提供点便利，不奢求star，但毕竟也写了一上午才弄好，如果好用还请大家多多支持，不足之处，欢迎指点！
另附下载链接：
[MBManager下载地址](https://github.com/hungryBoy/alertView)
