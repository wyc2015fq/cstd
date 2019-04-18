# hibernate学习笔记之getCurrentSession和openSession - 苝花向暖丨楠枝向寒 - CSDN博客

2018年04月16日 00:33:41[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：23标签：[hibernate](https://so.csdn.net/so/search/s.do?q=hibernate&t=blog)
个人分类：[框架](https://blog.csdn.net/weixin_40247263/article/category/7511186)



通过openSession来创建session 每次都是新的session，且提交事务以后需要手动close掉这个session

通过getCurrentSession来创建session，只要事务没提交，那么第二次再通过getCurrentSession来创建session，拿的还是同一个session。

使用 getCurrentSession 需要在配置文件中 配置 current-session_context_class

比较常用的 有 两个 ，一个是thread，一个是jta。

thread 使用场景一般就是 针对于对同一个数据库进行事务操作，thread就可以解决问题。

jta使用场景 一般是 针对于不同的数据库进行事务操作。

注意 tomcat 中并不具备 jta 这个功能。

