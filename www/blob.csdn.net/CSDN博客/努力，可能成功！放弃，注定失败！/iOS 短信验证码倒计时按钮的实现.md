# iOS 短信验证码倒计时按钮的实现 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年05月04日 09:40:01[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：2487
#### 引言:
> 
验证码倒计时按钮的应用是非常普遍的，本文介绍了**IOS**实现验证码倒计时功能，点击获取验证码，进入时间倒计时，感兴趣的小伙伴们可以参考一下:
#### 实现思路:
- 创建按钮, 添加点击方法;
- 用NSTimer定时器, 每秒执行一次, 定时改变Button的title,改变Button的样式, 设置Button不可点击;
- 若倒计时结束, 定时器关闭, 并改变Button的样式, 可以点击;
#### 代码如下:
在按钮的点击事件里调用该方法.
```
// 开启倒计时效果
-(void)openCountdown{
    __block NSInteger time = 59; //倒计时时间
    dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
    dispatch_source_t _timer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, queue);
    dispatch_source_set_timer(_timer,dispatch_walltime(NULL, 0),1.0*NSEC_PER_SEC, 0); //每秒执行
    dispatch_source_set_event_handler(_timer, ^{
        if(time <= 0){ //倒计时结束，关闭
            dispatch_source_cancel(_timer);
            dispatch_async(dispatch_get_main_queue(), ^{
                //设置按钮的样式
                [self.authCodeBtn setTitle:@"重新发送" forState:UIControlStateNormal];
                [self.authCodeBtn setTitleColor:[UIColor colorFromHexCode:@"FB8557"] forState:UIControlStateNormal];
                self.authCodeBtn.userInteractionEnabled = YES;
            });
        }else{
            int seconds = time % 60;
            dispatch_async(dispatch_get_main_queue(), ^{
                //设置按钮显示读秒效果
                [self.authCodeBtn setTitle:[NSString stringWithFormat:@"重新发送(%.2d)", seconds] forState:UIControlStateNormal];
                [self.authCodeBtn setTitleColor:[UIColor colorFromHexCode:@"979797"] forState:UIControlStateNormal];
                self.authCodeBtn.userInteractionEnabled = NO;
            });
            time--;
        }
    });
    dispatch_resume(_timer);
}
```
#### 注意点:
我们在创建Button时, 要设置Button的样式:
当type为: UIButtonTypeCustom时 , 是读秒的效果.
当type为: 其他时, 是一闪一闪的效果.
文／Li_Cheng（简书作者）
原文链接：http://www.jianshu.com/p/2104865e7dba
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
