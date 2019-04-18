# MyBatis一对多和多对一 - z69183787的专栏 - CSDN博客
2015年07月10日 15:31:03[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1533
   在学习MyBatis3的过程中，文档上面一直在强调一个id的东西！在做这个实验的时候，也因为没有理解清楚id含义而导致**一对多的“多”中也只有一条数据**。id和result的唯一不同是id表示的结果将是当比较对象实例时用到的标识属性。这帮助来改进整体表现，特别是缓存和嵌入结果映射。**所以不同数据的id应该唯一区别**，不然导致数据**结果集只有一条数据。**
**一、表**
**![](https://img-blog.csdn.net/20131029232523250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcmFpbjA5Nzc5MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20131029232538828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcmFpbjA5Nzc5MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)**
**二、实体**
**1.person**
**[java]**[view
 plain](http://blog.csdn.net/rain097790/article/details/13615291#)[copy](http://blog.csdn.net/rain097790/article/details/13615291#)
- package com.kerwin.mybatis.pojo;  
- 
- import java.util.List;  
- 
- publicclass Person {  
- 
- privateint id;  
- private String name;  
- private List<Orders> orderList;  
- 
- publicint getId() {  
- return id;  
-     }  
- 
- publicvoid setId(int id) {  
- this.id = id;  
-     }  
- 
- public String getName() {  
- return name;  
-     }  
- 
- publicvoid setName(String name) {  
- this.name = name;  
-     }  
- 
- public List<Orders> getOrderList() {  
- return orderList;  
-     }  
- 
- publicvoid setOrderList(List<Orders> orderList) {  
- this.orderList = orderList;  
-     }  
- 
- @Override
- public String toString() {  
- return"Person [id=" + id + ", name=" + name + "]";  
-     }  
- 
- public Person() {  
- super();  
- // TODO Auto-generated constructor stub
-     }  
- 
- public Person(int id, String name, List<Orders> orderList) {  
- super();  
- this.id = id;  
- this.name = name;  
- this.orderList = orderList;  
-     }  
- 
- }  
2.order
**[java]**[view
 plain](http://blog.csdn.net/rain097790/article/details/13615291#)[copy](http://blog.csdn.net/rain097790/article/details/13615291#)
- package com.kerwin.mybatis.pojo;  
- 
- publicclass Orders {  
- privateint id;  
- privatedouble price;  
- private Person person;  
- 
- 
- 
- public Person getPerson() {  
- return person;  
-     }  
- 
- publicvoid setPerson(Person person) {  
- this.person = person;  
-     }  
- 
- publicint getId() {  
- return id;  
-     }  
- 
- publicvoid setId(int id) {  
- this.id = id;  
-     }  
- 
- 
- 
- publicdouble getPrice() {  
- return price;  
-     }  
- 
- publicvoid setPrice(double price) {  
- this.price = price;  
-     }  
- 
- 
- 
- 
- @Override
- public String toString() {  
- return"Orders [id=" + id + ", price=" + price + "]";  
-     }  
- 
- public Orders() {  
- super();  
- // TODO Auto-generated constructor stub
-     }  
- 
- }  
三、映射mapper文件
**1. PersonMapper.xml**
**[html]**[view
 plain](http://blog.csdn.net/rain097790/article/details/13615291#)[copy](http://blog.csdn.net/rain097790/article/details/13615291#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <!DOCTYPE mapper  
-   PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"  
-   "http://mybatis.org/dtd/mybatis-3-mapper.dtd">
- 
- <mappernamespace="com.kerwin.mybatis.pojo.Person">
- <resultMaptype="com.kerwin.mybatis.pojo.Person"id="personreSultMap">
- <idcolumn="p_id"property="id"/>
- <resultcolumn="name"property="name"/>
- <collectionproperty="orderList"ofType="com.kerwin.mybatis.pojo.Orders"column="pid">
- <idcolumn="o_id"property="id"/>
- <resultcolumn="price"property="price"/>
- </collection>
- 
- </resultMap>
- 
- <selectid="selectPersonFetchOrder"parameterType="int"resultMap="personreSultMap">
-         select p.*,o.* from person p,orders o where o.pid=p.p_id and p.p_id=#{id}  
- </select>
- 
- 
- </mapper>
**2.     OrdersMapper.xml**
**[html]**[view
 plain](http://blog.csdn.net/rain097790/article/details/13615291#)[copy](http://blog.csdn.net/rain097790/article/details/13615291#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <!DOCTYPE mapper  
-   PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"  
-   "http://mybatis.org/dtd/mybatis-3-mapper.dtd">
- 
- <mappernamespace="com.kerwin.mybatis.pojo.Orders">
- <resultMaptype="com.kerwin.mybatis.pojo.Orders"id="OrdersResultMap">
- <idcolumn="o_id"property="id"/>
- <resultcolumn="price"property="price"/>
- <associationproperty="person"javaType="com.kerwin.mybatis.pojo.Person">
- <idcolumn="p_id"property="id"/>
- <resultcolumn="name"property="name"/>
- </association>
- </resultMap>
- 
- <selectid="selectOrdersFetchPerson"resultMap="OrdersResultMap">
-         select p.*,o.* from person p,orders o where o.pid=p.p_id and o.o_id=#{id}   
- </select>
- 
- </mapper>
**3.sqlMapConfig.xml**
**[html]**[view
 plain](http://blog.csdn.net/rain097790/article/details/13615291#)[copy](http://blog.csdn.net/rain097790/article/details/13615291#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <!DOCTYPE configuration  
-   PUBLIC "-//mybatis.org//DTD Config 3.0//EN"  
-   "http://mybatis.org/dtd/mybatis-3-config.dtd">
- <configuration>
- 
- <typeAliases>
- <typeAliastype="com.kerwin.mybatis.pojo.Author"alias="Author"/>
- </typeAliases>
- <environmentsdefault="development">
- <environmentid="development">
- <transactionManagertype="JDBC"/>
- <dataSourcetype="POOLED">
- <propertyname="driver"value="com.mysql.jdbc.Driver"/>
- <propertyname="url"value="jdbc:mysql://localhost:3306/mybatis"/>
- <propertyname="username"value="root"/>
- <propertyname="password"value="root"/>
- </dataSource>
- </environment>
- </environments>
- <mappers>
- <mapperresource="com/kerwin/mybatis/pojo/AuthorMapper.xml"/>
- <mapperresource="com/kerwin/mybatis/pojo/PostMapper.xml"/>
- <mapperresource="com/kerwin/mybatis/pojo/PersonMapper.xml"/>
- <mapperresource="com/kerwin/mybatis/pojo/OrdersMapper.xml"/>
- </mappers>
- </configuration>
四。测试类
**[java]**[view
 plain](http://blog.csdn.net/rain097790/article/details/13615291#)[copy](http://blog.csdn.net/rain097790/article/details/13615291#)
- /**
-  * 
-  */
- package com.kerwin.mybatis.test;  
- 
- import java.io.InputStream;  
- 
- import org.apache.ibatis.io.Resources;  
- import org.apache.ibatis.session.SqlSession;  
- import org.apache.ibatis.session.SqlSessionFactory;  
- import org.apache.ibatis.session.SqlSessionFactoryBuilder;  
- import org.junit.BeforeClass;  
- import org.junit.Test;  
- 
- import com.kerwin.mybatis.pojo.Orders;  
- import com.kerwin.mybatis.pojo.Person;  
- 
- /**
-  * @author Administrator
-  * 
-  */
- publicclass PersonAndOrderTest {  
- 
- privatestatic SqlSessionFactory sessionFactory;  
- 
- /**
-      * @throws java.lang.Exception
-      */
- @BeforeClass
- publicstaticvoid setUpBeforeClass() throws Exception {  
-         SqlSessionFactoryBuilder factoryBuilder = new SqlSessionFactoryBuilder();  
-         InputStream inputStream = Resources.getResourceAsStream("sqlMapConfig.xml");  
-         sessionFactory = factoryBuilder.build(inputStream);  
- 
-     }  
- 
- //一对多，查询person（一）级联查询订单order（多）
- @Test
- publicvoid testSelectPersonFetchOrder() throws Exception {  
-         SqlSession session = sessionFactory.openSession();  
-         Person person = session.selectOne("com.kerwin.mybatis.pojo.Person.selectPersonFetchOrder", 1);  
-         System.out.println(person);  
-         System.out.println(person.getOrderList().size());  
- for(Orders orders : person.getOrderList()){  
-             System.out.println(orders);  
-         }  
-         session.close();  
-     }  
- 
- //多对一，查询订单order（多）级联查询person（一）
- @Test
- publicvoid testSelectOrdersFetchPerson() throws Exception{  
-         SqlSession session = sessionFactory.openSession();  
-         Orders orders = session.selectOne("com.kerwin.mybatis.pojo.Orders.selectOrdersFetchPerson", 1);  
-         System.out.println(orders);  
-         System.out.println(orders.getPerson());  
-         session.close();  
-     }  
- 
- }  
**五、测试结果**
**1.一对多，查询person（一）级联查询订单order（多）**
**![](https://img-blog.csdn.net/20131029233424515?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcmFpbjA5Nzc5MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**
**2.多对一，查询订单order（多）级联查询person（一）**
**![](https://img-blog.csdn.net/20131029233509359?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcmFpbjA5Nzc5MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**
**注意：两张表中的主键id字段名要唯一，例如不能都写id，不然的话，在一对多查询的时候就会出现：级联出来的订单项只有一条记录。我之前就是将两张表的主键id字段名都写为id，导致测试结果级联出来的多一直只有一条数据，具体如下：**
**![](https://img-blog.csdn.net/20131029234156484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcmFpbjA5Nzc5MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**
