# Proxy - weixin_33985507的博客 - CSDN博客
2018年05月09日 11:41:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：10
![3744244-19cbb31d784f6d8f.png](https://upload-images.jianshu.io/upload_images/3744244-19cbb31d784f6d8f.png)
本文主要介绍代理在Java中的应用，代理包括两种：静态代理 和 动态代理，其中动态代理又分为 JDK动态代理和 CGLIB动态代理。为什么要学习Java代理，因为它是很多框架的基础，理解了它，在阅读其它框架源码的时候，才能更舒服。
# 反射机制
要理解java中的反射机制，需要理解java中的Class对象，它包含了与类有关的信息，事实Class对象就是用来创建类的所有“常规对象”的。类是程序的一部分，每个类都有一个Class对象。换而言之，每当编写并且编译一个新类时，就会产生一个Class对象(更恰当的说，是保存在一个同名的.class文件中)，为了生成这个类的对象，运行这个程序的java虚拟机将使用被称为“类加载器”的子系统。
Class的使用也和其他对象一样，我们可以获取并操作它的引用。其中有一个forName()静态方法，可以返回一个Class对象的引用：Class.forName(全类名)，通过该返回的Class对象引用，你可以做很多事情。例如：有一个类A , 通过 Class clazz = Class.forName(“A”)，你拿到了A对应的class对象 clazz，然后通过clazz可以做很多事情，比如，实创建一个A实例，或者是得到A的接口，方法等类信息。下面演示一个例子：
```
public class ToyTest {
    public static void main(String[] args) throws Exception{
        Class c = null;
        c = Class.forName("com.sxy.reflect.FancyToy");
        printInfo(c);
        for(Class face : c.getInterfaces())
            printInfo(face);
        Class up = c.getSuperclass();
        Object obj = up.newInstance( );
        printInfo(obj.getClass());
    }
    static void printInfo(Class cc){
        System.out.println("className " + cc.getName() + " is interface? [ " + cc.isInterface() + " ]");
        System.out.println("simpleName " + cc.getSimpleName());
        System.out.println("CanonicalName " + cc.getCanonicalName());
    }
}
interface HasBatteries{}
interface Waterproof{}
interface  Shoots{}
class Toy{
    Toy(){}
    Toy(int i){}
}
class FancyToy extends Toy implements HasBatteries, Waterproof, Shoots{
    FancyToy(){ super(1);}
}
```
运行结果
> 
![3744244-53266bdea7814417.png](https://upload-images.jianshu.io/upload_images/3744244-53266bdea7814417.png)
image.png
其实，反射机制也并没有什么神奇之处，我们普通的调用，是在编译的时候打开和检查.class文件，但是，对于反射而言，.class文件在编译时是不可获取的，所以在运行时打开和运行.class文件。
# 静态代理
静态代理比较简单，大体的流程如下：
- 定义目标接口。
- 实现目标类接口。
- 定义代理类，实现目标接口，目标类接口作为其私有属性。
- 基本完成，新建一个测试类就可以了。
下面用代码演示一遍静态代理的实现过程：
```
public interface Learn {
    void learnEnglish();
}
```
```
public class LearnImpl implements Learn{
    @Override
    public void learnEnglish() {
        System.out.println(" learn english");
    }
}
```
```
public class LearnProxy implements Learn{
    private Learn target;
    public LearnProxy(Learn target){
        this.target = target;
    }
    @Override
    public void learnEnglish() {
        System.out.println("执行目标方法之前");
        target.learnEnglish();
        System.out.println("执行目标方法之后");
    }
}
```
```
public class TestProxy {
    public static void main(String[] args){
        LearnImpl target = new LearnImpl();
        LearnProxy proxy = new LearnProxy(target);
        proxy.learnEnglish();
    }
}
```
> 
![3744244-a48a639a3936860b.png](https://upload-images.jianshu.io/upload_images/3744244-a48a639a3936860b.png)
image.png
静态感觉更像是一种单纯的设计模式，用任何语言都可以达到这样的效果。优点就是在不修改目标对象的时候，完成了对目标对象的扩展，但是缺点也非常那个明显，代理类需要实现目标对象一样的接口，因此不便于维护。
# JDK原生动态代理
Java中实现动态代理有两种方法，一种是JDK提供的原生动态代理；还有一种是CGLIB动态代理。动态代理比静态代理的思想更进一步，因为他可以动态的创建代理并且动态的处理对所代理方法的调用。就比如上提到的静态代理，如果要代理的类太多，那么代理类的代码量是十分庞大的，很多重复的代码。动态代理就是为了解决这个问题的。
直接演示一个例子：
```
public interface Learn {
    void learnEnglish();
    void learnChinese();
}
```
```
public class LearnImpl implements Learn{
    public void learnEnglish() {
        System.out.print("I want to learn English");
    }
    public void learnChinese() {
        System.out.print("I want to learn Chinese");
    }
}
```
```
public class LogInvocationHandler implements InvocationHandler {
    /**
     * the target that we want to be proxy
     */
    private Learn target;
    public LogInvocationHandler(Learn target) {
        this.target = target;
    }
    public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
        System.out.println("执行代理方法之前");
        method.invoke(target, args);
        System.out.println("执行代理方法之后");
        return null;
    }
}
```
```
public class JDKProxyTest {
    public static void main(String[] args) {
        // 允许生成代理类的class文件
        System.getProperties().put("sun.misc.ProxyGenerator.saveGeneratedFiles", "true");
        Learn learnReal = new LearnImpl();
        InvocationHandler handler = new LogInvocationHandler(learnReal);
        Learn learn = (Learn) Proxy.newProxyInstance(
                learnReal.getClass().getClassLoader(),
                learnReal.getClass().getInterfaces(),
                handler);
        System.out.println(learn.getClass().getName());
        learn.learnChinese();
//        Class clazzProxy = Proxy.getProxyClass(Collection.class.getClassLoader(), Collection.class);
//        System.out.println(clazzProxy);
    }
}
```
这里主要介绍 Proxy.newProxyinstance() 方法，这是JDK中为我们提供的API，可以动态的创建代理。这个方法需要传递3个参数，第一个是代理对象的类加载器（通常可以从已经被加载过的对象中获取其类加载器，然后传递给它），第二个是代理对象需要实现的接口，可以同时指定多个接口；第三个方法调用的实际处理者，代理对象的方法调用都会转发到这里，这是一个方法委托类，我们通过代理调用被代理类的方法时，就可以将方法名和方法参数都委托给这个委托类。执行结果如下：
> 
![3744244-caf23a7a6b6a7298.png](https://upload-images.jianshu.io/upload_images/3744244-caf23a7a6b6a7298.png)
image.png
从上图中可以看到，代理已经生效了？那么，它究竟是如何实现动态代理的呢？动态代理的流程是什么？我们关注最核心的代码：
```
Learn learnReal = new LearnImpl();
        InvocationHandler handler = new LogInvocationHandler(learnReal);
        Learn learn = (Learn) Proxy.newProxyInstance(
                learnReal.getClass().getClassLoader(),
                learnReal.getClass().getInterfaces(),
                handler);
```
首先是 InvocationHandler ， 这是个啥东西？其源码如下，只有一个invoke方法
```
package java.lang.reflect;
public interface InvocationHandler {
    public Object invoke(Object proxy, Method method, Object[] args)
        throws Throwable;
}
```
有关于这个接口的解释如下：
> 
InvocationHandler is the interface implemented by  the invocation handler of a proxy instance.
Each proxy instance has an associated invocation handler. When a method is invoked on a proxy instance, the method invocation is encoded and dispatched to the invoke method of its invocation handler.
哎哟喂，这这这啥意思？先换成国语看看吧：
> 
InvocationHandler是由代理实例的调用处理程序实现的接口。每个代理实例都有一个关联的调用处理程序。当在代理实例上调用方法时，方法调用将被编码并分派给其调用处理程序的invoke方法。
额，好像还是不太好理解哦。不过其中有两个名词：proxy instance、invocation handler。
- proxy instance：在代码中没有直观的表现，可以代表以下语句的返回值：
```
(Learn)Proxy.newProxyInstance(learnReal.getClass().getClassLoader(), learnReal.getClass().getInterfaces(),handler);
```
不纠结了，至于它到底返回的是什么，接下来会介绍。
- invocation handler：InvocationHandler 接口的实现类，即代码中的 LogInvocationHandler，并且和 proxy instance 相关联。
总结一下大概的意思就是：当我们通过代理实例调用方法时，实际上调用的是LogInvocationHandler的invoke方法。正常理解下也肯定是这样的，因为我们得日志就是织入在 invoke 方法里面的，肯定是内部通过什么花里胡哨得手法调用了invoke方法。
那么，它究竟是如何花里胡哨的调用了 invoke 方法呢？
回到刚刚的问题上来，proxy instance ，上面说了它是如下语句的返回值：
```
Learn learn = (Learn) Proxy.newProxyInstance(
                learnReal.getClass().getClassLoader(),
                learnReal.getClass().getInterfaces(),
                handler);
```
Proxy.newProxyInstance 方法我们知道它是 通过拼接字节码的方式来创建代理类 就可以了，不能再深入了，要不然没玩没了。
观察我们打印的一条日志，我们打印返回值的 全类名
```
System.out.println(learn.getClass().getName());
```
发现得到的是这么一个玩意，这是啥？这就是动态创建得代理实例
> 
com.sun.proxy.$Proxy0
为了得到 com.sun.proxy.$Proxy0 得全貌，在 我们得代码中添加这么一句，这样就可以看到 对应得class 文件了
```
System.getProperties().put("sun.misc.ProxyGenerator.saveGeneratedFiles", "true");
```
> 
![3744244-b63a2f3b6464b2bc.png](https://upload-images.jianshu.io/upload_images/3744244-b63a2f3b6464b2bc.png)
image.png
再次运行程序，发现生成了这个文件
> 
![3744244-23bfc329f032cc81.png](https://upload-images.jianshu.io/upload_images/3744244-23bfc329f032cc81.png)
image.png
其源码如下：
```
//
// Source code recreated from a .class file by IntelliJ IDEA
// (powered by Fernflower decompiler)
//
package com.sun.proxy;
import com.hand.hsp.proxy.Learn;
import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;
import java.lang.reflect.UndeclaredThrowableException;
public final class $Proxy0 extends Proxy implements Learn {
    private static Method m1;
    private static Method m2;
    private static Method m3;
    private static Method m4;
    private static Method m0;
    public $Proxy0(InvocationHandler var1) throws  {
        super(var1);
    }
    public final boolean equals(Object var1) throws  {
        try {
            return (Boolean)super.h.invoke(this, m1, new Object[]{var1});
        } catch (RuntimeException | Error var3) {
            throw var3;
        } catch (Throwable var4) {
            throw new UndeclaredThrowableException(var4);
        }
    }
    public final String toString() throws  {
        try {
            return (String)super.h.invoke(this, m2, (Object[])null);
        } catch (RuntimeException | Error var2) {
            throw var2;
        } catch (Throwable var3) {
            throw new UndeclaredThrowableException(var3);
        }
    }
    public final void learnEnglish() throws  {
        try {
            super.h.invoke(this, m3, (Object[])null);
        } catch (RuntimeException | Error var2) {
            throw var2;
        } catch (Throwable var3) {
            throw new UndeclaredThrowableException(var3);
        }
    }
    public final void learnChinese() throws  {
        try {
            super.h.invoke(this, m4, (Object[])null);
        } catch (RuntimeException | Error var2) {
            throw var2;
        } catch (Throwable var3) {
            throw new UndeclaredThrowableException(var3);
        }
    }
    public final int hashCode() throws  {
        try {
            return (Integer)super.h.invoke(this, m0, (Object[])null);
        } catch (RuntimeException | Error var2) {
            throw var2;
        } catch (Throwable var3) {
            throw new UndeclaredThrowableException(var3);
        }
    }
    static {
        try {
            m1 = Class.forName("java.lang.Object").getMethod("equals", Class.forName("java.lang.Object"));
            m2 = Class.forName("java.lang.Object").getMethod("toString");
            m3 = Class.forName("com.hand.hsp.proxy.Learn").getMethod("learnEnglish");
            m4 = Class.forName("com.hand.hsp.proxy.Learn").getMethod("learnChinese");
            m0 = Class.forName("java.lang.Object").getMethod("hashCode");
        } catch (NoSuchMethodException var2) {
            throw new NoSuchMethodError(var2.getMessage());
        } catch (ClassNotFoundException var3) {
            throw new NoClassDefFoundError(var3.getMessage());
        }
    }
}
```
观察其中的 learnChinese 方法，发现内部最终调用的就是  LogInvocationHandler 的 invoke 方法，LogInvocationHandler 的 invoke又通过反射技术调用了目标类(realLearn)的 learnChinese 方法。
```
method.invoke(target, args);
```
JDK动态代理的流程大概就是这样了，核心就是动态生成一个代理实例。不过这种方式有一个我呢提，Java动态代理是基于接口的，如果对象没有实现接口我们该如何代理呢？这时候就应用用CGLIB动态代理技术了。
# CGLIB动态代理
> 
CGLIB的底层使用ASM在内存中动态的生成被代理类的子类，使用CGLIB即使代理类没有实现任何接口也可以实现动态代理功能。CGLIB具有简单易用，它的运行速度要远远快于JDK的Proxy动态代理。
使用CGLIB需要导入以下两个jar文件：
- asm.jar：CGLIB的底层实现。
- cglib.jar：CGLIB的核心jar包。
有关于引入包的时候，需要注意版本问题。如果没有引入 asm.jar，会报错如下：
```
Exception in thread "main" java.lang.NoClassDefFoundError: org/objectweb/asm/Type
    at net.sf.cglib.core.TypeUtils.parseType(TypeUtils.java:184)
    at net.sf.cglib.core.KeyFactory.<clinit>(KeyFactory.java:66)
    at net.sf.cglib.proxy.Enhancer.<clinit>(Enhancer.java:69)
    at com.hand.hsp.proxy.CGProxyTest.main(CGProxyTest.java:11)
Caused by: java.lang.ClassNotFoundException: org.objectweb.asm.Type
    at java.net.URLClassLoader.findClass(URLClassLoader.java:381)
    at java.lang.ClassLoader.loadClass(ClassLoader.java:424)
    at sun.misc.Launcher$AppClassLoader.loadClass(Launcher.java:331)
    at java.lang.ClassLoader.loadClass(ClassLoader.java:357)
    ... 4 more
```
如果引入的 cglib.jar 和 asm.jar 的版本有问题，如这里以 cglib.jar 3.1 和 asm.jar 3.2 为例
```
Exception in thread "main" java.lang.IncompatibleClassChangeError: class net.sf.cglib.core.DebuggingClassWriter has interface org.objectweb.asm.ClassVisitor as super class
    at java.lang.ClassLoader.defineClass1(Native Method)
    at java.lang.ClassLoader.defineClass(ClassLoader.java:763)
    at java.security.SecureClassLoader.defineClass(SecureClassLoader.java:142)
    at java.net.URLClassLoader.defineClass(URLClassLoader.java:467)
```
最终我引入的版本：  cglib.jar 3.1 和 asm.jar 5.0.4。
下面是测试案例：接口和实现类保持不变，新增两个类：
```
public class LogMethodInterceptor implements MethodInterceptor {
    public Object intercept(Object object, Method method, Object[] args, MethodProxy methodProxy) throws Throwable {
        System.out.println("执行代理方法之前");
        Object invoke = methodProxy.invokeSuper(object, args);
        System.out.println("执行代理方法之后");
        return invoke;
    }
}
```
```
public class CGProxyTest {
    public static void main(String[] args) {
        Enhancer enhancer = new Enhancer();
        enhancer.setSuperclass(LearnImpl.class);
        enhancer.setCallback(new LogMethodInterceptor());
        Learn realLearn = (LearnImpl) enhancer.create();
        realLearn.learnChinese();
    }
}
```
启动程序，观察控制台输出
> 
![3744244-6e05be98de552439.png](https://upload-images.jianshu.io/upload_images/3744244-6e05be98de552439.png)
image.png
CGLIB动态代理要比JDK动态代理速度更快，因为它不是通过反射技术来执行目标类方法的，但也比JDK动态代理更难理解一些。
为了更好的理解生成的代理类，设置如下属性
```
// com/sun/proxy 表示class文件要输出到的文件夹
System.setProperty(DebuggingClassWriter.DEBUG_LOCATION_PROPERTY, "com/sun/proxy");
```
再次运行程序，生鲜生成了一下class文件，这里主要关注这三个：
> 
![3744244-03f0a2ea32da5832.png](https://upload-images.jianshu.io/upload_images/3744244-03f0a2ea32da5832.png)
image.png
其中LearnImple1c74c1d.class 就是动态生成的代理类，其源码如下：
```
package com.hand.hsp.proxy;
import java.lang.reflect.Method;
import net.sf.cglib.core.ReflectUtils;
import net.sf.cglib.core.Signature;
import net.sf.cglib.proxy.Callback;
import net.sf.cglib.proxy.Factory;
import net.sf.cglib.proxy.MethodInterceptor;
import net.sf.cglib.proxy.MethodProxy;
public class LearnImpl$$EnhancerByCGLIB$$e1c74c1d extends LearnImpl implements Factory {
    private boolean CGLIB$BOUND;
    private static final ThreadLocal CGLIB$THREAD_CALLBACKS;
    private static final Callback[] CGLIB$STATIC_CALLBACKS;
    private MethodInterceptor CGLIB$CALLBACK_0;
    private static final Method CGLIB$learnEnglish$0$Method;
    private static final MethodProxy CGLIB$learnEnglish$0$Proxy;
    private static final Object[] CGLIB$emptyArgs;
    private static final Method CGLIB$learnChinese$1$Method;
    private static final MethodProxy CGLIB$learnChinese$1$Proxy;
    private static final Method CGLIB$finalize$2$Method;
    private static final MethodProxy CGLIB$finalize$2$Proxy;
    private static final Method CGLIB$equals$3$Method;
    private static final MethodProxy CGLIB$equals$3$Proxy;
    private static final Method CGLIB$toString$4$Method;
    private static final MethodProxy CGLIB$toString$4$Proxy;
    private static final Method CGLIB$hashCode$5$Method;
    private static final MethodProxy CGLIB$hashCode$5$Proxy;
    private static final Method CGLIB$clone$6$Method;
    private static final MethodProxy CGLIB$clone$6$Proxy;
    static void CGLIB$STATICHOOK1() {
        CGLIB$THREAD_CALLBACKS = new ThreadLocal();
        CGLIB$emptyArgs = new Object[0];
        Class var0 = Class.forName("com.hand.hsp.proxy.LearnImpl$$EnhancerByCGLIB$$e1c74c1d");
        Class var1;
        Method[] var10000 = ReflectUtils.findMethods(new String[]{"finalize", "()V", "equals", "(Ljava/lang/Object;)Z", "toString", "()Ljava/lang/String;", "hashCode", "()I", "clone", "()Ljava/lang/Object;"}, (var1 = Class.forName("java.lang.Object")).getDeclaredMethods());
        CGLIB$finalize$2$Method = var10000[0];
        CGLIB$finalize$2$Proxy = MethodProxy.create(var1, var0, "()V", "finalize", "CGLIB$finalize$2");
        CGLIB$equals$3$Method = var10000[1];
        CGLIB$equals$3$Proxy = MethodProxy.create(var1, var0, "(Ljava/lang/Object;)Z", "equals", "CGLIB$equals$3");
        CGLIB$toString$4$Method = var10000[2];
        CGLIB$toString$4$Proxy = MethodProxy.create(var1, var0, "()Ljava/lang/String;", "toString", "CGLIB$toString$4");
        CGLIB$hashCode$5$Method = var10000[3];
        CGLIB$hashCode$5$Proxy = MethodProxy.create(var1, var0, "()I", "hashCode", "CGLIB$hashCode$5");
        CGLIB$clone$6$Method = var10000[4];
        CGLIB$clone$6$Proxy = MethodProxy.create(var1, var0, "()Ljava/lang/Object;", "clone", "CGLIB$clone$6");
        var10000 = ReflectUtils.findMethods(new String[]{"learnEnglish", "()V", "learnChinese", "()V"}, (var1 = Class.forName("com.hand.hsp.proxy.LearnImpl")).getDeclaredMethods());
        CGLIB$learnEnglish$0$Method = var10000[0];
        CGLIB$learnEnglish$0$Proxy = MethodProxy.create(var1, var0, "()V", "learnEnglish", "CGLIB$learnEnglish$0");
        CGLIB$learnChinese$1$Method = var10000[1];
        CGLIB$learnChinese$1$Proxy = MethodProxy.create(var1, var0, "()V", "learnChinese", "CGLIB$learnChinese$1");
    }
    final void CGLIB$learnEnglish$0() {
        super.learnEnglish();
    }
    public final void learnEnglish() {
        MethodInterceptor var10000 = this.CGLIB$CALLBACK_0;
        if (this.CGLIB$CALLBACK_0 == null) {
            CGLIB$BIND_CALLBACKS(this);
            var10000 = this.CGLIB$CALLBACK_0;
        }
        if (var10000 != null) {
            var10000.intercept(this, CGLIB$learnEnglish$0$Method, CGLIB$emptyArgs, CGLIB$learnEnglish$0$Proxy);
        } else {
            super.learnEnglish();
        }
    }
    final void CGLIB$learnChinese$1() {
        super.learnChinese();
    }
    public final void learnChinese() {
        MethodInterceptor var10000 = this.CGLIB$CALLBACK_0;
        if (this.CGLIB$CALLBACK_0 == null) {
            CGLIB$BIND_CALLBACKS(this);
            var10000 = this.CGLIB$CALLBACK_0;
        }
        if (var10000 != null) {
            var10000.intercept(this, CGLIB$learnChinese$1$Method, CGLIB$emptyArgs, CGLIB$learnChinese$1$Proxy);
        } else {
            super.learnChinese();
        }
    }
    final void CGLIB$finalize$2() throws Throwable {
        super.finalize();
    }
    protected final void finalize() throws Throwable {
        MethodInterceptor var10000 = this.CGLIB$CALLBACK_0;
        if (this.CGLIB$CALLBACK_0 == null) {
            CGLIB$BIND_CALLBACKS(this);
            var10000 = this.CGLIB$CALLBACK_0;
        }
        if (var10000 != null) {
            var10000.intercept(this, CGLIB$finalize$2$Method, CGLIB$emptyArgs, CGLIB$finalize$2$Proxy);
        } else {
            super.finalize();
        }
    }
    final boolean CGLIB$equals$3(Object var1) {
        return super.equals(var1);
    }
    public final boolean equals(Object var1) {
        MethodInterceptor var10000 = this.CGLIB$CALLBACK_0;
        if (this.CGLIB$CALLBACK_0 == null) {
            CGLIB$BIND_CALLBACKS(this);
            var10000 = this.CGLIB$CALLBACK_0;
        }
        if (var10000 != null) {
            Object var2 = var10000.intercept(this, CGLIB$equals$3$Method, new Object[]{var1}, CGLIB$equals$3$Proxy);
            return var2 == null ? false : (Boolean)var2;
        } else {
            return super.equals(var1);
        }
    }
    final String CGLIB$toString$4() {
        return super.toString();
    }
    public final String toString() {
        MethodInterceptor var10000 = this.CGLIB$CALLBACK_0;
        if (this.CGLIB$CALLBACK_0 == null) {
            CGLIB$BIND_CALLBACKS(this);
            var10000 = this.CGLIB$CALLBACK_0;
        }
        return var10000 != null ? (String)var10000.intercept(this, CGLIB$toString$4$Method, CGLIB$emptyArgs, CGLIB$toString$4$Proxy) : super.toString();
    }
    final int CGLIB$hashCode$5() {
        return super.hashCode();
    }
    public final int hashCode() {
        MethodInterceptor var10000 = this.CGLIB$CALLBACK_0;
        if (this.CGLIB$CALLBACK_0 == null) {
            CGLIB$BIND_CALLBACKS(this);
            var10000 = this.CGLIB$CALLBACK_0;
        }
        if (var10000 != null) {
            Object var1 = var10000.intercept(this, CGLIB$hashCode$5$Method, CGLIB$emptyArgs, CGLIB$hashCode$5$Proxy);
            return var1 == null ? 0 : ((Number)var1).intValue();
        } else {
            return super.hashCode();
        }
    }
    final Object CGLIB$clone$6() throws CloneNotSupportedException {
        return super.clone();
    }
    protected final Object clone() throws CloneNotSupportedException {
        MethodInterceptor var10000 = this.CGLIB$CALLBACK_0;
        if (this.CGLIB$CALLBACK_0 == null) {
            CGLIB$BIND_CALLBACKS(this);
            var10000 = this.CGLIB$CALLBACK_0;
        }
        return var10000 != null ? var10000.intercept(this, CGLIB$clone$6$Method, CGLIB$emptyArgs, CGLIB$clone$6$Proxy) : super.clone();
    }
    public static MethodProxy CGLIB$findMethodProxy(Signature var0) {
        String var10000 = var0.toString();
        switch(var10000.hashCode()) {
        case -1574182249:
            if (var10000.equals("finalize()V")) {
                return CGLIB$finalize$2$Proxy;
            }
            break;
        case -508378822:
            if (var10000.equals("clone()Ljava/lang/Object;")) {
                return CGLIB$clone$6$Proxy;
            }
            break;
        case 4351180:
            if (var10000.equals("learnChinese()V")) {
                return CGLIB$learnChinese$1$Proxy;
            }
            break;
        case 800444521:
            if (var10000.equals("learnEnglish()V")) {
                return CGLIB$learnEnglish$0$Proxy;
            }
            break;
        case 1826985398:
            if (var10000.equals("equals(Ljava/lang/Object;)Z")) {
                return CGLIB$equals$3$Proxy;
            }
            break;
        case 1913648695:
            if (var10000.equals("toString()Ljava/lang/String;")) {
                return CGLIB$toString$4$Proxy;
            }
            break;
        case 1984935277:
            if (var10000.equals("hashCode()I")) {
                return CGLIB$hashCode$5$Proxy;
            }
        }
        return null;
    }
    public LearnImpl$$EnhancerByCGLIB$$e1c74c1d() {
        CGLIB$BIND_CALLBACKS(this);
    }
    public static void CGLIB$SET_THREAD_CALLBACKS(Callback[] var0) {
        CGLIB$THREAD_CALLBACKS.set(var0);
    }
    public static void CGLIB$SET_STATIC_CALLBACKS(Callback[] var0) {
        CGLIB$STATIC_CALLBACKS = var0;
    }
    private static final void CGLIB$BIND_CALLBACKS(Object var0) {
        LearnImpl$$EnhancerByCGLIB$$e1c74c1d var1 = (LearnImpl$$EnhancerByCGLIB$$e1c74c1d)var0;
        if (!var1.CGLIB$BOUND) {
            var1.CGLIB$BOUND = true;
            Object var10000 = CGLIB$THREAD_CALLBACKS.get();
            if (var10000 == null) {
                var10000 = CGLIB$STATIC_CALLBACKS;
                if (CGLIB$STATIC_CALLBACKS == null) {
                    return;
                }
            }
            var1.CGLIB$CALLBACK_0 = (MethodInterceptor)((Callback[])var10000)[0];
        }
    }
    public Object newInstance(Callback[] var1) {
        CGLIB$SET_THREAD_CALLBACKS(var1);
        LearnImpl$$EnhancerByCGLIB$$e1c74c1d var10000 = new LearnImpl$$EnhancerByCGLIB$$e1c74c1d();
        CGLIB$SET_THREAD_CALLBACKS((Callback[])null);
        return var10000;
    }
    public Object newInstance(Callback var1) {
        CGLIB$SET_THREAD_CALLBACKS(new Callback[]{var1});
        LearnImpl$$EnhancerByCGLIB$$e1c74c1d var10000 = new LearnImpl$$EnhancerByCGLIB$$e1c74c1d();
        CGLIB$SET_THREAD_CALLBACKS((Callback[])null);
        return var10000;
    }
    public Object newInstance(Class[] var1, Object[] var2, Callback[] var3) {
        CGLIB$SET_THREAD_CALLBACKS(var3);
        LearnImpl$$EnhancerByCGLIB$$e1c74c1d var10000 = new LearnImpl$$EnhancerByCGLIB$$e1c74c1d;
        switch(var1.length) {
        case 0:
            var10000.<init>();
            CGLIB$SET_THREAD_CALLBACKS((Callback[])null);
            return var10000;
        default:
            throw new IllegalArgumentException("Constructor not found");
        }
    }
    public Callback getCallback(int var1) {
        CGLIB$BIND_CALLBACKS(this);
        MethodInterceptor var10000;
        switch(var1) {
        case 0:
            var10000 = this.CGLIB$CALLBACK_0;
            break;
        default:
            var10000 = null;
        }
        return var10000;
    }
    public void setCallback(int var1, Callback var2) {
        switch(var1) {
        case 0:
            this.CGLIB$CALLBACK_0 = (MethodInterceptor)var2;
        default:
        }
    }
    public Callback[] getCallbacks() {
        CGLIB$BIND_CALLBACKS(this);
        return new Callback[]{this.CGLIB$CALLBACK_0};
    }
    public void setCallbacks(Callback[] var1) {
        this.CGLIB$CALLBACK_0 = (MethodInterceptor)var1[0];
    }
    static {
        CGLIB$STATICHOOK1();
    }
}
```
代码比较多，所以这里只关注我们在 main函数中调用到的方法learnChinese，其对应在代理类中的实现如下：
```
public final void learnChinese() {
        MethodInterceptor var10000 = this.CGLIB$CALLBACK_0;
        if (this.CGLIB$CALLBACK_0 == null) {
            CGLIB$BIND_CALLBACKS(this);
            var10000 = this.CGLIB$CALLBACK_0;
        }
        if (var10000 != null) {
            var10000.intercept(this, CGLIB$learnChinese$1$Method, CGLIB$emptyArgs, CGLIB$learnChinese$1$Proxy);
        } else {
            super.learnChinese();
        }
    }
```
CGLIB$CALLBACK_0是代理类的一个成员变量，就一个变量，无关紧要
```
private MethodInterceptor CGLIB$CALLBACK_0;
```
来梳理一下这段逻辑：
当 CGLIBBIND_CALLBACKS 方法，方法源码如下：
```
private static final ThreadLocal CGLIB$THREAD_CALLBACKS;
......
    private static final void CGLIB$BIND_CALLBACKS(Object var0) {
        LearnImpl$$EnhancerByCGLIB$$e1c74c1d var1 = (LearnImpl$$EnhancerByCGLIB$$e1c74c1d)var0;
        if (!var1.CGLIB$BOUND) {
            var1.CGLIB$BOUND = true;
            Object var10000 = CGLIB$THREAD_CALLBACKS.get();
            if (var10000 == null) {
                var10000 = CGLIB$STATIC_CALLBACKS;
                if (CGLIB$STATIC_CALLBACKS == null) {
                    return;
                }
            }
            var1.CGLIB$CALLBACK_0 = (MethodInterceptor)((Callback[])var10000)[0];
        }
    }
```
CGLIBTHREAD_CALLBACKS.get())，如果没有获取到值，就使用 CGLIBCALLBACK_0。**【先不要管各个变量的值是怎么来的，之后会详细介绍】**
CGLIB$CALLBACK_0获取值返回到learnChinese方法之后，接着调用 intercept 方法:
```
var10000.intercept(this, CGLIB$learnChinese$1$Method, CGLIB$emptyArgs, CGLIB$learnChinese$1$Proxy);
```
如果不去纠结细节，大概可以猜测得出，这里调用的就是 LogMethodInterceptor 的 intercept 方法。如果我们没有设置MethodInterceptor， 就直接执行目标类的learnChinese，否则的话，执行intercept方法，逻辑上是可以走通的，但是不够严谨，所以接下来解释一下这些变量值是怎么来的。
在 main 方法内，我们使用到了 Enhancer 类，
```
Enhancer enhancer = new Enhancer();
        enhancer.setSuperclass(LearnImpl.class);
        enhancer.setCallback(new LogMethodInterceptor());
        Learn realLearn = (LearnImpl) enhancer.create();
        realLearn.learnChinese();
```
- 设置 父类。
- 设置 MethodInterceptor。
- 调用create方法创建 代理实例。
create 方法源码如下：
```
public Object create() {
        classOnly = false;
        argumentTypes = null;
        return createHelper();
    }
    private Object createHelper() {
        validate();
        if (superclass != null) {
            setNamePrefix(superclass.getName());
        } else if (interfaces != null) {
            setNamePrefix(interfaces[ReflectUtils.findPackageProtected(interfaces)].getName());
        }
        return super.create(KEY_FACTORY.newInstance((superclass != null) ? superclass.getName() : null,
                                                    ReflectUtils.getNames(interfaces),
                                                    filter,
                                                    callbackTypes,
                                                    useFactory,
                                                    interceptDuringConstruction,
                                                    serialVersionUID));
    }
```
发现调用了父类(AbstractClassGenerator)的create方法，源码如下：
```
protected Object create(Object key) {
        try {
            Class gen = null;
            
            synchronized (source) {
                ClassLoader loader = getClassLoader();
                Map cache2 = null;
                cache2 = (Map)source.cache.get(loader);
                if (cache2 == null) {
                    cache2 = new HashMap();
                    cache2.put(NAME_KEY, new HashSet());
                    source.cache.put(loader, cache2);
                } else if (useCache) {
                    Reference ref = (Reference)cache2.get(key);
                    gen = (Class) (( ref == null ) ? null : ref.get()); 
                }
                if (gen == null) {
                    Object save = CURRENT.get();
                    CURRENT.set(this);
                    try {
                        this.key = key;
                        
                        if (attemptLoad) {
                            try {
                                gen = loader.loadClass(getClassName());
                            } catch (ClassNotFoundException e) {
                                // ignore
                            }
                        }
                        if (gen == null) {
                            byte[] b = strategy.generate(this);
                            String className = ClassNameReader.getClassName(new ClassReader(b));
                            getClassNameCache(loader).add(className);
                            gen = ReflectUtils.defineClass(className, b, loader);
                        }
                       
                        if (useCache) {
                            cache2.put(key, new WeakReference(gen));
                        }
                        return firstInstance(gen);
                    } finally {
                        CURRENT.set(save);
                    }
                }
            }
            return firstInstance(gen);
        } catch (RuntimeException e) {
            throw e;
        } catch (Error e) {
            throw e;
        } catch (Exception e) {
            throw new CodeGenerationException(e);
        }
    }
    abstract protected Object firstInstance(Class type) throws Exception;
```
这里就暂时就认为 create方法 通过字节码技术创建了一个 Class，在得到Class之后，它是通过 firstInstance(gen) 得到一个代理实例的。firstInstance 在 AbstractClassGenerator中是一个抽象方法，其在Enhancer类中的实现如下：
```
private static final String SET_THREAD_CALLBACKS_NAME = "CGLIB$SET_THREAD_CALLBACKS";
......
    protected Object firstInstance(Class type) throws Exception {
        if (classOnly) {
            return type;
        } else {
            return createUsingReflection(type);
        }
    }
    private Object createUsingReflection(Class type) {
        setThreadCallbacks(type, callbacks);
        try{
        
        if (argumentTypes != null) {
            
             return ReflectUtils.newInstance(type, argumentTypes, arguments);
             
        } else {
            
            return ReflectUtils.newInstance(type);
            
        }
        }finally{
         // clear thread callbacks to allow them to be gc'd
         setThreadCallbacks(type, null);
        }
    }
    private static void setThreadCallbacks(Class type, Callback[] callbacks) {
        setCallbacksHelper(type, callbacks, SET_THREAD_CALLBACKS_NAME);
    }
    private static void setCallbacksHelper(Class type, Callback[] callbacks, String methodName) {
        // TODO: optimize
        try {
            Method setter = getCallbacksSetter(type, methodName);
            setter.invoke(null, new Object[]{ callbacks });
        } catch (NoSuchMethodException e) {
            throw new IllegalArgumentException(type + " is not an enhanced class");
        } catch (IllegalAccessException e) {
            throw new CodeGenerationException(e);
        } catch (InvocationTargetException e) {
            throw new CodeGenerationException(e);
        }
    }
    private static Method getCallbacksSetter(Class type, String methodName) throws NoSuchMethodException {
        return type.getDeclaredMethod(methodName, new Class[]{ Callback[].class });
    }
```
现在就比较明朗了，Enhancer类 在通过父类AbstractClassGenerator 的 create 方法得到一个 Class对象之后，在通过自己的firstInstance(Class type) 方法得到一个实例(即代理对象)。在生成代理实例的过程中，通过反射技术，调用了代理类的 CGLIBSET_THREAD_CALLBACKS  方法在代理类(LearnImple1c74c1d.class)中是一个静态方法，而静态方法的调用是可以不依赖实例的 **
```
Method setter = getCallbacksSetter(type, methodName);
    setter.invoke(null, new Object[]{ callbacks });
```
代理类(LearnImple1c74c1d.class)中的CGLIB$SET_THREAD_CALLBACKS 源码如下，就是给 ThreadLocal 设置了一个值，这个值就是我们在main方法中传进去的 LogMethodInterceptor 实例：
```
public static void CGLIB$SET_THREAD_CALLBACKS(Callback[] var0) {
        CGLIB$THREAD_CALLBACKS.set(var0);
    }
```
至此，有关于代理类中的各个变量值来源已经真相大白，唯一还需要解释的就是 intercept 方法。
之前我们说过，CGLIB动态代理速度比JDK动态代理运行速度更快，那么它为什么可以更快呢？我们知道，JDK动态代理是通过放射调用目标类的方法的，反射的速度很慢，所以CGLIB并没有采用反射的技术调用目标方法，**而是通过建立方法与下标的映射来调用方法**，所以这里很有必要研究一下 intercept 方法。
```
Class var0 = Class.forName("com.hand.hsp.proxy.LearnImpl$$EnhancerByCGLIB$$e1c74c1d");
Class var1;
Method[] var10000 = ReflectUtils.findMethods(new String[]{"finalize", "()V", "equals", "(Ljava/lang/Object;)Z", "toString", "()Ljava/lang/String;", "hashCode", "()I", "clone", "()Ljava/lang/Object;"}, (var1 = Class.forName("java.lang.Object")).getDeclaredMethods());
CGLIB$learnChinese$1$Method = var10000[1];
CGLIB$learnChinese$1$Proxy = MethodProxy.create(var1, var0, "()V", "learnChinese", "CGLIB$learnChinese$1");
.....
var10000.intercept(this, CGLIB$learnChinese$1$Method, CGLIB$emptyArgs, CGLIB$learnChinese$1$Proxy)
......
public Object intercept(Object object, Method method, Object[] args, MethodProxy methodProxy) throws Throwable {
    System.out.println("执行代理方法之前");
    Object invoke = methodProxy.invokeSuper(object, args);
    System.out.println("执行代理方法之后");
    return invoke;
}
```
可以看到，intercept 接收 4个参数
- Object object：代理实例(LearnImple1c74c1d.class 实例)
- Method method：代理实例中的方法(CGLIB1$Method)
- Object[] args：参数(这里为空)
- MethodProxy methodProxy：MethodProxy实例(CGLIB1$Proxy)
> 
()V 表示 该方法的参数类表为空，返回类型为 void。
(Ljava/lang/Object;)Z 表示该方法接收一个 Object类型的参数，返回类型为 boolean
其中最关键的可能就是 MethodProxy实例了，也就是CGLIB1$Proxy ，发现它是通过MethodProxy.create方法创建的，该方法实现如下：
```
public static MethodProxy create(Class c1, Class c2, String desc, String name1, String name2) {
        MethodProxy proxy = new MethodProxy();
        proxy.sig1 = new Signature(name1, desc);
        proxy.sig2 = new Signature(name2, desc);
        proxy.createInfo = new CreateInfo(c1, c2);
        return proxy;
    }
```
解释一下sig1 和 sig2 。在代理类的源码中我们已经发现，除了生成一个与目标类的同名的learnChinese方法，还生成了一个对应 CGLIB1 方法，而 CGLIB1 方法内部仅仅是调用了目标类的learnChinese方法。这里的sig1 和 sig2 就分别对象这两个方法。
**其实调用 invokeSuper 方法的逻辑大致如下：先执行方法内部的逻辑，当执行  methodProxy.invokeSuper(object, args); 的时候，通过下标的方式找到CGLIB1 方法，然后调用，这样就间接达到调用目标类方法的目的。**
MethodProxy 的 invokeSuper方法实现如下：
```
public Object invokeSuper(Object obj, Object[] args) throws Throwable {
        try {
            init();
            FastClassInfo fci = fastClassInfo;
            return fci.f2.invoke(fci.i2, obj, args);
        } catch (InvocationTargetException e) {
            throw e.getTargetException();
        }
    }
    private void init()
    {
        /* 
         * Using a volatile invariant allows us to initialize the FastClass and
         * method index pairs atomically.
         * 
         * Double-checked locking is safe with volatile in Java 5.  Before 1.5 this 
         * code could allow fastClassInfo to be instantiated more than once, which
         * appears to be benign.
         */
        if (fastClassInfo == null)
        {
            synchronized (initLock)
            {
                if (fastClassInfo == null)
                {
                    CreateInfo ci = createInfo;
                    FastClassInfo fci = new FastClassInfo();
                    fci.f1 = helper(ci, ci.c1);
                    fci.f2 = helper(ci, ci.c2);
                    fci.i1 = fci.f1.getIndex(sig1);
                    fci.i2 = fci.f2.getIndex(sig2);
                    fastClassInfo = fci;
                    createInfo = null;
                }
            }
        }
    }
```
init方法中，先生成LearnImplc714789 类，然后根据 sig1 和 sig2 获取到对应方法的下标。所谓方法和下标映射，就是以下形式，这是在 LearnImplc714789 的实现：
```
public int getIndex(Signature var1) {
        String var10000 = var1.toString();
        switch(var10000.hashCode()) {
        case -1725733088:
            if (var10000.equals("getClass()Ljava/lang/Class;")) {
                return 8;
            }
            break;
        case -1026001249:
            if (var10000.equals("wait(JI)V")) {
                return 3;
            }
            break;
        case 4351180:
            if (var10000.equals("learnChinese()V")) {
                return 1;
            }
            break;
        case 243996900:
            if (var10000.equals("wait(J)V")) {
                return 4;
            }
            break;
        case 800444521:
            if (var10000.equals("learnEnglish()V")) {
                return 0;
            }
            break;
        case 946854621:
            if (var10000.equals("notifyAll()V")) {
                return 10;
            }
            break;
        case 1116248544:
            if (var10000.equals("wait()V")) {
                return 2;
            }
            break;
        case 1826985398:
            if (var10000.equals("equals(Ljava/lang/Object;)Z")) {
                return 5;
            }
            break;
        case 1902039948:
            if (var10000.equals("notify()V")) {
                return 9;
            }
            break;
        case 1913648695:
            if (var10000.equals("toString()Ljava/lang/String;")) {
                return 6;
            }
            break;
        case 1984935277:
            if (var10000.equals("hashCode()I")) {
                return 7;
            }
        }
        return -1;
    }
```
然后
```
fci.f2.invoke(fci.i2, obj, args);
```
这行代码就是调用了 LearnImplc714789 中的 invoke 方法
- fci.i2 对应的下标，其实就是 CGLIB1 对应的下标
- obj 代理实例，LearnImple1c74c1d.class
- args 参数，这里为空
至此，整体流程已经很明了，整体梳理一遍：
```
Enhancer enhancer = new Enhancer();
enhancer.setSuperclass(LearnImpl.class);
enhancer.setCallback(new LogMethodInterceptor());
Learn realLearn = (LearnImpl) enhancer.create();
realLearn.learnChinese();
```
**Enhancer在创建代理实例的时候，维护了新生成代理实例与LogMethodInterceptor的关系。然后通过代理实例调用learnChinese方法的时候，代理实例的learnChinese 方法内部调用了 LogMethodInterceptor 的 intercept 方法，而 intercept 方法内部又调用了MethodProxy的invokeSuper方法，invokeSuper 方法通过下标的形式调用了代理实例的CGLIB1方法，而CGLIB1方法内部调用了父类的learnChinese方法(即目标类的learnChinese方法)，这样就实现了不通过反射的方式间接的调用目标类的learnChinese方法。**
