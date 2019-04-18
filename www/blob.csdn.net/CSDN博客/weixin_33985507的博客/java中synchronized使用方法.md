# java中synchronized使用方法 - weixin_33985507的博客 - CSDN博客
2015年02月16日 10:02:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3

synchronized的一个简单样例
public class TextThread 
{
 /**
  * @param args
  */
 public static void main(String[] args) 
 {
  // TODO 自己主动生成方法存根
        TxtThread tt = new TxtThread();
        new Thread(tt).start();
        new Thread(tt).start();
        new Thread(tt).start();
        new Thread(tt).start();
 }
}
class TxtThread implements Runnable
{
 int num = 100;
 String str = new String();
 public void run()
 {
  while (true)
  {
   synchronized(str)
   {
   if (num>0)
   {
    try
    {
     Thread.sleep(10);
    }
    catch(Exception e)
    {
     e.getMessage();
    }
    System.out.println(Thread.currentThread().getName()+ "this is "+ num--);
   }
   }
  }
 }
}
上面的样例中为了制造一个时间差,也就是出错的机会,使用了Thread.sleep(10)
Java对多线程的支持与同步机制深受大家的喜爱，似乎看起来使用了synchronizedkeyword就能够轻松地解决多线程共享数据同步问题。究竟怎样？――还得对synchronizedkeyword的作用进行深入了解才可定论。
总的说来，synchronizedkeyword能够作为函数的修饰符，也可作为函数内的语句，也就是平时说的同步方法和同步语句块。假设再细的分类，synchronized可作用于instance变量、object reference（对象引用）、static函数和class literals(类名称字面常量)身上。
在进一步阐述之前，我们须要明白几点：
A．不管synchronizedkeyword加在方法上还是对象上，它取得的锁都是对象，而不是把一段代码或函数当作锁――并且同步方法非常可能还会被其它线程的对象訪问。
B．每一个对象仅仅有一个锁（lock）与之相关联。
C．实现同步是要非常大的系统开销作为代价的，甚至可能造成死锁，所以尽量避免无谓的同步控制。
**接着来讨论synchronized****用到不同地方对代码产生的影响：**
如果P1、P2是同一个类的不同对象，这个类中定义了下面几种情况的同步块或同步方法，P1、P2就都能够调用它们。
1．把synchronized当作函数修饰符时，演示样例代码例如以下：
Public synchronized void methodAAA()
{
//….
}
这也就是同步方法，那这时synchronized锁定的是哪个对象呢？它锁定的是调用这个同步方法对象。也就是说，当一个对象P1在不同的线程中运行这个同步方法时，它们之间会形成相互排斥，达到同步的效果。可是这个对象所属的Class所产生的还有一对象P2却能够随意调用这个被加了synchronizedkeyword的方法。
上边的演示样例代码等同于例如以下代码：
public void methodAAA()
{
synchronized (this)//(1)
{
//…..
}
}
(1)处的this指的是什么呢？它指的就是调用这种方法的对象，如P1。可见同步方法实质是将synchronized作用于object reference。――那个拿到了P1对象锁的线程，才干够调用P1的同步方法，而对P2而言，P1这个锁与它毫不相干，程序也可能在这样的情形下摆脱同步机制的控制，造成数据混乱：（
2．同步块，演示样例代码例如以下：
public void method3(SomeObject so)
{
synchronized(so)
{
//…..
}
}
这时，锁就是so这个对象，谁拿到这个锁谁就能够执行它所控制的那段代码。当有一个明白的对象作为锁时，就能够这样敲代码，但当没有明白的对象作为锁，仅仅是想让一段代码同步时，能够创建一个特殊的instance变量（它得是一个对象）来充当锁：
class Foo implements Runnable
{
private byte[] lock = new byte[0];// 特殊的instance变量
Public void methodA()
{
synchronized(lock) { //… }
}
//…..
}
注：零长度的byte数组对象创建起来将比不论什么对象都经济――查看编译后的字节码：生成零长度的byte[]对象仅仅需3条操作码，而Object lock = new Object()则须要7行操作码。
3．将synchronized作用于static 函数，演示样例代码例如以下：
Class Foo 
{
public synchronized static void methodAAA()// 同步的static 函数
{
//….
}
public void methodBBB()
{
synchronized(Foo.class)//class literal(类名称字面常量)
}
}
代码中的methodBBB()方法是把class literal作为锁的情况，它和同步的static函数产生的效果是一样的，取得的锁非常特别，是当前调用这种方法的对象所属的类（Class，而不再是由这个Class产生的某个详细对象了）。
记得在《Effective Java》一书中看到过将 Foo.class和 P1.getClass()用于作同步锁还不一样，不能用P1.getClass()来达到锁这个Class的目的。P1指的是由Foo类产生的对象。
能够判断：假设一个类中定义了一个synchronized的static函数A，也定义了一个synchronized 的instance函数B，那么这个类的同一对象Obj在多线程中分别訪问A和B两个方法时，不会构成同步，由于它们的锁都不一样。A方法的锁是Obj这个对象，而B的锁是Obj所属的那个Class。
小结例如以下：
搞清楚synchronized锁定的是哪个对象，就能帮助我们设计更安全的多线程程序。
另一些技巧能够让我们对共享资源的同步訪问更加安全：
1．定义private 的instance变量+它的 get方法，而不要定义public/protected的instance变量。假设将变量定义为public，对象在外界能够绕过同步方法的控制而直接取得它，并修改它。这也是JavaBean的标准实现方式之中的一个。
2．假设instance变量是一个对象，如数组或ArrayList什么的，那上述方法仍然不安全，由于当外界对象通过get方法拿到这个instance对象的引用后，又将其指向还有一个对象，那么这个private变量也就变了，岂不是非常危急。这个时候就须要将get方法也加上synchronized同步，而且，仅仅返回这个private对象的clone()――这样，调用端得到的就是对象副本的引用了。
