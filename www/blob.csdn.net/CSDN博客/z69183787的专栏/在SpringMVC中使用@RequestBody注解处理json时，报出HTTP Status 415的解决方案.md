# 在SpringMVC中使用@RequestBody注解处理json时，报出HTTP Status 415的解决方案 - z69183787的专栏 - CSDN博客
2016年08月24日 17:03:00[OkidoGreen](https://me.csdn.net/z69183787)阅读数：8144
Spring的@RequestBody非常牛x，可以将提交的json直接转换成POJO对象。
正好今天有这样的需求，使用一下，结果一直报415，十分头疼。
HTTP 415 错误 – 不支持的媒体类型(Unsupported media type)
我的angularJs是这样写的
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
$http({method: "POST",
    url: url;
    headers: {'Content-type': 'application/json;charset=UTF-8'},
    data: scope.$modelValue})
.success(function(data, status) {
    // success handle code  
})
.error(function(data, status) {
    // error handle code
});
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
url与scope.$modelValue都是项目中的代码，在这里占个坑，scope.$modelValue是一个js对象，会被angularJs转换成json字符串，
反复看angularJs的文档，又抓包分析，确认js没有问题。
在网上一查貌似是Spring的问题，有的网友说需要在*-servlet.xml中增加<mvc:annotation-driven />，一看我的项目没加，立刻加上。
当然还需要加上mvc的xml命名空间，否则该配置无法解析。
```
xmlns:mvc="http://www.springframework.org/schema/mvc"
xsi:schemaLocation="http://www.springframework.org/schema/mvc
http://www.springframework.org/schema/mvc/spring-mvc-3.0.xsd"
```
<mvc:annotation-driven />会自动注册DefaultAnnotationHandlerMapping和AnnotationMethodHandlerAdapter两个bean
AnnotationMethodHandlerAdapter将会初始化7个转换器，可以通过调用AnnotationMethodHandlerAdapter的getMessageConverts()方法来获取转换器的一个集合 List<HttpMessageConverter>
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
ByteArrayHttpMessageConverter 
StringHttpMessageConverter 
ResourceHttpMessageConverter 
SourceHttpMessageConverter 
XmlAwareFormHttpMessageConverter 
Jaxb2RootElementHttpMessageConverter 
MappingJacksonHttpMessageConverter
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
对于json的解析就是通过MappingJacksonHttpMessageConverter转换器完成的。
只添加<mvc:annotation-driven />还不行，需要在classpath环境中能找到Jackson包，用maven配置如下
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 <dependency>  
        <groupId>org.codehaus.jackson</groupId>  
        <artifactId>jackson-mapper-asl</artifactId>  
        <version>1.9.8</version>  
        <type>jar</type>  
        <scope>compile</scope>  
 </dependency>  
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
至此问题解决，附上Spring代码
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
@RequestMapping(value = "/testjson", method = RequestMethod.POST, consumes = "application/json")
@ResponseBody
public void testJson(@RequestBody JsonInfo jsonInfo,
    HttpServletRequest request, HttpServletResponse response)
{
    //handle jsonInfo object instance
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
 从下文得到帮助，对作者表示感谢：）
http://snowolf.iteye.com/blog/1628861
