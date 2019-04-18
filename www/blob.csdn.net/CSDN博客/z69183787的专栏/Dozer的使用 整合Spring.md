# Dozer的使用: 整合Spring - z69183787的专栏 - CSDN博客
2019年01月07日 19:23:39[OkidoGreen](https://me.csdn.net/z69183787)阅读数：71
个人分类：[Spring-属性替换&转换																[工具-Dozer对象复制](https://blog.csdn.net/z69183787/article/category/8592721)](https://blog.csdn.net/z69183787/article/category/7065996)
[https://blog.csdn.net/github_36581033/article/details/79187116](https://blog.csdn.net/github_36581033/article/details/79187116)
Dozer与Spring的使用
Dozer是对象转换工具。比如说，当你在Controller层有一个对象ClassA，但是你要传一个ClassAVO对象给Service层，这时可以使用Dozer将对象ClassA转换成所需要的VO类。当然你也可以在其它层做这样的操作，下面使用一个例子展现Dozer到底是如何工作的：
1
1.使用Maven管理依赖包 
pom.xml文件如下
```
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <groupId>com.symbio</groupId>
    <artifactId>dozer</artifactId>
    <version>0.0.1-SNAPSHOT</version>
    <properties>
        <spring.version>5.0.3.RELEASE</spring.version>
    </properties>
    <dependencies>
        <dependency>
            <groupId>net.sf.dozer</groupId>
            <artifactId>dozer</artifactId>
            <version>5.4.0</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-context</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-beans</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-webmvc</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-web</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-jdbc</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-aspects</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-context-support</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-test</artifactId>
            <version>${spring.version}</version>
            <scope>test</scope>
        </dependency>
    </dependencies>
</project>
```
2.新建四个类ClassA、ClassB、SubClassA、SubClassB
```java
import java.util.Date;
import java.util.List;
public class ClassA {
    private Integer id;
    private String name;
    private Date date;
    private List<SubClassA> subA;
    一律省略getter and setter...
}
import java.util.Date;
import java.util.List;
public class ClassB {
    private Integer id;
    private String username;
    private Date birth;
    private List<SubClassB> subB;
    ...
}
public class SubClassA {
    private String color;
    private Integer price;
    ...
}
public class SubClassB {
    private String myColor;
    private String price;
    ...
}
```
3.使用xml方式映射，在src/main/resource下新建dozer-mapping.xml文件
```
<?xml version="1.0" encoding="UTF-8"?>
<mappings xmlns="http://dozer.sourceforge.net" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://dozer.sourceforge.net
          http://dozer.sourceforge.net/schema/beanmapping.xsd">
    <mapping date-format="yyyy-MM-dd">
        <class-a>com.symbio.dozertest.ClassA</class-a>
        <class-b>com.symbio.dozertest.ClassB</class-b>
        <field>
            <a>name</a>
            <b>username</b>
        </field>
        <field>
            <a>date</a>
            <b>birth</b>
        </field>
        <field>
            <a>subA</a>
            <b>subB</b>
            <!-- 若两个对象中还有需要mapping的对象，使用hint标签，
            再新建一个mapping标签，一直到内部没有包含可以mapping
            的对象为止 -->
            <a-hint>com.symbio.dozertest.SubClassA</a-hint>
            <b-hint>com.symbio.dozertest.SubClassB</b-hint>
        </field>
    </mapping>
    <mapping>
        <class-a>com.symbio.dozertest.SubClassA</class-a>
        <class-b>com.symbio.dozertest.SubClassB</class-b>
        <field>
            <a>color</a>
            <b>myColor</b>
        </field>
        <!-- 如果两个fields的名字一样但属性不同，也可以忽略
        但比较简单的类型可以这样做，更复杂的需要自定义Converter
        来完成对象的转换 -->
        <!-- <field>
            <a>price</a>
            <b>price</b>
        </field> -->
    </mapping>
</mappings>
```
4.Spring和Dozer整合，在这里新建spring-dozer.xml文件，当然其它名字也可以de
```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xsi:schemaLocation="http://www.springframework.org/schema/beans
       http://www.springframework.org/schema/beans/spring-beans-2.0.xsd"
       default-autowire="byName" default-lazy-init="false">
  <bean id="mapper" class="org.dozer.spring.DozerBeanMapperFactoryBean">
    <property name="mappingFiles">
      <list>
        <!-- 把Dozer映射文件引入 -->
        <value>classpath*:dozer-mapping.xml</value>
      </list>
    </property>
  </bean>
</beans>
```
5.测试类DozerTest.java
```java
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import org.dozer.Mapper;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
@RunWith(SpringJUnit4ClassRunner.class)
<!-- 一定要指定Spring配置文件的路径 -->
@ContextConfiguration(locations = {"classpath:spring-dozer.xml"})
public class DozerTest {
    @Autowired
    Mapper mapper;
    @Test
    public void dozerMap() {
        System.out.println("mapper:"+mapper);
        ClassA a = new ClassA();
        a.setId(123);
        a.setName("Curry");
        a.setDate(new Date());
        List<SubClassA> subAList = new ArrayList<SubClassA>();
        SubClassA subA = new SubClassA();
        subA.setColor("red");
        subA.setPrice(120);
        subAList.add(subA);
        a.setSubA(subAList);
        ClassB b = mapper.map(a, ClassB.class);
        System.out.println("ClassB:"+b);
    }
}
```
测试结果 
