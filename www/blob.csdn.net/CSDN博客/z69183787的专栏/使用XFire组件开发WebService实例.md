# 使用XFire组件开发WebService实例 - z69183787的专栏 - CSDN博客
2013年12月25日 14:22:17[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4405
1、WebService简介
　　WebService又是一种高级应用，与之前学习的Struts、Spring、Hibernate等框架不同。WebService是面向服务的架构（SOA）。那么它到底是做什么用的？什么才是面向服务的架构？
　　让我们来看一种需求，集团公司可能具有多种WEB应用。比如，前年开发了个进销存系统、去年开发了一个ERP、今年又开发了一个OA。现在这家集团公司需要将这三个系统整合，难道需要重新编码将它们整合吗？而这三个系统又是用不同语言编写的，这种成本对公司来说无疑是一种浪费。WebService可以很好的解决这种需求。
WebService是可以进行跨语言、跨平台、分布式系统间整合的方案，WebService像是一条线将这些系统穿起来——企业服务总线（ESB）。WebService使用简单对象访问协议（SOAP）使用http协议传输xml数据（xml是最常用的，也有其他格式数据。）来完成系带间的整合。
　　什么是整合？当然是功能和数据的整合，也就是一个系统可以调用另一个系统的WebService接口来完成数据的交互。这样我们就需要知道，提供WebService服务功能的应用公开了哪些接口，我们可以通过WebService描述文档（WSDL）得知。WSDL不需要我们手动编写，Java的WebService实现可以为我们自动生成。JDK1.6新增支持WebService，但还不够成熟。所以我们可以使用一些第三方开源组织提供的WebService组件。
2、Xfire组件
　　1.XFire简介
　　XFire 是与Axis 2并列的新一代Web Service框架，通过提供简单的API支持Web Service各项标准协议，帮助你方便快速地开发Web Service应用。
　　相对于Axis来说，目前XFire相对受欢迎，加上其提供了和Spring集成的支持，在目前的Web Service开源社区拥有众多的追随者。并且因为XFire为Spring提供的支持，使得我们可以很容易在Spring中使用XFire构建Web Service应用。
　　XFire 与Axis2相比具有如下特征：
- 支持一系列Web Service的新标准--JSR181、WSDL2.0 、JAXB2、WS-Security等；
- 使用Stax解释XML,性能有了质的提高。XFire采用Woodstox 作Stax实现；
- 容易上手，可以方便快速地从pojo发布服务；
- 灵活的Binding机制，包括默认的Aegis,xmlbeans,jaxb2,castor；
- 高性能的SOAP 栈设计；
- 支持Spring、Pico、Plexus、Loom等容器。
　　XFire 与Axis1性能的比较如下：
- XFire 比Axis1.3快2-6倍；
- XFire 的响应时间是Axis1.3的1/2到1/5。
　　XFire 在WebService框架中开始较晚，它从现有的框架中借鉴了许多优秀的理念，力争将Web Service的应用开发难度降到最低。此外，还提供了各种绑定技术、支持多种传输协议，对WebService体系中许多新的规范提供了支持。
　　2.XFire组件下载
　　XFire在2007年后已停止更新。正式更名为Apache CXF,亦可以说是XFire2.0。
　　XFire - XFire安装包：XFire框架目前最后的一个版本是1.2.6，在2007年5月3日发布，可以访问xfire.codehaus.org下载XFire框架的安装包。
　　解压缩后形成的文件目录结构如下：
- api目录：api目录中是XFire框架中所有类（class）对应的API文档，为开发者使用XFire完成应用开发提供帮助。
- examples目录：examples目录中包含了所有随XFire二进制包发布的实例，包括这些实例的源代码和相关Web应用配置内容。
- lib目录：lib目录中包含XFire运行所需要的外部支持类包（.jar文件），可以根据不同项目所需的XFire特性选择所需要的支持类包。保守的方法是在Web项目中包含所有的外部支持类包（.jar文件）。
- manual目录：manual目录中包含有XFire框架的帮助文档，开发者可以从这些帮助文档中学习更多运用XFire框架实现SOA的知识和技巧。
- modules目录：modules目录中包含了XFire框架根据不同特性分别编译的二进制包文件。发布基于XFire框架的Web项目时，可以选择使用该目录下的所有.jar文件，也可以选择XFire-all-1.2.6.jar文件。
- XFire-all-1.2.6.jar：XFire框架的二进制包文件，包含了全部的模块（modules）。
- LICENSE.txt：LICENSE.txt文件中包含了XFire框架的授权协议。
- NOTICE.txt/README.txt：这两个文件中包含了XFire发布时的一些有用的信息。
3、MyEclipse使用XFire组件开发WebService实例
服务端（为客户端提供WebService接口）开发：
　　1.创建工程，导入XFire组件所需jar包到lib下：xfire-all-1.2.6.jar和lib目录所有jar包（根据实际需要确定）。
　　2.创建完成后，打开web.xml，XFire的配置如下：
```
<?xml version="1.0" encoding="UTF-8"?>
<web-app xmlns="http://java.sun.com/xml/ns/javaee" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    version="2.5" xsi:schemaLocation="http://java.sun.com/xml/ns/javaee   http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd">
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
```
　　3.创建Java Code和services.xml。
　　服务端接口和默认实现：
```
public interface HelloWebService {
    public String example(String message);
}
```
```
public class HelloWebServiceImpl implements HelloWebService {
    public String example(String message) {
        return "这是服务端返回:" + message;
    }
}
```
　　创建对应的services.xml配置文件，xfire默认的加载services.xml路径是classes\META-INF。代码如下：
```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://xfire.codehaus.org/config/1.0">
    <service>
        <name>HelloWebService</name>
        <serviceClass>com.server.HelloWebService</serviceClass>
        <implementationClass>com.server.HelloWebServiceImpl</implementationClass>
    </service>
</beans>
```
　　至此，服务端代码生成完毕！工程结构如图：
![](http://pic002.cnblogs.com/images/2012/471047/2012120611473595.jpg)
　　部署并测试服务端：
- 将服务端部署到Tomcat上，并启动Tomcat。
- 选择Toolbars上的Launch SOAP Web Service Explorer,Web Services Explorer窗口右侧WSDL Page，输入网址：http://localhost:8080/WebServiceServer/services/HelloWebService?wsdl，点Go，若成功则Status会提示。
- 双击examlpe,输入hello,下面会显示out(string):hello,测试通过。
　　开发客户端（调用服务端提供的WebService接口方法）：
　　经过以上几步，我们已经成功发布了一个WebService，现在需要创建一个客户端来调用该服务
- 创建Java Project：WebServiceClient
- 引入XFire组件所需jar包：commons-codec-1.2.jar、commons-httpclient-3.0.jar、jdom.jar、xfire-all-1.2.6.jar、wsdl4j-1.5.1.jar、commons-logging-1.0.4.jar。
- 创建Java Code客户端，调用WebService：
```
import java.net.URL;
import org.codehaus.xfire.client.Client;
public class HelloWSClient {
    public static void main(String[] args) throws Exception{
        Client client = new Client(new URL("http://localhost:8080/WebServiceServer/services/HelloWebService?wsdl"));
        Object[] result = client.invoke("example", new Object[]{"Hello World!"});
        System.out.println(result[0]);
    }
}
```
　　或将接口HelloWebService.java复制到工程中来，调用接口的方法。不止可以返回字符串，也可以是集合数组等。
```
import java.util.List;
import org.codehaus.xfire.client.Client;
import org.codehaus.xfire.client.XFireProxyFactory;
import org.codehaus.xfire.service.Service;
import org.codehaus.xfire.service.binding.ObjectServiceFactory;
import org.codehaus.xfire.transport.http.CommonsHttpMessageSender;
public class HelloWSClient {
    public static void main(String[] args) throws Exception{
        String serviceURL = "http://localhost:8080/WebServiceServer/services/HelloWebService";
        Service serviceModel = new ObjectServiceFactory().create(HelloWebService.class,null,"http://localhost:8080/WebServiceServer/services/HelloWebService?wsdl",null);
        
        XFireProxyFactory serviceFactory = new XFireProxyFactory();
        
        HelloWebService service = (HelloWebService) serviceFactory.create(serviceModel, serviceURL);
        Client client = Client.getInstance(service);
        // disable timeout
        client.setProperty(CommonsHttpMessageSender.HTTP_TIMEOUT, "0");
        
        List<String> list = service.getList();
        for (String string : list) {
            System.out.println(string);
        }
    }
}
```
　　客户端工程结构如图：
![](http://pic002.cnblogs.com/images/2012/471047/2012120611535832.jpg)
　　运行客户端代码，输出：这是服务端返回:Hello World!
　　调用WebService成功！
