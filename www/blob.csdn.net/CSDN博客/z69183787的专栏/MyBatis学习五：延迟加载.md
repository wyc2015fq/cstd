# MyBatis学习五：延迟加载 - z69183787的专栏 - CSDN博客
2017年07月13日 18:25:59[OkidoGreen](https://me.csdn.net/z69183787)阅读数：219
个人分类：[对象映射框架-Ibatis/Mybatis](https://blog.csdn.net/z69183787/article/category/2261201)
#### 一、什么是延迟加载
resultMap可实现高级映射（使用association、collection实现一对一及一对多映射），association、collection具备延迟加载功能。
需求：
如果查询订单并且关联查询用户信息。如果先查询订单信息即可满足要求，当我们需要查询用户信息时再查询用户信息。把对用户信息的按需去查询就是延迟加载。
延迟加载：先从单表查询，需要时再从关联表去关联查询，大大提高[数据库](http://lib.csdn.net/base/mysql)性能，因为查询单表要比关联查询多张表速度要快。
#### 二、使用association实现延迟加载
##### 1、需求：查询订单并且关联查询用户信息
##### 2、mapper.xml
需要定义两个mapper的方法对应的statement。
###### 1）  只查询订单信息
SELECT * FROM orders
在查询订单的statement中使用association去延迟加载（执行）下边的statement。
**[html]**[view
 plain](http://blog.csdn.net/mwj_88/article/details/50295131#)[copy](http://blog.csdn.net/mwj_88/article/details/50295131#)
- <!-- 查询订单关联查询用户，用户信息需要延迟加载 -->
- <selectid="findOrdersUserLazyLoading"resultMap="OrderUserLazyLoadingResultMap">
-         SELECT * FROM orders  
- </select>
resultMap怎么写，看下面的3。
###### 2）  关联查询用户信息
通过上边查询到的订单信息中user_id去关联查询用户信息
使用UserMapper.xml中的findUserById
**[html]**[view
 plain](http://blog.csdn.net/mwj_88/article/details/50295131#)[copy](http://blog.csdn.net/mwj_88/article/details/50295131#)
- <selectid="findUserById"resultType="user"parameterType="int">
-         SELECT * FROM USER WHERE id=#{value}  
- </select>
###### 3）  执行思路：
上边先去执行findOrdersuserLazyLoading，当需要的时候再去执行findUserById，通过resultMap的定义将延迟加载执行配置起来。
##### 3、延迟加载的resultMap
**[html]**[view
 plain](http://blog.csdn.net/mwj_88/article/details/50295131#)[copy](http://blog.csdn.net/mwj_88/article/details/50295131#)
- <resultMaptype="cn.itcast.mybatis.po.Orders"id="OrderUserLazyLoadingResultMap">
- <!-- 对订单信息进行映射配置 -->
- <idcolumn="id"property="id"/>
- <resultcolumn="user_id"property="userId"/>
- <resultcolumn="number"property="number"/>
- <resultcolumn="createtime"property="createtime"/>
- <resultcolumn="note"property="note"/>
- <!-- 对用户信息进行延迟加载 -->
-     <!--   
-     select：指定延迟加载要执行的statement的id（是根据user_id查询用户信息是statement）  
-     要使用UserMapper.xml中findUserById完成根据用户id（user_id）对用户信息的查询，如果findUserById不在本mapper中，  
-     需要前边加namespace  
-     column：订单信息中关联用户信息的列，是user_id  
-      -->
- <associationproperty="user"javaType="cn.itcast.mybatis.po.User"
- select="cn.itcast.mybatis.mapper.UserMapper.findUserById"column="user_id">
- 
- </association>
- </resultMap>
使用association中是select指定延迟加载去执行的statement的id。
#####  4、打开延迟加载开关
**[html]**[view
 plain](http://blog.csdn.net/mwj_88/article/details/50295131#)[copy](http://blog.csdn.net/mwj_88/article/details/50295131#)
- <settings>
- <!-- 打开延迟加载的开关 -->
- <settingname="lazyLoadingEnabled"value="true"/>
- <!-- 将积极加载改为消息加载即按需加载 -->
- <settingname="aggressiveLazyLoading"value="false"/>
- </settings>
lazyLoadingEnabled：设置懒加载，默认为false。如果为false：则所有相关联的都会被初始化加载。
aggressiveLazyLoading：默认为true。当设置为true时，懒加载的对象可能被任何懒属性全部加载；否则，每个属性按需加载。
#### 三、使用collection实现延迟加载
同理。
#### 四、思考总结
##### 1．不使用mybatis提供的association及collection中的延迟加载功能，如何实现延迟加载？
实现思路：
定义两个mapper方法：
1）  查询订单列表  2）根据用户id查询用户信息
先去查询第一个mapper方法，获取订单信息列表；在程序中（service），按需去调用第二个mapper方法去查询用户信息。
##### 2.总结
使用延迟加载方法，先去查询简单的sql（最好单表，也可关联查询），再去按需加载关联查询的其他信息。
