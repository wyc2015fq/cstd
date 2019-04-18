# Spring Boot系列——AOP配自定义注解的最佳实践 - z69183787的专栏 - CSDN博客
2019年04月02日 14:57:01[OkidoGreen](https://me.csdn.net/z69183787)阅读数：31
[https://www.cnblogs.com/bigdataZJ/p/springboot-aop.html](https://www.cnblogs.com/bigdataZJ/p/springboot-aop.html)
AOP(Aspect Oriented Programming)，即面向切面编程，是Spring框架的大杀器之一。
首先，我声明下，我不是来系统介绍什么是AOP，更不是照本宣科讲解什么是连接点、切面、通知和切入点这些让人头皮发麻的概念。
今天就来说说AOP的一些应用场景以及如何通过和其他特性的结合提升自己的灵活性。
### AOP应用举例
AOP的一大好处就是解耦。通过切面，我们可以将那些反复出现的代码抽取出来，放在一个地方统一处理。
同时，抽出来的代码很多是与业务无关的，这样可以方便开发者更加专注自己的业务逻辑的开发。
一个AOP的典型应用场景就是日志打印。
下面是一个极端情况的Controller
```
@RestController
@RequestMapping("/")
public class HelloController {
    private static final Logger LOG = LoggerFactory.getLogger(HelloController.class);
    @GetMapping(value = "/index")
    public String index(HttpServletRequest request) {
        LOG.info("============打印日志开始============");
        LOG.info("URL: " + request.getRequestURL().toString());
        LOG.info("============打印日志结束============");
        return "hello jackie";
    }
    @GetMapping(value = "/test1")
    public String test1(HttpServletRequest request, String var1) {
        LOG.info("============打印日志开始============");
        LOG.info("URL: " + request.getRequestURL().toString());
        LOG.info("============打印日志结束============");
        return "test1";
    }
    @DemoAnnotation
    @GetMapping(value = "/test2")
    public String test2(HttpServletRequest request, String var1, String var2) {
        LOG.info("============打印日志开始============");
        LOG.info("URL: " + request.getRequestURL().toString());
        LOG.info("============打印日志结束============");
//        int i = 1/0;
        if (1<2)
            throw new IllegalArgumentException("exception");
        return "test2";
    }
}
```
HelloController中提供了三个Http接口，由于业务需要，所以每次进入某个方法的时候都需要打印请求的相关信息。
当然，如果只是上面的例子，我们完全可以通过其他手段让代码看着并不这么糟糕。我们可以抽象一个打印方法，将相同的代码封装在这个方法中，之后在各个方法中每次调用即可。
但是，这种处理方法似乎抽象的还不够，因为我们在每个Http接口中还是要调用这个抽象的函数。而且，比较要命的是，这打印日志的代码与其他业务代码显得有些格格不入。
所以，这时候，我们想到了AOP。
### 如何使用AOP
在Spring Boot项目中，只需要如下几步，就可以轻松上手AOP。
添加maven依赖
```
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-aop</artifactId>
</dependency>
```
编写切面类
```
@Aspect
@Component
public class DemoAspect {
    private static final Logger LOG = LoggerFactory.getLogger(DemoAspect.class);
    @Pointcut("execution(public * com.jackie.springbootdemo.controller.HelloController.test*(..))")
    public void addAdvice(){}
    @Before("addAdvice()")
    public void before(JoinPoint joinPoint){
        Object[] args = joinPoint.getArgs();
        HttpServletRequest requests = (HttpServletRequest) args[0];
        LOG.info("============打印日志开始============");
        LOG.info("URL: " + requests.getRequestURL().toString());
        LOG.info("============打印日志结束============");
//        LOG.info("before....");
    }
}
```
结果验证对比
启动SpringBootDemoApplication，访问url：http://localhost:8080/test2?var1=1&var2=2
未使用切面功能打印日志
![](https://img2018.cnblogs.com/blog/619240/201811/619240-20181111164949249-1050215126.gif)
使用切面功能打印日志
![](https://img2018.cnblogs.com/blog/619240/201811/619240-20181111164958424-635929859.gif)
从上面的结果展示发现，最终的效果是一样的，但是使用切面更加简洁，而且可复用。
如上访问的是test2接口，如果访问test1接口也可以走切面类实现打印日志的需求，但是如果走index请求就不会打印日志了。
这是为什么呢？
### AOP的局限
在切面类DemoAspect中，我们看到了切入点的设置
```
@Pointcut("execution(public * com.jackie.springbootdemo.controller.HelloController.test*(..))")
public void addAdvice(){}
```
其中Pointcut后面的表达式是用于控制切面的有效影响范围。
**表达式中，第一个*表示返回任意类型，第二个*表示任意方法名，后面的小括号表示任意参数值，这里是以test为前缀的，所以可以匹配上test1和test2方法。
注意，在第二个*之前也可以再有个*，即HelloController所在位置，表示任意类名，假如这里是有两个*.则表示包括包里面的子包。**
好了，明白了表达式的含义，我们自然就看到了AOP的局限性。
当我们要使用切面前，就要写好表达式，但是项目一直在做，代码一直在加，那谁能保证后面接收代码的兄弟也正好知道这个test前缀的意义这么重大呢？
如果他非要用hello作为前缀，那么本应该匹配到的接口就匹配不上了，日志也就不能正常打印了。
这时候，自定义注解，就能够很好的解决这个问题。
### 自定义注解配合AOP
新建一个自定义注解
```
@Documented
@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.METHOD)
public @interface DemoAnnotation {
}
```
自定义注解的花样也很多，比如可以在注解中声明变量等，但这些不是我们这次讨论的重点。
将该注解添加到Http接口test2方法上
```
@DemoAnnotation
    @GetMapping(value = "/test2")
    public String test2(HttpServletRequest request, String var1, String var2) {
//        LOG.info("============打印日志开始============");
//        LOG.info("URL: " + request.getRequestURL().toString());
//        LOG.info("============打印日志结束============");
//        int i = 1/0;
        if (1<2)
            throw new IllegalArgumentException("exception");
        return "test2";
    }
```
在切面类中将切入点的表达式改为
```
@Pointcut("execution(public * com.jackie.springbootdemo.controller.*.*(..)) && @annotation(com.jackie.springbootdemo.annotation.DemoAnnotation)")
public void addAdvice(){}
```
这样，我们不需要限制在controller类中是以test作为前缀了，只要是在上面定义的类路径下，并且扫描到注解DemoAnnotation就可以让切面生效。
![](https://img2018.cnblogs.com/blog/619240/201811/619240-20181111165010880-173972568.gif)
从结果可以看出，访问http://localhost:8080/test1?var1=1并没有经过切面处理，因为不满足切入点中的表达式要求。
这样做的好处在于，控制的粒度更细，也更加灵活，方便切面功能的实现和细分。
