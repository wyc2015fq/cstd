# Springmvc 4.1新特性-jsonp支持 - 零度的博客专栏 - CSDN博客
2017年01月10日 13:45:51[零度anngle](https://me.csdn.net/zmx729618)阅读数：608
1、模型
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
![](http://static.blog.csdn.net/images/save_snippets.png)
定义了三个视图：OnlyIdView、OnlyNameView和AllView。
2、控制器
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
3、使用AbstractJsonpResponseBodyAdvice 实现支持jsonp 
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- @Order(2)  
- @ControllerAdvice(basePackages = "com.github")  
- publicclass JsonpAdvice extends AbstractJsonpResponseBodyAdvice {  
- public JsonpAdvice() {  
- super("callback", "jsonp"); //指定jsonpParameterNames
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
        然后访问如[http://localhost:8080/json?callback=callback](http://localhost:8080/json?callback=callback)即可得到JSONP响应：callback({"user":{"id":1,"name":"zhangsan"}});。 
@ContollerAdvice的作用请参考《[Spring3.2新注解@ControllerAdvice](http://jinnianshilongnian.iteye.com/blog/1866350)》，basePackages用于指定对哪些包里的Controller起作用。
4、使用ResponseBodyAdvice也可以对响应体做一些处理，如下自定义MyResponseBodyAdvice 
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
![](http://static.blog.csdn.net/images/save_snippets.png)
       supports指定支持哪些类型的方法进行处理，此处是返回值为User的；2、我们得到User对象然后在名字前后拼上”---“ ；3、可以指定多个ResponseBodyAdvice，使用@Order指定顺序。访问[http://localhost:8080/jackson2?callback=callback](http://localhost:8080/jackson2?callback=callback)可以看到效果。
