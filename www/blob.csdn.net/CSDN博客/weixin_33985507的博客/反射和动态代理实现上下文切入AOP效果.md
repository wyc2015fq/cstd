# 反射和动态代理实现上下文切入AOP效果 - weixin_33985507的博客 - CSDN博客
2014年03月29日 16:04:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
Java的反射框架提供了动态代理机制，允许在运行期对目标类生成代理，避免重复开发，实现上下文切入的功能。
代码是最好的交流语言：
Subject接口
RealSubject实现接口
SubjectHandler实现上下文切入，非显式动态代理功能
```
interface Subject {
    public String request(int[] array);
    
    public void anotherRequest();
}
```
```
public class RealSubject implements Subject {
    @Override
    public String request(int[] array) {
        System.out.println("real do something");
        for(int at:array) {
            System.out.print(at+" ");
        }
        System.out.println();
        return "";
    }
    @Override
    public void anotherRequest() {
        System.out.println("anotherRequest");
    }
    
    
    public void ownMethod() {
        System.out.println("ownMethod");
    }
    
}
```
```
import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
public class SubjectHandler implements InvocationHandler{
    private Subject subject;
    public SubjectHandler(Subject _subject) {
        subject = _subject;
    }
    @Override
    public Object invoke(Object proxy, Method method, Object[] args)
            throws Throwable {
        System.out.println("预处理...权限校验");
        Object obj = method.invoke(subject, args);
        System.out.println("后处理...数据校验");
        return obj;
    }
    
}
```
下面是如何调用：
```
public static void main(String[] args) {
        
        Subject subject = new RealSubject();
        InvocationHandler handler = new SubjectHandler(subject);
        ClassLoader cl = subject.getClass().getClassLoader();
        /*
         * Returns an instance of a proxy class for the specified interfaces 
         * that dispatches method invocations to the specified invocation handler.
         * 传入的是Subject接口的所有方法
         */
        Subject proxy = (Subject) Proxy.newProxyInstance(cl, subject.getClass().getInterfaces(), handler);
        
        //调用其中一个方法，有传入参数和返回参数
        int[] array = {1,2,3};
        String a =proxy.request(array);
        System.out.println(a);
        
        //调用另外一个方法
        proxy.anotherRequest();
        /**
         * 用了代理和反射实现了上下文切入！
         * 不需要显式创建代理类即实现代理功能，而且不仅仅代理Subject，
         * 在handler中读取持久化数据即可实现动态代理，而且invoke上下处理，还可以实现动态切入的效果，这也是AOP编程概念
         */
        
    }
```
输入如下
```
预处理...权限校验
real do something
1 2 3 
后处理...数据校验
预处理...权限校验
anotherRequest
后处理...数据校验
```
