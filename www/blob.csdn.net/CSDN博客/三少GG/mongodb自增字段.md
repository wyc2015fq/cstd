# mongodb自增字段 - 三少GG - CSDN博客
2017年05月18日 08:21:09[三少GG](https://me.csdn.net/scut1135)阅读数：2847
# 创建一个自增的序列字段
MongoDB使用所有文档中的顶层文档里的 _id 字段作为主键。 _id 必须是唯一的，并且总是有使用 [unique
 constraint](http://manual.ikeepstudying.com/mongodb/core/index-unique.html#index-type-unique) 的索引。然而，除了唯一约束外，你可以在你的集合中为 _id 字段使用任何值。这个教程介绍了为 _id 字段创建自增序列数字的两种方法，如下：
- [使用计数集合](http://manual.ikeepstudying.com/mongodb/tutorial/create-an-auto-incrementing-field.html#auto-increment-counters-collection)
- [乐观循环](http://manual.ikeepstudying.com/mongodb/tutorial/create-an-auto-incrementing-field.html#auto-increment-optimistic-loop)
##########
[**http://blog.51yip.com/php/1646.html**](http://blog.51yip.com/php/1646.html)
**[https://huoding.com/2011/02/09/47](https://huoding.com/2011/02/09/47)**
对象前生成ID赋值给_id就OK了，因为它的实现满足原子性，所以不存在并发问题。
另外说明一点，findAndModify本身提供了一个upsert参数，为true的话可以自动insert，但那样就不能自定义初始值了，所以文中示例没有使用upsert。
