# 运行异常：android.view.WindowManager$BadTokenException:is not valid; is your activity running? - DEVELOPER - CSDN博客





2014年04月23日 18:11:45[学术袁](https://me.csdn.net/u012827205)阅读数：2260








在项目中经常会遇到各种各样的问题：

下面就分享一下我的问题：

![](https://img-blog.csdn.net/20140423175747937?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


这个项目使用的架构TabHost以实现像新浪微博一样的页面效果。在个人中心的那个tab页面，需求是：由在未登录的情况下，个人中心tab页面显示登录页面；然后登录成功之后需要在本页卡基础之上跳转到个人中心页面<保持下面的页卡tab不变>。

基于这个需求，我使用了ActivityGroup来控制。但是问题出现了，当我从登录页面登陆成功之后跳转到个人中心页面。有一Button控制着微博绑定的注销，当点击时候需要弹出一对话框来提示是否确定解绑！当点击解绑的时候就报错了，就是上面的错误提示。




下面给出错误原因：

之所以在该Activity弹出错误信息，是因为show处的对话框是依附于当前的Activity，即：AlertDialog.Builder dialog = new Builder(this)。但是当前的Activity是属于子类的Activity且是动态建立的，有随时被kill的能。所以，基于该子Activity弹出的对话框会不稳定，因此报错。


给出解决方法：



```java
//防止报错使用getParent()
AlertDialog.Builder dialog = new Builder(getParent());
```





