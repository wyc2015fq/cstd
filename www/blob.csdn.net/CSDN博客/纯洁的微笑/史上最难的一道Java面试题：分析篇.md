# 史上最难的一道Java面试题：分析篇 - 纯洁的微笑 - CSDN博客
2018年09月06日 08:08:00[微笑很纯洁](https://me.csdn.net/ityouknow)阅读数：189
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/PgqYrEEtEnovnwcr7s635lKVQGQxia3wkR0c90BMKYib0067FtWBUvBEs0KJRgRQCsKb5VxGcAnSk22SCdmGkqrg/640?wx_fmt=jpeg)
作者
匠心零度
来源
http://www.jiangxinlingdu.com/thought/2017/10/16/thread-analysis.html
无意中了解到如下题目，觉得蛮好。
## **题目如下：**
```java
public class TestSync2 implements Runnable {
    int b = 100;          
    synchronized void m1() throws InterruptedException {
        b = 1000;
        Thread.sleep(500); //6
        System.out.println("b=" + b);
    }
    synchronized void m2() throws InterruptedException {
        Thread.sleep(250); //5
        b = 2000;
    }
    public static void main(String[] args) throws InterruptedException {
        TestSync2 tt = new TestSync2();
        Thread t = new Thread(tt);  //1
        t.start(); //2
        tt.m2(); //3
        System.out.println("main thread b=" + tt.b); //4
    }
    @Override
    public void run() {
        try {
            m1();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
```
## 该程序的输出结果？
## **程序输出结果**
main thread b=2000
b=1000
或
main thread b=1000
b=1000
## **考察知识点**
- 
synchronize实例锁。
- 
并发下的内存可见性。
> 
在java中，多线程的程序最难理解、调试，很多时候执行结果并不像我们想象的那样执行。所以在java多线程特别难，依稀记得大学的时候考c语言二级的时候，里面的题目是什么++和很多其他优先级的符合在一起问最后的输出结果，这类题目就想考一些运行符优先级和结合性问题。那个背背就行了，但是java多线程还是需要好好理解才行，靠背是不行的。
## **下面开始简单分析：**
该题目涉及到2个线程（主线程main、子线程）、关键词涉及到synchronized、Thread.sleep。
synchronized关键词还是比较复杂的（可能有时候没有理解到位所以上面题目会有点误区），他的作用就是实现线程的**同步**（实现线程同步有很多方法，它只是一种后续文章会说其他的，需要好好研究大神Doug Lea的一些实现），它的工作就是对需要同步的代码加锁，使得每一次只有一个线程可以进入同步块（其实是一种悲观策略）从而保证线程只记得安全性。
**一般关键词synchronized的用法**
- 
指定加锁对象：对给定对象加锁，进入同步代码前需要活的给定对象的锁。
- 
直接作用于实例方法：相当于对当前实例加锁，进入同步代码前要获得当前实例的锁。
- 
直接作用于静态方法：相当于对当前类加锁，进入同步代码前要获得当前类的锁。
上面的代码，synchronized用法其实就 属于第二种情况。直接作用于实例方法:相当于对当前实例加锁，进入同步代码前要获得当前实例的锁。
**可能存在的误区**
由于对synchronized理解的不到为，由于很多时候，我们多线程都是操作一个synchronized的方法，当2个线程调用2个不同synchronized的方法的时候，认为是没有关系的，这种想法是存在误区的。**直接作用于实例方法:相当于对当前实例加锁，进入同步代码前要获得当前实例的锁。**
如果一个调用synchronized方法。另外一个调用普通方法是没有关系的，2个是不存在等待关系的。
这些对于后面的分析很有作用。
**Thread.sleep**
使当前线程（即调用该方法的线程）暂停执行一段时间，让其他线程有机会继续执行，**但它并不释放对象锁。也就是说如果有synchronized同步快，其他线程仍然不能访问共享数据**。注意该方法要捕捉异常，对于后面的分析很有作用。
**分析流程**
java 都是从main方法执行的，上面说了有2个线程，但是这里就算修改线程优先级也没用，优先级是在2个程序都还没有执行的时候才有先后，现在这个代码一执行，主线程main已经执行了。
对于属性变量 int b =100由于使用了synchronized也不会存在可见性问题（也没有必要在说使用volatile申明）。
当执行**1步骤**的时候（Thread t = new Thread(tt); //1）线程是new状态，还没有开始工作。
当执行**2步骤**的时候（t.start(); //2）当调用start方法，这个线程才正真被启动，进入runnable状态，runnable状态表示可以执行，一切准备就绪了，但是并不表示一定在cpu上面执行，有没有真正执行取决服务cpu的调度。
在这里当执行**3步骤**必定是先获得锁（由于start需要调用native方法，并且在用完成之后在一切准备就绪了，但是并不表示一定在cpu上面执行，有没有真正执行取决服务cpu的调度，之后才会调用run方法，执行m1方法）。
这里其实2个synchronized方法里面的Thread.sheep其实要不要是无所谓的，估计是就为混淆增加难度。3步骤执行的时候其实很快子线程也准备好了，但是由于synchronized的存在，并且是作用同一对象，所以子线程就只有必须等待了。由于main方法里面执行顺序是顺序执行的，所以必须是步骤3执行完成之后才可以到**4步骤**，而由于3步骤执行完成，子线程就可以执行m1了。
**这里就存在一个多线程谁先获取到问题，如果4步骤先获取那么main thread b=2000，如果子线程m1获取到可能就b已经赋值成1000或者还没有来得及赋值4步骤就输出了可能结果就是main thread b=1000或者main thread b=2000**，在这里如果把**6步骤去掉**那么b=执行在前和main thread b=在前就不确定了。但是由于6步骤存在，所以不管怎么都是main thread b=在前面，那么等于1000还是2000看情况，之后b=1000是一定固定的了。
## **多线程一些建议**
- 
线程也很珍贵，所以建议使用线程池，线程池用的很多，后续准备分享下，特别总要，需要做到心中有数。
- 
给线程起名字，当线上cpu高的时候，需要用到高级jstack，如果有名称就方便很多。
- 
多线程特别需要注意线程安全问题，也需要了解jdk那些是线程安全不安全，那样使用的时候不会出现莫名其妙问题。
还有一些技巧后续文章分享在慢慢提，多线程特别重要，也特别难，希望大家也多多花心思在上面。
## **多线程的一些调试技巧**
由于断点，所有线程经过断点的时候，都需要停下，导致这个点不停的断住，很难受，eclispe里面有条件断点，当满足条件的时候就可以停下来，那么这样就方便了。
![640?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/wvkocF2MXjWuOaZ8zUpH616osxf1HrFau4iaCiczBwpdaXGpG5yYsVaangoDK0HqMmZibJowrwLGKsiaqeRMqPs32Q/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/wvkocF2MXjWuOaZ8zUpH616osxf1HrFaeNYDQPcqspTrUQZ2Ufg4b2QWZM7UtuV9lpnvIJfAxjVZ9hPiaYicRjBQ/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/wvkocF2MXjWuOaZ8zUpH616osxf1HrFa67iabIfbW68IicfeCCPt4CL4fzq3b112xSZN1ofKKAzTHxPbicSdpYcWA/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/wvkocF2MXjWuOaZ8zUpH616osxf1HrFaTHLvNzzDhtSZbUcWVs69DnxMF4dcicO69hvSHfFw5UesHdqC1cewzOQ/640?wx_fmt=png)
END
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/PgqYrEEtEnrnzUNX2KRVxAAzQcdevxOMnrGvPJ9mMeUR4XiatKR6F3auSmDICibxseIKoNLfo3fhiam8icfbSl4FTw/640?wx_fmt=jpeg)
