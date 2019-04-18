# 关于AOP无法切入同类调用方法的问题 - z69183787的专栏 - CSDN博客
2015年05月10日 17:44:53[OkidoGreen](https://me.csdn.net/z69183787)阅读数：10338
最近在开发中遇到一个问题，当在Service中定义了一个方法并且切入之后，从Controller里面调用该方法可以实现切入，但是当在同一个Service中实现另一方法并调用改方法时却无法切入。代码类似于：
```java
/**
 * Controller 你懂的
*/
public class Ctrl {
 public void ctrl() {
......
service.callMethodA();
......
}
}
/**
 * 业务实现
*/
public class Service {
/**
 * 被Controller调用的方法
 * 这个方法调用被切的方法
*/
 public void callMethodA() {
......
callMethodB();
......
}
/**
 * Aop切入的方法
*/
 public void callMethodB() {
......
}
}
/**
 * AOP的实现
*/
public class Aspect {
 @@AfterReturning("execution(* Service.callMethodB(..))")
 public void after() {
 Logger.info("after call and do something.");
}
}
```
总之这样的调用根本不会进去切面，找了很久资料都不知道怎么回事，果然我还是太弱了。后来有幸得问高人，指点说Service中如此调用并非调用的是代理类中的方法，是不会被切进去的。换言之，必须要调用代理类才会被切进去。 那么应该怎么破呢？既然只有调用代理类的方法才能切入，那我们拿到代理类不就好了嘛。尝试性的在IDE里面搜Aop相关的类，一眼就看到一个叫AopContext的东西，看来游戏啊，里面有一个方法叫做currentProxy()，返回一个Object。怎么样都要试一下，果断的把这个object打印出来。报错了……![](http://www.thinksaas.cn/images/smiles/icon_confused.gif)
```java
java.lang.IllegalStateException: Cannot find current proxy: Set 'exposeProxy' property on Advised to 'true' to make it available.
```
等等，这个错有点眼熟，之前傻傻找不到问题的时候看过Spring的配置。<aop:aspectj-autoproxy proxy-target-class="true"expose-proxy="true"/>
把expose-proxy设成true，再跑一次，终于没有报错了，而且打印出了令人欣喜的结果，currentProxy()的结果就是当前Service的代理类，那么事情简单了。修改Service的callMethodA()方法如下：
```java
((Service) AopContext.currentProxy()).callMethodB();
```
成功了，方法成功的切入进去了，等下只要再把获取代理类的方法独立处理就好了。等等，突然脑洞大开，既然这样可以，那是不是直接applicationContext.getBean()也可以呢？实验过后得到的结果是可行，而且配置中的expose-proxy也不用设置成true。![](http://www.thinksaas.cn/images/smiles/icon_biggrin.gif) 总算这个问题算是解决了，但是以后在方法调用的时候就需要谨慎了，感觉应该有更好的办法解决这个问题，不知道哪位有更好的办法，能不能告诉我一下下呢？
