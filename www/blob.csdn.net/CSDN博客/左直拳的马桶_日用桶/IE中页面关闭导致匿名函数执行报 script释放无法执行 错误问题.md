# IE中页面关闭导致匿名函数执行报"script释放无法执行"错误问题 - 左直拳的马桶_日用桶 - CSDN博客
2017年12月14日 19:33:07[左直拳](https://me.csdn.net/leftfist)阅读数：193标签：[ie																[javascript](https://so.csdn.net/so/search/s.do?q=javascript&t=blog)](https://so.csdn.net/so/search/s.do?q=ie&t=blog)
个人分类：[JavaScript](https://blog.csdn.net/leftfist/article/category/94500)
在IE中，在页面p1上传递匿名函数到另一张页面p2，然后关闭页面p1，则执行传递过去的匿名函数，会报“Script已释放，无法再执行”的错误。而这种情形，在chrome上则不会发生。
![这里写图片描述](https://img-blog.csdn.net/20171214192554612?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
比如上图，在iframe里传递匿名函数给容器页面的函数A进行执行。当这个iframe的页面被替换，这时再执行容器页面函数A，就会报上面说的错。
我暂时找不到好的办法，只能 
1、执行匿名函数的时候try catch；catch的时候将匿名函数置为空
2、iframe页面unload的时候，执行打扫工作
3、如果匿名函数被传递过去后，绑定到了事件，则打扫工作应包括解绑。
