# SpringMVC之四：渲染Web视图 - weixin_33985507的博客 - CSDN博客
2014年06月23日 11:38:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
## 理解视图解析
在前面的例子中，我们看到控制器返回的都是一个**逻辑视图的名称**，然后把这个逻辑视图名称交给**view resolver**，然后返回渲染后的 html 页面给 client。
将控制器中请求处理的逻辑和视图中的渲染实现解耦是Spring MVC的一个重要特性。如果控 制器中的方法直接负责产生HTML的话，就很难在不影响请求处理逻辑的前提下，维护和更新 视图。控制器方法和视图的实现会在模型内容上达成一致，这是两者的最大关联，除此之外， 两者应该保持足够的距离。但是，如果控制器只通过逻辑视图名来了解视图的话，那Spring该如何确定使用哪一个视图实 现来渲染模型呢？这就是Spring视图解析器的任务了。
优点：通过在控制器和视图之间传递 `model` 数据，可以使代码分离，逻辑清晰，更利于维护等优点。
Spring MVC 定义了一个 `ViewResolver` 的接口：
```
package org.springframework.web.servlet;
import java.util.Locale;
public interface ViewResolver {
    View resolveViewName(String viewName, Locale locale) throws Exception;
}
```
`ViewResolver` 方法，当提供一个 `viewName` 和 `locale` 以后，返回一个 `View` 实体。`View` 是另外一个接口：
```
package org.springframework.web.servlet;
public interface View {
    String getContentType();
    void render(Map<String, ?> model, HttpServletRequest request, HttpServletResponse response) throws Exception;
}
```
`View` 接口的作用就是利用 model 数据，还有 request 和 response 对象渲染视图内容，然后返回给 response。
当然你在实际使用的过程中不会接触到这些内部的接口，因为 Spring 提供了很多视图技术的支持：`FreeMarkerViewResolver`, `InternalResourceViewResolver`, `VelocityViewResolver`等。
![](https://images2017.cnblogs.com/blog/285763/201712/285763-20171230195028413-291117649.png)
Spring 4和Spring 3.2支持表中的所有视图解析器。Spring 3.1支持除Tiles 3 TilesViewResolver之外的所有视图解析器。
对于表中的大部分视图解析器来讲，每一项都对应Java Web应用中特定的某种视图技术。InternalResourceViewResolver一般会用于JSP，TilesViewResolver用于 Apache Tiles视图，而FreeMarkerViewResolver和VelocityViewResolver分别对应 FreeMarker和Velocity模板视图。
我们将会关注与大多数Java开发人员最息息相关的视图技术。因为大多数Java Web 应用都会用到JSP，我们首先将会介绍InternalResourceViewResolver，这个视图解析 器一般会用来解析JSP视图。接下来，我们将会介绍TilesViewResolver，控制JSP页面的布 局。
## 二、InternalResourceViewResolver解析JSP视图
Spring提供了两种支持JSP视图的方式：
- InternalResourceViewResolver会将视图名解析为JSP文件。另外，如果在你的JSP 页面中使用了JSP标准标签库（JavaServer Pages Standard Tag Library，JSTL）的 话，InternalResourceViewResolver能够将视图名解析为JstlView形式的JSP文件， 从而将JSTL本地化和资源bundle变量暴露给JSTL的格式化（formatting）和信息（message）标 签。
- Spring提供了两个JSP标签库，一个用于表单到模型的绑定，另一个提供了通用的工具类 特性。
不管你使用JSTL，还是准备使用Spring的JSP标签库，配置解析JSP的视图解析器都是非常重要 的。尽管Spring还有其他的几个视图解析器都能将视图名映射为JSP文件，但就这项任务来 讲，InternalResourceViewResolver是最简单和最常用的视图解析器。
### 2.1、配置适用于JSP的视图解析器
有一些视图解析器，如ResourceBundleViewResolver会直接将逻辑视图名映射为特定 的View接口实现，而InternalResourceViewResolver所采取的方式并不那么直接。它 遵循一种约定，会在视图名上添加前缀和后缀，进而确定一个Web应用中视图资源的物理路径。
作为样例，考虑一个简单的场景，假设逻辑视图名为home。通用的实践是将JSP文件放到Web 应用的WEB-INF目录下，防止对它的直接访问。如果我们将所有的JSP文件都放在“/WEBINF/views/”目录下，并且home页的JSP名为home.jsp，那么我们可以确定物理视图的路径就是 逻辑视图名home再加上“/WEB-INF/views/”前缀和“.jsp”后缀。如图所示。
## ![](https://images2017.cnblogs.com/blog/285763/201712/285763-20171230201212992-1923999032.png)
 当使用@Bean注解的时候，我们可以按照如下的方式配置Internal-ResourceView Resolver，使其在解析视图时，遵循上述的约定。
```
// 配置JSP视图解析器
    @Bean
    public ViewResolver viewResolver() {
        InternalResourceViewResolver resolver = new InternalResourceViewResolver();
        resolver.setPrefix("/WEB-INF/views/");
        resolver.setSuffix(".jsp");
        return resolver;
    }
```
作为替代方案，如果你更喜欢使用基于XML的Spring配置，那么可以按照如下的方式配 置InternalResourceViewResolver：
![](https://images2017.cnblogs.com/blog/285763/201712/285763-20171230201552648-562762864.png)
InternalResourceViewResolver配置就绪之后，它就会将逻辑视图名解析为JSP文件， 如下所示：
- home将会解析为“/WEB-INF/views/home.jsp”
- productList将会解析为“/WEB-INF/views/productList.jsp”
- books/detail将会解析为“/WEB-INF/views/books/detail.jsp”
### 2.2、解析JSTL视图
到目前为止，我们对InternalResourceViewResolver的配置都很基础和简单。它最终会将逻辑视图名解析为InternalResourceView实例，这个实例会引用JSP文件。但是如果这 些JSP使用JSTL标签来处理格式化和信息的话，那么我们会希 望InternalResourceViewResolver将视图解析为JstlView。 JSTL的格式化标签需要一个Locale对象，以便于恰当地格式化地域相关的值，如日期和货币。信息标签可以借助Spring的信息资源和Locale，从而选择适当的信息渲染到HTML之中。 通过解析JstlView，JSTL能够获得Locale对象以及Spring中配置的信息资源。 如果想让InternalResourceViewResolver将视图解析为JstlView，而不是InternalResourceView的话，那么我们只需设置它的viewClass属性即可：
![](https://images2017.cnblogs.com/blog/285763/201712/285763-20171230201842929-1318869532.png)
同样，我们也可以使用XML完成这一任务：
![](https://images2017.cnblogs.com/blog/285763/201712/285763-20171230201832179-835078326.png)
 不管使用Java配置还是使用XML，都能确保JSTL的格式化和信息标签能够获得Locale对象 以及Spring中配置的信息资源。
使用Spring的JSP库
当为JSP添加功能时，标签库是一种很强大的方式，能够避免在脚本块中直接编写Java代码。 Spring提供了两个JSP标签库，用来帮助定义Spring MVC Web的视图。其中一个标签库会用来渲染HTML表单标签，这些标签可以绑定model中的某个属性。另外一个标签库包含了一些工 具类标签，我们随时都可以非常便利地使用它们。
　　在这两个标签库中，你可能会发现表单绑定的标签库更加有用。所以，我们就从这个标签库开 始学习Spring的JSP标签。我们将会看到如何将Spittr应用的注册表单绑定到模型上，这样表单 就可以预先填充值，并且在表单提交失败后，能够展现校验错误。 将表单绑定到模型上 Spring的表单绑定JSP标签库包含了14个标签，它们中的大多数都用来渲染HTML中的表单标 签。但是，它们与原生HTML标签的区别在于它们会绑定模型中的一个对象，能够根据模型中 对象的属性填充值。标签库中还包含了一个为用户展现错误的标签，它会将错误信息渲染到 最终的HTML之中。
## 异常处理
在应用中抛出异常，最后还是需要写入到 response 中，Spring 提供如下方式将**异常转化为 response**：
- 特定的 Spring 异常自动映射为 HTTP 状态码
- 异常可映射为 HTTP 状态码通过在异常上使用 `@ResponseStatus` 注解
- 一个方法可用来处理异常通过在其上使用 `@ExceptionHandler` 注解
### 异常映射 HTTP 状态码
通过 `@ResponseStatus` 注解将异常和 HTTP 状态码对应：
```java
package spittr.web;
import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.ResponseStatus;
@ResponseStatus(value=HttpStatus.NOT_FOUND, 
            reason="Spittle Not Found")
public class SpittleNotFoundException extends RuntimeException {
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
现在只需要在控制器中抛出异常，就会被映射为指定的 HTTP 状态码：
```java
@RequestMapping(value="/{spittleId}", method=RequestMethod.GET)
public String spittle(@PathVariable("spittleId") long spittleId,
        Model model) {
    Spittle spittle = spittleRepository.findOne(spittleId);
    if (spittle == null) {
        throw new SpittleNotFoundException();
    }
    model.addAttribute(spittle);
    return "spittle";
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
### 使用 exception-handling 方法
映射异常到 HTTP 状态码的方式**简单**、**高效**，但是如果需要返回更多的信息就不行了（比如返回一个 view 视图）。所以，Spring 提供了 `@ExceptionHandler` 注解，可像处理请求那样处理异常。
```java
@RequestMapping(method=RequestMethod.POST)
public String saveSpittle(SpittleForm form, Model model) {
  try {
    spittleRepository.save(new Spittle(null, form.getMessage(), new Date(), 
        form.getLongitude(), form.getLatitude()));
    return "redirect:/spittles";
  } catch (DuplicateSpittleException e) {
    return "error/duplicate";
  }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
如上，我们在控制器中既包含了业务处理代码，也包含了异常处理代码。使用 `@ExceptionHandler` 就可以让你专注于业务逻辑代码，而在另外的方法中专门处理异常。
```java
@ExceptionHandler(DuplicateSpittleException.class)
public String handleDuplicateSpittle() {
    return "error/duplicate";
}
```
- 1
- 2
- 3
- 4
注意，异常处理的方法必须放在会抛出该异常的控制器类中才行，也就是说只能捕获当前控制器类抛出的指定的异常。该异常处理返回了一个逻辑视图的名称，Spring 会根据这个名称返回相应的 html 页面。通过 `@ExceptionHandler` 注解，我们可以定义一个方法处理所有该控制器中任意 handler 抛出的 `DuplicateSpittleException` 异常，简化了代码。
```java
// 新的业务逻辑 handler
@RequestMapping(method=RequestMethod.POST)
public String saveSpittle(SpittleForm form, Model model) {
    spittleRepository.save(
        new Spittle(null, form.getMessage(), new Date(),
            form.getLongitude(), form.getLatitude()));
    return "redirect:/spittles";
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
但是，需要为每个控制器都定义一个 `@ExceptionHandler` 方法是不是有点重复。所以，一般情况下会把 exception-handler 方法放在 BaseController，或者使用 `@ControllerAdvice`。
### Advising controllers
为了使某些方法（如 exception-handler 方法）在全部的控制器中都能够发挥作用，Spring 3.2 引入了 **controller advice**。*Controller advice* 是一个被 `@ControllerAdvice` 注解的类，它包含一个或多个如下类型的方法：
- 被 `@ExceptionHandler` 注解的方法
- 被 `@InitBinder` 注解的方法
- 被 `@ModelAttribute` 注解的方法
在被 `@ControllerAdvice` 注解的类中的这些方法能够应用到所有被 `@RequestMapping` 注解的方法。
因为，`@ControllerAdvice` 本身被 `@Component` 注解，所以能够被 component-scan 扫描被注入，就像 `@Controller` 注解一样。
如下，就是使用 `@ControllerAdvice` 为所有的控制器定义 `@ExceptionHandler` 异常处理方法。
```java
package spitter.web;
import org.springframework.web.bind.annotation.ControllerAdvice;
import org.springframework.web.bind.annotation.ExceptionHandler;
@ControllerAdvice
public class AppWideExceptionHandler {
    @ExceptionHandler(DuplicateSpittleException.class)
    public String duplicateSpittleHandler() {
        return "error/duplicate";
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
现在，然后被 `@RequestMapping` 注解的控制器方法如果抛出 `DuplicateSpittleException` 异常，都可以被该方法捕获到，从而进行处理。
## 面向资源的控制器
因为 Javascript 在客户端的大量使用，现在服务器端很多时候需要传回 XML 或 JSON 格式的数据，那么 Spring MVC 如何返回不同类型的数据，而不是返回 html 页面呢？
- Content negotiation：一种把 model 数据渲染成客户端需要的格式的视图
- Message conversion：能够把控制器返回的对象转换为 client 需要的格式的转换器，没有了视图渲染的环节
因为，Content negotiation 只能转换 model 数据，而 model 本质上来说是 map 类型的数据格式。所以，转换后的数据可能并不是理想的 client 需要的格式。出于这个原因，我们倾向于使用 Message conversion。
### 使用 HTTP message converters
Spring MVC 自带多种 message converters：`Jaxb2RootElementHttpMessageConverter`, `MappingJacksonHttpMessageConverter`, `MappingJackson2HttpMessageConverter`, `ResourceHttpMessageConverter`, 其他的 converters 请参考官方文档 [HTTP Message Conversion](http://docs.spring.io/spring/docs/current/spring-framework-reference/html/remoting.html#rest-message-conversion)。
那么如何使用这些 converters：
- 向 client 发送数据：根据 request’s `Accept` header 确定
- 从 client 接收数据：根据 `Content-Type` header 确定
很多 converter 都是默认注册的，所以你不需要格外的配置，但是可能需要添加**额外的依赖**到项目的 classpath 中以便使用这些 converters。比如，如果需要使用 `MappingJacksonHttpMessageConverter` 在 JSON messages 和 Java 对象之间相互转换，你需要添加 Jackson JSON Processor 依赖到 classpath 中。自定义 Jackson converter 参考 [Latest Jackson integration improvements in Spring](https://spring.io/blog/2014/12/02/latest-jackson-integration-improvements-in-spring)。
#### 自定义 Message converters
除了使用 Spring MVC 默认提供的 message converters 外，我们还可以自定义 converter。比如自定义 `MappingJackson2HttpMessageConverter`，让其可以处理其他的 media types。
```java
@Configuration
@EnableWebMvc
@ComponentScan("org.acherie.demo.web")
public class WebConfig extends WebMvcConfigurerAdapter {
    ...
    @Override
    public void configureMessageConverters(List<HttpMessageConverter<?>> converters) {
        converters.add(jacksonConverter());
    }
    @Bean
    public MappingJackson2HttpMessageConverter jacksonConverter() {
        List<MediaType> mediaTypes = new ArrayList<>();
        mediaTypes.add(MediaType.APPLICATION_XML);
        MappingJackson2HttpMessageConverter converter = 
                new MappingJackson2HttpMessageConverter();
        converter.setSupportedMediaTypes(mediaTypes);
        return converter;
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
#### `@ResponseBody` 返回资源
如果你需要返回 JSON 或 XML 到 client，你需要告诉 Spring 略过通常的 model/view 流程，并且使用 message converter。而这个东西就是 `@ResponseBody` 注解：
```java
@RequestMapping(method=RequestMethod.GET, produces="application/json")
public @ResponseBody List<Spittle> spittles(
    @RequestParam(value="max", defaultValue=MAX_LONG_AS_STRING) long max,
    @RequestParam(value="count", defaultValue="20") int count) {
  return spittleRepository.findSpittles(max, count);
}
```
- 1
- 2
- 3
- 4
- 5
- 6
`@ResponseBody` 告诉 Spring 你想将控制器返回的 Java object 转换（使用 converter）为资源返回 client。更近一步，`DispatcherServlet` 会通过请求的 `Accept` header 确定 client 想要的格式（比如 JSON），然后寻找合适的 message converter 去做转换。
#### `@RequestBody` 接收 client 的资源
使用 `@RequestBody` 告诉 Spring 根据 `Content-Type` header 确定合适的 message converter 转换资源（比如 JSON，XML）为 Java Objects。
```java
@RequestMapping(method=RequestMethod.POST, consumes="application/json")
@ResponseStatus(HttpStatus.CREATED)
public ResponseEntity<Spittle> saveSpittle(@RequestBody Spittle spittle, UriComponentsBuilder ucb) {
  Spittle saved = spittleRepository.save(spittle);
  HttpHeaders headers = new HttpHeaders();
  URI locationUri = ucb.path("/spittles/")
      .path(String.valueOf(saved.getId()))
      .build()
      .toUri();
  headers.setLocation(locationUri);
  ResponseEntity<Spittle> responseEntity = new ResponseEntity<Spittle>(saved, headers, HttpStatus.CREATED);
  return responseEntity;
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
### 错误状态返回
当控制器出现错误时，我们可能需要返回不同的 HTTP 状态码。在 Spring MVC 中有如下两种方式可以选择：
- `ResponseEntity`
- `@ResponseStatus`
使用 `ResponseEntity` 可以不用使用 `@ResponseBody`，Spring MVC 知道会使用 message converter 去转换。
我们首先定义 `Error` class：
```java
public class Error {
    private int code;
    private String message;
    public Error(int code, String message) {
        this.code = code;
        this.message = message;
    }
    public int getCode() {
        return code;
    }
    public String getMessage() {
        return message;
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
然后使用 `ResponseEntity` 返回对于信息：
```java
@RequestMapping(value="/{id}", method=RequestMethod.GET)
public ResponseEntity<?> spittleById(@PathVariable long id) {
    Spittle spittle = spittleRepository.findOne(id);
    if (spittle == null) {
        Error error = new Error(4, "Spittle [" + id + "] not found");
        return new ResponseEntity<Error>(error, HttpStatus.NOT_FOUND);
    }
    return new ResponseEntity<Spittle>(spittle, HttpStatus.OK);
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
而如果要使用 `@ResponseStatus` 的话，我们需要使用把**错误处理** 的代码提出来放在其他地方，因为一个方法只能有一个 `@ResponseStatus` 注解。我们使用 `@ExceptionHandler` 注解来统一处理**控制器中抛出的异常**，`@ExceptionHandler` 需使用在控制器方法上：
```java
@ExceptionHandler(SpittleNotFoundException.class)
public ResponseEntity<Error> spittleNotFound(SpittleNotFoundException e) {
    long spittleId = e.getSpittleId();
    Error error = new Error(4, "Spittle [" + spittleId + "] not found");
    return new ResponseEntity<Error>(error, HttpStatus.NOT_FOUND);
}
```
- 1
- 2
- 3
- 4
- 5
- 6
```java
// SpittleNotFoundException类
public class SpittleNotFoundException extends RuntimeException {
    private long spittleId;
    public SpittleNotFoundException(long spittleId) {
        this.spittleId = spittleId;
    }
    public long getSpittleId() {
        return spittleId;
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
这样所有控制器抛出的 `SpittleNotFoundException` 异常都会被该处理器处理，看到我们还是使用的 `ResponseEntity` 返回的 404 状态码。现在原来的控制器就变得简单了：
```java
@RequestMapping(value="/{id}", method=RequestMethod.GET)
public ResponseEntity<Spittle> spittleById(@PathVariable long id) {
    Spittle spittle = spittleRepository.findOne(id);
    if (spittle == null) { throw new SpittleNotFoundException(id); }
    return new ResponseEntity<Spittle>(spittle, HttpStatus.OK);
}
```
- 1
- 2
- 3
- 4
- 5
- 6
接下来，我们使用 `@ResponseStatus` 注解：
```java
@RequestMapping(value="/{id}", method=RequestMethod.GET)
@ResponseStatus(HttpStatus.OK)
public @ResponseBody Spittle spittleById(@PathVariable long id) {
    Spittle spittle = spittleRepository.findOne(id);
    if (spittle == null) { throw new SpittleNotFoundException(id); }
    return spittle;
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
其中，因为控制器默认会返回 200（OK），所以这里的 `@ResponseStatus` 注解是可以省略的。
我们也可以对异常处理方法做同样的简化：
```java
@ExceptionHandler(SpittleNotFoundException.class)
@ResponseStatus(HttpStatus.NOT_FOUND)
public @ResponseBody Error spittleNotFound(SpittleNotFoundException e) {
    long spittleId = e.getSpittleId();
    return new Error(4, "Spittle [" + spittleId + "] not found");
}
```
- 1
- 2
- 3
- 4
- 5
- 6
这样就能够返回 404（NOT_FOUND）状态码，而不需要使用 `@ResponseEntity` 了。
