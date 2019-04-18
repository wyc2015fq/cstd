# 使用 Spring Boot 快速构建应用服务 - z69183787的专栏 - CSDN博客
2015年06月16日 21:00:14[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5066
首先介绍一下Spring Boot在Coding上的使用场景。Coding 中使用 Spring Boot 搭建的**Email** 服务。
**之所以选择Spring Boot,原因就是更加轻量级。在往常的Spring项目中,依赖的库太多,配置太繁杂，用在这只提供Email服务的程序上未免也小题大做了。而Spring Boot提供一些非功能性的常见的大型项目类特性（如内嵌服务器、安全、度量、健康检查、外部化配置）使得我们在部署上也更为方便，如可以直接地内嵌Tomcat/Jetty（不需要单独去部署war包）**
废话了这么多，就来介绍下怎么使用Spring Boot（简单demo，深入学习参考官方文档）
Java Maven这两个环境可少不了
**安装**
在pom.xml中添加
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- <parent>  
-     <groupId>org.springframework.boot</groupId>  
-     <artifactId>spring-boot-starter-parent</artifactId>  
-     <version>1.1.8.RELEASE</version>  
- </parent>  
- <dependencies>  
-     <dependency>  
-         <groupId>org.springframework.boot</groupId>  
-         <artifactId>spring-boot-starter-web</artifactId>  
-     </dependency>  
- </dependencies>  
如项目想添加更多的依赖自行添加即可
**搭建Demo**
添加启动入口文件
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- @EnableWebMvc
- @Configuration
- @EnableAutoConfiguration
- @ComponentScan
- publicclass Application extends WebMvcConfigurerAdapter {  
- 
- // dataSource这里使用的是Hikari,你也可以使用其他的
- @Bean
- public DataSource dataSource() {  
-         HikariConfig config = new HikariConfig(getClass().getClassLoader().getResource("db.properties").getPath());  
- returnnew HikariDataSource(config);  
-     }  
- 
- // 用于处理编码问题
- @Bean
- public Filter characterEncodingFilter() {  
-         CharacterEncodingFilter characterEncodingFilter = new CharacterEncodingFilter();  
-         characterEncodingFilter.setEncoding("UTF-8");  
-         characterEncodingFilter.setForceEncoding(true);  
- return characterEncodingFilter;  
-     }  
- publicstaticvoid main(String[] args) throws Exception {  
-         SpringApplication.run(Application.class, args);  
-     }  
- }  
添加Controller
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- @ResponseBody
- @RestController
- publicclass EmailController {  
- @Autowired
- private EmailService emailService;  
- 
- @ResponseBody
- @RequestMapping(value = "/sendMail", method = RequestMethod.POST,produces = MediaType.APPLICATION_JSON_VALUE)  
- publicboolean sendMail(String title, String content, String email) {  
- returntrue;  
- }  
- }  
添加Service
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- @Component
- 
- // 用于读取配置文件的信息,默认从resources目录下的application.properties文件中读取
- @Value("${EmailQueueListener.enabled}")  
- publicboolean EmailQueueListenerEnbaled;  
- 
- @PostConstruct
- publicvoid init() {  
- // 初始化一些服务,例如队列服务等
-      }  
- }  
- public List<String> sendMail(String title, String body, String... to) {  
- returnnull;  
-   }  
- }  
只要对Spring有过一定了解的上手Spring Boot绝对很快。
搭建好之后可以使用 mvn spring-boot:run 启动服务,Tomcat之类的都不需要装。
