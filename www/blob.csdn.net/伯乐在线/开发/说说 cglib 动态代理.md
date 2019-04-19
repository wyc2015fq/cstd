# 说说 cglib 动态代理 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [占小狼](http://www.jobbole.com/members/wx4097456919) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
### 前言
JDK中的动态代理通过反射类`Proxy`和`InvocationHandler`回调接口实现，要求委托类必须实现一个接口，只能对该类接口中定义的方法实现代理，这在实际编程中有一定的局限性。
### cglib实现
使用[cglib[Code Generation Library]](https://github.com/cglib/cglib)实现动态代理，并不要求委托类必须实现接口，底层采用asm字节码生成框架生成代理类的字节码，下面通过一个例子看看使用CGLib如何实现动态代理。
1、定义业务逻辑


```
public class UserServiceImpl {  
    public void add() {  
        System.out.println("This is add service");  
    }  
    public void delete(int id) {  
        System.out.println("This is delete service：delete " + id );  
    }  
}
```
2、实现`MethodInterceptor`接口，定义方法的拦截器


```
public class MyMethodInterceptor implements MethodInterceptor {
    public Object intercept(Object obj, Method method, Object[] arg, MethodProxy proxy) throws Throwable {
        System.out.println("Before:" + method);  
        Object object = proxy.invokeSuper(obj, arg);
        System.out.println("After:" + method); 
        return object;
    }
}
```
3、利用`Enhancer`类生成代理类；


```
Enhancer enhancer = new Enhancer();  
enhancer.setSuperclass(UserServiceImpl.class);  
enhancer.setCallback(new MyMethodInterceptor());  
UserServiceImpl userService = (UserServiceImpl)enhancer.create();
```
4、`userService.add()`的执行结果：


```
Before: add
This is add service
After: add
```
代理对象的生成过程由[Enhancer](http://www.boyunjian.com/javasrc/org.sonatype.sisu.inject/cglib/3.0/_/net/sf/cglib/proxy/Enhancer.java)类实现，大概步骤如下：
1、生成代理类Class的二进制字节码；
2、通过`Class.forName`加载二进制字节码，生成Class对象；
3、通过反射机制获取实例构造，并初始化代理类对象。
### cglib字节码生成
Enhancer是CGLib的字节码增强器，可以方便的对类进行扩展，内部调用`GeneratorStrategy.generate`方法生成代理类的字节码，通过以下方式可以生成class文件。


```
System.setProperty(DebuggingClassWriter.DEBUG_LOCATION_PROPERTY, "C:\\\\Code\\\\whywhy\\\\target\\\\classes\\\\zzzzzz")
```
使用 [反编译工具 procyon](https://bitbucket.org/mstrobel/procyon/downloads) 查看代理类实现


```
java -jar procyon-decompiler-0.5.30.jar UserService\(EnhancerByCGLIB\)394dddeb;
```
反编译之后的代理类add方法实现如下：


```
import net.sf.cglib.core.Signature;
import net.sf.cglib.core.ReflectUtils;
import net.sf.cglib.proxy.MethodProxy;
import java.lang.reflect.Method;
import net.sf.cglib.proxy.MethodInterceptor;
import net.sf.cglib.proxy.Callback;
import net.sf.cglib.proxy.Factory;
// 
// Decompiled by Procyon v0.5.30
// 
public class UserService\(EnhancerByCGLIB\)394dddeb extends UserService implements Factory
{
    private boolean CGLIB$BOUND;
    private static final ThreadLocal CGLIB$THREAD_CALLBACKS;
    private static final Callback[] CGLIB$STATIC_CALLBACKS;
    private MethodInterceptor CGLIB$CALLBACK_0;
    private static final Method CGLIB$add$0$Method;
    private static final MethodProxy CGLIB$add$0$Proxy;
    private static final Object[] CGLIB$emptyArgs;
    static void CGLIB$STATICHOOK2() {
        CGLIB$THREAD_CALLBACKS = new ThreadLocal();
        CGLIB$emptyArgs = new Object[0];
        final Class<?> forName = Class.forName("UserService\(EnhancerByCGLIB\)394dddeb");
        final Class<?> forName3;
        CGLIB$add$0$Method = ReflectUtils.findMethods(new String[] { "add", "()V" }, (forName3 = Class.forName("UserService")).getDeclaredMethods())[0];
        CGLIB$add$0$Proxy = MethodProxy.create((Class)forName3, (Class)forName, "()V", "add", "CGLIB$add$0");
    }
    final void CGLIB$add$0() {
        super.add();
    }
    public final void add() {
        MethodInterceptor cglib$CALLBACK_2;
        MethodInterceptor cglib$CALLBACK_0;
        if ((cglib$CALLBACK_0 = (cglib$CALLBACK_2 = this.CGLIB$CALLBACK_0)) == null) {
            CGLIB$BIND_CALLBACKS(this);
            cglib$CALLBACK_2 = (cglib$CALLBACK_0 = this.CGLIB$CALLBACK_0);
        }
        if (cglib$CALLBACK_0 != null) {
            cglib$CALLBACK_2.intercept((Object)this, UserService\(EnhancerByCGLIB\)394dddeb.CGLIB$add$0$Method, UserService\(EnhancerByCGLIB\)394dddeb.CGLIB$emptyArgs, UserService\(EnhancerByCGLIB\)394dddeb.CGLIB$add$0$Proxy);
            return;
        }
        super.add();
    }
    static {
        CGLIB$STATICHOOK2();
    }
}
```
通过cglib生成的字节码相比jdk实现来说显得更加复杂。
1、代理类`UserService\(EnhancerByCGLIB\)394dddeb`继承了委托类`UserSevice`，且委托类的final方法不能被代理；
2、代理类为每个委托方法都生成两个方法，以add方法为例，一个是重写的add方法，一个是CGLIB$add$0方法，该方法直接调用委托类的add方法；
3、当执行代理对象的add方法时，会先判断是否存在实现了MethodInterceptor接口的对象`cglib$CALLBACK_0`，如果存在，则调用MethodInterceptor对象的`intercept`方法：


```
public Object intercept(Object obj, Method method, Object[] arg, MethodProxy proxy) {
    System.out.println("Before:" + method);  
    Object object = proxy.invokeSuper(obj, arg);
    System.out.println("After:" + method); 
    return object;
}
```
参数分别为：1、代理对象；2、委托类方法；3、方法参数；4、代理方法的MethodProxy对象。
4、每个被代理的方法都对应一个MethodProxy对象，`methodProxy.invokeSuper`方法最终调用委托类的add方法，实现如下：


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
```
单看`invokeSuper`方法的实现，似乎看不出委托类add方法调用，在MethodProxy实现中，通过FastClassInfo维护了委托类和代理类的FastClass。


```
private static class FastClassInfo {
    FastClass f1;
    FastClass f2;
    int i1;
    int i2;
}
```
以add方法的methodProxy为例，f1指向委托类对象，f2指向代理类对象，i1和i2分别是方法add和CGLIB$add$0在对象中索引位置。
### FastClass实现机制
FastClass其实就是对Class对象进行特殊处理，提出下标概念index，通过索引保存方法的引用信息，将原先的反射调用，转化为方法的直接调用，从而体现所谓的fast，下面通过一个例子了解一下FastClass的实现机制。
1、定义原类


```
class Test {
    public void f(){
        System.out.println("f method");
    }
    public void g(){
        System.out.println("g method");
    }
}
```
2、定义Fast类


```
class FastTest {
    public int getIndex(String signature){
        switch(signature.hashCode()){
        case 3078479:
            return 1;
        case 3108270:
            return 2;
        }
        return -1;
    }
    public Object invoke(int index, Object o, Object[] ol){
        Test t = (Test) o;
        switch(index){
        case 1:
            t.f();
            return null;
        case 2:
            t.g();
            return null;
        }
        return null;
    }
}
```
在FastTest中有两个方法，`getIndex`中对Test类的每个方法根据hash建立索引，`invoke`根据指定的索引，直接调用目标方法，避免了反射调用。所以当调用`methodProxy.invokeSuper`方法时，实际上是调用代理类的`CGLIB$add$0`方法，`CGLIB$add$0`直接调用了委托类的add方法。
### jdk和cglib动态代理实现的区别
1、jdk动态代理生成的代理类和委托类实现了相同的接口；
2、cglib动态代理中生成的字节码更加复杂，生成的代理类是委托类的子类，且不能处理被final关键字修饰的方法；
3、jdk采用反射机制调用委托类的方法，cglib采用类似索引的方式直接调用委托类方法；
END。
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
![](http://jbcdn2.b0.upaiyun.com/2016/08/c3fbbefbe3e302d0f6944c94db3cb1c4.jpg)
