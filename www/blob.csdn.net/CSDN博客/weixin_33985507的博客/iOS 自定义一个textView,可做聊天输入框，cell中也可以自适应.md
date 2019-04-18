# iOS 自定义一个textView,可做聊天输入框，cell中也可以自适应 - weixin_33985507的博客 - CSDN博客
2018年04月02日 10:30:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：23
> 
前言
项目中经常会用到`UITextView`，但是系统的`textView`在限制多行输入的时候会出现问题。（过渡动画很乱）。基于限制多行输入或者`cell`跟随`textView`内容自适应的需求，所以自定义一个`textView`。
> 
自定义textView
关于textView的一些摸索，[iOS仿微信输入框，限制最大行数，一体键盘(两种思路)](https://www.jianshu.com/p/d5a5ce3760fa)，这篇文章中有介绍，下面示例会用到。
封装一个可限制多行输入的`textView`，首先屏蔽掉系统对`textView`的默认设置`_textView.textContainerInset = UIEdgeInsetsZero`，屏蔽掉之后可以给`textView`添加一个上下左右的间距，`UIView`作为`textView`的容器。这样在限制多上输入的时候动画过度就会很自然，默认是不限制行数的。
.h接口的配置：
```
@property(nonatomic,assign)CGFloat v_margin;//竖直方向上下间距 默认为8；
@property(nonatomic,assign)CGFloat h_margin;//水平方向上下间距 默认为0；
@property(nonatomic,assign)NSInteger  initiLine;//初始需要展示的行数 默认为1；
@property(nonatomic,assign)NSInteger maxLine;//最大行数 默认为无穷大；
@property(nonatomic,strong)NSString *placeholder;//占位文字
@property(nonatomic,strong)UIFont *font;//默认为17
@property(nonatomic,assign)CGPoint placePoint;//设置占位符的位置，竖直方向设置v_margin即可  CGPointMake(5, 0);//占位文字的起始位置；
@property(nonatomic,copy)void (^textHeightChangeBlock)(CGFloat height);
```
##### textView需要一个默认的初始行数，默认是1，所以后续自定义的输入框是要根据行数来确定高度，随便给高度，动画就很尴尬。
`textView`已经屏蔽掉了`iOS11` 对`textView`作为`UIScrollView`子类的处理：
```
if (@available(iOS 11.0, *)) {
        self.contentInsetAdjustmentBehavior = UIScrollViewContentInsetAdjustmentNever;
    }
```
关于限制行数的处理：
```
-(void)textViewDidChange:(UITextView *)textView{
    
    
    //内容高度
    CGFloat contentSizeH = self.textView.contentSize.height;
    
    
    //最大高度
    CGFloat maxHeight = ceil(self.font.lineHeight * self.maxLine);
    
    //初始高度
    CGFloat initiTextViewHeight = ceilf(self.initiLine *self.font.lineHeight);
    if (contentSizeH <= maxHeight) {
        
        if (contentSizeH <= initiTextViewHeight) {
            self.textView.height = initiTextViewHeight;
        }else{
            self.textView.height = contentSizeH;
        }
        
        
    }else{
        self.textView.height = maxHeight;
    }
    
    self.height = self.textView.height + 2 * self.v_margin;
    
    if (self.textHeightChangeBlock) {
        self.textHeightChangeBlock(self.height);
    }
    [textView scrollRangeToVisible:NSMakeRange(textView.selectedRange.location, 1)];
    
    
}
```
##### 支持约束以及frame配置。
> 
示例1 自定义一体键盘，限制最大行数。
很简单的在`xib`底部关联一个自定义的`textView`，需要监测两个通知`UIKeyboardWillChangeFrameNotificatio`，`UIKeyboardDidChangeFrameNotification`，至于为什么要监听，前面那篇文章有详细说明。补充一点，如果项目中有使用到`IQKeyboardManager`，需要在当前页面设置`IQKeyboardManager`不可用，在自定义键盘的时候如果`IQKeyboardManager`开始，可能会出现导航栏突然消失很多问题，所以关闭掉，监听两个通知即可，改变自定义键盘 底部距离屏幕底部的约束即可，键盘弹出，高于键盘即可。
```
[[NSNotificationCenter defaultCenter]addObserver:self selector:@selector(keyboardWillChangeFrame:) name:UIKeyboardWillChangeFrameNotification object:nil];
     [[NSNotificationCenter defaultCenter]addObserver:self selector:@selector(keyboardDidChangeFrame:) name:UIKeyboardDidChangeFrameNotification object:nil];
-(void)viewWillAppear:(BOOL)animated{
    [super viewWillAppear:animated];
    self.isDisappear = NO;
     [IQKeyboardManager sharedManager].enable = NO; //关闭
}
-(void)viewWillDisappear:(BOOL)animated{
    [super viewWillDisappear:animated];
    self.isDisappear = YES;
    [IQKeyboardManager sharedManager].enable = YES; //开启
}
```
```
-(void)keyboardWillChangeFrame:(NSNotification *)notification{
    
    NSDictionary *userInfo = notification.userInfo;
    // 动画的持续时间
    double duration = [userInfo[UIKeyboardAnimationDurationUserInfoKey] doubleValue];
    
    // 键盘的frame
    CGRect keyboardF = [userInfo[UIKeyboardFrameEndUserInfoKey] CGRectValue];
    //        NSLog(@"%@",NSStyringFromCGRect(keyboardF));
   CGFloat keyboardY = keyboardF.origin.y;
    keyboardH =  keyboardF.size.height;
    
    
    if (!self.isDisappear) {
        [self dealKeyBoardWithKeyboardH:keyboardH keyboardY:keyboardY duration:duration];
    }
}
-(void)keyboardDidChangeFrame:(NSNotification *)notification{
    
    NSDictionary *userInfo = notification.userInfo;
   
    // 动画的持续时间
    double duration = [userInfo[UIKeyboardAnimationDurationUserInfoKey] doubleValue];
    CGRect keyboardF = [userInfo[UIKeyboardFrameEndUserInfoKey] CGRectValue];
    
    CGFloat keyboardY = keyboardF.origin.y;
    
    keyboardH =  keyboardF.size.height;
    
    if (self.isDisappear) {
         [self dealKeyBoardWithKeyboardH:keyboardH keyboardY:keyboardY duration:duration];
    }
   
    
}
#pragma mark---处理高度---
-(void)dealKeyBoardWithKeyboardH:(CGFloat)keyboardH keyboardY:(CGFloat)keyboardY duration:(CGFloat)duration{
    
    if (!self.isDisappear) {
        [UIView animateWithDuration:duration animations:^{
            // 工具条的Y值 == 键盘的Y值 - 工具条的高度
            
            if (keyboardY >= Device_Height) {
                self.bottomH.constant = 0;
            }else
            {
                self.bottomH.constant = keyboardH;
            }
        }];
    }else{
        if (keyboardY >= Device_Height) {
            self.bottomH.constant = 0;
        }else
        {
            self.bottomH.constant = keyboardH;
        }
    }
    
}
```
##### 如果在键盘弹出的时候同时textView需要自动增长，那么也需要设置如下：
```
self.textView.textHeightChangeBlock = ^(CGFloat height) {
        
        weakSelf.textH.constant = height;
        weakSelf.bottomH.constant = keyboardH;
        [weakSelf.view updateConstraints];
        [weakSelf.view updateConstraintsIfNeeded];
        [UIView animateWithDuration:0.25 animations:^{
            [weakSelf.view layoutIfNeeded];
        }];
    };
```
> 
示例2  cell跟随textView自适应，textView也可以限制多行
需要做的也很简单，在`textView`自动增长的地方，处理如下：(当前textView不能失去焦点)
```
- (void)awakeFromNib {
    [super awakeFromNib];
    // Initialization code
    
    self.textView.initiLine = 1;
    self.textView.maxLine = 3;
    self.textView.placeholder= @"请输入";
    
    
    self.textViewH.constant = self.textView.height;
    
    LXWS(weakSelf);
    self.textView.textHeightChangeBlock = ^(CGFloat height) {
        
        weakSelf.textViewH.constant = height;
        
        [weakSelf.contentView updateConstraints];
        [weakSelf.contentView updateConstraintsIfNeeded];
        [UIView animateWithDuration:0.25 animations:^{
            [weakSelf.contentView layoutIfNeeded];
        }];
        UITableView *tableView = [weakSelf tableView];
        [tableView beginUpdates];
        [tableView endUpdates];
      
    };
```
> 
补充
最近发现使用中发现示例2 结合`IQKeyboardManager`使用的时候出现的一点问题，在换行的时候输入框的位置并没有变化，只有当键盘重新定位的时候才可以，如果能够在换行的时候使键盘重新定位就好了。（2018年9月19日）
看一个`IQKeyboardManager`属性：
```
/**
 Refreshes textField/textView position if any external changes is explicitly made by user.
 */
- (void)reloadLayoutIfNeeded;
```
刚刚好可以在换行的时候重新进行键盘定位。
对于示例2 我们可以添加一行代码：
```
self.textView.textHeightChangeBlock = ^(CGFloat height) {
        
        weakSelf.textViewH.constant = height;
        
        IQKeyboardManager *manager =[IQKeyboardManager sharedManager];
        //当输入框位置变动时刷新位置
        [manager reloadLayoutIfNeeded];
        [weakSelf.contentView updateConstraints];
        [weakSelf.contentView updateConstraintsIfNeeded];
        [UIView animateWithDuration:0.25 animations:^{
            [weakSelf.contentView layoutIfNeeded];
        }];
        UITableView *tableView = [weakSelf tableView];
        [tableView beginUpdates];
        [tableView endUpdates];
      
    };
```
效果图：
![1694376-e8a6be8b285d2d4c.gif](https://upload-images.jianshu.io/upload_images/1694376-e8a6be8b285d2d4c.gif)
textView.gif
demo: [LXCustomTextView](https://github.com/liuxinixn/LXXibTextView)
