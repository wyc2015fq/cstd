# 使用Restful风格的Web Service(Maven版本) - z69183787的专栏 - CSDN博客
2015年06月10日 22:57:53[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3497
[该教程翻译自Spring官方，并进行适当删减。]
**你将搭建的**
你将创建的应用将使用Spring的RestTemplate来获取Facebook的Graph API的数据。（符合Restful风格）
**[plain]**[view
 plain](http://blog.csdn.net/chenloveit/article/details/38780775#)[copy](http://blog.csdn.net/chenloveit/article/details/38780775#)
- http://graph.facebook.com/pivotalsoftware  
它将返回的JSON字符串为：
**[plain]**[view
 plain](http://blog.csdn.net/chenloveit/article/details/38780775#)[copy](http://blog.csdn.net/chenloveit/article/details/38780775#)
- {  
-    "id": "161112704050757",  
-    "about": "Pivotal is enabling the creation of modern software applications that leverage big & fast data \u2013 on a single, cloud independent platform.",  
-    "can_post": false,  
-    "category": "Internet/software",  
-    "category_list": [  
-       {  
-          "id": "108472109230615",  
-          "name": "Computer Services"  
-       }  
-    ],  
-    "checkins": 42,  
-    "cover": {  
-       "cover_id": 163344023827625,  
-       "offset_x": 0,  
-       "offset_y": 0,  
-       "source": "https://fbcdn-sphotos-d-a.akamaihd.net/hphotos-ak-xaf1/t1.0-9/s720x720/554668_163344023827625_839302172_n.png"  
-    },  
-    "description": "Pivotal, the company at the intersection of big data, PaaS, and agile development, helps companies transform into great software companies. It starts with Pivotal One, a comprehensive solution that includes a set of application and data services that run on top of Pivotal CF, a turnkey platform-as-a-service (PaaS) solution for agile development teams to rapidly update and scale applications on a private cloud that can be instantly expanded and upgraded with no downtime, allowing enterprises to innovate with disruptive speed.\n\nREVOLUTIONARY COMPREHENSIVE PAAS\nPivotal One is a next generation PaaS that can be deployed on multiple cloud environments to deliver a turnkey experience for scaling and updating PaaS with no downtime. Pivotal One Services helps create a PaaS that no other vendor can offer in the industry by integrating differentiated data services such as Hadoop and visual analytics.\n\nSPEED TIME TO MARKET\nDriving the demand for PaaS is the trend of software being the competitive edge across all industries. This trend has unleashed a new generation of developers driving a deep shift in platforms and processes. These developers work in agile teams and demand a platform that allows them to continuously deliver updates to and horizontally scale their applications with no downtime. They seek standardized ways to plug in leading data services and perform deep user analytics on top of massive datasets to drive rapid iteration based on customer needs.\n\nABOUT PIVOTAL\nPivotal, committed to open source and open standards, is a leading provider of application and data infrastructure software, agile development services, and data science consulting. Follow Pivotal on Twitter \u0040gopivotal.",  
-    "founded": "2013",  
-    "has_added_app": false,  
-    "is_community_page": false,  
-    "is_published": true,  
-    "likes": 1022,  
-    "link": "https://www.facebook.com/pivotalsoftware",  
-    "location": {  
-       "city": "San Francisco",  
-       "country": "United States",  
-       "latitude": 37.78199,  
-       "longitude": -122.40406,  
-       "state": "CA",  
-       "street": "875 Howard St",  
-       "zip": "94103"  
-    },  
-    "mission": "Pivotal, the company at the intersection of big data, PaaS, and agile development, helps companies transform into great software companies. ",  
-    "name": "Pivotal",  
-    "parking": {  
-       "lot": 0,  
-       "street": 0,  
-       "valet": 0  
-    },  
-    "phone": "(650) 286-8012",  
-    "products": "PaaS:\nPivotal One, Pivotal CF, Cloud Foundry\n\nDATA: Pivotal HD, Pivotal HD with GemFire XD, Pivotal Greenplum DB, Pivotal Data Dispatch,  Pivotal GemFire, Pivotal SQLFire, Redis\nPaaS: Pivotal One, Pivotal CF, Pivotal Web Services, Cloud Foundry\nDATA TOOLS: Pivotal VRP, Pivotal Command Center\nANALYTICS: MADlib, Pivotal GPText\nAPPLICATIONS: Pivotal tc Server, Pivotal Web Server, Pivotal RabbitMQ, Spring, vFabric Suite",  
-    "talking_about_count": 77,  
-    "username": "pivotalsoftware",  
-    "website": "http://www.gopivotal.com",  
-    "were_here_count": 42  
- }  
这是关于一个公司的信息，类似的，如果你把最后的改为google，将返回：
![](https://img-blog.csdn.net/20140823194926435?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hlbmxvdmVpdA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**工具**
一个文本编辑器，JDK1.6及以上，Maven 3.0+或者Gradle 1.11+。（本文将使用Maven)
pom.xml清单：
**[html]**[view
 plain](http://blog.csdn.net/chenloveit/article/details/38780775#)[copy](http://blog.csdn.net/chenloveit/article/details/38780775#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <projectxmlns="http://maven.apache.org/POM/4.0.0"xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
- <modelVersion>4.0.0</modelVersion>
- 
- <groupId>org.springframework</groupId>
- <artifactId>gs-consuming-rest</artifactId>
- <version>0.1.0</version>
- 
- <parent>
- <groupId>org.springframework.boot</groupId>
- <artifactId>spring-boot-starter-parent</artifactId>
- <version>1.1.5.RELEASE</version>
- </parent>
- 
- <dependencies>
- <dependency>
- <groupId>org.springframework.boot</groupId>
- <artifactId>spring-boot-starter</artifactId>
- </dependency>
- <dependency>
- <groupId>org.springframework</groupId>
- <artifactId>spring-web</artifactId>
- </dependency>
- <dependency>
- <groupId>com.fasterxml.jackson.core</groupId>
- <artifactId>jackson-databind</artifactId>
- </dependency>
- </dependencies>
- 
- <properties>
- <start-class>hello.Application</start-class>
- </properties>
- 
- <build>
- <plugins>
- <plugin>
- <artifactId>maven-compiler-plugin</artifactId>
- <version>2.3.2</version>
- </plugin>
- <plugin>
- <groupId>org.springframework.boot</groupId>
- <artifactId>spring-boot-maven-plugin</artifactId>
- </plugin>
- </plugins>
- </build>
- 
- <repositories>
- <repository>
- <id>spring-releases</id>
- <url>http://repo.spring.io/libs-release</url>
- </repository>
- </repositories>
- 
- <pluginRepositories>
- <pluginRepository>
- <id>spring-releases</id>
- <url>http://repo.spring.io/libs-release</url>
- </pluginRepository>
- </pluginRepositories>
- 
- </project>
创建项目
首先你新建一个符合Maven规范的目录结构， src/main/java/hello。
**[plain]**[view
 plain](http://blog.csdn.net/chenloveit/article/details/38780775#)[copy](http://blog.csdn.net/chenloveit/article/details/38780775#)
- └── src  
-     └── main  
-         └── java  
-             └── hello  
如果直接通过浏览器获取上面的JSON数据是很拙劣的，更实用的方法是通过编程使用REST Web Service。为帮你完成这项工作，Spring提供了一个便利的模板类：RestTemplate。RestTemplate使得和大多数Restful风格Service的交互在一行内能够完成，并且能绑定到自定义的域类型。
笔者注：这里提到的一个观点是：Spring的便捷性之一体现在使用切面和模板减少代码，我们常用的JDBCTemplate就体现这一思想。
于是，我们新建一个类包含你所需要的信息，（假设你只想知道 名字、关于、电话、网站）
**[java]**[view
 plain](http://blog.csdn.net/chenloveit/article/details/38780775#)[copy](http://blog.csdn.net/chenloveit/article/details/38780775#)
- package hello;  
- 
- import com.fasterxml.jackson.annotation.JsonIgnoreProperties;  
- 
- @JsonIgnoreProperties(ignoreUnknown = true)  
- publicclass Page {  
- 
- private String name;  
- private String about;  
- private String phone;  
- private String website;  
- 
- public String getName() {  
- return name;  
-     }  
- 
- public String getAbout() {  
- return about;  
-     }  
- 
- public String getPhone() {  
- return phone;  
-     }  
- 
- public String getWebsite() {  
- return website;  
-     }  
- 
- }  
这里的@JsonIgnoreProperties是来自Jackson JSON包，指示忽略不在范围内的所有属性。
最后是Application类，
**[java]**[view
 plain](http://blog.csdn.net/chenloveit/article/details/38780775#)[copy](http://blog.csdn.net/chenloveit/article/details/38780775#)
- package hello;  
- 
- import org.springframework.web.client.RestTemplate;  
- 
- publicclass Application {  
- 
- publicstaticvoid main(String args[]) {  
-         RestTemplate restTemplate = new RestTemplate();  
-         Page page = restTemplate.getForObject("http://graph.facebook.com/pivotalsoftware", Page.class);  
-         System.out.println("Name:    " + page.getName());  
-         System.out.println("About:   " + page.getAbout());  
-         System.out.println("Phone:   " + page.getPhone());  
-         System.out.println("Website: " + page.getWebsite());  
-     }  
- 
- }  
因为Jackson JSON包在类加载路径，RestTemplate将使用它(通过一个消息转化器）来将JSON数据转成Page对象。（如果不使用Spring的这一特性，推荐使用FastJson完成对象的序列化和反序列化，这是阿里巴巴的一个项目）
目前，你只使用了RestTemplate的Http Get请求，但它实际上也支持HTTP的其他请求（如get、delete)
执行时和前一篇文章类似：
**[plain]**[view
 plain](http://blog.csdn.net/chenloveit/article/details/38780775#)[copy](http://blog.csdn.net/chenloveit/article/details/38780775#)
- mvn clean package  
然后，
**[plain]**[view
 plain](http://blog.csdn.net/chenloveit/article/details/38780775#)[copy](http://blog.csdn.net/chenloveit/article/details/38780775#)
- java -jar target/gs-consuming-rest-0.1.0.jar  
结果是在控制台打印结果：
![](https://img-blog.csdn.net/20140823201221937?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hlbmxvdmVpdA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
