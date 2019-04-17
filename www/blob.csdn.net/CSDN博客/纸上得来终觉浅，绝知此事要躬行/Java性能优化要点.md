# Java性能优化要点 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年02月13日 15:46:57[boonya](https://me.csdn.net/boonya)阅读数：699标签：[java																[编码																[优化																[性能优化](https://so.csdn.net/so/search/s.do?q=性能优化&t=blog)
个人分类：[代码优化](https://blog.csdn.net/boonya/article/category/1714933)




- JIT / Inlining内联
- [GC垃圾回收](http://www.jdon.com/performance/java-performance-optimizations-gc.html)
- [False-sharing伪共享](http://www.jdon.com/performance/java-performance-optimizations-fs.html)
- [Unsafe](http://www.jdon.com/performance/java-performance-optimizations-unsafe.html)
- [Queues队列](http://www.jdon.com/performance/java-performance-optimizations-queue.html)

 From：[http://www.jdon.com/performance/java-performance-optimizations.html](http://www.jdon.com/performance/java-performance-optimizations.html)

## JITJava即时编译器

当Java执行runtime环境时，每遇到一个新的类，JIT[编译器](http://cpro.baidu.com/cpro/ui/uijs.php?c=news&cf=1001&ch=0&di=128&fv=16&jk=62c7535857efb83b&k=%B1%E0%D2%EB%C6%F7&k0=%B1%E0%D2%EB%C6%F7&kdi0=0&luki=5&n=10&p=baidu&q=banq_cpr&rb=0&rs=1&seller_id=1&sid=3bb8ef575853c762&ssp2=1&stid=0&t=tpclicked3_hc&tu=u1683405&u=http%3A%2F%2Fwww.jdon.com%2Fperformance%2Fjava-performance-optimizations.html&urlid=0)在此时就会针对这个类别进行编译（compile）被优化成相当精简的原生型指令码（native
 code），会做一下工作：
- 展开循环loop-unrolling
- 重新安排代码
- 移除同步synchronized
- 优化锁
- 内联热点方法

首先，JIT会展开我们代码中的循环语句，所以，我们编码时尽量注意不要在关键热点部分编写让JIT难于展开的循环语句。




JIT比较难以展开的循环语句如下：



```java
int i = 0;
for (;;) {
  if (array.length == i) {
    break;
  }
  doSomething(array[i++]);
}
```



这种for循环虽然编写方便，但是JIT不喜欢，下面循环则易于JIT展开:



```java
int i = 0;
for (int i = 0; i < array.length; i++) {
  doSomething(array[i]);
}
```



其次，JIT会内联一些热点小方法代码，这些小方法缺省差不多是325字节。比如下面是普通代码：



```java
public void methodA() {
  ... // Do some work A
  methodB();
}
 
private void methodB() {
  ... // Do some more work B
}
```



JIT会将methodB内联合并到methodA中



```java
//采取methodB内联到到methodA
public void methodA() {
  ... // Do some work A
  ... // Do some more work B
}
```



可以通过下面的Java运行配置记录[检测](http://cpro.baidu.com/cpro/ui/uijs.php?c=news&cf=1001&ch=0&di=128&fv=16&jk=62c7535857efb83b&k=%BC%EC%B2%E2&k0=%BC%EC%B2%E2&kdi0=0&luki=2&n=10&p=baidu&q=banq_cpr&rb=0&rs=1&seller_id=1&sid=3bb8ef575853c762&ssp2=1&stid=0&t=tpclicked3_hc&tu=u1683405&u=http%3A%2F%2Fwww.jdon.com%2Fperformance%2Fjava-performance-optimizations.html&urlid=0)内联：

java 

-XX:+PrintCompilation 

-XX:+UnlockDiagnosticVMOptions 

-XX:+PrintInlining 

.... > inline.log

PrintCompilation：当JIT编译发生输出打印

UnlockDiagnosticVMOptions：这是[标识](http://cpro.baidu.com/cpro/ui/uijs.php?c=news&cf=1001&ch=0&di=128&fv=16&jk=62c7535857efb83b&k=%B1%EA%CA%B6&k0=%B1%EA%CA%B6&kdi0=0&luki=3&n=10&p=baidu&q=banq_cpr&rb=0&rs=1&seller_id=1&sid=3bb8ef575853c762&ssp2=1&stid=0&t=tpclicked3_hc&tu=u1683405&u=http%3A%2F%2Fwww.jdon.com%2Fperformance%2Fjava-performance-optimizations.html&urlid=0) -XX:+PrintInlining需要的

-XX:+PrintInlining ：当方法被内联后[打印](http://cpro.baidu.com/cpro/ui/uijs.php?c=news&cf=1001&ch=0&di=128&fv=16&jk=62c7535857efb83b&k=%B4%F2%D3%A1&k0=%B4%F2%D3%A1&kdi0=0&luki=1&n=10&p=baidu&q=banq_cpr&rb=0&rs=1&seller_id=1&sid=3bb8ef575853c762&ssp2=1&stid=0&t=tpclicked3_hc&tu=u1683405&u=http%3A%2F%2Fwww.jdon.com%2Fperformance%2Fjava-performance-optimizations.html&urlid=0)出来

内联日志inline.log效果如下：

@ 42 io.netty.channel.nio.AbstractNioByteChannel$NioByteUnsafe::read (191 bytes) inline (hot) (这表示方法hot被内联了)

@ 42 io.netty.channel.nio.AbstractNioMessageChannel$NioMessageUnsafe::read (327 bytes) hot method too big (但是方法hot用于内联太大了)

@ 4 io.netty.channel.socket.nio.NioSocketChannel::config (5 bytes) inline (hot)

@ 1 io.netty.channel.socket.nio.NioSocketChannel::config (5 bytes) inline (hot)

@ 12 io.netty.channel.AbstractChannel::pipeline (5 bytes) inline (hot)

我们编码时对于热点方法不要编写对内联太大的方法，如下面read方法：



```java
private final class NioMessageUnsafe extends AbstractNioUnsafe {
  public void read() {
    final SelectionKey key = selectionKey();
    if (!config().isAutoRead()) {
      int interestOps = key.interestOps();
      if ((interestOps & readInterestOp) != 0) {
        // only remove readInterestOp if needed
        key.interestOps(interestOps & ~readInterestOp);
      }
    }
    ... // rest of the method
  }
  ...
}
```



分解出read() 方法一部分代码到新的方法中：



```java
private final class NioMessageUnsafe extends AbstractNioUnsafe {
  public void read() {
    if (!config().isAutoRead()) {
      removeReadOp();
    }
 
  private void removeReadOp() {
    SelectionKey key = selectionKey();
    int interestOps = key.interestOps();
    if ((interestOps & readInterestOp) != 0) {
      // only remove readInterestOp if needed
      key.interestOps(interestOps & ~readInterestOp);
    }
  }
 
    ... // rest of the method
  }
  ...
```



注意到read方法从原来多行已经变成了简单几行，这时我们再看看JIT的内联日志：

@ 42 io.netty.channel.nio.AbstractNioMessageChannel$NioMessageUnsafe::read (288 bytes) inline (hot)

只有一行输出，说明read方法已经小到适合内联了。](https://so.csdn.net/so/search/s.do?q=优化&t=blog)](https://so.csdn.net/so/search/s.do?q=编码&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)




