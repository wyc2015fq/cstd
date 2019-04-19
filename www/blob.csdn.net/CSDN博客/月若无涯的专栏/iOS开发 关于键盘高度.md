# iOS开发 关于键盘高度 - 月若无涯的专栏 - CSDN博客
2013年08月13日 12:04:32[月若无涯](https://me.csdn.net/u010124617)阅读数：1170
在iOS开发中，经常有需要根据弹出来的键盘不同高度，改变我们视图显示的需求，而键盘的高度并不是一定的（比如中文输了字和英文键盘）。
那么我们可以通过注册键盘通知，监听键盘的高度改变事件来完成。
这里我只给出最核心的部分代码。
首先是注册通知：
```
//键盘高度改变的通知
        [[NSNotificationCenter defaultCenter]addObserver:selfselector:@selector(keyboardWillShow:) name:UIKeyboardWillShowNotification object:nil];
        [[NSNotificationCenter defaultCenter]addObserver:selfselector:@selector(keyboardWillHide:) name:UIKeyboardWillHideNotification object:nil];
        // 键盘高度变化通知，ios5.0新增的
#ifdef __IPHONE_5_0
        float version = [[[UIDevicecurrentDevice]systemVersion]floatValue];
       if (version >=5.0) {
[[NSNotificationCenter defaultCenter]addObserver:selfselector:@selector(keyboardWillShow:) name:UIKeyboardWillChangeFrameNotification object:nil];
        }
#endif
```
然后是实现相应的方法：
```
#pragma mark keyboardNotification
-(void)keyboardWillShow:(NSNotification*)notification{
   NSDictionary *userInfo = [notificationuserInfo];
    // 获取键盘的属性
    NSValue* aValue = [userInfo objectForKey:UIKeyboardFrameEndUserInfoKey];
    // 获取键盘的frame
   CGRect keyboardRect = [aValueCGRectValue];
    // 获取动画时长
    NSValue *animationDurationValue = [userInfo objectForKey:UIKeyboardAnimationDurationUserInfoKey];
   NSTimeInterval animationDuration;
    [animationDurationValue getValue:&animationDuration];
    // 下面就可以是你改变视图的操作
    [self 改变视图的操作(键盘出现)];
}
-(void)keyboardWillHide:(NSNotification*)notification{
   NSDictionary* userInfo = [notificationuserInfo];
   //获取动画时长
    NSValue *animationDurationValue = [userInfo objectForKey:UIKeyboardAnimationDurationUserInfoKey];
    NSTimeInterval animationDuration;
    [animationDurationValue getValue:&animationDuration];
    // 下面是你改变视图的操作
    [self 改变视图的操作(键盘消失)];
}
```

最后提醒一下，不要忘记在相应的地方（离开界面或者销毁此类时），取消注册的通知
```
[[NSNotificationCenter defaultCenter]removeObserver:self name:UIKeyboardWillShowNotification object:nil];
    [[NSNotificationCenter defaultCenter]removeObserver:self name:UIKeyboardWillHideNotification object:nil];
#ifdef __IPHONE_5_0 
    float version = [[[UIDevice currentDevice] systemVersion] floatValue];
   if (version >= 5.0) {
    [[NSNotificationCenter defaultCenter]removeObserver:self name:UIKeyboardWillChangeFrameNotification object:nil];
    }
#endif
```

