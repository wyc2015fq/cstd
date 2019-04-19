# FireBug的断点调试 - Big Smile - CSDN博客
2016年05月22日 17:53:30[王啸tr1912](https://me.csdn.net/tr1912)阅读数：2284
        Firebug是网页浏览器 Mozilla Firefox下的一款开发类插件， 现属于Firefox的五星级强力推荐插件之一。它集HTML查看和编辑、Javascript控制台、网络状况监视器于一体，是开发 JavaScript、CSS、HTML和Ajax的得力助手。Firebug如同一把精巧的瑞士军刀，从各个不同的角度剖析Web页面内部的细节层面， 给Web开发者带来很大的便利。
        以上为百度给出的介绍，但是这么好用的东西从下载了之后到现在就没有体会到他的好处，也不会对于页面进行简单的断点调试，经过一番查找和周末的时候师傅的指导才终于认识到了这个工具的强大之处。所以有此文来介绍一下FireBug的正确使用方法以及网页的页面代码的断点调试。
# 一、FireBug与浏览器自带管理的区别
        我们都知道了普通的浏览器在按下F12后也会出现一个能够显示样式，基本代码的界面，如图：
![](https://img-blog.csdn.net/20160522173925521?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20160522174110697?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20160522174339885?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        由上图我们可以很清楚的知道，浏览器自带的管理工具只有最基本的查看代码，测试性能（运行时间和占用）的用处，但是在看FireBug的图片会发现里面有一个脚本：
![](https://img-blog.csdn.net/20160522174536811?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        这个脚本就是我们加载在浏览器上的网页的原始执行代码的最终版，我们可以在里面的可触发的代码上面加上断点。
# 二、FireBug的断点调试
        FireBug的断点调试分为 断继、单步进入、单步跳过、单步退出这四个项。其中在断点调试的时候是需要先加断点的，加断点的方法就是在打开网页后进入脚本这个标签，然后在需要加断点的地方加上断点（方法和以前一样），最后刷新页面即可。如图：
![](https://img-blog.csdn.net/20160522174739220?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
注：一般用断点标记的都是执行代码，比如JS代码，VBS代码等等，且进入断点后界面上的内容都是不可以改变的，和IDE断点调试一样，个人推荐用FireFox的FireBug插件，比较全面，比较好用。
       结合着vs中的程序，我们就可以实现页面断点和后台代码断点的相互结合来解决一些bug问题。
# 三、总结
        这次使用FireBug真的是一次偶然，也是一次对于网页调试的一个开始，如果没有师傅和JS视频的提醒我可能还处于根本不知道FireBug可以调试的阶段，所以对于视频里面老师讲到的软件进行探索也是学习的一部分。
