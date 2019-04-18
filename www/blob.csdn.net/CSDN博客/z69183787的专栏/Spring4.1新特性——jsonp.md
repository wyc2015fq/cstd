# Spring4.1新特性——jsonp - z69183787的专栏 - CSDN博客
2016年08月08日 15:30:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2792
**5、 Jackson @JsonView支持**
可以使用@JsonView来分组渲染JSON数据，按需展示JSON数据。
5.1、模型
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- publicclass User implements Serializable {  
- publicstaticinterface OnlyIdView {}  
- publicstaticinterface OnlyNameView {}  
- publicstaticinterface AllView extends OnlyIdView, OnlyNameView {}  
- 
- @JsonView(OnlyIdView.class)  
- private Long id;  
- 
- @JsonView(OnlyNameView.class)  
- private String name;    
-     ……  
- }  
定义了三个视图：OnlyIdView、OnlyNameView和AllView。
5.2、控制器
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- @RestController
- publicclass JacksonJsonViewController {  
- 
- @RequestMapping("/jackson1")  
- @JsonView(User.OnlyIdView.class)  
- public User test1() {  
- returnnew User(1L, "zhangsan");  
-     }  
- 
- @RequestMapping("/jackson2")  
- @JsonView(User.OnlyNameView.class)  
- public User test2() {  
- returnnew User(1L, "zhangsan");  
-     }  
- 
- @RequestMapping("/jackson3")  
- @JsonView(User.AllView.class) //可以省略
- public User test3() {  
- returnnew User(1L, "zhangsan");  
-     }  
- }  
使用@JsonView控制渲染哪些数据。
**6、Jsonp支持**
6.1、MappingJackson2JsonView提供的支持 
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- <bean class="org.springframework.web.servlet.view.json.MappingJackson2JsonView">  
-     <property name="jsonpParameterNames">  
-         <set>  
-             <value>jsonp</value>  
-             <value>callback</value>  
-         </set>  
-    </property>  
- </bean>  
然后访问如http://localhost:8080/json?callback=callback即可得到JSONP响应：callback({"user":{"id":1,"name":"zhangsan"}});。
6.2、对使用HttpMessageConverter的@ResponseBody的支持 
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- @Order(2)  
- @ControllerAdvice(basePackages = "com.github")  
- publicclass JsonpAdvice extends AbstractJsonpResponseBodyAdvice {  
- public JsonpAdvice() {  
- super("callback", "jsonp"); //指定jsonpParameterNames
-     }  
- }  
访问http://localhost:8080/jackson1?callback=callback即可看到JSONP响应。 
@ContollerAdvice的作用请参考《[Spring3.2新注解@ControllerAdvice](http://jinnianshilongnian.iteye.com/blog/1866350)》，basePackages用于指定对哪些包里的Controller起作用。
6.3、ResponseBodyAdvice
我们之前实现的JsonpAdvice其继承自AbstractJsonpResponseBodyAdvice，而AbstractJsonpResponseBodyAdvice继承自ResponseBodyAdvice，其作用是在响应体写出之前做一些处理： 
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- @Order(1)  
- @ControllerAdvice(basePackages = "com.github")  
- publicclass MyResponseBodyAdvice implements ResponseBodyAdvice<Object> {  
- 
- @Override
- publicboolean supports(MethodParameter methodParameter, Class<? extends HttpMessageConverter<?>> converterType) {  
- return methodParameter.getMethod().getReturnType().isAssignableFrom(User.class);  
-     }  
- 
- @Override
- public Object beforeBodyWrite(  
-             Object obj, MethodParameter methodParameter, MediaType mediaType,  
-             Class<? extends HttpMessageConverter<?>> converterType,  
-             ServerHttpRequest serverHttpRequest, ServerHttpResponse serverHttpResponse) {  
- 
-         User user = ((User)obj);  
-         user.setName("---" + user.getName() + "---");  
- return user;  
-     }  
- }  
1、supports指定支持哪些类型的方法进行处理，此处是返回值为User的；2、我们得到User对象然后在名字前后拼上”---“ ；3、可以指定多个ResponseBodyAdvice，使用@Order指定顺序。访问http://localhost:8080/jackson2?callback=callback可以看到效果。
**7、Gson HttpMessageConverter**
7.1、Spring配置 
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- <mvc:annotation-driven>  
-     <mvc:message-converters>  
-         <bean class="org.springframework.http.converter.json.GsonHttpMessageConverter"/>  
-     </mvc:message-converters>  
- </mvc:annotation-driven>  
使用方式和Jackson Json类似。本文使用的是<gson.version>2.2.4</gson.version>版本。
