# SpringMvc 支持 jsonP - z69183787的专栏 - CSDN博客
2016年12月02日 11:45:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：574
原理：http://blog.csdn.net/z69183787/article/details/52290032
1、基于Controller 注解
Advice：
```java
@ControllerAdvice(annotations = RestController.class)
public class JsonpAdvice extends AbstractJsonpResponseBodyAdvice {
    public JsonpAdvice() {
        super("callback","jsonp");
    }
}
```
controller：
```java
@RestController
@RequestMapping("/api")
public class ApiController {}
```
2、基于包名
Advice：xxxxx包名下的所有Controller，且返回为 responseBody
```java
@ControllerAdvice(basePackages = "com.xxxxxxx")
public class JsonpAdvice extends AbstractJsonpResponseBodyAdvice {
    public JsonpAdvice() {
        super("callback", "jsonp");
    }
}
```
controller：
```java
@Controller
@RequestMapping("/api")
public class ApiController {}
```
