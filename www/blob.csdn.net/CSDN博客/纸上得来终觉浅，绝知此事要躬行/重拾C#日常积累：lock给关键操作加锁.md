# 重拾C#日常积累：lock给关键操作加锁 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年04月13日 16:18:10[boonya](https://me.csdn.net/boonya)阅读数：1379








##### lock基础介绍

lock 关键字将语句块标记为关键部分，方法是获取给定对象的互斥锁，执行语句，然后释放该锁。 以下示例包含一个 lock 语句。比如银行账户的金额变动这些，都需要通过加锁进行保护。

下面是一个取钱的操作：

```
class Account  
{  
    decimal balance;  
    private Object thisLock = new Object();  

    public void Withdraw(decimal amount)  
    {  
        lock (thisLock)  
        {  
            if (amount > balance)  
            {  
                throw new Exception("Insufficient funds");  
            }  
            balance -= amount;  
        }  
    }  
}
```

有关详细信息，请参阅[线程同步](https://docs.microsoft.com/zh-cn/dotnet/csharp/programming-guide/concepts/threading/thread-synchronization)。

##### lock进阶

lock 关键字可确保当一个线程位于代码的关键部分时，另一个线程不会进入该关键部分。 如果其他线程尝试进入锁定的代码，则它将一直等待（即被阻止），直到该对象被释放。




[线程](https://docs.microsoft.com/zh-cn/dotnet/csharp/programming-guide/concepts/threading/index)一节讨论了线程处理。
- lock 关键字在块的开始处调用 Enter，而在块的结尾处调用 Exit。 如果 Interrupt 中断正在等待输入 lock 语句的线程，将引发 ThreadInterruptedException。
- 通常，应避免锁定 public 类型，否则实例将超出代码的控制范围。 常见的结构 lock (this)、lock (typeof (MyType)) 和 lock ("myLock") 违反此准则：
- 如果可以公开访问实例，将出现 lock (this) 问题。
- 如果可以公开访问 lock (typeof (MyType))，将出现 MyType 问题。
- 由于进程中使用同一字符串的任何其他代码都将共享同一个锁，因此出现 lock("myLock") 问题。
- 最佳做法是定义 private 对象来进行锁定，或者定义 private static 对象变量来保护所有实例所共有的数据。
- 在 lock 语句的正文中不能使用 await 关键字。

下例使用线程和 lock。 只要 lock 语句存在，语句块就是关键部分并且 balance 永远不会是负数。


```
// using System.Threading;

class Account
{
    private Object thisLock = new Object();
    int balance;

    Random r = new Random();

    public Account(int initial)
    {
        balance = initial;
    }

    int Withdraw(int amount)
    {

        // This condition never is true unless the lock statement
        // is commented out.
        if (balance < 0)
        {
            throw new Exception("Negative Balance");
        }

        // Comment out the next line to see the effect of leaving out 
        // the lock keyword.
        lock (thisLock)
        {
            if (balance >= amount)
            {
                Console.WriteLine("Balance before Withdrawal :  " + balance);
                Console.WriteLine("Amount to Withdraw        : -" + amount);
                balance = balance - amount;
                Console.WriteLine("Balance after Withdrawal  :  " + balance);
                return amount;
            }
            else
            {
                return 0; // transaction rejected
            }
        }
    }

    public void DoTransactions()
    {
        for (int i = 0; i < 100; i++)
        {
            Withdraw(r.Next(1, 100));
        }
    }
}

class Test
{
    static void Main()
    {
        Thread[] threads = new Thread[10];
        Account acc = new Account(1000);
        for (int i = 0; i < 10; i++)
        {
            Thread t = new Thread(new ThreadStart(acc.DoTransactions));
            threads[i] = t;
        }
        for (int i = 0; i < 10; i++)
        {
            threads[i].Start();
        }
        
        //block main thread until all other threads have ran to completion.
        foreach (var t in threads)
            t.Join();
    }
}
```







