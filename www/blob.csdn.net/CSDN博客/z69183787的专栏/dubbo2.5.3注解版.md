# dubbo2.5.3注解版 - z69183787的专栏 - CSDN博客
2017年12月19日 18:21:19[OkidoGreen](https://me.csdn.net/z69183787)阅读数：423
1、环境
在机器192.168.0.4机器上安装了zookeeper，用于dubbo的服务注册，安装教程在另外一篇博客
      http://www.cnblogs.com/520playboy/p/6235415.html
2、生产端
2.1、项目结构
![](http://images2015.cnblogs.com/blog/917107/201701/917107-20170120214519171-91660848.png)
2.2、pom.xml
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <groupId>com.mujourney</groupId>
    <artifactId>dubbo-provider</artifactId>
    <version>0.0.1-SNAPSHOT</version>
    <name>dubbo-provider</name>
    <url>http://maven.apache.org</url>
    <properties>
        <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
    </properties>
    <dependencies>
        <dependency>
            <groupId>com.mujourney</groupId>
            <artifactId>dubbo-service</artifactId>
            <version>0.0.1-SNAPSHOT</version>
        </dependency>
        <dependency>
            <groupId>org.apache.zookeeper</groupId>
            <artifactId>zookeeper</artifactId>
            <version>3.4.6</version>
        </dependency>
        <dependency>
            <groupId>com.alibaba</groupId>
            <artifactId>dubbo</artifactId>
            <version>2.5.3</version>
        </dependency>
        <dependency>
            <groupId>com.101tec</groupId>
            <artifactId>zkclient</artifactId>
            <version>0.5</version>
        </dependency>
    </dependencies>
    <build>
        <plugins>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-compiler-plugin</artifactId>
                <version>3.3</version>
                <configuration>
                    <source>1.7</source>
                    <target>1.7</target>
                    <verbose>true</verbose>
                </configuration>
            </plugin>
        </plugins>
    </build>
</project>
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
 2.3、UserServiceImpl.java
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
package com.mujourney.service;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import com.alibaba.dubbo.config.annotation.Service;
@Service(interfaceName="com.mujourney.service.UserService",timeout=10000)
public class UserServiceImpl implements UserService {
    public Map<String, Object> findById(int id) {
        return get(id);
    }
    public List<Map<String, Object>> queryList() {
        List<Map<String, Object>> list = new ArrayList<Map<String, Object>>();
        for (int i = 1; i <= 10; i++) {
            list.add(get(i));
        }
        return list;
    }
    private Map<String, Object> get(int id) {
        Map<String, Object> res = new HashMap<String, Object>();
        res.put("id", id);
        res.put("name", "1");
        res.put("type", "1");
        res.put("date", "1");
        return res;
    }
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
2.4、StartServer.java
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
package com.mujourney.service;
import java.util.concurrent.TimeUnit;
import org.springframework.context.support.ClassPathXmlApplicationContext;
public class StartServer {
    public static void main(String[] args) {
        ClassPathXmlApplicationContext context = new ClassPathXmlApplicationContext(
                new String[] { "application-provider.xml" });
        context.start();
        System.out.println("=========dubbo生产者启动完毕========");
        try {
            TimeUnit.HOURS.sleep(1l);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
2.5、application-provider.xml
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
<?xml version="1.0" encoding="UTF-8"?>  
<beans xmlns="http://www.springframework.org/schema/beans"  
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"   
       xmlns:dubbo="http://code.alibabatech.com/schema/dubbo"  
       xsi:schemaLocation="http://www.springframework.org/schema/beans   
       http://www.springframework.org/schema/beans/spring-beans-4.0.xsd   
       http://code.alibabatech.com/schema/dubbo    
       http://code.alibabatech.com/schema/dubbo/dubbo.xsd">  
  
    <!-- 指定dubbo的服务名称 -->  
    <dubbo:application name="my_provider" />  
      
    <!-- 使用multicast广播注册中心暴露服务地址 -->  
    <!-- <dubbo:registry protocol="zookeeper" address="zookeeper://192.168.18.25:2181" /> -->  
      <dubbo:registry protocol="zookeeper" address="zookeeper://192.168.0.4:2181" />
      
    <!-- 用dubbo协议在20880端口暴露服务 -->  
    <dubbo:protocol name="dubbo" port="20880" />  
  
    <!-- 具体的实现bean -->  
   <!--  <bean id="userService" class="com.mujourney.service.UserServiceImpl" />   -->
  
    <!-- 声明需要暴露的服务接口 -->  
    <!-- <dubbo:service interface="com.mujourney.service.UserService" ref="userService" />  -->
     
    <!-- 扫描注解包路径，多个包用逗号分隔，不填pacakge表示当前ApplicationContext中所有的类 -->
   <dubbo:annotation package="com.mujourney.service"></dubbo:annotation>
</beans>  
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
因为使用注解来创建服务，所以就把生成者的配置文件创建UserServiceImpl类和指定接口给注释了
3、消费端 
3.1 、项目结构
![](http://images2015.cnblogs.com/blog/917107/201701/917107-20170120215757187-1364416312.png)
3.2、pom.xml
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <groupId>com.mujourney</groupId>
    <artifactId>dubbo-consumer</artifactId>
    <version>0.0.1-SNAPSHOT</version>
    <name>dubbo-consumer</name>
    <url>http://maven.apache.org</url>
    <properties>
        <spring.version>3.2.4.RELEASE</spring.version>
        <junit.version>4.8.2</junit.version>
        <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
    </properties>
    <dependencies>
        <dependency>
            <groupId>com.alibaba</groupId>
            <artifactId>dubbo</artifactId>
            <version>2.5.3</version>
            <exclusions>  
                <exclusion>  
                    <artifactId>spring</artifactId>  
                    <groupId>org.springframework</groupId>  
                </exclusion>  
            </exclusions>  
        </dependency> 
        <dependency>
            <groupId>org.apache.zookeeper</groupId>
            <artifactId>zookeeper</artifactId>
            <version>3.4.6</version>
        </dependency>
        <dependency>
            <groupId>com.github.sgroschupf</groupId>
            <artifactId>zkclient</artifactId>
            <version>0.1</version>
        </dependency>
        <!-- 关联dubbo-service工程源码 --> 
        <dependency>
            <groupId>com.mujourney</groupId>
            <artifactId>dubbo-service</artifactId>
            <version>0.0.1-SNAPSHOT</version>
        </dependency> 
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-core</artifactId>
            <version>${spring.version}</version>
            <type>jar</type>
            <optional>false</optional>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-beans</artifactId>
            <version>${spring.version}</version>
            <type>jar</type>
            <optional>false</optional>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-context</artifactId>
            <version>${spring.version}</version>
            <type>jar</type>
            <optional>false</optional>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-test</artifactId>
            <version>${spring.version}</version>
            <scope>test</scope>
            <type>jar</type>
        </dependency>
        <dependency>
            <groupId>junit</groupId>
            <artifactId>junit</artifactId>
            <version>${junit.version}</version>
            <scope>test</scope>
            <type>jar</type>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-context-support</artifactId>
            <version>3.0.5.RELEASE</version>
            <optional>false</optional>
        </dependency>
        
    </dependencies>
    <build>
        <plugins>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-compiler-plugin</artifactId>
                <version>3.3</version>
                <configuration>
                    <source>1.7</source>
                    <target>1.7</target>
                    <verbose>true</verbose>
                </configuration>
            </plugin>
        </plugins>
    </build>
</project>
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
3.3、application-consumer.xml
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
<?xml version="1.0" encoding="UTF-8"?>  
<beans xmlns="http://www.springframework.org/schema/beans"  
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"   
       xmlns:dubbo="http://code.alibabatech.com/schema/dubbo"  
       xsi:schemaLocation="http://www.springframework.org/schema/beans   
       http://www.springframework.org/schema/beans/spring-beans-4.0.xsd   
       http://code.alibabatech.com/schema/dubbo    
       http://code.alibabatech.com/schema/dubbo/dubbo.xsd">  
  
    <!-- 指定生成者 -->  
    <dubbo:application name="my_provider" />  
  
    <!-- 使用multicast广播注册中心暴露服务地址 -->  
    <dubbo:registry address="zookeeper://192.168.0.4:2181" />  
  
      <!-- 扫描注解包路径，多个包用逗号分隔，不填package表示扫描当前ApplicationContext中的所有类 -->
    <dubbo:annotation package="com.mujourney.service"></dubbo:annotation>   
</beans>
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
3.4、ClientTest.java
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
package com.mujourney.service;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.stereotype.Component;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import com.alibaba.dubbo.config.annotation.Reference;
@Component
@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations={"/application-consumer.xml"})
public class ClientTest {
    
    @Reference(interfaceName="com.mujourney.service.UserService")
    private UserService uservice;
    
    @Test
    public void pors()  {
        System.out.println(uservice.findById(1));
    }
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
4、服务的接口端
4.1、项目结构
![](http://images2015.cnblogs.com/blog/917107/201701/917107-20170120220440593-603076788.png)
4.2 pom.xml
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>
  <groupId>com.mujourney</groupId>
  <artifactId>dubbo-service</artifactId>
  <version>0.0.1-SNAPSHOT</version>
  
  <dependencies>  
        <dependency>  
            <groupId>junit</groupId>  
            <artifactId>junit</artifactId>  
            <version>4.10</version>  
            <scope>test</scope>  
        </dependency>  
    </dependencies>  
  
    <build>  
        <plugins>  
            <plugin>  
                <groupId>org.apache.maven.plugins</groupId>  
                <artifactId>maven-compiler-plugin</artifactId>  
                <version>3.3</version>  
                <configuration>  
                    <source>1.7</source>  
                    <target>1.7</target>  
                    <verbose>true</verbose>  
                </configuration>  
            </plugin>  
  
            <plugin>  
                <groupId>org.apache.maven.plugins</groupId>  
                <artifactId>maven-source-plugin</artifactId>  
                <version>2.4</version>  
                <executions>  
                    <execution>  
                        <id>attach-sources</id>  
                        <phase>verify</phase>  
                        <goals>  
                            <goal>jar-no-fork</goal>  
                        </goals>  
                    </execution>  
                </executions>  
            </plugin>  
  
        </plugins>  
    </build>  
</project>
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
4.3、UserService.java
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
package com.mujourney.service;
import java.util.List;
import java.util.Map;
public interface UserService {
     public Map<String, Object> findById(int id);  
     
     public List<Map<String, Object>> queryList();  
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
之所以有一个服务的接口端，因为生产者和消费者都要用到接口，把接口单独作为maven的java项目可以打jar包放到私库给生产端和消费端同时使用。
