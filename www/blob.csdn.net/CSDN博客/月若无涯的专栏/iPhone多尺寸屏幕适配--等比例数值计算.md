# iPhone多尺寸屏幕适配--等比例数值计算 - 月若无涯的专栏 - CSDN博客
2015年03月28日 15:15:38[月若无涯](https://me.csdn.net/u010124617)阅读数：2093
    作为一个iOS开放者，在iPhone6和iPhone6 plus发布之后，慢慢的都会遇到屏幕适配的问题。 
    在讨论适配之前，我们先来看看系统提供的放大模式。 
放大模式下：屏幕尺寸仍然为320宽度，会根据设备，界面等比放大至其他屏幕宽度的尺寸，由于是放大，会导致界面有轻微的模糊，并且状态栏也会和平时不一样（明显变大了）。
    那么适配模式下呢？不同尺寸设备的屏幕点像素是不一样的，如果要处理一个界面，并且要求iPhone6plus和iPhone5S是同一套设计稿，看起来一样比例。那么怎么破？
    无论是使用传统布局，还是使用AutoLayout，都避不开的是界面的数值计算，比如同一个左边距，在iPhone5下是10px，但是等比换算到iPhone6plus的时候差不多就是13px了，莫非为了不同屏幕看起来比例一样，我们还要分开处理，用条件来判断并分别布局么？
    当我开始这么想的时候，我是拒绝的，因为我们是爱加特效的开发者，怎么能连等比例计算都不会玩呢？
    好了拿到iPhone5s屏幕尺寸640*1136的设计稿，什么？左边距是20px？
```
CGFloat leftPadding = 10*SCREEN_WIDTH/320.0;
```
    LOOK! 我们已经有了一个可以在各个尺寸屏幕下按同一个设计稿等比例适应的左边距了！ 恩恩，还有很多值，一个个来…
```
CGFloat rightPadding = 15*SCREEN_WIDTH/320.0;
CGFloat topPadding = 20*SCREEN_WIDTH/320.0;
.....
```
    等等，这么算，貌似有很多重复的东西出现了：`SCREEN_WIDTH/320.0`，作为一个有追求的开发者，怎么能忍受那么多重复的计算。必须加特效！
    这里我为了便捷，使用类似CGRect之类C函数实现，考虑性能没有使用inline。建立一个空的UIView的category 
.h头文件中：
```
extern CGFloat CGFloatIn320(CGFloat value);
```
.m实现文件中：
```
static CGFloat ratio = 0;//这是为了只算一次不要浪费太多计算比例的性能
CGFloat CGFloatIn320(CGFloat value){
    if (ratio==0) {
        ratio = ([UIScreen mainScreen].bounds.size.width
        >[UIScreen mainScreen].bounds.size.height
        ?[UIScreen mainScreen].bounds.size.height
        :[UIScreen mainScreen].bounds.size.width)/320.0;
    }
    return value*ratio;
}
```
好了，现在再来计算我们的边距：
```
CGFloat leftPadding = CGFloatIn320(10);
CGFloat rightPadding = CGFloatIn320(15);
CGFloat topPadding = CGFloatIn320(20);
.....
```
    这样看起来真的是蛮不错啊，相信许多开发小伙伴也在用类似这样的方式做布局，总体上来说，这是要求不同尺寸相同比例布局的一个可行的方案，但是还是要多说两句：`这种方式下的计算有时候会存在很小很小的偏差`，虽然不多，但是有时候也会影响效果，所以，必要时的等分计算、自动布局的使用都是提高界面精确程度的方式。
    当然，这么简单的文章相信大家都能看得懂。最后贴上一篇别人家的文章，希望大家都能做好适配处理：  
[iPhone 6出现后,如何将一份设计稿支持多个尺寸？](http://mp.weixin.qq.com/s?__biz=MzA4NTQzNTMwOA==&mid=201174413&idx=3&sn=c3fe5b3459bac288e8ecafc9fb038a1d&scene=2&from=timeline&isappinstalled=0#rd)
2015.5.15更新 
我在实际开发中使用了这种方式，后来发现了一个额外的好处，就是可以通过控制比例尺来达到不同尺寸显示内容不一样多的处理，举例子： 
iPhone5下一行显示10个字，可能要求iPhone6下显示12个，iPhone6Plus下显示15个，那么其实我们可以通过设置这个scale的值来达到相应的目的。 
当然，针对分辨率等比例的值还是很有用处的。
