# WebService CXF学习（入门篇2）：HelloWorld示例 - z69183787的专栏 - CSDN博客
2014年06月30日 11:22:12[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1917
 理论联系实际，单单只讲理论那就成了纸上谈兵，用一个HelloWorld Demo可来说明事更加直观。那下面咱们就开始进行讲解： 
   首先到apache官方网下载apache-cxf-2.2.2，(现在有更高版本2.4)地址：[http://cxf.apache.org/](http://cxf.apache.org/)
   新建一个Java Project,导入cxf常用.jar包，cxf常用jar包如下：
> 
1、commons-logging-1.1.1.jar
2、cxf-2.4.1.jar
3、geronimo-activation_1.1_spec-1.1.jar
4、geronimo-annotation_1.0_spec-1.1.1.jar
5、geronimo-javamail_1.4_spec-1.7.1.jar
6、geronimo-jaxws_2.2_spec-1.0.jar
7、geronimo-servlet_3.0_spec-1.0.jar
8、geronimo-stax-api_1.0_spec-1.0.1.jar
9、geronimo-ws-metadata_2.0_spec-1.1.3.jar
10、jettison-1.3.jar
11、jetty-continuation-7.4.2.v20110526.jar
12、jetty-http-7.4.2.v20110526.jar
13、jetty-io-7.4.2.v20110526.jar
14、jetty-server-7.4.2.v20110526.jar
15、jetty-util-7.4.2.v20110526.jar
16、neethi-3.0.0.jar
17、saaj-api-1.3.jar
18、saaj-impl-1.3.2.jar
19、serializer-2.7.1.jar
cxf结合spring时所需jar包
(
    spring-asm-3.0.5.RELEASE.jar
    spring-beans-3.0.5.RELEASE.jar
    spring-context-3.0.5.RELEASE.jar
    spring-core-3.0.5.RELEASE.jar
    spring-expression-3.0.5.RELEASE.jar
    spring-aop-3.0.5.RELEASE.jar
    spring-web-3.0.5.RELEASE.jar
)
20、wsdl4j-1.6.2.jar
21、xalan-2.7.1.jar
22、xercesImpl.jar
23、xml-resolver-1.2.jar
24、xmlschema-core-2.0.jar
25、jaxb-api-2.2.1.jar  ---- webservices服务端需要加
26、jaxb-impl-2.2.1.1.jar ---- 如果jdk中的版本与该版本一致，则webservices服务端和客户端都不需要加
(注:jaxb-api和jaxb-impl包会和jdk中的冲突(jdk的版本<2.0>较低)，在用这两个jar包的时候，可将2.2版本的jar覆盖jdk的版本)
覆盖jdk版本的方法: 找到jdk的安装目录下的jre\lib\endorsed文件夹(如果endorsed文件夹不存在，可新建),将jaxb-api-2.2.1和jaxb-impl-2.2.1.1放到此文件夹下即可。
**第一步：新建一个webservice服务端接口和实现类**
 1、服务端接口
**[java]**[view
 plain](http://blog.csdn.net/vickychen89/article/details/6606273#)[copy](http://blog.csdn.net/vickychen89/article/details/6606273#)
- package com.ws.services;  
- import javax.jws.WebService;  
- 
- @WebService
- publicinterface IHelloServices {  
- public String sayHelloToAll(String[] userNames);  
- 
- public String[] getHelloWords();  
- 
- public String sayHello(String name);  
- }  
  2、服务端接口实现类
**[java]**[view
 plain](http://blog.csdn.net/vickychen89/article/details/6606273#)[copy](http://blog.csdn.net/vickychen89/article/details/6606273#)
- package com.ws.services.impl;  
- 
- import javax.jws.WebService;  
- 
- import com.ws.services.IHelloServices;  
- 
- @WebService(endpointInterface="com.ws.services.IHelloServices")  
- publicclass HelloServicesImpl implements IHelloServices {  
- 
- public String[] getHelloWords() {  
-         String[] words = {"hello vicky.","hello,i'm vicky.","hi,ivy and simon."};  
- return words;  
-     }  
- 
- public String sayHello(String name) {  
- return"hello "+name+" ! ";  
-     }  
- 
- public String sayHelloToAll(String[] userNames) {  
-         String hello = "hello ";  
- for(int i=0;i<userNames.length;i++){  
- if(i!=userNames.length-1)  
-                 hello += userNames[i]+" and ";  
- else
-                 hello += userNames[i]+" .";  
-         }  
- return hello;  
-     }  
- 
- }  
 3、创建webservices服务端，并发布服务
**[java]**[view
 plain](http://blog.csdn.net/vickychen89/article/details/6606273#)[copy](http://blog.csdn.net/vickychen89/article/details/6606273#)
- package com.test;  
- 
- import javax.xml.ws.Endpoint;  
- import org.apache.cxf.jaxws.JaxWsServerFactoryBean;  
- import com.ws.services.IHelloServices;  
- import com.ws.services.impl.HelloServicesImpl;  
- import com.ws.services.impl.UserServicesImpl;  
- 
- publicclass ServerTest {  
- public ServerTest(){  
- // 第一种发布方式
-         IHelloServices hello = new HelloServicesImpl();  
- // 创建WebServices服务接口
-         JaxWsServerFactoryBean factory = new JaxWsServerFactoryBean();  
- // 注册webservices接口
-         factory.setServiceClass(IHelloServices.class);  
- // 发布接口
-         factory.setAddress("http://localhost:8090/helloServices");  
-         factory.setServiceBean(hello);  
- // 创建服务
-         factory.create();  
- 
- // 第二种发布方式
- //Endpoint.publish("http://localhost:8090/helloServices", new HelloServicesImpl());
-     }  
- publicstaticvoid main(String[] args) {  
- // 启动服务
- new ServerTest();  
-         System.out.println("Server ready...");     
- try {  
-             Thread.sleep(1000*300); //休眠五分分钟，便于测试   
-         } catch (InterruptedException e) {  
-             e.printStackTrace();  
-         }     
-         System.out.println("Server exit...");     
-         System.exit(0);  
-     }  
- }  
**第二步：新建一个webservice客户端，并测试webServices的服务**
    1、在本工程中测试(即服务端与客户端在同一个工程中)
**[java]**[view
 plain](http://blog.csdn.net/vickychen89/article/details/6606273#)[copy](http://blog.csdn.net/vickychen89/article/details/6606273#)
- package com.ws.client;  
- 
- import org.apache.cxf.jaxws.JaxWsProxyFactoryBean;  
- import com.ws.server.IHelloServices;  
- 
- publicclass HelloTest {  
- publicstaticvoid main(String[] args) {  
- //创建WebService客户端代理工厂   
-         JaxWsProxyFactoryBean factory = new JaxWsProxyFactoryBean();     
- //注册WebService接口   
-         factory.setServiceClass(IHelloServices.class);     
- //设置WebService地址   
-         factory.setAddress("http://localhost:8090/helloServices");          
-         IHelloServices iHelloWorld = (IHelloServices)factory.create();     
-         System.out.println("invoke webservice...");     
-         String[] hellos = iHelloWorld.getHelloWords();  
- for(int i=0;i<hellos.length;i++){  
-             System.out.println(hellos[i]);  
-         }  
-         System.exit(0);     
-     }    
- }  
     2、新建一个webservices客户端测试工程
     (1)、新建一个Project，并加上cxf的jar包；
     (2)、将Webservices服务端工程中的接口类Copy到客户端工程中，且路径要一直；
     (3)、新建一个测试类，代码如上。
最后是万事俱备，只欠测试了 
    首先，运行服务端程序 
    其次，打开浏览器，在地址栏中输入http://localhost:8090/helloServices?wsdl（因为cxf自带了一个jetty服务器），查看接口是否发布成功,如里浏览器页面显示下面内容，证明接口发布成功 。
    最后，运行客户程序。
