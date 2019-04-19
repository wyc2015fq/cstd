# Mybatis之Mapper动态代理 - 建建的博客 - CSDN博客
2017年10月16日 11:44:23[纪建](https://me.csdn.net/u013898698)阅读数：78
个人分类：[java学习路线](https://blog.csdn.net/u013898698/article/category/7152763)
一、什么是Mapper的动态代理
采用Mapper动态代理方法只需要编写相应的Mapper接口（相当于Dao接口），那么Mybatis框架根据接口定义创建接口的动态代理对象，代理对象的方法体同Dao接口实现类方法。
Mapper接口开发需要遵循以下规范：
1、Mapper.xml文件中的namespace与mapper接口的全类名相同。
2、Mapper接口方法名和Mapper.xml中定义的每个statement的id相同。
3、Mapper接口方法的输入参数类型和mapper.xml中定义的每个sql 的parameterType的类型相同。
4、Mapper接口方法的输出参数类型和mapper.xml中定义的每个sql的resultType的类型相同。
二、实例演示
这里只演示了利用代理进行查询和插入的操作，其他操作与之情况类似。只需要在接口中定义相关方法，然后在xml中进行相关配置即可。
这个演示是建立在前一个增删改查的项目基础上。项目整体目录大致相同。基本属性文件信息也相同。这里不作详述。
1、新建一个com.kang.mapper的包，定义map接口，接口名任意，这里是UserMapper。
[java][view
 plain](http://blog.csdn.net/xiaokang123456kao/article/details/66476828#)[copy](http://blog.csdn.net/xiaokang123456kao/article/details/66476828#)
- package com.kang.mapper;  
- 
- 
- import java.util.List;  
- import com.kang.pojo.User;  
- publicinterface UserMapper {  
- //根据用户id查询用户信息
- public User findUserById(int id) throws Exception;  
- //查询用户列表
- public List<User> findUserByUsername(String username) throws Exception;  
- //添加用户信息
- publicvoid insertUser(User user)throws Exception;   
- }  
2、配置xml文件
UserMapper.xml
[html][view
 plain](http://blog.csdn.net/xiaokang123456kao/article/details/66476828#)[copy](http://blog.csdn.net/xiaokang123456kao/article/details/66476828#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <!DOCTYPE mapper  
- PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"  
- "http://mybatis.org/dtd/mybatis-3-mapper.dtd">
- <mappernamespace="com.kang.mapper.UserMapper">
- <!-- 注意这里的 namespace必须对应着map接口的全类名-->
- <selectid="findUserById"parameterType="int"resultType="user">
-         select * from user where id = #{id}  
- </select>
- 
- <selectid="findUserByUsername"parameterType="java.lang.String"
- resultType="user">
-         select * from user where username like '%${value}%'  
- </select>
- 
- <insertid="insertUser"parameterType="user">
- <selectKeykeyProperty="id"order="AFTER"resultType="java.lang.Integer">
-             select LAST_INSERT_ID()  
- </selectKey>
-         insert into user(username,birthday,sex,address)  
-         values(#{username},#{birthday},#{sex},#{address})  
- </insert>
- </mapper>
注意到配置文件中的id属性值和map接口中的方法名是一一对应的。
3、在SqlMapConfig.xml中加入映射文件
[html][view
 plain](http://blog.csdn.net/xiaokang123456kao/article/details/66476828#)[copy](http://blog.csdn.net/xiaokang123456kao/article/details/66476828#)
- <!-- 加载 映射文件 -->
- <mappers>
- <mapperresource="map/UserMapper.xml"/>
- </mappers>
4、测试代码
[java][view
 plain](http://blog.csdn.net/xiaokang123456kao/article/details/66476828#)[copy](http://blog.csdn.net/xiaokang123456kao/article/details/66476828#)
- package com.kang.test;  
- 
- 
- import java.io.IOException;  
- import java.io.InputStream;  
- import java.util.Date;  
- import java.util.List;  
- 
- 
- import org.apache.ibatis.io.Resources;  
- import org.apache.ibatis.session.SqlSession;  
- import org.apache.ibatis.session.SqlSessionFactory;  
- import org.apache.ibatis.session.SqlSessionFactoryBuilder;  
- import org.junit.Before;  
- import org.junit.Test;  
- 
- 
- import com.kang.mapper.UserMapper;  
- import com.kang.pojo.User;  
- 
- 
- publicclass UserTest {  
- // 会话工厂
- private SqlSessionFactory sqlSessionFactory;  
- 
- 
- @Before
- publicvoid createSqlSessionFactory() throws IOException {  
- // 配置文件SqlMapConfig.xml在类路径下
-         String resource = "SqlMapConfig.xml";  
- // 得到配置文件流
-         InputStream inputStream = Resources.getResourceAsStream(resource);  
- // 创建会话工厂，传入mybatis的配置文件信息
-         sqlSessionFactory = new SqlSessionFactoryBuilder().build(inputStream);  
- 
- 
-     }  
- 
- 
- @Test
- publicvoid testFindUserById() throws Exception {  
- //获取session
-         SqlSession session = sqlSessionFactory.openSession();  
- //获取mapper接口的代理对象
-         UserMapper userMapper = session.getMapper(UserMapper.class);  
- //调用代理对象方法
-         User user = userMapper.findUserById(27);  
-         System.out.println(user);  
- //关闭session
-         session.close();  
-         System.out.println("---------执行完毕-----------");  
- 
-     }  
- 
- @Test
- publicvoid testFindByUsername() throws Exception{  
- //获取session
-         SqlSession session = sqlSessionFactory.openSession();  
- //获取mapper接口的代理对象
-         UserMapper userMapper = session.getMapper(UserMapper.class);  
- //调用代理对象方法
-         List<User> list=userMapper.findUserByUsername("张");  
- for (User user : list) {  
-             System.out.println(user);  
-         }  
- //关闭session
-         session.close();  
-         System.out.println("---------执行完毕-----------");  
-     }  
- 
- 
- @Test
- publicvoid testInsert() throws Exception{  
-         User user=new User();  
-         user.setUsername("cxk");  
-         user.setSex("1");  
-         user.setBirthday(new Date());  
-         user.setAddress("四川成都");  
- 
-         SqlSession session=sqlSessionFactory.openSession();  
-         UserMapper userMapper=session.getMapper(UserMapper.class);  
-         userMapper.insertUser(user);  
-         session.commit();  
- //关闭session
-         session.close();  
-         System.out.println("---------执行完毕-----------");  
- 
-     }  
- 
- 
- 
- }  
5、单元测试
id查询测试
![](https://img-blog.csdn.net/20170326165640694)
模糊查询测试
![](https://img-blog.csdn.net/20170326165749742)
插入数据测试
![](https://img-blog.csdn.net/20170326165910931)
![](https://img-blog.csdn.net/20170326165940244)
