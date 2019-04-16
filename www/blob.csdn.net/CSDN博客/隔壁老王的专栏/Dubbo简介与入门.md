# Dubbo简介与入门 - 隔壁老王的专栏 - CSDN博客





2016年12月06日 22:17:42[隔壁老王来也](https://me.csdn.net/haoyuyang)阅读数：667








[Dubbo](http://dubbo.io/)是一个被国内很多互联网公司广泛使用的开源分布式服务框架，即使从国际视野来看应该也是一个非常全面的SOA基础框架。作为一个重要的技术研究课题，当当网根据自身的需求，为Dubbo实现了一些新的功能，并将其命名为Dubbox（Dubbo eXtensions）。![](https://img-blog.csdn.net/20161206222147424)

**1.Dubbo是什么？**

Dubbo是一个分布式服务框架，致力于提供高性能和透明化的RPC远程服务调用方案，以及SOA服务治理方案。简单的说，dubbo就是个服务框架，如果没有分布式的需求，其实是不需要用dubbo的，只有在分布式的时候，才有dubbo这样的分布式服务框架的需求，并且本质上就是个服务调用的东东。说白了dubbo就是个远程服务调用的分布式框架（告别WebService模式中的WSDL，以服务者与消费者的方式在dubbo上注册）。

其核心部分包含：

①远程通讯：提供对多种基于长连接的NIO框架抽象封装，包含多线程模型、序列化以及“请求—响应”模式的信息交换方式。

②集群容错：提供基于接口方法的透明远程过程调用，包括多协议支持以及软负载均衡、失败容错、地址路由、动态配置等集群支持。

③自动发现：基于注册中心目录服务，使服务消费方能够动态的查找服务提供方，使地址透明，使服务提供方可以平滑的增加或减少机器。

**2.Dubbo能做什么？**

①透明化的远程方法调用，就像调用本地方法一样调用远程方法，只需简单的配置，没有任何API侵入。

②软负载均衡及容错机制，可在内网替代F5等硬件负载均衡器，降低成本，减少单点。

③服务自动注册与发现，不再需要写死服务提供方地址，注册中心基于接口名查询服务提供者的IP地址，并且能够平滑的添加或删除服务提供者。

④Dubbo采用全Spring配置方式，透明化接入应用，对应用没有任何API侵入，只需用Spring加载Dubbo的配置即可。Dubbo基于Spring的Schema扩展进行加载。

**3.Dubbo架构**

**![](https://img-blog.csdn.net/20161206225208579)**

①服务提供者在启动时，向注册中心注册自己提供的服务。

②服务消费者在启动时，向注册中心订阅自己所需的服务。

③注册中心返回服务提供者的地址列表给消费者，如果有变更，注册中心将基于长连接推送变更数据给消费者。

④服务消费者从提供者的地址列表中基于软负载均衡算法，选一台提供者进行调用，如果调用失败，则另选一台调用。

⑤服务消费者和提供者在内存中累计调用次数和调用时间，定时每分钟发送一次统计数据到监控中心。

**4.Dubbo HelloWorld入门：**

使用Idea新建两个maven项目，一个叫provider，另一个叫consumer。不会新建Maven的看[这里](http://www.cnblogs.com/wql025/p/5215570.html)。

两个项目的pom.xml文件如下：

```
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>

    <groupId>com.hyy</groupId>
    <artifactId>dubbo</artifactId>
    <version>1.0-SNAPSHOT</version>
    <packaging>war</packaging>

    <properties>
        <spring.version>4.2.8.RELEASE</spring.version>
    </properties>

    <dependencies>
        <!-- dubbo -->
        <dependency>
            <groupId>com.alibaba</groupId>
            <artifactId>dubbo</artifactId>
            <version>2.5.3</version>
            <exclusions>
                <exclusion>
                    <groupId>org.springframework</groupId>
                    <artifactId>spring</artifactId>
                </exclusion>
            </exclusions>
        </dependency>
        <!-- zkClient -->
        <dependency>
            <groupId>com.101tec</groupId>
            <artifactId>zkclient</artifactId>
            <version>0.10</version>
        </dependency>
        <!-- Spring -->
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-core</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-beans</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-context</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-jdbc</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-web</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-webmvc</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-aop</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-tx</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-orm</artifactId>
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
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-jms</artifactId>
            <version>${spring.version}</version>
        </dependency>
    </dependencies>

</project>
```
provider项目的目录结构：



![](https://img-blog.csdn.net/20161208195341967)


User是一个实体类，实现了Serializable接口，yo

```java
public class User implements Serializable {
    private String name;
    private int age;
    private String gender;

    public User() {
    }

    public User(String name, int age, String gender) {
        this.name = name;
        this.age = age;
        this.gender = gender;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getAge() {
        return age;
    }

    public void setAge(int age) {
        this.age = age;
    }

    public String getGender() {
        return gender;
    }

    public void setGender(String gender) {
        this.gender = gender;
    }
}
```



SampleService接口类：

```java
public interface SampleService {
    String sayHello();

    List<User> getUsers();
}
```
SampleServiceImpl是SampleService的实现类：
```java
public class SampleServiceImpl implements SampleService {
    public String sayHello() {
        return "Hello World!";
    }

    public List<User> getUsers() {
        List<User> users = new LinkedList<User>();
        User u1 = new User("老王", 28, "male");
        User u2 = new User("老李", 27, "female");
        User u3 = new User("老张", 26, "male");
        users.add(u1);
        users.add(u2);
        users.add(u3);
        return users;
    }
}
```
sample-provider.xml为dubbo的配置文件：
```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:dubbo="http://code.alibabatech.com/schema/dubbo"
       xsi:schemaLocation="http://www.springframework.org/schema/beans
        http://www.springframework.org/schema/beans/spring-beans.xsd
        http://code.alibabatech.com/schema/dubbo
        http://code.alibabatech.com/schema/dubbo/dubbo.xsd">
    <bean id="sampleService" class="com.hyy.sample.provider.service.impl.SampleServiceImpl" />
    <!-- 提供方信息，用于计算依赖关系 -->
    <dubbo:application name="sample-provider" />
    <!-- 用dubbo协议在20880端口暴露服务 -->
    <dubbo:protocol name="dubbo" port="20880" />
    <!-- 使用zookeeper注册中心暴露服务地址 -->
    <dubbo:registry address="zookeeper://192.168.3.58:2181?backup=192.168.3.59:2181,192.168.3.66:2181" />
    <dubbo:service interface="com.hyy.sample.provider.service.SampleService" ref="sampleService" />
</beans>
```
Provider是测试类：
```java
public class Provider {
    public static void main(String[] args) throws IOException {
        ClassPathXmlApplicationContext context = new ClassPathXmlApplicationContext(new String[]{"sample-provider.xml"});
        context.start();
        System.out.println("提供者启动");
        System.in.read();
    }
}
```
consumer项目目录结构：



![](https://img-blog.csdn.net/20161208194848917)


User类需要与provider项目的包路径一致，要不然会报类找不到异常。

SampleService与provider项目中的一样，包路径也得一样。

sample-consumer.xml文件：

```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:dubbo="http://code.alibabatech.com/schema/dubbo"
       xsi:schemaLocation="http://www.springframework.org/schema/beans
        http://www.springframework.org/schema/beans/spring-beans.xsd
        http://code.alibabatech.com/schema/dubbo
        http://code.alibabatech.com/schema/dubbo/dubbo.xsd">
    <dubbo:registry address="zookeeper://192.168.3.58:2181?backup=192.168.3.59:2181,192.168.3.66:2181" />
    <dubbo:application name="sample-consumer" />
    <dubbo:reference id="sampleService" interface="com.hyy.sample.provider.service.SampleService" check="false" />
</beans>
```
Consumer为消费端测试类：
```java
public class Consumer {
    public static void main(String[] args) throws IOException {
        ClassPathXmlApplicationContext context = new ClassPathXmlApplicationContext(new String[]{"sample-consumer.xml"});
        context.start();
        SampleService sampleService = (SampleService) context.getBean("sampleService");
        System.out.println(sampleService.sayHello());
        List<User> users = sampleService.getUsers();
        System.in.read();
    }
}
```
如果看不懂这个例子，可以看[Dubbo配置方式详解](http://www.cnblogs.com/chanshuyi/p/5144288.html)，这篇文章中还介绍了基于注解的配置方式。



























