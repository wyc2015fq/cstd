# SVProgressHUD源码阅读笔记 - weixin_33985507的博客 - CSDN博客
2017年10月10日 16:40:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：15
`SVProgressHUD` 是iOS开发中比较常用的一个第三方库，之前好几个项目都用到了这个库，但是一个没有去看它到底是怎么实现的，现在项目基本完成了，深入了解了下`SVProgressHUD`的源码，下面是我的理解，如果有什么错误的地方，望指正。
## 实现的效果
- `SVIndefiniteAnimatedView`
![1843940-31445ea37df703d4.gif](https://upload-images.jianshu.io/upload_images/1843940-31445ea37df703d4.gif)
无限循环
- `SVProgressAnimatedView`
![1843940-bf846d2087deddc7.gif](https://upload-images.jianshu.io/upload_images/1843940-bf846d2087deddc7.gif)
单次滚动
- `SVRadialGradientLayer`
![1843940-b2dbbfab27c3a8df.gif](https://upload-images.jianshu.io/upload_images/1843940-b2dbbfab27c3a8df.gif)
渐变视图
## 类文件分析
整个`SVProgressHUD`包含4个类和一个bundle文件，除`SVProgressHUD`类之外，其他都是用来修饰显示效果的。
### `SVProgressHUD`
用来显示提示框，通过类方法来设置属性和显示/消失提示框。
#### 1. HUD样式
```
typedef NS_ENUM(NSInteger, SVProgressHUDStyle) {
    SVProgressHUDStyleLight,        // 白色背景
    SVProgressHUDStyleDark,         // 黑色背景
    SVProgressHUDStyleCustom        // 自定义
};
```
#### 2. HUD遮罩层样式
```
typedef NS_ENUM(NSUInteger, SVProgressHUDMaskType) {
    SVProgressHUDMaskTypeNone = 1,  // 允许用户交互，遮罩层透明
    SVProgressHUDMaskTypeClear,     // 不允许用户交互，遮罩层透明
    SVProgressHUDMaskTypeBlack,     // 不允许用户交互，遮罩层黑色
    SVProgressHUDMaskTypeGradient,  // 不允许用户交互，遮罩层渐变色
    SVProgressHUDMaskTypeCustom     // 不允许用户交互，遮罩层自定义颜色
};
```
#### 3. HUD动画类型
```
typedef NS_ENUM(NSUInteger, SVProgressHUDAnimationType) {
    SVProgressHUDAnimationTypeFlat,     // indefinite animated
    SVProgressHUDAnimationTypeNative    // iOS 系统提供的 UIActivityIndicatorView
};
```
#### 4. 常用属性
UI_APPEARANCE_SELECTOR 可以统一设置属性。
```
@property (assign, nonatomic) CGFloat ringThickness UI_APPEARANCE_SELECTOR;                 // 圆环的宽度 默认2 pt
@property (assign, nonatomic) CGFloat ringRadius UI_APPEARANCE_SELECTOR;                    // 圆环的半径 默认18 pt
@property (assign, nonatomic) CGFloat ringNoTextRadius UI_APPEARANCE_SELECTOR;              // 没有text的时候圆环的半径 默认24 pt
@property (assign, nonatomic) CGFloat cornerRadius UI_APPEARANCE_SELECTOR;                  // HUD的圆角 默认14 pt
@property (strong, nonatomic, nonnull) UIFont *font UI_APPEARANCE_SELECTOR;                 // 字体，默认使用 [UIFont preferredFontForTextStyle:UIFontTextStyleSubheadline]
```
#### 5. 图片类型
默认是从bundle文件中读取的
```
@property (strong, nonatomic, nonnull) UIImage *infoImage UI_APPEARANCE_SELECTOR;          // 显示信息的图片
@property (strong, nonatomic, nonnull) UIImage *successImage UI_APPEARANCE_SELECTOR;        // 成功图片
@property (strong, nonatomic, nonnull) UIImage *errorImage UI_APPEARANCE_SELECTOR;          // 错误图片
```
#### 6. 常用方法介绍
- 无限循环状态显示，不会自动消失，需要主动调用dismiss方法
```
+ (void)show;
+ (void)showWithStatus:(nullable NSString*)status;
+ (void)dismiss;
+ (void)dismissWithCompletion:(nullable SVProgressHUDDismissCompletion)completion;
+ (void)dismissWithDelay:(NSTimeInterval)delay;
+ (void)dismissWithDelay:(NSTimeInterval)delay completion:(nullable SVProgressHUDDismissCompletion)completion;
```
- 进度条状态显示
```
+ (void)showProgress:(float)progress;
+ (void)showProgress:(float)progress status:(nullable NSString*)status;
```
- 图片状态显示
```
+ (void)showInfoWithStatus:(nullable NSString*)status;
+ (void)showSuccessWithStatus:(nullable NSString*)status;
+ (void)showErrorWithStatus:(nullable NSString*)status;
+ (void)showImage:(nonnull UIImage*)image status:(nullable NSString*)status;
```
- `hud`距离中心点的偏移量
```
+ (void)setOffsetFromCenter:(UIOffset)offset;
+ (void)resetOffsetFromCenter;
```
#### 7. 通知
监听用户触摸事件，HUD显示和消失事件的通知
```
extern NSString * _Nonnull const SVProgressHUDDidReceiveTouchEventNotification;
extern NSString * _Nonnull const SVProgressHUDDidTouchDownInsideNotification;
extern NSString * _Nonnull const SVProgressHUDWillDisappearNotification;
extern NSString * _Nonnull const SVProgressHUDDidDisappearNotification;
extern NSString * _Nonnull const SVProgressHUDWillAppearNotification;
extern NSString * _Nonnull const SVProgressHUDDidAppearNotification;
```
#### 8. 显示流程
`SVProgressHUD` 全部采用类方法，使用单例模式初始化HUD对象，层次结构为
`UIWindow -> UIControl -> SVProgressHUD -> HUDView -> AnimatedView`。
处理显示逻辑的方法，显示的时候要回到主线程中去操作，每次显示之前都会将之前显示的HUD移除掉，并移除之前的动画，使用`CATransaction`避免被干扰。
```
- (void)showProgress:(float)progress status:(NSString*)status
```
```
- (void)showImage:(UIImage*)image status:(NSString*)status duration:(NSTimeInterval)duration
```
```
- (void)showProgress:(float)progress status:(NSString*)status {
    __weak SVProgressHUD *weakSelf = self;
    // 这里并没有采用GCD的方式 而是使用NSOperationQueue 回到主队列执行operation
    [[NSOperationQueue mainQueue] addOperationWithBlock:^{
        __strong SVProgressHUD *strongSelf = weakSelf;
        if(strongSelf){
            // Stop timer
            strongSelf.fadeOutTimer = nil;
            strongSelf.graceTimer = nil;
            
            // 更新视图层级 使得HUD可以始终显示在最外层
            // Update / Check view hierarchy to ensure the HUD is visible
            [strongSelf updateViewHierarchy];
            
            // Reset imageView and fadeout timer if an image is currently displayed
            strongSelf.imageView.hidden = YES;
            strongSelf.imageView.image = nil;
            
            // Update text and set progress to the given value
            strongSelf.statusLabel.hidden = status.length == 0;
            strongSelf.statusLabel.text = status;
            strongSelf.progress = progress;
            
            // progress >= 0时 显示进度条动画ring
            // Choose the "right" indicator depending on the progress
            if(progress >= 0) {
                // Cancel the indefiniteAnimatedView, then show the ringLayer
                [strongSelf cancelIndefiniteAnimatedViewAnimation];
                
                // Add ring to HUD
                // ringView 没有superView 即ringView没有被添加进view中
                if(!strongSelf.ringView.superview){
#if __IPHONE_OS_VERSION_MAX_ALLOWED >= 80000
                    [strongSelf.hudView.contentView addSubview:strongSelf.ringView];
#else
                    [strongSelf.hudView addSubview:strongSelf.ringView];
#endif
                }
                // backgroundRingView ringView 同一种类型
                if(!strongSelf.backgroundRingView.superview){
#if __IPHONE_OS_VERSION_MAX_ALLOWED >= 80000
                    [strongSelf.hudView.contentView addSubview:strongSelf.backgroundRingView];
#else
                    [strongSelf.hudView addSubview:strongSelf.backgroundRingView];
#endif
                }
                
                // Set progress animated
                // CATransaction 动画的事务性 这里是显式事物
                [CATransaction begin];
                // 不显示动画的过程
                [CATransaction setDisableActions:YES];
                strongSelf.ringView.strokeEnd = progress;
                [CATransaction commit];
            } else {
                // Cancel the ringLayer animation, then show the indefiniteAnimatedView
                // 取消progress的动画，显示无限循环的那个动画
                [strongSelf cancelRingLayerAnimation];
                
                // Add indefiniteAnimatedView to HUD
#if __IPHONE_OS_VERSION_MAX_ALLOWED >= 80000
                [strongSelf.hudView.contentView addSubview:strongSelf.indefiniteAnimatedView];
#else
                [strongSelf.hudView addSubview:strongSelf.indefiniteAnimatedView];
#endif
                // 开始执行动画
                if([strongSelf.indefiniteAnimatedView respondsToSelector:@selector(startAnimating)]) {
                    [(id)strongSelf.indefiniteAnimatedView startAnimating];
                }
            }
            
            // Fade in delayed if a grace time is set
            if (self.graceTimeInterval > 0.0 && self.backgroundView.alpha == 0.0f) {
                strongSelf.graceTimer = [NSTimer timerWithTimeInterval:self.graceTimeInterval target:strongSelf selector:@selector(fadeIn:) userInfo:nil repeats:NO];
                // 添加到NSRunLoopCommonModes中 避免其他的RunLoop干扰
                [[NSRunLoop mainRunLoop] addTimer:strongSelf.graceTimer forMode:NSRunLoopCommonModes];
            } else {
                [strongSelf fadeIn:nil];
            }
            
            // Tell the Haptics Generator to prepare for feedback, which may come soon
#if TARGET_OS_IOS && __IPHONE_OS_VERSION_MAX_ALLOWED >= 100000
            // 告诉触觉反馈器 开始接收反馈
            [strongSelf.hapticGenerator prepare];
#endif
        }
    }];
}
```
```
- (void)fadeIn:(id)data {
    // Update the HUDs frame to the new content and position HUD
    // 更新HUD的frame
    [self updateHUDFrame];
    // 更新HUD的位置
    [self positionHUD:nil];
    
    // Update accessibility as well as user interaction
    if(self.defaultMaskType != SVProgressHUDMaskTypeNone) {
        // 不拥护用户交互 controlView 继承自UIControl
        self.controlView.userInteractionEnabled = YES;
        self.accessibilityLabel = self.statusLabel.text ?: NSLocalizedString(@"Loading", nil);
        self.isAccessibilityElement = YES;
    } else {
        // 用户是可以交互的 可以点击除显示HUD之外的区域
        self.controlView.userInteractionEnabled = NO;
        self.hudView.accessibilityLabel = self.statusLabel.text ?: NSLocalizedString(@"Loading", nil);
        self.hudView.isAccessibilityElement = YES;
    }
    
    // Get duration
    // 获取显示时间
    id duration = [data isKindOfClass:[NSTimer class]] ? ((NSTimer *)data).userInfo : data;
    
    // Show if not already visible
    if(self.backgroundView.alpha != 1.0f) { // 开始要显示
        // Post notification to inform user
        // 通知用户将要显示HUD了
        [[NSNotificationCenter defaultCenter] postNotificationName:SVProgressHUDWillAppearNotification
                                                            object:self
                                                          userInfo:[self notificationUserInfo]];
        
        // Shrink HUD to to make a nice appear / pop up animation
        /*
         CGAffineTransform CGAffineTransformScale(CGAffineTransform t,
         CGFloat sx, CGFloat sy)
         t 要进行变换的矩阵
         sx x方向上的缩放倍数
         sy y方向上的缩放倍数
         
         缩小2/3
         */
        self.hudView.transform = self.hudView.transform = CGAffineTransformScale(self.hudView.transform, 1/1.5f, 1/1.5f);
        
        // 执行完动画后的回调 这里写成block的原因 可以在方法里调用方法
        __block void (^animationsBlock)(void) = ^{
            // Zoom HUD a little to make a nice appear / pop up animation
            self.hudView.transform = CGAffineTransformIdentity;
            
            // Fade in all effects (colors, blur, etc.)
            [self fadeInEffects];
        };
        
        // 显示完成后的回调
        __block void (^completionBlock)(void) = ^{
            // Check if we really achieved to show the HUD (<=> alpha)
            // and the change of these values has not been cancelled in between e.g. due to a dismissal
            if(self.backgroundView.alpha == 1.0f){ // 显示完成
                // Register observer <=> we now have to handle orientation changes etc.
                [self registerNotifications];
                
                // Post notification to inform user
                [[NSNotificationCenter defaultCenter] postNotificationName:SVProgressHUDDidAppearNotification
                                                                    object:self
                                                                  userInfo:[self notificationUserInfo]];
                
                // Update accessibility
                UIAccessibilityPostNotification(UIAccessibilityScreenChangedNotification, nil);
                UIAccessibilityPostNotification(UIAccessibilityAnnouncementNotification, self.statusLabel.text);
                
                // Dismiss automatically if a duration was passed as userInfo. We start a timer
                // which then will call dismiss after the predefined duration
                if(duration){
                    // 显示完成后使用计时器调用消失的方法
                    self.fadeOutTimer = [NSTimer timerWithTimeInterval:[(NSNumber *)duration doubleValue] target:self selector:@selector(dismiss) userInfo:nil repeats:NO];
                    [[NSRunLoop mainRunLoop] addTimer:self.fadeOutTimer forMode:NSRunLoopCommonModes];
                }
            }
        };
        
        // Animate appearance
        if (self.fadeInAnimationDuration > 0) {
            // Animate appearance
            [UIView animateWithDuration:self.fadeInAnimationDuration
                                  delay:0
                                options:(UIViewAnimationOptions) (UIViewAnimationOptionAllowUserInteraction | UIViewAnimationCurveEaseIn | UIViewAnimationOptionBeginFromCurrentState)
                             animations:^{
                                 animationsBlock();
                             } completion:^(BOOL finished) {
                                 completionBlock();
                             }];
        } else {
            animationsBlock();
            completionBlock();
        }
        
        // Inform iOS to redraw the view hierarchy
        // 告诉系统更新视图层级
        [self setNeedsDisplay];
    } else { // 如果已经显示
        // Update accessibility
        UIAccessibilityPostNotification(UIAccessibilityScreenChangedNotification, nil);
        UIAccessibilityPostNotification(UIAccessibilityAnnouncementNotification, self.statusLabel.text);
        
        // Dismiss automatically if a duration was passed as userInfo. We start a timer
        // which then will call dismiss after the predefined duration
        if(duration){
            self.fadeOutTimer = [NSTimer timerWithTimeInterval:[(NSNumber *)duration doubleValue] target:self selector:@selector(dismiss) userInfo:nil repeats:NO];
            [[NSRunLoop mainRunLoop] addTimer:self.fadeOutTimer forMode:NSRunLoopCommonModes];
        }
    }
}
```
处理消失逻辑的方法，同样需要到主线程中操作。
```
- (void)dismissWithDelay:(NSTimeInterval)delay completion:(SVProgressHUDDismissCompletion)completion
```
```
- (void)dismissWithDelay:(NSTimeInterval)delay completion:(SVProgressHUDDismissCompletion)completion {
    __weak SVProgressHUD *weakSelf = self;
    // 同show方法
    [[NSOperationQueue mainQueue] addOperationWithBlock:^{
        __strong SVProgressHUD *strongSelf = weakSelf;
        if(strongSelf){
            // Stop timer
            strongSelf.graceTimer = nil;
            
            // Post notification to inform user
            [[NSNotificationCenter defaultCenter] postNotificationName:SVProgressHUDWillDisappearNotification
                                                                object:nil
                                                              userInfo:[strongSelf notificationUserInfo]];
            
            __block void (^animationsBlock)(void) = ^{
                // Shrink HUD a little to make a nice disappear animation
                strongSelf.hudView.transform = CGAffineTransformScale(strongSelf.hudView.transform, 1/1.3f, 1/1.3f);
                
                // Fade out all effects (colors, blur, etc.)
                [strongSelf fadeOutEffects];
            };
            
            __block void (^completionBlock)(void) = ^{
                // Check if we really achieved to dismiss the HUD (<=> alpha values are applied)
                // and the change of these values has not been cancelled in between e.g. due to a new show
                if(self.backgroundView.alpha == 0.0f){
                    // Clean up view hierarchy (overlays)
                    [strongSelf.controlView removeFromSuperview];
                    [strongSelf.backgroundView removeFromSuperview];
                    [strongSelf.hudView removeFromSuperview];
                    [strongSelf removeFromSuperview];
                    
                    // Reset progress and cancel any running animation
                    strongSelf.progress = SVProgressHUDUndefinedProgress;
                    [strongSelf cancelRingLayerAnimation];
                    [strongSelf cancelIndefiniteAnimatedViewAnimation];
                    
                    // Remove observer <=> we do not have to handle orientation changes etc.
                    [[NSNotificationCenter defaultCenter] removeObserver:strongSelf];
                    
                    // Post notification to inform user
                    [[NSNotificationCenter defaultCenter] postNotificationName:SVProgressHUDDidDisappearNotification
                                                                        object:strongSelf
                                                                      userInfo:[strongSelf notificationUserInfo]];
                    
                    // Tell the rootViewController to update the StatusBar appearance
#if !defined(SV_APP_EXTENSIONS) && TARGET_OS_IOS
                    // 更新状态栏
                    UIViewController *rootController = [[UIApplication sharedApplication] keyWindow].rootViewController;
                    [rootController setNeedsStatusBarAppearanceUpdate];
#endif
                    
                    // Run an (optional) completionHandler
                    if (completion) {
                        completion();
                    }
                }
            };
            
            // UIViewAnimationOptionBeginFromCurrentState AND a delay doesn't always work as expected
            // When UIViewAnimationOptionBeginFromCurrentState is set, animateWithDuration: evaluates the current
            // values to check if an animation is necessary. The evaluation happens at function call time and not
            // after the delay => the animation is sometimes skipped. Therefore we delay using dispatch_after.
            
            dispatch_time_t dipatchTime = dispatch_time(DISPATCH_TIME_NOW, (int64_t)(delay * NSEC_PER_SEC));
            dispatch_after(dipatchTime, dispatch_get_main_queue(), ^{
                if (strongSelf.fadeOutAnimationDuration > 0) {
                    // Animate appearance
                    [UIView animateWithDuration:strongSelf.fadeOutAnimationDuration
                                          delay:0
                                        options:(UIViewAnimationOptions) (UIViewAnimationOptionAllowUserInteraction | UIViewAnimationCurveEaseOut | UIViewAnimationOptionBeginFromCurrentState)
                                     animations:^{
                                         animationsBlock();
                                     } completion:^(BOOL finished) {
                                         completionBlock();
                                     }];
                } else {
                    animationsBlock();
                    completionBlock();
                }
            });
            
            // Inform iOS to redraw the view hierarchy
            // 告诉系统更新视图层级
            [strongSelf setNeedsDisplay];
        }
    }];
}
```
### `SVIndefiniteAnimatedView`
这个类提供了无限旋转动画，可以单独拿出来使用，实现思路是
初始化两个layer对象，indefiniteAnimatedLayer和maskLayer，maskLayer有一张图片，设置indefiniteAnimatedLayer的mask为maskLayer，这样就得到一张渐变的layer。分别设置layer和layer.mask的动画达到效果。示例图：
![1843940-26a95153c8b2b295.jpg](https://upload-images.jianshu.io/upload_images/1843940-26a95153c8b2b295.jpg)
mask效果
### `SVProgressAnimatedView`
这个类提供环形加载效果，通过不断修改strokeEnd的值，实现了进度的显示。
```
- (void)setStrokeEnd:(CGFloat)strokeEnd {
    _strokeEnd = strokeEnd;
    _ringAnimatedLayer.strokeEnd = _strokeEnd;
}
```
### `SVRadialGradientLayer`
继承自`CALayer`，使用`CGContextDrawRadialGradient`方法画渐变层。
以上，就是暂时对`SVProgressHUD`的理解，理解不够的地方，欢迎交流。
