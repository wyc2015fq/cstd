# Log4j 记录error 日志 - z69183787的专栏 - CSDN博客
2015年06月25日 15:26:24[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1551
第一个bug的起始，是在线上日志发现一个频繁打印的异常——java.lang.ArrayIndexOutOfBoundsException。但是却没有堆栈，只有一行一行的ArrayIndexOutOfBoundsException。没有堆栈，不知道异常是从什么地方抛出来的，也就不能找到问题的根源，更谈不上解决。题外，工程师在用log4j记录错误异常的时候，我看到很多人这样用（假设e是异常对象）：
log.error(“发生错误:”+e);
或者：
log.error(“发生错误:”+e.getMessage());    这样的写法是不对，只记录了异常的信息，而没有将堆栈输出到日志，正确的写法是利用error的重载方法：
**log.error(“xxx发生错误”,e);**
这样才能在日志中完整地输出异常堆栈来。
使用这个方式，打印出的日志信息更加详细，所以使用这种方式。
