# Spring mvc + jackson2 返回json格式（包含日期格式解析） - z69183787的专栏 - CSDN博客
2014年10月22日 14:19:44[OkidoGreen](https://me.csdn.net/z69183787)阅读数：9561
写了那么多，发现白忙活了一场，原来jackson也有一个@JsonFormat注解，将它配置到Date类型的get方法上后，jackson就会按照配置的格式转换日期类型，而不自定义转换器类，欲哭无泪啊。辛苦了那么多，其实别人早已提供，只是没有发现而已。
不说了，直接上方案吧。
1.spring配置照样是这样：
**[html]**[view
 plain](http://blog.csdn.net/zhanngle/article/details/24123659#)[copy](http://blog.csdn.net/zhanngle/article/details/24123659#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/301882/fork)
- <mvc:annotation-driven>
2.JsonUtil可以不用了，但如果要自己从response对象输出json，那么还是可以用，但改成了这样
**[java]**[view
 plain](http://blog.csdn.net/zhanngle/article/details/24123659#)[copy](http://blog.csdn.net/zhanngle/article/details/24123659#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/301882/fork)
- package com.xxx.utils;  
- 
- import java.io.IOException;  
- import java.text.SimpleDateFormat;  
- import org.springframework.stereotype.Component;  
- import com.fasterxml.jackson.databind.ObjectMapper;  
- 
- /**
-  * json处理工具类
-  * @author zhangle
-  */
- @Component
- publicclass JsonUtil {  
- 
- privatestaticfinal String DEFAULT_DATE_FORMAT="yyyy-MM-dd HH:mm:ss";  
- privatestaticfinal ObjectMapper mapper;  
- 
- static {  
-         SimpleDateFormat dateFormat = new SimpleDateFormat(DEFAULT_DATE_FORMAT);  
-         mapper = new ObjectMapper();  
-         mapper.setDateFormat(dateFormat);  
-     }  
- 
- publicstatic String toJson(Object obj) {  
- try {  
- return mapper.writeValueAsString(obj);  
-         } catch (Exception e) {  
- thrownew RuntimeException("转换json字符失败!");  
-         }  
-     }  
- 
- public <t> T toObject(String json,Class<t> clazz) {  
- try {  
- return mapper.readValue(json, clazz);  
-         } catch (IOException e) {  
- thrownew RuntimeException("将json字符转换为对象时失败!");  
-         }  
-     }  
- }</t></t>  
3.实体类的get方法就需要多一个@JsonFormat的注解配置
**[java]**[view
 plain](http://blog.csdn.net/zhanngle/article/details/24123659#)[copy](http://blog.csdn.net/zhanngle/article/details/24123659#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/301882/fork)
- @DateTimeFormat(pattern="yyyy-MM-dd HH:mm:ss")  
- @JsonFormat(pattern="yyyy-MM-dd HH:mm:ss",timezone = "GMT+8")  
- public Date getCreateTime() {  
- returnthis.createTime;  
- }  
- @DateTimeFormat(pattern="yyyy-MM-dd")  
- @JsonFormat(pattern="yyyy-MM-dd",timezone = "GMT+8")  
- public Date getBirthday() {  
- returnthis.birthday;  
- }  
- 
