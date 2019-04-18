# Hessian - z69183787的专栏 - CSDN博客
2015年05月22日 15:14:38[OkidoGreen](https://me.csdn.net/z69183787)阅读数：951
    公司不同pool之间提供service都是通过hessian来开放接口的，个人觉得hessian的使用非常简单。做个demo如下：
0.首先创建web工程，并加载hessian.jar。
1.创建service，即我们通过它来提供接口：
**[java]**[view
 plain](http://blog.csdn.net/loujinhe/article/details/8479238#)[copy](http://blog.csdn.net/loujinhe/article/details/8479238#)
- publicinterface BasicService {  
- 
- publicvoid setServiceName(String serverName);  
- 
- public String getServiceName();  
- 
- public User createUser();  
- 
- }  
2.创建service实现：
**[java]**[view
 plain](http://blog.csdn.net/loujinhe/article/details/8479238#)[copy](http://blog.csdn.net/loujinhe/article/details/8479238#)
- publicclass BasicServiceImpl implements BasicService {  
- 
- private String serviceName;  
- 
- @Override
- publicvoid setServiceName(String serverName) {  
- this.serviceName = serverName;  
-     }  
- 
- @Override
- public String getServiceName() {  
- returnthis.serviceName;  
-     }  
- 
- @Override
- public User createUser() {  
- returnnew User("zhangsan", "123456");  
-     }  
- }  
3.创建需要通过hessian传递的对象（必须序列化）：
**[java]**[view
 plain](http://blog.csdn.net/loujinhe/article/details/8479238#)[copy](http://blog.csdn.net/loujinhe/article/details/8479238#)
- publicclass User implements Serializable {  
- 
- privatestaticfinallong serialVersionUID = 5792818254468116836L;  
- 
- private String username;  
- 
- private String password;  
- 
- public User(String username, String password) {  
- this.username = username;  
- this.password = password;  
-     }  
- 
- public String getUsername() {  
- return username;  
-     }  
- 
- publicvoid setUsername(String username) {  
- this.username = username;  
-     }  
- 
- public String getPassword() {  
- return password;  
-     }  
- 
- publicvoid setPassword(String password) {  
- this.password = password;  
-     }  
- }  
4.配置web.xml:
**[html]**[view
 plain](http://blog.csdn.net/loujinhe/article/details/8479238#)[copy](http://blog.csdn.net/loujinhe/article/details/8479238#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <web-appxmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"xmlns="http://java.sun.com/xml/ns/javaee"xmlns:web="http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd"xsi:schemaLocation="http://java.sun.com/xml/ns/javaee http://java.sun.com/xml/ns/javaee/web-app_3_0.xsd"id="WebApp_ID"version="3.0">
- <display-name>HessianServer</display-name>
- 
- <servlet>
- <servlet-name>basic</servlet-name>
- <servlet-class>com.caucho.hessian.server.HessianServlet</servlet-class>
- <init-param>
- <param-name>service-class</param-name>
- <param-value>com.loujinhe.service.impl.BasicServiceImpl</param-value>
- </init-param>
- </servlet>
- 
- <servlet-mapping>
- <servlet-name>basic</servlet-name>
- <url-pattern>/basic</url-pattern>
- </servlet-mapping>
- 
- </web-app>
5.创建测试用例：
**[java]**[view
 plain](http://blog.csdn.net/loujinhe/article/details/8479238#)[copy](http://blog.csdn.net/loujinhe/article/details/8479238#)
- import java.net.MalformedURLException;  
- 
- import org.junit.Before;  
- import org.junit.Test;  
- 
- import com.caucho.hessian.client.HessianProxyFactory;  
- import com.loujinhe.service.BasicService;  
- 
- 
- publicclass BasicServiceTest {  
- 
- private BasicService basicService;  
- 
- @Before
- publicvoid init() throws MalformedURLException {  
-         HessianProxyFactory factory = new HessianProxyFactory();  
-         String url = "http://localhost:8080/HessianServer/basic";  
-         basicService = (BasicService)factory.create(BasicService.class, url);  
-     }  
- 
- @Test
- publicvoid testBasic() {  
-         basicService.setServiceName("BasicService");  
-         System.out.println(basicService.getServiceName());  
- 
-         System.out.println(basicService.createUser().getUsername());  
-         System.out.println(basicService.createUser().getPassword());  
-     }  
- }  
6.启动服务器，运行测试用例，会得到如下结果：
