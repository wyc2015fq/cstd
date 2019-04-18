# Java通用对象池GenericObjectPool原理总结和自定义的Jedis管理池例子 - z69183787的专栏 - CSDN博客
2018年09月09日 21:44:45[OkidoGreen](https://me.csdn.net/z69183787)阅读数：190
[https://blog.csdn.net/qq838642798/article/details/54603450](https://blog.csdn.net/qq838642798/article/details/54603450)
在java中，有很多池管理的概念，典型的如线程池，数据库连接池，查看源码会发现这些池管理类都是继承于GenericObjectPool<T>，所以了解GenericObjectPool的工作机制，对进一步理解java中常用的池很有帮助，这样当我们使用这些池管理类时能做到心中有数，另外当我们需要用自定义的池管理类管理我们一些特殊的对象时，也能实现。
了解GenericObjectPool<T>，应该从其构造方法入手
GenericObjectPool<T>(PooledObjectFactory<T>factory, GenericObjectPoolConfig config)
创建一个对象池，需要传入两个参数PooledObjectFactory<T> factory和GenericObjectPoolConfigconfig，其中config是池管理类的一些参数配置，比如最小对象数量，最大对象数量，超时时间等，由于它有默认值，所以只需对关键的参数进行设置即可，比如最大对象管理数量。
下面以自创建的Jedis管理池为例：
**JedisFactory.java**
```
public class JedisFactory extends BasePooledObjectFactory<Jedis>{
         public PooledObject<Jedis> makeObject(){
                   Jedis jedis = new Jedis("127.0.0.1",6379);
                   jedis.connect();
                   System.out.println(jedis.isConnected());
                   return  new DefaultPooledObject<Jedis>(jedis);
         }
         public void destroyObject(Jedis jedis){
                   jedis.close();
         }
         public boolean validateObject(Jedis jedis) {
                   if(jedis.isConnected()){
                            return true;
                   }else{
                            return false;
                   }
         }
 
         @Override
         public PooledObject<Jedis> wrap(Jedis arg0) {
                   //TODO Auto-generated method stub
                   return null;
         }
         @Override
         public Jedis create() throws Exception {
                   //TODO Auto-generated method stub
                   return null;
         }
}
```
**GenericObjectPoolTest.java**
```java
public class GenericObjectPoolTest extends GenericObjectPool<Jedis>{
         public GenericObjectPoolTest(JedisFactory factory,GenericObjectPoolConfig config){
                   super(factory,config);
         }
         publicstatic void main(String[] args){
                   GenericObjectPoolConfigconfig=new GenericObjectPoolConfig();
                   config.setMinIdle(10);
                   config.setMaxTotal(10);
                   config.setMaxWaitMillis(30000);
                   JedisFactoryjedisFactory=new JedisFactory();
                   GenericObjectPoolTest redisPool=new GenericObjectPoolTest(jedisFactory,config);
                   Jedis jedis=null;
                   try{
                            jedis=redisPool.borrowObject();//获取池中对象
                            jedis.select(0);
                            jedis.set("test","pooltest");
                            System.out.println(redisPool.getNumActive());
                   }catch (Exception e) {
                            //TODO Auto-generated catch block
                            e.printStackTrace();
                   }finally{
                            redisPool.returnObject(jedis);//归还池中对象
                            System.out.println(redisPool.getNumActive());
                   }
         }
}
```
不需要redis自带的JedisPool池管理类，上面两个文件就实现了简单的Jedis池管理类
-- GenericObjectPool的工作原理后续补充
**GenericObjectPool的工作原理流程：**
**1.构造方法**
当我们执行构造方法时，主要工作就是创建了一个存储对象的LinkedList类型容器，也就是概念意义上的“池”。
**2.从对象池中获取对象**
从上面的例子可以看出,获取池中的对象是通过borrowObject()命令，源码比较复杂，简单而言就是去LinkedList中获取一个对象，如果不存在的话，要调用构造方法中第一个参数Factory工厂类的makeObject()方法去创建一个对象再获取，获取到对象后要调用validateObject方法判断该对象是否是可用的，如果是可用的才拿去使用。LinkedList容器减一。
--这就是为什么我们必须要继承并重写Factory工厂类的makeObject()和validateObject方法
**3.归还对象到线程池**
从上面的例子可以看出,归还对象到线程池是通过returnObject方法实现，简单而言就是先调用validateObject方法判断该对象是否是可用的，如果可用则归还到池中，LinkedList容器加一，如果是不可以的则则调用destroyObject方法进行销毁。
--这就是为什么我们必须要继承并重写Factory工厂类的destroyObject方法。
最简单的流程就是这样了，由于取和还的流程步骤都在borrowObject和returnObject方法中固定的，所以我们只要重写Factory工厂类的makeObject()和validateObject以及destroyObject方法即可实现最简单的池的管理控制，通过构造方法传入该Factory工厂类对象则可以创建最简单的对象池管理类。这算是比较好的解耦设计模式。
补充：
borrowObject中还要一重要步骤，就是activateObject(Objectobj)激活对象，只要成功激活可用的对象才会被使用，否则就直接destroyObject，所谓的激活就是对特定对象分配它需要的资源，并非所有类型对象都需要，所以对于不需要的类型而言不重写也可以。另外还有一步，当池中的对象已经被取光，但是创建的活跃对象数量已经超过config设置的最大值MaxAtive时，就不能再创建对象，只能阻塞等待其他使用者归还对象。阻塞用wait()实现。
returnObject中也还有一重要步骤，就是passivateObject(Objectobj)钝化对象，所谓钝化就是对特定对象释放它占用的资源，并非所有类型对象都需要，所以对于不需要的类型而言不重写也可以。归还时判断在池中可以对象数量是否超过config设置的最大值MaxIdle,超过则认为池中对象足够，不需要归还，直接销毁。另外还有一步，就是归还对象后调用notify方法唤醒等待对象的线程。
--所以要是实现更完善的池管理类还需要重写activateObject和passivateObject方法
下面流程图说明：
![](https://img-blog.csdn.net/20170118180846954)
