# 牛逼的验证码，printf返回值 - xiahouzuoxin - CSDN博客





2013年07月09日 21:18:30[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：1807
个人分类：[ComputerLanuage](https://blog.csdn.net/xiahouzuoxin/article/category/1172320)









牛逼的验证码，如下图，

![](https://img-blog.csdn.net/20130709211445890?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhaG91enVveGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




结果是4321，为什么呢，主要是printf返回值问题？那么printf到底返回什么？

经查阅，printf的返回值是打印的字符个数，因此结果是4321就很明显了。



