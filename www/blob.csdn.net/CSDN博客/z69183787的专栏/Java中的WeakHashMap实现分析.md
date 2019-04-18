# Java中的WeakHashMap实现分析 - z69183787的专栏 - CSDN博客
2017年01月17日 12:22:14[OkidoGreen](https://me.csdn.net/z69183787)阅读数：962
转：[https://www.dexcoder.com/selfly/article/289](https://www.dexcoder.com/selfly/article/289)
在Java集合中有一种特殊的Map类型：WeakHashMap。 WeakHashMap 继承于AbstractMap，实现了Map接口。 和HashMap一样，WeakHashMap 也是一个散列表，它存储的内容也是键值对(key-value)映射，而且键和值都可以是null。 不过WeakHashMap的键是“弱键”，里面存放了键对象的弱引用，当某个键不再正常使用时，会从WeakHashMap中被自动移除。当一个键对象被垃圾回收，那么相应的值对象的引用会从Map中删除。WeakHashMap能够节约存储空间，可用来缓存那些非必须存在的数据。
那么这个“弱键”的原理呢？大致上是通过WeakReference和ReferenceQueue实现的。 WeakHashMap的key是“弱键”，即是WeakReference类型的；ReferenceQueue是一个队列，它会保存被GC回收的“弱键”。实现步骤是： (01) 新建WeakHashMap，将“键值对”添加到WeakHashMap中。实际上，WeakHashMap是通过数组table保存Entry(键值对)；每一个Entry实际上是一个单向链表，即Entry是键值对链表。 (02) 当某“弱键”不再被其它对象引用，并被GC回收时。在GC回收该“弱键”时，这个“弱键”也同时会被添加到ReferenceQueue(queue)队列中。
 (03) 当下一次我们需要操作WeakHashMap时，会先同步table和queue。table中保存了全部的键值对，而queue中保存被GC回收的键值对；同步它们，就是删除table中被GC回收的键值对。 这就是“弱键”如何被自动从WeakHashMap中删除的步骤了。 和HashMap一样，WeakHashMap是不同步的。可以使用 Collections.synchronizedMap 方法来构造同步的 WeakHashMap。
WeakHashMap源码分析，WeakHashMap维护了一个ReferenceQueue，保存了所有存在引用的Key对象。WeakHashMap.Entry<K,V>中并没有保存Key，只是将Key与ReferenceQueue关联上了。
##### **注：当时发现，明明Entry 使用 WeakReference super方法  ****WeakReference(T referent, ReferenceQueue<? super T> q) 初始化时，****queue中加入的是key对象，为何poll的时候对象x的类型为Entry<X,Y>，后查看源码发现：****若初始化时使用到了ReferenceQueue，当对应的key（**referent**）对象除了WeakReference外没有其他直达的引用时，****遇到GC回收时会调用基类Reference****中的enqueue方法：**
```java
/**
     * Adds this reference object to the queue with which it is registered,
     * if any.
     *
     * <p> This method is invoked only by Java code; when the garbage collector
     * enqueues references it does so directly, without invoking this method.
     *
     * @return   <code>true</code> if this reference object was successfully
     *           enqueued; <code>false</code> if it was already enqueued or if
     *           it was not registered with a queue when it was created
     */
    public boolean enqueue() {
        return this.queue.enqueue(this);
    }
    /* -- Constructors -- */
    Reference(T referent) {
        this(referent, null);
    }
    Reference(T referent, ReferenceQueue<? super T> queue) {
        this.referent = referent;
        this.queue = (queue == null) ? ReferenceQueue.NULL : queue;
    }
```
###### **可以看出加入到 queue中的是 this，即继承WeakReference的 Entry对象。那么自然而然，之后的poll拿到的也是Entry而不是Key了。****结论：若一个对象 A 继承了WeakReference，且初始化时使用到了ReferenceQueue**
```java
public WeakReference(T referent, ReferenceQueue<? super T> q) {
        super(referent, q);
    }
```
**那么key，referent（通过使用对象主键） 用来决定当GC 来到时，是否还有其他引用。当确定无其他引用时，加入到queue的对象是 A。**
**同时初始化方法**
**1、WeakReference(T referent)：**
**未用到ReferenceQueue，GC 遇到时 不存在其他引用时直接回收。**
**获取对象：**
****public T get() {****        return this.referent;****    }****
**2、public WeakReference(T referent, ReferenceQueue<? super T> q)**
**GC 遇到时 不存在其他引用时，将上文分析后的**
**继承对象（有继承）或 T（无继承）放入queue，对象类型有所不同。**
**referenceQueue的 poll 方法 最终返回的都是 Reference 的子类：**
**继承的返回  继承对象 this；无继承的返回 weakreference**
- privatefinalReferenceQueue<K> queue =newReferenceQueue<K>();
- // Entry是单向链表。
- // 它是 “WeakHashMap链式存储法”对应的链表。
- // 它实现了Map.Entry 接口，即实现getKey(), getValue(), setValue(V value),
- // equals(Object o), hashCode()这些函数
- privatestaticclassEntry<K, V>extendsWeakReference<K>implements
- Map.Entry<K, V>{
- private V value;
- privatefinalint hash;
- // 指向下一个节点
- privateEntry<K, V>next;
- // 构造函数。
- Entry(K key, V value,ReferenceQueue<K> queue,int hash,
- Entry<K, V>next){
- super(key, queue);
- this.value = value;
- this.hash = hash;
- this.next=next;
- }
- ...
- }
WeakHashMap中有一个私有的expungeStaleEntries（）方法，会在大部分共有方法中被调用。这个方法会将ReferenceQueue中所有失效的引用从Map中去除。
- // 清空table中无用键值对。原理如下：
- // (01) 当WeakHashMap中某个“弱引用的key”由于没有再被引用而被GC收回时，
- // 被回收的“该弱引用key”也被会被添加到"ReferenceQueue(queue)"中。
- // (02) 当我们执行expungeStaleEntries时，
- // 就遍历"ReferenceQueue(queue)"中的所有key
- // 然后就在“WeakReference的table”中删除与“ReferenceQueue(queue)中key”对应的键值对
- privatevoid expungeStaleEntries(){
- Entry<K, V> e;
- while((e =(Entry<K, V>) queue.poll())!=null){
- int h = e.hash;
- int i = indexFor(h, table.length);
- Entry<K, V> prev = table[i];
- Entry<K, V> p = prev;
- while(p !=null){
- Entry<K, V>next= p.next;
- if(p == e){
- if(prev == e)
- 					table[i]=next;
- else
- 					prev.next=next;
- 				e.next=null;// Help GC
- 				e.value =null;// "   "
- 				size--;
- break;
- }
- 			prev = p;
- 			p =next;
- }
- }
- }
需要注意，WeakHashMap的Key是弱引用，Value不是。WeakHashMap不会自动释放失效的弱引用，仅当包含了expungeStaleEntries()的共有方法被调用的时候才会释放。
一个简单的例子：
- publicstaticvoid main(String args[]){
- WeakHashMap<String,String> map =newWeakHashMap<String,String>();
- 	map.put(newString("1"),"1");
- 	map.put("2","2");
- String s =newString("3");
- 	map.put(s,"3");
- while(map.size()>0){
- try{
- Thread.sleep(500);
- }catch(InterruptedException ignored){
- }
- System.out.println("Map Size:"+ map.size());
- System.out.println(map.get("1"));
- System.out.println(map.get("2"));
- System.out.println(map.get("3"));
- System.gc();
- }
- }
运行结果（一直循环当中）：
Map Size:3 1
2
3
Map Size:2
null
2
3
根据String的特性，
元素“1”的key已经没有地方引用了，所以进行了回收。
元素“2”是被放在常量池中的，所以没有被回收。
元素“3”因为还有变量s的引用，所以也没有进行回收。
