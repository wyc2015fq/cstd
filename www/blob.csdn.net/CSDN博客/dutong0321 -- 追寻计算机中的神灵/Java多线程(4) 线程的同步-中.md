# Java多线程(4) 线程的同步-中 - dutong0321 -- 追寻计算机中的神灵 - CSDN博客
2018年01月07日 17:51:17[dutong0321](https://me.csdn.net/dutong0321)阅读数：156
#### 概述
上一章说明了多个线程对同一变量进行读写时，如何解决竞争条件，也就是使用了线程锁。线程锁保证了每条语句都可以单独执行，保证了语句的原子性。但是，在实际应用中可能还要根据实际情况对线程进行某些约束的检查，例如在银行转账的过程中，如果卡中的余额不足以支付转账的余额，那么是不允许转账的。（我没有其他更好的例子，只好搬来Java核心技术中的例子，我看到其他好多博客关于多线程的例子都是银行转账，但都没有注明来自哪里，我的稍微简化了一些原来的代码。） 
这个约束必须要在线程锁外内，例如：检测到余额充足，但是在加锁前，其他线程从该账户中转出了一笔钱，那么，实际上该账户余额就已经不充足了。
#### 条件对象
为了解决线程进入临界区，却发现在某一条件满足之后它才能执行，要使用一个条件对象来管理那些已经获得一个锁但是却不能做游泳工作的线程。 
代码如下： 
Bank.java
```
package com.utopia.controls;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;
public class Bank {
    private int[] account;
    private Lock bankLock;
    private Condition bankCondition;
    // n 创建账户的数量
    // initAccount 账户中的初始值
    public Bank(int n, int initAccount) {
        bankLock = new ReentrantLock();
        bankCondition = bankLock.newCondition();
        account = new int[n];
        for (int i = 0; i < n; i++) {
            account[i] = initAccount;
        }
    }
    // 从序号为from到to传输value值
    public void transfer(int from, int to, int value) throws Exception {
        // 为保证原子性，进行锁
        bankLock.lock();
        // 检测账户中的余额是否满足输出
        // 必须要循环检测，因为当线程接收到信号从阻塞中唤醒后
        // 需要再次检测账户中的余额，是否大于输出金额
        while (account[from] <= value) {
            // 如果账户余额不足 那么该线程进行阻塞 等待其他线程唤醒
            bankCondition.await();
        }
        account[from] -= value;
        account[to] += value;
        System.out.println(account[0] + " " + account[1] + " " + value);
        // 账户信息变动 向其他线程发送信号 使其他线程从阻塞状态唤醒
        bankCondition.signalAll();
        // 解锁
        bankLock.unlock();
    }
}
```
BankTransferRunnable.java
```java
package com.utopia.controls;
import java.util.Random;
public class BankTransferRunnable implements Runnable {
    private Bank bank;
    private int from;
    private int to;
    private int count;
    private Random random;
    public BankTransferRunnable(Bank bank, int from, int to, int count) {
        this.bank = bank;
        this.from = from;
        this.to = to;
        this.count = count;
        this.random = new Random(System.currentTimeMillis());
    }
    @Override
    public void run() {
        // TODO Auto-generated method stub
        try {
            for (int i = 0; i < count; i++) {
                // 从from账户中取0-100之间任意数目到to账户当中
                bank.transfer(from, to, random.nextInt(101));
            }
        } catch (Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }
}
```
BankMain.java
```java
package com.utopia.controls;
public class BankMain {
    /**
     * @param args
     */
    public static void main(String[] args) {
        // TODO Auto-generated method stub
        Bank bank = new Bank(2, 100);
        Thread thread1 = new Thread(new BankTransferRunnable(bank, 1, 0, 10));
        Thread thread2 = new Thread(new BankTransferRunnable(bank, 0, 1, 10));
        thread1.start();
        thread2.start();
    }
}
```
如果在Bank类中的transfer()方法中没有条件对象，运行结果： 
![这里写图片描述](https://img-blog.csdn.net/20180106225624085?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHV0b25nMDMyMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
虽然账户1和账户2的总额一共是200，但是可以看到有大量的负值出现，这与银行账户是不可能出现负值的实际情况不符（信用卡不算）。 
而上面加上条件对象的结果如下： 
![这里写图片描述](https://img-blog.csdn.net/20180106225754324?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZHV0b25nMDMyMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
可以看到账户1和账户2中均没有出现负值的情况。
#### 内部对象锁 synchromized
使用Lock和Condition可以高度的锁定控制，但是大多数情况下并不需要这么高度的控制，为了简洁可以使用内部锁。 
例如，我们可以重新修改Bank.java
```
package com.utopia.controls;
public class Bank {
    private int[] account;
    // n 创建账户的数量
    // initAccount 账户中的初始值
    public Bank(int n, int initAccount) {
        account = new int[n];
        for (int i = 0; i < n; i++) {
            account[i] = initAccount;
        }
    }
    // 从序号为from到to传输value值
    // synchronized 使用内部锁来保证原子性
    public synchronized void transfer(int from, int to, int value) throws Exception {
        // 检测账户中的余额是否满足输出
        // 必须要循环检测，因为当线程接收到信号从阻塞中唤醒后
        // 需要再次检测账户中的余额，是否大于输出金额
        while (account[from] <= value) {
            // wait 添加线程到等待集中
            wait();
        }
        account[from] -= value;
        account[to] += value;
        System.out.println(account[0] + " " + account[1] + " " + value);
        // 解除等待线程的阻塞状态
        notifyAll();
    }
}
```
运行后可以看到运行代码简洁了不少，运行结果也依然正确无误。
#### 同步阻塞
对于Lock和Condition来讲，虽然编写的代码量略大，但是，使用起来却是非常灵活，适合各种场景；对于内部对象锁虽然代码简洁，但是只能对方法保持原子性；于是就又出现了一种比较简洁，但是相对于内部对象锁要灵活一些的方法。 
内部对象锁是用关键字synchronized直接修饰要保持原子性的方法，同步阻塞的使用方法如下(依旧修改Bank.java)：
```
package com.utopia.controls;
public class Bank {
    private int[] account;
    // n 创建账户的数量
    // initAccount 账户中的初始值
    public Bank(int n, int initAccount) {
        account = new int[n];
        for (int i = 0; i < n; i++) {
            account[i] = initAccount;
        }
    }
    // 从序号为from到to传输value值
    public void transfer(int from, int to, int value) throws Exception {
        // synchronized 使用同步阻塞来保证原子性
        // 注意：被synchronized的对象应该是共享的对象，或者说希望被上锁的对象
        // 否则，在调用notifyAll方法时会抛出IllegalMonitorStateException
        synchronized (this) {
            // 检测账户中的余额是否满足输出
            // 必须要循环检测，因为当线程接收到信号从阻塞中唤醒后
            // 需要再次检测账户中的余额，是否大于输出金额
            while (account[from] <= value) {
                // wait 添加线程到等待集中
                wait();
            }
            account[from] -= value;
            account[to] += value;
            System.out.println(account[0] + " " + account[1] + " " + value);
            // 解除等待线程的阻塞状态
            notifyAll();
        }
    }
}
```
