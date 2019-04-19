# iOS （UIButton封装）仿糯米首页缩放“按钮”效果 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年04月14日 08:59:59[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1011
# 前言
过年期间，少不了各种聚会，当下聚会大多数情况下自然是团购，然后就是用各种APP。。。使用度娘糯米时（不是广告，不是广告，不是广告！），偶然注意到了它的首页中一个有意思的效果，就是那些“按钮”点击时的缩放动画，有一种“按下去”的赶脚，恰逢前阵子做过一个录音按钮的“点动”效果，忍不住便封装了这个按钮效果：[GitHub](https://github.com/kukumaluCN/JXTPushInButton)
![](http://upload-images.jianshu.io/upload_images/1468630-9be6af6ae1397eea.gif?imageMogr2/auto-orient/strip)
动态缩放按钮，“按下去”的效果
> 
demo中的按钮背景图片截取自度娘糯米首页，仅供学习参考！
糯米首页应该是collectionView布局吧？也许，这里只是纯粹实现效果，封装了系统的按钮。
关于这个效果，首先不得不提一个概念——点动。
# 1.关于“点动”按钮
点动这一概念，我是从硬件那边搬过来的。
曾经做单片机时，按键是一个很重要的外设，硬件按键在编写代码实现功能时，一般有两种效果：带锁按钮，不带锁按钮。
所谓带锁按钮，就是点击按钮之后立刻松手，就能实现对应功能，例如我们曾经用过的按键手机的数字键都是这样的。
所谓不带锁按钮，也就是这里要说的“点动”，顾名思义，就是点击就动，松手即停。单片机那边一般在电机控制什么的场景经常用到，在我们软件这边，较普遍的一个应用场景就是“录音按钮”（微信语音神马的）。
录音按钮在点击时进行录音，一旦松手，录音就会停止。
为了便于说明，写了以下效果进行演示，button的`showsTouchWhenHighlighted`属性打开以指示按钮正在被点击中。
![](http://upload-images.jianshu.io/upload_images/1468630-5a209d97d7f7dee5.gif?imageMogr2/auto-orient/strip)
点动按钮效果
# 2.实现“点动”按钮
实现按钮的“点动”其实很简单：
```
//按下时
  [myButton addTarget:self 
               action:@selector(pressedEvent:) 
     forControlEvents:UIControlEventTouchDown];
  //松手后
  [myButton addTarget:self 
               action:@selector(unpressedEvent:) 
     forControlEvents:UIControlEventTouchUpInside | UIControlEventTouchUpOutside];
```
接下来在按钮的点击响应实现中：
`- (void)pressedEvent:(id)sender`
实现的是按钮按下的响应，例如录音开始并持续。
`- (void)unpressedEvent:(id)sender`
则实现的是松开按钮（停止点击）时的响应，此时执行结束录音。
> 
两个响应结合起来，也就实现了硬件按钮的“点动”效果（硬件按键是根据高低电平判定的，所以代码实现十分简单，对单片机有兴趣的朋友可以查阅相关资料，不过硬件按键按下时的电平“抖动”则是十分令人头疼的问题）。
这里还要提一点，就是`ControlEvents`，这里写的是`UIControlEventTouchUpInside | UIControlEventTouchUpOutside`，意思就是在按钮区域范围内或者范围外（按下之后，手指移动拖移）松手（停止点击），都会执行“松手响应”，这在一些情况下要注意区分，例如接下来要实现的“缩放按钮”。如果我们只写`UIControlEventTouchUpInside`，那么，我们在按钮区域范围外松手，也就是点击之后，手指拖动，移到按钮frame外，`unpressedEvent`响应是不执行的，我们从而也就实现了一个“取消”效果，当然这个不是用在录音按钮中的。
# 3.动画缩放按钮
前面啰嗦了一大堆，现在切入正题，其实，正题也就没什么要说的了，实现糯米的这一效果，无非还是上面的“动作拆分”思想，我们只要将按钮按下的缩放动画，以及动画执行之后的真正的响应，分别写在两个响应函数中就可以了。
```
//按钮的按下事件 按钮缩小
  - (void)pressedEvent:(JXTPushInButton *)btn
  {
      //缩放比例必须大于0，且小于等于1
      CGFloat scale = (_buttonScale && _buttonScale <=1.0) ? _buttonScale : defaultScale;//defaultScale默认为0.9
      [UIView animateWithDuration:animateDelay animations:^{
          btn.transform = CGAffineTransformMakeScale(scale, scale);
      }];
  }
  //按钮的松开事件 按钮复原 执行响应
  - (void)unpressedEvent:(JXTPushInButton *)btn
  {
      [UIView animateWithDuration:animateDelay animations:^{
          btn.transform = CGAffineTransformMakeScale(1.0, 1.0);
      } completion:^(BOOL finished) {
          //执行动作响应
          if (self.clickBlock) {
              self.clickBlock();
          }
      }];
  }
```
按钮按下真正要执行的响应，封装在了block中，button的封装详见我传到github中的代码，调用很方便，还是一句话：
```
JXTPushInButton * btn = [JXTPushInButton touchUpOutsideCancelButtonWithType:UIButtonTypeCustom frame:CGRectMake(0, 0, ScreenWidth - 20, 80) title:@"按钮-1" titleColor:[UIColor blackColor] backgroundColor:[UIColor redColor] backgroundImage:nil andBlock:^{
    NSLog(@"frame内部松手执，行按钮-1");
}];
[self.view addSubview:btn];
```
参考文章：
1.[UIButton---按住录音，松开停止](http://blog.csdn.net/wsxzk123/article/details/44620157)
2.[iOS下UIButton压下后播放动画，松开后动画消失的实现](http://www.cnblogs.com/janken/archive/2012/11/16/2773618.html)
# ·转载请注明出处·
文／霖溦（简书作者）
原文链接：http://www.jianshu.com/p/0704472878e9
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
