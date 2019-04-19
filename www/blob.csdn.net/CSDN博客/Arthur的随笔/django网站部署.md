# django网站部署 - Arthur的随笔 - CSDN博客
2012年04月13日 23:09:27[largetalk](https://me.csdn.net/largetalk)阅读数：5813
经过几个月得拖拖拉拉，项目终于上线了，项目本身没什么出彩得地方，那就说说整个项目得架构和部署吧
项目是个常见得OLTP（也叫CURD）项目了，使用django + mysql, mysql使用了现在正火得percona，前面用了些memcahced，cache了首页和一些template、model，感觉需要一个透明缓存框架了，不然很多代码要重复了，像很多model得cache代码都是这样的：
```python
key = 'xxxx'
obj = cache.get(key)
if obj is None:
    obj = model.objects.get(...)
    cache.set(key, obj)
return obj
```
被这样得代码恶心死了， 再看看有什么框架可以改进吧。
另外，为了性能，很多地方都raw sql了，这也比较恶心，但django orm自动出来得sql又太没效率了，难道是表设计问题？
log： 本来想用sentry来统一收集log得，但测试得时候发现会导致性能变慢，也有可能是我sentry没配好（和项目在同一台机器上且没有使用MQ）得原因，然后也因为部署环境限制，最后把它去掉了，还是写文件（ps。logging模块确实好用）
程序启动是用gunicorn + gevent， 纯wsgi启动性能确实比较惨， 用了gevent之后好点，至少比fastcgi好多了， gevent还是比较难懂，下面有些东西还是比较晦涩得，比如一开始我把session放在memcache里，但在测试人数不是很高得情况下，也会有从memcache查不到数据得情况，但是，这个数据确实存在于memcache中，碰见这种问题真得让人很崩溃，自觉上是与gevent有关，最后改用cookie存session才把这个问题wrok around过去。
项目发布一开始是用capistrano, 后来换成fabric, 对我来说都一样了，通通都是shell嘛
共享文件：使用很挫的nfs挂载了，毕竟小项目嘛，就不需要整什么mogilefs 等等了
supervisor: 使用还是很简单,和virtualenv也可以结合使用， 就是被他那个gunicorn不用加-D参数给耍了一下
virtualenv：其实服务就跑在虚拟机上，本来就不需要这玩意得，但前期也一直用，也就懒得去改了，至少测试环境还是多项目共用机器，也不错了
监控： 前期研究了很多，啥munin阿、nagios阿、zabbix阿等等等等，不过但了正式环境，呵呵，这个没有了
nginx: 测试环境用得是他，但正式环境有F5，也就不需要了
整个项目用到得大概就这些了，其实本来是想说点技术做点笔记得，但写着写着却变成了一篇吐槽文，没办法
总结下吧： django性能是个渣， gunicorn很不错（连django 1.4都推荐了）， gevent快是快，但用起来心里没底（文档也太tm少了）
打算： 下一步准备上个nosql和MQ，项目小但啥也不能缺阿， 哈哈
最后一句：理想很丰满，现实很骨感啊

