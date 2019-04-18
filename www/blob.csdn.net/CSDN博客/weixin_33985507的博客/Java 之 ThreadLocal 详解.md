# Java 之 ThreadLocal 详解 - weixin_33985507的博客 - CSDN博客
2019年02月05日 23:02:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
> 
更多并发相关内容，查看==>[Java 线程&并发学习目录](https://www.jianshu.com/p/3cbed7b0d571)
ThreadLocal 在java中是充当“线程本地变量”使用的，在每个线程都会创建该对象从而实现线程之间的隔离，具有一定程度上的线程安全（也就是说不是绝对的线程安全，后面会细说这一点）
```java
public T get()   // 获取当前线程的threadlocal存储的数据
public void set(T value)  // 为当前线程的threadlocal设置数据
public void remove()  // 移除threadlocal数据
protected T initialValue()  // 保护方法，构建初始值
```
接下来依次分析每个方法具体实现细节
```java
public T get() {
    Thread t = Thread.currentThread();
    // 获取当前执行线程t
    ThreadLocalMap map = getMap(t);
    // 这个getMap方法是获取当前线程t持有的ThreadLocalMap对象信息
    if (map != null) {
        ThreadLocalMap.Entry e = map.getEntry(this);
        // map的KEY 是 this 也就是当前这个threadLocal对象本身
        // map的VALUE 是 ThreadLocalMap.Entry 一个entry对象（初次看貌似和hashmap的entry类似）
        if (e != null) {
            // 如果当前的value有数据，则直接取出器entry的value对象，返回
            @SuppressWarnings("unchecked")
            T result = (T)e.value;
            return result;
        }
    }
    // 其余情况全部默认返回初始化值数据，具体看下面的方法
    return setInitialValue();
}
ThreadLocalMap getMap(Thread t) {
    return t.threadLocals;
    // 返回线程t的threadlocals参数信息
}
private T setInitialValue() {
    T value = initialValue();
    // 默认返回null
    Thread t = Thread.currentThread();
    ThreadLocalMap map = getMap(t);
    // 再次获取当前线程的threadlocals对象
    if (map != null)
        map.set(this, value);
        // 设置的值是key：this，value：null
    else
        createMap(t, value);
        // 把当前的值塞入到map中
    return value;
}
```
线程有一个名字叫threadlocals的Map对象，键值对数据是<ThreadLocal,Value>，在获取的时候先看map是否存在，如果存在直接获取键值对信息，否则填充一个默认初始值塞入到线程map中
再来看看ThreadLocalMap 对象的类型
```java
static class ThreadLocalMap {
    /**
     * The entries in this hash map extend WeakReference, using
     * its main ref field as the key (which is always a
     * ThreadLocal object).  Note that null keys (i.e. entry.get()
     * == null) mean that the key is no longer referenced, so the
     * entry can be expunged from table.  Such entries are referred to
     * as "stale entries" in the code that follows.
     */
    static class Entry extends WeakReference<ThreadLocal<?>> {
        /** The value associated with this ThreadLocal. */
        Object value;
        Entry(ThreadLocal<?> k, Object v) {
            super(k);
            value = v;
        }
    }
```
ThreadLocalMap 原来和一般意义上的HashMap并没有太多的关系，只是采取了类似的名称而已，而且需要注意到其使用了WeakReference 弱引用，当发生GC操作的时候其会被回收操作
因为threadlocals是线程的一个对象，那么不同的线程map是也是完全隔离的，也正是因为这一点才使得其具备了**一定程度的线程安全**的特性，不过并不是严格意义上的线程安全。例如下面这个例子说明为什么不是绝对的线程安全
![2064197-11f460f83bf45dfd.jpg](https://upload-images.jianshu.io/upload_images/2064197-11f460f83bf45dfd.jpg)
image
如上图所示在一个JVM里面包含了2个线程A和B，在各自线程中的threadlocals对象分别是a和b，age和sex参数都是基础类型，是完全隔离开的，但是却共同引用了同一个Student对象信息，那么就存在A线程修改Student信息 ，然后B获取的Student信息是A修改之后的可能，打破了**线程隔离**这个特性。
> 
第一点结论：ThreadLocal不具备线程安全的特性，想要拥有线程安全，Value必须是不可变对象
上面有提到ThreadLocalMap中的Entry 是`WeakReference<ThreadLocal<?>>`弱引用，每次GC操作都会被回收的，使得KEY为NULL，从而ThreadLocalMap存在`<NULL, Object>` 的对象信息，**从而出现内存泄露的问题**
为了防止这种问题，所以在使用完threadlocal对象后，需要显示调用remove方法
```java
public void remove() {
     ThreadLocalMap m = getMap(Thread.currentThread());
     if (m != null)
         // 在没有被GC回收期，从ThreadLocalMap 中主动移除该数据
         m.remove(this);
}
 
 private void remove(ThreadLocal<?> key) {
    Entry[] tab = table;
    int len = tab.length;
    int i = key.threadLocalHashCode & (len-1);
    for (Entry e = tab[i];
         e != null;
         e = tab[i = nextIndex(i, len)]) {
        if (e.get() == key) {
            e.clear();
            expungeStaleEntry(i);
             // 如果在tab中找到了对应的键值对，则直接设置其引用为null
            // 也就是使得tab[i] = null ，使得GC能够回收干净，不出现内存泄露的问题
            return;
        }
    }
}
```
> 
第二点结论：使用ThreadLocal完毕后确保不出现内存泄露的问题，需要显示调用remove方法
总结：ThreadLocal可以用来存储一些线程临时的不可变对象，并不能用来处理线程共享，需要明确注意其组成结构
**本人微信公众号（搜索jwfy）欢迎关注**
![2064197-7ce506fc5c96f414.png](https://upload-images.jianshu.io/upload_images/2064197-7ce506fc5c96f414.png)
微信公众号
