# IOS动画Core Animation详解 - game3108的专栏 - CSDN博客
2015年09月07日 14:58:44[game3108](https://me.csdn.net/game3108)阅读数：272
个人分类：[转载																[iOS](https://blog.csdn.net/game3108/article/category/2844789)](https://blog.csdn.net/game3108/article/category/2926393)
转载保留原文地址：[http://blog.csdn.net/kqjob/article/details/10417461](http://blog.csdn.net/kqjob/article/details/10417461)
在IOS中如果使用普通的动画则可以使用UIKit提供的动画方式来实现，如果想实现更复杂的效果，则需要使用Core Animation了。
下面详解各种类型动画的使用方式
### **1、通过动画上下文使用UIKit动画**
**[plain]**[view
 plain](http://blog.csdn.net/kqjob/article/details/10417461#)[copy](http://blog.csdn.net/kqjob/article/details/10417461#)
- -(void)animationOfUIKit  
- {  
-     UIView *redView=[[UIView alloc]initWithFrame:CGRectMake(10, 10, 100, 100)];  
-     redView.backgroundColor=[UIColor redColor];  
- 
-     [self.view addSubview:redView];  
-     //开始动画  
-     [UIView beginAnimations:@"test" context:nil];  
-     //动画时长  
-     [UIView setAnimationDuration:1];  
-     /*  
-      *要进行动画设置的地方  
-      */  
- 
-     redView.backgroundColor=[UIColor blueColor];  
-     redView.frame=CGRectMake(50, 50, 200, 200);  
-     redView.alpha=0.5;  
- 
- 
-     //动画结束  
-     [UIView commitAnimations];  
- }  
### 2、通过代码块使用UIKit动画
**[plain]**[view
 plain](http://blog.csdn.net/kqjob/article/details/10417461#)[copy](http://blog.csdn.net/kqjob/article/details/10417461#)
- -(void)animationOfBlock  
- {  
-     //初始化一个View，用来显示动画  
-     UIView *redView=[[UIView alloc]initWithFrame:CGRectMake(10, 10, 100, 100)];  
-     redView.backgroundColor=[UIColor redColor];  
- 
-     [self.view addSubview:redView];  
- 
-     [UIView animateWithDuration:1 //时长  
-                           delay:0 //延迟时间  
-                         options:UIViewAnimationOptionTransitionFlipFromLeft//动画效果  
-                      animations:^{  
- 
-                          //动画设置区域  
-                          redView.backgroundColor=[UIColor blueColor];  
-                          redView.frame=CGRectMake(50, 50, 200, 200);  
-                          redView.alpha=0.5;  
- 
-                      } completion:^(BOOL finish){  
-                        //动画结束时调用  
-                        //............  
-                      }];  
- 
- 
- }  
## 使用Core Animation对象来实现动画
在Core Animation中我们经常使用的是
- CABasicAnimation
- CAKeyframeAnimation
- CATransitionAnimation
其中**CABasicAnimation**和**CAKeyframeAnimation**是对图层中的不同属性进行动画的。
如果要多整个图层进行动画，则应该使用**CATransitionAnimation**
如果要使用组合动画，例如要改变图层的大小和透明度，则可以先为每个属性创建一个**CABasicAnimation**对象，再把他们组合到**CAAnimationGroup**中，最后把这个组合添加到要进行动画的**CALayer**中。
注：CAAnimation（以及CAAnimation的子类）,全部都是**显式动画**，这样动画播放后，表现层回恢复到模型层的原始状态，这就意味着，如果动画播放完后，会恢复到原来的样子，所以在动画播放完后要对模型层进行修改，例如：self.view.layer.backgroundColor=[UIColor blueColor].CGColor;
### 1、自定义动画：**CABasicAnimation**
**[plain]**[view
 plain](http://blog.csdn.net/kqjob/article/details/10417461#)[copy](http://blog.csdn.net/kqjob/article/details/10417461#)
- -(void)animationOfCABasicAnimation  
- {  
-     //创建一个CABasicAnimation对象  
-     CABasicAnimation *animation=[CABasicAnimation animation];  
-     //设置颜色  
-     animation.toValue=(id)[UIColor blueColor].CGColor;  
-     //动画时间  
-     animation.duration=1;  
-     //是否反转变为原来的属性值  
-     animation.autoreverses=YES;  
-     //把animation添加到图层的layer中，便可以播放动画了。forKey指定要应用此动画的属性  
-     [self.view.layer addAnimation:animation forKey:@"backgroundColor"];  
- 
- }  
### 2、关键帧动画：CAKeyframeAnimation
1. path
这是一个 CGPathRef  对象，默认是空的，当我们创建好CAKeyframeAnimation的实例的时候，可以通过制定一个自己定义的path来让  某一个物体按照这个路径进行动画。这个值默认是nil  当其被设定的时候  values  这个属性就被覆盖 
2. values
一个数组，提供了一组关键帧的值，  当使用path的 时候 values的值自动被忽略。
下面是改变依次改变view的颜色
**[plain]**[view
 plain](http://blog.csdn.net/kqjob/article/details/10417461#)[copy](http://blog.csdn.net/kqjob/article/details/10417461#)
- -(void)animationOfCAKeyframeAnimation  
- {  
-     CAKeyframeAnimation *animation=[CAKeyframeAnimation animation];  
-     //设置属性值  
-     animation.values=[NSArray arrayWithObjects:  
-                       (id)self.view.backgroundColor,  
-                       (id)[UIColor yellowColor].CGColor,  
-                       (id)[UIColor greenColor].CGColor,  
-                       (id)[UIColor blueColor].CGColor,nil];  
-     animation.duration=3;  
-     animation.autoreverses=YES;  
-     //把关键帧添加到layer中  
-     [self.view.layer addAnimation:animation forKey:@"backgroundColor"];  
- }  
### 3、使用路径制作动画：CAKeyframeAnimation
**[plain]**[view
 plain](http://blog.csdn.net/kqjob/article/details/10417461#)[copy](http://blog.csdn.net/kqjob/article/details/10417461#)
- -(void)animationOfCAKeyframeAnimationPath  
- {  
-     //初始化一个View，用来显示动画  
-     UIView *redView=[[UIView alloc]initWithFrame:CGRectMake(10, 10, 20, 20)];  
-     redView.backgroundColor=[UIColor redColor];  
- 
-     [self.view addSubview:redView];  
- 
-     CAKeyframeAnimation *ani=[CAKeyframeAnimation animation];  
-     //初始化路径  
-     CGMutablePathRef aPath=CGPathCreateMutable();  
-     //动画起始点  
-     CGPathMoveToPoint(aPath, nil, 20, 20);  
-     CGPathAddCurveToPoint(aPath, nil,   
-                           160, 30,//控制点  
-                           220, 220,//控制点   
-                           240, 380);//控制点  
- 
-     ani.path=aPath;  
-     ani.duration=10;  
-     //设置为渐出  
-     ani.timingFunction=[CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseIn];  
-     //自动旋转方向  
-     ani.rotationMode=@"auto";  
- 
-     [redView.layer addAnimation:ani forKey:@"position"];  
- }  
全文手敲，暂时就写到这里了，以后有时间再添加内容，有写的不好的地方欢迎指出来。
