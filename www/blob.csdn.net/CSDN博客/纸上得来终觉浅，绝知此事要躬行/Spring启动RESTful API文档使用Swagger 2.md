# Spring启动RESTful API文档使用Swagger 2 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年03月08日 17:28:54[boonya](https://me.csdn.net/boonya)阅读数：8307









原文地址：[https://dzone.com/articles/spring-boot-restful-api-documentation-with-swagger?edition=276883&utm_source=Daily%20Digest&utm_medium=email&utm_campaign=dd%202017-03-07](https://dzone.com/articles/spring-boot-restful-api-documentation-with-swagger?edition=276883&utm_source=Daily%20Digest&utm_medium=email&utm_campaign=dd%202017-03-07)

Spring Boot使开发RESTful服务变得非常容易 - 并且使用Swagger可以轻松地记录RESTful服务。

构建后端API层引入了一个全新的领域，超越了仅仅实现端点的挑战。您现在有客户端，现在将使用您的API。您的客户需要知道如何与您的API进行互动。在基于SOAP的Web服务中，您有一个WSDL可以使用。这为API开发人员提供了一个基于XML的合同，它定义了API。但是，对于RESTFul
 Web服务，没有WSDL。因此，您的API文档变得更加重要。

API文档应该结构化，使其具有信息性，简洁性和易于阅读。但是最佳实践，如何记录你的API，它的结构，什么包括和什么不是是一个不同的主题，我不会在这里覆盖。有关文档的最佳实践，我建议[由Andy
 Wikinson](https://www.infoq.com/presentations/doc-restful-api)进行此演示。

在本文中，我将介绍如何使用[Swagger 2](http://swagger.io/)为Spring Boot项目生成REST API文档。

## Swagger 2在Spring启动

Swagger 2是一个开源项目，用于描述和记录RESTful API。Swagger 2是语言无关的，可扩展到除HTTP之外的新技术和协议。当前版本定义了一组HTML，JavaScript和CSS资源，以便从符合Swagger的API动态生成文档。这些文件由Swagger
 UI项目捆绑，以在浏览器上显示API。除了渲染文档，[Swagger UI](http://swagger.io/swagger-ui/)允许其他API开发人员或用户与API的资源交互，而没有任何实现逻辑到位。

Swagger 2规范，被称为[OpenAPI规范](https://github.com/OAI/OpenAPI-Specification/blob/master/versions/2.0.md) ，有几个实现。目前，[Springfox](https://springfox.github.io/springfox/)已经取代了Swagger-SpringMVC（Swagger
 1.2及以上版本）是Spring Boot应用程序的常用选项。Springfox支持Swagger 1.2和2.0。

我们将在我们的项目中使用Springfox。

为了引入它，我们需要在我们的Maven POM中有下​​面的依赖声明。



```
<dependency>
    <groupId>io.springfox</groupId>
    <artifactId>springfox-swagger2</artifactId>
    <version>2.6.1</version>
    <scope>compile</scope>
</dependency>
```

除了Sprinfox，我们还需要Swagger UI。包含Swagger UI的代码是这样的。


```
<dependency>
    <groupId>io.springfox</groupId>
    <artifactId>springfox-swagger-ui</artifactId>
    <version>2.6.1</version>
    <scope>compile</scope>
</dependency>
```


## Spring启动RESTful应用程序

我们的应用程序实现了一组REST端点来管理产品。我们有一个产品JPA实体和一个名为`ProductRepository` ，它扩展了`CrudRepository` ，以便针对内存中的H2数据库执行产品的CRUD操作。

服务层由`ProductService`接口和`ProductServiceImpl`实现类组成。

Maven POM的应用程序是这样的。

`pom.xml` ：



```
<?xml version="1.0" encoding="UTF-8"?>
<project
	xmlns="http://maven.apache.org/POM/4.0.0"
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
	<modelVersion>4.0.0</modelVersion>
	<groupId>guru.springframework</groupId>
	<artifactId>spring-boot-web</artifactId>
	<version>0.0.1-SNAPSHOT</version>
	<packaging>jar</packaging>
	<name>Spring Boot Web Application</name>
	<description>Spring Boot Web Application</description>
	<parent>
		<groupId>org.springframework.boot</groupId>
		<artifactId>spring-boot-starter-parent</artifactId>
		<version>1.4.2.RELEASE</version>
		<relativePath/>
		<!-- lookup parent from repository -->
	</parent>
	<properties>
		<project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
		<java.version>1.8</java.version>
	</properties>
	<dependencies>
		<dependency>
			<groupId>org.springframework.boot</groupId>
			<artifactId>spring-boot-starter-data-rest</artifactId>
		</dependency>
		<dependency>
			<groupId>org.springframework.boot</groupId>
			<artifactId>spring-boot-starter-data-jpa</artifactId>
		</dependency>
		<dependency>
			<groupId>org.springframework.boot</groupId>
			<artifactId>spring-boot-starter-security</artifactId>
		</dependency>
		<dependency>
			<groupId>org.springframework.boot</groupId>
			<artifactId>spring-boot-starter-thymeleaf</artifactId>
		</dependency>
		<dependency>
			<groupId>org.springframework.boot</groupId>
			<artifactId>spring-boot-starter-web</artifactId>
		</dependency>
		<dependency>
			<groupId>com.jayway.jsonpath</groupId>
			<artifactId>json-path</artifactId>
			<scope>test</scope>
		</dependency>
		<dependency>
			<groupId>io.springfox</groupId>
			<artifactId>springfox-swagger-ui</artifactId>
			<version>2.6.1</version>
			<scope>compile</scope>
		</dependency>
		<dependency>
			<groupId>io.springfox</groupId>
			<artifactId>springfox-swagger2</artifactId>
			<version>2.6.1</version>
			<scope>compile</scope>
		</dependency>
		<!--WebJars-->
		<dependency>
			<groupId>com.h2database</groupId>
			<artifactId>h2</artifactId>
		</dependency>
		<dependency>
			<groupId>org.springframework.boot</groupId>
			<artifactId>spring-boot-starter-test</artifactId>
			<scope>test</scope>
		</dependency>
	</dependencies>
	<build>
		<plugins>
			<plugin>
				<groupId>org.springframework.boot</groupId>
				<artifactId>spring-boot-maven-plugin</artifactId>
			</plugin>
		</plugins>
	</build>
</project>
```



应用程序的控制器`ProductController`定义REST API端点。`ProductController`的代码是这样的：



```java
@RestController
@RequestMapping("/product")
public class ProductController {
    private ProductService productService;
    @Autowired
    public void setProductService(ProductService productService) {
        this.productService = productService;
    }
    @RequestMapping(value = "/list", method= RequestMethod.GET)
    public Iterable list(Model model){
        Iterable productList = productService.listAllProducts();
        return productList;
    }
    @RequestMapping(value = "/show/{id}", method= RequestMethod.GET)
    public Product showProduct(@PathVariable Integer id, Model model){
       Product product = productService.getProductById(id);
        return product;
    }
    @RequestMapping(value = "/add", method = RequestMethod.POST)
    public ResponseEntity saveProduct(@RequestBody Product product){
        productService.saveProduct(product);
        return new ResponseEntity("Product saved successfully", HttpStatus.OK);
    }
    @RequestMapping(value = "/update/{id}", method = RequestMethod.PUT)
    public ResponseEntity updateProduct(@PathVariable Integer id, @RequestBody Product product){
        Product storedProduct = productService.getProductById(id);
        storedProduct.setDescription(product.getDescription());
        storedProduct.setImageUrl(product.getImageUrl());
        storedProduct.setPrice(product.getPrice());
        productService.saveProduct(storedProduct);
        return new ResponseEntity("Product updated successfully", HttpStatus.OK);
    }
    @RequestMapping(value="/delete/{id}", method = RequestMethod.DELETE)
    public ResponseEntity delete(@PathVariable Integer id){
        productService.deleteProduct(id);
        return new ResponseEntity("Product deleted successfully", HttpStatus.OK);
    }
}
```



在这个控制器中，Spring 4.0中引入的`@RestController`注释将ProductController标记为REST API控制器。在引擎盖下， `@RestController`工作作为一个方便的注释用`@Controller`和`@ResponseBody`注释类。

`@RequestMapping`类级注释将请求映射到`/product`到`ProductController`类。方法级别`@RequestMapping`注释将Web请求映射到控制器的处理程序方法。



# 在应用程序中配置Swagger 2
对于我们的应用程序，我们将在Spring Boot配置中创建一个Docket bean，为应用程序配置Swagger 2。 Springfox Docket实例为主要的API配置提供了合理的默认值和方便的配置方法。 我们的Spring Boot配置类，SwaggerConfig就是这样。

```java
@Configuration
@EnableSwagger2
public class SwaggerConfig {
    @Bean
    public Docket productApi() {
        return new Docket(DocumentationType.SWAGGER_2)
                .select()                 .apis(RequestHandlerSelectors.basePackage("guru.springframework.controllers"))
                .paths(regex("/product.*"))
                .build();
    }
}
```




在此配置类中， `@EnableSwagger2`注释在类中启用Swagger支持。在Docket bean实例上调用的`select()`方法返回一个`ApiSelectorBuilder` ，它提供了用于过滤使用String谓词记录的控制器和方法的`apis()`和`paths()`方法。

在代码中， `RequestHandlerSelectors.basePackage`谓词匹配`guru.springframework.controllers`基础包以过滤API。传递给`paths()`的regex参数用作附加过滤器，以便仅对以`/product`开头的路径生成文档。

此时，您应该能够通过启动应用程序并将浏览器指向http：// localhost：8080 / v2 / api-docs来测试配置 。
![](https://img-blog.csdn.net/20170308165840626)



显然，Swagger 2为我们的端点生成的上述JSON转储不是我们想要的。


我们想要的是一些很好的人类可读的结构化文档，这是Swagger UI接管的地方。


在将浏览器指向http：// localhost：8080 / swagger-ui.html时，您将看到由Swagger UI呈现的生成文档，如下所示：

![](https://img-blog.csdn.net/20170308170027863)




如你所见，Swagger 2使用了合理的默认值来生成ProductController的`ProductController` 。

然后，Swagger UI包装一切，为我们提供一个直观的UI。这一切都是自动完成的。我们没有写任何代码或其他文档来支持Swagger。

## 自定义Swagger

到目前为止，我们一直在看Swagger文档，因为它开箱即用 - 但Swagger 2有一些伟大的自定义选项。

让我们开始通过在SwaggerConfig类中提供有关我们的API的信息来定制Swagger。

`SwaggerConfig.java` ：

```java
package guru.springframework.config;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import springfox.documentation.builders.RequestHandlerSelectors;
import springfox.documentation.service.ApiInfo;
import springfox.documentation.service.Contact;
import springfox.documentation.spi.DocumentationType;
import springfox.documentation.spring.web.plugins.Docket;
import springfox.documentation.swagger2.annotations.EnableSwagger2;
import static springfox.documentation.builders.PathSelectors.regex;
@Configuration
@EnableSwagger2
public class SwaggerConfig {
    @Bean
    public Docket productApi() {
        return new Docket(DocumentationType.SWAGGER_2)
                .select()
                .apis(RequestHandlerSelectors.basePackage("guru.springframework.controllers"))
                .paths(regex("/product.*"))
                .build()
                .apiInfo(metaData());
    }
    private ApiInfo metaData() {
        ApiInfo apiInfo = new ApiInfo(
                "Spring Boot REST API",
                "Spring Boot REST API for Online Store",
                "1.0",
                "Terms of service",
                new Contact("John Thompson", "https://springframework.guru/about/", "john@springfrmework.guru"),
               "Apache License Version 2.0",
                "https://www.apache.org/licenses/LICENSE-2.0");
        return apiInfo;
    }
}
```

在`SwaggerConfig`类中，我们添加了一个`metaData()`方法，该方法返回和使用有关我们的API的信息`ApiInfo`对象。第23行用新信息初始化Docket。

Swagger 2生成的文档现在看起来类似于：
![](https://img-blog.csdn.net/20170308170350321)


## Swagger 2 REST端点的注释
此时，如果单击产品控制器链接，Swagger UI将显示我们的操作端点的文档，如下所示：


![](https://img-blog.csdn.net/20170308170544401)

我们可以使用`ProductController`类上的`ProductController`来描述我们的API。



```java
@RestController @RequestMapping("/product") @Api(value="onlinestore", description="Operations pertaining to products in Online Store") public class ProductController {
```

Swagger UI生成的文档将反映描述，现在看起来像这样：
![](https://img-blog.csdn.net/20170308171054095)


对于每个操作端点，我们可以使用@ApiOperation注释来描述端点及其响应类型，如下所示：



```java
@ApiOperation(value = "View a list of available products", response = Iterable.class)
@RequestMapping(value = "/list", method= RequestMethod.GET,produces = "application/json")
public Iterable list(Model model){
    Iterable productList = productService.listAllProducts();
    return productList;
}
```
Swagger 2还允许覆盖HTTP方法的默认响应消息。除了常规的HTTP
 200 OK之外，您还可以使用`@ApiResponse`注释来记录其他响应，如下所示。


```java
@ApiOperation(value = "View a list of available products", response = Iterable.class)
@ApiResponses(value = {
        @ApiResponse(code = 200, message = "Successfully retrieved list"),
        @ApiResponse(code = 401, message = "You are not authorized to view the resource"),
        @ApiResponse(code = 403, message = "Accessing the resource you were trying to reach is forbidden"),
        @ApiResponse(code = 404, message = "The resource you were trying to reach is not found")
}
)
@RequestMapping(value = "/list", method= RequestMethod.GET, produces = "application/json")
public Iterable list(Model model){
    Iterable productList = productService.listAllProducts();
    return productList;
}
```


花了我一些时间的一个未记录的事情与响应内容类型的价值有关。Swagger 2生成`*/*` ，而我期待应用`application/json`响应内容类型。只有在更新了`@RequestMapping`注释（产生
```
=
 "application/json"
```
之后 ， `= "application/json"`生成所需的值。带注释的`ProductController`如下。

`ProductController.java` ：

```java
package guru.springframework.controllers;
import guru.springframework.domain.Product;
import guru.springframework.services.ProductService;
import io.swagger.annotations.Api;
import io.swagger.annotations.ApiOperation;
import io.swagger.annotations.ApiResponse;
import io.swagger.annotations.ApiResponses;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.*;
@RestController
@RequestMapping("/product")
@Api(value="onlinestore", description="Operations pertaining to products in Online Store")
public class ProductController {
    private ProductService productService;
    @Autowired
    public void setProductService(ProductService productService) {
        this.productService = productService;
    }
    @ApiOperation(value = "View a list of available products",response = Iterable.class)
    @ApiResponses(value = {
            @ApiResponse(code = 200, message = "Successfully retrieved list"),
            @ApiResponse(code = 401, message = "You are not authorized to view the resource"),
            @ApiResponse(code = 403, message = "Accessing the resource you were trying to reach is forbidden"),
            @ApiResponse(code = 404, message = "The resource you were trying to reach is not found")
    }
    )
    @RequestMapping(value = "/list", method= RequestMethod.GET, produces = "application/json")
    public Iterable<Product> list(Model model){
        Iterable<Product> productList = productService.listAllProducts();
        return productList;
    }
    @ApiOperation(value = "Search a product with an ID",response = Product.class)
    @RequestMapping(value = "/show/{id}", method= RequestMethod.GET, produces = "application/json")
    public Product showProduct(@PathVariable Integer id, Model model){
       Product product = productService.getProductById(id);
        return product;
    }
    @ApiOperation(value = "Add a product")
    @RequestMapping(value = "/add", method = RequestMethod.POST, produces = "application/json")
    public ResponseEntity saveProduct(@RequestBody Product product){
        productService.saveProduct(product);
        return new ResponseEntity("Product saved successfully", HttpStatus.OK);
    }
    @ApiOperation(value = "Update a product")
    @RequestMapping(value = "/update/{id}", method = RequestMethod.PUT, produces = "application/json")
    public ResponseEntity updateProduct(@PathVariable Integer id, @RequestBody Product product){
        Product storedProduct = productService.getProductById(id);
        storedProduct.setDescription(product.getDescription());
        storedProduct.setImageUrl(product.getImageUrl());
        storedProduct.setPrice(product.getPrice());
        productService.saveProduct(storedProduct);
        return new ResponseEntity("Product updated successfully", HttpStatus.OK);
    }
    @ApiOperation(value = "Delete a product")
    @RequestMapping(value="/delete/{id}", method = RequestMethod.DELETE, produces = "application/json")
    public ResponseEntity delete(@PathVariable Integer id){
        productService.deleteProduct(id);
        return new ResponseEntity("Product deleted successfully", HttpStatus.OK);
    }
}
```
浏览器上的操作端点的输出如下：
![](https://img-blog.csdn.net/20170308171512116)



当前文档缺少一件事：产品JPA实体的文档。我们将为我们的模型生成文档。

## Swagger 2注释模型

您可以使用`@ApiModelProperty`注释来描述Product模型的属性。使用`@ApiModelProperty` ，您还可以根据需要记录属性。

我们的产品类的代码是这样的。

`Product.java` ：

```java
package guru.springframework.domain;
import io.swagger.annotations.ApiModelProperty;
import javax.persistence.*;
import java.math.BigDecimal;
@Entity
public class Product {
    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @ApiModelProperty(notes = "The database generated product ID")
    private Integer id;
    @Version
    @ApiModelProperty(notes = "The auto-generated version of the product")
    private Integer version;
    @ApiModelProperty(notes = "The application-specific product ID")
    private String productId;
    @ApiModelProperty(notes = "The product description")
    private String description;
    @ApiModelProperty(notes = "The image URL of the product")
    private String imageUrl;
    @ApiModelProperty(notes = "The price of the product", required = true)
    private BigDecimal price;
    public String getDescription() {
        return description;
    }
    public void setDescription(String description) {
        this.description = description;
    }
    public Integer getVersion() {
        return version;
    }
    public void setVersion(Integer version) {
        this.version = version;
    }
    public Integer getId() {
        return id;
    }
    public void setId(Integer id) {
        this.id = id;
    }
    public String getProductId() {
        return productId;
    }
    public void setProductId(String productId) {
        this.productId = productId;
    }
    public String getImageUrl() {
        return imageUrl;
    }
    public void setImageUrl(String imageUrl) {
        this.imageUrl = imageUrl;
    }
    public BigDecimal getPrice() {
        return price;
    }
    public void setPrice(BigDecimal price) {
        this.price = price;
    }
}
```
Swagger 2为产品生成的文档是这样的：
![](https://img-blog.csdn.net/20170308171912624)




# 总结



除了使用Swagger Core和Swagger UI的REST API文档和演示外，Swagger 2还有许多其他用途，超出了本文的范围。我最喜欢的一个是[Swagger Editor](http://swagger.io/swagger-editor/)，一个工具来设计新的API或编辑现有的API。编辑器可视化呈现您的Swagger定义，并提供实时错误反馈。另一个是[Swagger
 Codegen](http://swagger.io/swagger-codegen/) ，一个代码生成框架，用于构建来自Swagger定义的客户端SDK，服务器和文档。

Swagger 2还支持通过JSON和YAML文件定义Swagger。如果你想通过在JSON和YAML文件中外部化它们来避免代码库中特定于实现的代码，那么你应该尝试一些东西 - 这将在以后的文章中介绍。

[此帖子的代码可以在这里下载](https://github.com/springframeworkguru/springboot_swagger_example)。








