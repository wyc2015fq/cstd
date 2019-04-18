# Springmvc中在controller注入request会有线程安全问题吗 - z69183787的专栏 - CSDN博客
2017年12月20日 10:27:12[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1114
```
```
@Controller
public class AController{
    @Autowire
    HttpServletRequest request;
    
    @RequestMapping("/test")
    public Result test(){
        System.out.println(request.toString());
        request.getHeader("uid");
    }
}
```
```
例如上述代码，
我使用`Autowire`注入`request`后，直接在`controller`的方法中使用`request`，
由于`controller`默认是单例的，我在想是否会有线程安全问题。
因为我输出了`request`的`hashcode`发现每次请求`hashcode`都是一样的。
那么后面的`request`是否会覆盖当前`request`导致信息失真？
·····························补充··························
1、我想在`controller`的每个方法里都使用`HttpServletRequest`，那么每次在方法里都要声明我觉得就比较麻烦了？不知道大家怎么解决这个问题？
2、我这样写的原因是，我想通过继承一个父类的方式，让request作为父类的一个成员变量，这样可以在方法里直接使用。
3、我通过楼下叉叉哥的方式(之前就是这样写的)
```
public Result test(HttpServletRequest request){
    System.out.println(request.toString());
}
```
同样不断访问，或者用不同客户端访问。发现打印出来的每个请求的request的hashcode居然也是相同的，为什么？
这么写是线程安全的，原因如下：
- 在controller中注入的request是jdk动态代理对象,ObjectFactoryDelegatingInvocationHandler的实例.当我们调用成员域request的方法的时候其实是调用了objectFactory的getObject()对象的相关方法.这里的objectFactory是RequestObjectFactory.
- RequestObjectFactory的getObject其实是从RequestContextHolder的threadlocal中去取值的.
- 请求刚进入springmvc的dispatcherServlet的时候会把request相关对象设置到RequestContextHolder的threadlocal中去.
参考： [在SpringMVC Controller中注入Request成员域](https://www.cnblogs.com/abcwt112/p/7777258.html)
