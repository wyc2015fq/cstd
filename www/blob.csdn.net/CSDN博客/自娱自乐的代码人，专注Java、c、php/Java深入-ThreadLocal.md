# Java深入 - ThreadLocal - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年12月05日 12:53:50[initphp](https://me.csdn.net/initphp)阅读数：6146
所属专栏：[Java学习笔记](https://blog.csdn.net/column/details/javago.html)[Java深入](https://blog.csdn.net/column/details/myjava.html)









### 关于ThreadLocal

Java提供了java.lang.ThreadLocal，主要是用于解决**多线程情况下变量安全**的问题。

当使用ThreadLocal维护变量时，ThreadLocal为每个使用该变量的线程提供独立的变量副本，所以每一个线程都可以独立地改变自己的副本，而不会影响其它线程所对应的副本。




**ThreadLocal提供了4个接口：**

1. void set(Object value) 设置当前局部变量的值

2. Object get()该方法返回当前线程所对应的线程局部变量。
****

3. public void remove()将当前线程局部变量的值删除。线程结束后，会自动回收内存。

4. protected Object initialValue()返回该线程局部变量的初始值
****




**使用场景**

1. 当我们需要维护一个变量，并且这个变量需要在多线程情况下安全

2. 不想使用synchronized复杂的同步机制的
****




### **ThreadLocal使用例子**

看一下下面的一个简单的例子，多线程情况下，更新变量。



```java
public class Test2 {

    public static ThreadLocal<Integer> tl = new ThreadLocal<Integer>();

    /**
     * 入口函数
     * @param args
     */
    public static void main(String[] args) throws Exception {

        for (int i = 0; i < 4; i++) {
            new Thread(new Runnable() {

                @Override
                public void run() {
                    for (int i = 0; i < 4; i++) {
                        Test2.tl.set(i);
                        System.out.println("Thread:" + Thread.currentThread().getId()
                                + " ThreadLocal值:" + Test2.tl.get());
                        try {
                            Thread.sleep(1000);
                        } catch (Exception e) {
                        }
                    }
                }
            }).start();
        }
    }
}
```


结果：





```java
Thread:9 ThreadLocal值:0
Thread:10 ThreadLocal值:0
Thread:12 ThreadLocal值:0
Thread:11 ThreadLocal值:0
Thread:11 ThreadLocal值:1
Thread:10 ThreadLocal值:1
Thread:9 ThreadLocal值:1
Thread:12 ThreadLocal值:1
Thread:11 ThreadLocal值:2
Thread:10 ThreadLocal值:2
Thread:9 ThreadLocal值:2
Thread:12 ThreadLocal值:2
```




### ThreadLocal源码实现

set方法源码。可以看到set方法的时候，会去取**当前的线程信息**。然后放入ThreadLocalMap中。ThreadLocalMap是ThreadLocal类的一个静态内部类，它实现了键值对的设置和获取（对比Map对象来理解），每个线程中都有一个独立的ThreadLocalMap副本，它所存储的值，只能被当前线程读取和修改。ThreadLocal类通过操作每一个线程特有的ThreadLocalMap副本，从而实现了变量访问在不同线程中的隔离。



```java
/**
     * Sets the current thread's copy of this thread-local variable
     * to the specified value.  Most subclasses will have no need to
     * override this method, relying solely on the {@link #initialValue}
     * method to set the values of thread-locals.
     *
     * @param value the value to be stored in the current thread's copy of
     *        this thread-local.
     */
    public void set(T value) {
        Thread t = Thread.currentThread();
        ThreadLocalMap map = getMap(t);
        if (map != null)
            map.set(this, value);
        else
            createMap(t, value);
    }
```


get方法源码。





```java
/**
     * Returns the value in the current thread's copy of this
     * thread-local variable.  If the variable has no value for the
     * current thread, it is first initialized to the value returned
     * by an invocation of the {@link #initialValue} method.
     *
     * @return the current thread's value of this thread-local
     */
    public T get() {
        Thread t = Thread.currentThread();
        ThreadLocalMap map = getMap(t);
        if (map != null) {
            ThreadLocalMap.Entry e = map.getEntry(this);
            if (e != null)
                return (T)e.value;
        }
        return setInitialValue();
    }
```


静态内部类和非静态内部类区别：

1. 内部静态类不需要有指向外部类的引用。但非静态内部类需要持有对外部类的引用。

2. 非静态内部类能够访问外部类的静态和非静态成员。静态类不能访问外部类的非静态成员。他只能访问外部类的静态成员。

3. 一个非静态内部类不能脱离外部类实体被创建，一个非静态内部类可以访问外部类的数据和方法，因为他就在外部类里面。




**ThreadLocalMap就是一个静态内部类，Thread每个线程上都会有一个ThreadLocalMap，所以能实现线程安全。**












