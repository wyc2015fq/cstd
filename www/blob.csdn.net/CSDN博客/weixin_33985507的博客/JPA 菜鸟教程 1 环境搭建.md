# JPA 菜鸟教程 1 环境搭建 - weixin_33985507的博客 - CSDN博客
2017年02月11日 11:52:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
### JPA
- 
**JPA全称Java Persistence API.**
- 
**JPA通过JDK 5.0注解或XML描述对象－关系表的映射关系，并将运行期的实体对象持久化到数据库中。**
- 
**Hibernate3.2+、TopLink(EclipseLink)以及OpenJPA都提供了JPA的实现。**
### 搭建JPA开发环境
- **JPA依赖的jar文件、数据库驱动文件**
- **配置文件：META-INF\persistence.xml**
- **领域对象，POJO**
- **EntityManagerFactory**
### 异常
- 
**javax.persistence.PersistenceException: No Persistence provider for EntityManager named  com.jege.jpa**
- 
**Hibernate从3.5开始必须添加hibernate-entitymanager相应jar文件支持JPA，否则使用JPA会出现如上异常**
### maven的pom.xml
```
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <groupId>com.jege.hibernate</groupId>
    <artifactId>jpa-single-table</artifactId>
    <version>1.0.0.RELEASE</version>
    <packaging>jar</packaging>
    <name>jpa-single-table</name>
    <url>http://maven.apache.org</url>
    <properties>
        <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
        <hibernate.version>4.3.8.Final</hibernate.version>
        <mysql.version>5.1.40</mysql.version>
        <junit.version>4.12</junit.version>
        <java.version>1.8</java.version>
    </properties>
    <dependencies>
        <dependency>
            <groupId>org.hibernate</groupId>
            <artifactId>hibernate-core</artifactId>
            <version>${hibernate.version}</version>
        </dependency>
        <!-- 必须添加 -->
        <dependency>
            <groupId>org.hibernate</groupId>
            <artifactId>hibernate-entitymanager</artifactId>
            <version>${hibernate.version}</version>
        </dependency>
        <dependency>
            <groupId>mysql</groupId>
            <artifactId>mysql-connector-java</artifactId>
            <version>${mysql.version}</version>
        </dependency>
        <dependency>
            <groupId>junit</groupId>
            <artifactId>junit</artifactId>
            <version>${junit.version}</version>
            <scope>test</scope>
        </dependency>
    </dependencies>
    <build>
        <finalName>jpa-single-table</finalName>
        <plugins>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-compiler-plugin</artifactId>
                <configuration>
                    <source>${java.version}</source>
                    <target>${java.version}</target>
                </configuration>
            </plugin>
        </plugins>
    </build>
</project>
```
### 配置文件persistence.xml
JPA规范要求在类路径的META-INF目录下放置persistence.xml，文件的名称是固定的
```
<persistence xmlns="http://java.sun.com/xml/ns/persistence" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://java.sun.com/xml/ns/persistence http://java.sun.com/xml/ns/persistence/persistence_2_0.xsd"
    version="2.0">
    <persistence-unit name="com.jege.jpa" transaction-type="RESOURCE_LOCAL">
        <properties>
            <property name="hibernate.connection.driver_class" value="com.mysql.jdbc.Driver" />
            <property name="hibernate.connection.url" value="jdbc:mysql:///jpa" />
            <property name="hibernate.connection.username" value="root" />
            <property name="hibernate.connection.password" value="admin" />
            <property name="hibernate.dialect" value="org.hibernate.dialect.MySQL5Dialect" />
            <property name="hibernate.show_sql" value="true" />
            <property name="hibernate.hbm2ddl.auto" value="create" />
        </properties>
    </persistence-unit>
</persistence>
```
### JPA领域对象POJO
- **必须使用javax.persistence.Entity注解或者在XML映射文件中有对应的元素。**
- **必须具有一个不带参的构造函数，类不能声明final，方法和持久化的属性也不能声明成final。并且持久化的属性修饰符不能为public。**
- **如果POJO实体需要以值的方式进行传递如Session bean，或者作为主键，则必须实现Serializable接口**
## 源码地址
[https://github.com/je-ge/jpa](https://link.jianshu.com?t=https://github.com/je-ge/jpa)
**如果觉得我的文章或者代码对您有帮助,可以请我喝杯咖啡。**
**您的支持将鼓励我继续创作!谢谢！ **
微信打赏
支付宝打赏
