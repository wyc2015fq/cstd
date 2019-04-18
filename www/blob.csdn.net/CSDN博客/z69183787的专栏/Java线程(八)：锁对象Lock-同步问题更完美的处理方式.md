# Java线程(八)：锁对象Lock-同步问题更完美的处理方式 - z69183787的专栏 - CSDN博客
2014年06月05日 16:58:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：794
      Lock是[java.util.concurrent.locks](http://blog.csdn.net/ghsau/article/details/7461369)包下的接口，Lock 实现提供了比使用synchronized 方法和语句可获得的更广泛的锁定操作，它能以更优雅的方式处理线程同步问题，我们拿[Java线程(二)](http://blog.csdn.net/ghsau/article/details/7424694)中的一个例子简单的实现一下和sychronized一样的效果，代码如下：
**[java]**[view
 plain](http://blog.csdn.net/ghsau/article/details/7461369#)[copy](http://blog.csdn.net/ghsau/article/details/7461369#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/126830/fork)
- publicclass LockTest {  
- publicstaticvoid main(String[] args) {  
- final Outputter1 output = new Outputter1();  
- new Thread() {  
- publicvoid run() {  
-                 output.output("zhangsan");  
-             };  
-         }.start();        
- new Thread() {  
- publicvoid run() {  
-                 output.output("lisi");  
-             };  
-         }.start();  
-     }  
- }  
- class Outputter1 {  
- private Lock lock = new ReentrantLock();// 锁对象
- publicvoid output(String name) {  
- // TODO 线程输出方法
-         lock.lock();// 得到锁
- try {  
- for(int i = 0; i < name.length(); i++) {  
-                 System.out.print(name.charAt(i));  
-             }  
-         } finally {  
-             lock.unlock();// 释放锁
-         }  
-     }  
- }  
        这样就实现了和sychronized一样的同步效果，需要注意的是，用sychronized修饰的方法或者语句块在代码执行完之后锁自动释放，而用Lock需要我们手动释放锁，所以为了保证锁最终被释放(发生异常情况)，要把互斥区放在try内，释放锁放在finally内。
        如果说这就是Lock，那么它不能成为同步问题更完美的处理方式，下面要介绍的是读写锁(ReadWriteLock)，我们会有一种需求，在对数据进行读写的时候，为了保证数据的一致性和完整性，需要读和写是互斥的，写和写是互斥的，但是读和读是不需要互斥的，这样读和读不互斥性能更高些，来看一下不考虑互斥情况的代码原型：
**[java]**[view
 plain](http://blog.csdn.net/ghsau/article/details/7461369#)[copy](http://blog.csdn.net/ghsau/article/details/7461369#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/126830/fork)
- publicclass ReadWriteLockTest {  
- publicstaticvoid main(String[] args) {  
- final Data data = new Data();  
- for (int i = 0; i < 3; i++) {  
- new Thread(new Runnable() {  
- publicvoid run() {  
- for (int j = 0; j < 5; j++) {  
-                         data.set(new Random().nextInt(30));  
-                     }  
-                 }  
-             }).start();  
-         }         
- for (int i = 0; i < 3; i++) {  
- new Thread(new Runnable() {  
- publicvoid run() {  
- for (int j = 0; j < 5; j++) {  
-                         data.get();  
-                     }  
-                 }  
-             }).start();  
-         }  
-     }  
- }  
- class Data {      
- privateint data;// 共享数据    
- publicvoid set(int data) {  
-         System.out.println(Thread.currentThread().getName() + "准备写入数据");  
- try {  
-             Thread.sleep(20);  
-         } catch (InterruptedException e) {  
-             e.printStackTrace();  
-         }  
- this.data = data;  
-         System.out.println(Thread.currentThread().getName() + "写入" + this.data);  
-     }     
- publicvoid get() {  
-         System.out.println(Thread.currentThread().getName() + "准备读取数据");  
- try {  
-             Thread.sleep(20);  
-         } catch (InterruptedException e) {  
-             e.printStackTrace();  
-         }  
-         System.out.println(Thread.currentThread().getName() + "读取" + this.data);  
-     }  
- }  
        部分输出结果：
**[java]**[view
 plain](http://blog.csdn.net/ghsau/article/details/7461369#)[copy](http://blog.csdn.net/ghsau/article/details/7461369#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/126830/fork)
- Thread-1准备写入数据  
- Thread-3准备读取数据  
- Thread-2准备写入数据  
- Thread-0准备写入数据  
- Thread-4准备读取数据  
- Thread-5准备读取数据  
- Thread-2写入12
- Thread-4读取12
- Thread-5读取5
- Thread-1写入12
        我们要实现写入和写入互斥，读取和写入互斥，读取和读取互斥，在set和get方法加入sychronized修饰符：
**[java]**[view
 plain](http://blog.csdn.net/ghsau/article/details/7461369#)[copy](http://blog.csdn.net/ghsau/article/details/7461369#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/126830/fork)
- publicsynchronizedvoid set(int data) {...}      
- publicsynchronizedvoid get() {...}  
        部分输出结果：
**[java]**[view
 plain](http://blog.csdn.net/ghsau/article/details/7461369#)[copy](http://blog.csdn.net/ghsau/article/details/7461369#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/126830/fork)
- Thread-0准备写入数据  
- Thread-0写入9
- Thread-5准备读取数据  
- Thread-5读取9
- Thread-5准备读取数据  
- Thread-5读取9
- Thread-5准备读取数据  
- Thread-5读取9
- Thread-5准备读取数据  
- Thread-5读取9
        我们发现，虽然写入和写入互斥了，读取和写入也互斥了，但是读取和读取之间也互斥了，不能并发执行，效率较低，用读写锁实现代码如下：
**[java]**[view
 plain](http://blog.csdn.net/ghsau/article/details/7461369#)[copy](http://blog.csdn.net/ghsau/article/details/7461369#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/126830/fork)
- class Data {      
- privateint data;// 共享数据
- private ReadWriteLock rwl = new ReentrantReadWriteLock();     
- publicvoid set(int data) {  
-         rwl.writeLock().lock();// 取到写锁
- try {  
-             System.out.println(Thread.currentThread().getName() + "准备写入数据");  
- try {  
-                 Thread.sleep(20);  
-             } catch (InterruptedException e) {  
-                 e.printStackTrace();  
-             }  
- this.data = data;  
-             System.out.println(Thread.currentThread().getName() + "写入" + this.data);  
-         } finally {  
-             rwl.writeLock().unlock();// 释放写锁
-         }  
-     }     
- publicvoid get() {  
-         rwl.readLock().lock();// 取到读锁
- try {  
-             System.out.println(Thread.currentThread().getName() + "准备读取数据");  
- try {  
-                 Thread.sleep(20);  
-             } catch (InterruptedException e) {  
-                 e.printStackTrace();  
-             }  
-             System.out.println(Thread.currentThread().getName() + "读取" + this.data);  
-         } finally {  
-             rwl.readLock().unlock();// 释放读锁
-         }  
-     }  
- }  
        部分输出结果：
**[java]**[view
 plain](http://blog.csdn.net/ghsau/article/details/7461369#)[copy](http://blog.csdn.net/ghsau/article/details/7461369#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/126830/fork)
- Thread-4准备读取数据  
- Thread-3准备读取数据  
- Thread-5准备读取数据  
- Thread-5读取18
- Thread-4读取18
- Thread-3读取18
- Thread-2准备写入数据  
- Thread-2写入6
- Thread-2准备写入数据  
- Thread-2写入10
- Thread-1准备写入数据  
- Thread-1写入22
- Thread-5准备读取数据  
        从结果可以看出实现了我们的需求，这只是锁的基本用法，锁的机制还需要继续深入学习。
        本文来自：[高爽|Coder](http://blog.csdn.net/ghsau)，原文地址：[](http://blog.csdn.net/ghsau/article/details/7424694)[http://blog.csdn.net/ghsau/article/details/7461369](http://blog.csdn.net/ghsau/article/details/7461369)，转载请注明
