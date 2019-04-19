# MyBatis缓存策略之二级缓存 - 零度的博客专栏 - CSDN博客
2017年09月15日 15:35:15[零度anngle](https://me.csdn.net/zmx729618)阅读数：11688
### 1. 二级缓存的原理
　　前面介绍了，mybatis中的二级缓存是mapper级别的缓存，值得注意的是，不同的mapper通常情况下有不同的namespace，就都有一个二级缓存，也就是说，不同的mapper之间的二级缓存是互不影响的。为了更加清楚的描述二级缓存，先来看一个示意图： 
![二级缓存](https://img-blog.csdn.net/20160614105455733)
　　从图中可以看出：
> - sqlSession1去查询用户id为1的用户信息，查询到用户信息会将查询数据存储到该UserMapper的二级缓存中。
- 如果SqlSession3去执行相同 mapper下sql，执行commit提交，则会清空该UserMapper下二级缓存区域的数据。
- sqlSession2去查询用户id为1的用户信息，去缓存中找是否存在数据，如果存在直接从缓存中取出数据。
　　 缓存的执行原理和前面提到的一级缓存是差不多的，二级缓存与一级缓存区别在于二级缓存的范围更大，多个sqlSession可以共享一个mapper中的二级缓存区域。mybatis是如何区分不同mapper的二级缓存区域呢？它是按照不同mapper有不同的namespace来区分的，也就是说，如果两个mapper的namespace相同，即使是两个mapper，那么这两个mapper中执行sql查询到的数据也将存在相同的二级缓存区域中。
### 2. 二级缓存的使用
　　明白了mybatis中二级缓存的原理后，接下来就是如何使用二级缓存了。在使用之前，首先得开启二级缓存的开关。
#### 2.1 开启二级缓存
　　由于mybaits的二级缓存是mapper范围级别，所以除了在SqlMapConfig.xml设置二级缓存的总开关外，还要在具体的mapper.xml中开启二级缓存。设置如下： 
![开启二级缓存](https://img-blog.csdn.net/20160614110339815)
　　这是在SqlMapConfig.xml中设置的，还得在具体的mapper.xml中设置，如下： 
![mapper中的二级缓存](https://img-blog.csdn.net/20160614110618819)
　　可以看到，具体的mapper中仅仅就一个`<cache>`标签，并没有配置啥东西，这是因为mybatis中有默认的实现，我们如果不配置，那么就默认使用那个默认的实现。在mybatis的核心包里有cache的接口和这个默认的实现，我截个图： 
![二级缓存默认实现类](https://img-blog.csdn.net/20160614111358194)
　　因此就明白了，不用配置也同样可以使用，mybatis中也就只实现了这一个默认实现类，如果不使用mybatis的默认二级缓存的话，就需要自己实现cache接口，然后再在mapper.xml中配置一下了，关于这个我在下面再谈，现在先把二级缓存用起来！
#### 2.2 将po类实现Serializable接口
开启了二级缓存后，还需要将要缓存的pojo实现Serializable接口，为了将缓存数据取出执行反序列化操作，因为二级缓存数据存储介质多种多样，不一定只存在内存中，有可能存在硬盘中，如果我们要再取这个缓存的话，就需要反序列化了。所以建议mybatis中的pojo都去实现Serializable接口。下面以User为例截个图： 
![序列化](https://img-blog.csdn.net/20160614112135026)
#### 2.3 测试mybatis的二级缓存
```java
@Test
public void testCache2() throws Exception {
```
```java
SqlSession sqlSession1 = sqlSessionFactory.openSession();
    SqlSession sqlSession2 = sqlSessionFactory.openSession();
    SqlSession sqlSession3 = sqlSessionFactory.openSession();
```
```java
// 创建代理对象
    UserMapper userMapper1 = sqlSession1.getMapper(UserMapper.class);
    // 第一次发起请求，查询id为1的用户
    User user1 = userMapper1.findUserById(1);
    System.out.println(user1);  
    //这里执行关闭操作，将sqlsession中的数据写到二级缓存区域
    sqlSession1.close();
    //sqlSession3用来清空缓存的，如果要测试二级缓存，需要把该部分注释掉
    //使用sqlSession3执行commit()操作
    UserMapper userMapper3 = sqlSession3.getMapper(UserMapper.class);
    User user  = userMapper3.findUserById(1);
    user.setUsername("张三");
    userMapper3.updateUser(user);
    //执行提交，清空UserMapper下边的二级缓存
    sqlSession3.commit();
    sqlSession3.close();
    UserMapper userMapper2 = sqlSession2.getMapper(UserMapper.class);
    // 第二次发起请求，查询id为1的用户
    User user2 = userMapper2.findUserById(1);
    System.out.println(user2);
    sqlSession2.close();
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
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
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
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
![](http://static.blog.csdn.net/images/save_snippets.png)
　　我们先把sqlSession3部分注释掉来[测试](http://lib.csdn.net/base/softwaretest)一下二级缓存的结果： 
![二级缓存](https://img-blog.csdn.net/20160614135639431)
　　当我们把sqlSession3部分加上后，再测试一下二级缓存结果： 
![二级缓存2](https://img-blog.csdn.net/20160614140559779)
　　到这里，就明白了mybatis中二级缓存的执行原理了，这个跟[hibernate](http://lib.csdn.net/base/javaee)还是有点像的。
#### 2.4 其他配置（useCache和flushCache）
　　mybatis中还可以配置useCache和flushCache等配置项，useCache是用来设置是否禁用二级缓存的，在statement中设置useCache=false可以禁用当前select语句的二级缓存，即每次查询都会发出sql去查询，默认情况是true，即该sql使用二级缓存。
```xml
<select id="findOrderListResultMap" resultMap="ordersUserMap" useCache="false">
```
```xml
```
```xml
```
- 1
- 1
![](http://static.blog.csdn.net/images/save_snippets_01.png)
　　这种情况是针对每次查询都需要最新的数据sql，要设置成useCache=false，禁用二级缓存，直接从[数据库](http://lib.csdn.net/base/mysql)中获取。 在mapper的同一个namespace中，如果有其它insert、update、delete操作数据后需要刷新缓存，如果不执行刷新缓存会出现脏读。
 设置statement配置中的flushCache=”true” 属性，默认情况下为true，即刷新缓存，如果改成false则不会刷新。使用缓存时如果手动修改数据库表中的查询数据会出现脏读。
```xml
<insert id="insertUser" parameterType="cn.itcast.mybatis.po.User" flushCache="true">
```
- 1
- 1
　　一般下执行完commit操作都需要刷新缓存，flushCache=true表示刷新缓存，这样可以避免数据库脏读。所以我们不用设置，默认即可，这里只是提一下。
### 3. MyBatis整合ehcache分布式缓存框架
#### 3.1 问题的由来
　　上面的部分主要总结了一下mybatis中二级缓存的使用，但是mybatis中默认自带的二级缓存有个弊端，即无法实现分布式缓存，什么意思呢？就是说缓存的数据在本地的服务器上，假设现在有两个服务器A和B，用户访问的时候访问了A服务器，查询后的缓存就会放在A服务器上，假设现在有个用户访问的是B服务器，那么，他在B服务器上就无法获取刚刚那个缓存，如下图所示：
![缓存弊端](https://img-blog.csdn.net/20160614142727547)
　　所以我们为了解决这个问题，就得找一个分布式的缓存，专门用来存储缓存数据的，这样不同的服务器要缓存数据都往它那里存，取缓存数据也从它那里取，如下图所示： 
![分布式缓存](https://img-blog.csdn.net/20160614142915959)
　　这样就能解决上面所说的问题，为了提高系统并发性能、我们一般对系统进行上面这种分布式部署（集群部署方式），因此要使用分布式缓存对缓存数据进行集中管理。但是mybatis无法实现分布式缓存，需要和其它分布式缓存框架进行整合，这里主要介绍ehcache。
#### 3.2 整合方法
　　上文一开始提到过，mybatis提供了一个cache接口，如果要实现自己的缓存逻辑，实现cache接口开发即可。mybatis也默认实现了一个，但是这个缓存的实现无法实现分布式缓存，所以我们要自己来实现。ehcache分布式缓存就可以，mybatis提供了一个针对cache接口的ehcache实现类，这个类在mybatis和ehcache的整合包中。所以首先我们需要导入整合包（[点我下载](http://download.csdn.net/detail/eson_15/9549252)）。 
![jar包](https://img-blog.csdn.net/20160614144248661)
　　导入了jar包后，配置mapper中cache中的type为ehcache对cache接口的实现类型。ehcache对cache接口有一个实现类为： 
![实现类](https://img-blog.csdn.net/20160614145219392)
　　我们将该类的完全限定名写到type属性中即可，如下： 
![配置](https://img-blog.csdn.net/20160614145333597)
　　OK，配置完成，现在mybatis就会自动去执行这个ehcache实现类了，就不会使用自己默认的二级缓存了，但是使用ehcache还有一个缓存配置别忘了，在classpath下新建一个ehcache.xml文件：
```xml
<ehcache xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
    xsi:noNamespaceSchemaLocation="../config/ehcache.xsd">
    <diskStore path="F:\develop\ehcache"/>
    <defaultCache
            maxElementsInMemory="10000"
            eternal="false"
            timeToIdleSeconds="120"
            timeToLiveSeconds="120"
            maxElementsOnDisk="10000000"
            diskExpiryThreadIntervalSeconds="120"
            memoryStoreEvictionPolicy="LRU">
        <persistence strategy="localTempSwap"/>
    </defaultCache>
```
```xml
</ehcache>
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
- 15
- 16
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
- 15
- 16
　　这里面配置的作用跟hibernate差不多，大家可以去参考我那篇hibernate二级缓存的博文。接下来就是测试了，还是用上面的那个测试程序，因为只改掉了缓存，其他没动。到此为止，mybatis的二级缓存差不多就总结完了。
### 4. 二级缓存的应用场景和局限性
　　对于访问多的查询请求且用户对查询结果实时性要求不高，此时可采用mybatis二级缓存技术降低数据库访问量，提高访问速度。业务场景比如：耗时较高的统计分析sql、电话账单查询sql等。实现方法如下：通过设置刷新间隔时间，由mybatis每隔一段时间自动清空缓存，根据数据变化频率设置缓存刷新间隔flushInterval，比如设置为30分钟、60分钟、24小时等，根据需求而定。 
　　mybatis二级缓存对细粒度的数据级别的缓存实现不好，比如如下需求：对商品信息进行缓存，由于商品信息查询访问量大，但是要求用户每次都能查询最新的商品信息，此时如果使用mybatis的二级缓存就无法实现当一个商品变化时只刷新该商品的缓存信息而不刷新其它商品的信息，因为mybaits的二级缓存区域以mapper为单位划分的，当一个商品信息变化会将所有商品信息的缓存数据全部清空。解决此类问题可能需要在业务层根据需求对数据有针对性缓存。 

