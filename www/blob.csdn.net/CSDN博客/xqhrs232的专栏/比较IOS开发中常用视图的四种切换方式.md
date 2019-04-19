# 比较IOS开发中常用视图的四种切换方式 - xqhrs232的专栏 - CSDN博客
2019年03月14日 17:42:31[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：20
原文地址::[https://blog.csdn.net/thanklife/article/details/78069921](https://blog.csdn.net/thanklife/article/details/78069921)
相关文章
1、iOS UIView之间常用视图之间切换方式----[https://blog.csdn.net/thanklife/article/details/78069968](https://blog.csdn.net/thanklife/article/details/78069968)
2、关于一个controller里面几个全屏view切换的问题-----[https://blog.csdn.net/winsdom123456/article/details/47105361?utm_source=blogkpcl11](https://blog.csdn.net/winsdom123456/article/details/47105361?utm_source=blogkpcl11)
在iOS开发中，比较常用的切换视图的方式主要有以下几种：
**1. push、pop**
使用举例（ViewController假设为需要跳转的控制器）：
[?](http://www.jb51.net/article/90833.htm#)
|1234|`[self.navigationController pushViewController:ViewController animated:YES]; ``//入栈，跳转到指定控制器视图``[self.navigationController popViewControllerAnimated:YES]; ``//弹栈，返回到前一个视图``[self.navigationController popToViewController:ViewController animated:YES]; ``//弹栈 返回指定控制器所在视图``[self.navigationController popToRootViewControllerAnimated:YES]; ``//弹栈 返回根控制器视图`|
**2. modal**
使用举例（ViewController假设为需要跳转的控制器）：
[?](http://www.jb51.net/article/90833.htm#)
|12|`[self presentViewController:ViewController animated:YES completion:nil]; ``//跳转到ViewController所在视图``[self dismissModalViewControllerAnimated:YES]; ``//退出当前视图`|
**3. 切换window的rootViewController**
使用举例（ViewController假设为需要跳转的控制器）：
[?](http://www.jb51.net/article/90833.htm#)
|12|`UIWindow *window = [UIApplication sharedApplication].keyWindow; ``//获得主窗口``window.rootViewController = [[ViewController alloc] init]; ``//将主窗口的根视图设置为当前控制器`|
**4. 将另一个控制器的视图添加为当前控制器视图的子视图**
[?](http://www.jb51.net/article/90833.htm#)
|1|`[self.view addSubview: ViewController.View]; ``//实现复杂界面的时候常用`|
**简要介绍下各种方式的优点和不足：**
**1、push、pop方式**
优点：控制器切换是可逆的，且原始图不会销毁
缺点：依赖于UINavigationController，故需要有一个导航控制器，与某些界面不需要导航控制器有可能会冲突
push操作是可逆的，即控制器的切换时可逆的。
**2、modal**
优点：控制器切换是可逆的，且原始图不会销毁
缺点：跳转没有push方式自由，dismiss方式只能按顺序返回，不能跳跃， 从前一个modal到后一个时候，前一个控制器不会消失，一直在内存中
**3、切换window的rootViewController**
优点：跳转直接迅速，而且可以销毁前一个控制器，因为将window的强引用改为当前控制器，故前一个控制器会消失（ARC环境）
缺点：视图跳转不可逆
**4、将一个控制器的视图添加到当前视图上面**
优点：通过一个控制器将另一控制器设置为属性，自由读取其他控制器中的视图从而能够实现复杂UI界面
缺点：复杂，多数场合不适用
**列表比较如下：**
|比较项目|push、pop|modal|切换窗口的根控制器|添加子视图|
|----|----|----|----|----|
|销毁源视图|否|否|是|否|
|使用场合|有导航控制器，需要能够返回前一个视图的情况|没有导航控制器的跳转，能返回前一个视图|版本新特性或者不需要保留前一个控制器的情况|需要实现复杂界面的情况|
|是否有导航控制器|是|否|否|否|
**总结**
视图切换的方式各有利弊，需要根据我们的需求选择使用的方式，以上就是本文的全部呢荣，水平有限，若有不正确之处，望各位批评指正，共同学习，共同提高！
原文链接：http://www.jianshu.com/p/0f33e6abf5f6
