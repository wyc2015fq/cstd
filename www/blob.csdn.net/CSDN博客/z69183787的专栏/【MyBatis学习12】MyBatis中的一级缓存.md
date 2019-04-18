# 【MyBatis学习12】MyBatis中的一级缓存 - z69183787的专栏 - CSDN博客
2017年07月28日 11:14:11[OkidoGreen](https://me.csdn.net/z69183787)阅读数：232
个人分类：[Mybatis-学习](https://blog.csdn.net/z69183787/article/category/7026142)
[http://blog.csdn.net/eson_15/article/details/51669021](http://blog.csdn.net/eson_15/article/details/51669021)
缓存的作用是减轻[数据库](http://lib.csdn.net/base/mysql)的压力，提高数据库的性能的。mybatis中提供了一级缓存和二级缓存，先来看一下两个缓存的示意图： 
![缓存示意图](https://img-blog.csdn.net/20160614100036529)
　　从图中可以看出：
> - 一级缓存是SqlSession级别的缓存。在操作数据库时需要构造sqlSession对象，在对象中有一个数据结构（HashMap）用于存储缓存数据。不同的sqlSession之间的缓存数据区域（HashMap）是互相不影响的。
- 二级缓存是mapper级别的缓存，多个SqlSession去操作同一个Mapper的sql语句，多个SqlSession可以共用二级缓存，二级缓存是跨SqlSession的。
　　这篇博文主要总结一下mybatis中的一级缓存，看一下一级缓存的工作原理： 
![一级缓存工作原理](https://img-blog.csdn.net/20160614100249876)
　　从图中可以看出：第一次发起查询用户id为1的用户信息，先去找缓存中是否有id为1的用户信息，如果没有，从数据库查询用户信息。得到用户信息，将用户信息存储到一级缓存中。 
　　如果中间sqlSession去执行commit操作（执行插入、更新、删除），则会清空SqlSession中的一级缓存，这样做的目的为了让缓存中存储的是最新的信息，避免脏读。 
　　第二次发起查询用户id为1的用户信息，先去找缓存中是否有id为1的用户信息，缓存中有，直接从缓存中获取用户信息。 
　　mybatis的一级缓存比较简单，我们不知不觉中就在用了，为了完整性，我写一个[测试](http://lib.csdn.net/base/softwaretest)代码来测一下：
```java
@Test
public void testCache1() throws Exception {
    SqlSession sqlSession = sqlSessionFactory.openSession();//创建代理对象
    UserMapper userMapper = sqlSession.getMapper(UserMapper.class);
    //下边查询使用一个SqlSession
    //第一次发起请求，查询id为1的用户
    User user1 = userMapper.findUserById(1);
    System.out.println(user1);
//      如果sqlSession去执行commit操作（执行插入、更新、删除），清空SqlSession中的一级缓存，这样做的目的为了让缓存中存储的是最新的信息，避免脏读。
    //更新user1的信息
    user1.setUsername("测试用户22");
    userMapper.updateUser(user1);
    //执行commit操作去清空缓存
    sqlSession.commit();
    //第二次发起请求，查询id为1的用户
    User user2 = userMapper.findUserById(1);
    System.out.println(user2);
    sqlSession.close();
}
```
- 
　　一级缓存很简单，就不再赘述了，下一篇博文来总结一下mybatis的二级缓存。 
