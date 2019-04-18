# WeakHashMap是如何清除不用的key的 - z69183787的专栏 - CSDN博客
2017年01月17日 13:36:47[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1837
先把[问题](http://www.javaeye.com/topic/587995)说清楚:
WeakHashMap是主要通过expungeStaleEntries这个函数的来实现移除其内部不用的条目从而达到的自动释放内存的目的的.基本上只要对WeakHashMap的内容进行访问就会调用这个函数，从而达到清除其内部不在为外部引用的条目。但是如果预先生成了WeakHashMap，而在GC以前又不曾访问该WeakHashMap,那不是就不能释放内存了吗？
对应的两个测试案例:
WeakHashMapTest1:
public class WeakHashMapTest1 {
public static void main(String[] args) throws Exception {
List<WeakHashMap<byte[][], byte[][]>> maps = new ArrayList<WeakHashMap<byte[][], byte[][]>>();
for (int i= 0;i<1000;i++) {
WeakHashMap<byte[][], byte[][]> d = new WeakHashMap<byte[][], byte[][]>();
d.put(new byte[1000][1000], new byte[1000][1000]);
maps.add(d);
System.gc();
System.err.println(i);
}
}
}
由于Java默认内存是64M，所以再不改变内存参数的情况下，该测试跑不了几步循环就内存溢出了。果不其然，WeakHashMap这个时候并没有自动帮我们释放不用的内存。
WeakHashMapTest2:
public class WeakHashMapTest2 {
    public static void main(String[] args) throws Exception {
        List<WeakHashMap<byte[][], byte[][]>> maps = new ArrayList<WeakHashMap<byte[][], byte[][]>>();
        for (int i= 0;i<1000;i++) {
            WeakHashMap<byte[][], byte[][]> d = new WeakHashMap<byte[][], byte[][]>();
            d.put(new byte[1000][1000], new byte[1000][1000]);
            maps.add(d);
            System.gc();
            System.err.println(i);
            for (int j = 0; j <i; j++) {
                System.err.println(j + " size" + maps.get(j).size());
            }
        }
    }
} 
这次测试输出正常,不在出现内存溢出问题.
总结来说：WeakHashMap并不是你啥也干他就能自动释放内部不用的对象的，而是在你访问它的内容的时候释放内部不用的对象
问题讲清楚了,现在我们来梳理一下.了解清楚其中的奥秘.
WeakHashMap实现弱引用，是因为它的Entry<K,V>是继承自WeakReference<K>的
在WeakHashMap$Entry<K,V>的类定义及构造函数里面是这样写的：
private static class Entry<K,V>
    extends WeakReference<K>
    implements Map.Entry<K,V> Entry(K key, V value, ReferenceQueue<K> queue,int hash, Entry<K,V> next) { 
super(key, queue); 
this.value= value;
this.hash= hash;
this.next= next;
}
请注意它构造父类的语句：“super(key,
 queue);”，传入的是key，因此key才是进行弱引用的，value是直接强引用关联在this.value之中.在System.gc()时，key中的byte数组进行了回收,而value依然保持(value被强关联到entry上,entry又关联在map中,map关联在arrayList中.).
如何证明key中的byte被回收了呢?可以通过内存溢出时导出的内存镜像进行分析,也可以通过如下的小测试得出结论:
把上面的value用小对象代替，
for (int i= 0;i<10000;i++) { 
    WeakHashMap<byte[][], Object> d = new WeakHashMap<byte[][], Object>(); 
    d.put(new byte[1000][1000], new Object()); 
    maps.add(d); System.gc(); 
    System.err.println(i); 
} 
上面的代码，即使执行10000次也没有问题，证明key中的byte数组确实被回收了。
for循环中每次都new一个新的WeakHashMap，在put操作后，虽然GC将WeakReference的key中的byte数组回收了，并将事件通知到了ReferenceQueue，但后续却没有相应的动作去触发 WeakHashMap 去处理 ReferenceQueue
所以 WeakReference 包装的key依然存在在WeakHashMap中，其对应的value也当然存在。
那value是何时被清除的呢?
对两个例子进行分析可知,例子二中的maps.get(j).size()触发了value的回收,那又如何触发的呢.查看WeakHashMap源码可知,size方法调用了expungeStaleEntries方法,该方法对vm要回收的的entry(quene中)进行遍历,并将entry的value置空,回收了内存.
所以效果是key在GC的时候被清除,value在key清除后访问WeakHashMap被清除.
疑问:key的quene与map的quene是同一个quene,poll操作会减少一个reference,那问题是key如果先被清除,expungeStaleEntries遍历quene时那个被回收的key对应的entry还能取出来么???
关于执行System.GC时,key中的byte数据如何被回收了,请见WeakReference
 referenceQuene
