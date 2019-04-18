# SpringAop之ProxyConfig - z69183787的专栏 - CSDN博客
2018年07月28日 09:59:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：99
[https://www.jianshu.com/p/1f8dbeadd79d](https://www.jianshu.com/p/1f8dbeadd79d)
SpringAop模块中，我们先从入门级的ProxyFactoryBean讲起，它的继承关系主要为ProxyConfig->AdvisedSupport->ProxyCreatorSupport->ProxyFactoryBean ，每个类都有各自特定的功能
ProxyConfig这个类的功能是维护几个在Spring代理配置中比较通用的属性，spring中和代理生成有关的类都继承了这个类
具体属性有以下5个
```java
private boolean proxyTargetClass = false;
private boolean optimize = false;
boolean opaque = false;
boolean exposeProxy = false;
private boolean frozen = false;
```
具体功能为:
- proxyTargetClass，true代表直接代理类，false代表代理接口。默认为false
- optimize，是否执行某些优化，感觉基本没怎么用到
- opaque，代表子类是否能被转换为Advised接口，默认为false，表示可以
- exposeProxy，是否暴露代理，也就是是否把当前代理对象绑定到AopContext的ThreadLocal属性currentProxy上去，常用于代理类里面的代理方法需要调用同类里面另外一个代理方法的场景。
- frozen，当前代理配置是否被冻结，如果被冻结，配置将不能被修改
## exposeProxy详解
在实际开发过程中，我们有时候会碰到一个类中2个方法嵌套调用，如果这两个方法都有切面逻辑，比如事务，会发现内部嵌套的那个方法切面会失效，exposeProxy就是为这种场景做的兼容
为什么这个代理会失效?Spring对方法生成代理，当我们对代理对象方法调用时，这个外层方法的切面逻辑肯定是生效的，但是内部的那个方法，**对应方法的对象根本不是代理对象**，是当前对象，所以根本就没有切面逻辑。
所以我们需要让内部方法切面逻辑生效，就必须拿到代理对象！
在代理类的逻辑中，会判断exposeProxy是否为true，如果为true的话，会把当前的代理对象绑定到AopContext的ThreadLocal属性currentProxy上去，那么我们就能在被切方法中通过AopContext.currentProxy()来获取这个代理对象，进行调用。
比如，在JdkDynamicAopProxy的invoke方法中有
```java
if (this.advised.exposeProxy) {
                // Make invocation available if necessary.
                oldProxy = AopContext.setCurrentProxy(proxy);
                setProxyContext = true;
            }
```
另外一种方式的话，就是在从ApplicationContext再把这个代理对象取出来调用。
下面来看下如何使用，我们以ProxyFactory作为例子
```java
public interface HelloService {
    void hello(String name);
    void bye(String name);
}
public class HelloServiceImpl implements HelloService{
    @Override
    public void hello(String name) {
        System.out.println("hello"+name);
        ((HelloService) AopContext.currentProxy()).bye(name);
    }
    @Override
    public void bye(String name) {
        System.out.println("bye"+name);
    }
}
public class TestAfterAdvice implements AfterReturningAdvice{
    @Override
    public void afterReturning(Object returnValue, Method method, Object[] args, Object target) throws Throwable {
        System.out.println("after ...");
    }
}
public static void main(String[] args) {
        ProxyFactory proxyFactory =new ProxyFactory();
        proxyFactory.setInterfaces(HelloService.class);
        proxyFactory.setTarget(new HelloServiceImpl());
        proxyFactory.addAdvice(new TestBeforeAdvice());
        proxyFactory.addAdvice(new TestAfterAdvice());
        proxyFactory.addAdvice(new TestAroundAdvice());
        proxyFactory.setExposeProxy(true);
        HelloService helloService = (HelloService) proxyFactory.getProxy();
        helloService.bye("scj");
        helloService.hello("scj");
    }
}
```
通过proxyFactory.setExposeProxy(true);设置exposeProxy=true之后，在hello方法中就可以通过 ((HelloService) AopContext.currentProxy()).bye(name);来调用代理的bye方法
