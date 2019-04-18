# 用 Java 实现拦截器 Interceptor 的拦截功能 - z69183787的专栏 - CSDN博客
2018年01月30日 10:59:59[OkidoGreen](https://me.csdn.net/z69183787)阅读数：827
　Java 里的拦截器是动态拦截 action 调用的对象。它提供了一种机制可以使开发者可以定义在一个 action 执行的前后执行的代码，也可以在一个 action 执行前阻止其执行，同时也提供了一种可以提取 action 中可重用部分的方式。在AOP（Aspect-Oriented Programming）中拦截器用于在某个方法或字段被访问之前进行拦截，然后在之前或之后加入某些操作。
　　此外，拦截器在流行的开源框架中也很常见，其依赖的技术就是 Java 的动态代理。理解拦截器的核心原理对理解这些开源框架的体系结构至关重要。下面，咱们就以一个简单的模型的来说明拦截器实现的一般方法。模型主要分为五个模块，分别：
- 业务组件，被代理和被拦截的对象；
- 代理处理器，实现了 InvocationHandler 接口的一个对象；
- 代理对象，Proxy 对象；
- 拦截器，普通的 JavaBean，在调用业务方法之前或者之后会自动拦截并执行自己的一些方法；
- 客户端，执行业务处理的入口。
接下来，咱们就用 Java 语言来实现拦截器 Interceptor 的拦截功能：
第 1 步：创建业务组件接口 BusinessFacade
```java
/**
 * @author 维C果糖
 * @create 2017-03-30
 *
 * GitHub：github.com/guobinhit
 *
 * 业务组件接口
 */
public interface BusinessFacade {
    public void doSomething();
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
第 2步：创建业务组件实现类 BusinessClass
```java
/**
 * @author 维C果糖
 * @create 2017-03-30
 *
 * GitHub：github.com/guobinhit
 *
 * 业务组件接口的实现类
 */
public class BusinessClass implements BusinessFacade {
    public void doSomething() {
        System.out.println("在业务组件 BusinessClass 中调用方法: doSomething()");
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
第 3 步：创建拦截器 InterceptorClass
```java
/**
 * @author 维C果糖
 * @create 2017-03-30
 *
 * GitHub：github.com/guobinhit
 *
 * 拦截器
 */
public class InterceptorClass {
    // 在 action 之前调用
    public void before(){
        System.out.println("在拦截器 InterceptorClass 中调用方法: before()");
    }
    // 在 action 之后调用
    public void after(){
        System.out.println("在拦截器 InterceptorClass 中调用方法: after()");
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
第 4 步：创建动态代理处理器工具 DynamicProxyHandler
```java
/**
 * @author 维C果糖
 * @create 2017-03-30
 *
 * GitHub：github.com/guobinhit
 *
 * 动态代理处理器工具
 */
public class DynamicProxyHandler implements InvocationHandler {
    // 声明被代理对象
    private Object business;
    // 创建拦截器
    private InterceptorClass interceptor = new InterceptorClass();
    /**
 　　* 动态生成一个代理类对象，并绑定被代理类和代理处理器。
 　　*
 　　* @param business
 　　* @return 代理类对象
 　　*/
    public Object bind(Object business) {
        this.business = business;
        /**
         * Proxy.newProxyInstance(参数1, 参数2, 参数3)
         *
         * 参数1, 表示被代理类的 ClassLoader
         * 参数2, 表示被代理的接口
         * 参数3, 表示代理处理器对象
         *
         * 该方法，返回代理实例
         */
        return Proxy.newProxyInstance(business.getClass().getClassLoader(),
                business.getClass().getInterfaces(),
                this);
    }
    /**
 　　* 代理需要调用的方法，并在该方法调用前后，先调用连接器的方法。
 　　*
 　　* @param proxy 代理类对象
 　　* @param method 被代理的接口方法
 　　* @param args 被代理接口方法的参数
 　　* @return 方法调用返回的结果
 　　* @throws Throwable
 　　*/
   public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
       Object result = null;
       interceptor.before();
       result = method.invoke(business, args);
       interceptor.after();
       return null;
   }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
第 5 步：创建客户端 ClientDemo
```java
/**
 * @author 维C果糖
 * @create 2017-03-30
 *
 * GitHub：github.com/guobinhit
 *
 * 客户端
 */
public class ClientDemo {
    public static void main(String args[]) {
        // 创建动态代理处理工具
        DynamicProxyHandler handler = new DynamicProxyHandler();
        // 创建业务组件对象
        BusinessFacade business = new BusinessClass();
        // 创建业务组件对象，并用动态代理绑定代理类
        BusinessFacade businessProxy = (BusinessFacade) handler.bind(business);
        // 调用业务组件中的方法，演示拦截器效果
        businessProxy.doSomething();
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
运行上面的项目代码，结果如下图所示：
![1](https://img-blog.csdn.net/20170330104751947?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzUyNDY2MjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
如上图所示，显然咱们拦截器的拦截功能实现啦！
通过这篇文章，咱们可能会对拦截器的实现原理有一个更透彻的理解。But，在真正的项目实践之中，要想实现拦截器的功能，咱们一般采用继承类`HandlerInterceptorAdapter`或者抽象类`AbstractInterceptor`，或者实现`HandleInterceptor`接口。也就是说，咱们只需要关心如何重写方法，而不需要关心其内部的实现原理。
