# 初探Java中的异常处理 - bigfacesafdasgfewgf - CSDN博客





2015年03月12日 15:29:28[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：556










Java中的异常有下面几种：

1） Error：Java运行时的内部错误；

2） Exception：程序中应该捕获的异常。

         RuntimeException：由于编程产生的错误；

         IOException：输入输出产生的错误。



异常处理的方法：

**1）try-catch**

**![](https://img-blog.csdn.net/20150312153003852?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**



try语句块只能有一个，而catch可以有多个，catch必须紧跟try语句，中间不能有其他代码。如果有多个catch语句，那么捕获父类异常的catch语句必须放在后面，否则它会捕获它的所有子类异常，而使得子类异常catch语句永远不会执行。



**2）finally**

出现异常时，程序会跳出当前运行的语句块，找到异常捕获语句块，然后再跳回程序中执行catch语句。

不过有的时候在发生异常之后，有些语句是一定要执行的，这个时候可以用finally。它规定的语句块无论如何都要执行。在一个try-catch中只能有一个finally语句块。而且，一般情况下，finally语句块放在最后一个catch语句块之后。



**3）throws**

在方法中抛出异常，对异常的处理工作留给方法的调用者，在方法定义中抛出异常；而且，在调用方法时，必须要把方法调用语句放入try-catch 语句块中，并在catch中捕获相应的异常。







