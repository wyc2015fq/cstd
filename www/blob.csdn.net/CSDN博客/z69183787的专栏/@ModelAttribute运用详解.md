# @ModelAttribute运用详解 - z69183787的专栏 - CSDN博客
2015年10月07日 19:15:35[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2913
被@ModelAttribute注释的方法会在此controller每个方法执行前被执行，因此对于一个controller映射多个URL的用法来说，要谨慎使用。
我们编写控制器代码时，会将保存方法独立成一个控制器也是如此。
[1.@ModelAttribute](mailto:1.@ModelAttribute)注释void返回值的方法
```
@Controller
public class HelloModelController {
    
    @ModelAttribute 
    public void populateModel(@RequestParam String abc, Model model) {  
       model.addAttribute("attributeName", abc);  
    }  
    @RequestMapping(value = "/helloWorld")  
    public String helloWorld() {  
       return "helloWorld.jsp";  
    }  
}
```
在这个代码中，访问控制器方法helloWorld时，会首先调用populateModel方法，将页面参数abc(/helloWorld.ht?abc=text)放到model的attributeName属性中，在视图中可以直接访问。
jsp页面页面如下。
```
<%@ page language="java" contentType="text/html; charset=utf-8"
    pageEncoding="utf-8"%>
<%@taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core"%>
<html>
<head>
</head>
<body>
<c:out value="${attributeName}"></c:out>
</body>
</html>
```
[2.@ModelAttribute](mailto:2.@ModelAttribute)注释返回具体类的方法
```
@Controller
public class Hello2ModelController {
    
    @ModelAttribute 
    public User populateModel() {  
       User user=new User();
       user.setAccount("ray");
       return user;
    }  
    @RequestMapping(value = "/helloWorld2")  
    public String helloWorld() {  
       return "helloWorld.jsp";  
    }  
}
```
当用户请求 [http://localhost:8080/test/helloWorld2.ht](http://localhost:8080/test/helloWorld2.ht)时，首先访问populateModel方法，返回User对象，model属性的名称没有指定，
它由返回类型隐含表示，如这个方法返回User类型，那么这个model属性的名称是user。 
这个例子中model属性名称有返回对象类型隐含表示，model属性对象就是方法的返回值。它无须要特定的参数。
jsp 中如下访问：
```
<c:out value="${user.account}"></c:out>
```
也可以指定属性名称
```
@Controller
public class Hello2ModelController {
    
    @ModelAttribute(value="myUser")
    public User populateModel() {  
       User user=new User();
       user.setAccount("ray");
       return user;
    }  
    @RequestMapping(value = "/helloWorld2")  
    public String helloWorld(Model map) {  
       return "helloWorld.jsp";  
    }  
}
```
jsp中如下访问：
```
<c:out value="${myUser.account}"></c:out>
```
对象合并:
```
@Controller
public class Hello2ModelController {
    
    @ModelAttribute
    public User populateModel() {  
       User user=new User();
       user.setAccount("ray");
       return user;
    }  
    
    @RequestMapping(value = "/helloWorld2")  
    public String helloWorld(User user) {
        user.setName("老王");
       return "helloWorld.jsp";  
    }  
}
```
这个在编写代码的时候很有用处,比如在更新的时候，我们可以现在populateModel方法中根据ID获取对象，然后使用spring mvc的自动组装功能，组装
User对象，这样在客户端提交了值的属性才会被组装到对象中。
比如：User对象，首先从数据库中获取此对象，客户端表单只有account属性，提交时就只会改变account属性。
对象合并指定对象名称：
```
@Controller
public class Hello2ModelController {
    
    @ModelAttribute("myUser")
    public User populateModel() {  
       User user=new User();
       user.setAccount("ray");
       return user;
    }  
    
    @RequestMapping(value = "/helloWorld2")  
    public String helloWorld(@ModelAttribute("myUser") User user) {
        user.setName("老王");
       return "helloWorld.jsp";  
    }  
}
```
这样在jsp中可以使用如下方式访问
```
<c:out value="${myUser.name}"></c:out>
<c:out value="${myUser.account}"></c:out>
```
3.通过此特性控制权限.
我们可以在基类方法中控制写此注解，需要控制权限的控制器，继承控制器就可以了。
```
public class BaseController {
    
    @ModelAttribute
    public void populateModel() throws Exception {  
       SysUser user=ContextUtil.getCurrentUser();
       if(user.getAccount().equals("admin")){
           throw new Exception("没有权限");
       }
    }  
}
```
需要控制权限的类继承BaseController
```
@Controller
public class Hello2ModelController extends BaseController {
    
    @RequestMapping(value = "/helloWorld2")  
    public String helloWorld(@ModelAttribute("myUser") User user) {
        user.setName("老王");
       return "helloWorld.jsp";  
    }  
}
```
这样就可以控制权限了，当然控制权限的方法有很多，比如通过过滤器等。这里只是提供一种思路。
