# Hibernate 之 oscache实现二级缓存 - z69183787的专栏 - CSDN博客
2013年11月08日 09:24:41[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1412
1.导入oscache包。
2.sessionFactory结点中加入属性：
   <prop key="hibernate.cache.use_second_level_cache">true</prop> 这个属性默认是true的
   <prop key="hibernate.cache.provider_class">org.hibernate.cache.OSCacheProvider</prop>
3.src下面添加文件oscache.properies  
4.
（方式一）
   在要实现缓存的映射文件中加入
   <cache usage="read-only"/>
   read-only:只在读取的时候使用二级缓存
   read-write:在读取和写入的时候使用二级缓存，可以保证并发的正确性
   nonstrict-read-write:非严格的读写，很少有并发出现，或者并发时读写到失效的数据也没关系时使用
   transactional:oscache不支持
（方式二）
   在配置文件中为每个要使用二级缓存的对象加入
   <class-cache class="包路径" useage="read-only"/>
