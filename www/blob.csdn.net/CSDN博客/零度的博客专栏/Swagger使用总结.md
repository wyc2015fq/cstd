# Swagger使用总结 - 零度的博客专栏 - CSDN博客
2018年07月06日 16:03:38[零度anngle](https://me.csdn.net/zmx729618)阅读数：653

原文出处：https://www.cnblogs.com/h9527/p/5506956.html
#### 1. Swagger是什么？
官方说法：Swagger是一个规范和完整的框架，用于生成、描述、调用和可视化 RESTful 风格的 Web 服务。总体目标是使客户端和文件系统作为服务器以同样的速度来更新。文件的方法，参数和模型紧密集成到服务器端的代码，允许API来始终保持同步。
个人觉得，swagger的一个最大的优点是能实时同步api与文档。在项目开发过程中，发生过多次：修改代码但是没有更新文档，前端还是按照老旧的文档进行开发，在联调过程中才发现问题的情况（当然依据开闭原则，对接口的修改是不允许的，但是在项目不稳定阶段，这种情况很难避免）。
#### 2. spring boot 集成 Swagger
目前维护的系统是基于springboot框架开发的，因此本文会详细描述springboot与swagger集成的过程。
**spring-boot系统集成swagger需要进行如下操作：**
- 
添加maven依赖，需要在系统的pom中添加如下依赖：
```
<dependency>
    <groupId>io.springfox</groupId>
    <artifactId>springfox-swagger2</artifactId>
    <version>2.2.2</version>
 </dependency>
```
- 
添加swagger配置文件，配置文件如下：
```java
@Configuration
@EnableSwagger2
public class SwaggerConfig {
    @Bean
    public Docket api() {
        return new Docket(DocumentationType.SWAGGER_2)
                .select()  // 选择那些路径和api会生成document
                .apis(RequestHandlerSelectors.any()) // 对所有api进行监控
                .paths(PathSelectors.any()) // 对所有路径进行监控
                .build();
    }
}
```
通过注解EnableSwagger2声明Swagger的可用性，此处会定义一个类型为Docket的bean，
 关于docket类的说明如下：
> 
A builder which is intended to be the primary interface into the swagger-springmvc framework.Provides sensible defaults and convenience methods for configuration.
Docket的select()方法会提供给swagger-springmvc framework的一个默认构造器（ApiSelectorBuilder），这个构造器为配置swagger提供了一系列的默认属性和便利方法。
- 
测试
通过访问：[http://localhost:8080/your-app-root/v2/api-docs](http://localhost:8080/your-app-root/v2/api-docs) ，能测试生成的api是否可用。此时返回的是一个json形式的页面，可读性不好。可以通过Swagger UI来生成一个可读性良好的api页面。具体做法就是，在pom中添加相关依赖。如下：
```
<dependency>
    <groupId>io.springfox</groupId>
    <artifactId>springfox-swagger-ui</artifactId>
    <version>2.2.2</version>
</dependency>
```
再次访问：[http://localhost:8080/your-app-root/swagger-ui.html](http://localhost:8080/your-app-root/swagger-ui.html) 就可以看到可读性较好的api文档页面。
- 
注意：
- http://localhost:8080/your-app-root/v2/api-docs中**your-app-root**指的你的wabapp的根路径，我目前的webapp就默认在根路径下，所以地址是：[http://localhost:8080/v2/api-docs](http://localhost:8080/v2/api-docs)
- spring-mvc上引用swagger需要做其他相关的配置，具体请查看[参考文献](http://www.baeldung.com/swagger-2-documentation-for-spring-rest-api)
- swagger对restful风格的api支持的比较好，非restful风格的api支持的不是很好，对于非restful风格的api或者其他语言（非java语言）可以采用 [http://editor.swagger](http://editor.swagger.io/#/) 编辑器来收工完成相关的API编写
*参考文献*
- [swagger-2-documentation-for-spring-rest-api](http://www.baeldung.com/swagger-2-documentation-for-spring-rest-api)
