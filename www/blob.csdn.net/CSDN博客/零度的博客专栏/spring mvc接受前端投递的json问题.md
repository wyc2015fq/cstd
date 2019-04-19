# spring mvc接受前端投递的json问题 - 零度的博客专栏 - CSDN博客
2017年01月03日 14:23:45[零度anngle](https://me.csdn.net/zmx729618)阅读数：480标签：[spring mvc](https://so.csdn.net/so/search/s.do?q=spring mvc&t=blog)
个人分类：[SpringMVC](https://blog.csdn.net/zmx729618/article/category/6248665)
                
现有bean类User
```java
class User {
    long id;
    String name;
}
```
## 从前端投递过来json数据
```
json: {"id":1, "name": "steven", "age":30}
```
## 后端通过spring mvc接受json数据并转为bean对象
```java
public void create(User user) {
    //todo save
}
```
## 问
此时因为json数据中比bean对象多了一个属性，所以后台spring mvc在处理的时候会报`400 bad request`错误，有什么办法可以过滤掉前端传过来的多余的属性么？json传过来的属性可能很多且未知属性名，我只想取其中需要的部分
解决方法：
SpringMVC默认使用的是jackson，配置一下就好了：
```
objectMapper.configure(DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES, false);
```
SpringMVC 用的是MappingJackson2HttpMessageConverter这个类。
可以写一个类继承MappingJackson2HttpMessageConverter，构造方法传入objectMapper对象的时候进行配置
web.xml 指定自己的类
```
<bean  
 class="org.springframework.web.servlet.mvc.method.annotation.RequestMappingHandlerMapping">  
</bean>  
<bean  
    class="org.springframework.web.servlet.mvc.method.annotation.RequestMappingHandlerAdapter">  
    <property name="messageConverters">  
        <list>  
            <bean  
                class="MyConvert" />  
        </list>  
    </property>  
</bean>
```
如果使用的是config的方式进行配置，可以继承WebMvcConfigurerAdapter，重写configureMessageConverters
```java
```
@EnableWebMvc
@Configuration
public class JSONConfig extends WebMvcConfigurerAdapter{
    @Override
    public void configureMessageConverters(List<HttpMessageConverter<?>> converters) {
        ObjectMapper objectMapper = Jackson2ObjectMapperBuilder.json().build();
        objectMapper.configure(DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES, false);
        converters.add(new MappingJackson2HttpMessageConverter(objectMapper));
    }
}
```
```
