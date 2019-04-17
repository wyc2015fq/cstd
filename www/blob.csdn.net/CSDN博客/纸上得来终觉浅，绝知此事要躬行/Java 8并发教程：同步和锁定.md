# Java 8并发教程：同步和锁定 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年05月25日 13:39:24[boonya](https://me.csdn.net/boonya)阅读数：689








 原文地址：[http://winterbe.com/posts/2015/04/30/java8-concurrency-tutorial-synchronized-locks-examples/](http://winterbe.com/posts/2015/04/30/java8-concurrency-tutorial-synchronized-locks-examples/)



欢迎来到我的Java 8并发教程的第二部分，在一系列指导教程中使用Java 8中的多线程编程与易于理解的代码示例。在接下来的15分钟内，您将学习如何通过synchronized关键字，锁和信号量同步对可变共享变量的访问。
- 第1部分： [线程和执行器](http://winterbe.com/posts/2015/04/07/java8-concurrency-tutorial-thread-executor-examples/)
- 第2部分：同步和锁定 
- 第3部分： [原子变量和并发图](http://winterbe.com/posts/2015/05/22/java8-concurrency-tutorial-atomic-concurrent-map-examples/)

本文中显示的大多数概念也适用于较旧版本的Java。然而，代码示例关注于Java 8，并大量使用lambda表达式和新的并发功能。如果你还不熟悉lambdas，我建议先阅读我的[Java 8 Tutorial](http://winterbe.com/posts/2014/03/16/java-8-tutorial/)。

为了简单起见，本教程的代码示例使用[这里](https://github.com/winterbe/java8-tutorial/blob/master/src/com/winterbe/java8/samples/concurrent/ConcurrentUtils.java)定义的两个辅助方法sleep(seconds)和stop(executor)。

## Synchronized同步

在上[一个教程中，](http://winterbe.com/posts/2015/04/30/java8-concurrency-tutorial-synchronized-locks-examples/(/posts/2015/04/07/java8-concurrency-tutorial-thread-executor-examples/)我们已经学会了如何通过执行器服务并行执行代码。当编写这样的多线程代码时，必须特别注意从多个线程并发访问共享可变变量。我们只是说我们要增加一个可以从多个线程同时访问的整数。

我们用一个方法increment()定义一个字段count，以增加一个计数：

```java
int count = 0; 
 void increment() { 
    count = count + 1;
 }
```


当从多个线程同时调用此方法时，我们遇到了严重问题：

```java
ExecutorService executor = Executors.newFixedThreadPool(2); 
IntStream.range(0, 10000).forEach(i -> executor.submit(this::increment));
stop(executor); 
System.out.println(count); // 9965
```


每次执行上述代码时，实际结果不是看到常数结果计数为10000。原因是我们在不同线程上共享一个可变变量，而不同步访问该变量，这导致[竞争条件](http://en.wikipedia.org/wiki/Race_condition)。

必须执行三个步骤以增加数字：（i）读取当前值，（ii）将该值增加1，（iii）将新值写入变量。如果两个线程并行执行这些步骤，则两个线程都可能同时执行步骤1，从而读取相同的当前值。这会导致写入丢失，因此实际结果较低。在上面的示例中，由于并发的不同步访问计数而导致35个增量丢失，但是您可能会在执行自己的代码时看到不同的结果。

幸运的是，Java早期通过synchronized关键字支持线程synchronized。当增加计数时，我们可以利用synchronized来修复上述竞争条件：

```java
synchronized void incrementSync() {
 count = count + 1; 
}
```


当同时使用incrementSync()时，我们得到所需的结果计数为10000.不再出现任何竞争条件，并且每次执行代码时结果都是稳定的：

```java
ExecutorService executor = Executors.newFixedThreadPool(2); 
IntStream.range(0, 10000).forEach(i -> executor.submit(this::incrementSync)); 
stop(executor); 
System.out.println(count); // 10000
```

synchronized关键字也可用作块语句。

```java
void incrementSync() { 
    synchronized (this) {
       count = count + 1;
    }
 }
```


内部Java使用所谓的*监视器，*也称为[监视器锁定或内部锁定](https://docs.oracle.com/javase/tutorial/essential/concurrency/locksync.html)，以便管理同步。该监视器绑定到一个对象，例如，当使用同步方法时，每个方法共享相应对象的相同监视器。

所有隐式监视器都实现了可*重*入特性。可重入意味着锁绑定到当前线程。线程可以安全地获取相同的锁，而不会遇到死锁（例如，同步方法调用同一对象上的另一个同步方法）。

## Locks锁

Concurrency API不是通过synchronized关键字使用隐式锁，而是支持由Lock界面指定的各种显式锁。锁定支持各种方法来进行更精细的锁定控制，因此比隐式监视器更具表现力。

标准JDK中提供了多个锁实现，将在以下部分中进行说明。

### ReentrantLock 

ReentrantLock类是一个互斥锁，具有与通过synchronized关键字访问的隐式监视器相同的基本行为，但具有扩展功能。顾名思义，该锁实现了可重入的特性，就像隐式监视器一样。

让我们看看上面的例子如何使用ReentrantLock：

```java
ReentrantLock lock = new ReentrantLock();

int count = 0; 

void increment() { 
  lock.lock(); 
  try { 
    count++;
  }finally {
    lock.unlock(); 
  }
}
```


锁定通过lock()获取并通过unlock()释放。将代码包装到try/finally块中以确保在异常情况下解锁非常重要。这种方法是线程安全的，就像同步的对应方法一样。如果另一个线程已经获得锁定，则随后调用lock()暂停当前​​线程，直到锁定已解锁。在任何给定的时间只有一个线程可以握住锁。

锁定支持各种精细控制的方法，如下一个示例所示：

```java
ExecutorService executor = Executors.newFixedThreadPool(2);

ReentrantLock lock = new ReentrantLock(); 

executor.submit(() -> {
 lock.lock(); 
 try { 
   sleep(1); 
 } finally { 
   lock.unlock();
 } 
});

executor.submit(() -> { System.out.println("Locked: " + lock.isLocked());
System.out.println("Held by me: " + lock.isHeldByCurrentThread());
boolean locked = lock.tryLock(); 
System.out.println("Lock acquired: " + locked); }); 
stop(executor);
```


当第一个任务保持锁一秒钟时，第二个任务获取有关锁的当前状态的不同信息：
`Locked: true Held by me: false Lock acquired: false`



tryLock()作为lock()的替代方法试图获取锁而不会暂停当前线程。必须使用布尔值结果来检查在访问任何共享的可变变量之前是否已经获取了锁。

### ReadWriteLock 

接口ReadWriteLock指定了另一种类型的锁，用于保持用于读取和写入访问的一对锁。读写锁的想法是，只要没人写入这个变量，通常可以同时读取可变变量。因此，只要没有线程持有写锁，读锁可以由多个线程同时保存。这可以提高性能和吞吐量，以便读取比写入更频繁。

```java
ExecutorService executor = Executors.newFixedThreadPool(2);
 Map<String, String> map = new HashMap<>();
 ReadWriteLock lock = new ReentrantReadWriteLock(); 
 executor.submit(() -> { 
   lock.writeLock().lock();
   try { 
     sleep(1); 
     map.put("foo", "bar");
   } finally { 
    lock.writeLock().unlock();
   } 
 });
```


上述示例首先获取写锁定，以便在睡眠一秒钟之后向地图放置新值。在此任务完成之前，正在提交另外两个任务，尝试从地图读取条目并睡眠一秒钟：

```java
Runnable readTask = () -> { 
   lock.readLock().lock();
   try { 
     System.out.println(map.get("foo")); 
     sleep(1);
   } finally { 
     lock.readLock().unlock();
   }
 }; 
executor.submit(readTask); 
executor.submit(readTask); 
stop(executor);
```


当您执行此代码示例时，您会注意到，两个读取任务必须等待一整秒钟，直到写入任务完成。在写锁定已经被释放之后，读取任务并行执行，并将结果同时打印到控制台。它们不必等待彼此完成，因为只要没有写锁定被另一个线程保持，可以安全地并发获取锁定。

### StampedLock 

Java 8附带一种称为StampedLock的新锁，它也支持读写锁，就像上面的例子一样。与ReadWriteLock相反，ReadWriteLock的锁定方法返回由long值表示的戳记。您可以使用这些邮票释放锁或检查锁是否仍然有效。额外的加盖锁支持另一种称为*乐观锁定的*锁定模式。

我们重写最后一个示例代码以使用StampedLock而不是ReadWriteLock：

```java
ExecutorService executor = Executors.newFixedThreadPool(2);
Map<String, String> map = new HashMap<>(); 
StampedLock lock = new StampedLock(); 
executor.submit(() -> { 
    long stamp = lock.writeLock(); 
    try { 
       sleep(1); 
       map.put("foo", "bar");
    } finally { 
       lock.unlockWrite(stamp); 
    } }); 

Runnable readTask = () -> { 
    long stamp = lock.readLock();
    try { 
      System.out.println(map.get("foo"));
      sleep(1);
    } finally { 
      lock.unlockRead(stamp);
   } 
}; 

executor.submit(readTask); 
executor.submit(readTask); 
stop(executor);
```


通过readLock()或writeLock()获取读取或写入锁定返回一个稍后用于在finally块中解锁的印记。请记住，冲压锁不能实现可重入的特性。如果没有锁可用，即使同一个线程已经持有锁，每次调用锁都会返回一个新的戳记。所以你必须特别注意不要陷入僵局。

就像之前的ReadWriteLock示例一样，两个读取任务都必须等到写锁定被释放。然后两个读取任务同时打印到控制台，因为只要不保留写锁定，因为多个读取不会相互阻塞。

下一个示例演示了*乐观锁定*：

```java
ExecutorService executor = Executors.newFixedThreadPool(2); 

StampedLock lock = new StampedLock(); 

executor.submit(() -> { 
   long stamp = lock.tryOptimisticRead();
   try { 
       System.out.println("Optimistic Lock Valid: " + lock.validate(stamp));
       sleep(1); 
       System.out.println("Optimistic Lock Valid: " + lock.validate(stamp));
       sleep(2); 
       System.out.println("Optimistic Lock Valid: " + lock.validate(stamp));
    } finally { 
       lock.unlock(stamp); 
    } 
});

executor.submit(() -> { 
    long stamp = lock.writeLock(); 
    try { 
       System.out.println("Write Lock acquired");
       sleep(2);
    } finally {
       lock.unlock(stamp);
       System.out.println("Write done");
    } 
}); 

stop(executor);
```


通过调用tryOptimisticRead()获取一个乐观的读锁，它始终返回一个邮票而不阻止当前线程，无论锁实际上是否可用。如果已经有一个写锁活动，则返回的戳等于零。您可以随时通过调用lock.validate(stamp)来检查邮票是否有效。

执行上述代码会产生以下输出：
`Optimistic Lock Valid: true Write Lock acquired Optimistic Lock Valid: false Write done Optimistic Lock Valid: false`

获得锁后乐观锁有效。与普通读锁相比，乐观锁并不能阻止其他线程立即获取写锁定。在发送第一个线程睡眠一秒钟之后，第二个线程获得一个写入锁定，而不用等待乐观读取锁定被释放。从这一点来看，乐观的读锁不再有效。即使写锁定被释放，乐观的读锁仍然无效。

所以当使用乐观锁定时，您必须*在*访问任何共享的可变变量*后*每次都验证该锁，以确保读取仍然有效。

有时，将读锁定转换为写锁而无需解锁并再次锁定是有用的。StampedLock提供了tryConvertToWriteLock()方法，如下一个示例所示：

```java
ExecutorService executor = Executors.newFixedThreadPool(2); 

StampedLock lock = new StampedLock(); 

executor.submit(() -> { 
   long stamp = lock.readLock(); 
   try { 
      if (count == 0) { 
        stamp = lock.tryConvertToWriteLock(stamp); 
        if (stamp == 0L) { 
           System.out.println("Could not convert to write lock"); 
           stamp = lock.writeLock();
        } 
        count = 23;
     } 
     System.out.println(count);
   } finally { 
     lock.unlock(stamp);
   } 
});
stop(executor);
```


任务首先获得读锁定，并将当前的字段count打印到控制台。但是，如果当前值为零，我们要分配一个新的值23。我们首先必须将读锁转换成一个写锁，以便不会破坏其他线程的潜在并发访问。调用tryConvertToWriteLock()不阻止，但可能返回一个零戳，表示当前没有可用的写锁定。在这种情况下，我们调用writeLock()来阻止当前线程，直到写入锁可用。

## Semaphores信号灯

除了锁定外，Concurrency API还支持计数信号量。而锁通常授予对变量或资源的独占访问权限，信号量能够维护整套许可证。这在不同的场景中很有用，您必须限制对应用程序某些部分的并发访问量。

以下是一个例子，说明如何限制访问由sleep(5)模拟的长时间运行的任务：

```java
ExecutorService executor = Executors.newFixedThreadPool(10); 
Semaphore semaphore = new Semaphore(5); 
Runnable longRunningTask = () -> { 
   boolean permit = false; 
   try { 
     permit = semaphore.tryAcquire(1, TimeUnit.SECONDS); 
     if (permit) { 
        System.out.println("Semaphore acquired"); 
        sleep(5);
     } else { 
        System.out.println("Could not acquire semaphore");
     }
  } catch (InterruptedException e) {
      throw new IllegalStateException(e);
  } finally { 
    if (permit) { 
       semaphore.release();
    }
  } 
} 

IntStream.range(0, 10).forEach(i -> executor.submit(longRunningTask));
stop(executor);
```


执行器可以同时运行10个任务，但是我们使用大小为5的信号量，从而限制了对5的并发访问。使用try/finally块即使在异常的情况下也能正确地释放信号量很重要。

执行上述代码会产生以下输出：

```
Semaphore acquired Semaphore acquired Semaphore acquired Semaphore acquired Semaphore acquired Could not acquire semaphore Could not acquire semaphore Could not acquire semaphore Could not acquire semaphore Could not acquire semaphore
```


信号量允许访问由sleep(5)模拟的实际长时间运行，最多为5。每次后续调用tryAcquire()经过一秒钟的最大等待超时，导致适当的控制台输出，没有信号量可能收购。

这是一系列并发教程中的第二部分。更多的零件将在不久的将来发布，敬请关注。像往常一样，您可以在[GitHub上](https://github.com/winterbe/java8-tutorial)找到本文中的所有代码示例，因此可以随意分配回购并自行尝试。

我希望你喜欢这篇文章。如果您有任何其他问题，请在下面的评论中向我发送您的反馈。你也应该[跟随我在](https://twitter.com/winterbe_)Twitter上更多的开发相关的东西！





