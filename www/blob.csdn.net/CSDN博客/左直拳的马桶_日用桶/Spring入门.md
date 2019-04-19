# Spring入门 - 左直拳的马桶_日用桶 - CSDN博客
2017年09月12日 19:54:11[左直拳](https://me.csdn.net/leftfist)阅读数：286标签：[spring](https://so.csdn.net/so/search/s.do?q=spring&t=blog)
个人分类：[Java](https://blog.csdn.net/leftfist/article/category/2412493)
我照着网上一篇教程[《Spring入门》](https://www.liaoxuefeng.com/article/0013738774263173c42eae58864423698dd40556af23bb5000)演练了一遍，然后又在网上搜了搜，说Spring东西不是很多，核心就是IoC和AOP，其中又以IoC使用最为广泛。如此说来，我已经算入门啦？
IoC，依赖注入，实质是面向接口编程，在asp.net mvc里使用Autofac，早已习惯；
AOP，实际就是拦截器，不用修改什么代码，不知不觉就起作用，也好理解。
不管咋样，先记录一下心得：
1、下载Spring 
[Spring的官网](https://projects.spring.io/spring-framework/)没有ZIP直接下载的，要用构建工具来下载，但天朝特色，呵呵。搜了搜，有这样一个网址可下载，版本稍低，但也算很新了： 
[http://maven.springframework.org/release/org/springframework/spring/](http://maven.springframework.org/release/org/springframework/spring/)
2、使用Spring
1）IoC
```java
package Spring1;
import org.springframework.context.ApplicationContext;
import org.springframework.context.support.FileSystemXmlApplicationContext;
public class Main {
    public static void main(String[] args) throws Exception{
        //IServiceBean service = new MyServiceBean();
        ApplicationContext context = new FileSystemXmlApplicationContext("beans.xml");//IoC
        IServiceBean service = (IServiceBean)context.getBean("service");
        ....
    }
}
```
```xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE beans PUBLIC "-//SPRING//DTD BEAN//EN"
 "http://www.springframework.org/dtd/spring-beans.dtd">
<beans>
    <bean id="service" class="Spring1.MyServiceBean" />
</beans>
```
2）AOP 
代码不用做什么更改，只改配置文件。将配置文件beans.xml增强，改为：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE beans PUBLIC "-//SPRING//DTD BEAN//EN"
 "http://www.springframework.org/dtd/spring-beans.dtd">
<beans>
    <bean id="serviceTarget" class="Spring1.MyServiceBean" />
    <!-- 日志 -->
    <bean id="logAdvisor" class="Spring1.LogAdvisor" />
    <!-- 将密码以星号返回 -->
    <bean id="passwordAdvisorTarget" class="Spring1.PasswordAdvisor" />
    <bean id="passwordAdvisor" class="org.springframework.aop.support.RegexpMethodPointcutAdvisor">
        <property name="advice">
            <ref local="passwordAdvisorTarget"/>
        </property>
        <property name="patterns">
            <list>
                <value>.*getPassword</value>
            </list>
        </property>
    </bean>
    <!-- 异常处理 -->
    <bean id="exceptionAdvisor" class="Spring1.ExceptionAdvisor" />
    <bean id="service" class="org.springframework.aop.framework.ProxyFactoryBean">
        <property name="proxyInterfaces"><value>Spring1.IServiceBean</value></property>
        <property name="target"><ref local="serviceTarget"/></property>
        <property name="interceptorNames">
            <list>
                <value>logAdvisor</value>
                <value>passwordAdvisor</value>
                <value>exceptionAdvisor</value>
            </list>
        </property>
    </bean>    
</beans>
```
这个XML看起来内容还不少，但仔细琢磨一下，还是能看懂。
