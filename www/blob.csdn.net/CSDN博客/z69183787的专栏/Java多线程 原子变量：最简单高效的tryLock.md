# Java多线程 原子变量：最简单高效的tryLock - z69183787的专栏 - CSDN博客
2013年01月06日 20:19:18[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1790

java 文档：[http://doc.java.sun.com/DocWeb/api/all/java.util.concurrent.atomic.AtomicBoolean](http://doc.java.sun.com/DocWeb/api/all/java.util.concurrent.atomic.AtomicBoolean)
Java代码  ![收藏代码](http://asme2u.iteye.com/images/icon_star.png)
- **import** java.util.concurrent.atomic.AtomicBoolean;  
- 
- 
- **public****class** Lock {  
- 
- **private** AtomicBoolean lock = **new** AtomicBoolean(**true**);  
- 
- **public****boolean** tryLock() {  
- **return** lock.compareAndSet(**true**, **false**);  
-     }  
- 
- **public****void** unlock() {  
-         lock.set(**true**);  
-     }  
- 
- }  
原理：CAS
优点：
     速度快，约是ReentrantLock的2-3倍
 缺点：
     需JDK5.0+
     无条件对象且不可重入
     未获取锁时直接调用unlock不抛IllegalMonitorStateException所以代码必须严格保证获取锁后才能调用unlock
适用场景：
    不需要条件对象且当ReentrantLock的tryLock影响了你的性能的时候
我的应用场景：
    最近项目中通信程序中的客户端的socket长连接的连接池，客户端使用连接具有高并发但占用连接时间非常短的特点，使用这个类代替ReentrantLock，性能提高了3倍左右
import java.util.concurrent.locks.ReentrantLock; 
public class TestLock { 
private static long c = 0; 
public static void main(String[] args) { 
final Lock l = new Lock(); 
// final ReentrantLock l = new ReentrantLock(); 
Thread t1 = new Thread() { 
public void run() { 
for (int i = 0; i < 100000000;) { 
if (l.tryLock()) { 
c++; 
i++; 
l.unlock(); 
} 
} 
} 
}; 
Thread t2 = new Thread() { 
public void run() { 
for (int i = 0; i < 100000000;) { 
if (l.tryLock()) { 
c++; 
i++; 
l.unlock(); 
} 
} 
} 
}; 
long t = System.currentTimeMillis(); 
Thread t3 = new Thread() { 
public void run() { 
for (int i = 0; i < 100000000;) { 
if (l.tryLock()) { 
c++; 
i++; 
l.unlock(); 
} 
} 
} 
}; 
t1.start(); 
t2.start(); 
t3.start(); 
try { 
t1.join(); 
t2.join(); 
t3.join(); 
} catch (InterruptedException e) { 
} 
System.out.println(c); 
System.out.println(System.currentTimeMillis() - t); 
} 
} 
