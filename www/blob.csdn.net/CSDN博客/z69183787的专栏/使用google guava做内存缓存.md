# 使用google guava做内存缓存 - z69183787的专栏 - CSDN博客
2015年11月17日 10:42:48[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5782
google guava中有cache包，此包提供内存缓存功能。内存缓存需要考虑很多问题，包括并发问题，缓存失效机制，内存不够用时缓存释放，缓存的命中率，缓存的移除等等。 当然这些东西guava都考虑到了。
guava中使用缓存需要先声明一个CacheBuilder对象，并设置缓存的相关参数，然后调用其build方法获得一个Cache接口的实例。请看下面的代码和注释，注意在注释中指定了Cache的各个参数。
```
public static void main(String[] args) throws ExecutionException, InterruptedException{
        //缓存接口这里是LoadingCache，LoadingCache在缓存项不存在时可以自动加载缓存
        LoadingCache<Integer,Student> studentCache
                //CacheBuilder的构造函数是私有的，只能通过其静态方法newBuilder()来获得CacheBuilder的实例
                = CacheBuilder.newBuilder()
                //设置并发级别为8，并发级别是指可以同时写缓存的线程数
                .concurrencyLevel(8)
                //设置写缓存后8秒钟过期
                .expireAfterWrite(8, TimeUnit.SECONDS)
                //设置缓存容器的初始容量为10
                .initialCapacity(10)
                //设置缓存最大容量为100，超过100之后就会按照LRU最近虽少使用算法来移除缓存项
                .maximumSize(100)
                //设置要统计缓存的命中率
                .recordStats()
                //设置缓存的移除通知
                .removalListener(new RemovalListener<Object, Object>() {
                    @Override
                    public void onRemoval(RemovalNotification<Object, Object> notification) {
                        System.out.println(notification.getKey() + " was removed, cause is " + notification.getCause());
                    }
                })
                //build方法中可以指定CacheLoader，在缓存不存在时通过CacheLoader的实现自动加载缓存
                .build(
                        new CacheLoader<Integer, Student>() {
                            @Override
                            public Student load(Integer key) throws Exception {
                                System.out.println("load student " + key);
                                Student student = new Student();
                                student.setId(key);
                                student.setName("name " + key);
                                return student;
                            }
                        }
                );
        for (int i=0;i<20;i++) {
            //从缓存中得到数据，由于我们没有设置过缓存，所以需要通过CacheLoader加载缓存数据
            Student student = studentCache.get(1);
            System.out.println(student);
            //休眠1秒
            TimeUnit.SECONDS.sleep(1);
        }
        System.out.println("cache stats:");
        //最后打印缓存的命中率等 情况
        System.out.println(studentCache.stats().toString());
    }
```
publicstaticvoid main(String[] args)throwsExecutionException,InterruptedException{//缓存接口这里是LoadingCache，LoadingCache在缓存项不存在时可以自动加载缓存LoadingCache<Integer,Student> studentCache
                //CacheBuilder的构造函数是私有的，只能通过其静态方法newBuilder()来获得CacheBuilder的实例=CacheBuilder.newBuilder()//设置并发级别为8，并发级别是指可以同时写缓存的线程数.concurrencyLevel(8)//设置写缓存后8秒钟过期.expireAfterWrite(8,TimeUnit.SECONDS)//设置缓存容器的初始容量为10.initialCapacity(10)//设置缓存最大容量为100，超过100之后就会按照LRU最近虽少使用算法来移除缓存项.maximumSize(100)//设置要统计缓存的命中率.recordStats()//设置缓存的移除通知.removalListener(newRemovalListener<Object,Object>(){@Overridepublicvoid onRemoval(RemovalNotification<Object,Object> notification){System.out.println(notification.getKey()+" was removed, cause is "+ notification.getCause());}})//build方法中可以指定CacheLoader，在缓存不存在时通过CacheLoader的实现自动加载缓存.build(newCacheLoader<Integer,Student>(){@OverridepublicStudent load(Integer key)throwsException{System.out.println("load student "+ key);Student student =newStudent();
                                student.setId(key);
                                student.setName("name "+ key);return student;}});for(int i=0;i<20;i++){//从缓存中得到数据，由于我们没有设置过缓存，所以需要通过CacheLoader加载缓存数据Student student = studentCache.get(1);System.out.println(student);//休眠1秒TimeUnit.SECONDS.sleep(1);}System.out.println("cache stats:");//最后打印缓存的命中率等 情况System.out.println(studentCache.stats().toString());}
以上程序的输出如下：
```
load student 1
Student{id=1, name=name 1}
Student{id=1, name=name 1}
Student{id=1, name=name 1}
Student{id=1, name=name 1}
Student{id=1, name=name 1}
Student{id=1, name=name 1}
Student{id=1, name=name 1}
Student{id=1, name=name 1}
1 was removed, cause is EXPIRED
load student 1
......
Student{id=1, name=name 1}
Student{id=1, name=name 1}
Student{id=1, name=name 1}
Student{id=1, name=name 1}
cache stats:
CacheStats{hitCount=17, missCount=3, loadSuccessCount=3, loadExceptionCount=0, totalLoadTime=1348802, evictionCount=2}
```
1Student{id=1, name=name 1}Student{id=1, name=name 1}Student{id=1, name=name 1}Student{id=1, name=name 1}Student{id=1, name=name 1}Student{id=1, name=name 1}Student{id=1, name=name 1}Student{id=1, name=name 1}1 was removed, cause is EXPIRED
load student 1......Student{id=1, name=name 1}Student{id=1, name=name 1}Student{id=1, name=name 1}Student{id=1, name=name 1}
cache stats:CacheStats{hitCount=17, missCount=3, loadSuccessCount=3, loadExceptionCount=0, totalLoadTime=1348802, evictionCount=2}
看看到在20此循环中命中次数是17次，未命中3次，这是因为我们设定缓存的过期时间是写入后的8秒，所以20秒内会失效两次，另外第一次获取时缓存中也是没有值的，所以才会未命中3次，其他则命中。
guava的内存缓存非常强大，可以设置各种选项，而且很轻量，使用方便。另外还提供了下面一些方法，来方便各种需要：
- `ImmutableMap<K, V> getAllPresent(Iterable<?> keys) `一次获得多个键的缓存值
- `put`和`putAll`方法向缓存中添加一个或者多个缓存项
- `invalidate` 和 `invalidateAll`方法从缓存中移除缓存项
- `asMap()`方法获得缓存数据的`ConcurrentMap<K, V>`快照
- `cleanUp()`清空缓存
- `refresh(Key)` 刷新缓存，即重新取缓存数据，更新缓存
