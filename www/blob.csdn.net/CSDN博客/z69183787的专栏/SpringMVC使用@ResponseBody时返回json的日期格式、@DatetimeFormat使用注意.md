# SpringMVC使用@ResponseBody时返回json的日期格式、@DatetimeFormat使用注意 - z69183787的专栏 - CSDN博客
2014年10月22日 14:22:10[OkidoGreen](https://me.csdn.net/z69183787)阅读数：65057
  一、SpringMVC使用@ResponseBody时返回json的日期格式
     前提了解： @ResponseBody 返回json字符串的核心类是org.springframework.http.converter.json.MappingJacksonHttpMessageConverter，它使用了Jackson 这个开源的第三方类库。主要是以下两个jar包：jackson-core-asl-1.6.4.jar；jackson-mapper-asl-1.6.4.jar。
    出现问题： 使用@ResponseBody时返回json字符串的日期格式。Date类型属性默认返回一个Long型的时间戳，怎样能够返回自定义的日期格式？
    解决方案：目前有两种方式实现,
             1、局部修改(网上较多，但不推荐)；
          继承Jackson的抽象类：JsonSerializer<T>，然后在javabean的属性getter()上添加注解@JsonSerialize即可实现。
         代码如下：
Java代码  ![收藏代码](http://aokunsang.iteye.com/images/icon_star.png)
- import java.io.IOException;  
- import java.text.SimpleDateFormat;  
- import java.util.Date;  
- 
- import org.codehaus.jackson.JsonGenerator;  
- import org.codehaus.jackson.JsonProcessingException;  
- import org.codehaus.jackson.map.JsonSerializer;  
- import org.codehaus.jackson.map.SerializerProvider;  
- 
- /**
-  * @description 自定义返回JSON 数据格中日期格式化处理
-  * @author aokunsang
-  * @date 2013-5-28
-  */
- publicclass CustomDateSerializer extends JsonSerializer<Date> {  
- 
- @Override
- publicvoid serialize(Date value,   
-             JsonGenerator jsonGenerator,   
-             SerializerProvider provider)  
- throws IOException, JsonProcessingException {  
-         SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");  
-         jsonGenerator.writeString(sdf.format(value));  
-     }  
- }  
    使用方式：
Java代码  ![收藏代码](http://aokunsang.iteye.com/images/icon_star.png)
- @JsonSerialize(using = CustomDateSerializer.class)  
- public Date getCreateDate() {  
- return createDate;  
- }  
             2、全局修改(强烈推荐)：
         MappingJacksonHttpMessageConverter主要通过ObjectMapper来实现返回json字符串。这里我们继承该类，注册一个JsonSerializer<T>。然后在配置文件中注入自定义的ObjectMapper。
        代码如下：
Java代码  ![收藏代码](http://aokunsang.iteye.com/images/icon_star.png)
- import java.io.IOException;  
- import java.text.SimpleDateFormat;  
- import java.util.Date;  
- 
- import org.codehaus.jackson.JsonGenerator;  
- import org.codehaus.jackson.JsonProcessingException;  
- import org.codehaus.jackson.map.JsonSerializer;  
- import org.codehaus.jackson.map.ObjectMapper;  
- import org.codehaus.jackson.map.SerializerProvider;  
- import org.codehaus.jackson.map.ser.CustomSerializerFactory;  
- 
- /**
-  * @description 解决Date类型返回json格式为自定义格式
-  * @author aokunsang
-  * @date 2013-5-28
-  */
- publicclass CustomObjectMapper extends ObjectMapper {  
- 
- public CustomObjectMapper(){  
-         CustomSerializerFactory factory = new CustomSerializerFactory();  
-         factory.addGenericMapping(Date.class, new JsonSerializer<Date>(){  
- @Override
- publicvoid serialize(Date value,   
-                     JsonGenerator jsonGenerator,   
-                     SerializerProvider provider)  
- throws IOException, JsonProcessingException {  
-                 SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");  
-                 jsonGenerator.writeString(sdf.format(value));  
-             }  
-         });  
- this.setSerializerFactory(factory);  
-     }  
- }  
   spring-servlet.xml中配置：
Java代码  ![收藏代码](http://aokunsang.iteye.com/images/icon_star.png)
- <mvc:annotation-driven>  
-         <mvc:message-converters>  
-             <bean class="org.springframework.http.converter.json.MappingJacksonHttpMessageConverter">  
-                 <property name="objectMapper" ref="customObjectMapper"></property>  
-             </bean>  
-         </mvc:message-converters>  
-     </mvc:annotation-driven>  
-    <bean id="customObjectMapper"class="com.pmc.dwa.common.custom.CustomObjectMapper"></bean> 
 二、@DatetimeFormat使用注意
     1、 使用@DatetimeFormat很简单，这里需要注意的是：使用时要引入一个类库joda-time-1.3.jar，否则会无法访问相应路径(400错误)。 
ps：该注解可以作用在METHOD，FIELD以及PARAMETER级别上。
使用介绍参考：[http://www.captaindebug.com/2011/08/using-spring-3-datetimeformat.html#.UaR3mWWZk0k](http://www.captaindebug.com/2011/08/using-spring-3-datetimeformat.html#.UaR3mWWZk0k)
   2、因为springMVC没有提供默认的日期转换器，前段页面传递过来日期字符串怎么转换为日期类型，如果没有提供全局日期转换器或者数据绑定的基础上，可以使用@DatetimeFormat注解完成。
