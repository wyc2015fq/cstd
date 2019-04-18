# WebService CXF学习（进阶篇1）：自定义对象传递（简单JavaBean对象） - z69183787的专栏 - CSDN博客
2014年06月30日 11:22:32[OkidoGreen](https://me.csdn.net/z69183787)阅读数：8274
这一节我们就CXF框架简单的JavaBean对象传递进行讲解。
第一步：创建传输的JavaBean对象(UserInfo)
**[java]**[view
 plain](http://blog.csdn.net/vickychen89/article/details/6606400#)[copy](http://blog.csdn.net/vickychen89/article/details/6606400#)
- package com.ws.model;  
- 
- import javax.xml.bind.annotation.XmlAccessType;  
- import javax.xml.bind.annotation.XmlAccessorType;  
- import javax.xml.bind.annotation.XmlRootElement;  
- import javax.xml.bind.annotation.XmlType;  
- 
- 
- @XmlRootElement(name="UserInfo")  
- @XmlAccessorType(XmlAccessType.FIELD)  
- @XmlType(propOrder={"userName","userAge"})  
- publicclass UserInfo {  
- private String userName;  
- private Integer userAge;  
- 
- public UserInfo(String name,Integer age){  
- this.userAge = age;  
- this.userName = name;  
-     }  
- 
- public UserInfo(){        
-     }  
- 
- // 添加geter/seter方法..
- 
- }  
注解：@XmlRootElement-指定XML根元素名称（可选） 
            @XmlAccessorType-控制属性或方法序列化 ， 四种方案： 
                       FIELD-对每个非静态，非瞬变属性JAXB工具自动绑定成XML，除非注明XmlTransient 
                       NONE-不做任何处理 
                       PROPERTY-对具有set/get方法的属性进行绑定，除非注明XmlTransient 
                       PUBLIC_MEMBER -对有set/get方法的属性或具有共公访问权限的属性进行绑定，除非注 明XmlTransient 
            @XmlType-映射一个类或一个枚举类型成一个XML Schema类型
**第二步：创建webservices服务端接口和实现类 ** 1、创建服务端接口类
**[java]**[view
 plain](http://blog.csdn.net/vickychen89/article/details/6606400#)[copy](http://blog.csdn.net/vickychen89/article/details/6606400#)
- package com.ws.services;  
- 
- import javax.jws.WebService;  
- import com.ws.model.UserInfo;  
- 
- @WebService
- publicinterface IUserServices {  
- public UserInfo getUserInfo(String userName, Integer userAge);  
- }  
   2、创建服务端接口实现类
**[java]**[view
 plain](http://blog.csdn.net/vickychen89/article/details/6606400#)[copy](http://blog.csdn.net/vickychen89/article/details/6606400#)
- package com.ws.services.impl;  
- 
- import javax.jws.WebService;  
- import com.ws.model.UserInfo;  
- import com.ws.services.IUserServices;  
- 
- @WebService
- publicclass UserServicesImpl implements IUserServices {  
- public UserInfo getUserInfo(String userName, Integer userAge) {  
- returnnew UserInfo(userName,userAge);  
-     }  
- }  
3、创建服务端，并发布服务
**[java]**[view
 plain](http://blog.csdn.net/vickychen89/article/details/6606400#)[copy](http://blog.csdn.net/vickychen89/article/details/6606400#)
- package com.test;  
- 
- import javax.xml.ws.Endpoint;  
- import org.apache.cxf.jaxws.JaxWsServerFactoryBean;  
- import com.ws.services.impl.UserServicesImpl;  
- 
- publicclass ServerTest {  
- public ServerTest(){  
- // 发布User服务接口
-         Endpoint.publish("http://localhost:8090/userInfoServices", new UserServicesImpl());  
-     }  
- publicstaticvoid main(String[] args) {  
- // 启动服务
- new ServerTest();  
-         System.out.println("Server ready...");     
- try {  
-             Thread.sleep(1000*300);//休眠五分分钟，便于测试   
-         } catch (InterruptedException e) {  
-             e.printStackTrace();  
-         }     
-         System.out.println("Server exit...");     
-         System.exit(0);  
-     }  
- }  
**第三步：创建webservices客户端，并进行测试(这里只例举在客户端工程中的测试方法) **    1、将服务端的JavaBean和Services接口类，copy到客户端工程中，且目录要一致
    2、在客户端工程中，新建一个测试类测试
**[java]**[view
 plain](http://blog.csdn.net/vickychen89/article/details/6606400#)[copy](http://blog.csdn.net/vickychen89/article/details/6606400#)
- package com.ws.client;  
- 
- import org.apache.cxf.jaxws.JaxWsProxyFactoryBean;  
- import com.ws.model.UserInfo;  
- import com.ws.server.IUserServices;  
- 
- publicclass UserTest {  
- publicstaticvoid main(String[] args) {  
- //创建WebService客户端代理工厂   
-         JaxWsProxyFactoryBean factory = new JaxWsProxyFactoryBean();     
- //注册WebService接口   
-         factory.setServiceClass(IUserServices.class);     
- //设置WebService地址   
-         factory.setAddress("http://localhost:8090/userInfoServices");          
-         IUserServices userServices = (IUserServices)factory.create();     
-         System.out.println("invoke userinfo webservice...");  
- // 测试返回JavaBean对象的
-         UserInfo user = userServices.getUserInfo("vicky", 23);  
-         System.out.println("UserName: "+user.getUserName());  
-         System.out.println("UserAge : "+user.getUserAge());  
- 
-         System.exit(0);     
-     }   
- }  
**第四步：运行webServices服务，在IE中输入[http://localhost:8090/userInfoServices?wsdl](http://localhost:8090/userInfoServices?wsdl)，验证服务是否成功发布第四步：运行客户端，验证。**
