# Proxy 那点事儿 - 文章 - 伯乐在线
原文出处： [黄勇](http://my.oschina.net/huangyong/blog/159788)
Proxy，也就是“代理”了。意思就是，你不用去做，别人代替你去处理。比如说：赚钱方面，我就是我老婆的 Proxy；带小孩方面，我老婆就是我的 Proxy；家务事方面，没有 Proxy。
它在程序开发中起到了非常重要的作用，比如传说中的 AOP（面向切面编程），就是针对代理的一种应用。此外，在设计模式中，还有一个“代理模式”。在公司里要上外网，要在浏览器里设置一个 HTTP 代理。wocao，代理无处不在啊，不搞明白，岂不是很没面子？
凡是都要由浅入深，学习也不例外。先来一个 Hello World 吧：

Java
```
public interface Hello {
    void say(String name);
}
```
这是一个 Hello 接口，不用解释了，大家都懂的。赶紧上实现类吧：

Java
```
public class HelloImpl implements Hello {
    @Override
    public void say(String name) {
        System.out.println("Hello! " + name);
    }
}
```
wocao！这简直就是 So easy 嘛！但是有个问题：如果要在 println() 方法前面和后面分别需要处理一些逻辑，怎么做呢？把这些逻辑写死在 say() 方法里面吗？肯定不够优雅啦，菜鸟一般这样干，作为一名资深的程序员，我坚决不能这样做！
我要用代理！写一个 HelloProxy 类，让它去调用 HelloImpl 的 say() 方法，在调用的前后分别进行逻辑处理不就行了吗？赶紧搞一个吧：

Java
```
public class HelloProxy implements Hello {
    private HelloImpl helloImpl;
    public HelloProxy() {
        helloImpl = new HelloImpl();
    }
    @Override
    public void say(String name) {
        before();
        helloImpl.say(name);
        after();
    }
    private void before() {
        System.out.println("Before");
    }
    private void after() {
        System.out.println("After");
    }
}
```
我将 HelloProxy 类实现了 Hello 接口（和 HelloImpl 实现相同的接口），并且在构造方法中 new 出一个 HelloImpl 类的实例。这样一来，我就可以在 HelloProxy 的 say() 方法里面去调用 HelloImpl 的 say() 方法了。更重要的是，我还可以在调用的前后分别加上 before() 与 after() 方法，在这两个方法里去实现那些前后逻辑。
用一个 main 方法来测试一下吧：

Java
```
public static void main(String[] args) {
    Hello helloProxy = new HelloProxy();
    helloProxy.say("Jack");
}
```
运行后，打印出：
Before
Hello! Jack
After
轻而易举，我就写出了这么优雅的代码（暗自小嗨了一把）。
不久后，我在一本设计模式的书上看到，原来我写的这个 HelloProxy 就是所谓的“代理模式”啊！我只能说，自己和 GoF（四人帮）的距离有接近了一点。
于是我疯狂的使用“代理模式”，项目中到处都有 XxxProxy 的声影。直到有一天，架构师看到了我的代码，他惊呆了！他对我说：“你怎么这么喜欢用静态代理呢？你就不会用动态代理吗？给我全都重构了！”。
我表面上点了点头，说：“好的！”。其实我根本都不知道什么是“静态代理”，什么又是“动态代理”。我继续翻开我那本垫桌脚的设计模式，深入得研究了一番，最后我才明白，原来我一直用的都是“静态代理”啊，怪不得架构师说我到处都是 XxxProxy 类了。我好恨我自己！一定要将这些垃圾 Proxy 都重构为“动态代理”。
于是我就是用 JDK 给我们提供的动态代理方案，写了一个 DynamicProxy：

Java
```
public class DynamicProxy implements InvocationHandler {
    private Object target;
    public DynamicProxy(Object target) {
        this.target = target;
    }
    @Override
    public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
        before();
        Object result = method.invoke(target, args);
        after();
        return result;
    }
    ...
}
```
在 DynamicProxy 类中，我定义了一个 Object 类型的 target 变量，它就是被代理的目标对象，通过构造函数来初始化（现在流行叫“注入”了，我觉得叫“射入”也不错哦！构造函数初始化叫“正着射”，所以 reflect 方式就叫“反着射”，简称“反射”）。
言归正传，DynamicProxy 实现了 InvocationHandler 接口，那么必须实现该接口的 invoke 方法，参数不做解释，望文生义吧，是 JRE 给我们“射”进来的。在该方法中，直接通过反射去 invoke method，在调用前后分别处理 before 与 after，最后将 result 返回。
写一个 main() 方法看看实际怎么用吧：

Java
```
public static void main(String[] args) {
    Hello hello = new HelloImpl();
    DynamicProxy dynamicProxy = new DynamicProxy(hello);
    Hello helloProxy = (Hello) Proxy.newProxyInstance(
        hello.getClass().getClassLoader(),
        hello.getClass().getInterfaces(),
        dynamicProxy
    );
    helloProxy.say("Jack");
}
```
没错，意思就是，用我写的这个通用的 DynamicProxy 类去包装 HelloImpl 实例，然后再调用 JDK 给我们提供的 Proxy 类的工厂方法 newProxyInstance() 去动态地创建一个 Hello 接口的代理类，最后调用这个代理类的 say() 方法。
运行一下，结果和以前一样，动态代理成功了。其实，动态代理就是帮我们自动生成 XxxProxy 类的法宝啊！
要注意的是，Proxy.newProxyInstance() 方法的参数实在是让我“蛋碎一地”！
参数1：ClassLoader
参数2：该实现类的所有接口
参数3：动态代理对象
调用完了还要来一个强制类型转换一下。
wocao！这一坨 shi 一定要想办法封装一下，避免再次发生到处都是 Proxy.newProxyInstance()，这样架构师又要骂我了。于是我将这个 DynamicProxy 重构了：

Java
```
public class DynamicProxy implements InvocationHandler {
    ...
    @SuppressWarnings("unchecked")
    public <T> T getProxy() {
        return (T) Proxy.newProxyInstance(
            target.getClass().getClassLoader(),
            target.getClass().getInterfaces(),
            this
        );
    }
    ...
}
```
我在 DynamicProxy 里添加了一个 getProxy() 方法，无需传入任何参数，将刚才所说的那一坨 shi，放在这个方法中，并且该方法返回一个泛型类型，就不会强制类型转换了。方法头上加那个 @SuppressWarnings(“unchecked”) 注解表示忽略编译时的警告（因为 Proxy.newProxyInstance() 方法返回的是一个 Object，这里我强制转换为 T 了，这是向下转型，IDE 中就会有警告，编译时也会出现提示，很烦）。
好了，这下子使用 DynamicProxy 就简单了吧：

Java
```
public static void main(String[] args) {
    DynamicProxy dynamicProxy = new DynamicProxy(new HelloImpl());
    Hello helloProxy = dynamicProxy.getProxy();
    helloProxy.say("Jack");
}
```
确实简单用 2 行代理就去掉了前面的 7 行代码（省了 5 行），架构师看到了这样的代码肯定会表扬我！
经过一番代码重构后，我提交了所有的代码，架构师看到了，没有吱声…… 可我总算学会了动态代理。
用了这个 DynamicProxy 以后，我觉得它还是非常爽的，爽的地方是，接口变了，这个动态代理类不用动。而静态代理就不一样了，接口变了，实现类还要动，代理类也要动。但我也发现动态代理并不是“万灵丹”，它也有搞不定的时候，比如说，我要代理一个没有任何接口的类，它就没有勇武之地了！这就是 JDK 给我们提供的动态代理，让我不知道该说什么了。
于是我又开始调研，能否代理没有接口的类呢？终于让我找到了这颗“银弹”！那就是 [CGLib](http://www.oschina.net/p/cglib) 这个类库。虽然它看起来不太起眼，但 Spring、Hibernate 这样牛逼的开源框架都用到了它。它就是一个在运行期间动态生成字节码的工具，也就是动态生成代理类了。说起来好高深，实际用起来一点都不难。我再搞一个 CGLibProxy 吧：

Java
```
public class CGLibProxy implements MethodInterceptor {
    public <T> T getProxy(Class<T> cls) {
        return (T) Enhancer.create(cls, this);
    }
    public Object intercept(Object obj, Method method, Object[] args, MethodProxy proxy) throws Throwable {
        before();
        Object result = proxy.invokeSuper(obj, args);
        after();
        return result;
    }
    ...
}
```
需要实现 CGLib 给我们提供的 MethodInterceptor 实现类，并填充 intercept() 方法。方法中最后一个 MethodProxy 类型的参数 proxy，值得注意！CGLib 给我们提供的是方法级别的代理，也可以理解为对方法的拦截（这不就是传说中的“方法拦截器”吗？）。这个功能对于我们这群屌丝程序员而言，如同雪中送炭啊，此乃神器也！我们直接调用 proxy 的 invokeSuper() 方法，将被代理的对象 obj 以及方法参数 args 传入其中即可。
与 DynamicProxy 类似，我在 CGlibProxy 中也添加了一个泛型的 getProxy() 方法，便于我们可以快速地获取自动生成的代理对象。还是用一个 main() 方法来描述吧：

Java
```
public static void main(String[] args) {
    CGLibProxy cgLibProxy = new CGLibProxy();
    HelloImpl helloProxy = cgLibProxy.getProxy(HelloImpl.class);
    helloProxy.say("Jack");
}
```
仍然通过 2 行代码就可以返回代理对象了，与 JDK 动态代理不同的是，这里不需要任何的接口信息，对谁都可以生成动态代理对象（不管它是“屌丝”还是“高富帅”）。说它是神器，过分吗？
我一向都是以追求完美而著称，2 行代码返回代理对象，我觉得还是有些多余，我不想总是去 new 这个 CGLibProxy 对象，最好 new 一次，以后随时拿随时用。于是我想到了“单例模式”：

Java
```
public class CGLibProxy implements MethodInterceptor {
    private static CGLibProxy instance = new CGLibProxy();
    
    private CGLibProxy() {
    }
    public static CGLibProxy getInstance() {
        return instance;
    }
    ...
}
```
我加了以上几行代码，就搞定了！需要说明的是：这里有一个 private 的构造方法，就是为了限制外界不能再去 new 它了，换句话说，我在这里把它给“阉”了。
用一个 main() 方法来证明我的简单主义思想：

Java
```
public static void main(String[] args) {
    HelloImpl helloImpl = CGLibProxy.getInstance().getProxy(HelloImpl.class);
    helloImpl.say("Jack");
}
```
没错吧？只需 1 行代码就可以获取代理对象了！
总结一下，我们今天谈到了无代理、静态代理、JDK 动态代理、CGLib 动态代理，其实代理的世界远不止这么小，还有很多实际的应用场景。本文一开始谈到的 AOP 是一个最为典型的案例，所以有必要再进行继续下去。
