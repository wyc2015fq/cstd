# TestNG + Spring Integration Example - z69183787的专栏 - CSDN博客
2016年10月14日 18:20:39[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1322
In this tutorial, we will show you how to test Spring’s components with TestNG.
Tools used :
- TestNG 6.8.7
- Spring 3.2.2.RELEASE
- Maven 3
- Eclipse IDE
## 1. Project Dependencies
To integrate Spring with TestNG, you need `spring-test.jar`, add the following :
pom.xml
```
<properties>
	<spring.version>3.2.2.RELEASE</spring.version>
	<testng.version>6.8.7</testng.version>
</properties>
<dependencies>
	<dependency>
		<groupId>org.springframework</groupId>
		<artifactId>spring-core</artifactId>
		<version>${spring.version}</version>
	</dependency>
	<dependency>
		<groupId>org.springframework</groupId>
		<artifactId>spring-test</artifactId>
		<version>${spring.version}</version>
	</dependency>
	<dependency>
		<groupId>org.testng</groupId>
		<artifactId>testng</artifactId>
		<version>${testng.version}</version>
		<scope>test</scope>
	</dependency>
</dependencies>
```
## 2. Spring Component
Create a simple Spring component, later we will test this component with TestNG.
EmailGenerator.java
```java
```java
package com.mkyong.testng.project.service.email;
public interface EmailGenerator {
	String generate();
}
```
```
RandomEmailGenerator.java
```java
```java
package com.mkyong.testng.project.service.email;
import org.springframework.stereotype.Service;
@Service
public class RandomEmailGenerator implements EmailGenerator {
	@Override
	public String generate() {
		return "feedback@yoursite.com";
	}
}
```
```
## 3. TestNG + Spring
Create a Spring configuration file in test folder, for Spring components scanning.
${project}/src/test/resources/spring-test-config.xml
```
<beans xmlns="http://www.springframework.org/schema/beans"
	xmlns:context="http://www.springframework.org/schema/context"
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xsi:schemaLocation="
		http://www.springframework.org/schema/beans
		http://www.springframework.org/schema/beans/spring-beans-3.2.xsd
    http://www.springframework.org/schema/context
	http://www.springframework.org/schema/context/spring-context-3.2.xsd
	">
	<context:component-scan base-package="com.mkyong.testng" />
</beans>
```
To access the Spring components in TestNG, extends `AbstractTestNGSpringContextTests`, see
 the following example :
${project}/src/test/java/com/mkyong/testng/examples/spring/TestSpring.java
```java
```java
package com.mkyong.testng.examples.spring;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.testng.AbstractTestNGSpringContextTests;
import org.testng.Assert;
import org.testng.annotations.Test;
import com.mkyong.testng.project.service.email.EmailGenerator;
@Test
@ContextConfiguration(locations = { "classpath:spring-test-config.xml" })
public class TestSpring extends AbstractTestNGSpringContextTests {
	@Autowired
	EmailGenerator emailGenerator;
	@Test()
	void testEmailGenerator() {
		String email = emailGenerator.generate();
		System.out.println(email);
		Assert.assertNotNull(email);
		Assert.assertEquals(email, "feedback@yoursite.com");
	}
}
```
```
Output
```bash
feedback@yoursite.com
PASSED: testEmailGenerator
===============================================
    Default test
    Tests run: 1, Failures: 0, Skips: 0
===============================================
```
