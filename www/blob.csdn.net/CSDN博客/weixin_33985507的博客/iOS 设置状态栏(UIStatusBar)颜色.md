# iOS 设置状态栏(UIStatusBar)颜色 - weixin_33985507的博客 - CSDN博客
2016年09月24日 09:46:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
ios上状态栏 就是指的最上面的20像素高的部分
状态栏分前后两部分，要分清这两个概念，后面会用到：
前景部分：就是指的显示电池、时间等部分；
背景部分：就是显示黑色或者图片的背景部分；
注意：这里只涉及到ios7以及更高版本，低版本下面的讲解可能无效。
> 
- (UIStatusBarStyle)preferredStatusBarStyle:
在你自己的UIViewController里重写此方法，返回你需要的值(UIStatusBarStyleDefault 或者 UIStatusBarStyleLightContent)；即可。
