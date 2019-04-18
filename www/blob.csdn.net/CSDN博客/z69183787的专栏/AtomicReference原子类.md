# AtomicReference原子类 - z69183787的专栏 - CSDN博客
2015年09月04日 21:42:17[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1452
### **AtomicReference介绍和函数列表**
AtomicReference是作用是对"对象"进行原子操作。
**AtomicReference函数列表**
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
// 使用 null 初始值创建新的 AtomicReference。
AtomicReference()
// 使用给定的初始值创建新的 AtomicReference。
AtomicReference(V initialValue)
// 如果当前值 == 预期值，则以原子方式将该值设置为给定的更新值。
boolean compareAndSet(V expect, V update)
// 获取当前值。
V get()
// 以原子方式设置为给定值，并返回旧值。
V getAndSet(V newValue)
// 最终设置为给定值。
void lazySet(V newValue)
// 设置为给定值。
void set(V newValue)
// 返回当前值的字符串表示形式。
String toString()
// 如果当前值 == 预期值，则以原子方式将该值设置为给定的更新值。
boolean weakCompareAndSet(V expect, V update)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
### AtomicReference源码分析(基于JDK1.7.0_40)
在JDK1.7.0_40中AtomicReference.java的源码如下：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
public class AtomicReference<V>  implements java.io.Serializable {
    private static final long serialVersionUID = -1848883965231344442L;
    // 获取Unsafe对象，Unsafe的作用是提供CAS操作
    private static final Unsafe unsafe = Unsafe.getUnsafe();
    private static final long valueOffset;
    static {
      try {
        valueOffset = unsafe.objectFieldOffset
            (AtomicReference.class.getDeclaredField("value"));
      } catch (Exception ex) { throw new Error(ex); }
    }
    // volatile类型
    private volatile V value;
    public AtomicReference(V initialValue) {
        value = initialValue;
    }
    public AtomicReference() {
    }
    public final V get() {
        return value;
    }
    public final void set(V newValue) {
        value = newValue;
    }
    public final void lazySet(V newValue) {
        unsafe.putOrderedObject(this, valueOffset, newValue);
    }
    public final boolean compareAndSet(V expect, V update) {
        return unsafe.compareAndSwapObject(this, valueOffset, expect, update);
    }
    public final boolean weakCompareAndSet(V expect, V update) {
        return unsafe.compareAndSwapObject(this, valueOffset, expect, update);
    }
    public final V getAndSet(V newValue) {
        while (true) {
            V x = get();
            if (compareAndSet(x, newValue))
                return x;
        }
    }
    public String toString() {
        return String.valueOf(get());
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
**说明**：
AtomicReference的源码比较简单。它是通过"volatile"和"Unsafe提供的CAS函数实现"原子操作。
(01) value是volatile类型。这保证了：当某线程修改value的值时，其他线程看到的value值都是最新的value值，即修改之后的volatile的值。
(02) 通过CAS设置value。这保证了：当某线程池通过CAS函数(如compareAndSet函数)设置value时，它的操作是原子的，即线程在操作value时不会被中断。
### **AtomicReference示例**
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
// AtomicReferenceTest.java的源码
import java.util.concurrent.atomic.AtomicReference;
public class AtomicReferenceTest {
    
    public static void main(String[] args){
        // 创建两个Person对象，它们的id分别是101和102。
        Person p1 = new Person(101);
        Person p2 = new Person(102);
        // 新建AtomicReference对象，初始化它的值为p1对象
        AtomicReference ar = new AtomicReference(p1);
        // 通过CAS设置ar。如果ar的值为p1的话，则将其设置为p2。
        ar.compareAndSet(p1, p2);
        Person p3 = (Person)ar.get();
        System.out.println("p3 is "+p3);
        System.out.println("p3.equals(p1)="+p3.equals(p1));
    }
}
class Person {
    volatile long id;
    public Person(long id) {
        this.id = id;
    }
    public String toString() {
        return "id:"+id;
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
**运行结果**：
```
p3 is id:102
p3.equals(p1)=false
```
**结果说明**：
新建AtomicReference对象ar时，将它初始化为p1。
紧接着，通过CAS函数对它进行设置。如果ar的值为p1的话，则将其设置为p2。
最后，获取ar对应的对象，并打印结果。p3.equals(p1)的结果为false，这是因为Person并没有覆盖equals()方法，而是采用继承自Object.java的equals()方法；而Object.java中的equals()实际上是调用"=="去比较两个对象，即比较两个对象的地址是否相等。
