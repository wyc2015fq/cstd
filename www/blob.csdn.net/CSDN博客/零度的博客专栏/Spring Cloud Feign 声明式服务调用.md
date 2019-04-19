# Spring Cloud Feign 声明式服务调用 - 零度的博客专栏 - CSDN博客
2018年06月14日 18:56:24[零度anngle](https://me.csdn.net/zmx729618)阅读数：71标签：[springcloud																[feign](https://so.csdn.net/so/search/s.do?q=feign&t=blog)](https://so.csdn.net/so/search/s.do?q=springcloud&t=blog)
个人分类：[spring boot](https://blog.csdn.net/zmx729618/article/category/7595492)

原文出处：http://www.cnblogs.com/li3807
在实际开发中，对于服务依赖的调用可能不止一处，往往一个接口会被多处调用，所以我们通常会针对各个微服务自行封装一些客户端类来包装这些依赖服务的调用，Spring Cloud Feign 在此基础上做了进一步的封装，由他来帮助我们定义和实现依赖服务接口的定义，我们只需要创建一个接口并用注解的方式来配置他，即可完成对服务提供方的接口绑定，简化了在使用 Spring Cloud Ribbon 时自行封装服务调用客户端的开发量。 
**快速入门**
- 
首先创建一个 Spring Cloud 的基础工程，并增加 spring-cloud-starter-eureka 依赖 和 spring-cloud-starter-feign 依赖，示例代码如下： 
<?xmlversion="1.0"encoding="UTF-8"?>
<projectxmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 [http://maven.apache.org/xsd/maven-4.0.0.xsd](http://maven.apache.org/xsd/maven-4.0.0.xsd)">
<modelVersion>**4.0.0**</modelVersion>
<groupId>org.lixue</groupId>
<artifactId>eureka-feign-consumer</artifactId>
<version>**0.0.1**-SNAPSHOT</version>
<packaging>jar</packaging>
<name>eureka-feign-consumer</name>
<description>Demo project for Spring Boot</description>
<parent>
<groupId>org.springframework.boot</groupId>
<artifactId>spring-boot-starter-parent</artifactId>
<version>**1.5.6**.RELEASE</version>
<relativePath/><!-- lookup parent from repository -->
</parent>
<properties>
<project.build.sourceEncoding>UTF-**8**</project.build.sourceEncoding>
<project.reporting.outputEncoding>UTF-**8**</project.reporting.outputEncoding>
<java.version>**1.8**</java.version>
<spring-cloud.version>Dalston.SR3</spring-cloud.version>
</properties>
<dependencies>
<dependency>
<groupId>org.springframework.cloud</groupId>
<artifactId>spring-cloud-starter-eureka</artifactId>
</dependency>
<dependency>
<groupId>org.springframework.cloud</groupId>
<artifactId>spring-cloud-starter-feign</artifactId>
</dependency>
<dependency>
<groupId>org.springframework.boot</groupId>
<artifactId>spring-boot-starter-test</artifactId>
<scope>test</scope>
</dependency>
</dependencies>
<dependencyManagement>
<dependencies>
<dependency>
<groupId>org.springframework.cloud</groupId>
<artifactId>spring-cloud-dependencies</artifactId>
<version>${spring-cloud.version}</version>
<type>pom</type>
<scope>import</scope>
</dependency>
</dependencies>
</dependencyManagement>
<build>
<plugins>
<plugin>
<groupId>org.springframework.boot</groupId>
<artifactId>spring-boot-maven-plugin</artifactId>
</plugin>
</plugins>
</build>
</project> 
- 
创建应用主类 EurekaFeignConsumerApplication 并通过 @EnableFeignClients 注解开启 Spring Cloud Feign 功能，使用 @EnableDiscoveryClient 注解开启 Eureka的服务发现，代码如下： 
@EnableFeignClients 
@EnableDiscoveryClient 
@SpringBootApplication 
public class EurekaFeignConsumerApplication { 
public staticvoidmain(String[] args) { 
                SpringApplication.run(EurekaFeignConsumerApplication.class, args); 
        } 
} 
- 
定义 HelloWorldService 接口，通过 @FeignClient 注解指定服务名来绑定服务，然后再使用Spring MVC 的注解来绑定具体该服务提供的 REST 接口，代码如下： 
@FeignClient ("ORG.LIXUE.HELLOWORLD") 
public interface HelloWorldService { 
@RequestMapping ("/hi") 
        String hi(); 
} 
- 
在需要调用服务的位置，使用 @Autowired直接注入HelloWorldService 实例，并使用该接口实例调用方法 hi 来完成 /hi 接口的调用，示例代码如下： 
@RestController 
public class FeignConsumerController { 
@Autowired
        HelloWorldService helloWorldService; 
@RequestMapping ("/hi") 
public String hi() { 
return helloWorldService.hi(); 
        } 
} 
- 
在 application.yml 中需要指定服务注册中心，并定义自身的服务名和端口，示例如下： 
server: 
    port: **9200**
eureka: 
    client: 
        service-url: 
            defaultZone: [http://eurekaserver2:**9002**/eureka,http://eurekaserver1:**9001**/eureka](http://eurekaserver2:9002/eureka,http:/eurekaserver1:9001/eureka)
spring: 
    application: 
        name: eureka-feign-consumer 
- 启动服务注册中心以及二个ORG.LIXUE.HELLOWORLD服务，然后启动 eureka-feign-consumer ，此时发送几次 GET 请求到 [http://localhost:9200/hi](http://localhost:9200/hi) ，可以正确的返回 Hello World hi 9100 或者 Hello World hi 9101 ，可以看到Feign实现的消费者，依然是利用 Ribbon 维护了针对 ORG.LIXUE.HELLOWORLD服务列表信息，并且通过轮询实现了客户端负载均衡。 
**参数绑定**
在入门示例中我们实现的是一个不带参数的REST服务绑定，然而现实系统中的各种业务接口要比他复杂很多，我们会在HTTP的各个位置传入各种不同类型的参数，并且在返回请求响应的时候也可能是一个复杂对象结构，因此我们需要使用Feign来对不同形式的参数进行绑定 
- 
**@RequestParam** 注解：常用来处理简单类型的绑定，通过Request.getParameter() 获取的String可直接转换为简单类型的情况（ String--> 简单类型的转换操作由ConversionService配置的转换器来完成）；因为使用request.getParameter()方式获取参数，所以可以处理get 方式中queryString的值，也可以处理post方式中 body data的值；用来处理Content-Type: 为 application/x-www-form-urlencoded编码的内容，提交方式GET、POST；该注解有两个属性： value、required； value用来指定要传入值的id名称，required用来指示参数是否必须绑定 
@RequestMapping (value = "/hi1", method = RequestMethod.GET) 
public String hi(@RequestParam ("name") String name) { 
return"Hello World hi " + port + " name " + name; 
} 
- 
**@CookieValue**注解：可以把Request header中关于cookie的值绑定到方法的参数上 
@RequestMapping("/displayHeaderInfo.do")   
publicvoiddisplayHeaderInfo(@CookieValue("JSESSIONID") String cookie)  {     
//...   
}  
- 
**@RequestBody** 注解：用于读取Request请求的body部分数据，使用系统默认配置的HttpMessageConverter进行解析，然后把相应的数据绑定到要返回的对象上 
@RequestMapping (value = "/hi2", method = RequestMethod.POST) 
public String hi(@RequestBody User user) { 
return"Hello World hi " + port + "\tUser=" + user; 
} 
注意：User 类必须有默认构造函数 
- 
**@RequestHeader** 注解：可以把Request请求header部分的值绑定到方法的参数上 
@RequestMapping("/displayHeaderInfo.do")   
publicvoiddisplayHeaderInfo(@RequestHeader("Accept-Encoding") String encoding,   
@RequestHeader("Keep-Alive") long keepAlive)  {    
//...   
} 
- 
**@PathVariable** 注解：当使用@RequestMapping URI template 样式映射时， 即 someUrl/{paramId}, 这时的paramId可通过 @Pathvariable注解绑定它传过来的值到方法的参数上，示例代码如下： 
@Controller
@RequestMapping("/owners/{ownerId}")   
publicclass RelativePathUriTemplateController {   
@RequestMapping("/pets/{petId}")   
publicvoidfindPet(@PathVariable String ownerId, @PathVariable String petId, Model model) {       
// implementation omitted   
  }   
} 
**继承特性**
在SpringCloudFeign 中，提供了继承特性来帮助我们构建相应的服务客户端不安定接口和服务控制器，进一步减少编码量，示例如下： 
- 
创建一个基础的 Maven 工程，命名为service-contract，由于需要使用到 Spring MVC 的注解，因此在 pom.xml 中引入 spring-boot-starter-web 依赖，具体内容如下： 
<?xmlversion="1.0"encoding="UTF-8"?>
<projectxmlns="http://maven.apache.org/POM/4.0.0"
                  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 [http://maven.apache.org/xsd/maven-4.0.0.xsd](http://maven.apache.org/xsd/maven-4.0.0.xsd)">
<modelVersion>**4.0.0**</modelVersion>
<groupId>org.lixue</groupId>
<artifactId>service-contract</artifactId>
<version>**0.0.1**-SNAPSHOT</version>
<packaging>jar</packaging>
<name>service-contract</name>
<description>Demo project for Spring Boot</description>
<parent>
<groupId>org.springframework.boot</groupId>
<artifactId>spring-boot-starter-parent</artifactId>
<version>**1.5.6**.RELEASE</version>
<relativePath/><!-- lookup parent from repository -->
</parent>
<properties>
<project.build.sourceEncoding>UTF-**8**</project.build.sourceEncoding>
<project.reporting.outputEncoding>UTF-**8**</project.reporting.outputEncoding>
<java.version>**1.8**</java.version>
<spring-cloud.version>Dalston.SR3</spring-cloud.version>
</properties>
<dependencies>
<dependency>
<groupId>org.springframework.boot</groupId>
<artifactId>spring-boot-starter-web</artifactId>
</dependency>
</dependencies>
</project> 
- 
在项目中创建服务需要使用的实体类和服务接口，代码如下： 
public class User { 
private String name; 
privateint age; 
// setter和getter方法         
@Override
public String toString() { 
return"name=" + name + "\tage=" + age; 
        } 
} 
@RequestMapping("/hello") 
public interface HelloWorldService { 
@RequestMapping (value = "/hi", method = RequestMethod.GET) 
        String hi(); 
@RequestMapping (value = "/hi1", method = RequestMethod.GET) 
        String hi(@RequestParam ("name") String name); 
@RequestMapping (value = "/hi2", method = RequestMethod.POST) 
        String hi(@RequestBody User user); 
} 
- 
在服务的具体实现项目中增加 service-contract 的依赖，并且实现服务需要继承上面的 HelloWorldService  接口，并重写接口方法实现具体的服务实现 
@RestController 
public class HelloWorldController implements HelloWorldService { 
@Value ("${server.port}") 
int port; 
@Override
public String hi() { 
return"hi port " + port; 
        } 
@Override
public String hi(String name) { 
return"hi port " + port + " name " + name; 
        } 
@Override
public String hi(@RequestBody User user) { 
return"hi port " + port + " user " + user; 
        } 
} 
- 
在具体的消费项目中增加 service-contract 依赖增加接口HelloWorldServiceProxy ，继承 service-contract 项目中的HelloWorldService并使用 @FeignClient 注解来声明调用服务名称 
@FeignClient ("ORG.LIXUE.HELLOWORLD") 
public interface HelloWorldServiceProxy extends HelloWorldService { 
} 
- 
在需要调用服务的位置，使用 @Autowired直接注入HelloWorldServiceProxy 实例，并使用该接口实例调用方法 hi 来完成 /hi 接口的调用，示例代码如下： 
@RestController 
public class FeignConsumerController { 
@Autowired
        HelloWorldService helloWorldService; 
@RequestMapping ("/hi") 
public String hi() {                 
User user = newUser(); 
user.setName("liyong"); 
user.setAge(**3434**); 
StringBuilder stringBuilder = newStringBuilder(); 
stringBuilder.append("hi=" + helloWorldServiceProxy.hi()).append("<br/>"); 
stringBuilder.append("hi1=" + helloWorldServiceProxy.hi("lixue")).append("<br/>"); 
stringBuilder.append("hi2=" + helloWorldServiceProxy.hi(user)).append("<br/>"); 
return stringBuilder.toString(); 
 } 
} 
- 
在 application.yml 中需要指定服务注册中心，并定义自身的服务名和端口，示例如下： 
server: 
    port: **9200**
eureka: 
    client: 
        service-url: 
            defaultZone: [http://eurekaserver2:**9002**/eureka,http://eurekaserver1:**9001**/eureka](http://eurekaserver2:9002/eureka,http:/eurekaserver1:9001/eureka)
spring: 
    application: 
        name: eureka-feign-consumer 
- 
启动服务注册中心以及二个ORG.LIXUE.HELLOWORLD服务，然后启动 eureka-feign-consumer ，此时发送几次 GET 请求到 [http://localhost:9200/hi](http://localhost:9200/hi) ，可以正确的返回 Hello World hi 9100 或者 Hello World hi 9101 ，可以看到Feign实现的消费者，依然是利用 Ribbon 维护了针对 ORG.LIXUE.HELLOWORLD服务列表信息，并且通过轮询实现了客户端负载均衡。 
