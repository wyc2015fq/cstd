# DUBBO服务管理配置Zookeeper集群服务 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年05月11日 13:15:11[boonya](https://me.csdn.net/boonya)阅读数：484








Dubbo如何使用请参考我之前的文章：[DUBBO分布式服务框架与SpringMVC+Zookeeper实现服务提供和消费](http://blog.csdn.net/boonya/article/details/69397962)


本文就之前的内容继续深入研究，并整理出更详细的文档：[http://pan.baidu.com/s/1kUMx3in](http://pan.baidu.com/s/1kUMx3in)


### Dubbo与Zookeeper集群架构



![](https://img-blog.csdn.net/20170511130733444)

Tomcat注册服务到Zookeeper集群，由Dubbo进行统一管理。

### Dubbo配置Zookeeper

修改dubbo/WEB-INF/dubbo.properties文件：

```
dubbo.registry.address=zookeeper://192.168.200.122:2181?backup=192.168.200.122:2182,192.168.200.122:2183

dubbo.admin.root.password=root

dubbo.admin.guest.password=guest
```


###  配置Provider-Customer节点

####  Node1

***    Provider applicationContext.xml***

```
<?xmlversion="1.0" encoding="UTF-8"?>

<beansxmlns="http://www.springframework.org/schema/beans"

    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"xmlns:dubbo="http://code.alibabatech.com/schema/dubbo"

    xsi:schemaLocation="http://www.springframework.org/schema/beans

     http://www.springframework.org/schema/beans/spring-beans.xsd       

     http://code.alibabatech.com/schema/dubbo       

     http://code.alibabatech.com/schema/dubbo/dubbo.xsd">

 

    <!-- 提供方应用信息，用于计算依赖关系 -->

    <dubbo:applicationname="webapp-api-provider" />

 

    <!-- 使用zookeeper注册中心暴露服务地址 -->

    <!--<dubbo:registryaddress="zookeeper://192.168.200.122:2181?" />-->

   

    <!-- zookeeper注册中心暴露服务集群配置 -->

    <dubbo:registryaddress="zookeeper://192.168.200.122:2181?backup=192.168.200.122:2182,192.168.200.122:2183"/>

   

    <!-- 用dubbo协议在20881端口暴露服务（如果有多个提供者，则端口号不能重复） -->

    <dubbo:protocol name="dubbo"port="20881" />

 

    <!-- 声明需要暴露的服务接口（interface是全路径名，不能自定义） -->

    <dubbo:serviceinterface="dubbo.web.api.service.UserService"ref="userService" />

    <!-- 和本地bean一样实现服务 -->

    <bean id="userService"class="com.boonya.dubbo.provider.UserServiceProvider"/>

    <!-- 声明需要暴露的服务接口（interface是全路径名，不能自定义） -->

    <dubbo:serviceinterface="dubbo.web.api.service.ProductService"ref="productService" />

     <!-- 和本地bean一样实现服务 -->

    <bean id="productService"class="com.boonya.dubbo.provider.ProductServiceProvider"/>

 

</beans>
```


***Customer applicationContext.xml***



```
<?xmlversion="1.0" encoding="UTF-8"?>

<beansxmlns="http://www.springframework.org/schema/beans"

    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"xmlns:dubbo="http://code.alibabatech.com/schema/dubbo"

    xsi:schemaLocation="http://www.springframework.org/schema/beans

     http://www.springframework.org/schema/beans/spring-beans.xsd       

     http://code.alibabatech.com/schema/dubbo       

     http://code.alibabatech.com/schema/dubbo/dubbo.xsd">


    <!-- 消费方应用信息，用于计算依赖关系 -->
    <dubbo:applicationname="webapp-api-customer" />

 

    <!-- 使用zookeeper注册中心暴露服务地址 -->
    <!-- <dubbo:registryaddress="zookeeper://192.168.200.122:2181" /> -->

    <!-- zookeeper注册中心暴露服务集群配置 -->
    <dubbo:registryaddress="zookeeper://192.168.200.122:2181?backup=192.168.200.122:2182,192.168.200.122:2183"/>

    <!-- 生成远程服务代理，可以和本地bean一样使用userService -->

    <dubbo:referenceid="userService"interface="dubbo.web.api.service.UserService" />

    <dubbo:referenceid="productService"interface="dubbo.web.api.service.ProductService" />


</beans>
```





####  Node2
***   Provider applicationContext.xml***
```
<?xmlversion="1.0" encoding="UTF-8"?>

<beansxmlns="http://www.springframework.org/schema/beans"

    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"xmlns:dubbo="http://code.alibabatech.com/schema/dubbo"

    xsi:schemaLocation="http://www.springframework.org/schema/beans

     http://www.springframework.org/schema/beans/spring-beans.xsd       

     http://code.alibabatech.com/schema/dubbo       

     http://code.alibabatech.com/schema/dubbo/dubbo.xsd">

 

    <!-- 提供方应用信息，用于计算依赖关系 -->

    <dubbo:applicationname="webapp-api-provider" />

 

    <!-- 使用zookeeper注册中心暴露服务地址 -->

    <!--<dubbo:registryaddress="zookeeper://192.168.200.122:2181?" />-->

   

    <!-- zookeeper注册中心暴露服务集群配置 -->

    <dubbo:registryaddress="zookeeper://192.168.200.122:2181?backup=192.168.200.122:2182,192.168.200.122:2183"/>

   

    <!-- 用dubbo协议在20881端口暴露服务（如果有多个提供者，则端口号不能重复） -->

    <dubbo:protocol name="dubbo"port="20882" />

 

    <!-- 声明需要暴露的服务接口（interface是全路径名，不能自定义） -->

    <dubbo:serviceinterface="dubbo.web.api.service.UserService"ref="userService" />

    <!-- 和本地bean一样实现服务 -->

    <bean id="userService"class="com.boonya.dubbo.provider.UserServiceProvider"/>

    <!-- 声明需要暴露的服务接口（interface是全路径名，不能自定义） -->

    <dubbo:serviceinterface="dubbo.web.api.service.ProductService"ref="productService" />

     <!-- 和本地bean一样实现服务 -->

    <bean id="productService"class="com.boonya.dubbo.provider.ProductServiceProvider"/>

 

</beans>
```


***Customer applicationContext.xml***



```
<?xmlversion="1.0" encoding="UTF-8"?>

<beansxmlns="http://www.springframework.org/schema/beans"

    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"xmlns:dubbo="http://code.alibabatech.com/schema/dubbo"

    xsi:schemaLocation="http://www.springframework.org/schema/beans

     http://www.springframework.org/schema/beans/spring-beans.xsd       

     http://code.alibabatech.com/schema/dubbo       

     http://code.alibabatech.com/schema/dubbo/dubbo.xsd">


    <!-- 消费方应用信息，用于计算依赖关系 -->
    <dubbo:applicationname="webapp-api-customer" />

 

    <!-- 使用zookeeper注册中心暴露服务地址 -->
    <!-- <dubbo:registryaddress="zookeeper://192.168.200.122:2181" /> -->

    <!-- zookeeper注册中心暴露服务集群配置 -->
    <dubbo:registryaddress="zookeeper://192.168.200.122:2181?backup=192.168.200.122:2182,192.168.200.122:2183"/>

    <!-- 生成远程服务代理，可以和本地bean一样使用userService -->

    <dubbo:referenceid="userService"interface="dubbo.web.api.service.UserService" />

    <dubbo:referenceid="productService"interface="dubbo.web.api.service.ProductService" />


</beans>
```


####  Node3
***   Provider applicationContext.xml***
```
<?xmlversion="1.0" encoding="UTF-8"?>

<beansxmlns="http://www.springframework.org/schema/beans"

    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"xmlns:dubbo="http://code.alibabatech.com/schema/dubbo"

    xsi:schemaLocation="http://www.springframework.org/schema/beans

     http://www.springframework.org/schema/beans/spring-beans.xsd       

     http://code.alibabatech.com/schema/dubbo       

     http://code.alibabatech.com/schema/dubbo/dubbo.xsd">

 

    <!-- 提供方应用信息，用于计算依赖关系 -->

    <dubbo:applicationname="webapp-api-provider" />

 

    <!-- 使用zookeeper注册中心暴露服务地址 -->

    <!--<dubbo:registryaddress="zookeeper://192.168.200.122:2181?" />-->

   

    <!-- zookeeper注册中心暴露服务集群配置 -->

    <dubbo:registryaddress="zookeeper://192.168.200.122:2181?backup=192.168.200.122:2182,192.168.200.122:2183"/>

   

    <!-- 用dubbo协议在20881端口暴露服务（如果有多个提供者，则端口号不能重复） -->

    <dubbo:protocol name="dubbo"port="20883" />

 

    <!-- 声明需要暴露的服务接口（interface是全路径名，不能自定义） -->

    <dubbo:serviceinterface="dubbo.web.api.service.UserService"ref="userService" />

    <!-- 和本地bean一样实现服务 -->

    <bean id="userService"class="com.boonya.dubbo.provider.UserServiceProvider"/>

    <!-- 声明需要暴露的服务接口（interface是全路径名，不能自定义） -->

    <dubbo:serviceinterface="dubbo.web.api.service.ProductService"ref="productService" />

     <!-- 和本地bean一样实现服务 -->

    <bean id="productService"class="com.boonya.dubbo.provider.ProductServiceProvider"/>

 

</beans>
```


***Customer applicationContext.xml***



```
<?xmlversion="1.0" encoding="UTF-8"?>

<beansxmlns="http://www.springframework.org/schema/beans"

    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"xmlns:dubbo="http://code.alibabatech.com/schema/dubbo"

    xsi:schemaLocation="http://www.springframework.org/schema/beans

     http://www.springframework.org/schema/beans/spring-beans.xsd       

     http://code.alibabatech.com/schema/dubbo       

     http://code.alibabatech.com/schema/dubbo/dubbo.xsd">


    <!-- 消费方应用信息，用于计算依赖关系 -->
    <dubbo:applicationname="webapp-api-customer" />

 

    <!-- 使用zookeeper注册中心暴露服务地址 -->
    <!-- <dubbo:registryaddress="zookeeper://192.168.200.122:2181" /> -->

    <!-- zookeeper注册中心暴露服务集群配置 -->
    <dubbo:registryaddress="zookeeper://192.168.200.122:2181?backup=192.168.200.122:2182,192.168.200.122:2183"/>

    <!-- 生成远程服务代理，可以和本地bean一样使用userService -->

    <dubbo:referenceid="userService"interface="dubbo.web.api.service.UserService" />

    <dubbo:referenceid="productService"interface="dubbo.web.api.service.ProductService" />


</beans>
```


###  测试节点

####  API Services

![](https://img-blog.csdn.net/20170511131158416)

#### Providers

![](https://img-blog.csdn.net/20170511131210197)

####   Customers

注：一旦有接口访问就可以看到消费者，这里的“应用名”可以根据Tomcat对应的端口设置来加以区分。

### Dubbo+Zookeeper架构应用框架

官网地址：[http://minglisoft.cn/technology/index.htm](http://minglisoft.cn/technology/index.htm)


360图书馆介绍：[http://www.360doc.com/content/17/0511/15/36031295_652993734.shtml](http://www.360doc.com/content/17/0511/15/36031295_652993734.shtml)


### 最新示例代码下载
Github：[https://github.com/SunflowersOfJava/dubbo-zookeeper-spring.git](https://github.com/SunflowersOfJava/dubbo-zookeeper-spring.git)





