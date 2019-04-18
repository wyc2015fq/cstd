# HashMap与ConcurrentHashMap的区别 - z69183787的专栏 - CSDN博客
2015年07月01日 22:50:56[OkidoGreen](https://me.csdn.net/z69183787)阅读数：985
从JDK1.2起，就有了HashMap，正如前一篇文章所说，HashMap不是线程安全的，因此多线程操作时需要格外小心。
在JDK1.5中，伟大的Doug Lea给我们带来了concurrent包，从此Map也有安全的了。
![](https://img-blog.csdn.net/20141105220713595?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHVlZmVuZzA3MDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
ConcurrentHashMap具体是怎么实现线程安全的呢，肯定不可能是每个方法加synchronized，那样就变成了HashTable。
从ConcurrentHashMap代码中可以看出，它引入了一个“分段锁”的概念，具体可以理解为把一个大的Map拆分成N个小的HashTable，根据key.hashCode()来决定把key放到哪个HashTable中。
在ConcurrentHashMap中，就是把Map分成了N个Segment，put和get的时候，都是现根据key.hashCode()算出放到哪个Segment中：
![](https://img-blog.csdn.net/20141105221426125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHVlZmVuZzA3MDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20141105221506167?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHVlZmVuZzA3MDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20141105221547351?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHVlZmVuZzA3MDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20141105221959307?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHVlZmVuZzA3MDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
测试程序：
**[java]**[view
 plain](http://blog.csdn.net/xuefeng0707/article/details/40834595#)[copy](http://blog.csdn.net/xuefeng0707/article/details/40834595#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/509432/fork)
- import java.util.concurrent.ConcurrentHashMap;  
- 
- publicclass ConcurrentHashMapTest {  
- 
- privatestatic ConcurrentHashMap<Integer, Integer> map = new ConcurrentHashMap<Integer, Integer>();  
- publicstaticvoid main(String[] args) {  
- new Thread("Thread1"){  
- @Override
- publicvoid run() {  
-                 map.put(3, 33);  
-             }  
-         };  
- 
- new Thread("Thread2"){  
- @Override
- publicvoid run() {  
-                 map.put(4, 44);  
-             }  
-         };  
- 
- new Thread("Thread3"){  
- @Override
- publicvoid run() {  
-                 map.put(7, 77);  
-             }  
-         };  
-         System.out.println(map);  
-     }  
- }  
ConcurrentHashMap中默认是把segments初始化为长度为16的数组。
根据ConcurrentHashMap.segmentFor的算法，3、4对应的Segment都是segments[1]，7对应的Segment是segments[12]。
（1）Thread1和Thread2先后进入Segment.put方法时，Thread1会首先获取到锁，可以进入，而Thread2则会阻塞在锁上：
![](https://img-blog.csdn.net/20141105224157727?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHVlZmVuZzA3MDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
（2）切换到Thread3，也走到Segment.put方法，因为7所存储的Segment和3、4不同，因此，不会阻塞在lock()：
![](https://img-blog.csdn.net/20141105224539328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHVlZmVuZzA3MDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
以上就是ConcurrentHashMap的工作机制，通过把整个Map分为N个Segment（类似HashTable），可以提供相同的线程安全，但是效率提升N倍，默认提升16倍。
