# 关于hibernate的hql语句中的 fetch join - 苝花向暖丨楠枝向寒 - CSDN博客

2018年05月17日 22:09:10[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：461标签：[hibernate																[hql																[fetch](https://so.csdn.net/so/search/s.do?q=fetch&t=blog)](https://so.csdn.net/so/search/s.do?q=hql&t=blog)](https://so.csdn.net/so/search/s.do?q=hibernate&t=blog)
个人分类：[框架](https://blog.csdn.net/weixin_40247263/article/category/7511186)


  fetch参数指定了关联对象抓取的方式是select查询还是join查询，join方式主体对象和关联对象用一句外键关联的sql同时查询出来，不会形成多次查询。如果你的关联对象是延迟加载的，它不会去查询关联对象。
  在hql查询中配置文件中设置的join方式是不起作用的（而在所有其他查询方式如get、criteria或再关联获取等等都是有效的），会使用select方式，除非你在hql中指定join fetch某个关联对象。
  fetch策略用于定义 get/load一个对象时，如何获取非lazy的对象/集合。 这些参数在Query中无效。
  查询抓取（默认的）在N+1查询的情况下是不好的，因此我们可能会要求在映射文档中定义使用连接抓取。
  在映射文档中定义的抓取策略将会有产生以下影响：
  通过get()或load()方法取得数据。只有在关联之间进行导航时，才会隐式的取得数据(延迟抓取)。在映射文档中显式的声明 连接抓取做为抓取策略并不会影响到随后的HQL查询。
  通常情况下，我们并不使用映射文档进行抓取策略的定制。更多的是，保持其默认值，然后在特定的事务中， 使用HQL的左连接抓取（left joinfetch） 对其进行重载。这将通知 Hibernate在第一次查询中使用外部关联（outer join），直接得到其关联数据。 在条件查询API中，应该调用setFetchMode(FetchMode.JOIN)语句。

