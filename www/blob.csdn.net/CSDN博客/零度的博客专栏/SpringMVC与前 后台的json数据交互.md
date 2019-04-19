# SpringMVC与前/后台的json数据交互 - 零度的博客专栏 - CSDN博客
2017年12月12日 09:53:56[零度anngle](https://me.csdn.net/zmx729618)阅读数：9430
json数据格式在接口调用中、html页面中比较常用，json格式比较简单，解析也比较方便，所以使用很普遍。在SpringMVC中，也支持对json数据的解析和转换，这篇文章主要总结一下在SpringMVC中如何和前台交互json数据。
# 两种交互形式
SpringMVC和前台交互主要有两种形式，如下图所示： 
![](http://images2015.cnblogs.com/blog/48590/201705/48590-20170527140316950-1204883095.png)
可以看出，前台传过来的方式有两种，一种是传json格式的数据过来，另一种就是在url的末尾传普通的key/value串过来，针对这两种方式，在Controller类中会有不同的解析，但是在Controller类中返回的json格式的数据都是一样的。下面来具体分析一下SpringMVC是如何与前台进行json数据的交互的。在讲之前先认识两个注解。
# @RequestBody注解
@RequestBody注解用于读取http请求的内容(字符串)，通过SpringMVC提供的HttpMessageConverter接口将读到的内容转换为json、xml等格式的数据并绑定到Controller类方法的参数上。 
本例子应用：@RequestBody注解实现接收http请求的json数据，将json数据转换为java对象。如下： 
![](http://images2015.cnblogs.com/blog/48590/201705/48590-20170527140328825-1849477734.png)
![](http://images2015.cnblogs.com/blog/48590/201705/48590-20170527140344419-1742253803.png)
# @ResponseBody注解
@ResponseBody注解用于将Controller类的方法返回的对象，通过HttpMessageConverter接口转换为指定格式的数据如：json、xml等，通过Response响应给客户端。 
本例子应用：@ResponseBody注解实现将Controller类方法返回对象转换为json响应给客户端，如下： 
![](http://images2015.cnblogs.com/blog/48590/201705/48590-20170527140357779-25279971.png)
![](http://images2015.cnblogs.com/blog/48590/201705/48590-20170527140545669-1401070681.png)
经过我如此细致地讲解，想必大家已认识到这两个注解的意思了。好了，下面来具体分析一下SpringMVC是如何与前台进行json数据的交互的。
# 环境的准备
## 加载json的jar包
SpringMVC默认用MappingJacksonHttpMessageConverter对json数据进行转换，需要加入jackson的包，又因为SpringMVC3和SpringMVC4针对json交互的jar包有区别，我用的是SpringMVC4，需要导入如下三个jar包： 
![](http://images2015.cnblogs.com/blog/48590/201705/48590-20170527140627857-1048637068.png)
读者千万不要忘了导入jQuery的类库，因为我是使用jQuery的ajax提交json串的，就像下图这样： 
![](http://images2015.cnblogs.com/blog/48590/201705/48590-20170527140637622-2073293882.png)
## 配置json转换器
配置json转换器有两种方式，如果是配置了注解适配器org.springframework.web.servlet.mvc.annotation.AnnotationMethodHandlerAdapter的话，需要在该适配器中配置json转换器，如下：
```
<!-- 用于将对象转换为 JSON  -->  
<bean id="stringConverter" class="org.springframework.http.converter.StringHttpMessageConverter">  
    <property name="supportedMediaTypes">  
        <list>  
            <value>text/plain;charset=UTF-8</value>  
        </list>  
    </property>  
</bean>  
<bean id="jsonConverter" class="org.springframework.http.converter.json.MappingJackson2HttpMessageConverter"></bean>  
<bean class="org.springframework.web.servlet.mvc.annotation.AnnotationMethodHandlerAdapter">  
    <property name="messageConverters">  
        <list>  
            <ref bean="stringConverter" />  
            <ref bean="jsonConverter" />  
        </list>  
    </property>  
</bean>
```
但是如果使用`<mvc:annotation-driven />`注解驱动的话就不用以上的配置了，默认已经配好了。建议使用这种，比较方便。
# json交互的测试
这里，我使用jQuery的ajax提交json串，对输出的json结果进行解析。前台的程序如下：
```
<button onclick="sendJson()">json数据交互测试</button>
<script type="text/javascript">
    function sendJson() {
        $.ajax({
            type:"post",
            url:"${pageContext.request.contextPath }/item/json_test.action",
            data:'{"id":"1","name":"电冰箱","price":"1999"}',
            contentType:"application/json;charset=utf-8",
            success:function(data) {
                alert(data.id + ":" + data.name);
            }
        });
    }
</script>
```
那么前台itemList.jsp页面的内容就应改造为： 
![](http://images2015.cnblogs.com/blog/48590/201705/48590-20170527140656810-388787205.png)
接着编辑ItemController类，并在该类中编写如下方法：
```
// JSON数据交互
// @RequestBody：接收json数据并转换成pojo对象
// @ResponseBody：响应json数据，把pojo对象转换成json数据并响应
@RequestMapping("/json_test")
@ResponseBody
public Items jsonTest(@RequestBody Items items) {
    return items;
}
```
由于前台传的是id、name和price三个属性，所以在后台就用Items类来接收了，这个类中也有这三个属性。重点是@RequestBody注解，它是将前台传过来的json串转换成items对象，然后再将该对象return回去，通过@ResponseBody注解将items对象转成json格式返回给前台。这样前台接收到了后就可以解析了。我们看一下测试的结果： 
![](http://images2015.cnblogs.com/blog/48590/201705/48590-20170527140714419-1538346551.png)
响应的结果，null表示空值而已，这里就返回了原对象，也就只有id、name和price属性。 
![](http://images2015.cnblogs.com/blog/48590/201705/48590-20170527140758325-95009939.png)
springmvc与json数据的交互就总结到此。
