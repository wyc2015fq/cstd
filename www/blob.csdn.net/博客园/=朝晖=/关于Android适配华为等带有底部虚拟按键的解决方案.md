# 关于Android适配华为等带有底部虚拟按键的解决方案 - =朝晖= - 博客园
# [关于Android适配华为等带有底部虚拟按键的解决方案](https://www.cnblogs.com/dhcn/p/7130822.html)
http://blog.csdn[.NET](http://lib.csdn.net/base/dotnet)/a91694451/article/details/50469857
最近公司的项目里遇到了一个问题，就是最后适配的时候时候同事的华为手机时，由于底部带有虚拟按键，所以正好遮住了底部导航栏的按钮以及其他子页面的一些存在于底部的功能。
也在网上找了些解决方案，但是不是很完美，下面说下我的解决步骤。
首先我找到了一个方案，说是在布局富容器中添加一句话
[Android](http://lib.csdn.net/base/android):fitsSystemWindows="true"
通过这句话让布局容器来适应手机屏幕，加上了之后，果然管用，底部导航栏确实在虚拟按键弹出时也跟随弹上来了，但是样式很丑，虚拟按键的背景色是为透明色，看起来很不美观，不像其他应用那样是黑色的。
在继续往下找，最后发现，是因为我在项目中设置了沉浸式通知栏导致的，取消掉了，果然好了，而且也不需要 [Android](http://lib.csdn.net/base/android):fitsSystemWindows="true"这句话就能完美适配了。
以上是我的解决方法，但是又有一个问题，就是沉浸式通知栏和底部虚拟按键不能共存么？如果有大神看到了，求解答。
还有一个办法，对界面的windows对象使用以下方法:
- getDialog().getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_ADJUST_RESIZE);  
