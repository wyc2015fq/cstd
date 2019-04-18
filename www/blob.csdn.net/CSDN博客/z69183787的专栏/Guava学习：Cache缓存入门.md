# Guava学习：Cache缓存入门 - z69183787的专栏 - CSDN博客
2019年03月27日 16:10:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：32
[https://segmentfault.com/a/1190000007300118](https://segmentfault.com/a/1190000007300118)
## 一、什么是缓存？
根据科普中国的定义，`缓存就是数据交换的缓冲区（称作Cache）`，当某一硬件要读取数据时，会首先从缓存中查找需要的数据，如果找到了则直接执行，找不到的话则从内存中找。由于缓存的运行速度比内存快得多，故缓存的作用就是帮助硬件更快地运行。
在这里，我们借用了硬件缓存的概念，当在Java程序中计算或查询数据的代价很高，并且对同样的计算或查询条件需要不止一次获取数据的时候，就应当考虑使用缓存。换句话说，缓存就是以空间换时间，大部分应用在各种IO，数据库查询等耗时较长的应用当中。
## 二、缓存原理
当获取数据时，程序将先从一个存储在内存中的数据结构中获取数据。如果数据不存在，则在磁盘或者数据库中获取数据并存入到数据结构当中。之后程序需要再次获取数据时，则会先查询这个数据结构。从内存中获取数据时间明显小于通过IO获取数据，这个数据结构就是缓存的实现。
这里引入一个概念，缓存`命中率`：从缓存中获取到数据的次数/全部查询次数，命中率越高说明这个缓存的效率好。由于机器内存的限制，缓存一般只能占据有限的内存大小，缓存需要不定期的删除一部分数据，从而保证不会占据大量内存导致机器崩溃。
如何提高命中率呢？那就得从删除一部分数据着手了。目前有三种删除数据的方式，分别是：`FIFO（先进先出）`、`LFU（定期淘汰最少使用次数）`、`LRU（淘汰最长时间未被使用）`。
## 三、GuavaCache工作方式
GuavaCache的**工作流程**：`获取数据->如果存在，返回数据->计算获取数据->存储返回`。由于特定的工作流程，使用者必须在创建Cache或者获取数据时指定不存在数据时应当怎么获取数据。GuavaCache采用LRU的工作原理，使用者必须指定缓存数据的大小，当超过缓存大小时，必定引发数据删除。GuavaCache还可以让用户指定缓存数据的过期时间，刷新时间等等很多有用的功能。
## 四、GuavaCache使用Demo
### 4.1 简单使用
有人说我就想简简单单的使用cache，就像Map那样方便就行。接下来展示一段简单的使用方式。
首先定义一个需要存储的Bean，对象Man：
```
/**
 * @author jiangmitiao
 * @version V1.0
 * @Title: 标题
 * @Description: Bean
 * @date 2016/10/27 10:01
 */
public class Man {
    //身份证号
    private String id;
    //姓名
    private String name;
    public String getId() {
        return id;
    }
    public void setId(String id) {
        this.id = id;
    }
    public String getName() {
        return name;
    }
    public void setName(String name) {
        this.name = name;
    }
    @Override
    public String toString() {
        return "Man{" +
                "id='" + id + '\'' +
                ", name='" + name + '\'' +
                '}';
    }
}
```
接下来我们写一个Demo：
```
import com.google.common.cache.*;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import java.util.concurrent.*;
/**
 * @author jiangmitiao
 * @version V1.0
 * @Description: Demo
 * @date 2016/10/27 10:00
 */
public class GuavaCachDemo {
    private LoadingCache<String,Man> loadingCache;
    //loadingCache
    public void InitLoadingCache() {
        //指定一个如果数据不存在获取数据的方法
        CacheLoader<String, Man> cacheLoader = new CacheLoader<String, Man>() {
            @Override
            public Man load(String key) throws Exception {
                //模拟mysql操作
                Logger logger = LoggerFactory.getLogger("LoadingCache");
                logger.info("LoadingCache测试 从mysql加载缓存ing...(2s)");
                Thread.sleep(2000);
                logger.info("LoadingCache测试 从mysql加载缓存成功");
                Man tmpman = new Man();
                tmpman.setId(key);
                tmpman.setName("其他人");
                if (key.equals("001")) {
                    tmpman.setName("张三");
                    return tmpman;
                }
                if (key.equals("002")) {
                    tmpman.setName("李四");
                    return tmpman;
                }
                return tmpman;
            }
        };
        //缓存数量为1，为了展示缓存删除效果
        loadingCache = CacheBuilder.newBuilder().maximumSize(1).build(cacheLoader);
    }
    //获取数据，如果不存在返回null
    public Man getIfPresentloadingCache(String key){
        return loadingCache.getIfPresent(key);
    }
    //获取数据，如果数据不存在则通过cacheLoader获取数据，缓存并返回
    public Man getCacheKeyloadingCache(String key){
        try {
            return loadingCache.get(key);
        } catch (ExecutionException e) {
            e.printStackTrace();
        }
        return null;
    }
    //直接向缓存put数据
    public void putloadingCache(String key,Man value){
        Logger logger = LoggerFactory.getLogger("LoadingCache");
        logger.info("put key :{} value : {}",key,value.getName());
        loadingCache.put(key,value);
    }
}
```
接下来，我们写一些测试方法，检测一下
```
public class Test {
    public static void main(String[] args){
        GuavaCachDemo cachDemo = new GuavaCachDemo()
        System.out.println("使用loadingCache");
        cachDemo.InitLoadingCache();
        System.out.println("使用loadingCache get方法  第一次加载");
        Man man = cachDemo.getCacheKeyloadingCache("001");
        System.out.println(man);
        System.out.println("\n使用loadingCache getIfPresent方法  第一次加载");
        man = cachDemo.getIfPresentloadingCache("002");
        System.out.println(man);
        System.out.println("\n使用loadingCache get方法  第一次加载");
        man = cachDemo.getCacheKeyloadingCache("002");
        System.out.println(man);
        System.out.println("\n使用loadingCache get方法  已加载过");
        man = cachDemo.getCacheKeyloadingCache("002");
        System.out.println(man);
        System.out.println("\n使用loadingCache get方法  已加载过,但是已经被剔除掉,验证重新加载");
        man = cachDemo.getCacheKeyloadingCache("001");
        System.out.println(man);
        System.out.println("\n使用loadingCache getIfPresent方法  已加载过");
        man = cachDemo.getIfPresentloadingCache("001");
        System.out.println(man);
        System.out.println("\n使用loadingCache put方法  再次get");
        Man newMan = new Man();
        newMan.setId("001");
        newMan.setName("额外添加");
        cachDemo.putloadingCache("001",newMan);
        man = cachDemo.getCacheKeyloadingCache("001");
        System.out.println(man);
    }
}
```
测试结果如下：
![](https://img-blog.csdnimg.cn/2019032718004784.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3o2OTE4Mzc4Nw==,size_16,color_FFFFFF,t_70)
### 4.2 高级特性
```
由于目前使用有局限性，接下来只讲我用到的一些方法。
```
我来演示一下GuavaCache自带的两个Cache
```
GuavaCacheDemo.java
import com.google.common.cache.*;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import java.util.concurrent.*;
/**
 * @author jiangmitiao
 * @version V1.0
 * @Description: Demo
 * @date 2016/10/27 10:00
 */
public class GuavaCachDemo {
    private Cache<String, Man> cache;
    private LoadingCache<String,Man> loadingCache;
    private RemovalListener<String, Man> removalListener;
    public void Init(){
        //移除key-value监听器
        removalListener = new RemovalListener<String, Man>(){
            public void onRemoval(RemovalNotification<String, Man> notification) {
                Logger logger = LoggerFactory.getLogger("RemovalListener");
                logger.info(notification.getKey()+"被移除");
                //可以在监听器中获取key,value,和删除原因
                notification.getValue();
                notification.getCause();//EXPLICIT、REPLACED、COLLECTED、EXPIRED、SIZE
            }};
        //可以使用RemovalListeners.asynchronous方法将移除监听器设为异步方法
        //removalListener = RemovalListeners.asynchronous(removalListener, new ThreadPoolExecutor(1,1,1000, TimeUnit.MINUTES,new ArrayBlockingQueue<Runnable>(1)));
    }
    //loadingCache
    public void InitLoadingCache() {
        //指定一个如果数据不存在获取数据的方法
        CacheLoader<String, Man> cacheLoader = new CacheLoader<String, Man>() {
            @Override
            public Man load(String key) throws Exception {
                //模拟mysql操作
                Logger logger = LoggerFactory.getLogger("LoadingCache");
                logger.info("LoadingCache测试 从mysql加载缓存ing...(2s)");
                Thread.sleep(2000);
                logger.info("LoadingCache测试 从mysql加载缓存成功");
                Man tmpman = new Man();
                tmpman.setId(key);
                tmpman.setName("其他人");
                if (key.equals("001")) {
                    tmpman.setName("张三");
                    return tmpman;
                }
                if (key.equals("002")) {
                    tmpman.setName("李四");
                    return tmpman;
                }
                return tmpman;
            }
        };
        //缓存数量为1，为了展示缓存删除效果
        loadingCache = CacheBuilder.newBuilder().
                //设置2分钟没有获取将会移除数据
                expireAfterAccess(2, TimeUnit.MINUTES).
                //设置2分钟没有更新数据则会移除数据
                expireAfterWrite(2, TimeUnit.MINUTES).
                //每1分钟刷新数据
                refreshAfterWrite(1,TimeUnit.MINUTES).
                //设置key为弱引用
                weakKeys().
//                weakValues().//设置存在时间和刷新时间后不能再次设置
//                softValues().//设置存在时间和刷新时间后不能再次设置
                maximumSize(1).
                removalListener(removalListener).
                build(cacheLoader);
    }
    //获取数据，如果不存在返回null
    public Man getIfPresentloadingCache(String key){
        return loadingCache.getIfPresent(key);
    }
    //获取数据，如果数据不存在则通过cacheLoader获取数据，缓存并返回
    public Man getCacheKeyloadingCache(String key){
        try {
            return loadingCache.get(key);
        } catch (ExecutionException e) {
            e.printStackTrace();
        }
        return null;
    }
    //直接向缓存put数据
    public void putloadingCache(String key,Man value){
        Logger logger = LoggerFactory.getLogger("LoadingCache");
        logger.info("put key :{} value : {}",key,value.getName());
        loadingCache.put(key,value);
    }
    public void InitDefault() {
        cache = CacheBuilder.newBuilder().
                expireAfterAccess(2, TimeUnit.MINUTES).
                expireAfterWrite(2, TimeUnit.MINUTES).
//                refreshAfterWrite(1,TimeUnit.MINUTES).//没有cacheLoader的cache不能设置刷新，因为没有指定获取数据的方式
                weakKeys().
//                weakValues().//设置存在时间和刷新时间后不能再次设置
//                softValues().//设置存在时间和刷新时间后不能再次设置
                maximumSize(1).
                removalListener(removalListener).
                build();
    }
    public Man getIfPresentCache(String key){
        return cache.getIfPresent(key);
    }
    public Man getCacheKeyCache(final String key) throws ExecutionException {
        return cache.get(key, new Callable<Man>() {
            public Man call() throws Exception {
                //模拟mysql操作
                Logger logger = LoggerFactory.getLogger("Cache");
                logger.info("Cache测试 从mysql加载缓存ing...(2s)");
                Thread.sleep(2000);
                logger.info("Cache测试 从mysql加载缓存成功");
                Man tmpman = new Man();
                tmpman.setId(key);
                tmpman.setName("其他人");
                if (key.equals("001")) {
                    tmpman.setName("张三");
                    return tmpman;
                }
                if (key.equals("002")) {
                    tmpman.setName("李四");
                    return tmpman;
                }
                return tmpman;
            }
        });
    }
    public void putCache(String key,Man value){
        Logger logger = LoggerFactory.getLogger("Cache");
        logger.info("put key :{} value : {}",key,value.getName());
        cache.put(key,value);
    }
}
```
在这个demo中，分别采用了Guava自带的两个Cache：LocalLoadingCache和LocalManualCache。并且添加了监听器，当数据被删除后会打印日志。
Main：
```
public static void main(String[] args){
    GuavaCachDemo cachDemo = new GuavaCachDemo();
    cachDemo.Init();
    System.out.println("使用loadingCache");
    cachDemo.InitLoadingCache();
    System.out.println("使用loadingCache get方法  第一次加载");
    Man man = cachDemo.getCacheKeyloadingCache("001");
    System.out.println(man);
    System.out.println("\n使用loadingCache getIfPresent方法  第一次加载");
    man = cachDemo.getIfPresentloadingCache("002");
    System.out.println(man);
    System.out.println("\n使用loadingCache get方法  第一次加载");
    man = cachDemo.getCacheKeyloadingCache("002");
    System.out.println(man);
    System.out.println("\n使用loadingCache get方法  已加载过");
    man = cachDemo.getCacheKeyloadingCache("002");
    System.out.println(man);
    System.out.println("\n使用loadingCache get方法  已加载过,但是已经被剔除掉,验证重新加载");
    man = cachDemo.getCacheKeyloadingCache("001");
    System.out.println(man);
    System.out.println("\n使用loadingCache getIfPresent方法  已加载过");
    man = cachDemo.getIfPresentloadingCache("001");
    System.out.println(man);
    System.out.println("\n使用loadingCache put方法  再次get");
    Man newMan = new Man();
    newMan.setId("001");
    newMan.setName("额外添加");
    cachDemo.putloadingCache("001",newMan);
    man = cachDemo.getCacheKeyloadingCache("001");
    System.out.println(man);
    ///////////////////////////////////
    System.out.println("\n\n使用Cache");
    cachDemo.InitDefault();
    System.out.println("使用Cache get方法  第一次加载");
    try {
        man = cachDemo.getCacheKeyCache("001");
    } catch (ExecutionException e) {
        e.printStackTrace();
    }
    System.out.println(man);
    System.out.println("\n使用Cache getIfPresent方法  第一次加载");
    man = cachDemo.getIfPresentCache("002");
    System.out.println(man);
    System.out.println("\n使用Cache get方法  第一次加载");
    try {
        man = cachDemo.getCacheKeyCache("002");
    } catch (ExecutionException e) {
        e.printStackTrace();
    }
    System.out.println(man);
    System.out.println("\n使用Cache get方法  已加载过");
    try {
        man = cachDemo.getCacheKeyCache("002");
    } catch (ExecutionException e) {
        e.printStackTrace();
    }
    System.out.println(man);
    System.out.println("\n使用Cache get方法  已加载过,但是已经被剔除掉,验证重新加载");
    try {
        man = cachDemo.getCacheKeyCache("001");
    } catch (ExecutionException e) {
        e.printStackTrace();
    }
    System.out.println(man);
    System.out.println("\n使用Cache getIfPresent方法  已加载过");
    man = cachDemo.getIfPresentCache("001");
    System.out.println(man);
    System.out.println("\n使用Cache put方法  再次get");
    Man newMan1 = new Man();
    newMan1.setId("001");
    newMan1.setName("额外添加");
    cachDemo.putloadingCache("001",newMan1);
    man = cachDemo.getCacheKeyloadingCache("001");
    System.out.println(man);
}
```
测试结果如下：
![](https://img-blog.csdnimg.cn/20190327180104714.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3o2OTE4Mzc4Nw==,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190327180125617.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3o2OTE4Mzc4Nw==,size_16,color_FFFFFF,t_70)
由上述结果可以表明，GuavaCache可以在数据存储到达指定大小后删除数据结构中的数据。我们可以**设置定期删除而达到定期从数据库、磁盘等其他地方更新数据**等（再次访问时数据不存在重新获取）。也可以采用**定时刷新的方式更新数据**。
还可以设置移除监听器对被删除的数据进行一些操作。通过`RemovalListeners.asynchronous(RemovalListener,Executor)`方法将监听器设为异步，笔者通过实验发现，异步监听不会在删除数据时立刻调用监听器方法。
## 五、GuavaCache结构初探
![](https://img-blog.csdnimg.cn/20190327180136657.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3o2OTE4Mzc4Nw==,size_16,color_FFFFFF,t_70)
类结构图
GuavaCache并不希望我们设置复杂的参数，而让我们采用`建造者模式`创建Cache。GuavaCache分为两种Cache:`Cache`，`LoadingCache`。LoadingCache继承了Cache，他比Cache主要多了get和refresh方法。多这两个方法能干什么呢？
在第四节高级特性demo中，我们看到builder生成不带CacheLoader的Cache实例。在类结构图中其实是生成了`LocalManualCache`类实例。而带CacheLoader的Cache实例生成的是`LocalLoadingCache`。他可以定时刷新数据，因为**获取数据的方法已经作为构造参数方法存入了Cache实例中**。同样，在get时，不需要像LocalManualCache还需要传入一个Callable实例。
实际上，这两个Cache实现类都继承自`LocalCache`，大部分实现都是父类做的。
## 六、总结回顾
缓存加载：CacheLoader、Callable、显示插入(put)
缓存回收：LRU，定时(`expireAfterAccess`，`expireAfterWrite`)，软弱引用，显示删除(Cache接口方法`invalidate`，`invalidateAll`)
监听器：`CacheBuilder.removalListener(RemovalListener)`
清理缓存时间：只有在获取数据时才或清理缓存LRU，使用者可以单起线程采用`Cache.cleanUp()`方法主动清理。
刷新：主动刷新方法`LoadingCache.referesh(K)`
信息统计：`CacheBuilder.recordStats()` 开启Guava Cache的统计功能。`Cache.stats() `返回CacheStats对象。（其中包括命中率等相关信息）
获取当前缓存所有数据：`cache.asMap()`，cache.asMap().get(Object)会刷新数据的访问时间（影响的是：创建时设置的在多久没访问后删除数据）
### LocalManualCache和LocalLoadingCache的选择
`ManualCache`可以在get时动态设置获取数据的方法，而`LoadingCache`可以定时刷新数据。如何取舍？我认为在缓存数据有很多种类的时候采用第一种cache。而数据单一，数据库数据会定时刷新时采用第二种cache。
具体工程中的情况也欢迎大家与我交流，互相学习。
### 参考资料：
[http://www.cnblogs.com/peida/...](http://www.cnblogs.com/peida/p/Guava_Cache.html)
[https://github.com/tiantianga...](https://github.com/tiantiangao/guava-study/blob/master/doc/caches.md)
[http://www.blogjava.net/DLevi...](http://www.blogjava.net/DLevin/archive/2013/10/20/404847.html)
[http://ifeve.com/google-guava...](http://ifeve.com/google-guava-cachesexplained/)
