# MyBatis学习笔记(一）入门 - xqhrs232的专栏 - CSDN博客
2018年10月31日 11:12:44[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：41
个人分类：[JavaWeb技术](https://blog.csdn.net/xqhrs232/article/category/8231541)
原文地址::[https://www.cnblogs.com/hellokitty1/p/5216025.html](https://www.cnblogs.com/hellokitty1/p/5216025.html)
相关文章
1、一个简单的MyBatis程序实例----[https://blog.csdn.net/abc5232033/article/details/79048663](https://blog.csdn.net/abc5232033/article/details/79048663)
首先给大家推荐几个网页：
[http://www.mybatis.cn/](http://www.mybatis.cn/)
[http://blog.csdn.net/isea533/article/category/2092001](http://blog.csdn.net/isea533/article/category/2092001)
[http://www.mybatis.org/mybatis-3/zh/index.html](http://www.mybatis.org/mybatis-3/zh/index.html)
[http://www.mybatis.org/spring/zh/index.html](http://www.mybatis.org/spring/zh/index.html)
[http://www.mybatis.tk/](http://www.mybatis.tk/)
这些也是我朋友介绍的。刚刚接触MyBatis可以看看。接下来就开始学习mybatis了。
MyBatis 本是apache的一个开源项目iBatis, 2010年这个项目由apache software foundation 迁移到了google code，并且改名为MyBatis 。2013年11月迁移到Github。
![](https://images2015.cnblogs.com/blog/664931/201602/664931-20160225091618443-1699061978.png)
# 一、理解什么是MyBatis？
MyBatis 是支持普通 SQL 查询，存储过程和高级映射的优秀持久层框架。 MyBatis 消除了几乎所有的 JDBC 代码和参数的手工设置以及对结果集的检索。 MyBatis 可以使用简单的XML 或注解用于配置和原始映射，将接口和 Java 的 POJO（ Plain Old Java Objects，普通的Java 对象）映射成数据库中的记录.
  1）MyBATIS 目前提供了三种语言实现的版本，包括：Java、.NET以及Ruby。（我主要学习java，就讲java的使用）
  2）它提供的持久层框架包括SQL Maps和Data Access Objects（DAO）。
  3）mybatis与hibernate的对比？
   mybatis提供一种“半自动化”的ORM实现。
   这里的“半自动化”，是相对Hibernate等提供了全面的数据库封装机制的“全自动化”ORM实现而言，“全自动”ORM实现了POJO和数据库表之间的映射，以及 SQL 的自动生成和执行。
    而mybatis的着力点，则在于POJO与SQL之间的映射关系。
# 二、简单例子（快速入门）
**1）首先建立项目java web**
![](https://images2015.cnblogs.com/blog/664931/201602/664931-20160225092058583-1663927646.png)
**2）导入mybatis所需的jar包**
![](https://images2015.cnblogs.com/blog/664931/201602/664931-20160225092252193-1652759470.png)
mybatis需要jar包：mybatis-3.3.0.jar
mysql驱动jar包：mysql-connector-java-5.1.15.-bin.jar
日志记录jar包：log4j.jar
**3)创建数据库数据（mysql）**
![](https://images2015.cnblogs.com/blog/664931/201602/664931-20160225093113318-576198027.png)
**4）mysql驱动配置文件（这样可以优化性能） **
 我个人喜欢把所需要使用到的包先建立好。配置文件先创建好。
![](https://images2015.cnblogs.com/blog/664931/201602/664931-20160225093611255-796459809.png)
![](https://images2015.cnblogs.com/blog/664931/201602/664931-20160225093719365-600230536.png)
**5）添加mybatis配置文件mybatis.cfg.xml**
![](https://images2015.cnblogs.com/blog/664931/201602/664931-20160225094128786-1384508381.png)
 1 <?xml version="1.0" encoding="UTF-8"?>
 2 <!DOCTYPE configuration PUBLIC "-//mybatis.org//DTD Config 3.0//EN" "http://mybatis.org/dtd/mybatis-3-config.dtd">
 3 <configuration>
 4 
 5   <!-- 引入外部配置文件 -->
 6   <properties resource="mysql.properties"></properties>
 7    
 8    <!-- 配置mybatis运行环境 -->
 9      <environments default="cybatis">
10         <environment id="cybatis">
11             <!-- type="JDBC" 代表使用JDBC的提交和回滚来管理事务 -->
12             <transactionManager type="JDBC" />
13             
14             <!-- mybatis提供了3种数据源类型，分别是：POOLED,UNPOOLED,JNDI -->
15             <!-- POOLED 表示支持JDBC数据源连接池 -->
16             <!-- UNPOOLED 表示不支持数据源连接池 -->
17             <!-- JNDI 表示支持外部数据源连接池 -->
18             <dataSource type="POOLED">
19                 <property name="driver" value="${jdbc.driver}" />
20                 <property name="url" value="${jdbc.url}" />
21                 <property name="username" value="${jdbc.username}" />
22                 <property name="password" value="${jdbc.password}" />
23             </dataSource>
24         </environment>
25     </environments> 
26     
27 </configuration>
**6)创建对应的实体对象**
![](https://images2015.cnblogs.com/blog/664931/201602/664931-20160225094348130-1229762563.png)
对应的java代码：
 View Code
**7）创建方法接口UserMapper.java和定义操作t_user表的sql映射文件UserMapper.xml**
![](https://images2015.cnblogs.com/blog/664931/201602/664931-20160225094939490-2127633602.png)
 提供简单的增删改查数据信息。
 1 package com.cy.mybatis.mapper;
 2 
 3 import java.util.List;
 4 
 5 import com.cy.mybatis.beans.UserBean;
 6 
 7 public interface UserMapper {
 8     /**
 9      * 新增用戶
10      * @param user
11      * @return
12      * @throws Exception
13      */
14     public int insertUser(UserBean user) throws Exception;
15     /**
16      * 修改用戶
17      * @param user
18      * @param id
19      * @return
20      * @throws Exception
21      */
22     public int updateUser (UserBean user,int id) throws Exception;
23      /**
24       * 刪除用戶
25       * @param id
26       * @return
27       * @throws Exception
28       */
29     public int deleteUser(int id) throws Exception;
30     /**
31      * 根据id查询用户信息
32      * @param id
33      * @return
34      * @throws Exception
35      */
36     public UserBean selectUserById(int id) throws Exception;
37      /**
38       * 查询所有的用户信息
39       * @return
40       * @throws Exception
41       */
42     public List<UserBean> selectAllUser() throws Exception;
43 }
![](https://images2015.cnblogs.com/blog/664931/201602/664931-20160225100026052-314249221.png)
UserMapper.xml
 1 <?xml version="1.0" encoding="UTF-8"?>
 2 <!DOCTYPE mapper PUBLIC "-//mybatis.org/DTD Mapper 3.0" "http://mybatis.org/dtd/mybatis-3-mapper.dtd">
 3 <mapper namespace="com.cy.mybatis.mapper.UserMapper">
 4 <!-- 自定义返回结果集 -->
 5    <resultMap id="userMap" type="UserBean">
 6         <id property="id" column="id" javaType="java.lang.Integer"></id>
 7         <result property="username" column="username" javaType="java.lang.String"></result>
 8         <result property="password" column="password" javaType="java.lang.String"></result>
 9         <result property="account" column="account" javaType="java.lang.Double"></result>
10     </resultMap>
11 <!-- 在各种标签中的id属性必须和接口中的方法名相同 ， id属性值必须是唯一的，不能够重复使用。parameterType属性指明查询时使用的参数类型，resultType属性指明查询返回的结果集类型-->    
12 <!-- useGeneratedKeys：（ 仅 对 insert 有 用 ） 这 会 告 诉 MyBatis 使 用 JDBC 的getGeneratedKeys 
13             方法来取出由数据（比如：像 MySQL 和 SQLServer 这样的数据库管理系统的自动递增字段）内部生成的主键。默认值： false。 -->    
14 <!--keyProperty： （仅对 insert有用）标记一个属性， MyBatis 会通过 getGeneratedKeys或者通过 insert 语句的 selectKey 子元素设置它的值。默认：不设置。 -->
15 <!--#{}中的内容，为占位符，当参数为某个JavaBean时，表示放置该Bean对象的属性值  -->
16 
17 
18     <insert id="insertUser" useGeneratedKeys="true" keyProperty="id">
19         insert into t_user (username,password,account) values (#{username},#{password},#{account})
20     </insert>
21     
22     <update id="updateUser" >
23       update t_user set username=#{username},password=#{password},account=#{account} where id=#{id}
24     </update>
25     
26     <delete id="deleteUser" parameterType="int">
27      delete from t_user where id=#{id}  
28     </delete>
29     
30     <select id="selectUserById" parameterType="int" resultMap="userMap">
31      select * from t_user where id=#{id}
32     </select>
33     
34     <select id="selectAllUser" resultMap="userMap">
35      select * from t_user
36     </select>
37     
38     
39 </mapper>    
这时需要为mybatis.cfg.xml里注册UserMapper.xml文件。
 1 <?xml version="1.0" encoding="UTF-8"?>
 2 <!DOCTYPE configuration PUBLIC "-//mybatis.org//DTD Config 3.0//EN" "http://mybatis.org/dtd/mybatis-3-config.dtd">
 3 <configuration>
 4 
 5   <!-- 引入外部配置文件 -->
 6   <properties resource="mysql.properties"></properties>
 7   
 8   
 9    <!-- 为JAVA Bean起类别名 -->
10    <typeAliases >
11         <!-- 别名方式1，一个一个的配置 type中放置的是类的全路径，alias中放置的是类别名
12         <typeAliase type="com.cy.mybatis.beans.UserBean" alias="UserBean"/> -->
13         <!-- 别名方式2，自动扫描，将JAVA类的类名作为类的类别名 -->
14         <package name="com.cy.mybatis.beans"/>
15    </typeAliases>
16    
17    
18    <!-- 配置mybatis运行环境 -->
19      <environments default="cybatis">
20         <environment id="cybatis">
21             <!-- type="JDBC" 代表使用JDBC的提交和回滚来管理事务 -->
22             <transactionManager type="JDBC" />
23             
24             <!-- mybatis提供了3种数据源类型，分别是：POOLED,UNPOOLED,JNDI -->
25             <!-- POOLED 表示支持JDBC数据源连接池 -->
26             <!-- UNPOOLED 表示不支持数据源连接池 -->
27             <!-- JNDI 表示支持外部数据源连接池 -->
28             <dataSource type="POOLED">
29                 <property name="driver" value="${jdbc.driver}" />
30                 <property name="url" value="${jdbc.url}" />
31                 <property name="username" value="${jdbc.username}" />
32                 <property name="password" value="${jdbc.password}" />
33             </dataSource>
34         </environment>
35     </environments> 
36     
37     
38     <mappers>
39         <!-- 告知映射文件方式1，一个一个的配置
40         <mapper resource="com/cy/mybatis/mapper/UserMapper.xml"/>-->
41         <!-- 告知映射文件方式2，自动扫描包内的Mapper接口与配置文件 -->
42         <package name="com/cy/mybatis/mapper"/>
43     </mappers>
44 </configuration>
**8）需要建立一个工具类文件**
![](https://images2015.cnblogs.com/blog/664931/201602/664931-20160225100542740-1105319311.png)
 1 package com.cy.mybatis.tools;
 2 
 3 import java.io.Reader;
 4 
 5 import org.apache.ibatis.io.Resources;
 6 import org.apache.ibatis.session.SqlSession;
 7 import org.apache.ibatis.session.SqlSessionFactory;
 8 import org.apache.ibatis.session.SqlSessionFactoryBuilder;
 9 
10 public class DBTools {
11     public static SqlSessionFactory sessionFactory;
12     
13     static{
14         try {
15             //使用MyBatis提供的Resources类加载mybatis的配置文件
16             Reader reader = Resources.getResourceAsReader("mybatis.cfg.xml");
17             //构建sqlSession的工厂
18             sessionFactory = new SqlSessionFactoryBuilder().build(reader);
19         } catch (Exception e) {
20             e.printStackTrace();
21         }
22         
23     }
24     //创建能执行映射文件中sql的sqlSession
25     public static SqlSession getSession(){
26         return sessionFactory.openSession();
27     }
28     
29 }
**9）写个测试**
![](https://images2015.cnblogs.com/blog/664931/201602/664931-20160225101250880-1610201607.png)
 1 package com.cy.mybatis.service;
 2 
 3 import java.util.List;
 4 
 5 import org.apache.ibatis.session.SqlSession;
 6 
 7 import com.cy.mybatis.beans.UserBean;
 8 import com.cy.mybatis.tools.DBTools;
 9 import com.cy.mybatis.mapper.UserMapper;
10 
11 public class UserService {
12 
13    
14 15      
16     public static void main(String[] args) {
17           insertUser();
18 //        deleteUser();
19 //        selectUserById();
20 //        selectAllUser();
21     }
22 
23     
24     /**
25      * 新增用户
26      */
27     private static void insertUser() {
28         SqlSession session = DBTools.getSession();
29         UserMapper mapper = session.getMapper(UserMapper.class);
30         UserBean user = new UserBean("懿", "1314520", 7000.0);
31         try {
32             mapper.insertUser(user);
33             System.out.println(user.toString());
34              session.commit();
35         } catch (Exception e) {
36             e.printStackTrace();
37             session.rollback();
38         }
39     }
40     
41     
42     /**
43      * 删除用户
44      */
45     private static void deleteUser(){
46         SqlSession session=DBTools.getSession();
47         UserMapper mapper=session.getMapper(UserMapper.class);
48         try {
49             mapper.deleteUser(1);
50             session.commit();
51         } catch (Exception e) {
52             e.printStackTrace();
53             session.rollback();
54         }
55     }
56     
57     
58     /**
59      * 根据id查询用户
60      */
61     private static void selectUserById(){
62         SqlSession session=DBTools.getSession();
63         UserMapper mapper=session.getMapper(UserMapper.class);
64         try {
65         UserBean user=    mapper.selectUserById(2);
66         System.out.println(user.toString());
67             
68             session.commit();
69         } catch (Exception e) {
70             e.printStackTrace();
71             session.rollback();
72         }
73     }
74     
75     /**
76      * 查询所有的用户
77      */
78     private static void selectAllUser(){
79         SqlSession session=DBTools.getSession();
80         UserMapper mapper=session.getMapper(UserMapper.class);
81         try {
82         List<UserBean> user=mapper.selectAllUser();
83         System.out.println(user.toString());
84         session.commit();
85         } catch (Exception e) {
86             e.printStackTrace();
87             session.rollback();
88         }
89     }
90     
91 
92 }
测试结果
![](https://images2015.cnblogs.com/blog/664931/201602/664931-20160225101805599-1095270516.png)insert。
每件事都需要坚持！
