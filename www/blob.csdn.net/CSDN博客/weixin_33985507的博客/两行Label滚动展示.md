# 两行Label滚动展示 - weixin_33985507的博客 - CSDN博客
2017年04月18日 17:28:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
> 
大概思路
分为两个View，一个view上展示两行Label；
两个View使用定时器切换滚动;
Label上添加点击手势，获取被点击的Label的tag值。
```
- (void)initAction
{
    CGRect showFrame = CGRectMake(0, 0, 320, 88);//正在展示
    CGRect middleFrame = CGRectMake(0, -88, 320, 88);//上移后的位置
    CGRect nextFrame = CGRectMake(0, 88, 320, 88);//准备展示
    
    firView = [[UIView alloc] initWithFrame:showFrame];
    secView = [[UIView alloc] initWithFrame:middleFrame];
   
    for (int i = 0; i < 4; i ++)
    {
        CGRect labelFrame = CGRectMake(0, 0 + (i%2) * 44, 320, 44);
        UILabel *label = [[UILabel alloc] initWithFrame:labelFrame];
        label.textAlignment = NSTextAlignmentCenter;
        label.text = [NSString stringWithFormat:@"%d",i];
        label.backgroundColor = [UIColor grayColor];
        label.tag = i;
        
        UITapGestureRecognizer *gesture = [[UITapGestureRecognizer alloc] init];
        [gesture addTarget:self action:@selector(labelTaped:)];
        label.userInteractionEnabled = YES;
        [label addGestureRecognizer:gesture];
        
        if (i == 0 || i == 1)
        {
            [firView addSubview:label];
        }
        else
        {
            [secView addSubview:label];
        }
    }
    
    [self.view addSubview:firView];
    [self.view addSubview:secView];
    
    timer = [NSTimer scheduledTimerWithTimeInterval:3 repeats:YES block:^(NSTimer * _Nonnull timer)
             {
                 if (firView.frame.origin.y == showFrame.origin.y)
                 {
                     //1.两个都向上滚动
                     [UIView animateWithDuration:0.5 animations:^
                      {
                          [firView setFrame:middleFrame];
                          [secView setFrame:showFrame];
                          
                      }
                                      completion:^(BOOL finished)
                      {
                          [firView setFrame:nextFrame];//准备展示
                      }];
                 }
                 else
                 {
                     [UIView animateWithDuration:0.5 animations:^
                      {
                          [firView setFrame:showFrame];
                          [secView setFrame:middleFrame];
                          
                      }
                                      completion:^(BOOL finished)
                      {
                          [secView setFrame:nextFrame];
                      }];
                 }
             }];
}
//点击后获取被点击的信息，可以进行页面跳转操作等。。。
- (void)labelTaped:(UITapGestureRecognizer *)gesture
{
    UILabel *label = (UILabel *)gesture.view;
    NSLog(@"label.tag == %ld",(long)label.tag);
}
```
