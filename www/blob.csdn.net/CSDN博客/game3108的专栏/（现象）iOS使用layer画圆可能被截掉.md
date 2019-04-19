# （现象）iOS使用layer画圆可能被截掉 - game3108的专栏 - CSDN博客
2015年03月24日 16:52:50[game3108](https://me.csdn.net/game3108)阅读数：462
做项目中碰到这样一个问题：
UIView *test = [[UIViewalloc]initWithFrame:CGRectMake(50,50,10,10)];
    test.backgroundColor = [UIColorblackColor];
    test.layer.masksToBounds =YES;
    test.layer.cornerRadius =5;
    [self.viewaddSubview:test];
    [test release];
上面的代码就是画了一个10*10的一个圆，但如果把X坐标的50改为50.7（非整数），就会出现圆的左边有一点被截掉。（右边同理）
还在寻找原因，先记录下来，不知道是否有人能解释一下。
