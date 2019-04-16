# Java 学习笔记11：Spring MVC 中的基于注解的 Controller - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年11月28日 10:45:02[initphp](https://me.csdn.net/initphp)阅读数：2994
所属专栏：[Java学习笔记](https://blog.csdn.net/column/details/javago.html)









终于来到了基于注解的 Spring MVC 了。之前我们所讲到的 handler，需要根据 url 并通过 HandlerMapping 来映射出相应的 handler 并调用相应的方法以响应请求。实际上，ControllerClassNameHandlerMapping, MultiActionController 和选择恰当的 methodNameResolver(如 InternalPathMethodNameResolver) 就已经可以在很大程度上帮助我们省去不少的
 XML 配置，谁让 ControllerClassNameHandlerMapping 极度的拥抱了 Convention Over Configuration 呢。

        那为什么还要用基于注解的 Controller 呢？Spring MVC 在 Spring 2.5 发布中新添加了一种基于注解的 Controller 形式。借助于与 Spring 2.5 一同发布的容器内 <context:component-scan> 功能支持，基于注解的 Controller 几乎可以达到 XML 零配置，进而极大地提高我们的开发效率。

        和其它 Controller 一样，基于注解的 Controller 同样有相应的 HandlerMapping，那就是 DefaultAnnotationHandlerMapping。同样，也有相应的 HandlerAdapter，那就是 AnnotationMethodHandlerAdapter。甚至，我们都可以不把 Controller 注册到容器里，那么肯定需要一种机制来帮助我们完成这点，这就是 <context:component-scan>。开发基于注解的
 Controller，我们需要做以下准备工作：


**<context:compnent-scan>**


```
<!-- 切记，这不是必需的！除非你把注解的 Controller 一个个的注册到容器中。相信大家还是喜欢用 context:compnent-scan 吧。不要认为在 Spring MVC 中才提到 context:component-scan，就认为它只能扫描 @Controller。component-scan 默认扫描的注解类型是 @Component，不过，在 @Component 语义基础上细化后的 @Repository, @Service 和 @Controller 也同样可以获得 component-scan 的青睐 -->
<context:component-scan base-package="org.zachary.spring3.anno.web" />
```


**HandlerMapping**


```
<bean class="org.springframework.web.servlet.mvc.annotation.DefaultAnnotationHandlerMapping">
  <description>
  这点是必需的还是非必需的呢？
  如果定义了 DefaultAnnotationHandlerMapping，它就可以将请求来的 url 和被注解了 @RequesMapping 的指进行匹配。当然，说这句话的前提是定义 DefaultAnnotationHandlerMapping 的优先级比定义了其它的 HandlerMapping 的优先级要高(如果定义了其它的话)。
  如果没有定义 DefaultAnnotationHandlerMapping，并不代表不能映射到相应的 handler 上。因为如果你定义了其它的 HandlerMapping，请求过来的 url 和注解了的 @RequestMapping 里的值正好能匹配上，那么没有 DefaultAnnotationHandlerMapping，@Controller 一样可以如鱼得水的被捕获到。
  当然，如果你要使用基于注解的 @Controller，最好还是老老实实地注册 DefaultAnnotationHandlerMapping。
  </description>
</bean>
```


**HandlerAdaptor**


```
<bean class="org.springframework.web.servlet.mvc.annotation.AnnotationMethodHandlerAdapter">
  <description>
  和上面的 HandlerMapping 一样，是必需的还是非必需的呢？
  Spring MVC 中，如果我们没有注册任何 HandlerAdaptor 到容器中，注意，我说的是任何。那么 DispatcherServlet 将启用后备的几个默认使用的 HandlerAdaptor 实现，包括：
  org.springframework.web.servlet.mvc.HttpRequestHandlerAdapter
  org.springframework.web.servlet.mvc.SimpleControllerHandlerAdapter
  org.springframework.web.servlet.mvc.AnnotationMethodHandlerAdaptor

   看见没，如果我们没有注册任何的 HandlerAdaptor，框架会准备 AnnotationMethodHandlerAdaptor 的。可是由于某些原因，我们需要为某些 HandlerAdaptoer 进行一些定制化，即在容器中注册了某个 HandlerAdaptor，那么很抱歉，框架只会启用你注册的那个，而框架本身准备的不会被启用。所以，你一旦为某个 HandlerMapping 进行了定制化，请别忘了把其它的 HandlerAdaptor 也注册进来，即便这些不需要定制化。否则的话，后果你是可以想象的。当然，除非你确保你真的只需要那一个你注册进容器的 HandlerAdaptor，否则，我再啰嗦一遍，别忘了把其它的 HandlerAdaptor 也注册进来。
  </description>
</bean>
```


        好了，有了以上几点准备工作，我们就可以开始基于注解的 Controller 之旅了。下面我们来一个一个注解的来讲解。

**@Controller**

```java
/**
 * @Controller，类级别上的注解。我们定义的类可以只是一个 javabean，不需要实现任何接口。标注了
 * @Controller，借助 <context:component-scan>，框架能自动识别到这就是一个 Controller
 */
@Controller
public class MyController {
  // ......
}
```

**@RequestMapping**

```java
/**
 * @RequestMapping 可以出现在类级别上，也可以出现在方法上。如果出现在类级别上，那请求的 url 为 类级别
 * 上的 @RequestMapping + 方法级别上的 @RequestMapping，否则直接取方法级上的 @RequestMapping。
 * 类级别的 @RequestMapping 不是必需的。
 */
@Controller
@RequestMapping("/my")
public class MyController {
  
  /**
   * 由于类级别上定义了 @RequestMapping，那么想匹配到这个方法来处理请求，url 必须为 /my/somelist。
   * 如果没有定义类级别上的 @RequestMapping，url 为 /somelist 即可。同时，请求方法必须为 POST
   */
  @RequestMapping(value="/somelist", method=RequestMethod.POST);
  public String getSomeList() {...}

    /**
     * 在方法级别使用 @RequestMapping 来限定请求处理的时候，可以指定两个属性。除了我们在上面刚使用过的
     * method 属性，还有一个 params 属性。使用 params 属性，可以达到与使用
     * ParameterMethodNameResolver 作为 MethodResolver的 MultiActionController 类似的功能。
     *
     * params 有两种表达形式，这里先说第一种："parameterName=parameterValue"
     *
     * 请求方法为 GET 或 POST，且具有 hello 参数，且值为 world 的请求才能匹配到该方法，如：
     *   /my?hello=world
     */
    @RequestMapping(params="hello=world", method={RequestMethod.GET, RequestMethod.POST})
    public String helloworld() {...}

    /**
     * 请求方法为 GET 或 POST，且具有 hello 参数，且值为 java 的请求才能匹配到该方法，如：
     *   /my?hello=java
     */
    @RequestMapping(params="hello=java", method={RequestMethod.GET, RequestMethod.POST})
    public String hellojava() {...}

    /**
     * params 属性的另外一种表达形式为："parameter"
     *
     * 请求方法为 GET，且具有请求参数 java 即匹配此方法，而不管 java 参数的值是什么，如：
     *   /my?java=anything
     */
    @RequestMapping(params="java", method={RequestMethod.GET})
    public String java() {...}

    /**
     * 请求方法为 GET，且具有请求参数 cplusplus 即匹配此方法，而不管 cplusplus 参数的值是什么，如：
     *   /my?cplusplus=anything
     */
    @RequestMapping(params="cplusplus", method={RequestMethod.GET})
    public String cplusplus() {...}

    /**
     * @RequestMapping 还有一个参数化 headers，它和 params 非常相似，也有两种表达式，只不过它是对
     * 请求头做限制罢了。大家可以通过 telnet 或 http-client 来发类似的请求以检验。以 telnet 为例：
     * 
     * telnet localhost 8080
     * POST /contextPath/my HTTP/1.1
     * Host: localhost
     * hello: world # 这个就是自定义请求头，和标准的请求头的写法别无二致
     * 【回车】
     * 【回车】
     */
    @RequestMapping(headers="hello=world", method={RequestMethod.POST})
    public String cplusplus() {...}
}
```

**@RequestParam(将请求参数绑定到方法参数)**

```java
@Controller
@RequestMapping("/my")
public class MyController {

  /**
   * 注意，这里的方法有一个参数。若请求 url 为 /my/test，会匹配此方法。这里的方法的参数名为 userId，
   * 那么请求参数中一定有名为 userId 的参数，且值为整数。这也是默认的绑定行为，它是根据名称匹配原则进行
   * 的数据绑定。当请求中的参数名与方法名一致的时候，相应的参数值将被绑定到相应的方法参数上。
   * 
   * 如果没有传递 userId 参数，框架会传入 null。可是这里我们定义的是 primitive type，异常伺候！若
   * 要解决此问题，需要将 primitive type 定义成相应的 wrapper type 即可，这里使用 Integer 就行了。
   *
   * 如果传递了 userId 参数，但值不是整数，你叫 test 怎么办呢？这种情况下，框架借助 PropertyEditor 
   * 数据类型转换失败，ExceptionResolver 会接手处理，请求是不会进入 test 方法的。
   *
   * 这种方式下，默认的绑定行为需要我们严格遵守命名一致性原则。如果我们对此不满，想自定义绑定关系，可以求
   * 助于 @RequestParam。
   */
  @RequestMapping("/test")
  public String test(int userId) { ... }

  /**
   * 当我们不想使用 userId 作为方法的参数名，即不想使用默认的数据绑定方式。如果我们要使用 id 作为方法
   * 的参数，为了保证名称为 userId 的请求参数可以绑定到新的名称为 id 的方法参数上，我们就可以使用 
   * @RequestParam 对这一参数进行标注。@RequestParam 只可以标注于方法参数上。
   *
   * 如果请求参数中有 age，和方法的参数名称一致，故 age 参数不需要 @RequestParam 标注。如果没有传递
   * age，我们又不想定义成 Integer，很显然框架会注入 null 值，报错是必然的。这是由于 @RequestParam 
   * 的 required 属性决定的，默认就是 true。如果我们定义成 false，
   * 即 @RequestParam(required=false) int age
   * 这个时候定义成 int 型的 age，即便请求参数没有 age 参数，也是没问题的。
   *
   * 同时，这里还能绑定 Date 类型，User 对象类型等等。如 date=2011-01-01&userName=Tom&userAge=18
   * 这里，User 类的属性需要为 userName 和 userAge，以免和 age，name 混淆。所以，Spring MVC 对对象
   * 的数据绑定就没有 Struts2 做的那么好了，Strtus2 可以这样：user.age=18&user.name=Tom
   */
  @RequestMapping("/test2")
  public String test2(@RequestParam("userId") int id, int age, Date date, User user) { ... }
}
```

****

**@PathVariable(将 url template 里的参数绑定到方法参数)**



```java
@Controller
@RequestMapping("/my")
public class MyController {

  /**
   * @PathVariable 是 url 模板，需要和 @RequestMapping 配合起来使用，这是 Spring 3.0 之后引入的。
   *
   * 在这个例子中，请求的 url 必须满足类似 /my/user/zhangsan/18 这样的格式才能匹配方法。url 模板里
   * 的参数名和方法参数名的绑定规则和 @RequestParam 类似，这里就不再赘述了。
   *
   * @PathVariable 和 @RequestParam 的区别在于：
   *   @PathVariable 的 url：/my//user/zhangsan/18
   *   @RequestParam 的 url：/my//user?nickname=zhangsan&age=18
   */
  @RequestMapping("/user/{nickname}/{age}");
  public String getUserInfo(@PathVariable("nickname") String name, @PathVariable int age) {...}
}
```


**@RequestBody(将请求正文绑定到方法参数)**


```java
/**
 * 来看一个 http 请求：
 * (请求行) POST /my HTTP/1.1
 * (请求头) Host: localhost
 * (请求头) Content-Type: text/plain
 * (请求头) Content-Length: 5
 *
 * (请求体) hello
 *
 * 这里的 hello，就是请求体，也称 request message。若有请求体，则必须提供请求体的类型和长度，这些信
 * 息是写在请求头里的，即 Content-Type 和 Content-Length
 */
@Controller
@RequestMapping("/my")
public class MyController {

  /**
   * 我们定义的 body 的数据类型是 String，请求体嘛，肯定是 String。实际上，@RequestBody 是用于将请
   * 求体的内容绑定到方法参数上，数据类型不一定是 String。Spring MVC 是通过 HttpMessageConverter
   * 来完成这种转换的。AnnotationMethodHandlerAdapter 默认注册了一些 HttpMessageConverters：
   *   ByteArrayHttpMessageConverter - converts byte arrays
   *   StringHttpMessageConverter - converts strings
   *   FormHttpMessageConverter - converts form data to/from MultiValueMap<String,String>
   *   SourceHttpMessageConverter - convert to/from a javax.xml.transform.Source
   *   MappingJacksonHttpMessageConverter - converts json
   *   MarshallingHttpMessageConverter - convert to/from an object using the 
   *                                     org.springframework.oxm package.
   *
   * 正如上所述，HttpMessageConverter 用于从请求正文绑定到对象和把对象序列化成 String 予客户端响应。
   * 即 HttpMessageConverter is responsible for converting from the HTTP request message to
   * an object and converting from an object to the HTTP response body
   *
   * 我们可以在 AnnotationMethodHandlerAdapter 定义任意多的 HttpMessageConverters。
   *
   * 既然 HttpMessageConverter 可以用于双向 convert，这里讨论的是 @RequestBody，那这部分我们只讲 
   * converting from the HTTP request message to an object。
   *
   * 假设我们只向 AnnotationMethodHandlerAdapter 注入了 MappingJacksonHttpMessageConverter 和
   * MarshallingHttpMessageConverter。处理请求的方法有如下签名：
   *     public String test(@RequestBody User user) { ... }
   *
   * 不管请求正文的内容是什么，对于客户端和服务器而言，它们只是用文本来互相通信。把字符串转为 User 对
   * 象，该用哪个 HttpMessageConverter 来完成此项工作呢？
   *
   * 在定义 HttpMessageConverters 时，我们可以为其指定 supportedMediaTypes。对于将请求正文转为对象
   * 这个方向的操作，HttpMessageConverters 会从请求头得到 Content-Type 头信息，看其是否隶属于其定义
   * 的 supportedMediaTypes。若没有匹配上，则会使用下一个 HttpMessageConverter 做同样的判断。只要
   * 某个 HttpMessageConverter 支持请求头中的 Content-Type，那么就会应用此 HttpMessageConverter
   * 来将 String 转为 Object。当然，若请求正文并没有按照 Content-Type 所规定的格式来编写，必然要收到
   * 500 的响应。同时请注意，请求头中还必须提供 Content-Length，否则拿不到请求正文。
   *
   * 如果所有的 HttpMessageConverters 中定义的 supportedMediaTypes 均不能匹配上 Content-Type 请
   * 求头中的类型，那么就会收到 415 Unsupported Media Type 响应。
   */
  @RequestMapping("/user/body");
  public String getBody(@RequestBody String body) {

    // 这里的 body 的内容就是 hello
    System.out.println(body);
    return null;
  }
}
```


**@ResponseBody(将处理完请求后返回的对象绑定到响应正文)**


```java
/**
 * 上面的 @RequestBody 讲了 HttpMessageConverter 从请求正文到对象转换的方向，现在来讲讲另外一个方
 * 向，@ResponseBody，此时，HttpMessageConverter 用于将处理完请求后返回的对象序列化成字符串，即
 * converting from an object to the HTTP response body.
 */
@Controller
@RequestMapping("/my")
public class MyController {

  /**
   * 该方法的返回类型是 User，并不符合含有 @RequestMapping 的注解所需的签名方式。但它仍然是合法的，因
   * 为在返回类型前有 @ResponseBody 注解，此注解将告知框架，将 User 对象作为影响正文返回？什么？对象
   * 作为响应正文！所以，HttpMessageConverter 在这里就起到作用了。这里讨论的是 @ResponseBody，所以
   * 这里我们只讲 converting from an object to the HTTP response body。
   *
   * User 对象要转成什么样的 String，或者说要转成什么格式的 String？这个时候需要从请求头中获得此信息
   * 了，这里，就是请求头的 Accept 头。Accept 头可以使用逗号分隔定义多个类型，用以告知服务器我只接受
   * 哪些类型的响应。AnnotationMethodHandlerAdapter 中同样注入了多个 HttpMessageConverter，每个 
   * HttpMessageConverter 都可以定义各自的 supportedMediaTypes。这个时候该用哪个 
   * HttpMessageConverter 来完成对象到文本的序列化操作呢？
   *
   * 遍历 Accept 头中的每种媒体类型，在定义的多个 HttpMessageConverters 中依次去匹配，若匹配上，就使
   * 用该 HttpMessageConverter 来完成序列化操作，并且响应头的 Content-Type 并不是请求头 Accept 头
   * 的诸多类型中第一个被匹配的类型，而是匹配到的 HttpMessageConverter 定义的 supportedMediaTypes
   * 中的第一个类型。
   *
   * 如果所有的 HttpMessageConverters 中定义的 supportedMediaTypes 均不能匹配上 Accept 请求头中
   * 的诸多的类型，那么就会收到 406 Not Acceptable 响应。
   */
  @RequestMapping("/user")
  public @ResponseBody User getUser() {
    return new User(18, "Jack", "计算机");
  }
}
```


**@ModelAttribute**


```java
/**
 * @ModelAttribute 可以为视图渲染提供更多的模型数据，而不需要在处理请求的方法里添加 ModelMap 或
 * Model 类型的参数。
 *
 * @ModelAttribute 可以标注在方法(存数据)上，也可以标注在方法参数(取数据)上。
 */
@Controller
@RequestMapping("/my")
public class MyController {

  /**
   * 在处理该请求时，方法的返回类型是 User，貌似不符合返回类型的规范。由于这里使用了 @ModelAttribute
   * 注解，表示将返回的对象以 "user" 为 key 放入模型数据里。这里的 key 值默认值是返回的数据类型首字母
   * 小写的结果。如果想自定义 key，可以写成 @ModelAttribute("myAttribute")，那么模型数据将会将 
   * User 对象绑定到 key 为 "myAttribute" 上。
   * 
   * jsp 里可以这样访问模型里的数据：
   *   age: ${user.age}
   *   name: ${user.name}
   *   job: ${user.job}
   *
   * 当然，这里只是提到了 @ModelAttribute 存数据的操作。
   */
  @RequestMapping("/user")
  @ModelAttribute
  public User getUser() {
    return new User(18, "Jack", "计算机");
  }
  
  /**
   * 这里将 @ModelAttribute 标注在方法参数上，表示要从模型数据里取 key 为 "user" 的对象，绑定在方法
   * 参数上。如果这样做的话，其实你是得不到上面的那个请求放入的 User 对象，得到的是另外一个对象。其实
   * 也好理解，这是两个互相独立的请求，作用域不一样。要想达到我们的目的，即能够从模型数据里取数据，需要
   * 求助于 @SessionAttributes
   */
  @RequestMapping("/user2")
  public String showUser(@ModelAttribute User user) {
    System.out.println(user);
    return null;
  }
}
```


**@SessionAttributes**


```java
/**
 * @SessionAttributes 和 @ModelAttribute 类似，只不过 @SessionAttributes 是将数据存放于 session 
 * 中或从 session 中取数据。
 *
 * @SessionAttributes 只能应用在类型声明上。比如下面的类的声明中，只有属性名为 "the-attribute" 的数
 * 据才会纳入到 session 的管理。
 *
 * @SessionAttributes 允许以属性名名称或者类型两种方法，来表明将哪些数据通过 session 进行管理。这里
 * 我们使用的是指定属性名称的方式，但通过类型来指定也是可行的，如：
 *   @SessionAttributes(types=User.class)
 */
@Controller
@RequestMapping("/my")
@SessionAttributes("the-attribute")
public class MyController {

  @RequestMapping("/getUser")
  public String getUser(int userId, Model model) {
    /**
     * 注意，这里将 User 对象添加到属性名为 "the-attribute" 上，所以 User 对象将纳入到 session 的
     * 管理。如果这里添加的对象的属性名不是 "the-attribute"，那么它只会作用于当前请求，而不会纳入到 
     * session 的管理中。
     */
    User user = userService.getUserById(userId);
    model.addAtrribute("the-attribute", user);
    return "userinfo";
  }
  
  /**
   * 将模型里的 "the-attribute" 为 key 的对象绑定到 User 类上。由于在类级别上声明了只有 "the-
   * attribute" 的属性名才会纳入到 session 的管理，所以就解决了在 @ModelAttribute 注解中讲解中最
   * 后提到的问题。
   *
   * 另外，这个方法还有两个参数，BindingResult 和 SessionStatus。由于这里有绑定数据的动作，我们可以
   * 根据 BindingResult 对象获得数据绑定结果以决定后继流程该如何处理。SessionStatus 在这里用于处理
   * 完请求后，清空 session 里的数据。
   */
  @RequestMapping("/updateUser")
  public String updateUser(@ModelAttribute("the-attribute") User user, 
  			BindingResult result, SessionStatus status) {

    if (result.hasErrors) {
      return "error";
    }
    
    userService.updateUser(user);
    // 我们通过调用 status.setComplete() 方法，该 Controller 所有放在 session 级别的模型属性数据
    // 将从 session 中清空
    status.setComplete();
    return "redirect:getUser?userId=" + user.getId();
  }
}
```


Spring MVC 里的大部分的注解，这里基本上都讲到了。日后随着 Spring 的升级，我也会逐一补充新加的注解。其实，仅凭以上的注解，是可以构建一个足够强大的 RESTFul Webservices 的了。

        这里，补充讲下被标注了 @RequestMapping 注解的请求方法的签名。使用 @RequestMapping 标注的 web 请求处理方法的签名比较灵活，我们几乎可以声明并使用任何类型的方法参数。不过，以下几种类型的方法参数将拥有更多语义，它们均来自框架内部(或者说 AnnotationMethodHandlerAdapter)所管理的对象引用：

- **request/response/session**
- **org.springframework.web.context.request.WebRequest**。当前处理方法中获得可用的 WebRequest 实例。
- **java.util.Locale**。通过相应的 LocalResolver 所返回的对应当前 web 请求的 Locale。
- **java.io.InputStream/java.io.Reader**。相当于 request.getInputStream() 或 request.getReader() 所获得的对象引用。
- **java.io.OutputStream/java.io.Writer**。相当于 response.getOutputStream() 或 response.getWriter() 所获得的对象引用。
- **java.util.Map/org.springframework.ui.ModelMap**。你现在可用对模型数据为所欲为了。
- **org.springframework.validation.Errors/org.springframework.validation.BindingResult**。用于对 Command 对象进行数据验证的 Errors 或者 BindingResult 对象。声明这两种类型的方法参数有一个限制，它们的声明必须紧跟着 Command 对象的定义。其它类型的方法参数是没有任何顺序限制的。
- **org.springframework.web.bind.supportt.SessionStatus**。SessionStatus 主要用于管理请求处理之后 Session 的状态，比如清除 Session 中的指定的数据。


        基于注解的 Controller 的请求处理方法返回类型可以有如下 4 种形式(当然，前面提到的 @ResponseBody 和 @ModelAttribute 并没下面所描述的返回类型，具体参见上面对各自注解的讲解)：

- **org.springframework.web.servlet.ModelAndView**。这个不用多说，视图信息和模型信息都能通过它返回。
- **java.lang.String**。该类型返回值代表逻辑视图名，模型数据需要以其它形式提供，比如为处理方法声明一个 ModelMap 类型的参数。**注意，如果返回 null，并不代表向客户端输出空页面(定向思维惹的祸)，这种情况下，框架会从请求路径中提取视图信息。如果返回 null 就是要表示方法内部已处理完请求，也不需要通知页面，就是想仅仅返回空白页面，唉，我还没有想出来咋整。。。反正 writer.write("") 这样写可以，还得声明一个 Writer 类型的方法参数。**
- **org.springframework.ui.ModelMap**。ModelMap 类型返回值只包含了模型数据信息而没有视图信息，框架将根据请求的路径来提取视图信息。
- **void**。没有任何返回值，视图信息将从请求路径中提取，模型数据需要通过其它形式提供。


        String 类型的返回值为 null, 还有返回类型为 ModelMap 和 void，从请求路径中如何提取视图信息呢？框架将截取请求路径中的最后一个 / 后面的内容，并去掉后缀名，剩下来的内容就是视图名。如请求路径为 /spring3/user/welcome，那么视图名是 welcome，/spring3/user/welcome.action 的视图名也是 welcome。

        接下来来讲最后一个部分，请求参数到方法参数的绑定。这个在 @RequestParam 中已经讲过，不过，这里要讲的是绑定复杂的对象。在 @RequestParam 中，我们这样请求，date=2011-01-01 其实是绑定不到 Date 对象的。因为不同的 Locale 处理日期的字符串的表达方式不一样。总之，这部分涉及到字符串到对象的转换，这很像 PropertyEditor，对吧？Spring MVC 中，可以为某个 Controller
 定制数据绑定，即在被标注了 @InitBinder 的方法里写绑定逻辑，方法名可以随意，如：


```java
/**
 * 初始化方法不能有返回值，而且至少应该有一个类型为 org.springframework.web.bind.WebDataBinder 的
 * 方法参数。同时，一个典型的基于注解的 Controller 的处理方法可以使用的方法参数中，除了 Command 对象
 * 以及相关的 Errors/BindingResult 对象作为方法的参数外，都可以作为初始化方法的参数。
 *
 * 这里，我们没有必要为日期再定制自定义绑定规则，Spring 已经为我们提供了 CustomDateEditor，这里只是演
 * 示如何提供自定义数据绑定规则。
 *
 * 这里的 WebDataBinder，是不是很像 PropertyEditorRegistry？
 */
@InitBinder
public void initBinder(WebDataBinder binder) {
  binder.registerCustomEditor(Date.class, new PropertyEditorSupport() {
  
    final SimpleDateFormat sf = new SimpleDateFormat("yyyy-MM-dd");
    
    @Override
    public void setAsText(String text) throws IllegalArgumentException {
      try {
        Date date = sf.parse(text);
        setValue(date);
      } catch (ParseException e) {
        Date data = sf.parse(text);
        throw new IllegalArgumentException(e);
      }
    }
  })
}
```


在 Controller 里使用 @InitBinder 标注的初始化方法只能对一个 Controller 对应的 WebBinder 做定制。如果想在整个应用中共享绑定规则，可以为 AnnotationMethodHandlerAdapter 指定一个自定义的 org.springframework.web.bind.support.WebBindingInitializer 实例，这样可以避免在每个 Controller 中都重复定义几乎相同逻辑的
 @InitBinder 的初始化方法。


```java
public class MyBindingInitializer implements WebBindingInitializer {

  public void initBinder(WebBinder binder, WebRequest request) {
    binder.registerCustomEditor(SomeDataType.class, somePropertyEditor)
    // 如果需要，这里可以继续注册更多的 propertyEditor
    // ......
  }
}
```




```
<bean class=""org.springframework.web.servlet.mvc.annotation.AnnotationMethodHandlerAdapter>
  <property name="webBindingInitializer">
    <bean class="...MyBindingInitializer" />
  </property>
</bean>
```


结束该篇文章前，我们来看几个容易混淆的用于简化开发的配置： <mvc:annotation-driven />, <context:annotation-config/>, <context:component-scan />。

**<mvc:annotation-driven />** 会做以下几件事：

- 向 spring 容器中注册 DefaultAnnotationHandlerMapping。
- 向 spring 容器中注册 AnnotationMethodHandlerAdapter。
- 配置一些 messageconverter。
- **解决了 @Controller 注解的使用前提配置，即 HandlerMapping 能够知道谁来处理请求。**

**<context:annotation-config />** 会做以下几件事：

- 向 spring 容器中注册 AutowiredAnnotationBeanPostProcessor。
- 向 spring 容器中注册 CommonAnnotationBeanPostProcessor。
- 向 spring 容器中注册 PersistenceAnnotationBeanPostProcessor。
- 向 spring 容器中注册 RequiredAnnotationBeanPostProcessor。
- **使用 <context:annotationconfig />之前，必须在 <beans> 元素中声明 context 命名空间 <context:component-scan />。<context:component-scan /> 对包进行扫描，实现注解驱动 Bean 定义。即，将 @Controller 标识的类的 bean 注册到容器中。**

**<context:component-scan/>**，不但启用了对类包进行扫描以实施注解驱动 Bean 定义的功能，同时还启用了注解驱动自动注入的功能（即还隐式地在内部注册了 AutowiredAnnotationBeanPostProcessor 和 CommonAnnotationBeanPostProcessor）。因此当使用 <context:component-scan /> 后，除非需要使用PersistenceAnnotationBeanPostProcessor
 和 RequiredAnnotationBeanPostProcessor 两个 Processor 的功能（例如 JPA 等），否则就可以将 <context:annotation-config /> 移除了。



