
# Spring基础：快速入门spring boot（8）：使用Junit进行测试 - 知行合一 止于至善 - CSDN博客

2018年11月04日 05:40:39[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：1851所属专栏：[spring快速入门](https://blog.csdn.net/column/details/13907.html)



![在这里插入图片描述](https://img-blog.csdn.net/20161203082310637)
使用Junit或者TestNG可以进行单体测试，这篇文章简单说明一下如何在Spring boot的项目中使用Junit进行单体测试。
# pom设定
pom中需要添加spring-boot-starter-test
`<dependency>
			<groupId>org.springframework.boot</groupId>
			<artifactId>spring-boot-starter-test</artifactId>
			<scope>test</scope>
		</dependency>`
# 确认依赖
使用Java进行Unit测试至少需要Junit之类的测试框架，另外spring boot相关的测试还应该有一些Mock相关的依赖，这个spring-boot-starter-test到底包含什么，可以使用maven dependency来进行确认一下。
> 使用命令：mvn dependency:tree
结果信息：
`[INFO] --- maven-dependency-plugin:3.0.2:tree (default-cli) @ springbootdemo ---
[INFO] com.liumiaocn:springbootdemo:jar:0.0.1-SNAPSHOT
[INFO] +- org.springframework.boot:spring-boot-starter-web:jar:2.0.6.RELEASE:compile
[INFO] |  +- org.springframework.boot:spring-boot-starter:jar:2.0.6.RELEASE:compile
[INFO] |  |  +- org.springframework.boot:spring-boot:jar:2.0.6.RELEASE:compile
[INFO] |  |  +- org.springframework.boot:spring-boot-autoconfigure:jar:2.0.6.RELEASE:compile
[INFO] |  |  +- org.springframework.boot:spring-boot-starter-logging:jar:2.0.6.RELEASE:compile
[INFO] |  |  |  +- ch.qos.logback:logback-classic:jar:1.2.3:compile
[INFO] |  |  |  |  \- ch.qos.logback:logback-core:jar:1.2.3:compile
[INFO] |  |  |  +- org.apache.logging.log4j:log4j-to-slf4j:jar:2.10.0:compile
[INFO] |  |  |  |  \- org.apache.logging.log4j:log4j-api:jar:2.10.0:compile
[INFO] |  |  |  \- org.slf4j:jul-to-slf4j:jar:1.7.25:compile
[INFO] |  |  +- javax.annotation:javax.annotation-api:jar:1.3.2:compile
[INFO] |  |  \- org.yaml:snakeyaml:jar:1.19:runtime
[INFO] |  +- org.springframework.boot:spring-boot-starter-json:jar:2.0.6.RELEASE:compile
[INFO] |  |  +- com.fasterxml.jackson.core:jackson-databind:jar:2.9.7:compile
[INFO] |  |  |  +- com.fasterxml.jackson.core:jackson-annotations:jar:2.9.0:compile
[INFO] |  |  |  \- com.fasterxml.jackson.core:jackson-core:jar:2.9.7:compile
[INFO] |  |  +- com.fasterxml.jackson.datatype:jackson-datatype-jdk8:jar:2.9.7:compile
[INFO] |  |  +- com.fasterxml.jackson.datatype:jackson-datatype-jsr310:jar:2.9.7:compile
[INFO] |  |  \- com.fasterxml.jackson.module:jackson-module-parameter-names:jar:2.9.7:compile
[INFO] |  +- org.springframework.boot:spring-boot-starter-tomcat:jar:2.0.6.RELEASE:compile
[INFO] |  |  +- org.apache.tomcat.embed:tomcat-embed-core:jar:8.5.34:compile
[INFO] |  |  +- org.apache.tomcat.embed:tomcat-embed-el:jar:8.5.34:compile
[INFO] |  |  \- org.apache.tomcat.embed:tomcat-embed-websocket:jar:8.5.34:compile
[INFO] |  +- org.hibernate.validator:hibernate-validator:jar:6.0.13.Final:compile
[INFO] |  |  +- javax.validation:validation-api:jar:2.0.1.Final:compile
[INFO] |  |  +- org.jboss.logging:jboss-logging:jar:3.3.2.Final:compile
[INFO] |  |  \- com.fasterxml:classmate:jar:1.3.4:compile
[INFO] |  +- org.springframework:spring-web:jar:5.0.10.RELEASE:compile
[INFO] |  |  \- org.springframework:spring-beans:jar:5.0.10.RELEASE:compile
[INFO] |  \- org.springframework:spring-webmvc:jar:5.0.10.RELEASE:compile
[INFO] |     +- org.springframework:spring-aop:jar:5.0.10.RELEASE:compile
[INFO] |     +- org.springframework:spring-context:jar:5.0.10.RELEASE:compile
[INFO] |     \- org.springframework:spring-expression:jar:5.0.10.RELEASE:compile
[INFO] \- org.springframework.boot:spring-boot-starter-test:jar:2.0.6.RELEASE:test
[INFO]    +- org.springframework.boot:spring-boot-test:jar:2.0.6.RELEASE:test
[INFO]    +- org.springframework.boot:spring-boot-test-autoconfigure:jar:2.0.6.RELEASE:test
[INFO]    +- com.jayway.jsonpath:json-path:jar:2.4.0:test
[INFO]    |  +- net.minidev:json-smart:jar:2.3:test
[INFO]    |  |  \- net.minidev:accessors-smart:jar:1.2:test
[INFO]    |  |     \- org.ow2.asm:asm:jar:5.0.4:test
[INFO]    |  \- org.slf4j:slf4j-api:jar:1.7.25:compile
[INFO]    +- junit:junit:jar:4.12:test
[INFO]    +- org.assertj:assertj-core:jar:3.9.1:test
[INFO]    +- org.mockito:mockito-core:jar:2.15.0:test
[INFO]    |  +- net.bytebuddy:byte-buddy:jar:1.7.11:test
[INFO]    |  +- net.bytebuddy:byte-buddy-agent:jar:1.7.11:test
[INFO]    |  \- org.objenesis:objenesis:jar:2.6:test
[INFO]    +- org.hamcrest:hamcrest-core:jar:1.3:test
[INFO]    +- org.hamcrest:hamcrest-library:jar:1.3:test
[INFO]    +- org.skyscreamer:jsonassert:jar:1.5.0:test
[INFO]    |  \- com.vaadin.external.google:android-json:jar:0.0.20131108.vaadin1:test
[INFO]    +- org.springframework:spring-core:jar:5.0.10.RELEASE:compile
[INFO]    |  \- org.springframework:spring-jcl:jar:5.0.10.RELEASE:compile
[INFO]    +- org.springframework:spring-test:jar:5.0.10.RELEASE:test
[INFO]    \- org.xmlunit:xmlunit-core:jar:2.5.1:test`在org.springframework.boot:spring-boot-starter-test:jar:2.0.6.RELEASE:test下面我们看到了junit:junit:jar:4.12:test以及org.mockito:mockito-core:jar:2.15.0:test的信息，这是spring boot已经整理完毕的内容，我们只需要写TestCase即可。
# 创建测试目录
根据惯例创建测试目录如下：
`liumiaocn:src liumiao$ ls
main test
liumiaocn:src liumiao$ find . -type d
.
./test
./test/java
./test/java/com
./test/java/com/liumiaocn
./test/java/com/liumiaocn/springbootdemo
./main
./main/resources
./main/java
./main/java/com
./main/java/com/liumiaocn
./main/java/com/liumiaocn/springbootdemo
liumiaocn:src liumiao$`
# 创建测试用例
## 代码示例
`liumiaocn:src liumiao$ cat ./test/java/com/liumiaocn/springbootdemo/SpringbootdemoApplicationTests.java 
package com.liumiaocn.springbootdemo;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringRunner;
@RunWith(SpringRunner.class)
@SpringBootTest
public class SpringbootdemoApplicationTests {
	@Test
	public void contextLoads() {
	}
}
liumiaocn:src liumiao$`
## 代码说明
COC：根据惯例进行定义测试类的名称
SpringBootTest注解：SpringBootTest是1.4之后引入的一个注解，使得springboot的测试变得更加方便
RunWith注解：使用了此注解的情况下，JUnit会调用RunWith中所指定的类。不同的框架提供相应的Runner用于测试，比如Junit自己的JUnit4.class，在比如spring的SpringJUnit4ClassRunner或者SpringRunner，都可与之结合使用。
Test注解：Junit的常用注解之一，用于定义测试方法，不再赘述。
# 执行测试
> 命令： mvn test
部分执行内容如下：
`[INFO] Results:
[INFO] 
[INFO] Tests run: 1, Failures: 0, Errors: 0, Skipped: 0`在实施的结果中可以看到运行起来的测试用例为1个，没有失败/跳过/出错的。
# 总结
通过spring-boot-starter-test的引入，在springboot中已经做好所有单元测试的准备，根据惯例的方式设定测试目录和文件名称，结合使用springboottest等注解，可以使用Junit对springboot的应用进行测试。最后使用mvn test即可运行相关的测试用例并可确认结果，后面将会进一步说明如何与sonarqube以及jacoco等进行结合确认代码扫描等。

