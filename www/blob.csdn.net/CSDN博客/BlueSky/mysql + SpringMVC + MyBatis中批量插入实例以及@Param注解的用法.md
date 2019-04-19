# mysql + SpringMVC + MyBatis中批量插入实例以及@Param注解的用法 - BlueSky - CSDN博客
2016年01月20日 19:30:35[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：8273
1.使用到的jar包如下：
mybatis-3.2.2.jar、mybatis-spring-1.2.2.jar、mysql-connector-java-5.1.32.jar
2.Spring配置文件
**[html]**[view
 plain](http://blog.csdn.net/yansong_8686/article/details/46975953#)[copy](http://blog.csdn.net/yansong_8686/article/details/46975953#)
- <beanid="sqlSessionFactory"class="org.mybatis.spring.SqlSessionFactoryBean">
- <propertyname="dataSource"ref="myDataSource"/>
- 
- <!-- <property name="configLocation" value=""/> -->
- <!--  
-     该属性用来指定MyBatis的XML配置文件路径,跟Spring整合时，编写MyBatis映射文件的目的无非是配置一下typeAlias、setting之类的  
-     元素。不用在其中指定数据源，或者事务处理方式。就算配置了也会被忽略。因为这些都是使用Spring中的配置  
-     。当然如果你不打算添加 typeAlias 之类的设置的话，你连MyBatis的配置文件都不用写，更不用配置这个属性了  
- -->
- <!--<property name="mapperLocations" value="src/UserMapper.xml"/>-->
- <!-- 该配置文件用来指定Mapper映射文件的位置 ，如果映射文件与相应的接口同名，且在同一路径下，那么可以不配置该选项-->
- </bean>
- 
-  <!--注册Mapper方式一   
- 
- <beanid="userMapper"class="org.mybatis.spring.mapper.MapperFactoryBean">
- <propertynamepropertyname="mapperInterface"value="org.hyn.maper.UserMapper"/>
- <propertynamepropertyname="sqlSessionFactory"ref="sqlSessionFactory"/>
- </bean>-->
- 
- <!-- 注册Mapper方式二：也可不指定特定mapper，而使用自动扫描包的方式来注册各种Mapper ，配置如下：-->
- <beanclass="org.mybatis.spring.mapper.MapperScannerConfigurer">
- <propertyname="basePackage"value="com.xxxx.xxx.mapper"/>
- </bean>
**[html]**[view
 plain](http://blog.csdn.net/yansong_8686/article/details/46975953#)[copy](http://blog.csdn.net/yansong_8686/article/details/46975953#)
- </pre><pre>
3.mapper.xml文件
**[html]**[view
 plain](http://blog.csdn.net/yansong_8686/article/details/46975953#)[copy](http://blog.csdn.net/yansong_8686/article/details/46975953#)
- <?xmlversion="1.0"encoding="UTF-8"?>
-     <!DOCTYPE mapper   
-         PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"   
-         "http://mybatis.org/dtd/mybatis-3-mapper.dtd">
- <mappernamespace="org.hyn.maper.UserMapper">
- <!-- 批量插入user表中 -->
- <insertid="insertUser">
-        insert into user(name,age，createTime,createBy,modifyTime,modifyBy)  
-      values  
- <foreachcollection="list"item="item"index="index"separator=",">
-         ( #{item.name},#{item.age},#{item.createTime},#{item.createBy},#{item.modifyTime},#{item.modifyBy} )  
- </foreach>
- </insert>
- 
- <!-- 当使用该Mybatis与Spring整合的时候，该文件必须和相应的Mapper接口文件同名 -->
- </mapper>
4.Mapper接口 (方法名要与映射文件的id一致)
**[java]**[view
 plain](http://blog.csdn.net/yansong_8686/article/details/46975953#)[copy](http://blog.csdn.net/yansong_8686/article/details/46975953#)
- package com.xxxx.xxx.mapper;  
- 
- @MapperScan
- publicinterface XxxxMapper{  
- publicvoid insertUser(@Param("list")List<User> userList);  
- }  
5.java Service层调用
- @Service("userService")  
- publicclass userService
 implements UserService {  
- @Autowired
- private XxxxMapper
 xxxxMapper;  
- 
-     public void addUsers(List<User> users) throws Excetion{
-        xxxxMapper.insertUser(users);
-     }    
- }  
