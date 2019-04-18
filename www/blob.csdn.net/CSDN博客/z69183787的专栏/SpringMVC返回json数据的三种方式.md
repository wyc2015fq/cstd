# SpringMVC返回json数据的三种方式 - z69183787的专栏 - CSDN博客
2016年08月08日 15:30:44[OkidoGreen](https://me.csdn.net/z69183787)阅读数：957
**1、第一种方式是spring2时代的产物，也就是每个json视图controller配置一个Jsoniew。**
如：<bean id="defaultJsonView" class="org.springframework.web.servlet.view.json.MappingJacksonJsonView"/> 
或者<bean id="defaultJsonView" class="org.springframework.web.servlet.view.json.MappingJackson2JsonView"/>
同样要用jackson的jar包。
**2、第二种使用JSON工具将对象序列化成json，常用工具Jackson，fastjson，gson。**
利用HttpServletResponse，然后获取response.getOutputStream()或response.getWriter()
直接输出。
示例：
**[java]**[view
 plain](http://blog.csdn.net/shan9liang/article/details/42181345#)[copy](http://blog.csdn.net/shan9liang/article/details/42181345#)
[print](http://blog.csdn.net/shan9liang/article/details/42181345#)[?](http://blog.csdn.net/shan9liang/article/details/42181345#)
- publicclass JsonUtil  
- {  
- 
- privatestatic Gson gson=new Gson();  
- 
- 
- /**
-      * @MethodName : toJson
-      * @Description : 将对象转为JSON串，此方法能够满足大部分需求
-      * @param src
-      *            :将要被转化的对象
-      * @return :转化后的JSON串
-      */
- publicstatic String toJson(Object src) {  
- if (src == null) {  
- return gson.toJson(JsonNull.INSTANCE);  
-         }  
- return gson.toJson(src);  
-     }  
- }  
**3、第三种利用spring mvc3的注解@ResponseBody**
例如：
**[java]**[view
 plain](http://blog.csdn.net/shan9liang/article/details/42181345#)[copy](http://blog.csdn.net/shan9liang/article/details/42181345#)
[print](http://blog.csdn.net/shan9liang/article/details/42181345#)[?](http://blog.csdn.net/shan9liang/article/details/42181345#)
- @ResponseBody
- @RequestMapping("/list")  
- public List<String> list(ModelMap modelMap) {  
-     String hql = "select c from Clothing c ";  
-     Page<Clothing> page = new Page<Clothing>();  
-     page.setPageSize(6);  
-     page  = clothingServiceImpl.queryForPageByHql(page, hql);  
- 
- return page.getResult();  
-   }  
然后使用spring mvc的默认配置就可以返回json了，不过需要jackson的jar包哦。
注意：当springMVC-servlet.xml中使用<mvc:annotation-driven />时，如果是3.1之前已经默认注入AnnotationMethodHandlerAdapter，3.1之后默认注入RequestMappingHandlerAdapter只需加上上面提及的jar包即可!
如果是手动注入RequestMappingHandlerAdapter 可以这样设置
配置如下：
```
<bean class="org.springframework.web.servlet.mvc.method.annotation.RequestMappingHandlerAdapter"
p:ignoreDefaultModelOnRedirect="true" >
<property name="messageConverters">
<list>
<bean class="org.springframework.http.converter.json.MappingJacksonHttpMessageConverter"/>
<list>
<property>
<bean>
```
添加包
jackson-mapper-asl-*.jar
jackson-core-asl-*.jar
