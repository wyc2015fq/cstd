# "synchronized" 的四种基本用法 - DEVELOPER - CSDN博客





2014年10月07日 17:34:15[学术袁](https://me.csdn.net/u012827205)阅读数：998








*synchronized的4种用法:*

*用法一：*

方法声明时使用,放在范围操作符(public等)之后,返回类型声明(void等)之前.这时,线程获得的是成员锁,即一次只能有一个线程进入该方法,其他线程要想在此时调用该方法,只能排队等候,当前线程(就是在synchronized方法内部的线程)执行完该方法后,别的线程才能进入.

```java
public synchronized void synMethod() {
        //方法体
      }
```



*用法二：*

对某一代码块使用,synchronized后跟括号,括号里是变量,这样,一次只有一个线程进入该代码块.此时,线程获得的是成员锁.例如: 

```java
public int synMethod(int a1){
        synchronized(a1) {
          //一次只能有一个线程进入
        }
      }
```



*用法三：*

synchronized后面括号里是一对象,此时,线程获得的是对象锁.例如:

```java
public class MyThread implements Runnable {
    public static void main(String args[]) {
    MyThread mt = new MyThread();
    Thread t1 = new Thread(mt, "t1");
    Thread t2 = new Thread(mt, "t2");
    Thread t3 = new Thread(mt, "t3");
    Thread t4 = new Thread(mt, "t4");
    Thread t5 = new Thread(mt, "t5");
    Thread t6 = new Thread(mt, "t6");
    t1.start();
    t2.start();
    t3.start();
    t4.start();
    t5.start();
    t6.start();
  }

  public void run() {
    synchronized (this) {
      System.out.println(Thread.currentThread().getName());
    }
  }
}
```

 对于3,如果线程进入,则得到当前对象锁,那么别的线程在该类所有对象上的任何操作都不能进行.在对象级使用锁通常是一种比较粗糙的方法。为什么要将整个对象都上锁，而不允许其他线程短暂地使用对象中其他同步方法来访问共享资源？如果一个对象拥有多个资源，就不需要只为了让一个线程使用其中一部分资源，就将所有线程都锁在外面。由于每个对象都有锁，可以如下所示使用虚拟对象来上锁：

```java
class FineGrainLock {

   MyMemberClass x, y;
   Object xlock = new Object(), ylock = new Object();

   public void foo() {
      synchronized(xlock) {
         //access x here
      }

      //do something here - but don't use shared resources

      synchronized(ylock) {
         //access y here
      }
   }

   public void bar() {
      synchronized(this) {
         //access both x and y here
      }
      //do something here - but don't use shared resources
   }
  }
```



*用法四：*

synchronized后面括号里是类,此时,线程获得的是对象锁.例如:

```java
class ArrayWithLockOrder{
  private static long num_locks = 0;
  private long lock_order;
  private int[] arr;

  public ArrayWithLockOrder(int[] a)
  {
    arr = a;
    synchronized(ArrayWithLockOrder.class) {//-----这里
      num_locks++;             // 锁数加 1。

      lock_order = num_locks;  // 为此对象实例设置唯一的 lock_order。
    }
  }
  public long lockOrder()
  {
    return lock_order;
  }
  public int[] array()
  {
    return arr;
  }
  }

  class SomeClass implements Runnable
 {
  public int sumArrays(ArrayWithLockOrder a1,
                       ArrayWithLockOrder a2)
  {
    int value = 0;
    ArrayWithLockOrder first = a1;       // 保留数组引用的一个
    ArrayWithLockOrder last = a2;        // 本地副本。
    int size = a1.array().length;
    if (size == a2.array().length)
    {
      if (a1.lockOrder() > a2.lockOrder())  // 确定并设置对象的锁定
      {                                     // 顺序。
        first = a2;
        last = a1;
      }
      synchronized(first) {              // 按正确的顺序锁定对象。
        synchronized(last) {
          int[] arr1 = a1.array();
          int[] arr2 = a2.array();
          for (int i=0; i<size; i++)
            value += arr1[i] + arr2[i];
        }
      }
    }
    return value;

  }
  public void run() {
    //
  }
  }
```

对于4,如果线程进入,则线程在该类中所有操作不能进行,包括静态变量和静态方法,实际上,对于含有静态方法和静态变量的代码块的同步,我们通常用4来加锁.

原文章出自于：http://www.blogjava.net/konhon/archive/2005/08/17/10296.html




