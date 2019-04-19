# 设计模式学习笔记——Mediator中介者模式 - 左直拳的马桶_日用桶 - CSDN博客
2017年03月13日 07:57:45[左直拳](https://me.csdn.net/leftfist)阅读数：403
将众多对象之间的网状关系转为全部通过一个中间对象间接发生关系，此中间对象为中介者。
看图最直观： 
![这里写图片描述](https://img-blog.csdn.net/20170313075327989?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170313075341942?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170313075548602?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
作用不言而喻，就是降低对象之间的耦合度，乃至降低了整个系统的复杂度。
有点象代理模式，更象外观模式： 
![这里写图片描述](https://img-blog.csdn.net/20170313075951857?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
