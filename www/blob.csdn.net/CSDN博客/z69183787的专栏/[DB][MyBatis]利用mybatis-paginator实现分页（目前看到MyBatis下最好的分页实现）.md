# [DB][MyBatis]利用mybatis-paginator实现分页（目前看到MyBatis下最好的分页实现） - z69183787的专栏 - CSDN博客
2015年10月07日 11:16:24[OkidoGreen](https://me.csdn.net/z69183787)阅读数：7037
利用mybatis-paginator实现分页
1、mybatis-paginator简介
mybatis-paginator是gethub上的一个开源项目、用于java后台获取分页数据、该开源项目还提供一个列表组件（mmgrid）用于前端展示。
该开源项目地址：[https://github.com/miemiedev](https://github.com/miemiedev)
2、该开源项目的使用说明：
Maven中加入依赖：
**[html]**[view
 plain](http://blog.csdn.net/szwangdf/article/details/27859847#)[copy](http://blog.csdn.net/szwangdf/article/details/27859847#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/371962/fork)
- <dependencies>
-   ...  
- <dependency>
- <groupId>com.github.miemiedev</groupId>
- <artifactId>mybatis-paginator</artifactId>
- <version>1.2.10</version>
- </dependency>
-   ...  
- </dependencies>
Mybatis配置文件添加分页插件：
**[html]**[view
 plain](http://blog.csdn.net/szwangdf/article/details/27859847#)[copy](http://blog.csdn.net/szwangdf/article/details/27859847#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/371962/fork)
- <?xmlversionxmlversion="1.0"encoding="UTF-8"?>
- <!DOCTYPE configuration  PUBLIC "-//ibatis.apache.org//DTD Config 3.0//EN"  "http://ibatis.apache.org/dtd/ibatis-3-config.dtd">
- <configuration>
- <plugins>
- <plugininterceptor="com.github.miemiedev.mybatis.paginator.OffsetLimitInterceptor">
- <propertynamepropertyname="dialectClass"value="com.github.miemiedev.mybatis.paginator.dialect.OracleDialect"/>
- </plugin>
- </plugins>
- </configuration>
创建一个查询，内容可以是任何Mybatis表达式，包括foreach和if等：
**[html]**[view
 plain](http://blog.csdn.net/szwangdf/article/details/27859847#)[copy](http://blog.csdn.net/szwangdf/article/details/27859847#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/371962/fork)
- <selectidselectid="findByCity"resultType="map">
-     select * from TEST_USER where city = #{city};  
- </select>
Dao中的方法或许是这样（用接口也是类似）：
**[java]**[view
 plain](http://blog.csdn.net/szwangdf/article/details/27859847#)[copy](http://blog.csdn.net/szwangdf/article/details/27859847#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/371962/fork)
- public List findByCity(String city, PageBounds pageBounds){  
-     Map<String, Object> params =new HashMap<String, Object>();  
-     params.put("city",city);  
-     returngetSqlSession().selectList("db.table.user.findByCity", params, pageBounds);  
- }  
调用方式（分页加多列排序）：
**[java]**[view
 plain](http://blog.csdn.net/szwangdf/article/details/27859847#)[copy](http://blog.csdn.net/szwangdf/article/details/27859847#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/371962/fork)
- int page = 1; //页号
- int pageSize = 20; //每页数据条数
- String sortString = "age.asc,gender.desc";//如果你想排序的话逗号分隔可以排序多列
- 
- PageBounds pageBounds = newPageBounds(page, pageSize , Order.formString(sortString));  
- 
- List list = findByCity("BeiJing",pageBounds);  
- 
- //获得结果集条总数
- PageList pageList = (PageList)list;  
- System.out.println("totalCount: "+ pageList.getPaginator().getTotalCount());  
PageList类是继承于ArrayList的，这样Dao中就不用为了专门分页再多写一个方法。
使用PageBounds这个对象来控制结果的输出，常用的使用方式一般都可以通过构造函数来配置。
**[java]**[view
 plain](http://blog.csdn.net/szwangdf/article/details/27859847#)[copy](http://blog.csdn.net/szwangdf/article/details/27859847#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/371962/fork)
- new PageBounds();//默认构造函数不提供分页，返回ArrayList
- new PageBounds(int limit);//取TOPN操作，返回ArrayList
- new PageBounds(Order... order);//只排序不分页，返回ArrayList
- new PageBounds(int page, int limit);//默认分页，返回PageList
- new PageBounds(int page, int limit, Order... order);//分页加排序，返回PageList
- new PageBounds(int page, int limit, List<Order> orders,boolean containsTotalCount);//使用containsTotalCount来决定查不查询totalCount，即返回ArrayList还是PageList
=========================================
如果用的是Spring MVC的话可以把JSON的配置写成这样：
**[html]**[view
 plain](http://blog.csdn.net/szwangdf/article/details/27859847#)[copy](http://blog.csdn.net/szwangdf/article/details/27859847#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/371962/fork)
- <mvc:annotation-driven>
- <mvc:message-convertersregister-defaults="true">
- <beanclass="org.springframework.http.converter.StringHttpMessageConverter">
- <constructor-argvalueconstructor-argvalue="UTF-8"/>
- </bean>
- 
- <beanclass="org.springframework.http.converter.json.MappingJackson2HttpMessageConverter">
- <propertyname="objectMapper">
- <beanclass="com.github.miemiedev.mybatis.paginator.jackson2.PageListJsonMapper"/>
- </property>
- </bean>
- </mvc:message-converters>
- </mvc:annotation-driven>
那么在Controller就可以这样用了：
**[java]**[view
 plain](http://blog.csdn.net/szwangdf/article/details/27859847#)[copy](http://blog.csdn.net/szwangdf/article/details/27859847#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/371962/fork)
- @ResponseBody
- @RequestMapping(value ="/findByCity.json")  
- public List findByCity(@RequestParam String city,  
- @RequestParam(required =false,defaultValue ="1") intpage,  
- @RequestParam(required =false,defaultValue ="30") intlimit,  
- @RequestParam(required =false) String sort,  
- @RequestParam(required =false) String dir) {  
- 
- return userService.findByCity(city, newPageBounds(page, limit, Order.create(sort,dir)));  
- }  
- 

然后序列化后的JSON字符串就会变成这样的：
**[javascript]**[view
 plain](http://blog.csdn.net/szwangdf/article/details/27859847#)[copy](http://blog.csdn.net/szwangdf/article/details/27859847#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/371962/fork)
- {  
- "items":[  
-         {"NAME":"xiaoma","AGE":30,"GENDER":1,"ID":3,"CITY":"BeiJing"},  
-         {"NAME":"xiaoli","AGE":30,"SCORE":85,"GENDER":1,"ID":1,"CITY":"BeiJing"},  
-         {"NAME":"xiaowang","AGE":30,"SCORE":92,"GENDER":0,"ID":2,"CITY":"BeiJing"},  
-         {"NAME":"xiaoshao","AGE":30,"SCORE":99,"GENDER":0,"ID":4,"CITY":"BeiJing"}  
-     ],  
- 
- "slider": [1, 2, 3, 4, 5, 6, 7],  
- "hasPrePage":false,  
- "startRow": 1,  
- "offset": 0,  
- "lastPage":false,  
- "prePage": 1,  
- "hasNextPage":true,  
- "nextPage": 2,  
- "endRow": 30,  
- "totalCount": 40351,  
- "firstPage":true,  
- "totalPages": 1346,  
- "limit": 30,  
- "page": 1  
- }  
=========================================
在SpringMVC中使用JSTL的话可以参考一下步骤（懒人用法）
在Spring配置文件中加入拦截器,或则参考拦截器实现定义自己的拦截器
**[html]**[view
 plain](http://blog.csdn.net/szwangdf/article/details/27859847#)[copy](http://blog.csdn.net/szwangdf/article/details/27859847#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/371962/fork)
- <mvc:interceptors>
- <mvc:interceptor>
- <mvc:mappingpath="/**"/>
- <beanclass="com.github.miemiedev.mybatis.paginator.springmvc.PageListAttrHandlerInterceptor"/>
- </mvc:interceptor>
- </mvc:interceptors>
然后Controller方法可以这样写
**[java]**[view
 plain](http://blog.csdn.net/szwangdf/article/details/27859847#)[copy](http://blog.csdn.net/szwangdf/article/details/27859847#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/371962/fork)
- @RequestMapping(value ="/userView.action")  
- public ModelAndView userView(@RequestParam String city,  
- @RequestParam(required =false,defaultValue ="1")intpage,  
- @RequestParam(required =false,defaultValue ="30")intlimit,  
- @RequestParam(required =false) String sort,  
- @RequestParam(required =false) String dir) {  
-     List users = userService.findByCity(city,newPageBounds(page, limit, Order.create(sort,dir)));  
-     returnnewModelAndView("account/user","users", users);  
- }  
JSP中就可以这样用了，拦截器会将PageList分拆添加Paginator属性，默认命名规则为"原属性名称"+"Paginator"
**[html]**[view
 plain](http://blog.csdn.net/szwangdf/article/details/27859847#)[copy](http://blog.csdn.net/szwangdf/article/details/27859847#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/371962/fork)
- <table>
- <c:forEachitems="${users}"var="user">
- <tr>
- <td>${user['ID']}</td>
- <td>${user['NAME']}</td>
- <td>${user['AGE']}</td>
- </tr>
- </c:forEach>
- </table>
- 
- 上一页: ${usersPaginator.prePage}   
- 当前页: ${usersPaginator.page}   
- 下一页: ${usersPaginator.nextPage}   
- 总页数: ${usersPaginator.totalPages}   
- 总条数: ${usersPaginator.totalCount}   
- 
- 
- 更多属性参考Paginator类提供的方法  
=========================================
如果用如下方法设置pageBounds，当前这个查询就可以用两个线程同时查询list和totalCount了
|``|```xmlpageBounds.setAsyncTotalCount(true);```|
如果所有的分页查询都是用异步的方式查询list和totalCount，可以在插件配置加入asyncTotalCount属性
**[html]**[view
 plain](http://blog.csdn.net/szwangdf/article/details/27859847#)[copy](http://blog.csdn.net/szwangdf/article/details/27859847#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/371962/fork)
- <plugininterceptor="com.github.miemiedev.mybatis.paginator.OffsetLimitInterceptor">
- <propertyname="dialectClass"value="com.github.miemiedev.mybatis.paginator.dialect.OracleDialect"/>
- <propertyname="asyncTotalCount"value="true"/>
- </plugin>
但是你仍然可以用下面代码强制让这个查询不用异步
**[java]**[view
 plain](http://blog.csdn.net/szwangdf/article/details/27859847#)[copy](http://blog.csdn.net/szwangdf/article/details/27859847#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/371962/fork)
- pageBounds.setAsyncTotalCount(false);  
当然需要注意的是，只要你用到了异步查询，由于里面使用了线程池，所以在使用时就要加入清理监听器，以便在停止服务时关闭线程池。需要在web.xml中加入
**[html]**[view
 plain](http://blog.csdn.net/szwangdf/article/details/27859847#)[copy](http://blog.csdn.net/szwangdf/article/details/27859847#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/371962/fork)
- <listener>
- <listener-class>com.github.miemiedev.mybatis.paginator.CleanupMybatisPaginatorListener</listener-class>
- </listener>

