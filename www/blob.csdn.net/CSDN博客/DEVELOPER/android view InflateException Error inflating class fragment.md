# android.view.InflateException:Error inflating class fragment - DEVELOPER - CSDN博客





2014年08月04日 17:56:26[学术袁](https://me.csdn.net/u012827205)阅读数：1256标签：[Error inflateing cla																[InflateException](https://so.csdn.net/so/search/s.do?q=InflateException&t=blog)](https://so.csdn.net/so/search/s.do?q=Error inflateing cla&t=blog)
个人分类：[android.view.InflateException:Error inflating class fragment](https://blog.csdn.net/u012827205/article/category/2453607)








![](https://img-blog.csdn.net/20140804174655584?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




错误分析：首先我的编码方式是在一个Activity中引入fragment，即：通过在Activity的布局中加入要引入的fragment标签，通过对Fragment类的操作来改变对应的Activity的内容；但是，感觉一切都好了却出现了*“类转换异常*”<Class CastException:xxx.class cannot be cast to android.app.Fragment>的错误！

*想了一想才发现，这样就能解决了：在要引入fragment的Activity中类要继承FragmentActivity！！*








