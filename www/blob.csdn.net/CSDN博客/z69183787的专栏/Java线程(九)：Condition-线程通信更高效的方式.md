# Java线程(九)：Condition-线程通信更高效的方式 - z69183787的专栏 - CSDN博客
2014年06月05日 16:59:13[OkidoGreen](https://me.csdn.net/z69183787)阅读数：696
    接近一周没更新《Java线程》专栏了，主要是这周工作上比较忙，生活上也比较忙，呵呵，进入正题，上一篇讲述了并发包下的Lock，Lock可以更好的解决线程同步问题，使之更面向对象，并且ReadWriteLock在处理同步时更强大，那么同样，线程间仅仅互斥是不够的，还需要通信，本篇的内容是基于上篇之上，使用Lock如何处理线程通信。
        那么引入本篇的主角，Condition，Condition 将 Object 监视器方法（wait、notify 和 notifyAll）分解成截然不同的对象，以便通过将这些对象与任意 Lock 实现组合使用，为每个对象提供多个等待 set （wait-set）。其中，Lock 替代了 synchronized 方法和语句的使用，Condition 替代了 Object 监视器方法的使用。下面将之前写过的一个线程通信的例子替换成用Condition实现(Java线程(三))，代码如下：
**[java]**[view
 plain](http://blog.csdn.net/ghsau/article/details/7481142#)[copy](http://blog.csdn.net/ghsau/article/details/7481142#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/129603/fork)
- publicclass ThreadTest2 {  
- publicstaticvoid main(String[] args) {  
- final Business business = new Business();  
- new Thread(new Runnable() {  
- @Override
- publicvoid run() {  
-                 threadExecute(business, "sub");  
-             }  
-         }).start();  
-         threadExecute(business, "main");  
-     }     
- publicstaticvoid threadExecute(Business business, String threadType) {  
- for(int i = 0; i < 100; i++) {  
- try {  
- if("main".equals(threadType)) {  
-                     business.main(i);  
-                 } else {  
-                     business.sub(i);  
-                 }  
-             } catch (InterruptedException e) {  
-                 e.printStackTrace();  
-             }  
-         }  
-     }  
- }  
- class Business {  
- privateboolean bool = true;  
- private Lock lock = new ReentrantLock();  
- private Condition condition = lock.newCondition();   
- public/*synchronized*/void main(int loop) throws InterruptedException {  
-         lock.lock();  
- try {  
- while(bool) {                 
-                 condition.await();//this.wait();
-             }  
- for(int i = 0; i < 100; i++) {  
-                 System.out.println("main thread seq of " + i + ", loop of " + loop);  
-             }  
-             bool = true;  
-             condition.signal();//this.notify();
-         } finally {  
-             lock.unlock();  
-         }  
-     }     
- public/*synchronized*/void sub(int loop) throws InterruptedException {  
-         lock.lock();  
- try {  
- while(!bool) {  
-                 condition.await();//this.wait();
-             }  
- for(int i = 0; i < 10; i++) {  
-                 System.out.println("sub thread seq of " + i + ", loop of " + loop);  
-             }  
-             bool = false;  
-             condition.signal();//this.notify();
-         } finally {  
-             lock.unlock();  
-         }  
-     }  
- }  
        在Condition中，用await()替换wait()，用signal()替换notify()，用signalAll()替换notifyAll()，传统线程的通信方式，Condition都可以实现，这里注意，Condition是被绑定到Lock上的，要创建一个Lock的Condition必须用newCondition()方法。
        这样看来，Condition和传统的线程通信没什么区别，Condition的强大之处在于它可以为多个线程间建立不同的Condition，下面引入API中的一段代码，加以说明。
**[java]**[view
 plain](http://blog.csdn.net/ghsau/article/details/7481142#)[copy](http://blog.csdn.net/ghsau/article/details/7481142#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/129603/fork)
- class BoundedBuffer {  
- final Lock lock = new ReentrantLock();//锁对象
- final Condition notFull  = lock.newCondition();//写线程条件 
- final Condition notEmpty = lock.newCondition();//读线程条件 
- 
- final Object[] items = new Object[100];//缓存队列
- int putptr/*写索引*/, takeptr/*读索引*/, count/*队列中存在的数据个数*/;  
- 
- publicvoid put(Object x) throws InterruptedException {  
-      lock.lock();  
- try {  
- while (count == items.length)//如果队列满了 
-          notFull.await();//阻塞写线程
-        items[putptr] = x;//赋值 
- if (++putptr == items.length) putptr = 0;//如果写索引写到队列的最后一个位置了，那么置为0
-        ++count;//个数++
-        notEmpty.signal();//唤醒读线程
-      } finally {  
-        lock.unlock();  
-      }  
-    }  
- 
- public Object take() throws InterruptedException {  
-      lock.lock();  
- try {  
- while (count == 0)//如果队列为空
-          notEmpty.await();//阻塞读线程
-        Object x = items[takeptr];//取值 
- if (++takeptr == items.length) takeptr = 0;//如果读索引读到队列的最后一个位置了，那么置为0
-        --count;//个数--
-        notFull.signal();//唤醒写线程
- return x;  
-      } finally {  
-        lock.unlock();  
-      }  
-    }   
-  }  
        这是一个处于多线程工作环境下的缓存区，缓存区提供了两个方法，put和take，put是存数据，take是取数据，内部有个缓存队列，具体变量和方法说明见代码，这个缓存区类实现的功能：有多个线程往里面存数据和从里面取数据，其缓存队列(先进先出后进后出)能缓存的最大数值是100，多个线程间是互斥的，当缓存队列中存储的值达到100时，将写线程阻塞，并唤醒读线程，当缓存队列中存储的值为0时，将读线程阻塞，并唤醒写线程，下面分析一下代码的执行过程：
        1. 一个写线程执行，调用put方法；
        2. 判断count是否为100，显然没有100；
        3. 继续执行，存入值；
        4. 判断当前写入的索引位置++后，是否和100相等，相等将写入索引值变为0，并将count+1；
        5. 仅唤醒**读线程阻塞队列**中的一个；
        6. 一个读线程执行，调用take方法；
        7. ……
        8. 仅唤醒**写线程阻塞队列**中的一个。
        这就是多个Condition的强大之处，假设缓存队列中已经存满，那么阻塞的肯定是写线程，唤醒的肯定是读线程，相反，阻塞的肯定是读线程，唤醒的肯定是写线程，那么假设只有一个Condition会有什么效果呢，缓存队列中已经存满，这个Lock不知道唤醒的是读线程还是写线程了，如果唤醒的是读线程，皆大欢喜，如果唤醒的是写线程，那么线程刚被唤醒，又被阻塞了，这时又去唤醒，这样就浪费了很多时间。
        本文来自：[高爽|Coder](http://blog.csdn.net/ghsau)，原文地址：[](http://blog.csdn.net/ghsau/article/details/7436458)[](http://blog.csdn.net/ghsau/article/details/7451464)[http://blog.csdn.net/ghsau/article/details/7481142](http://blog.csdn.net/ghsau/article/details/7481142)，转载请注明。
