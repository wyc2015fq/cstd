# hibernate的lazy中的一个extra属性 - weixin_33985507的博客 - CSDN博客
2008年09月21日 08:11:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
hibernate的lazy中的一个extra属性是不大容易重视的,其实它和true差不多,
但有个小的智能的地方是,即调用集合的size/contains等方法的时候，hibernate并不会去加载整个集合的数据，而是发出一条聪明的SQL语句，以便获得需要的值，只有在真正需要用到这些集合元素对象数据的时候，才去发出查询语句加载所有对象的数据 
