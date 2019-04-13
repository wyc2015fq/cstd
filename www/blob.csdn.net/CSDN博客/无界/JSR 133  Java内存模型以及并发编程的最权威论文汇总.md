
# JSR 133  Java内存模型以及并发编程的最权威论文汇总 - 无界 - CSDN博客

置顶2019年03月24日 01:26:16[21aspnet](https://me.csdn.net/21aspnet)阅读数：259



# Java内存模型
先看官方文档：
[https://docs.oracle.com/javase/specs/](https://docs.oracle.com/javase/specs/)
![](https://img-blog.csdnimg.cn/2019032401360596.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
JSR 133：Java TM内存模型和线程规范修订版：[https://www.jcp.org/en/jsr/detail?id=133](https://www.jcp.org/en/jsr/detail?id=133)
![](https://img-blog.csdnimg.cn/20190324013225946.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
JSR：Java规范请求所有JSR的列表：[https://jcp.org/en/jsr/all](https://jcp.org/en/jsr/all)
[https://www.jcp.org/en/home/index](https://www.jcp.org/en/home/index)
JSR是Java Specification Requests的缩写，意思是Java 规范请求。是指向JCP(Java Community Process)提出新增一个标准化技术规范的正式请求。任何人都可以提交JSR，以向Java平台增添新的API和服务。JSR已成为Java界的一个重要标准。
![](https://img-blog.csdnimg.cn/20190325123003788.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
----------
![](https://img-blog.csdnimg.cn/20190324013319122.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
JSR-000133 Java TM内存模型和线程规范修订版**（最终版本）：**
[https://www.jcp.org/aboutJava/communityprocess/final/jsr386/index.html](https://www.jcp.org/aboutJava/communityprocess/final/jsr386/index.html)
![](https://img-blog.csdnimg.cn/20190324013402816.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
[http://www.cs.umd.edu/~pugh/jmm.pdf](http://www.cs.umd.edu/~pugh/jmm.pdf)
[http://openjdk.java.net/jeps/188](http://openjdk.java.net/jeps/188)
![](https://img-blog.csdnimg.cn/20190325122237835.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
该JEP的产品将被放置在[OpenJDK Wiki上](https://wiki.openjdk.java.net/)。该过程主要在专门的OpenJDK邮件列表上进行。我们希望结果包括以下内容：
改进了形式化。将重新制定基础模型的部分内容。我们的目标是修改后的模型可以进行机械检查，并且更易于人类理解。当根据JLS第17章更新措辞时，这也将解决许多学术论文中指出的现有错误。（最早见 David Aspinall和JaroslavŠevčík撰写的[“Java内存模型示例：好，坏，丑”](http://groups.inf.ed.ac.uk/request/jmmexamples.pdf)）
JVM覆盖范围。现有规范侧重于语言级构造。这使得一些问题（例如初始化）未完全定义，特别是对于在JVM上运行的其他语言。这些将通过将核心模型建立在最小字节代码和内在函数集上来解决。
扩展范围。现有规范明确涵盖Java线程，锁，监视器以及volatile和final字段。但是，自Java SE 5以来，添加的功能无法在这些术语中严格指定（例如，AtomicX.weakCompareAndSet）。必须解决这些问题。我们还预计在其他即将到来的JEP过程中可能会出现进一步的扩展。
C11 / C ++ 11兼容性。C ++ 11和C11标准改编了JSR 133 JMM规范工作的想法。但是，他们还扩展了它们以涵盖仅在JSR 133（见上文）之后已经（或可能）添加到Java的构造。部分原因是因为Java程序可能调用C本机库，所以应该是等效结构具有跨语言的兼容规范的情况。我们将进一步探索是否可以建立跨语言约定以确保这些构造的低级实现在公共平台上兼容。
实施指南。JVM实现者，JDK库开发人员和开发人员通常发现依赖于解释JMM如何影响特定问题和解决方案的文档很有用。我们打算提供这样的文件。
测试支持。符合内存模型要求很难测试。我们希望与工程师一起设计和实施具有明确规格基础的测试。
工具支持。重新构建的模型将适合由软件开发工具使用，这些工​​具可以分析性地检查竞争条件等错误，以及检查安全属性是否跨越并发执行的错误。虽然工具本身的设计和构造超出了范围，但是这个JEP可以为注释提供指导，以实现高质量的静态和动态分析。
------------------------------------
The Java Memory Model：[http://www.cs.umd.edu/users/pugh/java/memoryModel/](http://www.cs.umd.edu/users/pugh/java/memoryModel/)
# Java内存模型
该网页是讨论Java内存模型和[Java语言规范](http://java.sun.com/docs/books/jls/)[第17章的](http://java.sun.com/docs/books/jls/third_edition/html/memory.html)信息的起点。Java Memory Model定义了线程如何通过内存进行交互。过去有点不清楚和不必要的限制，因此进行了修订。这是该修订的参考页面。JSR 133的官方站点 - Java（tm）内存模型和线程规范修订版 -[就在这里](http://www.jcp.org/en/jsr/detail?id=133)。
此页面分为几个部分：
内存模型上的[主要参考资料](#reference)。
[指向邮件列表和档案的指针](http://www.cs.umd.edu/~pugh/java/memoryModel/index.html#mailing)。
[内存模型上的其他材料，包括有关双重检查锁定的信息](http://www.cs.umd.edu/~pugh/java/memoryModel/index.html#additional)。
内存模型上的[旧材料](http://www.cs.umd.edu/~pugh/java/memoryModel/index.html#older)现已过时。
[指示进一步阅读](http://www.cs.umd.edu/~pugh/java/memoryModel/index.html#bib)其他来源的[材料](http://www.cs.umd.edu/~pugh/java/memoryModel/index.html#bib)。
---

### 参考资料
对于试图理解内存模型的人来说，这些参考资料是一个很好的起点。在它们之间，它们涵盖了所涉及的大多数主要问题。
对于初次访客一个[ 程序员JSR-133 FAQ资源](http://www.cs.umd.edu/~pugh/java/memoryModel/jsr-133-faq.html)是可用的。对于那些刚刚意识到这些问题的人来说，这是一个很好的起点。（2004年2月11日）

对于JVM和编译器实现者[Doug Lea的JSR-133烹饪书](http://gee.cs.oswego.edu/dl/jmm/cookbook.html)，是希望实现Java内存模型的编译器编写者的指南。
Sarita Adve和Kourosh Gharachorloo在1995年撰写了关于记忆模型的教程，这仍然是一本很好的参考和入门书。Compaq Research Report 95/7，1995年9月，[95.7 - 共享内存一致性模型：教程](http://research.compaq.com/wrl/techreports/abstracts/95.7.html)。

对于那些希望完全了解内存模型的人关于记忆模型的[ 期刊提交](http://dl.dropbox.com/u/1011627/journal.pdf)，结合了Jeremy Manson的论文，POPL论文和CSJP论文。对于那些对内存模型问题有深入讨论感兴趣的人来说，这是最好的选择。（2005年10月7日）。
[JSR-133规范，发送到最终批准选票](http://www.cs.umd.edu/~pugh/java/memoryModel/jsr133.pdf)。这是“官方规范”（2004年8月9日）。它的解释方式并不多。
[最终字段语义的新表示/描述](http://www.cs.umd.edu/~pugh/java/memoryModel/may-12.pdf)。这是最终字段语义的简要描述。（2004年5月12日）
JSR 133（Java内存模型）常见问题解答：[https://www.cs.umd.edu/~pugh/java/memoryModel/jsr-133-faq.html](https://www.cs.umd.edu/~pugh/java/memoryModel/jsr-133-faq.html)
## JSR 133（Java内存模型）常见问题解答
## Jeremy Manson和Brian Goetz，2004年2月
### 目录
[无论如何，什么是记忆模型？](https://www.cs.umd.edu/~pugh/java/memoryModel/jsr-133-faq.html#whatismm)
[其他语言，比如C ++，有内存模型吗？](https://www.cs.umd.edu/~pugh/java/memoryModel/jsr-133-faq.html#otherlanguages)
[什么是JSR 133？](https://www.cs.umd.edu/~pugh/java/memoryModel/jsr-133-faq.html#jsr133)
[重新排序是什么意思？](https://www.cs.umd.edu/~pugh/java/memoryModel/jsr-133-faq.html#reordering)
[旧的记忆模型出了什么问题？](https://www.cs.umd.edu/~pugh/java/memoryModel/jsr-133-faq.html#oldmm)
[错误同步是什么意思？](https://www.cs.umd.edu/~pugh/java/memoryModel/jsr-133-faq.html#incorrectlySync)
[同步有什么作用？](https://www.cs.umd.edu/~pugh/java/memoryModel/jsr-133-faq.html#synchronization)
[最终字段如何显示更改其值？](https://www.cs.umd.edu/~pugh/java/memoryModel/jsr-133-faq.html#finalWrong)
[最终字段如何在新的JMM下工作？](https://www.cs.umd.edu/~pugh/java/memoryModel/jsr-133-faq.html#finalRight)
[挥发物有什么作用？](https://www.cs.umd.edu/~pugh/java/memoryModel/jsr-133-faq.html#volatile)
[新内存模型是否修复了“双重检查锁定”问题？](https://www.cs.umd.edu/~pugh/java/memoryModel/jsr-133-faq.html#dcl)
[如果我正在编写VM怎么办？](https://www.cs.umd.edu/~pugh/java/memoryModel/jsr-133-faq.html#vmwriters)
[我为什么要在乎？](https://www.cs.umd.edu/~pugh/java/memoryModel/jsr-133-faq.html#conclusion)
------------
# [“双重检查锁定”声明](http://www.cs.umd.edu/~pugh/java/memoryModel/DoubleCheckedLocking.html)
Double-Checked Locking被广泛引用并用作在多线程环境中实现延迟初始化的有效方法。
不幸的是，当在Java中实现时，它将无法以独立于平台的方式可靠地工作，而无需额外的同步。当用其他语言（如C ++）实现时，它取决于处理器的内存模型，编译器执行的重新排序以及编译器和同步库之间的交互。由于这些都不是用C ++这样的语言指定的，因此对它的工作情况几乎没有什么可说的。可以使用显式内存屏障使其在C ++中工作，但这些障碍在Java中不可用。
![](https://img-blog.csdnimg.cn/20190324014922448.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190324014137297.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
-----------------------
# [同步和Java内存模型](http://gee.cs.oswego.edu/dl/cpj/jmm.html)
![](https://img-blog.csdnimg.cn/20190324014248148.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190324014510781.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
# JSR-133编写器编写手册
![](https://img-blog.csdnimg.cn/20190324020755585.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
# [轻松内存并发](https://www.cl.cam.ac.uk/~pes20/weakmemory/index.html)
多处理器现在已经普及，并发编程正在成为主流，但典型的多处理器（x86，Sparc，Power，ARM，Itanium）和编程语言（C，C ++，Java）不能提供大多数工作所假设的*顺序一致的*共享内存。关于语义和验证。相反，他们有微妙的*放松*（或*弱*）内存模型，暴露出对程序员的硬件和编译器优化所产生的行为。而且，这些记忆模型通常只在含糊不清（有时有缺陷）的散文中描述，导致广泛的混淆。该页面收集了一组人员的工作，他们致力于为多处理器程序开发数学上严格且可用的语义。我们主要关注三种处理器架构（x86，Power和ARM），最近的C ++和C语言版本（C ++ 11和C11），以及使用这些模型的推理和验证。
## [英特尔®64和IA-32架构软件开发人员手册](https://software.intel.com/en-us/articles/intel-sdm)
[Fixing the Java Memory Model, Part 1](https://www.ibm.com/developerworks/library/j-jtp02244/index.html)
[Fixing the Java Memory Model, Part 2](https://www.ibm.com/developerworks/library/j-jtp03304/index.html)
[Going atomic](https://www.ibm.com/developerworks/java/library/j-jtp11234/index.html?ca=drs-)
-------------
[https://hllvm-group.iteye.com/group/topic/21468](https://hllvm-group.iteye.com/group/topic/21468)
![RednaxelaFX的博客](https://www.iteye.com/upload/logo/user/179842/e7f2a3da-4ed0-3627-ba26-c63a7a4f5f13-thumb.jpg?1260702218)
|[RednaxelaFX](https://rednaxelafx.iteye.com/)|2010-09-11
|
[关于VM的帖的目录](https://rednaxelafx.iteye.com/blog/362738)
==============
视频：《[Java Memory Model in 10 minutes](https://www.youtube.com/watch?v=Z4hMFBvCDV4)》
《[Java ExecutorService - Part 1 - Introduction](https://www.youtube.com/watch?v=6Oo-9Can3H8)》
[https://www.youtube.com/channel/UCiz26UeGvcTy4_M3Zhgk7FQ](https://www.youtube.com/channel/UCiz26UeGvcTy4_M3Zhgk7FQ)
![](https://img-blog.csdnimg.cn/2019032820245789.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)

## 重排序
编译器只保证单线程下没问题满足 happens-before，但是单线程下没有happens-before的多线程下会出现重排序指令从而导致不可预期的问题
```python
public class PossibleReordering {
    static int x, y;
    static int a, b;
    public static void main(String[] args) throws InterruptedException {
        for (int i = 0; i < 20; i++) {
            x = 0;
            y = 0;
            a = 0;
            b = 0;
            Thread one = new Thread(new Runnable() {
                public void run() {
                    a = 1;
                    x = b;
                    System.out.println("one run");
                }
            });
            Thread other = new Thread(new Runnable() {
                public void run() {
                    b = 1;
                    y = a;
                   System.out.println("other run");
                }
            });
            System.out.println("one.start()>>>");
            one.start();
            System.out.println("other.start()>>>");
            other.start();
            System.out.println("one.join()>>>");
            one.join();
            System.out.println("other.join()>>>");
            other.join();
            System.out.println(i + " x=" + x + ",y=" + y);
            System.out.println("===============================");
        }
    }
}
```
为什么加join是因为你不加join，main的主线程先执行，这样输出的就不是xy的真实结果而是过程中的数据，所以要先等自线程结束：
第一个线程先执行等第二个线程
![](https://img-blog.csdnimg.cn/20190329175005317.PNG)
如果循环1000次会看出更多的这种不一致。
那么其实程序员想要的是xy需要等ab赋值完成再去赋值，这样就涉及同步，
使用CyclicBarrier就可以很好的完成这个任务：
```python
import java.util.concurrent.CyclicBarrier;

public class PossibleReordering {
    static int x, y;
    static int a, b;

    public static void main(String[] args) throws InterruptedException {
        
       
        for (int i = 0; i < 1000; i++) { 
            CyclicBarrier barrier  = new CyclicBarrier(2);
            x = 0;
            y = 0;
            a = 0;
            b = 0;
            Thread one = new Thread(new Runnable() {
                public void run() {
                    a = 1;
                    try {
                        barrier.await();
                    } catch (Exception ex) {
                        System.out.println("异常"+ex.toString());
                    } 
                    x = b;
                    System.out.println("one run");
                }
            });
            Thread other = new Thread(new Runnable() {
                public void run() {
                    b = 1;
                    try {
                        barrier.await();
                    } catch (Exception ex) {
                        System.out.println("异常"+ex.toString());
                    } 
                    y = a;
                    System.out.println("other run");
                }
            });
            System.out.println("one.start()>>>");
            one.start();
            System.out.println("other.start()>>>");
            other.start();
            System.out.println("one.join()>>>");
            one.join();
            System.out.println("other.join()>>>");
            other.join();
            System.out.println(i + " x=" + x + ",y=" + y);
            System.out.println("===============================");
        }
    }
}
```
![](https://img-blog.csdnimg.cn/20190329182518720.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
从结果可以看出是1000次都是x=1,y=1的。
如果 不写上join
```python
one.join();
other.join();
```
如果不加上join其实会看不到真实的结果，不便于测试的验证，当然了这样是不是反而加深了对join的理解呢？
![](https://img-blog.csdnimg.cn/20190329182715569.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
参考：[JVM(十一)Java指令重排序 ](https://blog.csdn.net/yjp198713/article/details/78839698)[Java并发编程--CyclicBarrier](https://www.cnblogs.com/zaizhoumo/p/7787064.html)
我觉得这幅图画的很好，对多线程理解起来很有帮助：
![](https://img-blog.csdnimg.cn/20190329182958623.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/2019032918304280.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
针对前面main函数必须加join等待的情况其实可以使用 CountDownLatch更优雅的实现：
```python
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package com.current.www;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.CyclicBarrier;

public class PossibleReordering {
    static int x, y;
    static int a, b;

    public static void main(String[] args) throws InterruptedException {
        for (int i = 0; i < 1000; i++) { 
            CountDownLatch latch = new CountDownLatch(2);
            CyclicBarrier barrier  = new CyclicBarrier(2);
            x = 0;
            y = 0;
            a = 0;
            b = 0;
            Thread one = new Thread(new Runnable() {
                public void run() {
                    a = 1;
                    try {
                        barrier.await();
                    } catch (Exception ex) {
                        System.out.println("异常"+ex.toString());
                    } 
                    x = b;
                    latch.countDown();
                }
            });
            Thread other = new Thread(new Runnable() {
                public void run() {
                    b = 1;
                    try {
                        barrier.await();
                    } catch (Exception ex) {
                        System.out.println("异常"+ex.toString());
                    } 
                    y = a;
                    latch.countDown();
                }
            });
            one.start();
            other.start();
            latch.await();
            System.out.println(i + " x=" + x + ",y=" + y);
            System.out.println("===============================");
        }
    }
}
```
![](https://img-blog.csdnimg.cn/20190329184318477.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
至此这个例子也比较好的解释了 CountDownLatch和CyclicBarrier的差异。
我给出一个总结就是：
## CyclicBarrier是线程内部等待，CountDownLatch是线程外等待。
下面看看CyclicBarrier的源码：
```python
/*
 * ORACLE PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/*
 *
 *
 *
 *
 *
 * Written by Doug Lea with assistance from members of JCP JSR-166
 * Expert Group and released to the public domain, as explained at
 * http://creativecommons.org/publicdomain/zero/1.0/
 */
package java.util.concurrent;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;
/**
 * A synchronization aid that allows a set of threads to all wait for
 * each other to reach a common barrier point.  CyclicBarriers are
 * useful in programs involving a fixed sized party of threads that
 * must occasionally wait for each other. The barrier is called
 * <em>cyclic</em> because it can be re-used after the waiting threads
 * are released.
 *
 * <p>A {@code CyclicBarrier} supports an optional {@link Runnable} command
 * that is run once per barrier point, after the last thread in the party
 * arrives, but before any threads are released.
 * This <em>barrier action</em> is useful
 * for updating shared-state before any of the parties continue.
 *
 * <p><b>Sample usage:</b> Here is an example of using a barrier in a
 * parallel decomposition design:
 *
 *  <pre> {@code
 * class Solver {
 *   final int N;
 *   final float[][] data;
 *   final CyclicBarrier barrier;
 *
 *   class Worker implements Runnable {
 *     int myRow;
 *     Worker(int row) { myRow = row; }
 *     public void run() {
 *       while (!done()) {
 *         processRow(myRow);
 *
 *         try {
 *           barrier.await();
 *         } catch (InterruptedException ex) {
 *           return;
 *         } catch (BrokenBarrierException ex) {
 *           return;
 *         }
 *       }
 *     }
 *   }
 *
 *   public Solver(float[][] matrix) {
 *     data = matrix;
 *     N = matrix.length;
 *     Runnable barrierAction =
 *       new Runnable() { public void run() { mergeRows(...); }};
 *     barrier = new CyclicBarrier(N, barrierAction);
 *
 *     List<Thread> threads = new ArrayList<Thread>(N);
 *     for (int i = 0; i < N; i++) {
 *       Thread thread = new Thread(new Worker(i));
 *       threads.add(thread);
 *       thread.start();
 *     }
 *
 *     // wait until done
 *     for (Thread thread : threads)
 *       thread.join();
 *   }
 * }}</pre>
 *
 * Here, each worker thread processes a row of the matrix then waits at the
 * barrier until all rows have been processed. When all rows are processed
 * the supplied {@link Runnable} barrier action is executed and merges the
 * rows. If the merger
 * determines that a solution has been found then {@code done()} will return
 * {@code true} and each worker will terminate.
 *
 * <p>If the barrier action does not rely on the parties being suspended when
 * it is executed, then any of the threads in the party could execute that
 * action when it is released. To facilitate this, each invocation of
 * {@link #await} returns the arrival index of that thread at the barrier.
 * You can then choose which thread should execute the barrier action, for
 * example:
 *  <pre> {@code
 * if (barrier.await() == 0) {
 *   // log the completion of this iteration
 * }}</pre>
 *
 * <p>The {@code CyclicBarrier} uses an all-or-none breakage model
 * for failed synchronization attempts: If a thread leaves a barrier
 * point prematurely because of interruption, failure, or timeout, all
 * other threads waiting at that barrier point will also leave
 * abnormally via {@link BrokenBarrierException} (or
 * {@link InterruptedException} if they too were interrupted at about
 * the same time).
 *
 * <p>Memory consistency effects: Actions in a thread prior to calling
 * {@code await()}
 * <a href="package-summary.html#MemoryVisibility"><i>happen-before</i></a>
 * actions that are part of the barrier action, which in turn
 * <i>happen-before</i> actions following a successful return from the
 * corresponding {@code await()} in other threads.
 *
 * @since 1.5
 * @see CountDownLatch
 *
 * @author Doug Lea
 */
public class CyclicBarrier {
    /**
     * Each use of the barrier is represented as a generation instance.
     * The generation changes whenever the barrier is tripped, or
     * is reset. There can be many generations associated with threads
     * using the barrier - due to the non-deterministic way the lock
     * may be allocated to waiting threads - but only one of these
     * can be active at a time (the one to which {@code count} applies)
     * and all the rest are either broken or tripped.
     * There need not be an active generation if there has been a break
     * but no subsequent reset.
     */
    private static class Generation {
        boolean broken = false;
    }
    /** The lock for guarding barrier entry */
    private final ReentrantLock lock = new ReentrantLock();
    /** Condition to wait on until tripped */
    private final Condition trip = lock.newCondition();
    /** The number of parties */
    private final int parties;
    /* The command to run when tripped */
    private final Runnable barrierCommand;
    /** The current generation */
    private Generation generation = new Generation();
    /**
     * Number of parties still waiting. Counts down from parties to 0
     * on each generation.  It is reset to parties on each new
     * generation or when broken.
     */
    private int count;
    /**
     * Updates state on barrier trip and wakes up everyone.
     * Called only while holding lock.
     */
    private void nextGeneration() {
        // signal completion of last generation
        trip.signalAll();
        // set up next generation
        count = parties;
        generation = new Generation();
    }
    /**
     * Sets current barrier generation as broken and wakes up everyone.
     * Called only while holding lock.
     */
    private void breakBarrier() {
        generation.broken = true;
        count = parties;
        trip.signalAll();
    }
    /**
     * Main barrier code, covering the various policies.
     */
    private int dowait(boolean timed, long nanos)
        throws InterruptedException, BrokenBarrierException,
               TimeoutException {
        final ReentrantLock lock = this.lock;
        lock.lock();
        try {
            final Generation g = generation;
            if (g.broken)
                throw new BrokenBarrierException();
            if (Thread.interrupted()) {
                breakBarrier();
                throw new InterruptedException();
            }
            int index = --count;
            if (index == 0) {  // tripped
                boolean ranAction = false;
                try {
                    final Runnable command = barrierCommand;
                    if (command != null)
                        command.run();
                    ranAction = true;
                    nextGeneration();
                    return 0;
                } finally {
                    if (!ranAction)
                        breakBarrier();
                }
            }
            // loop until tripped, broken, interrupted, or timed out
            for (;;) {
                try {
                    if (!timed)
                        trip.await();
                    else if (nanos > 0L)
                        nanos = trip.awaitNanos(nanos);
                } catch (InterruptedException ie) {
                    if (g == generation && ! g.broken) {
                        breakBarrier();
                        throw ie;
                    } else {
                        // We're about to finish waiting even if we had not
                        // been interrupted, so this interrupt is deemed to
                        // "belong" to subsequent execution.
                        Thread.currentThread().interrupt();
                    }
                }
                if (g.broken)
                    throw new BrokenBarrierException();
                if (g != generation)
                    return index;
                if (timed && nanos <= 0L) {
                    breakBarrier();
                    throw new TimeoutException();
                }
            }
        } finally {
            lock.unlock();
        }
    }
    /**
     * Creates a new {@code CyclicBarrier} that will trip when the
     * given number of parties (threads) are waiting upon it, and which
     * will execute the given barrier action when the barrier is tripped,
     * performed by the last thread entering the barrier.
     *
     * @param parties the number of threads that must invoke {@link #await}
     *        before the barrier is tripped
     * @param barrierAction the command to execute when the barrier is
     *        tripped, or {@code null} if there is no action
     * @throws IllegalArgumentException if {@code parties} is less than 1
     */
    public CyclicBarrier(int parties, Runnable barrierAction) {
        if (parties <= 0) throw new IllegalArgumentException();
        this.parties = parties;
        this.count = parties;
        this.barrierCommand = barrierAction;
    }
    /**
     * Creates a new {@code CyclicBarrier} that will trip when the
     * given number of parties (threads) are waiting upon it, and
     * does not perform a predefined action when the barrier is tripped.
     *
     * @param parties the number of threads that must invoke {@link #await}
     *        before the barrier is tripped
     * @throws IllegalArgumentException if {@code parties} is less than 1
     */
    public CyclicBarrier(int parties) {
        this(parties, null);
    }
    /**
     * Returns the number of parties required to trip this barrier.
     *
     * @return the number of parties required to trip this barrier
     */
    public int getParties() {
        return parties;
    }
    /**
     * Waits until all {@linkplain #getParties parties} have invoked
     * {@code await} on this barrier.
     *
     * <p>If the current thread is not the last to arrive then it is
     * disabled for thread scheduling purposes and lies dormant until
     * one of the following things happens:
     * <ul>
     * <li>The last thread arrives; or
     * <li>Some other thread {@linkplain Thread#interrupt interrupts}
     * the current thread; or
     * <li>Some other thread {@linkplain Thread#interrupt interrupts}
     * one of the other waiting threads; or
     * <li>Some other thread times out while waiting for barrier; or
     * <li>Some other thread invokes {@link #reset} on this barrier.
     * </ul>
     *
     * <p>If the current thread:
     * <ul>
     * <li>has its interrupted status set on entry to this method; or
     * <li>is {@linkplain Thread#interrupt interrupted} while waiting
     * </ul>
     * then {@link InterruptedException} is thrown and the current thread's
     * interrupted status is cleared.
     *
     * <p>If the barrier is {@link #reset} while any thread is waiting,
     * or if the barrier {@linkplain #isBroken is broken} when
     * {@code await} is invoked, or while any thread is waiting, then
     * {@link BrokenBarrierException} is thrown.
     *
     * <p>If any thread is {@linkplain Thread#interrupt interrupted} while waiting,
     * then all other waiting threads will throw
     * {@link BrokenBarrierException} and the barrier is placed in the broken
     * state.
     *
     * <p>If the current thread is the last thread to arrive, and a
     * non-null barrier action was supplied in the constructor, then the
     * current thread runs the action before allowing the other threads to
     * continue.
     * If an exception occurs during the barrier action then that exception
     * will be propagated in the current thread and the barrier is placed in
     * the broken state.
     *
     * @return the arrival index of the current thread, where index
     *         {@code getParties() - 1} indicates the first
     *         to arrive and zero indicates the last to arrive
     * @throws InterruptedException if the current thread was interrupted
     *         while waiting
     * @throws BrokenBarrierException if <em>another</em> thread was
     *         interrupted or timed out while the current thread was
     *         waiting, or the barrier was reset, or the barrier was
     *         broken when {@code await} was called, or the barrier
     *         action (if present) failed due to an exception
     */
    public int await() throws InterruptedException, BrokenBarrierException {
        try {
            return dowait(false, 0L);
        } catch (TimeoutException toe) {
            throw new Error(toe); // cannot happen
        }
    }
    /**
     * Waits until all {@linkplain #getParties parties} have invoked
     * {@code await} on this barrier, or the specified waiting time elapses.
     *
     * <p>If the current thread is not the last to arrive then it is
     * disabled for thread scheduling purposes and lies dormant until
     * one of the following things happens:
     * <ul>
     * <li>The last thread arrives; or
     * <li>The specified timeout elapses; or
     * <li>Some other thread {@linkplain Thread#interrupt interrupts}
     * the current thread; or
     * <li>Some other thread {@linkplain Thread#interrupt interrupts}
     * one of the other waiting threads; or
     * <li>Some other thread times out while waiting for barrier; or
     * <li>Some other thread invokes {@link #reset} on this barrier.
     * </ul>
     *
     * <p>If the current thread:
     * <ul>
     * <li>has its interrupted status set on entry to this method; or
     * <li>is {@linkplain Thread#interrupt interrupted} while waiting
     * </ul>
     * then {@link InterruptedException} is thrown and the current thread's
     * interrupted status is cleared.
     *
     * <p>If the specified waiting time elapses then {@link TimeoutException}
     * is thrown. If the time is less than or equal to zero, the
     * method will not wait at all.
     *
     * <p>If the barrier is {@link #reset} while any thread is waiting,
     * or if the barrier {@linkplain #isBroken is broken} when
     * {@code await} is invoked, or while any thread is waiting, then
     * {@link BrokenBarrierException} is thrown.
     *
     * <p>If any thread is {@linkplain Thread#interrupt interrupted} while
     * waiting, then all other waiting threads will throw {@link
     * BrokenBarrierException} and the barrier is placed in the broken
     * state.
     *
     * <p>If the current thread is the last thread to arrive, and a
     * non-null barrier action was supplied in the constructor, then the
     * current thread runs the action before allowing the other threads to
     * continue.
     * If an exception occurs during the barrier action then that exception
     * will be propagated in the current thread and the barrier is placed in
     * the broken state.
     *
     * @param timeout the time to wait for the barrier
     * @param unit the time unit of the timeout parameter
     * @return the arrival index of the current thread, where index
     *         {@code getParties() - 1} indicates the first
     *         to arrive and zero indicates the last to arrive
     * @throws InterruptedException if the current thread was interrupted
     *         while waiting
     * @throws TimeoutException if the specified timeout elapses.
     *         In this case the barrier will be broken.
     * @throws BrokenBarrierException if <em>another</em> thread was
     *         interrupted or timed out while the current thread was
     *         waiting, or the barrier was reset, or the barrier was broken
     *         when {@code await} was called, or the barrier action (if
     *         present) failed due to an exception
     */
    public int await(long timeout, TimeUnit unit)
        throws InterruptedException,
               BrokenBarrierException,
               TimeoutException {
        return dowait(true, unit.toNanos(timeout));
    }
    /**
     * Queries if this barrier is in a broken state.
     *
     * @return {@code true} if one or more parties broke out of this
     *         barrier due to interruption or timeout since
     *         construction or the last reset, or a barrier action
     *         failed due to an exception; {@code false} otherwise.
     */
    public boolean isBroken() {
        final ReentrantLock lock = this.lock;
        lock.lock();
        try {
            return generation.broken;
        } finally {
            lock.unlock();
        }
    }
    /**
     * Resets the barrier to its initial state.  If any parties are
     * currently waiting at the barrier, they will return with a
     * {@link BrokenBarrierException}. Note that resets <em>after</em>
     * a breakage has occurred for other reasons can be complicated to
     * carry out; threads need to re-synchronize in some other way,
     * and choose one to perform the reset.  It may be preferable to
     * instead create a new barrier for subsequent use.
     */
    public void reset() {
        final ReentrantLock lock = this.lock;
        lock.lock();
        try {
            breakBarrier();   // break the current generation
            nextGeneration(); // start a new generation
        } finally {
            lock.unlock();
        }
    }
    /**
     * Returns the number of parties currently waiting at the barrier.
     * This method is primarily useful for debugging and assertions.
     *
     * @return the number of parties currently blocked in {@link #await}
     */
    public int getNumberWaiting() {
        final ReentrantLock lock = this.lock;
        lock.lock();
        try {
            return parties - count;
        } finally {
            lock.unlock();
        }
    }
}
```
很容易发现使用了ReentrantLock，最主要就是dowait方法。
一开始初始化CyclicBarrier
![](https://img-blog.csdnimg.cn/20190329193133131.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190329193208747.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
就是初始化计数器，直到每个parties 变成0
起了3个线程：
![](https://img-blog.csdnimg.cn/20190329190731220.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
先进入线程0 ，可以看到计数器是2。
![](https://img-blog.csdnimg.cn/20190329193534872.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
进入dowait，拿到锁：
![](https://img-blog.csdnimg.cn/20190329193654788.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
执行到trip.await()让出锁：
![](https://img-blog.csdnimg.cn/20190329200124145.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
这时候再进入线程1：
由于经过2次--操作计数已经是0，所以进入if (index == 0) {  // tripped
![](https://img-blog.csdnimg.cn/20190329200241493.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190329200252677.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)一直执行到unlock之前，在这之前线程0都是等待被唤醒，如图所示。
当线程1执行了其实unlock之后其实线程0已经唤醒，线程1继续往下执行。
![](https://img-blog.csdnimg.cn/20190329200316472.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
如果这时候切回主函数可以看到线程0已经跳出await了：
![](https://img-blog.csdnimg.cn/20190329200535177.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
这时候线程0已经可以去执行下面的操作，因为已经到了安全屏障之后。
线程1回到主函数，然后也是和线程0一样去执行下面的操作，因为也已经到了安全屏障之后。
![](https://img-blog.csdnimg.cn/20190329194636569.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
再总结下思路其实就是产生一个计数，然后每个线程都要同步加锁 -1，减完之后等待其他线程-1，等于0的时候唤醒全部线程继续往下执行。
需要注意的是在IDEA下需要设置所有的多线程涉及的断点需要设置为Thead，否则多线程调试很难达到你想要的效果，也就是会直接跳过其他线程。
![](https://img-blog.csdnimg.cn/20190330082008562.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
==================
再来看看Go语言中的一些处理并发问题的哲学，其实思想都是相通的：
[Share Memory By Communicating](https://golang.org/doc/codewalk/sharemem/)
Go的并发方法与线程和共享内存的传统使用不同。从哲学上讲，可以概括为：
**不要通过共享内存来通信，而应该通过通信来共享内存。**
通道允许您将引用传递给goroutines之间的数据结构。如果您认为这是传递数据的所有权（读取和写入数据的能力），它们将成为一种强大而富有表现力的同步机制。
[Share Memory By Communicating](https://blog.golang.org/share-memory-by-communicating)
=======================
## [消息传递和共享内存并发模型之间有什么区别？](https://stackoverflow.com/questions/1853284/whats-the-difference-between-the-message-passing-and-shared-memory-concurrency)
这是一个非常简单的区别。在共享内存模型中，多个工作程序都对相同的数据进行操作。这开辟了许多并行编程中常见的并发问题。
消息传递系统使工作人员通过消息系统进行通信。消息使每个人都分开，这样工作人员就无法修改彼此的数据。
在共享存储器模型中，存储器由协作进程共享，协作进程可以通过读取和写入数据来交换信息，但是在消息传递中，通过在协作进程之间交换的消息进行通信。
共享内存有助于同时运行进程，但消息传递不能。
消息传递工具有两个操作：send（消息）和receive（消息）。其过程具有固定或可变的大小。
消息传递对于交换较少量的数据很有用，因为不需要避免冲突。与用于进程间通信的共享内存相比，消息传递更容易实现。
在共享内存系统中，仅需要系统调用来建立共享内存区域。一旦建立了共享内存，所有访问都被视为例行内存访问，并且不需要内核的帮助。
共享内存允许最大的通信速度和便利性，因为它可以在计算机内以内存速度完成。共享内存比消息传递更快，因为消息传递系统通常使用系统调用来实现，因此需要更耗时的内核干预任务。
消息传递模型（例如Erlang）没有任何共享状态; 通过交换消息完成所有同步和通信。共享内存模型通过读/写共享内存块进行通信，共享内存块受信号量或类似内容的保护。
对于某些需要快速响应时间的应用程序，消息传递系统的吞吐量可能太低，但如果您需要更高的速度或实时处理，则可以使用共享内存系统。
----
# [区分Java线程和OS线程？](https://stackoverflow.com/questions/1888160/distinguishing-between-java-threads-and-os-threads)
在Linux上，Java线程是使用本机线程实现的，因此使用线程的Java程序与使用线程的本机程序没有什么不同。“Java线程”只是属于JVM进程的线程。
在现代Linux系统（使用NPTL的系统）上，属于进程的所有线程具有相同的进程ID和父进程ID，但具有不同的线程ID。您可以通过运行查看这些ID`ps -eLf`。PID列是进程ID，PPID列是父进程ID，LWP列是线程（LightWeight进程）ID。“main”线程的线程ID与进程ID相同，其他线程将具有不同的线程ID值。
较旧的Linux系统可能使用“linuxthreads”线程实现，而不是完全符合POSIX，而不是NPTL。在linuxthreads系统上，线程具有不同的进程ID。
您可以通过将系统的C库（libc）作为独立程序运行并查看其输出中的“Available extensions”来检查您的系统是使用NPTL还是linuxthreads。它应该提到“Native POSIX Threads Library”或linuxthreads。到C库中的路径改变因系统：它可以是`/lib/libc.so.6`，`/lib64/libc.so.6`（在64位为基础的RedHat系统中），或类似的东西`/lib/x86_64-linux-gnu/libc.so.6`（在现代基于Debian的系统如Ubuntu）。
在操作系统级别，theads没有名称; 那些只存在于JVM中。
在`pthread_kill()`C函数可以被用来把信号发送到特定线程，你可以用它来试图杀死特定线程从JVM外面，但我不知道JVM将如何回应。它可能会杀死整个JVM。
《[内存屏障与 JVM 并发](https://www.infoq.cn/article/memory_barriers_jvm_concurrency)》
内存屏障，又称内存栅栏，是一组处理器指令，用于实现对内存操作的顺序限制。
内存屏障不直接由 JVM 暴露，相反它们被 JVM 插入到指令序列中以维持语言层并发原语的语义。
内存屏障是多线程编程的必要装备。它们形式多样，某些是显式的，某些是隐式的。某些是双向的，某些是单向的。JVM 利用这些形式在所有平台中有效地支持 Java 内存模型。
=========================
推荐3本并行编程的书：
1.入门的，《The Little Book of Semaphores》信号量小书
*信号量小书*是一本免费的（在两个意义上）教科书，它介绍了并发编程的同步原理。
在大多数计算机科学课程中，同步是操作系统类中的一个模块。
操作系统教科书使用一套标准的解决方案提出了一系列标准问题，但是大多数学生并没有很好地理解材料或解决类似问题的能力。
本书的方法是识别对各种同步问题有用的模式，然后展示如何将它们组合成解决方案。在每个问题出现后，本书在提供解决方案之前提供了一个提示，让学生有更好的机会自己发现解决方案。
[http://greenteapress.com/semaphores/](http://greenteapress.com/semaphores/)
这里有人翻译好的中文版：[https://blog.csdn.net/booksyhay/article/category/8055255/2?](https://blog.csdn.net/booksyhay/article/category/8055255/2?)
2. RCU的作者，大牛Paul E. McKenney写的书，“并行编程很难，如果是这样，你能做些什么呢？？”。
[https://mirrors.edge.kernel.org/pub/linux/kernel/people/paulmck/perfbook/perfbook.html](https://mirrors.edge.kernel.org/pub/linux/kernel/people/paulmck/perfbook/perfbook.html)
3.七周七并发模型
[http://www.ituring.com.cn/book/1649](http://www.ituring.com.cn/book/1649)

