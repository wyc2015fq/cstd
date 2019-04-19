# iOS-TabBar中间凸起点击完全有反应 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年05月30日 11:10:33[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：2273
- 先看下效果
![](http://upload-images.jianshu.io/upload_images/313229-63fd3633d2cbbc39.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
仿闲鱼.png
- 为什么还要继续说这个内容呢？
- 前一段微博上很火的tabbar封装大赛想必大家都知道吧，各位大神尽其所能
- 自己正在仿写闲鱼，看了网上一些朋友写的关于tabbar中间按钮的处理，发现关于处理中间按钮的点击这块有点模糊
- 个人估计是大家在写这一块的时候忽略了这个细节
- 有的是中间按钮凸起的部分点击没有反应，按钮其他地方可以点击
- 有的是中间凸起按钮可以完全点击了，但是没有做细节处理，导致push到其他页面，在和凸起按钮同样的位置还可以被点击，而且点击效果和点击凸起按钮效果是一样的 
- 还有的是虽然实现了功能，但是处理方法上并不是很完善
###### 因此决定熬夜写下这篇文章，帮助有需要的朋友看一下
- 
突然感觉没有什么可说的了，哈哈，主要是我代码里面已经写得非常详细了，而且文字多了效果也不好
- 
简单的说下大致结构和思路吧
- tabbar的话也是遵循主流，自定义一个继承自系统UITabbar的LBTabbar，然后用KVC和系统的进行替换
- 中间的凸起按钮和tabbar内部的子控件不是同一类型，是一个UIButton而已
- 根据tabbar内部子控件的类型去调整内部子控件的位置，从而腾出一个中间位置给凸起按钮
- 给tabbar弄一个代理，添加一个点击中间凸起按钮的代理方法，让LBTabBarController成为它的代理，实现对应代理方法即可实现按钮点击
- 如果对以上步骤有不清楚的地方可以看代码或者随时咨询我哦，这篇文字主要讲的核心就是中间按钮点击
1）要想监听整个发布按钮的点击，包括凸起部分点击也有反应，那么我是通过在自定义的LBTabbar内部重写`- (UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)even`方法来实现的
2）我们都知道，凸起按钮是自定义的LBTabbar的子控件，默认情况下子控件尺寸如果超出父控件，那么超出的部分点击是没有反应的
3）`hitTest`这个方法就是专门返回一个处理响应事件最合适view的，一般情况下我们不实现这个方法，默认就是让系统帮我们去判断处理事件响应最合适的view，一旦我们想要改变一下这种情况，我们就需要通过重写这个方法
4）我们的需求是只要我们点击的point在凸起按钮的任何位置(无论是否超出tabbar)都可以有响应，那么我们首先需要判断这个point是否在凸起按钮自身上
`[self convertPoint:point toView:self.plusBtn]`
这句代码就是将当前tabbar的触摸点转换坐标系，转换到凸起按钮的身上，它会生成一个新的点，然后我们通过
`[self.plusBtn pointInside:newP withEvent:event]`方法判断如果这个新的点是在发布按钮身上，那么处理点击事件最合适的view就是发布按钮，否则直接让系统帮我们处理点击事件就可以了
5）对了，这里还有一步也是非常关键，因为我们重写了寻找最合适view的方法，那么我们还需要考虑什么情况下我们需要由我们自己选择最合适的view，什么情况下不需要，所以我们需要加一个判断`if (self.isHidden == NO)`，
这句代码代表了当前页面是有tabbar的，那么肯定是在导航控制器的根控制器页面，这个时候就需要由我们自己选择最合适的view，其他的push页面直接让系统选择
6）如果不做第五步判断，bug就是由导航控制器的根控制器页面push到其他页面后，点击该页面和tabbar凸起按钮同样的位置也会有反应
7）好了，该上关键代码了
```
//重写hitTest方法，去监听发布按钮的点击，目的是为了让凸出的部分点击也有反应
- (UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)event {
    //这一个判断是关键，不判断的话push到其他页面，点击发布按钮的位置也是会有反应的，这样就不好了
    //self.isHidden == NO 说明当前页面是有tabbar的，那么肯定是在导航控制器的根控制器页面
    //在导航控制器根控制器页面，那么我们就需要判断手指点击的位置是否在发布按钮身上
    //是的话让发布按钮自己处理点击事件，不是的话让系统去处理点击事件就可以了
    if (self.isHidden == NO) {
        //将当前tabbar的触摸点转换坐标系，转换到发布按钮的身上，生成一个新的点
        CGPoint newP = [self convertPoint:point toView:self.plusBtn];
        //判断如果这个新的点是在发布按钮身上，那么处理点击事件最合适的view就是发布按钮
        if ( [self.plusBtn pointInside:newP withEvent:event]) {
            return self.plusBtn;
        }else{//如果点不在发布按钮身上，直接让系统处理就可以了
          return [super hitTest:point withEvent:event];
     }
   } 
    else {//tabbar隐藏了，那么说明已经push到其他的页面了，这个时候还是让系统去判断最合适的view处理就好了
        return [super hitTest:point withEvent:event];
    }
}
```
###### 下面是排布tabbar里面的子控件的
```
- (void)layoutSubviews
{
    [super layoutSubviews];
    //系统自带的按钮类型是UITabBarButton，找出这些类型的按钮，然后重新排布位置，空出中间的位置
    Class class = NSClassFromString(@"UITabBarButton");
    self.plusBtn.centerX = self.centerX;
    //调整发布按钮的中线点Y值
    self.plusBtn.centerY = self.height * 0.5 - 2*LBMagin ;
    self.plusBtn.size = CGSizeMake(self.plusBtn.currentBackgroundImage.size.width, self.plusBtn.currentBackgroundImage.size.height);
        UILabel *label = [[UILabel alloc] init];
        label.text = @"发布";
        label.font = [UIFont systemFontOfSize:11];
        [label sizeToFit];
        label.textColor = [UIColor grayColor];
        [self addSubview:label];
        label.centerX = self.plusBtn.centerX;
        label.centerY = CGRectGetMaxY(self.plusBtn.frame) + LBMagin ;
    int btnIndex = 0;
    for (UIView *btn in self.subviews) {//遍历tabbar的子控件
        if ([btn isKindOfClass:class]) {//如果是系统的UITabBarButton，那么就调整子控件位置，空出中间位置
            //每一个按钮的宽度==tabbar的五分之一
            btn.width = self.width / 5;
            btn.x = btn.width * btnIndex;
            btnIndex++;
            //如果是索引是2(从0开始的)，直接让索引++，目的就是让消息按钮的位置向右移动，空出来发布按钮的位置
            if (btnIndex == 2) {
                btnIndex++;
            }
        }
    }
}
```
- OK，结束了，不足之处欢迎大家指正，共同学习
[代码地址](https://github.com/LBCoderLee/iOS_XianYu)
###### 后续准备写一篇关于事件响应和传递的文章，如果对于hitTest方法不是很熟悉的朋友可以后续看看，个人感觉这块也是面试常客，感谢支持
文／小小小阿博er（简书作者）
原文链接：http://www.jianshu.com/p/46f61bc7a938
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
