# google guava cache 处理 - z69183787的专栏 - CSDN博客
2015年11月17日 10:43:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2973
使用本地内存
Java代码  ![收藏代码](http://patrick002.iteye.com/images/icon_star.png)
- LoadingCache<Long, User> cache = CacheBuilder.newBuilder().maximumSize(100)  
-                 .expireAfterAccess(5, TimeUnit.SECONDS).build(new CacheLoader<Long, User>() {  
- @Override
- public User load(Long key) throws Exception {  
-                         logger.info("fetch from database");  
- return accountManager.getUser(key);  
-                     }  
- 
-                 });  
//设置缓存最大个数为100，缓存过期时间为5秒
回收的参数设置
1. 大小的设置：CacheBuilder.maximumSize(long) CacheBuilder.weigher(Weigher) CacheBuilder.maxumumWeigher(long)
2. 时间：expireAfterAccess(long, TimeUnit) expireAfterWrite(long, TimeUnit)
3. 引用：CacheBuilder.weakKeys() CacheBuilder.weakValues() CacheBuilder.softValues()
4. 明确的删除：invalidate(key) invalidateAll(keys) invalidateAll()
5. 删除监听器：CacheBuilder.removalListener(RemovalListener)
refresh机制
1. LoadingCache.refresh(K) 在生成新的value的时候，旧的value依然会被使用。
2. CacheLoader.reload(K, V) 生成新的value过程中允许使用旧的value
3. CacheBuilder.refreshAfterWrite(long, TimeUnit) 自动刷新cache
类似ConcurrentMap， 数据存在内存而不是file，外部server， 否则可以用memcached（待会儿解释）
如果value是通过正常的func使用key来得到的，那么可以使用CacheLoader，如果需要override，那么使用Callable
正常的使用方法是， LoadingCache.get(K), 将会查询cache，如果cache没有，那么会CacheLoader会将value加入cache，
注意的是，CacheLoader可能会抛出异常，所以如果你的LoadingCache中没有处理异常的话，那么最好使用 getUnchecked(K)方法
getAll(Iterable<? extends K>) 方法可以 批读取， 也是分成一个个key来处理，但是肯定比手工的效率更高， 可以通过override CacheLoader.loadAll 来更好的处理
get(K, Callable<V>)， 这个方法实现了if cached, return; otherwise create, cache and return 模式
即如果cache了返回，如果没有通过callable接口的方法来create，然后cache，最后返回
Java代码  ![收藏代码](http://patrick002.iteye.com/images/icon_star.png)
- Cache<Key, Value> cache = CacheBuilder.newBuilder()  
-     .maximumSize(1000)  
-     .build(); // look Ma, no CacheLoader
- ...  
- try {  
- // If the key wasn't in the "easy to compute" group, we need to
- // do things the hard way.
-   cache.get(key, new Callable<Value>() {  
- @Override
- public Value call() throws AnyException {  
- return doThingsTheHardWay(key);  
-     }  
-   });  
- } catch (ExecutionException e) {  
- thrownew OtherException(e.getCause());  
- }  
直接操作cache
使用cache.put(key,value), 同时可以使用Cache.asMap()来调用所以ConcurrentMap的方法来操作cache，但是通过asMap的数据不会自动loading到cache
三种清出cache的模式size-based eviction, time-based eviction, and reference-based eviction.
size-based：
Java代码  ![收藏代码](http://patrick002.iteye.com/images/icon_star.png)
- LoadingCache<Key, Graph> graphs = CacheBuilder.newBuilder()  
-        .maximumWeight(100000)  
-        .weigher(new Weigher<Key, Graph>() {  
- publicint weigh(Key k, Graph g) {  
- return g.vertices().size();  
-           }  
-         })  
-        .build(  
- new CacheLoader<Key, Graph>() {  
- public Graph load(Key key) { // no checked exception
- return createExpensiveGraph(key);  
-              }  
-            });  
weigher，作用是将g.vertices().size()作为一个单位大小，重载此方法， 同时是在cache创建的时候计算
time-based:
expireAfterAccess(long, TimeUnit)  基于访问（read or write）
expireAfterWrite(long, TimeUnit)基于创建（after create）
reference-based
CacheBuilder.weakKeys()， 使用weak references来建立key，这样的话如果key没有引用了(strong or soft)，key就会被垃圾回收，同时垃圾回收使用==来判断key值
CacheBuilder.weakValues(),  同上，作用到value上
CacheBuilder.softValues() ， 对value进行排序，
主动清出cache
Cache.invalidate(key)
 Cache.invalidateAll(keys)
  Cache.invalidateAll()
clearUp,  cache不会自己clearup，一般需要通过线程定制，定时的clearup
refresh 操作， 与evict不同， 是给key一个new value， 同时如果在refresh时有访问，那么将会返回old value， 而evict则会等待evict结束返回new value
定义refresh一般使用异步的操作
如：
Java代码  ![收藏代码](http://patrick002.iteye.com/images/icon_star.png)
- // Some keys don't need refreshing, and we want refreshes to be done asynchronously.
- LoadingCache<Key, Graph> graphs = CacheBuilder.newBuilder()  
-        .maximumSize(1000)  
-        .refreshAfterWrite(1, TimeUnit.MINUTES)  
-        .build(  
- new CacheLoader<Key, Graph>() {  
- public Graph load(Key key) { // no checked exception
- return getGraphFromDatabase(key);  
-              }  
- 
- public ListenableFuture<Graph> reload(final Key key, Graph prevGraph) {  
- if (neverNeedsRefresh(key)) {  
- return Futures.immediateFuture(prevGraph);  
-                } else {  
- // asynchronous!
- return ListenableFutureTask.create(new Callable<Graph>() {  
- public Graph call() {  
- return getGraphFromDatabase(key);  
-                    }  
-                  });  
-                }  
-              }  
-            });  
 在refreshAfterWrite方法，会调用reload
数据
  提供了一些数据采集的方法
   CacheBuilder.recordStats() 方法启动了 cache的数据收集
    Cache.stats() 返回了一个CacheStats对象， 提供一些数据方法
    hitRate()， 请求点击率
    averageLoadPenalty()， 加载new value，花费的时间， 单位nanosecondes
    evictionCount()， 清除的个数
