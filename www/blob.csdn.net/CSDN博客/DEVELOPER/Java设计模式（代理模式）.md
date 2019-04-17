# Java设计模式（代理模式） - DEVELOPER - CSDN博客





2017年04月13日 11:13:03[学术袁](https://me.csdn.net/u012827205)阅读数：983标签：[Java设计模式](https://so.csdn.net/so/search/s.do?q=Java设计模式&t=blog)
个人分类：[java设计模式](https://blog.csdn.net/u012827205/article/category/2345605)

所属专栏：[JAVA设计模式](https://blog.csdn.net/column/details/15234.html)









今天上午趁着老板不在偷偷在网上又温习了一下代理模式，这种偷着公司时间拿来学习的感觉真过瘾，就像我的偶像“白百何事件”一样！其实，对于白百何这件事情我觉得挺正常。我们之所以觉得不正常，而且新闻上的头条啊？？！疑问就处在她的身份太不普通。至少，唉！比我苦逼写代码强多了～  binggo！回归正题吧，别再操心别人的闲事了！！白百何，我支持你。

其实说到代理模式，我印象里都不咋使用它！但是对她，我还是充满了好奇心。

那到底什么是代理模式麽？？代理模式是给某一个对象提供一个代理对象，并由代理对象控制对原对象的引用。（代理模式使用代理对象完成用户请求，屏蔽用户对真实对象的访问。）

举例说明：就是我的玛莎拉蒂跑车被偷，并且猫贼已经被逮到。我要起诉她！碍于面子地位和法律专业知识，我请了一名大律师帮我处理这件事！而律师（代理）可以在我（委托人）和猫贼（目标对象）之前起到中介的作用。

作为天真的代理模式，她有：静态代理、动态代理

静态代理特点：所谓静态代理就是程序运行前就已经存在的编译好的代理类。（缺点：有时候会显得非常麻烦，不灵活）

动态代理特点：它不用在我们设计实现的时候，就指定某一个代理类来代理哪一个被代理对象。可以把这种指定延迟到程序运行时，并由JVM来实现。（优点：1、减少编程的工作量：假如需要实现多种代理处理逻辑，只要写多个代理处理器就可以了，无需每种方式都写一个代理类。2、系统扩展性和维护性增强，程序修改起来也方便多了！一般只要改代理处理器类就行了。）

静态



```java
public interface IProxyListener {
    //代理接口 实现打官司功能(首先创建一个打官司行为的接口)
    String lawRequest(String lawInfo);
}
```

```java
//客户（委托人）
public class Client implements IProxyListener{

    public void prepareWork(){
        //自己维权 为打官司做准备 （耗时操作）
        //有可能打官司前期准备要耗时几个月
        System.out.println("打官司～做准备～耗时");
    }

    @Override
    public String lawRequest(String lawInfo) {
        String doing = "正在打官司！";
        return doing;
    }
}
```

```java
///律师 （代理）
public class Lawyer implements IProxyListener {
    private Client client;

    public Lawyer(Client client) {
        //通过代理模式实现代理者对委托人的权限控制
        //初始化代理类，接受客户端打官司委托；
        // 而非真实的为客户打官司，其实律师（代理类）什么都没有做。
        // 不像客户一样，并没有进行耗时任务操作，减少资源消耗。
        //当客户要真正进行打官司，则会去调用方法lawRequest(String lawInfo)
        this.client = client;

    }

    @Override
    public String lawRequest(String lawInfo) {
        //准备工作
        client.prepareWork();
        //律师（代理） 代理 用户（委托人）打官司
        return client.lawRequest(lawInfo);
    }
}
```

```java
//    main实现方法
    public static void main(String[] args){
        //实现用户或者客户的打官司工作 全权交给律师 代理完成
        Lawyer lawyer = new Lawyer(new Client());
        lawyer.lawRequest("开始打官司");
    }
```


动态
```java
public interface IProxyListener {
    //代理接口 实现打官司功能(首先创建一个打官司行为的接口)
    String lawRequest(String lawInfo);
}
```


```java
//客户（委托人）
public class Client implements IProxyListener{

    public void prepareWork(){
        //自己维权 为打官司做准备 （耗时操作）
        //有可能打官司前期准备要耗时几个月
        System.out.println("打官司～做准备～耗时");
    }

    @Override
    public String lawRequest(String lawInfo) {
        String doing = "正在打官司！";
        return doing;
    }
}
```



```java
//动态代理 具备普遍性
public class ClientProxyHandler implements InvocationHandler {
    // 委托人（被代理类）的实例
    private Object client;

    // 将被代理者的实例传进动态代理类的构造函数中
    // 通过代理模式实现代理者对委托人的权限控制
    public ClientProxyHandler(Object client){
        this.client = client;
    }

    /**
     * 重写 InvocationHandler 接口中的 invoke() 方法
     * 更重要的是，动态代理模式可以使得我们在不改变原来已有的代码结构
     * 的情况下，对原来的“真实方法”进行扩展、增强其功能，并且可以达到
     * 代替委托人执行行为，下面的before、after就是我们可以进行特殊
     * 代码切入的扩展点了。
     */
    @Override
    public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {

        //before:do something TODO: 2017/4/13
        Object ob = method.invoke(this.client, args);
        // after:do something TODO: 2017/4/13

        return ob;
    }
}
```

```java
public static void main(String[] args){
        //创建真实委托人对象
        Client client = new Client();
        //委托人 授权 代理 ；交接个人权力
        ClientProxyHandler handler = new ClientProxyHandler(client);

          /*
         * 通过类Proxy的方法 newProxyInstance创建代理对象
         * 第一个参数 handler.getClass().getClassLoader() ，使用handler类的ClassLoader对象来加载代理对象
         * 第二个参数 client.getClass().getInterfaces()，为代理对象提供的接口是委托人所施行行为的接口，表示律师代理的是委托人。接下来就能调用接口中的方法实现各种行为手段了；
         * 第三个参数handler， 我们这里将这个代理对象关联到了上方的 InvocationHandler 这个对象上
         */
        IProxyListener iProxyListener = (IProxyListener) Proxy.newProxyInstance(handler.getClass().getClassLoader(),
                client.getClass().getInterfaces(),
                handler);
        iProxyListener.lawRequest("立即打官司~");
    }
```






附图理解：经典博客链接<[https://www.zybuluo.com/pastqing/note/174679](https://www.zybuluo.com/pastqing/note/174679)>

![](https://img-blog.csdn.net/20170413135603644)




