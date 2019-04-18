# 关于spring中 @Aspect的不起作用的问题 - z69183787的专栏 - CSDN博客
2012年12月05日 20:05:34[OkidoGreen](https://me.csdn.net/z69183787)阅读数：11444
**今天用到 spring中的aop ，@Aspect 注解，当我将@Before中的 **
**描述指定为 单独的1个类时，aop 不起作用；**
**而 当作用在1个 实现某个接口的实现类后，**
**或者继承于某个超类时**
**就奏效了。**
**或许这就是Aspect编程的一种模式吧。**
**2016年6月30**
**注解时  切面类同时要配置bean 和 aspect 注解，不然无法切入，以前一直以为只需要配置 @aspect**
**@Aspect@Componentpublic class AuthAspect {**
