# iOS仿支付宝/微信支付键盘输入demo - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年05月26日 13:09:15[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：3527
前段时间公司需要用到类似支付宝/微信支付的键盘输入界面,于是和同事写了一个,可能代码质量不是很高,但是也实现了功能,这里记录一下给广大 简书好友参考哈...
先给下demo的gitHub地址:(喜欢的话,star下吧)
[https://github.com/BrightMoment/D-Y/tree/master/仿微信:支付宝密码输入框/仿微信:支付宝密码输入框](https://github.com/BrightMoment/D-Y/tree/master/%E4%BB%BF%E5%BE%AE%E4%BF%A1:%E6%94%AF%E4%BB%98%E5%AE%9D%E5%AF%86%E7%A0%81%E8%BE%93%E5%85%A5%E6%A1%86/%E4%BB%BF%E5%BE%AE%E4%BF%A1:%E6%94%AF%E4%BB%98%E5%AE%9D%E5%AF%86%E7%A0%81%E8%BE%93%E5%85%A5%E6%A1%86)
1.先给大家看看截屏(简书刚玩还不知道怎么加入录屏哈)
![](http://upload-images.jianshu.io/upload_images/2120180-cead8156a9554418.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Simulator Screen Shot 2016年5月22日 19.27.53.png
2.简单介绍下代码实现
主要是利用了6个textField实现的,每当输入一位数字以后,就刷新6个textField的界面显示
下面给出核心代码实现:
```
-(void)allocPasswrod
{
    self.backView=[[UIView alloc] initWithFrame:CGRectMake(0, 0,SCREEN_WIDTH,SCREEN_HEIGHT)];
    self.backView.backgroundColor=[UIColor blackColor];
    [self.view addSubview:self.backView];
    self.backView.alpha=0;
    UITapGestureRecognizer*singleRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(hiddenBackVew)];
    singleRecognizer.numberOfTapsRequired = 1; // 单击
    [self.backView addGestureRecognizer:singleRecognizer];
    self.PhotoView=[[UIView alloc] initWithFrame:CGRectMake(10,SCREEN_HEIGHT, SCREEN_WIDTH-20, 100)];
    self.PhotoView.backgroundColor=[UIColor whiteColor];
    self.PhotoView.layer.cornerRadius=6;
    self.PhotoView.layer.masksToBounds=YES;
    [self.view addSubview:self.PhotoView];
#pragma 方法写得笨，勿喷
    la1= [[UITextField alloc] initWithFrame:CGRectMake((SCREEN_WIDTH-240)/2, 30, 40, 40)];
    la1.textAlignment= NSTextAlignmentCenter;
    la1.textColor=[UIColor blackColor];
    [la1 setSecureTextEntry:YES];
    la1.font=[UIFont systemFontOfSize:14.0f];
    la1.layer.borderWidth=1;
    [la1 setEnabled:NO];
    la1.layer.borderColor=[ColorHUI CGColor];
    [self.PhotoView addSubview:la1];
    la2= [[UITextField alloc] initWithFrame:CGRectMake((SCREEN_WIDTH-240)/2+40, 30, 40, 40)];
    la2.textAlignment= NSTextAlignmentCenter;
    la2.textColor=[UIColor blackColor];
    [la2 setSecureTextEntry:YES];
    la2.font=[UIFont systemFontOfSize:14.0f];
    la2.layer.borderWidth=1;
    [la2 setEnabled:NO];
    la2.layer.borderColor=[ColorHUI CGColor];
    [self.PhotoView addSubview:la2];
    la3= [[UITextField alloc] initWithFrame:CGRectMake((SCREEN_WIDTH-240)/2+80, 30, 40, 40)];
    la3.textAlignment= NSTextAlignmentCenter;
    la3.textColor=[UIColor blackColor];
    [la3 setSecureTextEntry:YES];
    la3.font=[UIFont systemFontOfSize:14.0f];
    la3.layer.borderWidth=1;
    [la3 setEnabled:NO];
    la3.layer.borderColor=[ColorHUI CGColor];
    [self.PhotoView addSubview:la3];
    la4= [[UITextField alloc] initWithFrame:CGRectMake((SCREEN_WIDTH-240)/2+120, 30, 40, 40)];
    la4.textAlignment= NSTextAlignmentCenter;
    la4.textColor=[UIColor blackColor];
    [la4 setSecureTextEntry:YES];
    la4.font=[UIFont systemFontOfSize:14.0f];
    la4.layer.borderWidth=1;
    la4.layer.borderColor=[ColorHUI CGColor];
    [la4 setEnabled:NO];
    [self.PhotoView addSubview:la4];
    la5= [[UITextField alloc] initWithFrame:CGRectMake((SCREEN_WIDTH-240)/2+160, 30, 40, 40)];
    la5.textAlignment= NSTextAlignmentCenter;
    la5.textColor=[UIColor blackColor];
    [la5 setSecureTextEntry:YES];
    la5.font=[UIFont systemFontOfSize:14.0f];
    la5.layer.borderWidth=1;
    la5.layer.borderColor=[ColorHUI CGColor];
    [la5 setEnabled:NO];
    [self.PhotoView addSubview:la5];
    la6= [[UITextField alloc] initWithFrame:CGRectMake((SCREEN_WIDTH-240)/2+200, 30, 40, 40)];
    la6.textAlignment= NSTextAlignmentCenter;
    la6.textColor=[UIColor blackColor];
    [la6 setSecureTextEntry:YES];
    la6.font=[UIFont systemFontOfSize:14.0f];
    la6.layer.borderWidth=1;
    la6.layer.borderColor=[ColorHUI CGColor];
    [la6 setEnabled:NO];
    [self.PhotoView addSubview:la6];
    la3.text=@"";
    la2.text=@"";
    la1.text=@"";
    la4.text=@"";
    la5.text=@"";
    la6.text=@"";
    self.keyBordView=[[UIView alloc] initWithFrame:CGRectMake(0,SCREEN_HEIGHT,SCREEN_WIDTH,200)];
    self.keyBordView.backgroundColor=[UIColor whiteColor];
    [self.view addSubview:self.keyBordView];
    NSArray * ary =[[NSArray alloc] initWithObjects:@"1",@"2",@"3",@"4",@"5",@"6",@"7",@"8",@"9",@"0",@"清除",@"x", nil];
    for(int i=0;i<ary.count;i++)
    {
        NSInteger index = i%3;
        NSInteger page = i/3;
        UIButton * btn =[UIButton buttonWithType:UIButtonTypeCustom];
        btn.frame = CGRectMake(index * (SCREEN_WIDTH/3), page  * 50,SCREEN_WIDTH/3,50);
        btn.tag=i;
        [btn setTitle:[ary objectAtIndex:i] forState:normal];
        [btn setTitleColor:[UIColor blackColor] forState:normal];
        btn.layer.borderColor=[ColorHUI CGColor];
        btn.layer.borderWidth=0.5;
        if(i<=9)
        {
            [btn addTarget:self action:@selector(KeyBoradClass:) forControlEvents:UIControlEventTouchUpInside];
        }
        if(i==10)
        {
            [btn addTarget:self action:@selector(KeyBoradClear:) forControlEvents:UIControlEventTouchUpInside];
        }
        if(i==11)
        {
            [btn addTarget:self action:@selector(KeyBoradRemove:) forControlEvents:UIControlEventTouchUpInside];
        }
        [self.keyBordView addSubview:btn];
    }
}
#pragma 清除
-(void)KeyBoradRemove:(UIButton *)btn
{
    if(passwordStr.length==0)
    {
        la3.text=@"";
        la2.text=@"";
        la1.text=@"";
        la4.text=@"";
        la5.text=@"";
        la6.text=@"";
        return;
    }
    NSString *str=[passwordStr substringToIndex:passwordStr.length-1];
    if(str.length==6)
    {
    }
    else if (str.length==5)
    {
        la6.text=@"";
    }
    else if(str.length==4)
    {
        la5.text=@"";
        la6.text=@"";
    }
    else if (str.length==3)
    {
        la4.text=@"";
        la5.text=@"";
        la6.text=@"";
    }
    else if(str.length==2)
    {
        la3.text=@"";
        la4.text=@"";
        la5.text=@"";
        la6.text=@"";
    }
    else if(str.length==1)
    {
        la3.text=@"";
        la2.text=@"";
        la4.text=@"";
        la5.text=@"";
        la6.text=@"";
    }
    else
    {
        la3.text=@"";
        la2.text=@"";
        la1.text=@"";
        la4.text=@"";
        la5.text=@"";
        la6.text=@"";
    }
    passwordStr=str;
    NSLog(@"%@",passwordStr);
}
#pragma 清空
-(void)KeyBoradClear:(UIButton *)btn{
    la3.text=@"";
    la2.text=@"";
    la1.text=@"";
    la4.text=@"";
    la5.text=@"";
    la6.text=@"";
}
#pragma 输入密码
-(void)KeyBoradClass:(UIButton *)btn
{
    NSArray * ary =[[NSArray alloc] initWithObjects:@"1",@"2",@"3",@"4",@"5",@"6",@"7",@"8",@"9",@"0",@"清除",@"x", nil];
    if (la1.text.length<1) {
        if (la1.text.length==0) {
            la1.text=[ary objectAtIndex:btn.tag];
        }
    }
    else if (la2.text.length<1 && la1.text.length==1) {
        if (la2.text.length==0) {
            la2.text=[ary objectAtIndex:btn.tag];
        }
    }
    else if (la3.text.length<1 && la2.text.length==1) {
        if (la3.text.length==0) {
            la3.text=[ary objectAtIndex:btn.tag];
        }
    }
    else if (la4.text.length<1 && la3.text.length==1) {
        if (la4.text.length==0) {
            la4.text=[ary objectAtIndex:btn.tag];
        }
    }
    else if (la5.text.length<1 && la4.text.length==1) {
        if (la5.text.length==0) {
            la5.text=[ary objectAtIndex:btn.tag];
        }
    }
    else if (la6.text.length<1 && la5.text.length==1) {
        if (la6.text.length==0) {
            la6.text=[ary objectAtIndex:btn.tag];
        }
    }
#pragma 密码输入己有6位
    passwordStr=[NSString stringWithFormat:@"%@%@%@%@%@%@",la1.text,la2.text,la3.text,la4.text,la5.text,la6.text];
    if(passwordStr.length==6)
    {
        NSLog(@"%@",passwordStr);
        passwordLable.text=passwordStr;
        [self hiddenBackVew];
    }
}
```
3.下面给出我的demo的gitHub地址,有需要的朋友友可以移步看下,喜欢的话给个star,鼓励一下吧...我会继续给大家带来好的demo的
[https://github.com/BrightMoment/D-Y/tree/master/仿微信:支付宝密码输入框/仿微信:支付宝密码输入框](https://github.com/BrightMoment/D-Y/tree/master/%E4%BB%BF%E5%BE%AE%E4%BF%A1:%E6%94%AF%E4%BB%98%E5%AE%9D%E5%AF%86%E7%A0%81%E8%BE%93%E5%85%A5%E6%A1%86/%E4%BB%BF%E5%BE%AE%E4%BF%A1:%E6%94%AF%E4%BB%98%E5%AE%9D%E5%AF%86%E7%A0%81%E8%BE%93%E5%85%A5%E6%A1%86)
文／D了个Y（简书作者）
原文链接：http://www.jianshu.com/p/5eed0b5ebfa4
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
