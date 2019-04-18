# java.lang.OutOfMemoryError:Java heap space的解决方法 - bigfacesafdasgfewgf - CSDN博客





2015年03月09日 10:19:38[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：835













出现这种报错的原因：JAVA的堆栈设置太小。




解决方法：设置JVM参数，让java程序在运行的时候，可以拥有更多的堆栈内存。




如下：




![](https://img-blog.csdn.net/20150309103545126?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





![](https://img-blog.csdn.net/20150309103518576?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)








在VM arguments中设置VM参数，也就是允许的堆栈内存大小为：-Xms1024m -Xmx1024m。




默认为64M，太小了，一些大的程序无法运行，我们这里设置为1024M，程序就可以顺利运行了。





















