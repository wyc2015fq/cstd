# AtomicInteger简介 - z69183787的专栏 - CSDN博客
2015年06月09日 21:44:26[OkidoGreen](https://me.csdn.net/z69183787)阅读数：545
这个类真的非常实用，更重要的是 它确实非常简单：
附上自己的代码，可以自己试试：
AtomicInteger，一个提供原子操作的Integer的类。在Java语言中，++i和i++操作并不是线程安全的，在使用的时候，不可避免的会用到synchronized关键字。而AtomicInteger则通过一种线程安全的加减操作接口。
代码：
package test;
import java.util.concurrent.atomic.AtomicInteger;
/**
 * 来看AtomicInteger提供的接口。
 //获取当前的值
 public final int get()
 //取当前的值，并设置新的值
  public final int getAndSet(int newValue)
 //获取当前的值，并自增
  public final int getAndIncrement() 
 //获取当前的值，并自减
 public final int getAndDecrement()
 //获取当前的值，并加上预期的值
 public final int getAndAdd(int delta)
 * @author YangBaoBao
 *
 */
public class AtomicIntegerDemo {
 public static void main(String[] args) {
  AtomicInteger ai=new AtomicInteger(0);
  int i1=ai.get();
  v(i1);
  int i2=ai.getAndSet(5);
  v(i2);
  int i3=ai.get();
  v(i3);
  int i4=ai.getAndIncrement();
  v(i4);
  v(ai.get());
 }
 static void v(int i)
 {
  System.out.println("i : "+i);
 }
}
