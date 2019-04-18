# WebService与XFire - z69183787的专栏 - CSDN博客
2014年02月21日 20:10:03[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1110
**1、WebService与XFire简介**
  WebService即分布式开发，是将一个完整的项目拆分为多个项目，分别完成前台部分（VC）和后台部分（M），各个项目之间是单独存在的，可以单独部署。 
     理论上分布式开发可以提高开发效率，但会影响访问的性能。这种分布式开发一般在政府项目或多公司合作开发时使用。
     Java最早的分布式的开发是SUN提出的RMI（远程方法调用），在这个的基础上，结合ORMapping的概念，出现了EJB1/2。
     WebService概念实际上是微软提出的，也是完成分布式开发的功能，但交互数据使用w3c提供的标准XML完成。这样可以进行跨语言的交互。在这之后，Java也出现了支持WebService的语法JAXB，但语法非常复杂，基本上不用，在这个基础上出现了两个框架（XFire和AXIS），但AXIS没有工具支持，需要手工对发布的远程接口进行编译以及修改。在XFire的1.2.6版本后，XFire被Apache收购，更名为CXF，但更新版本后，只能在Spring2.5环境下使用，如果不使用Spring，该框架无法单独使用。
      在这里XML的语法格式是由w3c制订好的，语法为WSDL，之间的传输协议为SOAP。 
这种项目一般在以下情况下使用： 
政府项目，特别是电信项目和银行项目 
多公司合作的项目 
子公司项目
**2、XFire+Hibernate完成登陆功能 **
     这里先不使用Spring框架，先单独使用Hibernate完成后台项目，Struts2完成前台项目 
建立WebService项目 
![](http://dl.iteye.com/upload/attachment/446868/9ac0f26f-5984-3984-b35f-d2217b412c5e.jpg)
![](http://dl.iteye.com/upload/attachment/446887/df391f84-9a57-3de4-b9a4-5599f80c42ec.jpg)
![](http://dl.iteye.com/upload/attachment/446890/cfdedcbc-dea7-3b91-bcab-ec7e34604055.jpg)
![](http://dl.iteye.com/upload/attachment/446892/5b936f8d-2ed6-36b0-99c1-dda93f431fc7.jpg)
在web.xml中会自动加入以下内容：
<servlet> 
        <servlet-name>XFireServlet</servlet-name> 
        <servlet-class>org.codehaus.xfire.transport.http.XFireConfigurableServlet</servlet-class> 
          <load-on-startup>0</load-on-startup> 
</servlet> 
<servlet-mapping> 
      <servlet-name>XFireServlet</servlet-name> 
       <url-pattern>/services/*</url-pattern> 
</servlet-mapping>
    加入Hibernate支持 
在编写好服务接口和实现类后。
![](http://dl.iteye.com/upload/attachment/446896/7cd0dccd-35e1-3045-b218-3c8061c07514.jpg)
需要建立一个WebService后台的服务 
![](http://dl.iteye.com/upload/attachment/446901/ccb0f007-9c37-3a17-b37a-2ea7a7524047.jpg)
![](http://dl.iteye.com/upload/attachment/446903/1ec8a978-8782-36df-83b0-c6c2a2b2587e.jpg)
![](http://dl.iteye.com/upload/attachment/446905/7b22f4bb-9fcf-3645-9a18-cfcda56ad408.jpg)
这时会在项目的根目录下增加一下WebServices的目录，和一个services.xml文件：
<?xml version="1.0" encoding="UTF-8"?> 
<beans xmlns="http://xfire.codehaus.org/config/1.0"> 
     <service> 
          <name>UserService</name> 
           <serviceClass>org.wu.xfire.service.UserService</serviceClass> 
           <implementationClass> 
                     org.wu.xfire.service.impl.UserServiceImpl 
           </implementationClass> 
     <style>rpc</style> 
     <use>literal</use> 
     <scope>application</scope> 
</service> 
</beans>
 后台项目完成后，如果部署可以查看WSDL的信息:则表示后台项目完成。 
http://localhost/WebServiceBack/services/UserService?wsdl，
![](http://dl.iteye.com/upload/attachment/446914/74e06254-9981-3766-a287-f0943f881c6d.jpg)
 在WebServiceBack中编写测试类进行测试：
测试成功后可以编写前台项目调用后台项目发布的服务接口UserService！
package org.wu.xfire.test; 
import java.net.MalformedURLException; 
import org.codehaus.xfire.XFireFactory; 
import org.codehaus.xfire.client.XFireProxyFactory; 
import org.codehaus.xfire.service.Service; 
import org.codehaus.xfire.service.binding.ObjectServiceFactory; 
import org.wu.xfire.dao.HelloDAO; 
import org.wu.xfire.pojo.User; 
import org.wu.xfire.service.UserService; 
public class Test { 
public static void main(String[] args) { 
              Service service = new ObjectServiceFactory().create(UserService.class); 
              Service helloservice = new ObjectServiceFactory().create(HelloDAO.class); 
              XFireProxyFactory factory = new XFireProxyFactory(XFireFactory .newInstance().getXFire());
               String url = "http://localhost/WebServiceBack/services/UserService"; 
              try { 
                       UserService userdao = (UserService) factory.create(service , url); 
                       User user = new User(); 
                         user.setUsername("Wuzhulin"); 
                          user.setUserid("Hello"); 
                          user.setPassword("12345"); 
                         System.out.println(userdao.isLogin(user)); 
                         System.out.println(hellodao.sayHello("张三")); 
                    } catch (MalformedURLException e) { 
                              e.printStackTrace(); 
                    } catch (Exception e) { 
                         e.printStackTrace(); 
                    } 
                 }
          }
建立前台项目WebServiceFront,加入Struts支持.方法同上,删除web.xml中的XFire相关内容。 
需要建立一个WebService前台的服务（Web Service Client）
![](http://dl.iteye.com/upload/attachment/446917/245ce427-282b-3962-9e24-6fb3a4118034.jpg)
![](http://dl.iteye.com/upload/attachment/446919/2529bf04-77dd-31fd-85df-4fabca1b1c7e.jpg)
![](http://dl.iteye.com/upload/attachment/446921/1ba603c7-dbd3-31b3-adc6-ee1e50c6de4b.jpg)
![](http://dl.iteye.com/upload/attachment/446924/060d39dd-51df-3e79-8258-19a409bd1a47.jpg)
生成的pojo对象属性类型为JAXBElement，使用比较麻烦，最好重新加入一些setter和getter方法，方便使用，注意getter方法必须改名。User.java
package org.wu.xfire.pojo; 
import javax.xml.bind.JAXBElement; 
import javax.xml.bind.annotation.XmlAccessType; 
import javax.xml.bind.annotation.XmlAccessorType; 
import javax.xml.bind.annotation.XmlElementRef; 
import javax.xml.bind.annotation.XmlType; 
@XmlAccessorType(XmlAccessType.FIELD) 
@XmlType(name = "User", propOrder = { 
    "password", 
    "userid", 
    "username" 
}) 
public class User { 
private static ObjectFactory factory = new ObjectFactory(); 
    @XmlElementRef(name = "password", namespace = "http://pojo.xfire.wu.org", type = JAXBElement.class) 
    protected JAXBElement<String> password; 
    @XmlElementRef(name = "userid", namespace = "http://pojo.xfire.wu.org", type = JAXBElement.class) 
    protected JAXBElement<String> userid; 
    @XmlElementRef(name = "username", namespace = "http://pojo.xfire.wu.org", type = JAXBElement.class) 
    protected JAXBElement<String> username; 
    public JAXBElement<String> getPassword() { 
        return password; 
    } 
    public void setPassword(JAXBElement<String> value) { 
        this.password = ((JAXBElement<String> ) value); 
    } 
    public JAXBElement<String> getUserid() { 
        return userid; 
    } 
    public void setUserid(JAXBElement<String> value) { 
        this.userid = ((JAXBElement<String> ) value); 
    } 
    public JAXBElement<String> getUsername() { 
        return username; 
    } 
    public void setUsername(JAXBElement<String> value) { 
        this.username = ((JAXBElement<String> ) value); 
    } 
    public String getPasswordValue() { 
return password.getValue(); 
} 
public void setPassword(String value) { 
this.password = factory.createUserPassword(value); 
} 
public String getUseridValue() { 
return userid.getValue(); 
} 
public void setUserid(String value) { 
this.userid = factory.createUserUserid(value); 
} 
public String getUsernameValue() { 
return username.getValue(); 
} 
public void setUsername(String value) { 
this.username = factory.createUserUsername(value); 
} 
}
如果服务器端的style选择的是rpc，可以直接在前台项目中自己定义接口和pojo对象，通过XFire提供的方法直接使用。
 <service>
        <name>UserService</name>
         <serviceClass>org.wu.xfire.service.UserService</serviceClass>
         <implementationClass>
               org.wu.xfire.service.impl.UserServiceImpl
         </implementationClass>
       <style>rpc</style>
       <use>literal</use>
       <scope>application</scope>
 </service>
在WebServiceFront项目中编写测试类进行测试：
package org.wu.xfire.test; 
import org.wu.xfire.hello.HelloDAOClient; 
import org.wu.xfire.hello.HelloDAOPortType; 
import org.wu.xfire.pojo.User; 
import org.wu.xfire.user.UserServiceClient; 
import org.wu.xfire.user.UserServicePortType; 
public class Test { 
public static void main(String[] args) { 
            UserServicePortType userdao = new UserServiceClient().getUserServiceHttpPort(); 
             System.out.println(userdao.getClass().getName()); 
             User user = new User(); 
              user.setUsername("Wuzhulin"); 
              user.setUserid("hello"); 
              user.setPassword("12345"); 
              System.out.println(userdao.isLogin(user)); 
       } 
}
注意，这里不支持引用传递，因此如果要前后台沟通数据，必须通过参数和返回值的形式来传递。 
而且WebService对Hibernate关系映射的支持也很差。只支持单方向的一对多或多对多关系，而且Set集合必须加入泛型。 
分页代码也不能直接返回Map了，因为WebService不支持Object类型。需要单独定义一个Page对象。 
3、XFire + Spring 开发 
Spring在开发中主要还是结合Hibernate开发。 
在WebService的开发中，主要还是使用Spring完成后台项目，用来进行服务的发布功能。
![](http://dl.iteye.com/upload/attachment/446927/3bffbb16-45cb-3789-b9ae-2582ee06fcb9.jpg)
选择XFireSpringServlet来完趁服务的发布。 
加入 Spring与Hibernate支持，并在web.xml中配置Spring结合其他框架的监听器
<context-param> 
           <param-name>contextConfigLocation</param-name> 
           <param-value>/WEB-INF/classes/applicationContext-*.xml</param-value> 
  </context-param> 
<listener>
            <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class> 
</listener>
正常编写好接口与实现类，在spring中，可以直接通过bean发布服务。
<!-- 需要先导入一个发布服务的配置文件 --> 
<import resource="classpath:org/codehaus/xfire/spring/xfire.xml"></import> 
<bean name="userService" class="org.codehaus.xfire.spring.ServiceBean"> 
          <property name="name" value="UserService"></property> 
          <property name="serviceClass" value="org.wu.xfire.service.UserService"></property> 
          <property name="serviceBean"> 
             <ref bean="userserviceimpl"/> 
          </property> 
          <property name="style" value="rpc"></property> 
          <property name="use" value="literal"></property> 
          <property name="scope" value="application"></property> 
</bean>
