# Java线程之线程同步synchronized和volatile - 零度的博客专栏 - CSDN博客
2016年06月07日 10:54:12[零度anngle](https://me.csdn.net/zmx729618)阅读数：410标签：[线程](https://so.csdn.net/so/search/s.do?q=线程&t=blog)
个人分类：[Java并发编程](https://blog.csdn.net/zmx729618/article/category/6181602)
**        可见性与一致性：**
        要说明线程同步问题首先要说明Java线程的两个特性，**可见性**和**有序性**。
        多个线程之间是不能直接传递数据交互的，它们之间的交互只能通过共享变量来实现。拿上篇博文中的例子来说明，在多个线程之间共享了Count类的一个对象，这个对象是被创建在主内存(堆内存)中，每个线程都有自己的工作内存(线程栈)，工作内存存储了主内存Count对象的一个副本，当线程操作Count对象时，首先从主内存复制Count对象到工作内存中，然后执行代码count.count()，改变了num值，最后用工作内存Count刷新主内存Count。当一个对象在多个内存中都存在副本时，如果一个内存修改了共享变量，其它线程也应该能够看到被修改后的值，**此为可见性**。
        多个线程执行时，CPU对线程的调度是随机的，我们不知道当前程序被执行到哪步就切换到了下一个线程，一个最经典的例子就是银行汇款问题，一个银行账户存款100，这时一个人从该账户取10元，同时另一个人向该账户汇10元，那么余额应该还是100。那么此时可能发生这种情况，A线程负责取款，B线程负责汇款，A从主内存读到100，B从主内存读到100，A执行减10操作，并将数据刷新到主内存，这时主内存数据100-10=90，而B内存执行加10操作，并将数据刷新到主内存，最后主内存数据100+10=110，显然这是一个严重的问题，我们要保证A线程和B线程有序执行，先取款后汇款或者先汇款后取款，**此为有序性**。
       本文讲述了JDK5.0之前传统线程的同步方式，下面同样用代码来展示一下线程同步问题。
       TraditionalThreadSynchronized.java：创建两个线程，执行同一个对象的输出方法。
**[java]**[view plain](http://blog.csdn.net/ghsau/article/details/7424694#)[copy](http://blog.csdn.net/ghsau/article/details/7424694#)[print](http://blog.csdn.net/ghsau/article/details/7424694#)[?](http://blog.csdn.net/ghsau/article/details/7424694#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/112339/fork)
- publicclass TraditionalThreadSynchronized {  
- publicstaticvoid main(String[] args) {  
- final Outputter output = new Outputter();  
- new Thread() {  
- publicvoid run() {  
-                 output.output("zhangsan");  
-             }  
-         }.start();        
- new Thread() {  
- publicvoid run() {  
-                 output.output("lisi");  
-             }  
-         }.start();  
-     }  
- }  
- class Outputter {  
- publicvoid output(String name) {  
- // TODO 为了保证对name的输出不是一个原子操作，这里逐个输出name的每个字符
- for(int i = 0; i < name.length(); i++) {  
-             System.out.print(name.charAt(i));  
- // Thread.sleep(10);
-         }  
-     }  
- }  
```java
<span style="font-family:Arial;font-size:12px;">public class TraditionalThreadSynchronized {
	public static void main(String[] args) {
		final Outputter output = new Outputter();
		new Thread() {
			public void run() {
				output.output("zhangsan");
			}
		}.start();		
		new Thread() {
			public void run() {
				output.output("lisi");
			}
		}.start();
	}
}
class Outputter {
	public void output(String name) {
		// TODO 为了保证对name的输出不是一个原子操作，这里逐个输出name的每个字符
		for(int i = 0; i < name.length(); i++) {
			System.out.print(name.charAt(i));
			// Thread.sleep(10);
		}
	}
}</span>
```
        运行结果：
**[java]**[view plain](http://blog.csdn.net/ghsau/article/details/7424694#)[copy](http://blog.csdn.net/ghsau/article/details/7424694#)[print](http://blog.csdn.net/ghsau/article/details/7424694#)[?](http://blog.csdn.net/ghsau/article/details/7424694#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/112339/fork)
- zhlainsigsan  
```java
<span style="font-family:Arial;font-size:12px;">zhlainsigsan</span>
```
        显然输出的字符串被打乱了，我们期望的输出结果是zhangsanlisi，这就是线程同步问题，我们希望output方法被一个线程完整的执行完之后再切换到下一个线程，Java中使用synchronized保证一段代码在多线程执行时是互斥的，有两种用法：
        1. 使用synchronized将需要互斥的代码包含起来，并上一把锁。
**[java]**[view plain](http://blog.csdn.net/ghsau/article/details/7424694#)[copy](http://blog.csdn.net/ghsau/article/details/7424694#)[print](http://blog.csdn.net/ghsau/article/details/7424694#)[?](http://blog.csdn.net/ghsau/article/details/7424694#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/112339/fork)
- {  
- synchronized (this) {  
- for(int i = 0; i < name.length(); i++) {  
-             System.out.print(name.charAt(i));  
-         }  
-     }  
- }  
```java
<span style="font-family:Arial;font-size:12px;">{
    synchronized (this) {
        for(int i = 0; i < name.length(); i++) {
            System.out.print(name.charAt(i));
        }
    }
}
</span>
```
        这把锁必须是需要互斥的多个线程间的共享对象，像下面的代码是没有意义的。
**[java]**[view plain](http://blog.csdn.net/ghsau/article/details/7424694#)[copy](http://blog.csdn.net/ghsau/article/details/7424694#)[print](http://blog.csdn.net/ghsau/article/details/7424694#)[?](http://blog.csdn.net/ghsau/article/details/7424694#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/112339/fork)
- {  
-     Object lock = new Object();  
- synchronized (lock) {  
- for(int i = 0; i < name.length(); i++) {  
-             System.out.print(name.charAt(i));  
-         }  
-     }  
- }  
```java
<span style="font-family:Arial;font-size:12px;">{
    Object lock = new Object();
    synchronized (lock) {
        for(int i = 0; i < name.length(); i++) {
            System.out.print(name.charAt(i));
        }
    }
}
</span>
```
        每次进入output方法都会创建一个新的lock，这个锁显然每个线程都会创建，没有意义。
        2. 将synchronized加在需要互斥的方法上。
**[java]**[view plain](http://blog.csdn.net/ghsau/article/details/7424694#)[copy](http://blog.csdn.net/ghsau/article/details/7424694#)[print](http://blog.csdn.net/ghsau/article/details/7424694#)[?](http://blog.csdn.net/ghsau/article/details/7424694#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/112339/fork)
- publicsynchronizedvoid output(String name) {  
- // TODO 线程输出方法
- for(int i = 0; i < name.length(); i++) {  
-         System.out.print(name.charAt(i));  
-     }  
- }  
```java
<span style="font-family:Arial;font-size:12px;">public synchronized void output(String name) {
    // TODO 线程输出方法
    for(int i = 0; i < name.length(); i++) {
        System.out.print(name.charAt(i));
    }
}</span>
```
        这种方式就相当于用this锁住整个方法内的代码块，如果用synchronized加在静态方法上，就相当于用××××.class锁住整个方法内的代码块。使用synchronized在某些情况下会造成死锁，死锁问题以后会说明。**使用synchronized修饰的方法或者代码块可以看成是一个原子操作**。
        每个锁对(JLS中叫monitor)都有两个队列，一个是就绪队列，一个是阻塞队列，就绪队列存储了将要获得锁的线程，阻塞队列存储了被阻塞的线程，当一个线程被唤醒(notify)后，才会进入到就绪队列，等待CPU的调度，反之，当一个线程被wait后，就会进入阻塞队列，等待下一次被唤醒，这个涉及到线程间的通信，下一篇博文会说明。看我们的例子，当第一个线程执行输出方法时，获得同步锁，执行输出方法，恰好此时第二个线程也要执行输出方法，但发现同步锁没有被释放，第二个线程就会进入就绪队列，等待锁被释放。一个线程执行互斥代码过程如下：
        1. 获得同步锁；
        2. 清空工作内存；
        3. 从主内存拷贝对象副本到工作内存；
        4. 执行代码(计算或者输出等)；
        5. 刷新主内存数据；
        6. 释放同步锁。
        所以，synchronized既保证了多线程的并发有序性，又保证了多线程的内存可见性。
        volatile是第二种Java多线程同步的机制，根据[JLS(Java LanguageSpecifications)](http://docs.oracle.com/javase/specs/jls/se7/jls7.pdf)的说法，一个变量可以被volatile修饰，在这种情况下内存模型(主内存和线程工作内存)确保所有线程可以看到一致的变量值，来看一段代码：
**[java]**[view plain](http://blog.csdn.net/ghsau/article/details/7424694#)[copy](http://blog.csdn.net/ghsau/article/details/7424694#)[print](http://blog.csdn.net/ghsau/article/details/7424694#)[?](http://blog.csdn.net/ghsau/article/details/7424694#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/112339/fork)
- class Test {  
- staticint i = 0, j = 0;  
- staticvoid one() {  
-         i++;  
-         j++;  
-     }  
- staticvoid two() {  
-         System.out.println("i=" + i + " j=" + j);  
-     }  
- }  
```java
<span style="font-family:Arial;font-size:12px;">class Test {
	static int i = 0, j = 0;
	static void one() {
		i++;
		j++;
	}
	static void two() {
		System.out.println("i=" + i + " j=" + j);
	}
}</span>
```
        一些线程执行one方法，另一些线程执行two方法，two方法有可能打印出j比i大的值，按照之前分析的线程执行过程分析一下：
        1. 将变量i从主内存拷贝到工作内存；
        2. 改变i的值；
        3. 刷新主内存数据；
        4. 将变量j从主内存拷贝到工作内存；
        5. 改变j的值；
        6. 刷新主内存数据；
        这个时候执行two方法的线程先读取了主存i原来的值又读取了j改变后的值，这就导致了程序的输出不是我们预期的结果，要阻止这种不合理的行为的一种方式是在one方法和two方法前面加上synchronized修饰符：
**[java]**[view plain](http://blog.csdn.net/ghsau/article/details/7424694#)[copy](http://blog.csdn.net/ghsau/article/details/7424694#)[print](http://blog.csdn.net/ghsau/article/details/7424694#)[?](http://blog.csdn.net/ghsau/article/details/7424694#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/112339/fork)
- class Test {  
- staticint i = 0, j = 0;  
- staticsynchronizedvoid one() {  
-         i++;  
-         j++;  
-     }  
- staticsynchronizedvoid two() {  
-         System.out.println("i=" + i + " j=" + j);  
-     }  
- }  
```java
<span style="font-family:Arial;font-size:12px;">class Test {
	static int i = 0, j = 0;
	static synchronized void one() {
		i++;
		j++;
	}
	static synchronized void two() {
		System.out.println("i=" + i + " j=" + j);
	}
}</span>
```
       根据前面的分析，我们可以知道，这时one方法和two方法再也不会并发的执行了，i和j的值在主内存中会一直保持一致，并且two方法输出的也是一致的。另一种同步的机制是在共享变量之前加上volatile：
**[java]**[view plain](http://blog.csdn.net/ghsau/article/details/7424694#)[copy](http://blog.csdn.net/ghsau/article/details/7424694#)[print](http://blog.csdn.net/ghsau/article/details/7424694#)[?](http://blog.csdn.net/ghsau/article/details/7424694#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/112339/fork)
- class Test {  
- staticvolatileint i = 0, j = 0;  
- staticvoid one() {  
-         i++;  
-         j++;  
-     }  
- staticvoid two() {  
-         System.out.println("i=" + i + " j=" + j);  
-     }  
- }  
```java
<span style="font-family:Arial;font-size:12px;">class Test {
	static volatile int i = 0, j = 0;
	static void one() {
		i++;
		j++;
	}
	static void two() {
		System.out.println("i=" + i + " j=" + j);
	}
}</span>
```
       one方法和two方法还会并发的去执行，但是加上volatile可以将共享变量i和j的改变直接响应到主内存中，这样保证了**主内存中i和j的值一致性**，然而在执行two方法时，在two方法获取到i的值和获取到j的值中间的这段时间，one方法也许被执行了好多次，导致j的值会大于i的值。所以volatile可以保证内存可见性，不能保证并发有序性。
