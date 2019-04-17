# Redis整合及使用参考文档 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年03月20日 15:51:33[boonya](https://me.csdn.net/boonya)阅读数：626








From：[https://github.com/redisson/redisson/wiki/%E7%9B%AE%E5%BD%95](https://github.com/redisson/redisson/wiki/%E7%9B%AE%E5%BD%95)



- [. 概述](https://github.com/redisson/redisson/wiki/1.-%E6%A6%82%E8%BF%B0)
- [2. 配置方法](https://github.com/redisson/redisson/wiki/2.-%E9%85%8D%E7%BD%AE%E6%96%B9%E6%B3%95)- [2.1. 程序化配置](https://github.com/redisson/redisson/wiki/2.-%E9%85%8D%E7%BD%AE%E6%96%B9%E6%B3%95#21-%E7%A8%8B%E5%BA%8F%E5%8C%96%E9%85%8D%E7%BD%AE%E6%96%B9%E6%B3%95)
- [2.2. 文件方式配置](https://github.com/redisson/redisson/wiki/2.-%E9%85%8D%E7%BD%AE%E6%96%B9%E6%B3%95#22-%E6%96%87%E4%BB%B6%E6%96%B9%E5%BC%8F%E9%85%8D%E7%BD%AE)- [2.2.1 通过JSON或YAML格式配置](https://github.com/redisson/redisson/wiki/2.-%E9%85%8D%E7%BD%AE%E6%96%B9%E6%B3%95#221-%E9%80%9A%E8%BF%87json%E6%88%96yaml%E6%A0%BC%E5%BC%8F%E9%85%8D%E7%BD%AE)
- [2.2.2 通过Spring XML命名空间配置](https://github.com/redisson/redisson/wiki/2.-%E9%85%8D%E7%BD%AE%E6%96%B9%E6%B3%95#222-%E9%80%9A%E8%BF%87spring-xml%E5%91%BD%E5%90%8D%E7%A9%BA%E9%97%B4%E9%85%8D%E7%BD%AE)

- [2.3. 常用设置](https://github.com/redisson/redisson/wiki/2.-%E9%85%8D%E7%BD%AE%E6%96%B9%E6%B3%95#23-%E5%B8%B8%E7%94%A8%E8%AE%BE%E7%BD%AE)
- [2.4. 集群模式](https://github.com/redisson/redisson/wiki/2.-%E9%85%8D%E7%BD%AE%E6%96%B9%E6%B3%95#24-%E9%9B%86%E7%BE%A4%E6%A8%A1%E5%BC%8F)- [2.4.1. 集群设置](https://github.com/redisson/redisson/wiki/2.-%E9%85%8D%E7%BD%AE%E6%96%B9%E6%B3%95#241-%E9%9B%86%E7%BE%A4%E8%AE%BE%E7%BD%AE)
- [2.4.2. 通过JSON、YAML和Spring
 XML文件配置集群模式](https://github.com/redisson/redisson/wiki/2.-%E9%85%8D%E7%BD%AE%E6%96%B9%E6%B3%95#242-%E9%80%9A%E8%BF%87jsonyaml%E5%92%8Cspring-xml%E6%96%87%E4%BB%B6%E9%85%8D%E7%BD%AE%E9%9B%86%E7%BE%A4%E6%A8%A1%E5%BC%8F)

- [2.5. 云托管模式](https://github.com/redisson/redisson/wiki/2.-%E9%85%8D%E7%BD%AE%E6%96%B9%E6%B3%95#25-%E4%BA%91%E6%89%98%E7%AE%A1%E6%A8%A1%E5%BC%8F)- [2.5.1. 云托管模式设置](https://github.com/redisson/redisson/wiki/2.-%E9%85%8D%E7%BD%AE%E6%96%B9%E6%B3%95#251-%E4%BA%91%E6%89%98%E7%AE%A1%E6%A8%A1%E5%BC%8F%E8%AE%BE%E7%BD%AE)
- [2.5.2. 通过JSON、YAML和Spring
 XML文件配置云托管模式](https://github.com/redisson/redisson/wiki/2.-%E9%85%8D%E7%BD%AE%E6%96%B9%E6%B3%95#252-%E9%80%9A%E8%BF%87jsonyaml%E5%92%8Cspring-xml%E6%96%87%E4%BB%B6%E9%85%8D%E7%BD%AE%E4%BA%91%E6%89%98%E7%AE%A1%E6%A8%A1%E5%BC%8F)

- [2.6. 单Redis节点模式](https://github.com/redisson/redisson/wiki/2.-%E9%85%8D%E7%BD%AE%E6%96%B9%E6%B3%95#26-%E5%8D%95redis%E8%8A%82%E7%82%B9%E6%A8%A1%E5%BC%8F)- [2.6.1. 单节点设置](https://github.com/redisson/redisson/wiki/2.-%E9%85%8D%E7%BD%AE%E6%96%B9%E6%B3%95#261-%E5%8D%95%E8%8A%82%E7%82%B9%E8%AE%BE%E7%BD%AE)
- [2.6.2. 通过JSON、YAML和Spring
 XML文件配置单节点模式](https://github.com/redisson/redisson/wiki/2.-%E9%85%8D%E7%BD%AE%E6%96%B9%E6%B3%95#262-%E9%80%9A%E8%BF%87jsonyaml%E5%92%8Cspring-xml%E6%96%87%E4%BB%B6%E9%85%8D%E7%BD%AE%E5%8D%95%E8%8A%82%E7%82%B9%E6%A8%A1%E5%BC%8F)

- [2.7. 哨兵模式](https://github.com/redisson/redisson/wiki/2.-%E9%85%8D%E7%BD%AE%E6%96%B9%E6%B3%95#27-%E5%93%A8%E5%85%B5%E6%A8%A1%E5%BC%8F)- [2.7.1. 哨兵模式设置](https://github.com/redisson/redisson/wiki/2.-%E9%85%8D%E7%BD%AE%E6%96%B9%E6%B3%95#271-%E5%93%A8%E5%85%B5%E6%A8%A1%E5%BC%8F%E8%AE%BE%E7%BD%AE)
- [2.7.2. 通过JSON、YAML和Spring
 XML文件配置哨兵模式](https://github.com/redisson/redisson/wiki/2.-%E9%85%8D%E7%BD%AE%E6%96%B9%E6%B3%95#272-%E9%80%9A%E8%BF%87jsonyaml%E5%92%8Cspring-xml%E6%96%87%E4%BB%B6%E9%85%8D%E7%BD%AE%E5%93%A8%E5%85%B5%E6%A8%A1%E5%BC%8F)

- [2.8. 主从模式](https://github.com/redisson/redisson/wiki/2.-%E9%85%8D%E7%BD%AE%E6%96%B9%E6%B3%95#28-%E4%B8%BB%E4%BB%8E%E6%A8%A1%E5%BC%8F)- [2.8.1. 主从模式设置](https://github.com/redisson/redisson/wiki/2.-%E9%85%8D%E7%BD%AE%E6%96%B9%E6%B3%95#281-%E4%B8%BB%E4%BB%8E%E6%A8%A1%E5%BC%8F%E8%AE%BE%E7%BD%AE)
- [2.8.2. 通过JSON、YAML和Spring
 XML文件配置主从模式](https://github.com/redisson/redisson/wiki/2.-%E9%85%8D%E7%BD%AE%E6%96%B9%E6%B3%95#282-%E9%80%9A%E8%BF%87jsonyaml%E5%92%8Cspring-xml%E6%96%87%E4%BB%B6%E9%85%8D%E7%BD%AE%E4%B8%BB%E4%BB%8E%E6%A8%A1%E5%BC%8F)


- [3. 程序接口调用方式](https://github.com/redisson/redisson/wiki/3.-%E7%A8%8B%E5%BA%8F%E6%8E%A5%E5%8F%A3%E8%B0%83%E7%94%A8%E6%96%B9%E5%BC%8F)- [3.1. 异步执行方式](https://github.com/redisson/redisson/wiki/3.-%E7%A8%8B%E5%BA%8F%E6%8E%A5%E5%8F%A3%E8%B0%83%E7%94%A8%E6%96%B9%E5%BC%8F#31-%E5%BC%82%E6%AD%A5%E6%89%A7%E8%A1%8C%E6%96%B9%E5%BC%8F)
- [3.2. 异步流执行方式](https://github.com/redisson/redisson/wiki/3.-%E7%A8%8B%E5%BA%8F%E6%8E%A5%E5%8F%A3%E8%B0%83%E7%94%A8%E6%96%B9%E5%BC%8F#32-%E5%BC%82%E6%AD%A5%E6%B5%81%E6%89%A7%E8%A1%8C%E6%96%B9%E5%BC%8F)

- [4. 数据序列化](https://github.com/redisson/redisson/wiki/4.-%E6%95%B0%E6%8D%AE%E5%BA%8F%E5%88%97%E5%8C%96)
- [5. 单个集合数据分片](https://github.com/redisson/redisson/wiki/5.-%E5%8D%95%E4%B8%AA%E9%9B%86%E5%90%88%E6%95%B0%E6%8D%AE%E5%88%86%E7%89%87)
- [6. 分布式对象](https://github.com/redisson/redisson/wiki/6.-%E5%88%86%E5%B8%83%E5%BC%8F%E5%AF%B9%E8%B1%A1)- [6.1. 通用对象桶（Object Bucket）](https://github.com/redisson/redisson/wiki/6.-%E5%88%86%E5%B8%83%E5%BC%8F%E5%AF%B9%E8%B1%A1#61-%E9%80%9A%E7%94%A8%E5%AF%B9%E8%B1%A1%E6%A1%B6object-bucket)
- [6.2. 二进制流（Binary Stream）](https://github.com/redisson/redisson/wiki/6.-%E5%88%86%E5%B8%83%E5%BC%8F%E5%AF%B9%E8%B1%A1#62-%E4%BA%8C%E8%BF%9B%E5%88%B6%E6%B5%81binary-stream)
- [6.3. 地理空间对象桶（Geospatial Bucket）](https://github.com/redisson/redisson/wiki/6.-%E5%88%86%E5%B8%83%E5%BC%8F%E5%AF%B9%E8%B1%A1#63-%E5%9C%B0%E7%90%86%E7%A9%BA%E9%97%B4%E5%AF%B9%E8%B1%A1%E6%A1%B6geospatial-bucket)
- [6.4. BitSet](https://github.com/redisson/redisson/wiki/6.-%E5%88%86%E5%B8%83%E5%BC%8F%E5%AF%B9%E8%B1%A1#64-bitset)
- [6.5. 原子整长形（AtomicLong）](https://github.com/redisson/redisson/wiki/6.-%E5%88%86%E5%B8%83%E5%BC%8F%E5%AF%B9%E8%B1%A1#65-%E5%8E%9F%E5%AD%90%E6%95%B4%E9%95%BF%E5%BD%A2atomiclong)
- [6.6. 原子双精度浮点数（AtomicDouble）](https://github.com/redisson/redisson/wiki/6.-%E5%88%86%E5%B8%83%E5%BC%8F%E5%AF%B9%E8%B1%A1#66-%E5%8E%9F%E5%AD%90%E5%8F%8C%E7%B2%BE%E5%BA%A6%E6%B5%AE%E7%82%B9%E6%95%B0atomicdouble)
- [6.7. 话题（订阅分发）](https://github.com/redisson/redisson/wiki/6.-%E5%88%86%E5%B8%83%E5%BC%8F%E5%AF%B9%E8%B1%A1#67-%E8%AF%9D%E9%A2%98%E8%AE%A2%E9%98%85%E5%88%86%E5%8F%91)- [6.7.1. 模糊话题](https://github.com/redisson/redisson/wiki/6.-%E5%88%86%E5%B8%83%E5%BC%8F%E5%AF%B9%E8%B1%A1#671-%E6%A8%A1%E7%B3%8A%E8%AF%9D%E9%A2%98)

- [6.8. 布隆过滤器（Bloom Filter）](https://github.com/redisson/redisson/wiki/6.-%E5%88%86%E5%B8%83%E5%BC%8F%E5%AF%B9%E8%B1%A1#68-%E5%B8%83%E9%9A%86%E8%BF%87%E6%BB%A4%E5%99%A8bloom-filter)
- [6.9. 基数估计算法（HyperLogLog）](https://github.com/redisson/redisson/wiki/6.-%E5%88%86%E5%B8%83%E5%BC%8F%E5%AF%B9%E8%B1%A1#69-%E5%9F%BA%E6%95%B0%E4%BC%B0%E8%AE%A1%E7%AE%97%E6%B3%95hyperloglog)

- [7. 分布式集合](https://github.com/redisson/redisson/wiki/7.-%E5%88%86%E5%B8%83%E5%BC%8F%E9%9B%86%E5%90%88)- [7.1. 映射（Map）](https://github.com/redisson/redisson/wiki/7.-%E5%88%86%E5%B8%83%E5%BC%8F%E9%9B%86%E5%90%88#71-%E6%98%A0%E5%B0%84map)- [7.1.1. 映射（Map）淘汰机制（Eviction）](https://github.com/redisson/redisson/wiki/7.-%E5%88%86%E5%B8%83%E5%BC%8F%E9%9B%86%E5%90%88#711-%E6%98%A0%E5%B0%84map%E6%B7%98%E6%B1%B0%E6%9C%BA%E5%88%B6eviction)
- [7.1.2. 本地缓存映射（LocalCachedMap）](https://github.com/redisson/redisson/wiki/7.-%E5%88%86%E5%B8%83%E5%BC%8F%E9%9B%86%E5%90%88#712-%E6%9C%AC%E5%9C%B0%E7%BC%93%E5%AD%98%E6%98%A0%E5%B0%84localcachedmap)
- [7.1.3. 映射（Map）数据分片](https://github.com/redisson/redisson/wiki/7.-%E5%88%86%E5%B8%83%E5%BC%8F%E9%9B%86%E5%90%88#713-%E6%98%A0%E5%B0%84map%E6%95%B0%E6%8D%AE%E5%88%86%E7%89%87)

- [7.2. 多值映射（Multimap）](https://github.com/redisson/redisson/wiki/7.-%E5%88%86%E5%B8%83%E5%BC%8F%E9%9B%86%E5%90%88#72-%E5%A4%9A%E5%80%BC%E6%98%A0%E5%B0%84multimap)- [7.2.1. 基于集（Set）的多值映射（Multimap）](https://github.com/redisson/redisson/wiki/7.-%E5%88%86%E5%B8%83%E5%BC%8F%E9%9B%86%E5%90%88#721-%E5%9F%BA%E4%BA%8E%E9%9B%86set%E7%9A%84%E5%A4%9A%E5%80%BC%E6%98%A0%E5%B0%84multimap)
- [7.2.2. 基于列表（List）的多值映射（Multimap）](https://github.com/redisson/redisson/wiki/7.-%E5%88%86%E5%B8%83%E5%BC%8F%E9%9B%86%E5%90%88#722-%E5%9F%BA%E4%BA%8E%E5%88%97%E8%A1%A8list%E7%9A%84%E5%A4%9A%E5%80%BC%E6%98%A0%E5%B0%84multimap)
- [7.2.3. 多值映射（Multimap）淘汰机制（Eviction）](https://github.com/redisson/redisson/wiki/7.-%E5%88%86%E5%B8%83%E5%BC%8F%E9%9B%86%E5%90%88#723-%E5%A4%9A%E5%80%BC%E6%98%A0%E5%B0%84multimap%E6%B7%98%E6%B1%B0%E6%9C%BA%E5%88%B6eviction)

- [7.3. 集（Set）](https://github.com/redisson/redisson/wiki/7.-%E5%88%86%E5%B8%83%E5%BC%8F%E9%9B%86%E5%90%88#73-%E9%9B%86set)- [7.3.1. 集（Set）淘汰机制（Eviction）](https://github.com/redisson/redisson/wiki/7.-%E5%88%86%E5%B8%83%E5%BC%8F%E9%9B%86%E5%90%88#731--%E9%9B%86set%E6%B7%98%E6%B1%B0%E6%9C%BA%E5%88%B6eviction)
- [7.3.2. 集（Set）数据分片](https://github.com/redisson/redisson/wiki/7.-%E5%88%86%E5%B8%83%E5%BC%8F%E9%9B%86%E5%90%88#732--%E9%9B%86set%E6%95%B0%E6%8D%AE%E5%88%86%E7%89%87)

- [7.4. 有序集（SortedSet）](https://github.com/redisson/redisson/wiki/7.-%E5%88%86%E5%B8%83%E5%BC%8F%E9%9B%86%E5%90%88#74-%E6%9C%89%E5%BA%8F%E9%9B%86sortedset)
- [7.5. 计分排序集（ScoredSortedSet）](https://github.com/redisson/redisson/wiki/7.-%E5%88%86%E5%B8%83%E5%BC%8F%E9%9B%86%E5%90%88#75-%E8%AE%A1%E5%88%86%E6%8E%92%E5%BA%8F%E9%9B%86scoredsortedset)
- [7.6. 字典排序集（LexSortedSet）](https://github.com/redisson/redisson/wiki/7.-%E5%88%86%E5%B8%83%E5%BC%8F%E9%9B%86%E5%90%88#76-%E5%AD%97%E5%85%B8%E6%8E%92%E5%BA%8F%E9%9B%86lexsortedset)
- [7.7. 列表（List）](https://github.com/redisson/redisson/wiki/7.-%E5%88%86%E5%B8%83%E5%BC%8F%E9%9B%86%E5%90%88#77-%E5%88%97%E8%A1%A8list)
- [7.8. 列队（Queue）](https://github.com/redisson/redisson/wiki/7.-%E5%88%86%E5%B8%83%E5%BC%8F%E9%9B%86%E5%90%88#78-%E5%88%97%E9%98%9Fqueue)
- [7.9. 双端队列（Deque）](https://github.com/redisson/redisson/wiki/7.-%E5%88%86%E5%B8%83%E5%BC%8F%E9%9B%86%E5%90%88#79-%E5%8F%8C%E7%AB%AF%E9%98%9F%E5%88%97deque)
- [7.10. 阻塞队列（Blocking Queue）](https://github.com/redisson/redisson/wiki/7.-%E5%88%86%E5%B8%83%E5%BC%8F%E9%9B%86%E5%90%88#710-%E9%98%BB%E5%A1%9E%E9%98%9F%E5%88%97blocking-queue)
- [7.11. 有界阻塞列队（Bounded Blocking Queue）](https://github.com/redisson/redisson/wiki/7.-%E5%88%86%E5%B8%83%E5%BC%8F%E9%9B%86%E5%90%88#711-%E6%9C%89%E7%95%8C%E9%98%BB%E5%A1%9E%E5%88%97%E9%98%9Fbounded-blocking-queue)
- [7.12. 阻塞双端列队（Blocking Deque）](https://github.com/redisson/redisson/wiki/7.-%E5%88%86%E5%B8%83%E5%BC%8F%E9%9B%86%E5%90%88#712-%E9%98%BB%E5%A1%9E%E5%8F%8C%E7%AB%AF%E5%88%97%E9%98%9Fblocking-deque)
- [7.13. 阻塞公平列队（Blocking Fair Queue）](https://github.com/redisson/redisson/wiki/7.-%E5%88%86%E5%B8%83%E5%BC%8F%E9%9B%86%E5%90%88#713-%E9%98%BB%E5%A1%9E%E5%85%AC%E5%B9%B3%E5%88%97%E9%98%9Fblocking-fair-queue)
- [7.14. 延迟列队（Delayed Queue）](https://github.com/redisson/redisson/wiki/7.-%E5%88%86%E5%B8%83%E5%BC%8F%E9%9B%86%E5%90%88#714-%E5%BB%B6%E8%BF%9F%E5%88%97%E9%98%9Fdelayed-queue)
- [7.15. 优先队列（Priority Queue）](https://github.com/redisson/redisson/wiki/7.-%E5%88%86%E5%B8%83%E5%BC%8F%E9%9B%86%E5%90%88#715-%E4%BC%98%E5%85%88%E9%98%9F%E5%88%97priority-queue)
- [7.16. 优先双端队列（Priority Deque）](https://github.com/redisson/redisson/wiki/7.-%E5%88%86%E5%B8%83%E5%BC%8F%E9%9B%86%E5%90%88#716-%E4%BC%98%E5%85%88%E5%8F%8C%E7%AB%AF%E9%98%9F%E5%88%97priority-deque)

- [8. 分布式锁（Lock）和同步器（Synchronizer）](https://github.com/redisson/redisson/wiki/8.-%E5%88%86%E5%B8%83%E5%BC%8F%E9%94%81%E5%92%8C%E5%90%8C%E6%AD%A5%E5%99%A8)- [8.1. 可重入锁（Reentrant Lock）](https://github.com/redisson/redisson/wiki/8.-%E5%88%86%E5%B8%83%E5%BC%8F%E9%94%81%E5%92%8C%E5%90%8C%E6%AD%A5%E5%99%A8#81-%E5%8F%AF%E9%87%8D%E5%85%A5%E9%94%81reentrant-lock)
- [8.2. 公平锁（Fair Lock）](https://github.com/redisson/redisson/wiki/8.-%E5%88%86%E5%B8%83%E5%BC%8F%E9%94%81%E5%92%8C%E5%90%8C%E6%AD%A5%E5%99%A8#82-%E5%85%AC%E5%B9%B3%E9%94%81fair-lock)
- [8.3. 联锁（MultiLock）](https://github.com/redisson/redisson/wiki/8.-%E5%88%86%E5%B8%83%E5%BC%8F%E9%94%81%E5%92%8C%E5%90%8C%E6%AD%A5%E5%99%A8#83-%E8%81%94%E9%94%81multilock)
- [8.4. 红锁（RedLock）](https://github.com/redisson/redisson/wiki/8.-%E5%88%86%E5%B8%83%E5%BC%8F%E9%94%81%E5%92%8C%E5%90%8C%E6%AD%A5%E5%99%A8#84-%E7%BA%A2%E9%94%81redlock)
- [8.5. 读写锁（ReadWriteLock）](https://github.com/redisson/redisson/wiki/8.-%E5%88%86%E5%B8%83%E5%BC%8F%E9%94%81%E5%92%8C%E5%90%8C%E6%AD%A5%E5%99%A8#85-%E8%AF%BB%E5%86%99%E9%94%81readwritelock)
- [8.6. 信号量（Semaphore）](https://github.com/redisson/redisson/wiki/8.-%E5%88%86%E5%B8%83%E5%BC%8F%E9%94%81%E5%92%8C%E5%90%8C%E6%AD%A5%E5%99%A8#86-%E4%BF%A1%E5%8F%B7%E9%87%8Fsemaphore)
- [8.7. 可过期性信号量（PermitExpirableSemaphore）](https://github.com/redisson/redisson/wiki/8.-%E5%88%86%E5%B8%83%E5%BC%8F%E9%94%81%E5%92%8C%E5%90%8C%E6%AD%A5%E5%99%A8#87-%E5%8F%AF%E8%BF%87%E6%9C%9F%E6%80%A7%E4%BF%A1%E5%8F%B7%E9%87%8Fpermitexpirablesemaphore)
- [8.8. 闭锁（CountDownLatch）](https://github.com/redisson/redisson/wiki/8.-%E5%88%86%E5%B8%83%E5%BC%8F%E9%94%81%E5%92%8C%E5%90%8C%E6%AD%A5%E5%99%A8#88-%E9%97%AD%E9%94%81countdownlatch)

- [9. 分布式服务](https://github.com/redisson/redisson/wiki/9.-%E5%88%86%E5%B8%83%E5%BC%8F%E6%9C%8D%E5%8A%A1)- [9.1. 分布式远程服务（Remote Service）](https://github.com/redisson/redisson/wiki/9.-%E5%88%86%E5%B8%83%E5%BC%8F%E6%9C%8D%E5%8A%A1#91-%E5%88%86%E5%B8%83%E5%BC%8F%E8%BF%9C%E7%A8%8B%E6%9C%8D%E5%8A%A1remote-service)- [9.1.1. 分布式远程服务工作流程](https://github.com/redisson/redisson/wiki/9.-%E5%88%86%E5%B8%83%E5%BC%8F%E6%9C%8D%E5%8A%A1#911-%E5%88%86%E5%B8%83%E5%BC%8F%E8%BF%9C%E7%A8%8B%E6%9C%8D%E5%8A%A1%E5%B7%A5%E4%BD%9C%E6%B5%81%E7%A8%8B)
- [9.1.2.
 发送即不管（Fire-and-Forget）模式和应答回执（Ack-Response）模式](https://github.com/redisson/redisson/wiki/9.-%E5%88%86%E5%B8%83%E5%BC%8F%E6%9C%8D%E5%8A%A1#912-%E5%8F%91%E9%80%81%E5%8D%B3%E4%B8%8D%E7%AE%A1fire-and-forget%E6%A8%A1%E5%BC%8F%E5%92%8C%E5%BA%94%E7%AD%94%E5%9B%9E%E6%89%A7ack-response%E6%A8%A1%E5%BC%8F)
- [9.1.3. 异步调用](https://github.com/redisson/redisson/wiki/9.-%E5%88%86%E5%B8%83%E5%BC%8F%E6%9C%8D%E5%8A%A1#913-%E5%BC%82%E6%AD%A5%E8%B0%83%E7%94%A8)
- [9.1.4. 取消异步调用](https://github.com/redisson/redisson/wiki/9.-%E5%88%86%E5%B8%83%E5%BC%8F%E6%9C%8D%E5%8A%A1#914-%E5%8F%96%E6%B6%88%E5%BC%82%E6%AD%A5%E8%B0%83%E7%94%A8)

- [9.2. 分布式实时对象（Live Object）服务](https://github.com/redisson/redisson/wiki/9.-%E5%88%86%E5%B8%83%E5%BC%8F%E6%9C%8D%E5%8A%A1#92-%E5%88%86%E5%B8%83%E5%BC%8F%E5%AE%9E%E6%97%B6%E5%AF%B9%E8%B1%A1live-object%E6%9C%8D%E5%8A%A1)- [9.2.1. 介绍](https://github.com/redisson/redisson/wiki/9.-%E5%88%86%E5%B8%83%E5%BC%8F%E6%9C%8D%E5%8A%A1#921-%E4%BB%8B%E7%BB%8D)
- [9.2.2. 使用方法](https://github.com/redisson/redisson/wiki/9.-%E5%88%86%E5%B8%83%E5%BC%8F%E6%9C%8D%E5%8A%A1#922-%E4%BD%BF%E7%94%A8%E6%96%B9%E6%B3%95)
- [9.2.3. 高级使用方法](https://github.com/redisson/redisson/wiki/9.-%E5%88%86%E5%B8%83%E5%BC%8F%E6%9C%8D%E5%8A%A1#923-%E9%AB%98%E7%BA%A7%E4%BD%BF%E7%94%A8%E6%96%B9%E6%B3%95)
- [9.2.4. 使用限制](https://github.com/redisson/redisson/wiki/9.-%E5%88%86%E5%B8%83%E5%BC%8F%E6%9C%8D%E5%8A%A1#924-%E4%BD%BF%E7%94%A8%E9%99%90%E5%88%B6)

- [9.3. 分布式执行服务（Executor Service）](https://github.com/redisson/redisson/wiki/9.-%E5%88%86%E5%B8%83%E5%BC%8F%E6%9C%8D%E5%8A%A1#93-%E5%88%86%E5%B8%83%E5%BC%8F%E6%89%A7%E8%A1%8C%E6%9C%8D%E5%8A%A1executor-service)- [9.3.1. 分布式执行服务概述](https://github.com/redisson/redisson/wiki/9.-%E5%88%86%E5%B8%83%E5%BC%8F%E6%9C%8D%E5%8A%A1#931-%E5%88%86%E5%B8%83%E5%BC%8F%E6%89%A7%E8%A1%8C%E6%9C%8D%E5%8A%A1%E6%A6%82%E8%BF%B0)
- [9.3.2. 任务](https://github.com/redisson/redisson/wiki/9.-%E5%88%86%E5%B8%83%E5%BC%8F%E6%9C%8D%E5%8A%A1#932-%E4%BB%BB%E5%8A%A1)
- [9.3.3. 任务异步执行模式](https://github.com/redisson/redisson/wiki/9.-%E5%88%86%E5%B8%83%E5%BC%8F%E6%9C%8D%E5%8A%A1#933-%E4%BB%BB%E5%8A%A1%E5%BC%82%E6%AD%A5%E6%89%A7%E8%A1%8C%E6%A8%A1%E5%BC%8F)
- [9.3.4. 取消任务](https://github.com/redisson/redisson/wiki/9.-%E5%88%86%E5%B8%83%E5%BC%8F%E6%9C%8D%E5%8A%A1#934-%E5%8F%96%E6%B6%88%E4%BB%BB%E5%8A%A1)

- [9.4. 分布式调度任务服务（Scheduler Service）](https://github.com/redisson/redisson/wiki/9.-%E5%88%86%E5%B8%83%E5%BC%8F%E6%9C%8D%E5%8A%A1#94-%E5%88%86%E5%B8%83%E5%BC%8F%E8%B0%83%E5%BA%A6%E4%BB%BB%E5%8A%A1%E6%9C%8D%E5%8A%A1scheduler-service)- [9.4.1. 分布式调度任务服务概述](https://github.com/redisson/redisson/wiki/9.-%E5%88%86%E5%B8%83%E5%BC%8F%E6%9C%8D%E5%8A%A1#941-%E5%88%86%E5%B8%83%E5%BC%8F%E8%B0%83%E5%BA%A6%E4%BB%BB%E5%8A%A1%E6%9C%8D%E5%8A%A1%E6%A6%82%E8%BF%B0)
- [9.4.2. 设定任务计划](https://github.com/redisson/redisson/wiki/9.-%E5%88%86%E5%B8%83%E5%BC%8F%E6%9C%8D%E5%8A%A1#942-%E8%AE%BE%E5%AE%9A%E4%BB%BB%E5%8A%A1%E8%AE%A1%E5%88%92)
- [9.4.3. 通过CRON表达式设定任务计划](https://github.com/redisson/redisson/wiki/9.-%E5%88%86%E5%B8%83%E5%BC%8F%E6%9C%8D%E5%8A%A1#943-%E9%80%9A%E8%BF%87cron%E8%A1%A8%E8%BE%BE%E5%BC%8F%E8%AE%BE%E5%AE%9A%E4%BB%BB%E5%8A%A1%E8%AE%A1%E5%88%92)
- [9.4.4. 取消计划任务](https://github.com/redisson/redisson/wiki/9.-%E5%88%86%E5%B8%83%E5%BC%8F%E6%9C%8D%E5%8A%A1#944-%E5%8F%96%E6%B6%88%E8%AE%A1%E5%88%92%E4%BB%BB%E5%8A%A1)


- [10. 额外功能](https://github.com/redisson/redisson/wiki/10.-%E9%A2%9D%E5%A4%96%E5%8A%9F%E8%83%BD)- [10.1. 对Redis节点的操作](https://github.com/redisson/redisson/wiki/10.-%E9%A2%9D%E5%A4%96%E5%8A%9F%E8%83%BD#101-%E5%AF%B9redis%E8%8A%82%E7%82%B9%E7%9A%84%E6%93%8D%E4%BD%9C)
- [10.2.
 复杂多维对象结构和对象引用的支持](https://github.com/redisson/redisson/wiki/10.-%E9%A2%9D%E5%A4%96%E5%8A%9F%E8%83%BD#102-%E5%A4%8D%E6%9D%82%E5%A4%9A%E7%BB%B4%E5%AF%B9%E8%B1%A1%E7%BB%93%E6%9E%84%E5%92%8C%E5%AF%B9%E8%B1%A1%E5%BC%95%E7%94%A8%E7%9A%84%E6%94%AF%E6%8C%81)
- [10.3. 命令的批量执行](https://github.com/redisson/redisson/wiki/10.-%E9%A2%9D%E5%A4%96%E5%8A%9F%E8%83%BD#103-%E5%91%BD%E4%BB%A4%E7%9A%84%E6%89%B9%E9%87%8F%E6%89%A7%E8%A1%8C)
- [10.4. 脚本执行](https://github.com/redisson/redisson/wiki/10.-%E9%A2%9D%E5%A4%96%E5%8A%9F%E8%83%BD#104-%E8%84%9A%E6%9C%AC%E6%89%A7%E8%A1%8C)
- [10.5. 底层Redis客户端](https://github.com/redisson/redisson/wiki/10.-%E9%A2%9D%E5%A4%96%E5%8A%9F%E8%83%BD#105-%E5%BA%95%E5%B1%82redis%E5%AE%A2%E6%88%B7%E7%AB%AF)

- [11. Redis命令和Redisson对象匹配列表](https://github.com/redisson/redisson/wiki/11.-Redis%E5%91%BD%E4%BB%A4%E5%92%8CRedisson%E5%AF%B9%E8%B1%A1%E5%8C%B9%E9%85%8D%E5%88%97%E8%A1%A8)
- [12. 独立节点模式](https://github.com/redisson/redisson/wiki/12.-%E7%8B%AC%E7%AB%8B%E8%8A%82%E7%82%B9%E6%A8%A1%E5%BC%8F)- [12.1. 概述](https://github.com/redisson/redisson/wiki/12.-%E7%8B%AC%E7%AB%8B%E8%8A%82%E7%82%B9%E6%A8%A1%E5%BC%8F#121-%E6%A6%82%E8%BF%B0)
- [12.2. 配置方法](https://github.com/redisson/redisson/wiki/12.-%E7%8B%AC%E7%AB%8B%E8%8A%82%E7%82%B9%E6%A8%A1%E5%BC%8F#122-%E9%85%8D%E7%BD%AE%E6%96%B9%E6%B3%95)- [12.2.1. 配置参数](https://github.com/redisson/redisson/wiki/12.-%E7%8B%AC%E7%AB%8B%E8%8A%82%E7%82%B9%E6%A8%A1%E5%BC%8F#1221-%E9%85%8D%E7%BD%AE%E5%8F%82%E6%95%B0)
- [12.2.2.
 通过JSON和YAML配置文件配置独立节点](https://github.com/redisson/redisson/wiki/12.-%E7%8B%AC%E7%AB%8B%E8%8A%82%E7%82%B9%E6%A8%A1%E5%BC%8F#1222-%E9%80%9A%E8%BF%87json%E5%92%8Cyaml%E9%85%8D%E7%BD%AE%E6%96%87%E4%BB%B6%E9%85%8D%E7%BD%AE%E7%8B%AC%E7%AB%8B%E8%8A%82%E7%82%B9)

- [12.3. 初始化监听器](https://github.com/redisson/redisson/wiki/12.-%E7%8B%AC%E7%AB%8B%E8%8A%82%E7%82%B9%E6%A8%A1%E5%BC%8F#123-%E5%88%9D%E5%A7%8B%E5%8C%96%E7%9B%91%E5%90%AC%E5%99%A8)
- [12.4. 嵌入式运行方法](https://github.com/redisson/redisson/wiki/12.-%E7%8B%AC%E7%AB%8B%E8%8A%82%E7%82%B9%E6%A8%A1%E5%BC%8F#124-%E5%B5%8C%E5%85%A5%E5%BC%8F%E8%BF%90%E8%A1%8C%E6%96%B9%E6%B3%95)
- [12.5. 命令行运行方法](https://github.com/redisson/redisson/wiki/12.-%E7%8B%AC%E7%AB%8B%E8%8A%82%E7%82%B9%E6%A8%A1%E5%BC%8F#125-%E5%91%BD%E4%BB%A4%E8%A1%8C%E8%BF%90%E8%A1%8C%E6%96%B9%E6%B3%95)
- [12.6. Docker方式运行方法](https://github.com/redisson/redisson/wiki/12.-%E7%8B%AC%E7%AB%8B%E8%8A%82%E7%82%B9%E6%A8%A1%E5%BC%8F#126-docker%E6%96%B9%E5%BC%8F%E8%BF%90%E8%A1%8C%E6%96%B9%E6%B3%95)

- [13. 工具](https://github.com/redisson/redisson/wiki/13.-%E5%B7%A5%E5%85%B7)- [13.1. Redis部署工具](https://github.com/redisson/redisson/wiki/13.-%E5%B7%A5%E5%85%B7#131-redis%E9%83%A8%E7%BD%B2%E5%B7%A5%E5%85%B7)
- [13.2. 集群管理工具](https://github.com/redisson/redisson/wiki/13.-%E5%B7%A5%E5%85%B7#132-%E9%9B%86%E7%BE%A4%E7%AE%A1%E7%90%86%E5%B7%A5%E5%85%B7)- [13.2.1. 创建集群](https://github.com/redisson/redisson/wiki/13.-%E5%B7%A5%E5%85%B7#1321-%E5%88%9B%E5%BB%BA%E9%9B%86%E7%BE%A4)
- [13.2.2. 踢出节点](https://github.com/redisson/redisson/wiki/13.-%E5%B7%A5%E5%85%B7#1322-%E8%B8%A2%E5%87%BA%E8%8A%82%E7%82%B9)
- [13.2.3. 数据槽迁移](https://github.com/redisson/redisson/wiki/13.-%E5%B7%A5%E5%85%B7#1323-%E6%95%B0%E6%8D%AE%E6%A7%BD%E8%BF%81%E7%A7%BB)
- [13.2.4. 添加从节点](https://github.com/redisson/redisson/wiki/13.-%E5%B7%A5%E5%85%B7#1324-%E6%B7%BB%E5%8A%A0%E4%BB%8E%E8%8A%82%E7%82%B9)
- [13.2.5. 添加主节点](https://github.com/redisson/redisson/wiki/13.-%E5%B7%A5%E5%85%B7#1325-%E6%B7%BB%E5%8A%A0%E4%B8%BB%E8%8A%82%E7%82%B9)


- [14. 第三方框架整合](https://github.com/redisson/redisson/wiki/14.-%E7%AC%AC%E4%B8%89%E6%96%B9%E6%A1%86%E6%9E%B6%E6%95%B4%E5%90%88)- [14.1. Spring框架整合](https://github.com/redisson/redisson/wiki/14.-%E7%AC%AC%E4%B8%89%E6%96%B9%E6%A1%86%E6%9E%B6%E6%95%B4%E5%90%88#141-spring%E6%A1%86%E6%9E%B6%E6%95%B4%E5%90%88)
- [14.2. Spring Cache整合](https://github.com/redisson/redisson/wiki/14.-%E7%AC%AC%E4%B8%89%E6%96%B9%E6%A1%86%E6%9E%B6%E6%95%B4%E5%90%88#142-spring-cache%E6%95%B4%E5%90%88)
- [14.3. Hibernate整合](https://github.com/redisson/redisson/wiki/14.-%E7%AC%AC%E4%B8%89%E6%96%B9%E6%A1%86%E6%9E%B6%E6%95%B4%E5%90%88#143-hibernate%E6%95%B4%E5%90%88)
- [14.4. Java缓存标准规范JCache API (JSR-107）](https://github.com/redisson/redisson/wiki/14.-%E7%AC%AC%E4%B8%89%E6%96%B9%E6%A1%86%E6%9E%B6%E6%95%B4%E5%90%88#144-java%E7%BC%93%E5%AD%98%E6%A0%87%E5%87%86%E8%A7%84%E8%8C%83jcache-api-jsr-107)
- [14.5. Tomcat会话管理器（Tomcat Session Manager）](https://github.com/redisson/redisson/wiki/14.-%E7%AC%AC%E4%B8%89%E6%96%B9%E6%A1%86%E6%9E%B6%E6%95%B4%E5%90%88#145-tomcat%E4%BC%9A%E8%AF%9D%E7%AE%A1%E7%90%86%E5%99%A8tomcat-session-manager)
- [14.6. Spring Session会话管理器](https://github.com/redisson/redisson/wiki/14.-%E7%AC%AC%E4%B8%89%E6%96%B9%E6%A1%86%E6%9E%B6%E6%95%B4%E5%90%88#146-spring-session%E4%BC%9A%E8%AF%9D%E7%AE%A1%E7%90%86%E5%99%A8)







