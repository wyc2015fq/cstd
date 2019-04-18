# Java e.getMessage 错误信息为null - z69183787的专栏 - CSDN博客
2016年12月16日 13:46:38[OkidoGreen](https://me.csdn.net/z69183787)阅读数：11725
**解决方案：用e.printStackTrace()e.getMessage()为null也很正常，NullPointException的getMessage()就为null我在抛出Exception时也可以直接  throw new MyException(); 这样getMessage()返回的就是null了**
这几天做的项目，[测试](http://lib.csdn.net/base/softwaretest)小组说测出来的异常日志有问题，没有把异常的信息打印出来，但是我回去看代码每一个catch语句里面都有getMessage呀，怎么getMessage会为空的呢？很是不懂其中的原因，之后上百度搜了下资料，觉得有下面一丢丢的总结，不知道正确不，不妥的地方希望各位大神帮我补充下![可怜](http://static.blog.csdn.net/xheditor/xheditor_emot/default/cute.gif)![可怜](http://static.blog.csdn.net/xheditor/xheditor_emot/default/cute.gif)
[Java](http://lib.csdn.net/base/javase)的异常关系图如下
：![](https://img-blog.csdn.net/20151209105540646?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
测试小组说的那个没有getMessage信息打印出来的异常就是一个nullPointerException异常，后面我又去动手试了下其他的几个runtime异常，发现getMessage都是为空的，之后我又去试了下SQLException和IOException，发现者两种异常的在catch的时候getMessage是不为null的。由此我觉得runtime异常发生的时候JVM调用的是父类无参的构造器
public Exception() {
        super();
    }
而SQLException和IOException异常发生的时候JVM调用的是父类有参的构造器
public Exception(String message) {
        super(message);
    }
所以SQLException和IOException的getMessage不为null，而runtime异常却为空。但是为什么SQLException和IOException调用的是有参的，而runtime异常调用的是无参，我还不知道具体原因，但是避免getMessage为null的话可以使用e.printStackTrace()或者logger.error("msg", e)把异常的信息打印出来。
