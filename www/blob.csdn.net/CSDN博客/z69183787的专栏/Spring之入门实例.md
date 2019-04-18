# Spring之入门实例 - z69183787的专栏 - CSDN博客
2012年11月24日 20:01:39[OkidoGreen](https://me.csdn.net/z69183787)阅读数：602
个人分类：[Spring-知识](https://blog.csdn.net/z69183787/article/category/2175163)

在大型应用系统中，存在大量的工厂类。工厂类都是简单的、仅提供静态方法和变量的单实例。他们将创建对象，并将这些对象绑定在一起，这样就存在大量的重复代码。
Spring最基本的一项功能就是：充当创建对象的工厂。其具体工作步骤如下：
1. 读取并分析Spring配置文件（appcontext.xml放在classes目录下）。
2.通过Java反射机制，创建并集成上述配置文件中定义的对象。
3.将创建的对象传回给开发者的应用代码。因此，开发者不用编写工厂类。
注：Spring默认时仅创建单实例的JavaBean，通过Spring配置文件中的bean元素的singleton属性可以控制创建Java实例的方式。
appcontext.xml配置文件实例如下：
**[c-sharp]**[view
 plain](http://blog.csdn.net/weoln/article/details/4136553#)[copy](http://blog.csdn.net/weoln/article/details/4136553#)
- <?xml version="1.0" encoding="UTF-8"?>  
- <!DOCTYPE beans PUBLIC "-//SPRING//DTD BEAN//EN""http://www.springframework.org/dtd/spring-beans.dtd">  
- 
- <beans>  
- <bean name="fileHelloWorld"
- class="testspring.sample.HelloWorld">  
-     <constructor-arg>  
-         <ref bean="fileHello"/>  
-     </constructor-arg>  
- </bean>  
- 
- <bean name="fileHello"
- class="testspring.sample.HelloWorldStr">  
-     <constructor-arg>  
-         <value>helloworld.properties</value>  
-     </constructor-arg>  
- </bean>  
- 
- </beans>  
上述代码中，声明了一个名为fileHelloWorld的HelloWorld实例，该实例的构造方法需要传入一个HelloWorldStr类的实例作为参数。constructor-arg元素声明构造方法需要用到的参数，通过ref元素可以引用到Spring配置文件中的其他已定义的JavaBean。value元素直接获取参数。比如上例中，HelloWorldStr类接收“helloworld.properties”作为参数。
下面例子展示了一个简单的Spring程序。例子的功能是读取一个配置文件，并将文件里的内容输出到控制台。
**[java]**[view
 plain](http://blog.csdn.net/weoln/article/details/4136553#)[copy](http://blog.csdn.net/weoln/article/details/4136553#)
- package testspring.sample;  
- 
- import java.io.IOException;  
- import java.io.InputStream;  
- import java.util.Properties;  
- 
- publicclass HelloWorldStr {  
- 
- private String filename;  
- 
- public HelloWorldStr(String filename) {  
- this.filename = filename;  
-     }  
- 
- public String getContent() {  
-         String helloWorld = "";  
- try {  
-             Properties properties = new Properties();  
-             InputStream in = this.getClass().getClassLoader().getResourceAsStream(filename);  
-             properties.load(in);  
-             in.close();  
-             helloWorld = properties.getProperty("helloworld");  
-         } catch (IOException e) {  
-             e.printStackTrace();  
-         }  
- 
- return helloWorld;  
-     }  
- }  
HelloWorld.java:
**[java]**[view
 plain](http://blog.csdn.net/weoln/article/details/4136553#)[copy](http://blog.csdn.net/weoln/article/details/4136553#)
- package testspring.sample;  
- publicclass HelloWorld {  
- 
-     HelloWorldStr str;  
- 
- public HelloWorld(HelloWorldStr str){  
- this.str = str;  
-     }  
- 
- public String getContent(){  
- return str.getContent();  
-     }  
- }  
HelloWorldClient.java:
**[java]**[view
 plain](http://blog.csdn.net/weoln/article/details/4136553#)[copy](http://blog.csdn.net/weoln/article/details/4136553#)
- package testspring.sample;  
- 
- import java.io.IOException;  
- 
- import org.springframework.beans.factory.BeanFactory;  
- import org.springframework.beans.factory.xml.XmlBeanFactory;  
- import org.springframework.core.io.ClassPathResource;  
- import org.springframework.core.io.Resource;  
- 
- publicclass HelloWorldClient {  
- 
- /**
-      *<p>Description:<p>
-      * @param args
-      * @author mayk  2009-4-28
-      * @throws IOException 
-      */
- publicstaticvoid main(String[] args) throws IOException {  
- //      1. 读取并分析Spring配置文件（'appcontext.xml'）。
-         Resource resource = new ClassPathResource("appcontext.xml");  
- //      2. 通过Java反射机制，创建并集成上述配置文件中定义的对象。
-         BeanFactory factory = new XmlBeanFactory(resource);  
- //      3. 将创建的对象传回给开发者的应用代码。因此，开发者不用编写工厂类。
-         HelloWorld helloworld = (HelloWorld)factory.getBean("fileHelloWorld");  
-         System.out.println(helloworld.getContent());  
- 
- //
-     }  
- 
- }  
appcontext.xml文件位于src目录下。
**[c-sharp]**[view
 plain](http://blog.csdn.net/weoln/article/details/4136553#)[copy](http://blog.csdn.net/weoln/article/details/4136553#)
- <?xml version="1.0" encoding="UTF-8"?>  
- <!DOCTYPE beans PUBLIC "-//SPRING//DTD BEAN//EN""http://www.springframework.org/dtd/spring-beans.dtd">  
- 
- <beans>  
- <bean name="fileHelloWorld"
- class="testspring.sample.HelloWorld">  
-     <constructor-arg>  
-         <ref bean="fileHello"/>  
-     </constructor-arg>  
- </bean>  
- 
- <bean name="fileHello"
- class="testspring.sample.HelloWorldStr">  
-     <constructor-arg>  
-         <value>helloworld.properties</value>  
-     </constructor-arg>  
- </bean>  
- 
- </beans>  
helloworld.properties文件内容为：
  helloworld=Hi,Spring
