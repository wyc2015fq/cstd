# ibatis N+1问题 - z69183787的专栏 - CSDN博客
2015年05月31日 15:14:33[OkidoGreen](https://me.csdn.net/z69183787)阅读数：702
个人分类：[对象映射框架-Ibatis/Mybatis](https://blog.csdn.net/z69183787/article/category/2261201)

我们先来看一个问题：一个User用户对应多张信用卡Card
类User:
**[java]**[view
 plain](http://blog.csdn.net/baggio7095586/article/details/8588860#)[copy](http://blog.csdn.net/baggio7095586/article/details/8588860#)
- package com.demo.beans;  
- 
- import java.util.List;  
- 
- publicclass User {  
- privateint id;  
- private String name;  
- private List<Card> cards;  
- 
-         set get省略...  
- }  
类Card:
**[java]**[view
 plain](http://blog.csdn.net/baggio7095586/article/details/8588860#)[copy](http://blog.csdn.net/baggio7095586/article/details/8588860#)
- package com.demo.beans;  
- 
- publicclass Card {  
- privateint id;  
- private String cardName;  
- privatedouble balance;  
- privateint userId;  
- 
-         set get省略...  
- }  
对应数据库中的表结构：
表user:
**[sql]**[view
 plain](http://blog.csdn.net/baggio7095586/article/details/8588860#)[copy](http://blog.csdn.net/baggio7095586/article/details/8588860#)
- CREATETABLE `user` (  
-   `Id` int(11) NOTNULL AUTO_INCREMENT,  
-   `name` varchar(255) DEFAULTNULL,  
- PRIMARYKEY (`Id`)  
- ) ENGINE=InnoDB AUTO_INCREMENT=1004 DEFAULT CHARSET=utf8;  
表card:
**[sql]**[view
 plain](http://blog.csdn.net/baggio7095586/article/details/8588860#)[copy](http://blog.csdn.net/baggio7095586/article/details/8588860#)
- CREATETABLE `card` (  
-   `Id` int(11) NOTNULL AUTO_INCREMENT,  
-   `cardName` varchar(255) DEFAULTNULL,  
-   `balance` doubleDEFAULTNULL,  
-   `userId` int(11) DEFAULTNULL,  
- PRIMARYKEY (`Id`)  
- ) ENGINE=InnoDB AUTO_INCREMENT=2006 DEFAULT CHARSET=utf8;  
我们考虑这样一个需求：从数据库中获得所有用户的信息（包括他所持有的信用卡信息）。
对应的UserCards.xml:
**[java]**[view
 plain](http://blog.csdn.net/baggio7095586/article/details/8588860#)[copy](http://blog.csdn.net/baggio7095586/article/details/8588860#)
- <?xml version="1.0" encoding="UTF-8" ?>  
- 
- <!DOCTYPE sqlMap        
-     PUBLIC "-//ibatis.apache.org//DTD SQL Map 2.0//EN"
- "http://ibatis.apache.org/dtd/sql-map-2.dtd">  
- 
- <sqlMap namespace="UserCards">  
- 
-     <typeAlias alias="Card" type="com.demo.beans.Card" />  
-     <typeAlias alias="User" type="com.demo.beans.User"/>  
- 
-     <resultMap class="User" id="UserResult" >  
-         <result property="id" column="id"/>  
-         <result property="name" column="name"/>  
-         <result property="cards" select="UserCards.getCardsByUserId" column="id"/>  
-     </resultMap>  
- 
-     <resultMap id="CardResult"class="Card">  
-         <result property="id" column="id" />  
-         <result property="cardName" column="cardName" />  
-         <result property="balance" column="balance" />  
-         <result property="userId" column="userId" />  
-     </resultMap>  
- 
-     <select id="getUserById" resultMap="UserResult" parameterClass="int">  
-         select * from user where id = #value#  
-     </select>  
- 
-     <select id="getCardsByUserId" resultMap="CardResult" parameterClass="int">  
-         select * from card where userId = #value#  
-     </select>  
- 
-     <select id="getAllUsers" resultMap="UserResult">  
-         select * from user  
-     </select>  
- 
-     <insert id="addCard" parameterClass="Card" >  
-         <selectKey resultClass="int" type="post"  keyProperty="id" >  
-             select LAST_INSERT_ID() as value  
-         </selectKey>  
-         insert into card values(#id#,#cardName#,#balance#,#userId#)  
-     </insert>  
- 
-     <insert id="addUser" parameterClass="User" >  
-         <selectKey resultClass="int" type="post"  keyProperty="id" >  
-             select LAST_INSERT_ID() as value  
-         </selectKey>  
-         insert into user values(#id#,#name#)  
-     </insert>  
- 
- </sqlMap>  
注意上面xml关键部分：
**[java]**[view
 plain](http://blog.csdn.net/baggio7095586/article/details/8588860#)[copy](http://blog.csdn.net/baggio7095586/article/details/8588860#)
- <result property="cards" select="UserCards.getCardsByUserId" column="id"/>  
每查一个user，根据他的id来getCardsByUserId查卡信息
我们的单元测试部分：
**[java]**[view
 plain](http://blog.csdn.net/baggio7095586/article/details/8588860#)[copy](http://blog.csdn.net/baggio7095586/article/details/8588860#)
- publicvoid testGetAll(){  
-         BeanFactory factory = new ClassPathXmlApplicationContext("applicationContext.xml");  
-         UserDao userDao = (UserDao) factory.getBean("userDao");  
- 
- long start = System.currentTimeMillis();  
- 
-         List<User> userList = userDao.getAllUser();  
- for(User user : userList){  
-             System.out.println(user.getName());  
- for(Card card : user.getCards()){  
-                 System.out.println(card.getCardName()+"|"+card.getBalance());  
-             }  
-             System.out.println("------------------------");  
-         }  
- 
- long end = System.currentTimeMillis();  
-         System.out.println(end-start);  
-     }  
在数据库中我们添加了1000个左右的用户和1000张卡信息，相互对应
控制台输出如下：
**[java]**[view
 plain](http://blog.csdn.net/baggio7095586/article/details/8588860#)[copy](http://blog.csdn.net/baggio7095586/article/details/8588860#)
- xiaoye994  
- 2011-01-1410:38:52,218 DEBUG - {conn-102994} Connection  
- 2011-01-1410:38:52,218 DEBUG - {conn-102994} Preparing Statement:    select * from card where userId = ?    
- 2011-01-1410:38:52,218 DEBUG - {pstm-102995} Executing Statement:    select * from card where userId = ?    
- 2011-01-1410:38:52,218 DEBUG - {pstm-102995} Parameters: [998]  
- 2011-01-1410:38:52,218 DEBUG - {pstm-102995} Types: [java.lang.Integer]  
- 中信A00998|999.0
- ------------------------  
- xiaoye995  
- 2011-01-1410:38:52,218 DEBUG - {conn-102997} Connection  
- 2011-01-1410:38:52,218 DEBUG - {conn-102997} Preparing Statement:    select * from card where userId = ?    
- 2011-01-1410:38:52,218 DEBUG - {pstm-102998} Executing Statement:    select * from card where userId = ?    
- 2011-01-1410:38:52,218 DEBUG - {pstm-102998} Parameters: [999]  
- 2011-01-1410:38:52,218 DEBUG - {pstm-102998} Types: [java.lang.Integer]  
- 中信A00999|1000.0
- ------------------------  
- 2078
可以看到每次输出一个用户名如"xiaoye995"，然后发出一条select * from card where userId = ?语句。
这就是N+1问题：我们有995个用户 那么就会发出995条上面的语句 另外一条则是查询用户的SQL语句select * from user，
这里的N即为995。可以看到，如果要查询所有用户及其卡信息，这样的效率是很差的，总共花了大约2000多毫秒的时间。
解决办法之一，使用懒加载 就是在需要用户卡信息的时候才触发sql调用
**[java]**[view
 plain](http://blog.csdn.net/baggio7095586/article/details/8588860#)[copy](http://blog.csdn.net/baggio7095586/article/details/8588860#)
- <settings lazyLoadingEnabled="true" useStatementNamespaces="true"/>  
解决办法之二，ibatis中已经解决了N+1问题，具体看下面：
修改UserCards.xml文件，关键部分如下：
**[java]**[view
 plain](http://blog.csdn.net/baggio7095586/article/details/8588860#)[copy](http://blog.csdn.net/baggio7095586/article/details/8588860#)
- <resultMap class="User" id="UserResult" groupBy="id">  
-         <result property="id" column="id" nullValue="0"/>  
-         <result property="name" column="name"/>  
-         <result property="cards" resultMap="UserCardsN1.CardResult" />  
- </resultMap>  
- 
- <select id="getAllUsers" resultMap="UserResult">  
-         select * from user a left join card b on a.id = b.userId  
- </select>  
区别主要体现在groupBy="id"和<result property="cards" resultMap="UserCardsN1.CardResult" />及select用了表连接
控制台输出信息：
**[java]**[view
 plain](http://blog.csdn.net/baggio7095586/article/details/8588860#)[copy](http://blog.csdn.net/baggio7095586/article/details/8588860#)
- 2011-01-1411:10:55,156 DEBUG - {conn-100000} Connection  
- 2011-01-1411:10:55,171 DEBUG - {conn-100000} Preparing Statement:    select * from user a left join card b on a.id = b.userId    
- 2011-01-1411:10:55,703 DEBUG - {pstm-100001} Executing Statement:    select * from user a left join card b on a.id = b.userId    
- 2011-01-1411:10:55,703 DEBUG - {pstm-100001} Parameters: []  
- 2011-01-1411:10:55,703 DEBUG - {pstm-100001} Types: []  
- xiaoye  
- 建行卡|4353.0
- 招行卡|23112.0
- 中信A001|2.0
- ------------------------  
- king  
- 工行卡|1000.0
- 中信A002|3.0
- ------------------------  
- ..........  
- .........  
- ------------------------  
- 766
可以看到只发出了一条sql语句 输出了所有的信息 大概用时766毫秒
结论：
1）懒加载适合不需要一次性取出用户信息及其关联信息的场景
2）N+1的解决方法是一次性取出的，适合数据量较少的情况
[http://xiaoye4188.iteye.com/blog/871050](http://xiaoye4188.iteye.com/blog/871050)
