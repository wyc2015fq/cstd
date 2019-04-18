# Spring-MVC开发之全局异常捕获全面解读 - z69183787的专栏 - CSDN博客
2017年01月20日 11:13:17[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2975
异常，异常
我们一定要捕获一切该死的异常，宁可错杀一千也不能放过一个！
产品上线后的异常更要命，一定要屏蔽错误内容，以免暴露敏感信息！
在用Spring MVC开发WEB应用时捕获全局异常的方法基本有两种，
- 
WEB.XML，就是指定error-code和page到指定地址，这也是最传统和常见的做法
- 
用Spring的全局异常捕获功能，这种相对可操作性更强一些，可根据自己的需要做一后善后处理，比如日志记录等。
SO，本文列出Spring-MVC做WEB开发时常用全局异常捕获的几种解决方案抛砖引玉
互相没有依赖，每个都可单独使用！
## 定义服务器错误WEB.XML整合Spring MVC
web.xml
```
<error-page>
    <error-code>404</error-code>
    <location>/404</location>
</error-page>
<error-page>
    <error-code>500</error-code>
    <location>/500</location>
</error-page>
<!-- 未捕获的错误，同样可指定其它异常类，或自定义异常类 -->
<error-page>
    <exception-type>java.lang.Exception</exception-type>
    <location>/uncaughtException</location>
</error-page>
```
applicationContext.xml
```
<!-- 错误路径和错误页面，注意指定viewResolver -->
<mvc:view-controller path="/404" view-name="404"/>
<mvc:view-controller path="/500" view-name="500"/>
<mvc:view-controller path="/uncaughtException" view-name="uncaughtException"/>
```
## Spring全局异常，代码入侵方式
异常抛出
```
@Controller
public class MainController {
    @ResponseBody
    @RequestMapping("/")
    public String main(){
        throw new NullPointerException("NullPointerException Test!");
    }
}
```
异常捕获
```
//注意使用注解@ControllerAdvice作用域是全局Controller范围
//可应用到所有@RequestMapping类或方法上的@ExceptionHandler、@InitBinder、@ModelAttribute，在这里是@ExceptionHandler
@ControllerAdvice
public class AControllerAdvice {
    @ExceptionHandler(NullPointerException.class)
    @ResponseStatus(HttpStatus.BAD_REQUEST)
    @ResponseBody
    public String handleIOException(NullPointerException ex) {
        return ClassUtils.getShortName(ex.getClass()) + ex.getMessage();
    }
}
```
## Spirng全局异常，配置方式
异常抛出，同上！
异常捕获
```
<bean class="org.springframework.web.servlet.handler.SimpleMappingExceptionResolver">
    <!-- 默认错误页面，就是不在exceptionMappings指定范围内 -->
    <property name="defaultErrorView" value="uncaughtException" />
    <property name="exceptionMappings">
        <props>
            <!-- 异常类名，可以是全路径，错误页面或Controller路径！ -->
            <prop key=".NullPointerException">NullPointerException</prop>
            <prop key="java.io.IOException">IOException</prop>
        </props>
    </property>
</bean>
```
## Sping全局异常，自定义异常类和异常解析
自定义异常类：
```
public class CustomException extends RuntimeException {
    public CustomException(){
        super();
    }
    public CustomException(String msg, Throwable cause){
        super(msg, cause);
        //Do something...
    }
}
```
抛出异常
```
@ResponseBody
@RequestMapping("/ce")
public String ce(CustomException e){
    throw new CustomException("msg",e);
}
```
实现异常捕获接口HandlerExceptionResolver
```
public class CustomHandlerExceptionResolver implements HandlerExceptionResolver{
    @Override
    public ModelAndView resolveException(HttpServletRequest httpServletRequest, HttpServletResponse httpServletResponse, Object o, Exception e) {
        Map<String, Object> model = new HashMap<String, Object>();
        model.put("e", e);
        //这里可根据不同异常引起类做不同处理方式，本例做不同返回页面。
        String viewName = ClassUtils.getShortName(e.getClass());
        return new ModelAndView(viewName, model);
    }
}
```
配置Spring支持异常捕获
`<bean class="cn.bg.controller.CustomHandlerExceptionResolver"/>`
完！
