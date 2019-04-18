# Spring mvc动态多数据源 - z69183787的专栏 - CSDN博客
2012年11月25日 20:39:51[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2101
本文基于[Spring MVC，拦截器实现Session控制](http://blog.csdn.net/geloin/article/details/7537070)。
        本文通过拦截器取得当前使用的Locale，然后通过Locale找到不同的数据源。
        首先，新建类DynamicDataSource，使其继承org.springframework.jdbc.datasource.lookup.AbstractRoutingDataSource并实现其determineCurrentLookupKey方法，并实现获取currentLookupKey的方法，代码如下所示：
**[java]**[view
 plain](http://blog.csdn.net/geloin/article/details/7580685#)[copy](http://blog.csdn.net/geloin/article/details/7580685#)
- /**
-  *
-  * @author geloin
-  * @date 2012-5-18 下午3:20:51
-  */
- package com.embest.ruisystem.datasource;  
- 
- import org.springframework.jdbc.datasource.lookup.AbstractRoutingDataSource;  
- 
- /**
-  * 动态数据源
-  * 
-  * @author geloin
-  * @date 2012-5-18 下午3:20:51
-  */
- publicclass DynamicDataSource extends AbstractRoutingDataSource {  
- 
- privatestaticfinal ThreadLocal<String> contextHolder = new ThreadLocal<String>();  
- 
- /**
-      * 
-      * @author geloin
-      * @date 2012-5-18 下午4:06:44
-      * @return the currentLookupKey
-      */
- publicstatic String getCurrentLookupKey() {  
- return (String) contextHolder.get();  
-     }  
- 
- /**
-      * 
-      * @author geloin
-      * @date 2012-5-18 下午4:06:44
-      * @param currentLookupKey
-      *            the currentLookupKey to set
-      */
- publicstaticvoid setCurrentLookupKey(String currentLookupKey) {  
-         contextHolder.set(currentLookupKey);  
-     }  
- 
- /*
-      * (non-Javadoc)
-      * 
-      * @see
-      * org.springframework.jdbc.datasource.lookup.AbstractRoutingDataSource#
-      * determineCurrentLookupKey()
-      */
- @Override
- protected Object determineCurrentLookupKey() {  
- return getCurrentLookupKey();  
-     }  
- 
- }  
        上述代码中的determineCurrentLookupKey方法取得一个字符串，该字符串将与配置文件中的相应字符串进行匹配以定位数据源，配置文件，即applicationContext.xml文件中需要要如下代码：
**[java]**[view
 plain](http://blog.csdn.net/geloin/article/details/7580685#)[copy](http://blog.csdn.net/geloin/article/details/7580685#)
-       <!-- 动态数据源 -->  
- lt;bean id="dataSource"class="com.embest.ruisystem.datasource.DynamicDataSource">  
- <property name="targetDataSources">  
-     <map key-type="java.lang.String">  
-         <entry key="zh" value-ref="chinaDataSource" />  
-         <entry key="en" value-ref="englishDataSource" />  
-     </map>  
- </property>  
- <property name="defaultTargetDataSource" ref="chinaDataSource" />  
- lt;/bean>  
        determineCurrentLookupKey方法取得字符串后，将会与上述配置中的<map...></map>中的值对应，即当determineCurrentLookupKey方法取得值为en时，则数据源指向englishDataSource，当然，map中的value-ref对应的是你在applicationContext.xml文件中配置的数据源，如下所述：
**[java]**[view
 plain](http://blog.csdn.net/geloin/article/details/7580685#)[copy](http://blog.csdn.net/geloin/article/details/7580685#)
-        <!--创建中国jdbc数据源 -->  
- <bean id="chinaDataSource"class="org.apache.commons.dbcp.BasicDataSource"
-     destroy-method="close">  
-     <property name="driverClassName" value="${driver_zh}" />  
-     <property name="url" value="${url_zh}" />  
-     <property name="username" value="${username_zh}" />  
-     <property name="password" value="${password_zh}" />  
- </bean>  
- <!--创建英国jdbc数据源 -->  
- <bean id="englishDataSource"class="org.apache.commons.dbcp.BasicDataSource"
-     destroy-method="close">  
-     <property name="driverClassName" value="${driver_en}" />  
-     <property name="url" value="${url_en}" />  
-     <property name="username" value="${username_en}" />  
-     <property name="password" value="${password_en}" />  
- </bean>  
        若determineCurrentLookupKey方法未取得任何值时，则指向defaultTargetDataSource所代表的数据源。
        需要注明的是，上述配置中的{url_en}等值来自于jdbc.properties：
**[java]**[view
 plain](http://blog.csdn.net/geloin/article/details/7580685#)[copy](http://blog.csdn.net/geloin/article/details/7580685#)
- driver_zh=com.mysql.jdbc.Driver  
- url_zh=jdbc:mysql://localhost:3306/ruisystem
- username_zh=root  
- password_zh=root  
- 
- driver_en=com.mysql.jdbc.Driver  
- url_en=jdbc:mysql://localhost:3306/ruisystem_en
- username_en=root  
- password_en=root  
        以上工作做好后，还差最后一步，即何时给determineCurrentLookupKey()方法传值。通过DynamicDataSource类可知，该方法的值可通过外办调用setCurrentLookupKey方法设置，作者在拦截器中添加如下代码进行设置：
**[java]**[view
 plain](http://blog.csdn.net/geloin/article/details/7580685#)[copy](http://blog.csdn.net/geloin/article/details/7580685#)
- Locale locale = RequestContextUtils.getLocaleResolver(request) .resolveLocale(request);  
- DynamicDataSource.setCurrentLookupKey(locale.getLanguage());  
        进行上述所有操作后，可以实现国际化数据库，即各个国家使用不同的数据库。
