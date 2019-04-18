# springmvc支持jsonp - z69183787的专栏 - CSDN博客
2016年08月23日 13:13:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2708
And an additional `ControllerAdvice` like
 this ([see reference
 documentation](http://docs.spring.io/spring-framework/docs/4.1.7.RELEASE/spring-framework-reference/html/mvc.html#mvc-ann-jsonp)):
```java
```
@ControllerAdvice
public class JsonpAdvice extends AbstractJsonpResponseBodyAdvice {
    public JsonpAdvice() {
        super("callback");
    }
}
```
```
That's it.
### 1,新建JsonpAdvice控制器增强
继承org.springframework.web.servlet.mvc.method.annotation.AbstractJsonpResponseBodyAdvice类
```
import org.springframework.web.bind.annotation.ControllerAdvice;
import org.springframework.web.servlet.mvc.method.annotation.AbstractJsonpResponseBodyAdvice;
@ControllerAdvice(basePackages = "com.xxx.web.controller.jsonp")
public class JsonpAdvice extends AbstractJsonpResponseBodyAdvice {
    public JsonpAdvice() {
        super("callback", "jsonp");
    }
}
```
### [](http://www.codesnoopy.com/2016/07/09/%E7%94%A8Spring-MVC-4%E5%AE%9E%E7%8E%B0jsonp%E8%B7%A8%E5%9F%9F%E8%B0%83%E7%94%A8/#2-%E9%9C%80%E8%A6%81jsonp%E7%9A%84controller%E7%B1%BB%E4%BD%BF%E7%94%A8-RestController%E6%B3%A8%E8%A7%A3)2,需要jsonp的controller类使用@RestController注解
```
@RestController
@RequestMapping("/test")
public class RankingListController {
    @ResponseBody
    @RequestMapping(value = "/getPopularity", method = RequestMethod.GET, produces = "application/json;charset=UTF-8")
    //注意这里只能返回对象,JavaBean或JsonObject都可以，不能用String
    public JSONObject getPopularity() {
        return JSON.parseObject((String) redisClient.getObject(popularityRankingListKey));
    }
}
```
### [](http://www.codesnoopy.com/2016/07/09/%E7%94%A8Spring-MVC-4%E5%AE%9E%E7%8E%B0jsonp%E8%B7%A8%E5%9F%9F%E8%B0%83%E7%94%A8/#3-%E8%B0%83%E7%94%A8)3,调用
#### [](http://www.codesnoopy.com/2016/07/09/%E7%94%A8Spring-MVC-4%E5%AE%9E%E7%8E%B0jsonp%E8%B7%A8%E5%9F%9F%E8%B0%83%E7%94%A8/#%E6%96%B9%E5%BC%8F%E4%B8%80)方式一
```
<html>
<head>
<meta charset="UTF-8" />
<script src="http://apps.bdimg.com/libs/jquery/2.1.4/jquery.min.js"></script>
</head>
<body>
<div>
请用Chrome浏览器打开,返回结果在F12 Console日志显示。
</div>
<script language="JavaScript">
$(document).ready(function() {
	$.ajax({
		url:'http://localhost:8080/test/getPopularity',
		dataType:'jsonp',
		success:function(data){
			console.log("返回Json:")
			console.log(data)
		}
	});
});
</script>
</body>
</html>
```
#### [](http://www.codesnoopy.com/2016/07/09/%E7%94%A8Spring-MVC-4%E5%AE%9E%E7%8E%B0jsonp%E8%B7%A8%E5%9F%9F%E8%B0%83%E7%94%A8/#%E6%96%B9%E5%BC%8F%E4%BA%8C)方式二
```
http://localhost:8080/test/getPopularity?callback=jsonp
```

