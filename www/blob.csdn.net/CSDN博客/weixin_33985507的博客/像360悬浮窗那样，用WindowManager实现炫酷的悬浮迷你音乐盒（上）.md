# 像360悬浮窗那样，用WindowManager实现炫酷的悬浮迷你音乐盒（上） - weixin_33985507的博客 - CSDN博客
2016年05月26日 22:34:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：10
![430632-3cf36e1a163cb3f7.png](https://upload-images.jianshu.io/upload_images/430632-3cf36e1a163cb3f7.png)
说到悬浮窗大家一定会想到这俩货
打开简信我的首页，亮闪闪的几个字**6天之前**，是的，距离我的上一篇文章已经6天，原计划这篇文章会是在周一晚上的时候发的，可是最近也有点突发事件，打乱我原有的计划。咳！反正就是瞎忙，不过我还是会坚持用心写好每一篇文章。
记得上一篇讲的是[用RotateDrawable实现网易云音乐唱片机效果](https://www.jianshu.com/p/0e0de2cdd2bb)，而今天我要讲的是如何用WindowManager去实现一个悬浮窗迷你音乐盒。，由于WindowManager这一块内容会比较多，所以我决定分成上下两篇来写这一块，这一篇主要介绍WindowManager和它的一些属性，可能会有些枯燥，下一篇我会具体去写悬浮迷你音乐盒的实现。
#### WindowManager
在Android应用开发中，其实整个Android的窗口机制是基于一个叫做WindowManager的一个系统服务接口，WindowManager可以添加view到屏幕，也可以从屏幕删除view。它面向的对象一端是屏幕，另一端就是View，其实就连我们常用的Activity和Diolog的底层实现都是通过WindowManager， WindowManager是全局的，整个系统就只用一个Windowmanager服务，我们需要向系统获取服务才能调用它，而它就是显示View的最底层。
其实WindowManager用起来非常方便，就三个方法：
###### 添加View
```
addView(View view, WindowManager.LayoutParams params);
```
从方法中我们可以看到，addView需要两个参数，view简单，就是我们要向窗口中去添加的对象，至于params，就是给窗口设置的显示策略，包括窗口的大小、透明度等等，这个也是今天文章的重点，在后文会有所介绍。
###### 移除View
```
removeView(View view);
```
既然能够向窗口去添加View，当然也就能够从窗口上移除View，这个很简单view就是你要从窗口中移除的对象。
###### 刷新View
```
updateViewLayout(View view, ViewGroup.LayoutParams params)
```
同样窗口刷新也需要两个参数，和添加View一样view是需要更新的对象，而params就是更新后的策略属性。
#### WindowManager.LayoutParams
相比于WindowManager，WindowManager.LayoutParams可就要复杂好多了。WindowManager.LayoutParams是 WindowManager 接口的嵌套类，在窗口管理中扮演着重要的角色。它继承于ViewGroup.LayoutParams，它用于向WindowManager描述窗口的管理策略；WindowManager.LayoutParams可以直接new WindowManager.LayoutParams()新建，也可以从对窗口的getAttributes()得到其WindowManager.LayoutParams对象。WindowManager.LayoutParams常用的有以下主要常量成员：
###### flag
- `WindowManager.LayoutParams.FLAG_SECURE` 不允许截屏；设置了这个属性的窗口，在窗口可见的情况下，是会禁用系统的截图功能的。那么问题来了：假如有一天，你的公司要求写一个类似于‘阅后即焚’功能的页面的话，不妨在activity中获得WindowManager.LayoutParams并添加该属性，轻轻松松搞定。
- `WindowManager.LayoutParams.FLAG_BLUR_BEHIND` 背景模糊；假如你的窗口设置了这个属性，并且这个窗口可见，在这窗口之后的所有背景都会被模糊化，但我还没有发现一个属性是可以控制模糊程度的。
- `WindowManager.LayoutParams.FLAG_DIM_BEHIND` 背景变暗；设置这个效果的窗口，在窗口可见的情况下，窗口后方的背景会相应的变暗，这个属性需要配合参数dimAmount一起使用，dimAmount会在后文中介绍。
- `WindowManager.LayoutParams.FLAG_FULLSCREEN` 设置全屏；这个属性也许是大家接触的最多的一个属性，很多应用开发过程中会要求有些页面需要动态设置Activity为全屏，而我们只需要获得Activity的WindowManager.LayoutParams并设置`WindowManager.LayoutParams.FLAG_FULLSCREEN`属性就行。
- `WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON` 设备常亮；设置这个属性的窗口，在窗口可见的情况下，整个屏幕会处于常亮并且高亮度的状态，并且不受待机时间的约束。
- `WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS` 布局不受限制；设置这个属性的窗口，将不再受设备显示范围边界 的约束，通俗点讲，就是窗口可以出设备之外，然后移除部分不可见。具体会在坐标参数中讲到。
- `WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE` 不设置聚焦；关于焦点获得我有必要说明一下，如果窗口获得焦点的话，只要窗口处于可视化状态，当前设备的物理按键点击事件都会被这个窗口接收，但是如果不设置窗口的焦点的话，直接传递到之后窗口进行接收。这就导致一个问题，如果你的需求要求你写的悬浮窗点击返回键能够关闭或是进行其他操作的话，你就必须让你的窗口获得焦点，并为当前View设置按键监听事件。
- `WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE` 取消触摸事件； 设置这个属性的窗口将不再处理任何Touch事件，就算显示的View设置了onTouch事件，那么这个窗口就会是一个僵尸窗口。
- `WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL` 不知道怎么去归纳，这个属性还是比较有意思的，设置这个属性的窗口，在窗口可见的情况下，就算窗口没有设置属性`FLAG_NOT_FOCUSABLE`，也就是在窗口获得焦点的情况下，当触摸事件是在窗口之外区域的时候，窗口不在拦截触摸事件，而是将事件往下传递，也算是解决聚焦后的事件拦截问题吧。
- `WindowManager.LayoutParams.FLAG_SHOW_WALLPAPER` 显示壁纸；官方文档说明是在窗口之后显示系统壁纸，但是我亲测，似乎并没有这个想效果，还是这个属性需要配合其他的属性设置一起使用，希望有设置成功的小伙伴能够在评论区分享你的结果。
- `WindowManager.LayoutParams.FLAG_SHOW_WHEN_LOCKED` 锁屏显示；关于这个属性官方文档给出的说明是在锁屏的时候显示的窗口，但是，实在惭愧，在下还是没有能够有一个实验结果，不知道是需要给权限呢还是需要同时进行其他设置。同样，还是很希望有知道的小伙伴能够在评论区向大家分享。
- `WindowManager.LayoutParams.FLAG_TURN_SCREEN_ON` 点亮屏幕；设置这个属性的窗口，当窗口显示的时候，如果设备处于待机状态，会点亮设备。这个应该在很多锁屏窗口中用的比较多，比如收到消息点亮屏幕。
- `WindowManager.LayoutParams.FLAG_WATCH_OUTSIDE_TOUCH` 这个也不知道怎么去归纳，也是一个比较有意思的属性，之前我们说到`FLAG_NOT_TOUCH_MODAL`，在窗口获得焦点的情况下，当触摸事件是在窗口之外区域的时候，窗口不在拦截触摸事件，而是将事件往下传递，而如果再设置这个属性，窗口能在`MotionEvent.ACTION_OUTSIDE`中收获窗口之外的点击事件，遗憾的是不能进行屏蔽，也就是说事件依然会向下传递。
以上的也是最常用到的几个flag属性了吧，其他还有很多，也希望大家空闲之余能够去研究研究，欢迎再评论区补充。
###### type
type主要用于表示window的类型。我们可以通过WindowManager.LayoutParams的type变量对窗口类型直接进行设置。常用的窗口类型也就以下两种：
- `WindowManager.LayoutParams.TYPE_APPLICATION_PANEL` 我在之前文章中介绍过的[PopupWindow](https://www.jianshu.com/p/825d1cc9fa79)，我也翻阅过PopupWindow的源码，PopupWindow用的就是`TYPE_APPLICATION_PANEL`这个属性类型。这种类型的窗口在显示寄生于宿主窗口，并显示与宿主窗口之上,因此这种类型的窗口会随着宿主窗口的关闭而关闭，显然不能满足我们悬浮窗的要求。
- `WindowManager.LayoutParams.TYPE_SYSTEM_ALERT`  系统提示窗口,常见的比如内存不够的警告、低电量警告。它总是出现在应用程序窗口之上，而这一点，正合我们做一个能够显示在任何应用之上的悬浮迷你音乐盒的要求。
###### screenBrightness、buttonBrightness
其中screenBrightness表示屏幕的亮度，而buttonBrightness表示一般按键和键盘按键的亮度。它们都拥有以下三个系统属性：
- `WindowManager.LayoutParams.BRIGHTNESS_OVERRIDE_OFF`   最低屏幕亮度。
- `WindowManager.LayoutParams.BRIGHTNESS_OVERRIDE_NONE`  默认屏幕亮度。
- `WindowManager.LayoutParams.BRIGHTNESS_OVERRIDE_FULL`  最高屏幕亮度。
###### dimAmount
讲flag属性的时候有提到过，这个参数是要和`WindowManager.LayoutParams.FLAG_DIM_BEHIND`这个flag属性一起使用，dimAmount的取值在0.0f~1.0f之间，取值越大背景的变暗程度越高，默认取值1.0f。
###### width、height
这里的width、height其实和View中的width、height一样的理解，就是控制窗口视图的大小，可以具体取值，也可以使用系统属性：
- `WindowManager.LayoutParams.WRAP_CONTENT` 自适应大小
- `WindowManager.LayoutParams.MATCH_PARENT` 填满整个布局
###### gravity
窗口的对齐方式，一般在创建窗口的时候，都会设置gravity为左上角对齐，也就是`Gravity.LEFT | Gravity.TOP`，因为窗口的坐标设置，是基于gravity来进行计算的，设置gravity左上角，刚好是和系统的坐标相对应，方便计算。
###### x、y
x和y用于控制窗口的坐标位置，如果有设置gravity的话，x和y设置的就是在gravity这个基础上的一个偏移量。不设置gravity的话，x和y就是一个绝对坐标。因此，将gravity设置为`Gravity.LEFT | Gravity.TOP`是最易于开发的。*需要注意的一点是：设置y的时候常常需要考虑状态栏的高度。*
正常情况下，就算x和y的坐标已经在设备之外，也会贴边显示。而如果设置属性`FLAG_LAYOUT_NO_LIMITS`则相对于系统的坐标如果x和y超出设备，那么超出部分将无法显示。
###### windowAnimations
windowAnimations控制的是窗口出现和消失的动画效果，设置的是要系统自带的动画效果（android.R.style之下的动画效果），因为窗口管理器是不能访问应用资源的。
###### format
format可以理解为最后窗口生成的位图是什么格式，默认背景是黑色的。一般我们都设置为PixelFormat.RGBA_8888，这样我们的窗口就会有一个透明的背景。
###### alpha
这个不难理解，设置窗口的透明度。
其实WindowManager.LayoutParams的属性有很多，全介绍一遍恐怕要讲到天亮，而且还有一些我本人也没有试过，要是还有什么比较实用或是比较有趣的属性，也欢迎小伙伴们在评论区留言！！   不胜感激！！！！
下篇预告：[像360悬浮窗那样，用WindowManager实现炫酷的悬浮迷你音乐盒（下）](https://www.jianshu.com/p/7bb5dbaff7de)，会有意想不到的惊喜。
> 
如果文中有表述不当或阐述错误的地方，还望正在看文章的您可以帮忙指出，有疑惑也可以在评论区提问或者私信，期待您的意见和建议，欢迎关注交流。
