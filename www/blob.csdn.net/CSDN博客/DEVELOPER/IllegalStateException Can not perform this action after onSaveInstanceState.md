# IllegalStateException: Can not perform this action after onSaveInstanceState - DEVELOPER - CSDN博客





2014年07月28日 18:05:02[学术袁](https://me.csdn.net/u012827205)阅读数：1643标签：[android																[错误总结](https://so.csdn.net/so/search/s.do?q=错误总结&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android 调试错误总结](https://blog.csdn.net/u012827205/article/category/2135239)








![](https://img-blog.csdn.net/20140728174301610?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)













![](https://img-blog.csdn.net/20140728174143407?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


















































今天做项目的导航框架的时候遇到一个bug，框架如上图所示：

这个bug很奇怪，第一次进入运行。无论你怎么点击都OK！但是，当你退出去再进来的时候点击就会报上面的错误！![尴尬](http://static.blog.csdn.net/xheditor/xheditor_emot/default/awkward.gif)在网上找了找，像这样的一个错误会有千奇百怪的方式出现！但是，一句“***该操作不能执行在onSaveInstanceState()****之后***”却是一样的。像这种错误不是通用的，它相当于是一个很大的错误集，里面有很多报错的方式！

下面分析一下我的错误：

我的这个侧滑的左侧点击任一个item就会在右边位置显示该item对应的内容。这个效果我使用的是广播来实现的。经过我的一番努力，了解到：第一次进入的时候并没有什么错误，但是为什么第二次会错呢？？错误Can not perform this action after onSaveInstanceState原因是由于每当我第二次退出之后，所有的Activity都已经被kill掉并回收了。但是广播还是一直存在并时刻准备着接受他人发过来的广播。所以，当前Activity被K掉之后则立即调用方法：***onSaveInstanceState()，然后又在通过广播来进行fragment的入栈操作；***

*所以，解决方式就是在接受广播所在的Activity的onDestory()方法中解注册广播；*



*建一个群，方便大家交流：蹦蹦哒Android <群号：423923313>*







