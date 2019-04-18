# java:强引用,软引用,弱引用和虚引用 - weixin_33985507的博客 - CSDN博客
2019年02月10日 19:01:42[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
## 强引用
如果一个对象具有强引用,垃圾回收器绝不会回收它.当内存空间不足时,java虚拟机宁愿抛出OutOfMemoryError错误,使程序异常终止,也不会靠随意回收具有强引用的对象来解决内存不足问题.
## 软引用 SoftReference<T>
如果内存空间充足,垃圾回收器就不会回收它;如果内存空间不足,就会回收这些对象的内存.只要垃圾回收器没有回收它,该对象就可以继续被应用程序使用.软引用可以实现内存敏感的高速缓存.
## 弱引用 Weakreference<T>
只具有弱引用的对象具有更短的生命周期.在垃圾回收器线程扫描它所管辖的内存区域的过程中,一旦发现了只具有弱引用的对象,不管当前内存空间足够与否,都会回收它的内存.不过,垃圾回收器是一个优先级很低对的线程,因此不一定会很快发现那些只具有弱引用的对象.
## 虚引用 PhantomReference<T>
虚引用并不会决定对象的生命周期.如果一个对象仅持有虚引用,那么他就和没有任何引用一样,在任何时候都可能被回收.
## 引用队列 ReferenceQueue<T>
引用队列可以和软引用,弱引用和虚引用联合使用.并且虚引用必须和引用队列联合使用.如果软引用(弱引用或者虚引用)所引用的对象被垃圾回收,java虚拟机就会把这个引用加入到与之关联的引用队列中.
```
String reference = "hello";
        //引用队列
        ReferenceQueue<String> queue = new ReferenceQueue<>();
        //弱引用
        WeakReference<String> softReference = new WeakReference<String>(reference,queue);
        reference = null;
        //取出弱引用持有的String对象
        String str = softReference.get();
        //取出引用队列中保存的引用对象
        Reference<? extends String> poll = queue.poll();
```
