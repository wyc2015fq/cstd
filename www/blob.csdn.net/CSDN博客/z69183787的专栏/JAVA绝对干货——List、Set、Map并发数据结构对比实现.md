# JAVA绝对干货——List、Set、Map并发数据结构对比实现 - z69183787的专栏 - CSDN博客
2017年07月24日 11:15:36[OkidoGreen](https://me.csdn.net/z69183787)阅读数：340
[http://blog.csdn.net/tianyijavaoracle/article/details/41645393](http://blog.csdn.net/tianyijavaoracle/article/details/41645393)
**[Java](http://lib.csdn.net/base/java)并发[数据结构](http://lib.csdn.net/base/datastructure)对比实现**
在对List、Set、Map并发应用场合，我们可以使用Collections的下面方法将非线程安全List、set、Map转化为线程安全的。但是效率并不是最好的，JDK提供了专门的线程安全List与Set实现类，后面我们将讨论下他们的具体实现。
![](https://img-blog.csdn.net/20141201114953528?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbnlpamF2YW9yYWNsZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20141201115022031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbnlpamF2YW9yYWNsZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20141201115004104?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbnlpamF2YW9yYWNsZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**一、CopyOnWriteArrayList与Vector实现讨论**
CopyOnWriteArrayList与Vector是两个线程安全的List，ArrayList不是线程安全的。下面我们对比下这两个线程安全List的实现方式有什么不同，各有什么优点与缺点。
1. CopyOnWriteArrayList get()方法实现如下图
![](https://img-blog.csdn.net/20141201115114906?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbnlpamF2YW9yYWNsZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20141201115150606?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbnlpamF2YW9yYWNsZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
CopyOnWriteArrayList get方法没有进行任何锁。所以效率要高。
2. Vector的get方法实现如下：
![](https://img-blog.csdn.net/20141201115229703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbnlpamF2YW9yYWNsZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
Vector的get方法增加了锁，所以对比两者get方法的，可以说明CopeOnWriteArrayList读取更高效。
3. 下面我们对比Vector与CopyOnWriteArrayList 在添加数据方面的实现方法对比。
Vector 的add（）方法实现如下图：
![](https://img-blog.csdn.net/20141201115233178?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbnlpamF2YW9yYWNsZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
4. CopyOnWriteArrayList 的add（）方法实现如下图：
![](https://img-blog.csdn.net/20141201115306453?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbnlpamF2YW9yYWNsZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
CopyOnWriteArrayList add方法实现是先进行锁定，再拷贝数组，添加新元素后，再写入，相比Vector单纯的锁定比，CopyOnWriteArrayList 添加方法效率应该低些。所以
并发写多情况下建议用Vector，并发读多的情况下建议用CopyOnWriteArrayList 。
**二、并发Set**
CopyOnWriteArraySet是实现了Set接口线程安全的。内部依赖于CopyOnWriteArrayList，因此适合读多写少的并发。
**三、并发Map**
JDK提供了高并发的Map实现ConcurrentHashMap.它的get是无锁的，但是put是有锁的。
**四、并发Queue**
ConcurrentLinkedQueue是一个适用于高并发场景下 队列。它通过无锁的方式，实现了高并发状态下的高性能，通常它的性能好于BlockingQueue的典型实现ArrayBlockingQueue、LinkedBlockingQueue.
**五、并发Deque**
Deque是一个种双端队列，可以在头部或尾部进行出队入队。
实现类有 ArrayDeque、LinkedBlockingDeque.
其中LinkedBlockingDeque是线程安全的，但是读写都加锁，所以效率不是特别高。
