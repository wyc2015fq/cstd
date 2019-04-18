# jackson 枚举/日期 date enum json 解析类型 返回数字 或者自定义文字 - z69183787的专栏 - CSDN博客
2016年10月20日 16:27:14[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4769
jackson 解析枚举 时一般返回 MALE，FEMALE，
如果想返回 文字或者数字怎么办。网上stackflow上大都用***@JsonCreator***
官网提供的方法（下面的方法）更好点，想返回什么就在属性的get方法上加    @JsonValue。如果想在不同的方法中返回不同的值就用其他的方法了！
**[java]**[view
 plain](http://blog.csdn.net/my_flash/article/details/50802961#)[copy](http://blog.csdn.net/my_flash/article/details/50802961#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1597186/fork)
- package com.model;  
- 
- import com.fasterxml.jackson.annotation.JsonValue;  
- 
- publicenum Gender {  
-     MALE(0,"男"),FEMALE(1,"女"),UNKOWN(2,"未知");  
- 
- private Integer id;  
- private String name;  
- 
- private Gender(Integer id,String name) {  
- this.id = id;  
- this.name = name;  
-     }  
- private Gender(Integer id) {  
- this.id = id;  
-     }  
- 
- private Gender(String name) {  
- this.name = name;  
-     }  
- 
- 
- public String getName() {  
- return name;  
-     }  
- publicvoid setName(String name) {  
- this.name = name;  
-     }  
- @JsonValue
- public Integer getId() {  
- return id;  
-     }  
- publicvoid setId(Integer id) {  
- this.id = id;  
-     }  
- 
- }  
- 
OR ：
在使用该枚举的对象的 get方法上
@JsonSerialize(using
 = OrderTypeSerializer.class)
```java
public class OrderTypeSerializer extends JsonSerializer<OrderType> {
@Override
public void serialize(OrderType value, JsonGenerator generator,
SerializerProvider provider) throws IOException,
JsonProcessingException {
generator.writeStartObject();
generator.writeFieldName("id");
generator.writeNumber(value.getId());
generator.writeFieldName("name");
generator.writeString(value.getName());
generator.writeEndObject();
}}
```
同理，解决日期格式也可以这样：
在SPRING MVC3中，比如做REST时，经常要用JASON去解析，遇到了日期格式化的问题，
请看例子：
1） 比如有一个POJO
  package com.loiane.model;
import java.util.Date;
import org.codehaus.jackson.annotate.JsonAutoDetect;
@JsonAutoDetect
@Entity
public class Company {
    private int id;
    private double price;
    private String company;
    private Date date;
    private String size;
    private byte visible;
}
2）在controller中，返回一个MAP
   public @ResponseBody Map<String,List<Company>> view() throws Exception
3） 于是SPIRNG MVC返回如下的JSON字符：
   {"total":27,"data":[{"price":71.72,"company":"3m Co","visible":1,"id":1,"size":"large","date":1188615600000},{"price":29.01,"company":"Aloca
Inc","visible":0,"id":2,"size":"medium","date":1185937200000},{"price":83.81,"company":"Altria Group
Inc","visible":0,"id":3,"size":"large","date":1186110000000
。。。。。。。。。。。。。。。。
4） 可以看到，返回的时间格式不大好看
5）于是，可以这样设置：
   @JsonSerialize(using=JsonDateSerializer.class)
public Date getDate() {
    return date;
}
其中   JsonDateSerializer是新编写的类，继承了JsonSerializer
  import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;
import org.codehaus.jackson.JsonGenerator;
import org.codehaus.jackson.JsonProcessingException;
import org.codehaus.jackson.map.JsonSerializer;
import org.codehaus.jackson.map.SerializerProvider;
import org.springframework.stereotype.Component;
@Component
public class JsonDateSerializer extends JsonSerializer<Date>{
    private static final SimpleDateFormat dateFormat = new SimpleDateFormat("MM-dd-yyyy");
    @Override
    public void serialize(Date date, JsonGenerator gen, SerializerProvider provider)
            throws IOException, JsonProcessingException {
        String formattedDate = dateFormat.format(date);
        gen.writeString(formattedDate);
    }
}
   很简单，格式化一下，这下输出JSON就可以了
{"total":27,"data":[{"price":71.72,"company":"3m Co","visible":1,"id":1,"size":"large","date":"09-01-2007"},{"price":29.01,"company":"Aloca
Inc","visible":0,"id":2,"size":"medium","date":"08-01-2007"},{"price":83.81,"company":"Altria Group
Inc","visible":0,"id":3,"size":"large","date":"08-03-2007"},{"price":52.55,"company":"American Express Company","visible":1,"id":4,"size":"extra
large","date":"01-04-2008"},{"price":64.13,"company":"American International Group
Inc.","visible":1,"id":5,"size":"small","date":"03-04-2008"},{"price":31.61,"company":"AT&T Inc.","visible":0,"id":6,"size":"extra
large","date":"02-01-2008"},{"price":75.43,"company":"Boeing Co.","visible":1,"id":7,"size":"large","date":"01-01-2008"},{"price":67.27,"company":"Caterpillar
Inc.","visible":1,"id":8,"size":"medium","date":"12-03-2007"},{"price":49.37,"company":"Citigroup,
Inc.","visible":1,"id":9,"size":"large","date":"11-24-2007"},{"price":40.48,"company":"E.I. du Pont de Nemours and Company","visible":0,"id":10,"size":"extra
large","date":"05-09-2007"}],"success":true}
