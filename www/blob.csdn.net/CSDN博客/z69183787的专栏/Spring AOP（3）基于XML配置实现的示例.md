# Spring AOP（3）基于XML配置实现的示例 - z69183787的专栏 - CSDN博客
2016年08月15日 13:50:58[OkidoGreen](https://me.csdn.net/z69183787)阅读数：345
个人分类：[Spring-Aop & 代理](https://blog.csdn.net/z69183787/article/category/2175403)
使用注解的方式配置切面类在程序出问题的时候不方便查找问题，尤其是AOP这种tricky的功能，因此在spring配置文件中配置aop是一个相对好的选择（个人感觉）：
```xml
<aop:config>
    <aop:pointcut id="pointcut" expression="execution (* crm.service.*.*(..))"/>
    <aop:aspect ref="aspect">
        <aop:before method="before" pointcut-ref="pointcut"/>
        <aop:after-returning method="afterReturning" pointcut-ref="pointcut" returning="retVal"/>
        <aop:around method="around" pointcut-ref="pointcut"/>
        <aop:after method="afterAdvice" pointcut-ref="pointcut"/>
        <aop:after-throwing method="afterThrowing" arg-names="ex" throwing="ex" pointcut-ref="pointcut" />
    </aop:aspect>
</aop:config>
<bean id="aspect" class="crm.aop.XmlLogAspect"/>
```
XmlLogAspect功能和之前的LogAspect类一致，只是减去注解。
<aop:pointcut/>是声明切入点。
<aop:aspect/>声明切面
<aop:before/><aop:after/>声明前置通知、后置通知
后置返回通知需要配置returning，且规则与注解方式一致，需要保持与后置返回通知方法的参数名一致。
后置异常通知，需要配置args-names和throwing，且保持与方法中异常的参数名一致
