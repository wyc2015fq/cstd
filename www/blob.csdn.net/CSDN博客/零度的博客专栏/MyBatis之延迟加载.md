# MyBatis之延迟加载 - 零度的博客专栏 - CSDN博客
2017年09月15日 11:19:47[零度anngle](https://me.csdn.net/zmx729618)阅读数：215标签：[mybatis](https://so.csdn.net/so/search/s.do?q=mybatis&t=blog)
个人分类：[Mybatis](https://blog.csdn.net/zmx729618/article/category/6309398)
### 1. 什么是延迟加载
##         举个例子：如果查询订单并且关联查询用户信息。如果先查询订单信息即可满足要求，当我们需要查询用户信息时再查询用户信息。把对用户信息的按需去查询就是延迟加载。 所以延迟加载即先从单表查询、需要时再从关联表去关联查询，大大提高[数据库](http://lib.csdn.net/base/mysql)性能，因为查询单表要比关联查询多张表速度要快。 
        我们来对比一下：
##         关联查询：SELECT orders.*, user.username FROM orders, USER WHERE orders.user_id = user.`id`
##         延迟加载相当于： SELECT orders.*, (SELECT username FROM USER WHERE orders.user_id = user.id) as username FROM orders
　　所以这就比较直观了，也就是说，我把关联查询分两次来做，而不是一次性查出所有的。第一步只查询单表orders，必然会查出orders中的一个user_id字段，然后我再根据这个user_id查user表，也是单表查询。下面来总结一下如何使用这个延迟加载。
### 2. 延迟加载如何实现
　　前面博文中总结了resultMap可以实现高级映射（使用association、collection实现一对一及一对多映射），其实association和collection还具备延迟加载的功能，这里我就拿association来说明，collection和association使用的方法都是一样的。需求就是上面提到的，查询订单并且关联查询用户，查询用户使用延迟加载。 
　　由上面的分析可知，延迟加载要查询两次，第二次是按需查询，之前一对一关联查询的时候只需要查一次，把订单和用户信息都查出来了，所以只要写一个mapper即可，但是延迟加载查两次，所以理所当然要有两个mapper。
#### 2.1 两个mapper.xml
　　需要定义两个mapper的方法对应的statement。先来分析一下思路：
> - 只查询订单信息的statement，使用resultMap
- 通过查询到的订单信息中的user_id去查询用户信息的statement，得到用户
- 定义的resultMap将两者关联起来，即用订单信息user_id去查用户
　　下面来实现这个思路： 
1. 只查询订单信息的statement：
```xml
<select id="findOrdersUserLazyLoading" resultMap="OrdersUserLazyLoadingResultMap">
    SELECT * FROM orders
</select>
```
```xml
```
```xml
```
- 1
- 2
- 3
- 1
- 2
- 3
![](http://static.blog.csdn.net/images/save_snippets_01.png)
2. 只查询用户信息的statement：
```xml
<select id="findUserById" parameterType="int" resultType="user">
    select * from user where id = #{id}
</select>
```
```xml
```
```xml
```
- 1
- 2
- 3
- 1
- 2
- 3
![](http://static.blog.csdn.net/images/save_snippets_01.png)
3. 定义上面那个resultMap： 
![resultMap](https://img-blog.csdn.net/20160614090739882)
#### 2.2 延迟加载的配置
　　mybatis默认没有开启延迟加载，需要在SqlMapConfig.xml中setting配置。前面一篇博文中提到SqlMapConfig.xml中的一些配置，有一个`<settings>`，当时没说，这里就派上用场了，可以通过这个标签来配置一下延迟加载。
```xml
<settings>
    <!-- 打开延迟加载的开关 -->
    <setting name="lazyLoadingEnabled" value="true"/>
    <!-- 将积极加载改为消极加载，即延迟加载 -->
    <setting name="aggressiveLazyLoading" value="false"/>
</settings>
```
```xml
```
```xml
```
- 1
- 2
- 3
- 4
- 5
- 6
- 1
- 2
- 3
- 4
- 5
- 6
#### 2.3 mapper.java
　　别忘了写mapper接口：
```java
public interface UserMapperOrders {
```
```java
//省去不相关代码
    //查询订单，关联用户查询，用户查询用的是延迟加载
    public List<Orders> findOrdersUserLazyLoading() throws Exception;
```
```java
```
```java
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 1
- 2
- 3
- 4
- 5
- 6
- 7
![](http://static.blog.csdn.net/images/save_snippets.png)
　　到此为止，延迟加载就做完了，下面来[测试](http://lib.csdn.net/base/softwaretest)一下：
```java
@Test
public void testFindOrdersUserLazyLoading() throws Exception {
```
```java
SqlSession sqlSession = sqlSessionFactory.openSession();
```
```java
UserMapperOrders userMapperOrders = sqlSession.getMapper(UserMapperOrders.class);
```
```java
//查询订单表（单表）
    List<Orders> list = userMapperOrders.findOrdersUserLazyLoading();
    //遍历上边的订单列表
    for(Orders orders : list) {
        //执行getUser()去查询用户信息，这里实现按需加载
        User user = orders.getUser();
        System.out.println(user);
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
![](http://static.blog.csdn.net/images/save_snippets.png)
　　看一下执行结果： 
![执行结果](https://img-blog.csdn.net/20160614092059354)
　　执行结果很明显，使用了延迟加载，将关联查询分成了两次单表查询，但是有个奇怪的地方，就是第二次查用户的时候，并没有发sql，而是直接就拿到了，其实这就是mybatis中的一级缓存，也是下一篇博文要总结的。 
