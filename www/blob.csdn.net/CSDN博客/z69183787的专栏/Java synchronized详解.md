# Java synchronized详解 - z69183787的专栏 - CSDN博客
2014年03月24日 21:04:08[OkidoGreen](https://me.csdn.net/z69183787)阅读数：531
个人分类：[多线程](https://blog.csdn.net/z69183787/article/category/2176989)
**第一篇：**
**使用synchronized**
在编写一个类时，如果该类中的代码可能运行于多线程环境下，那么就要考虑同步的问题。在Java中内置了语言级的同步原语－－synchronized，这也大大简化了Java中多线程同步的使用。我们首先编写一个非常简单的多线程的程序，是模拟银行中的多个线程同时对同一个储蓄账户进行存款、取款操作的。
在程序中我们使用了一个简化版本的Account类，代表了一个银行账户的信息。在主程序中我们首先生成了1000个线程，然后启动它们，每一个线程都对John的账户进行存100元，然后马上又取出100元。这样，对于John的账户来说，最终账户的余额应该是还是1000元才对。然而运行的结果却超出我们的想像，首先来看看我们的演示代码：
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)class Account {
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)    String name;
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)float amount;
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)    public Account(String name, float amount) {
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)this.name = name;
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)this.amount = amount;
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)    }
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)    public  void deposit(float amt) {
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)float tmp = amount;
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)        tmp += amt;
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)try {
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)            Thread.sleep(100);//模拟其它处理所需要的时间，比如刷新数据库等
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)        } catch (InterruptedException e) {
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)// ignore
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)        }
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)        amount = tmp;
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)    }
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)    public  void withdraw(float amt) {
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)float tmp = amount;
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)        tmp -= amt;
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)try {
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)            Thread.sleep(100);//模拟其它处理所需要的时间，比如刷新数据库等
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)        } catch (InterruptedException e) {
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)// ignore
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)        }        
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)        amount = tmp;
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)    }
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)    public float getBalance() {
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)return amount;
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)    }
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)}
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)public class AccountTest{
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)    private static int NUM_OF_THREAD =1000;
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)    static Thread[] threads =new Thread[NUM_OF_THREAD];
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)    public static void main(String[] args){
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)        final Account acc =new Account("John", 1000.0f);
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)for (int i =0; i< NUM_OF_THREAD; i++) {
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)            threads[i] =new Thread(new Runnable() {
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)                public void run() {
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)                        acc.deposit(100.0f);
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)                        acc.withdraw(100.0f);
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)                }
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)            });
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)            threads[i].start();
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)        }
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)for (int i=0; i<NUM_OF_THREAD; i++){
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)try {
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)                threads[i].join(); //等待所有线程运行结束
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)            } catch (InterruptedException e) {
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)// ignore
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)            }
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)        }
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)        System.out.println("Finally, John's balance is:"+ acc.getBalance());
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)    }
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)}
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
注意，上面在Account的deposit和withdraw方法中之所以要把对amount的运算使用一个临时变量首先存储，sleep一段时间，然后，再赋值给amount，是为了模拟真实运行时的情况。因为在真实系统中，账户信息肯定是存储在持久媒介中，比如RDBMS中，此处的睡眠的时间相当于比较耗时的数据库操作，最后把临时变量tmp的值赋值给amount相当于把amount的改动写入数据库中。运行AccountTest，结果如下（每一次结果都会不同）：
E:\java\exer\bin>java AccountTest
Finally, John's balance is:3900.0
E:\java\exer\bin>java AccountTest
Finally, John's balance is:4900.0
E:\java\exer\bin>java AccountTest
Finally, John's balance is:4700.0
E:\java\exer\bin>java AccountTest
Finally, John's balance is:3900.0
E:\java\exer\bin>java AccountTest
Finally, John's balance is:3900.0
E:\java\exer\bin>java AccountTest
Finally, John's balance is:5200.0
为什么会出现这样的问题？这就是多线程中的同步的问题。在我们的程序中，Account中的amount会同时被多个线程所访问，这就是一个竞争资源，通常称作竞态条件。对于这样的多个线程共享的资源我们必须进行同步，以避免一个线程的改动被另一个线程所覆盖。在我们这个程序中，Account中的amount是一个竞态条件，所以所有对amount的修改访问都要进行同步，我们将deposit()和withdraw()方法进行同步，修改为：
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)  public synchronized void deposit(float amt) {
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)float tmp = amount;
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)        tmp += amt;
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)try {
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)            Thread.sleep(1);//模拟其它处理所需要的时间，比如刷新数据库等
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)        } catch (InterruptedException e) {
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)// ignore
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)        }
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)        amount = tmp;
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)    }
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)    public synchronized void withdraw(float amt) {
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)float tmp = amount;
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)        tmp -= amt;
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)try {
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)            Thread.sleep(1);//模拟其它处理所需要的时间，比如刷新数据库等
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)        } catch (InterruptedException e) {
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)// ignore
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)        }        
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)        amount = tmp;
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)    }
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
此时，再运行，我们就能够得到正确的结果了。Account中的getBalance()也访问了amount，为什么不对getBalance()同步呢？因为getBalance()并不会修改amount的值，所以，同时多个线程对它访问不会造成数据的混乱。
**同步加锁的是对象，而不是代码。**
因此，如果你的类中有一个同步方法，这个方法可以被两个不同的线程同时执行，只要每个线程自己创建一个的该类的实例即可。
　　参考下面的代码：
class Foo extends Thread 
{ 
　private int val; 
　public Foo(int v) 
　{ 
　　val = v; 
　} 
　public synchronized void printVal(int v) 
　{ 
　　while(true) 
　　　System.out.println(v); 
　} 
　public void run() 
　{ 
　　printVal(val); 
　} 
}
class SyncTest 
{ 
　public static void main(String args[]) 
　{ 
　　Foo f1 = new Foo(1); 
　　f1.start(); 
　　Foo f2 = new Foo(3);
　　f2.start(); 
　} 
} 
　　运行SyncTest产生的输出是1和3交叉的。如果printVal是断面，你看到的输出只能是1或者只能是3而不能是两者同时出现。程序运行的结果证明两个线程都在并发的执行printVal方法，即使该方法是同步的并且由于是一个无限循环而没有终止。
**类的同步：**
要实现真正的断面，你必须同步一个全局对象或者对类进行同步。下面的代码给出了一个这样的范例。
class Foo extends Thread 
{ 
　private int val; 
　public Foo(int v) 
　{ 
　　val = v; 
　} 
　public void printVal(int v) 
　{ 
　　synchronized(Foo.class) { 
　　　while(true)
　　　　System.out.println(v); 
　　} 
　} 
　public void run() 
　{ 
　　printVal(val); 
　} 
} 
　　上面的类不再对个别的类实例同步而是对类进行同步。对于类Foo而言，它只有唯一的类定义，两个线程在相同的锁上同步，因此只有一个线程可以执行printVal方法。
　　这个代码也可以通过对公共对象加锁。例如给Foo添加一个静态成员。两个方法都可以同步这个对象而达到线程安全。
面笔者给出一个参考实现，给出同步公共对象的两种通常方法：
　　1、
class Foo extends Thread 
{ 
　private int val;
　private static Object lock=new Object();
　public Foo(int v) 
　{ 
　　val = v; 
　} 
　public void printVal(int v) 
　{ 
　　synchronized(lock) { 
　　　while(true)
　　　　System.out.println(v); 
　　} 
　} 
　public void run() 
　{ 
　　printVal(val); 
　} 
}
　　上面的这个例子比原文给出的例子要好一些，因为原文中的加锁是针对类定义的，一个类只能有一个类定义，而同步的一般原理是应该尽量减小同步的粒度以到达更好的性能。笔者给出的范例的同步粒度比原文的要小。
　　2、
class Foo extends Thread 
{
　private String name;
　private String val;
　public Foo(String name,String v) 
　{ 
　　this.name=name;
　　val = v; 
　} 
　public void printVal() 
　{ 
　　synchronized(val) { 
　　　while(true) System.out.println(name+val); 
　　} 
　} 
　public void run() 
　{ 
　　printVal(); 
　} 
}
public class SyncMethodTest 
{ 
　public static void main(String args[]) 
　{ 
　　Foo f1 = new Foo("Foo 1:","printVal"); 
　　f1.start(); 
　　Foo f2 = new Foo("Foo 2:","printVal");
　　f2.start(); 
　} 
}
　　上面这个代码需要进行一些额外的说明，因为JVM有一种优化机制，因为String类型的对象是不可变的，因此当你使用""的形式引用字符串时，如果JVM发现内存已经有一个这样的对象，那么它就使用那个对象而不再生成一个新的String对象，这样是为了减小内存的使用。
　　上面的main方法其实等同于：
public static void main(String args[]) 
{
　String value="printVal";
　Foo f1 = new Foo("Foo 1:",value); 
　f1.start(); 
　Foo f2 = new Foo("Foo 2:",value);
　f2.start(); 
} 
**总结：**
1、synchronized关键字的作用域有二种： 
1）是某个对象实例内，synchronized aMethod(){}可以防止多个线程同时访问这个对象的synchronized方法（如果一个对象有多个synchronized方法，只要一个线程访问了其中的一个synchronized方法，其它线程不能同时访问这个对象中任何一个synchronized方法）。这时，不同的对象实例的synchronized方法是不相干扰的。也就是说，其它线程照样可以同时访问相同类的另一个对象实例中的synchronized方法； 
2）是某个类的范围，synchronized static aStaticMethod{}防止多个线程同时访问这个类中的synchronized static 方法。它可以对类的所有对象实例起作用。
2、除了方法前用synchronized关键字，synchronized关键字还可以用于方法中的某个区块中，表示只对这个区块的资源实行互斥访问。用法是: synchronized(this){/*区块*/}，它的作用域是当前对象；
3、synchronized关键字是不能继承的，也就是说，基类的方法synchronized f(){} 在继承类中并不自动是synchronized f(){}，而是变成了f(){}。继承类需要你显式的指定它的某个方法为synchronized方法；
Java语言的关键字，当它用来修饰一个方法或者一个代码块的时候，能够保证在同一时刻最多只有一个线程执行该段代码。
     一、当两个并发线程访问同一个对象object中的这个synchronized(this)同步代码块时，一个时间内只能有一个线程得到执行。另一个线程必须等待当前线程执行完这个代码块以后才能执行该代码块。
     二、然而，当一个线程访问object的一个synchronized(this)同步代码块时，另一个线程仍然可以访问该object中的非synchronized(this)同步代码块。
     三、尤其关键的是，当一个线程访问object的一个synchronized(this)同步代码块时，其他线程对object中所有其它synchronized(this)同步代码块的访问将被阻塞。
     四、第三个例子同样适用其它同步代码块。也就是说，当一个线程访问object的一个synchronized(this)同步代码块时，它就获得了这个object的对象锁。结果，其它线程对该object对象所有同步代码部分的访问都被暂时阻塞。
     五、以上规则对其它对象锁同样适用.
**第二篇：**
synchronized 关键字，它包括两种用法：synchronized 方法和 synchronized 块。  
1. synchronized 方法：通过在方法声明中加入 synchronized关键字来声明 synchronized 方法。如：  
public synchronized void accessVal(int newVal);  
synchronized 方法控制对类成员变量的访问：每个类实例对应一把锁，每个 synchronized 方法都必须获得调用该方法的类实例的锁方能
执行，否则所属线程阻塞，方法一旦执行，就独占该锁，直到从该方法返回时才将锁释放，此后被阻塞的线程方能获得该锁，重新进入可执行
状态。这种机制确保了同一时刻对于每一个类实例，其所有声明为 synchronized 的成员函数中至多只有一个处于可执行状态（因为至多只有
一个能够获得该类实例对应的锁），从而有效避免了类成员变量的访问冲突（只要所有可能访问类成员变量的方法均被声明为 synchronized）
。  
在 Java 中，不光是类实例，每一个类也对应一把锁，这样我们也可将类的静态成员函数声明为 synchronized ，以控制其对类的静态成
员变量的访问。  
synchronized 方法的缺陷：若将一个大的方法声明为synchronized 将会大大影响效率，典型地，若将线程类的方法 run() 声明为
synchronized ，由于在线程的整个生命期内它一直在运行，因此将导致它对本类任何 synchronized 方法的调用都永远不会成功。当然我们可
以通过将访问类成员变量的代码放到专门的方法中，将其声明为 synchronized ，并在主方法中调用来解决这一问题，但是 Java 为我们提供
了更好的解决办法，那就是 synchronized 块。  
2. synchronized 块：通过 synchronized关键字来声明synchronized 块。语法如下：  
synchronized(syncObject) {  
//允许访问控制的代码  
}  
synchronized 块是这样一个代码块，其中的代码必须获得对象 syncObject （如前所述，可以是类实例或类）的锁方能执行，具体机
制同前所述。由于可以针对任意代码块，且可任意指定上锁的对象，故灵活性较高。  
对synchronized(this)的一些理解 
一、当两个并发线程访问同一个对象object中的这个synchronized(this)同步代码块时，一个时间内只能有一个线程得到执行。另一个线
程必须等待当前线程执行完这个代码块以后才能执行该代码块。  
二、然而，当一个线程访问object的一个synchronized(this)同步代码块时，另一个线程仍然可以访问该object中的非synchronized
(this)同步代码块。  
三、尤其关键的是，当一个线程访问object的一个synchronized(this)同步代码块时，其他线程对object中所有其它synchronized(this)
同步代码块的访问将被阻塞。  
四、第三个例子同样适用其它同步代码块。也就是说，当一个线程访问object的一个synchronized(this)同步代码块时，它就获得了这个
object的对象锁。结果，其它线程对该object对象所有同步代码部分的访问都被暂时阻塞。  
五、以上规则对其它对象锁同样适用
**第三篇：**
打个比方：一个object就像一个大房子，大门永远打开。房子里有 很多房间（也就是方法）。
这些房间有上锁的（synchronized方法）， 和不上锁之分（普通方法）。房门口放着一把钥匙（key），这把钥匙可以打开所有上锁的房间。
另外我把所有想调用该对象方法的线程比喻成想进入这房子某个 房间的人。所有的东西就这么多了，下面我们看看这些东西之间如何作用的。
在此我们先来明确一下我们的前提条件。该对象至少有一个synchronized方法，否则这个key还有啥意义。当然也就不会有我们的这个主题了。
一个人想进入某间上了锁的房间，他来到房子门口，看见钥匙在那儿（说明暂时还没有其他人要使用上锁的 房间）。于是他走上去拿到了钥匙
，并且按照自己 的计划使用那些房间。注意一点，他每次使用完一次上锁的房间后会马上把钥匙还回去。即使他要连续使用两间上锁的房间，
中间他也要把钥匙还回去，再取回来。
因此，普通情况下钥匙的使用原则是：“随用随借，用完即还。”
这时其他人可以不受限制的使用那些不上锁的房间，一个人用一间可以，两个人用一间也可以，没限制。但是如果当某个人想要进入上锁的房
间，他就要跑到大门口去看看了。有钥匙当然拿了就走，没有的话，就只能等了。
要是很多人在等这把钥匙，等钥匙还回来以后，谁会优先得到钥匙？Not guaranteed。象前面例子里那个想连续使用两个上锁房间的家伙，他
中间还钥匙的时候如果还有其他人在等钥匙，那么没有任何保证这家伙能再次拿到。 （JAVA规范在很多地方都明确说明不保证，象
Thread.sleep()休息后多久会返回运行，相同优先权的线程那个首先被执行，当要访问对象的锁被 释放后处于等待池的多个线程哪个会优先得
到，等等。我想最终的决定权是在JVM，之所以不保证，就是因为JVM在做出上述决定的时候，绝不是简简单单根据 一个条件来做出判断，而是
根据很多条。而由于判断条件太多，如果说出来可能会影响JAVA的推广，也可能是因为知识产权保护的原因吧。SUN给了个不保证 就混过去了
。无可厚非。但我相信这些不确定，并非完全不确定。因为计算机这东西本身就是按指令运行的。即使看起来很随机的现象，其实都是有规律
可寻。学过 计算机的都知道，计算机里随机数的学名是伪随机数，是人运用一定的方法写出来的，看上去随机罢了。另外，或许是因为要想弄
的确定太费事，也没多大意义，所 以不确定就不确定了吧。）
再来看看同步代码块。和同步方法有小小的不同。
1.从尺寸上讲，同步代码块比同步方法小。你可以把同步代码块看成是没上锁房间里的一块用带锁的屏风隔开的空间。
2.同步代码块还可以人为的指定获得某个其它对象的key。就像是指定用哪一把钥匙才能开这个屏风的锁，你可以用本房的钥匙；你也可以指定
用另一个房子的钥匙才能开，这样的话，你要跑到另一栋房子那儿把那个钥匙拿来，并用那个房子的钥匙来打开这个房子的带锁的屏风。
         记住你获得的那另一栋房子的钥匙，并不影响其他人进入那栋房子没有锁的房间。
         为什么要使用同步代码块呢？我想应该是这样的：首先对程序来讲同步的部分很影响运行效率，而一个方法通常是先创建一些局部变
量，再对这些变量做一些 操作，如运算，显示等等；而同步所覆盖的代码越多，对效率的影响就越严重。因此我们通常尽量缩小其影响范围。
如何做？同步代码块。我们只把一个方法中该同 步的地方同步，比如运算。
         另外，同步代码块可以指定钥匙这一特点有个额外的好处，是可以在一定时期内霸占某个对象的key。还记得前面说过普通情况下钥
匙的使用原则吗。现在不是普通情况了。你所取得的那把钥匙不是永远不还，而是在退出同步代码块时才还。
          还用前面那个想连续用两个上锁房间的家伙打比方。怎样才能在用完一间以后，继续使用另一间呢。用同步代码块吧。先创建另外
一个线程，做一个同步代码 块，把那个代码块的锁指向这个房子的钥匙。然后启动那个线程。只要你能在进入那个代码块时抓到这房子的钥匙
，你就可以一直保留到退出那个代码块。也就是说 你甚至可以对本房内所有上锁的房间遍历，甚至再sleep(10*60*1000)，而房门口却还有
1000个线程在等这把钥匙呢。很过瘾吧。
          在此对sleep()方法和钥匙的关联性讲一下。一个线程在拿到key后，且没有完成同步的内容时，如果被强制sleep()了，那key还一
直在 它那儿。直到它再次运行，做完所有同步内容，才会归还key。记住，那家伙只是干活干累了，去休息一下，他并没干完他要干的事。为
了避免别人进入那个房间 把里面搞的一团糟，即使在睡觉的时候他也要把那唯一的钥匙戴在身上。
          最后，也许有人会问，为什么要一把钥匙通开，而不是一个钥匙一个门呢？我想这纯粹是因为复杂性问题。一个钥匙一个门当然更
安全，但是会牵扯好多问题。钥匙 的产生，保管，获得，归还等等。其复杂性有可能随同步方法的增加呈几何级数增加，严重影响效率。这也
算是一个权衡的问题吧。为了增加一点点安全性，导致效 率大大降低，是多么不可取啊。
synchronized的一个简单例子
public class TextThread {
public static void main(String[] args) { 
   TxtThread tt = new TxtThread(); 
   new Thread(tt).start(); 
   new Thread(tt).start(); 
   new Thread(tt).start(); 
   new Thread(tt).start(); 
} 
}
class TxtThread implements Runnable { 
int num = 100; 
String str = new String();
public void run() { 
   synchronized (str) { 
    while (num > 0) {
     try { 
      Thread.sleep(1); 
     } catch (Exception e) { 
      e.getMessage(); 
     } 
     System.out.println(Thread.currentThread().getName() 
       + "this is " + num--); 
    } 
   } 
} 
}
上面的例子中为了制造一个时间差,也就是出错的机会,使用了Thread.sleep(10)
Java对多线程的支持与同步机制深受大家的喜爱，似乎看起来使用了synchronized关键字就可以轻松地解决多线程共享数据同步问题。到底如
何？――还得对synchronized关键字的作用进行深入了解才可定论。
总的说来，synchronized关键字可以作为函数的修饰符，也可作为函数内的语句，也就是平时说的同步方法和同步语句块。如果再细的分类，
synchronized可作用于instance变量、object reference（对象引用）、static函数和class literals(类名称字面常量)身上。
在进一步阐述之前，我们需要明确几点：
A．无论synchronized关键字加在方法上还是对象上，它取得的锁都是对象，而不是把一段代码或函数当作锁――而且同步方法很可能还会被其
他线程的对象访问。
B．每个对象只有一个锁（lock）与之相关联。
C．实现同步是要很大的系统开销作为代价的，甚至可能造成死锁，所以尽量避免无谓的同步控制。
接着来讨论synchronized用到不同地方对代码产生的影响：
假设P1、P2是同一个类的不同对象，这个类中定义了以下几种情况的同步块或同步方法，P1、P2就都可以调用它们。
1． 把synchronized当作函数修饰符时，示例代码如下：
Public synchronized void methodAAA()
{
//….
}
这也就是同步方法，那这时synchronized锁定的是哪个对象呢？它锁定的是调用这个同步方法对象。也就是说，当一个对象P1在不同的线程中
执行这个同步方法时，它们之间会形成互斥，达到同步的效果。但是这个对象所属的Class所产生的另一对象P2却可以任意调用这个被加了
synchronized关键字的方法。
上边的示例代码等同于如下代码：
public void methodAAA()
{
synchronized (this)      // (1)
{
       //…..
}
}
(1)处的this指的是什么呢？它指的就是调用这个方法的对象，如P1。可见同步方法实质是将synchronized作用于object reference。――那个
拿到了P1对象锁的线程，才可以调用P1的同步方法，而对P2而言，P1这个锁与它毫不相干，程序也可能在这种情形下摆脱同步机制的控制，造
成数据混乱：（
2．同步块，示例代码如下：
public void method3(SomeObject so)
{
    synchronized(so)
    { 
       //….. 
    }
}
这时，锁就是so这个对象，谁拿到这个锁谁就可以运行它所控制的那段代码。当有一个明确的对象作为锁时，就可以这样写程序，但当没有明
确的对象作为锁，只是想让一段代码同步时，可以创建一个特殊的instance变量（它得是一个对象）来充当锁：
class Foo implements Runnable
{
        private byte[] lock = new byte[0]; // 特殊的instance变量
        Public void methodA() 
        {
           synchronized(lock) { //… }
        }
        //…..
}
注：零长度的byte数组对象创建起来将比任何对象都经济――查看编译后的字节码：生成零长度的byte[]对象只需3条操作码，而Object lock
= new Object()则需要7行操作码。
3．将synchronized作用于static 函数，示例代码如下：
Class Foo 
{
    public synchronized static void methodAAA()   // 同步的static 函数 
    { 
        //…. 
    }
    public void methodBBB() 
    {
       synchronized(Foo.class)   // class literal(类名称字面常量)
    } 
}
   代码中的methodBBB()方法是把class literal作为锁的情况，它和同步的static函数产生的效果是一样的，取得的锁很特别，是当前调用这
个方法的对象所属的类（Class，而不再是由这个Class产生的某个具体对象了）。
记得在《Effective Java》一书中看到过将 Foo.class和 P1.getClass()用于作同步锁还不一样，不能用P1.getClass()来达到锁这个Class的
目的。P1指的是由Foo类产生的对象。
可以推断：如果一个类中定义了一个synchronized的static函数A，也定义了一个synchronized 的instance函数B，那么这个类的同一对象Obj
在多线程中分别访问A和B两个方法时，不会构成同步，因为它们的锁都不一样。A方法的锁是Obj这个对象，而B的锁是Obj所属的那个Class。
小结如下：
搞清楚synchronized锁定的是哪个对象，就能帮助我们设计更安全的多线程程序。
还有一些技巧可以让我们对共享资源的同步访问更加安全：
1． 定义private 的instance变量+它的 get方法，而不要定义public/protected的instance变量。如果将变量定义为public，对象在外界可以
绕过同步方法的控制而直接取得它，并改动它。这也是JavaBean的标准实现方式之一。
2． 如果instance变量是一个对象，如数组或ArrayList什么的，那上述方法仍然不安全，因为当外界对象通过get方法拿到这个instance对象
的引用后，又将其指向另一个对象，那么这个private变量也就变了，岂不是很危险。 这个时候就需要将get方法也加上synchronized同步，并
且，只返回这个private对象的clone()――这样，调用端得到的就是对象副本的引用了
