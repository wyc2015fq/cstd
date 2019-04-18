# MyEclipse创建WebService - Orisun - 博客园







# [MyEclipse创建WebService](https://www.cnblogs.com/zhangchaoyang/articles/1954846.html)





使用Eclipse的话还要装web tool platform很多东西，用MyEclipse一步到位，创建WebService很方便。

MyEclipse中有自己的Tomcat，要把事先在电脑上独立安装的Tomcat关掉。

1.File->New->Web Service Project

![](https://pic002.cnblogs.com/images/2011/103496/2011021509192674.png)

Project Name取为webservice，Framework使用XFire

下一步使用默认配置即可。

![](https://pic002.cnblogs.com/images/2011/103496/2011021509211633.png)

勾上HTTP Client Library

![](https://pic002.cnblogs.com/images/2011/103496/2011021509215122.png)

选择Yse即可。MyEclipse8.6使用的JDK最高版本是5，而我本机上已经装到6了

2.在src下添加一个包org.demo.webservice，再添加一个接口IAdd，和一个类Add

package org.demo.webservice;

public interface IAdd {

	public abstract int add(int num1,int num2);
}


package org.demo.webservice;

public class Add implements IAdd{

	public int add(int num1,int num2){
		return num1+num2;
	}
}


3.配置webservice包下面的services.xml。

<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://xfire.codehaus.org/config/1.0">
  <service>
    <name>add</name><!-- 服务名称 --> 
    <namespace>org.demo.webservice/add</namespace><!-- 指定命名空间 --> 
    <serviceClass>org.demo.webservice.IAdd</serviceClass><!-- 指定服务的接口 --> 
    <implementationClass>org.demo.webservice.Add</implementationClass><!-- 指定接口的实现类 --> 
  </service>
</beans>



在WebRoot/WEB-INF下有一个web.xml，已经自动加好了XFireServlet

<?xml version="1.0" encoding="UTF-8"?>
<web-app xmlns="http://java.sun.com/xml/ns/javaee" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" version="2.5" xsi:schemaLocation="http://java.sun.com/xml/ns/javaee   http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd">
  <servlet>
    <servlet-name>XFireServlet</servlet-name>
    <servlet-class>org.codehaus.xfire.transport.http.XFireConfigurableServlet</servlet-class>
    <load-on-startup>0</load-on-startup>
  </servlet>
  <servlet-mapping>
    <servlet-name>XFireServlet</servlet-name>
    <url-pattern>/services/*</url-pattern>
  </servlet-mapping>
  <welcome-file-list>
    <welcome-file>index.jsp</welcome-file>
  </welcome-file-list>
</web-app>


4.在MyEclipse中运行程序Run As MyEclipse Server Application，地址输入：应用程序地址+“/services/"+服务名称+”？wsdl“

![](https://pic002.cnblogs.com/images/2011/103496/2011021509582361.png)

出现上图说明WebService发布成功。

当然在MyEclipse中可以很方便地创建客户端来调用WebService，不过接下来我们要在**Eclipse中使用XFire来****创建客户端调用WebService**

1**.**为Eclipse安装XFire插件

从Help->Install New SoftWare进入，地址输入http://xfire.codehaus.org/Eclipse+Plugin

2.新建Java Project名曰xfire

3.右键项目名New->other

![](https://pic002.cnblogs.com/images/2011/103496/2011021510123420.png)

![](https://pic002.cnblogs.com/images/2011/103496/2011021510125022.png)

Package的名称要小写。output directory和package的含义看生成的项目结构便知：

![](https://pic002.cnblogs.com/images/2011/103496/2011021510132212.png)

把service和webservice拖到src下，形成

![](https://pic002.cnblogs.com/images/2011/103496/2011021510151474.png)

4.右键项目属性，勾上这四项

![](https://pic002.cnblogs.com/images/2011/103496/2011021513273372.png)

5.src下创建包Invoke，包下创建类Tyr

package Invoke;

import service.*;

public class Tyr {
	public static void main(String[] args) {
		addClient client=new addClient();
		addPortType service=client.getaddHttpPort();
		System.out.println(service.add(4,8));
	}

}


6.运行

![](https://pic002.cnblogs.com/images/2011/103496/2011021513295367.png)

选择Tyr类，Console中可看到结果为12












