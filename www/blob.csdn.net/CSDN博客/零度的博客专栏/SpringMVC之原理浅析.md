# SpringMVC之原理浅析 - 零度的博客专栏 - CSDN博客
2016年06月01日 10:19:00[零度anngle](https://me.csdn.net/zmx729618)阅读数：4247
       SpringMVC主要包括以下要点：
       1：由DispatcherServlet控制的整个流程；
       2：注解驱动的控制器，其中包括请求映射、数据的绑定和格式化；
       3：文件上传；
       4：一些杂项，如静态资源处理、异常处理等等。
      这些东西构成了精致的Spring MVC框架，下面针对这些要点做详细讨论。
#### 1.   Spring MVC框架原理
#### ![Spring](http://dl.iteye.com/upload/attachment/0080/6785/45e7f2f5-3d2e-3e65-b882-8a597e704c5b.jpg)
        DispatcherServlet是Spring MVC的灵魂和心脏，它负责接收HTTP请求并协调Spring MVC的各个组件完成请求处理的工作。和任何Servlet一样，用户必须在web.xml中配置好DispatcherServlet，并且让其接受一 切HTTP请求。当用户的请求被截获时，DispatcherServlet通过HandlerMapping定位到特定的Controller（使用 @Controller注解的普通Java类，此处已经定位到了具体的业务处理方法了，所以我们称其为Handler）。然后通过 
 HandlerAdapter调用Handler中对应的业务处理方法（从这里可以看出与Struts不同的是，Spring MVC是方法级的拦截）。业务处理方法返回一个逻辑视图名（View）和模型数据（Model，二者统称ModelAndView）交给 DispatcherServlet，DispatcherServlet调用ViewResolver解析出真实的视图对象，得到这个视图对象后，DispatcherServlet就使用Model对其进行渲染，将最后结果返回给用户。
        要了解Spring MVC框架的工作原理，必须回答以下三个问题：
        1）  DispatcherServlet如何截获特定的HTTP请求，交由Spring MVC框架处理？
        2）  位于Web层的Spring容器（WebApplicationContext），如何与位于业务层的Spring容器（ApplicationContext）建立关联，以使Web层的Bean可以调用业务层的Bean？
        3）  如何初始化SpringMVC的各个组件，并将它们装配到DispatcherServlet中？
第一个问题已然在上面的步骤中说明了，很简单。对于第二条，其实Web层的容器是作为业务层容器的子容器来配置的，所以访问不是问题。第三，DispatcherServlet有一个初始化方法initStrategies，它在WebApplicationContext初始化后执行，此时所有的组件Bean均已可用。该方法通过反射机制查找并装配Spring容器中用户显示自定义的组件Bean，如果找不到再装配默认的组件实例。
       怎么样，是不是对SpringMVC的工作原理有一个模糊的认识了。到这一步你只要知道Spring MVC也是基于Servlet的，它可以根据URL直接定位到业务处理方法，同时我们可以天然地使用Spring容器，相当之美！
#### 2.   注解驱动的控制器
       正如上面所说，SpringMVC可以直接定位到业务处理方法，那么我们提交的数据是不是还要像Servlet那样通过HTTPServlet来获取，或 者可以像Struts一样绑定到Form中或是Action里面。当然可以，不仅仅如此，Spring MVC还提供了更加强大的数据绑定和转化的功能，使之将Struts之类远远甩到后面去了。
#####          2.1. 请求映射
       在POJO类定义处标注@Controller，再通过<context:component-scan/>扫描相应的类包，即可使POJO 成为一个能处理HTTP请求的控制器。一个控制器的每一个方法都可以成为请求处理方法，如何将请求映射到控制器的方法中是Spring MVC框架最重要的任务之一，这项任务由@RequestMapping承担。
![mapping.jpg](http://dl.iteye.com/upload/attachment/0080/6787/6d9f20a8-32ac-3f0c-aa98-6e4a2f3f5865.jpg)
       ①处的注解很重要，Spring会在启动的时候扫描它，将其划入到Handler中去，没有它一切都白搭。②处的@RequestMapping标注的路 径是相对于应用系统根路径的，在此处写这个注解是为了同一控制器的多个处理方法负责处理相同业务模块的不同操作，这个注解亦可省略，但建议不要这样。③处 的注解是必须的，要定位到具体的处理方法中去。上面的显示列表的URL可以是这样：host:port/app/excavation /list.XXX。
       @RequestMapping不但支持标准的URL，还支持Ant风格（即?、*和**的字符）的和带{xxx}占位符的URL。以下URL都是合法的：
```
/user/*/createUser
   匹配/user/aaa/createUser、/user/bbb/createUser等URL。
/user/**/createUser
   匹配/user/createUser、/user/aaa/bbb/createUser等URL。
/user/createUser??
   匹配/user/createUseraa、/user/createUserbb等URL。
/user/{userId}
   匹配user/123、user/abc等URL。
/user/**/{userId}
   匹配user/aaa/bbb/123、user/aaa/456等URL。
/company/{companyId}/user/{userId}/detail
   匹配company/123/user/456/detail等的URL。
```
       占位符的URL是Spring 3.0新增的功能，该功能在SpringMVC向REST目标挺进的发展过程中具有里程碑的意义。通过@PathVariable可以将URL中的占位符参数绑定到控制器处理方法的入参中，如下所示：
![rest.jpg](http://dl.iteye.com/upload/attachment/0080/6789/4321ae74-7be7-3635-a495-308d5a740262.jpg)
       除了通过URL进行映射外，我们还可以通过请求参数、请求方法和请求头进行映射，由于以上方法已经足够我们进行通常的开发，所以这里就不再详述其他映射方法了，有兴趣的同事可以查询互联网。
#####         2.2. 数据的绑定
       前面说过了，SpringMVC是方法级的映射，那么Spring是如何处理方法签名的，又是如何将表单数据绑定到方法参数中的？下面我们就来讨论这个问题。
**       1.处理方法签名**
       首先，我们可以在方法签名中放入@CookieValue注解参数，Spring自动将Cookie值绑定到参数中；同理@RequestHeader可 以绑定报文头的属性值；同时我们还可以将Servlet API如HttpServletRequest、HttpServletResponse、HttpSession、WebRequest直接作为方法参 数，Spring负责绑定；Spring MVC还允许控制器的处理方法使用java.io.InputStream/java.io.Reader及java.io.OutputStream
 /java.io.Writer作为方法的入参，SpringMVC将获取ServletRequest的InputStream/Reader或 ServletResponse的OutputStream/Writer，然后按类型匹配的方式，传递给控制器的处理方法入参；控制器处理方法的入参除 支持以上类型的参数以外，还支持java.util.Locale、java.security.Principal，可以通过Servlet的 HttpServletRequest的getLocale()及getUserPrincipal()得到相应的值。如果处理方法的入参类型为 
 Locale或Principal，Spring MVC自动从请求对象中获取相应的对象并传递给处理方法的入参。
**       2.表单数据绑定到方法参数**
      再有，表单的数据只要名称相同就可以往方法参数中放，或者是级联的可以封装成对象置于参数中，Spring会自动绑定，如下图所示，极其地方便简单：
![数据绑定.jpg](http://dl.iteye.com/upload/attachment/0080/6789/4321ae74-7be7-3635-a495-308d5a740262.jpg)
**       3. HttpMessageConverter<T>**
       最后，还有一类处理方法入参的形式，即使用HttpMessageConverter<T>，这个非常强大。它提供了两种途径：
              1)        使用@RequestBody/@ResponseBody对处理方法进行标注；
              2)        使用HttpEntity<T>/ResponseEntity<T>作为处理方法的入参或返回值。
       HttpMessageConverter顾名思义，它负责将请求信息转换为一个对象，或者将对象输出为响应信息。前面说过，当请求映射到具体的处理方法后，DispatcherServlet调用HandlerAdapter来封装并执行处理方法。DispatcherServlet默认已经安装了AnnotationMethodHandlerAdapter作为HandlerAdapter的组件实现类，HttpMessageConverter即由AnnotationMethodHandlerAdapter使用，将请求信息转换为对象，或者将对象转换为响应信息。先看几个示例：
```
//将报文体转换为字符串绑定到方法入参中
@RequestMapping(value="/handle41")
public String handle41(@RequestBody  StringrequestBody ) {
      System.out.println(requestBody);
      return "success";
}
//读取一张图片，并将图片数据输出到响应流中，客户端将显示这张图片
@ResponseBody @RequestMapping(value= "/handle42/{imageId}")
public byte[] handle42(@PathVariable("imageId") String imageId) throwsIOException {
       System.out.println("load image of"+imageId);
       Resource res = newClassPathResource("/image.jpg");
       byte[] fileData=FileCopyUtils.copyToByteArray(res.getInputStream());
       return fileData;
}
@RequestMapping(value="/handle43")
public String handle43(HttpEntity<String> httpEntity){
     long contentLen = httpEntity.getHeaders().getContentLength();
     System.out.println(httpEntity.getBody());
      return "success";
}
@RequestMapping(params="method=login")
public ResponseEntity<String> doFirst(){
    HttpHeaders headers = new HttpHeaders();
    MediaType mt=newMediaType("text","html",Charset.forName("UTF-8"));
    headers.setContentType(mt);
    ResponseEntity<String> re=null;
    String return = newString("test");
    re=newResponseEntity<String>(return,headers, HttpStatus.OK);
    return re;
}
```
       这里讲一下HttpMessageConverter中的重点@ResponseBody，我们用它来处理XML和JSON非常之方便。只要在 SpringWeb容器中为AnnotationMethodHandlerAdapter装配好相应的处理XML、JSON的 HttpMessageConverter（AnnotationMethodHandlerAdapter默认只装配部分转换器），并在交互中通过请求 的Accept指定MIME类型，Spring
 MVC就可以使服务端的处理方法和客户端透明地通过XML或JSON格式的消息进行通信了。
![XJ.jpg](http://dl.iteye.com/upload/attachment/0080/6799/cf04cbbd-2685-34c9-906e-e8a0221c5a2a.jpg)
代码中我们可以这样做：
![J.jpg](http://dl.iteye.com/upload/attachment/0080/6791/364a2a6a-c422-329e-a79f-6c2b3f99fd38.jpg)
       这部分其实很简单，对于服务端的处理方法而言，除使用@RequestBody/@ResponseBody或HttpEntity<T>/ResponseEntity<T>进行方法签名外，不需要进行任何额外的处理，借由Spring
 MVC中装配的HttpMessageConverter，它即拥有了处理XML及JSON的能力了。
#### 3.   文件上传
       Spring MVC为文件上传提供了直接的支持，这种支持是通过即插即用的MultipartResolver实现的。
![文件上传.jpg](http://dl.iteye.com/upload/attachment/0080/6793/27dcff75-0bca-3ad8-b900-15474eeb733c.jpg)
       为了能使CommonsMultipartResolver正确工作，必须将JakartaCommons FileUpload和Jakarta Commons io的类包添加到类路径下。下面是代码的写法：
![文件上传代码.jpg](http://dl.iteye.com/upload/attachment/0080/6795/b3c59760-833e-3d5f-9cb2-8573b16e9125.jpg)
      保存到：file:///C:\Users\wmq\AppData\Local\Temp\msohtmlclip1\01\clip_image015.jpg
#### 4.   杂项
       这里主要讲一下静态文件的处理。如何访问到静态的文件，如jpg,js,css？如果你的DispatcherServlet拦截 *.do这样的URL，就不存在访问不到静态资源的问题。如果你的DispatcherServlet拦截“/”，拦截了所有的请求，同时 对*.js,*.jpg的访问也就被拦截了。这种情况下如何搞定静态文件访问问题：
**       方案一：激活Tomcat的defaultServlet来处理静态文件**
```
<servlet-mapping>
            <servlet-name>default</servlet-name>
            <url-pattern>*.jpg</url-pattern>
         </servlet-mapping>
         <servlet-mapping>
            <servlet-name>default</servlet-name>
            <url-pattern>*.js</url-pattern>
         </servlet-mapping>
         <servlet-mapping>
            <servlet-name>default</servlet-name>
            <url-pattern>*.css</url-pattern>
         </servlet-mapping>
```
       要配置多个，每种文件配置一个；要写在DispatcherServlet的前面，让defaultServlet先拦截，这个就不会进入Spring了，我想性能是最好的吧。各服务器defaultServlet名称如下：
```
Tomcat,Jetty, JBoss, and GlassFish  默认 Servlet的名字 --"default"
GoogleApp Engine 默认 Servlet的名字 -- "_ah_default"
Resin 默认 Servlet的名字 --"resin-file"
WebLogic 默认 Servlet的名字  -- "FileServlet"
WebSphere  默认 Servlet的名字 --"SimpleFileServlet"
```
**        方案二：在spring3.0.4以后版本提供了mvc:resources**
```
<!-- 对静态资源文件的访问-->
<mvc:resourcesmapping="/images/**" location="/images/" />
```
/images/**映射到ResourceHttpRequestHandler进行处理，location指定静态资源的位置.可以是webapplication根目录下、jar包里面，这样可以把静态资源压缩到jar包中。使用<mvc:resources/>元素,把mapping的URI注册到SimpleUrlHandlerMapping的urlMap中,key为mapping的URI
 pattern值,而value为ResourceHttpRequestHandler,这样就巧妙的把对静态资源的访问由HandlerMapping转到ResourceHttpRequestHandler处理并返回,所以就支持classpath目录,jar包内静态资源的访问.另外需要注意的一点是,不要对SimpleUrlHandlerMapping设置defaultHandler.因为对static
 uri的defaultHandler就是ResourceHttpRequestHandler,否则无法处理static resourcesrequest.
**方案三****，使用<mvc:default-servlet-handler/>**
        <mvc:default-servlet-handler/>会把"/**" url,注册到SimpleUrlHandlerMapping的urlMap中,把对静态资源的访问由HandlerMapping转到DefaultServletHttpRequestHandler处理并返回。DefaultServletHttpRequestHandler使用就是各个Servlet容器自己的默认Servlet。
补充说明：多个HandlerMapping的执行顺序问题：
```
DefaultAnnotationHandlerMapping的order属性值是：0
<mvc:resources/>自动注册的 SimpleUrlHandlerMapping的order属性值是：2147483646
<mvc:default-servlet-handler/>自动注册 的SimpleUrlHandlerMapping的order属性值是： 2147483647
```
Spring会先执行order值比较小的。当访问一个a.jpg图片文件时，先通过DefaultAnnotationHandlerMapping来找处理器，一定是找不到的，我们没有叫a.jpg的Action。再按order值升序找，由于最后一个SimpleUrlHandlerMapping是匹配"/**"的，所以一定会匹配上，再响应图片。
最后再说明一下，如果你的DispatcherServlet拦截 *.do这样的URL，就不存上述问题了。
