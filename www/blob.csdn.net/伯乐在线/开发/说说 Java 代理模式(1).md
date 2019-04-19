# 说说 Java 代理模式(1) - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [占小狼](http://www.jobbole.com/members/wx4097456919) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
代理实现可以分为静态代理和动态代理。
### 静态代理
静态代理模式其实很常见，比如买火车票这件小事：黄牛相当于是火车站的代理，我们可以通过黄牛买票，但只能去火车站进行改签和退票。在代码实现中相当于为一个委托对象realSubject提供一个代理对象proxy，通过proxy可以调用realSubject的部分功能，并添加一些额外的业务处理，同时可以屏蔽realSubject中未开放的接口。
![](http://ww1.sinaimg.cn/large/7cc829d3gw1f7hcpgzzccj20f608cjrn.jpg)
1、RealSubject 是委托类，Proxy 是代理类；
2、Subject 是委托类和代理类的接口；
3、request() 是委托类和代理类的共同方法；
具体代码实现如下：


```
interface Subject {
    void request();
}
class RealSubject implements Subject {
    public void request(){
        System.out.println("RealSubject");
    }
}
class Proxy implements Subject {
    private Subject subject;
    public Proxy(Subject subject){
        this.subject = subject;
    }
    public void request(){
        System.out.println("begin");
        subject.request();
        System.out.println("end");
    }
}
public class ProxyTest {
    public static void main(String args[]) {
        RealSubject subject = new RealSubject();
        Proxy p = new Proxy(subject);
        p.request();
    }
}
```
静态代理实现中，一个委托类对应一个代理类，代理类在编译期间就已经确定。
### 动态代理
动态代理中，代理类并不是在Java代码中实现，而是在运行时期生成，相比静态代理，动态代理可以很方便的对委托类的方法进行统一处理，如添加方法调用次数、添加日志功能等等，动态代理分为jdk动态代理和cglib动态代理，下面我们通过一个例子看看如何实现jdk动态代理。
1、定义业务逻辑


```
public interface Service {  
    //目标方法 
    public abstract void add();  
} 
public class UserServiceImpl implements Service {  
    public void add() {  
        System.out.println("This is add service");  
    }  
}
```
2、利用`java.lang.reflect.Proxy`类和`java.lang.reflect.InvocationHandler`接口定义代理类的实现。


```
class MyInvocatioHandler implements InvocationHandler {
    private Object target;
    public MyInvocatioHandler(Object target) {
        this.target = target;
    }
    @Override
    public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
        System.out.println("-----before-----");
        Object result = method.invoke(target, args);
        System.out.println("-----end-----");
        return result;
    }
    // 生成代理对象
    public Object getProxy() {
        ClassLoader loader = Thread.currentThread().getContextClassLoader();
        Class<?>[] interfaces = target.getClass().getInterfaces();
        return Proxy.newProxyInstance(loader, interfaces, this);
    }
}
```
3、使用动态代理


```
public class ProxyTest {
    public static void main(String[] args) {
        Service service = new UserServiceImpl();
        MyInvocatioHandler handler = new MyInvocatioHandler(service);
        Service serviceProxy = (Service)handler.getProxy();
        serviceProxy.add();
    }
}
```
执行结果：


```
-----before-----
This is add service
-----end-----
```
代理对象的生成过程由Proxy类的newProxyInstance方法实现，分为3个步骤：
1、`ProxyGenerator.generateProxyClass`方法负责生成代理类的字节码，生成逻辑比较复杂，有兴趣的同学可以继续分析源码 [sun.misc.ProxyGenerator](http://grepcode.com/file/repository.grepcode.com/java/root/jdk/openjdk/8u40-b25/sun/misc/ProxyGenerator.java#ProxyGenerator.generateProxyClass(java.lang.String,java.lang.Class%5B%5D,int))；


```
// proxyName：格式如 "com.sun.proxy.$Proxy.1"；
// interfaces：代理类需要实现的接口数组；
// accessFlags：代理类的访问标识；
byte[] proxyClassFile = ProxyGenerator.generateProxyClass(proxyName, interfaces, accessFlags);
```
2、native方法`Proxy.defineClass0`负责字节码加载的实现，并返回对应的Class对象。


```
Class clazz = defineClass0(loader, proxyName, proxyClassFile, 0, proxyClassFile.length);
```
3、利用`clazz.newInstance`反射机制生成代理类的对象；
**反编译代理类**
为了更清楚的理解动态代理，通过以下方式把代理类字节码生成class文件。


```
byte[] classFile = ProxyGenerator.generateProxyClass("com.sun.proxy.$Proxy.1", service.getClass().getInterfaces());
FileOutputStream out = new FileOutputStream("com.sun.proxy.$Proxy.1.class");
out.write(classFile);
out.flush();
```
使用 [反编译工具 jad](http://varaneckas.com/jad/)`jad com.sun.proxy.$Proxy.1` 看看代理类如何实现，反编译出来的java代码如下：


```
public final class $proxy1 extends Proxy implements Service {
    public $proxy1(InvocationHandler invocationhandler) {
        super(invocationhandler);
    }
    public final boolean equals(Object obj) {
        try {
            return ((Boolean)super.h.invoke(this, m1, new Object[] {
                obj
            })).booleanValue();
        }
        catch(Error _ex) { }
        catch(Throwable throwable) {
            throw new UndeclaredThrowableException(throwable);
        }
    }
    public final String toString() {
        try {
            return (String)super.h.invoke(this, m2, null);
        }
        catch(Error _ex) { }
        catch(Throwable throwable) {
            throw new UndeclaredThrowableException(throwable);
        }
    }
    public final void add() {
        try {
            super.h.invoke(this, m3, null);
            return;
        }
        catch(Error _ex) { }
        catch(Throwable throwable) {
            throw new UndeclaredThrowableException(throwable);
        }
    }
    public final int hashCode() {
        try {
            return ((Integer)super.h.invoke(this, m0, null)).intValue();
        }
        catch(Error _ex) { }
        catch(Throwable throwable) {
            throw new UndeclaredThrowableException(throwable);
        }
    }
    private static Method m1;
    private static Method m2;
    private static Method m3;
    private static Method m0;
    static {
        try {
            m1 = Class.forName("java.lang.Object").getMethod("equals", new Class[] {
                Class.forName("java.lang.Object")
            });
            m2 = Class.forName("java.lang.Object").getMethod("toString", new Class[0]);
            m3 = Class.forName("zzzzzz.Service").getMethod("add", new Class[0]);
            m0 = Class.forName("java.lang.Object").getMethod("hashCode", new Class[0]);
        }
        catch(NoSuchMethodException nosuchmethodexception) {
            throw new NoSuchMethodError(nosuchmethodexception.getMessage());
        }
        catch(ClassNotFoundException classnotfoundexception) {
            throw new NoClassDefFoundError(classnotfoundexception.getMessage());
        }
    }
}
```
从上述代码可以发现：
1、生成的$proxy1继承自Proxy类，并实现了Service接口。
2、执行代理对象的方法，其实就是执行InvocationHandle对象的invoke方法，传入的参数分别是当前代理对象，当前执行的方法和参数。


```
super.h.invoke(this, m3, null);
```
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
![](http://jbcdn2.b0.upaiyun.com/2016/08/c3fbbefbe3e302d0f6944c94db3cb1c4.jpg)
