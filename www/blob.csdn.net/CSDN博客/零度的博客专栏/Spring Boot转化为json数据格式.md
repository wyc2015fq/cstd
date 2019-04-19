# Spring Boot转化为json数据格式 - 零度的博客专栏 - CSDN博客
2018年07月04日 17:05:05[零度anngle](https://me.csdn.net/zmx729618)阅读数：3835
Spring Boot为我们良好的提供了我们需要的数据,将数据转化为json格式,然后返回,
下面请看springboot转化为json的方式;
**第一种方式:**
**SpringBoot框架默认的方式;**
步骤:
*** 1.编写实体类student;**
package com.gmm;*/****测试的实体类*** Created by john on 2017-04-30.** */*public class Student {private Integer sId;
    private String sName;
    private String gender;
    public Integer getsId() {return sId;
}public void setsId(Integer sId) {this.sId = sId;}public String getsName() {return sName;
}public void setsName(String sName) {this.sName = sName;}public String getGender() {return gender;
}public void setGender(String gender) {this.gender = gender;}}
*** 2.编写getStudernt()方法在controller中;**
**写法:创建一个学生对象设置学生属性信息数据然后获取以json的方式获取这些数据;**
package com.gmm;import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
*/**** * Created by john on 2017-04-30.** */*@RestControllerpublic class Controller {*/****     * springboot**默认使用的**json**解析框架是**jackson;**     * ****@return*****/*@RequestMapping("/getStudent")public Student getStudent(){        Student student = new Student();student.setGender("女");student.setsName("莉香");student.setsId(1);        return student;}}
注意: @RestController 此注解是代表当前类是一个Restful API的controller;
@RequestMapping("/getStudent") 此注解是请求的映射路径;
*** 3.进行测试;**
**编写一个Springboot的入口程序;**
package com.gmm;import org.apache.log4j.Logger;import org.springframework.boot.SpringApplication;import org.springframework.boot.autoconfigure.SpringBootApplication;
*/**** * Created by john on 2017-04-30.** */*@SpringBootApplicationpublic class Application {private static Logger *logger *= Logger.getLogger(Application.class);    public static void main(String[] args) {        SpringApplication.run(Application.class,args);*logger*.info("=====spring boot start success====");}}
注:访问路径为http://localhost:8080/getStudent 
**谷歌游览器页面显示的结果为:**
{"sId":1,"sName":"莉香","gender":"女"}
以上我们了解到SpringBootmoren 默认使用的json解析技术框架是jackson;
在maven下载的架包中可以看到有一个jackson.jar架包;
以上是第一种解析方式,通常我们也是用其它的解析方式;
第二种方式;
Spring Boot使用FastJson解析JSON数据
步骤
*** 1.在上面pom,xml文件依赖的基础上添加FastJson的依赖包;**
**完整pom文件**
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>    <groupId>springbootjson</groupId>    <artifactId>com.gmm</artifactId>    <version>1.0-SNAPSHOT</version>    <parent>        <groupId>org.springframework.boot</groupId>        <artifactId>spring-boot-starter-parent</artifactId>        <version>1.4.1.RELEASE</version>    </parent>    <properties>        <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding><!-- 指定一下jdk的版本 ，这里我们使用jdk 1.8 ,默认是1.6 -->
<java.version>1.8</java.version>    </properties>    <dependencies>        <dependency>            <groupId>org.springframework.boot</groupId>            <artifactId>spring-boot-starter-web</artifactId>        </dependency>        <dependency>            <groupId>org.springframework.boot</groupId>            <artifactId>spring-boot-configuration-processor</artifactId>            <optional>true</optional>            <scope>test</scope>        </dependency>        <dependency>            <groupId>com.alibaba</groupId>            <artifactId>fastjson</artifactId>            <version>1.2.15</version>        </dependency>    </dependencies></project>在这个地方我们要强调一下，在官方文档说的1.2.10以后，会有两个方法支持HttpMessageconvert，一个是FastJsonHttpMessageConverter，支持4.2以下的版本，一个是FastJsonHttpMessageConverter4支持4.2以上的版本，这里也就是说：低版本的就不支持了，所以这里最低要求就是1.2.10+。具体看文档!
*** 2.配置fastjon(支持两种方法);**
**第一种方法:**
**一:启动类继承extends WebMvcConfigurerAdapter**
**二:覆盖方法configureMessageConverters**
package com.gmm;import com.alibaba.fastjson.serializer.SerializerFeature;import com.alibaba.fastjson.support.config.FastJsonConfig;import com.alibaba.fastjson.support.spring.FastJsonHttpMessageConverter;import org.apache.log4j.Logger;import org.springframework.boot.SpringApplication;import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.autoconfigure.web.HttpMessageConverters;import org.springframework.context.annotation.Bean;
import org.springframework.http.MediaType;import org.springframework.http.converter.HttpMessageConverter;import org.springframework.web.servlet.config.annotation.WebMvcConfigurerAdapter;import java.util.ArrayList;import java.util.List;*/** springboot**以**fastjon**方式转化**json**数据**;** * Created by john on 2017-04-30.** */*@SpringBootApplication
public class Application extends WebMvcConfigurerAdapter {private static Logger *logger *= Logger.getLogger(Application.class);@Override
public void configureMessageConverters(List<HttpMessageConverter<?>> converters) {super.configureMessageConverters(converters);//1.需要定义一个convert转换消息的对象;
FastJsonHttpMessageConverter fastJsonHttpMessageConverter = new FastJsonHttpMessageConverter();//2.添加fastJson的配置信息，比如：是否要格式化返回的json数据;
FastJsonConfig fastJsonConfig = new FastJsonConfig();fastJsonConfig.setSerializerFeatures(SerializerFeature.*PrettyFormat*);//3处理中文乱码问题
List<MediaType> fastMediaTypes = new ArrayList<>();fastMediaTypes.add(MediaType.*APPLICATION_JSON_UTF8*);//4.在convert中添加配置信息.
fastJsonHttpMessageConverter.setSupportedMediaTypes(fastMediaTypes);fastJsonHttpMessageConverter.setFastJsonConfig(fastJsonConfig);//5.将convert添加到converters当中.
converters.add(fastJsonHttpMessageConverter);}public static void main(String[] args) {        SpringApplication.run(Application.class,args);*logger*.info("=====spring boot start success====");}}
三:在启动测试实体类中添加一个属性;并提供getset方法;
@JSONField(format = "yyyy-MM-dd HH:mm")private Date createTime;
public Date getCreateTime() {return createTime;
}public void setCreateTime(Date createTime) {this.createTime = createTime;}@JSONField(format = "yyyy-MM-dd HH:mm")此注解是fastjon包提供的用于指定将数据返回什么样的json格式数据;四:在controller中在将添加的实体属性给加进去;
@RequestMapping("/getStudent")
public Student getStudent(){
    Student student = new Student();
student.setGender("女");
student.setsName("莉香");
student.setsId(1);
student.setCreateTime(new Date());    return student;
}五:开启springboot测试结果;
**结果为:**
**![](https://img-blog.csdn.net/20170501001515869?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWJjY2hlbmd4dXl1YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**
说明springboot按照fastjson的方式将数据以json格式解析出来了;
**第二种方法:**
**一:启动测试类中注入Bean: HttpMessageConverters**
**具体做法:删除继承extends WebMvcConfigurerAdapter;**
**      删除重写的方法改为以下代码;**
@Beanpublic HttpMessageConverters fastJsonHttpMessageConverters(){//1.需要定义一个convert转换消息的对象;
FastJsonHttpMessageConverter fastJsonHttpMessageConverter = new FastJsonHttpMessageConverter();//2:添加fastJson的配置信息;
FastJsonConfig fastJsonConfig = new FastJsonConfig();fastJsonConfig.setSerializerFeatures(SerializerFeature.*PrettyFormat*);//3处理中文乱码问题
List<MediaType> fastMediaTypes = new ArrayList<>();fastMediaTypes.add(MediaType.*APPLICATION_JSON_UTF8*);//4.在convert中添加配置信息.
fastJsonHttpMessageConverter.setSupportedMediaTypes(fastMediaTypes);fastJsonHttpMessageConverter.setFastJsonConfig(fastJsonConfig);HttpMessageConverter<?> converter = fastJsonHttpMessageConverter;    return new HttpMessageConverters(converter);}
二:在入口springboot类中测试结果; 
**![](https://img-blog.csdn.net/20170501001515869?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWJjY2hlbmd4dXl1YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**
**结果显示和上面的结果是一样的所有两种方式都是有效的,具体使用哪种方法我感觉还是看你自己了!**
