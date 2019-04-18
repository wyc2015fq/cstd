# Spring AOP-通知顺序 - z69183787的专栏 - CSDN博客
2017年06月23日 10:45:17[OkidoGreen](https://me.csdn.net/z69183787)阅读数：823
[https://my.oschina.net/mushui/blog/161273](https://my.oschina.net/mushui/blog/161273)
如果我们有多个通知想要在同一连接点执行，那执行顺序如何确定呢？Spring AOP使用AspectJ的优先级规则来确定通知执行顺序。
    总共有两种情况：同一切面中通知执行顺序、不同切面中的通知执行顺序。
    同一切面中通知执行顺序：
    1. 前置通知/环绕通知proceed方法之前部分    --执行顺序根据配置顺序决定
    2. 被通知方法
    3. 后置方法/环绕通知proceed方法之后部分    --执行顺序根据配置顺序决定
前置通知与环绕通知proceed之前的顺序是根据配置顺序决定的，比如：
```
<aop:aspect ref="aspect">
    <aop:around method="around" pointcut-ref="pointcut"/>
    <aop:before method="before" pointcut-ref="pointcut"/>
    <aop:after-returning method="afterReturning" pointcut-ref="pointcut" returning="retVal"/>
    <aop:after method="afterAdvice" pointcut-ref="pointcut"/>
    <aop:after-throwing method="afterThrowing" arg-names="ex" throwing="ex" pointcut-ref="pointcut" />
</aop:aspect>
```
环绕通知在前置通知之前，则会先执行环绕通知proceed之前的部分。
同样环绕通知与后置通知、后置返回通知的顺序也是根据配置顺序决定的。因此在环绕通知中修改了返回值，根据配置的顺序结果会不一样。
不同切面中的通知执行顺序：当定义在不同切面的相同类型的通知需要在同一个连接点执行，如果没指定切面的执行顺序，这两个通知的执行顺序将是未知的。
如果需要他们顺序执行，可以通过指定切面的优先级来控制通知的执行顺序。Spring中可以通过在切面实现类上实现org.springframework.core.Ordered接口或使用Order注解来指定切面优先级。在多个切面中，Ordered.getValue()方法返回值（或者注解值）较小值的那个切面拥有较高优先级：
```java
public class XmlLogAspect implements Ordered {
    private int order = 1;
    private static final Logger LOGGER = LoggerFactory.getLogger(XmlLogAspect.class);
    public void publicPointcut(){}
    public void before() {
        LOGGER.info("支付前打印一点字.........");
    }
    public void afterAdvice() {
        LOGGER.info("后置通知，支付完成了呵呵............");
    }
    // 注意，此处returning后的值必须与方法参数名的值一致
    public void afterReturning(String retVal) {
        LOGGER.info("支付结果如何，hoho检查检查：" + retVal);
    }
    public void afterThrowing(Exception ex) {
        LOGGER.info(ex.getMessage());
    }
    // 此方法必须有返回值，否则会使得被代理的方法的返回值变为null
    public Object around(ProceedingJoinPoint joinPoint) throws Throwable {
        LOGGER.info("支付方法中，先来一下........");
        Object result = joinPoint.proceed();
        LOGGER.info("支付方法结束了，走你.........");
        return result + "add";
    }
    @Override
    public int getOrder() {
        return this.order;
    }
    public void setOrder(int order) {
        this.order = order;
    }
}
```
使用@Order注解更加简单：
```java
@Order(2)
public class XmlLogAspect {
private static final Logger LOGGER = LoggerFactory.getLogger(XmlLogAspect.class);
    public void publicPointcut(){}
    public void before() {
        LOGGER.info("支付前打印一点字.........");
    }
    public void afterAdvice() {
        LOGGER.info("后置通知，支付完成了呵呵............");
    }
    // 注意，此处returning后的值必须与方法参数名的值一致
    public void afterReturning(String retVal) {
        LOGGER.info("支付结果如何，hoho检查检查：" + retVal);
    }
}
```
或者在xml中配置：
```
<aop:aspect ref="aspect1" order="1">
……
</aop:aspect>
```
