# java静态代理和动态代理 - westbrook1998的博客 - CSDN博客





2018年02月19日 19:14:38[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：54标签：[java																[代理](https://so.csdn.net/so/search/s.do?q=代理&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[java](https://blog.csdn.net/westbrook1998/article/category/7459913)








## 静态代理

### 基本思路：

> 
1.创建委托类接口，定义抽象方法 

  2.创建实现类，实现接口方法 

  3.创建同样实现该接口的代理类（与实现类平级） 

  4.代理类持有委托类的引用并通过构造器构造 

  5.创建静态代理工厂类，通过静态方法返回代理类实例 

  6.在客户类里通过工厂类的方法得到代理类再通过代理类执行委托类的方法
### 代码：

```java
//委托类接口
package staticProxy;

public interface User {
    public abstract void fun(String name);
}
------
//委托类的实现
package staticProxy;

public class UserImpl implements User {
    @Override
    public void fun(String name) {
        System.out.println(name+"运行了fun方法");
    }
}
------
//代理类
package staticProxy;

public class UserProxy implements User{
    private User user;

    public UserProxy(User user) {
        this.user=user;
    }

    @Override
    public void fun(String name) {
        user.fun(name);
    }
}
------
//静态工厂类
package staticProxy;

public class UserStaticFactory {
    public static User getInstance(){
        return new UserProxy(new UserImpl());
    }
}
------
//客户类
package staticProxy;

public class Client {
    public static void main(String[] args){
        User proxy=UserStaticFactory.getInstance();
        proxy.fun("Keane");
    }
}
------
//运行结果：
//Keane运行了fun方法
```

### 优缺点：

> 
优点：业务类只关注业务逻辑本身，保证业务重用性 

  缺点： 

   - 代理类只代理一种类型的对象，不适合规模较大的程序 

   - 代理类也需实现接口的所有方法，增加代码维护的复杂度
## 动态代理

### 动态代理基于java的反射机制

### 思路：

> 
1.创建委托类接口，定义抽象方法  

  2.创建实现类，实现接口方法  

  3.创建动态代理类实现InvocationHandler接口，持有委托类的引用并通过构造器构造  

  4.代理类***重写invoke方法***(可以在这里实现添加日志等功能) 

  5.创建静态代理工厂类，通过静态方法返回代理类实例  

  6.在客户类里通过工厂类的方法得到代理类再通过代理类执行委托类的方法
### 代码：

```java
//委托类接口
package dynamicProxy;

public interface User {
    public abstract void fun(String name);
}
------
//委托类的实现
package dynamicProxy;

public class UserImpl implements User{
    @Override
    public void fun(String name) {
        System.out.println("运行了fun方法:"+name);
    }
}
------
//代理类
package dynamicProxy;

import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;

public class dynamicProxy implements InvocationHandler{
    //代理类持有一个委托类对象引用
    private Object target=null;

    public dynamicProxy(Object target){
        this.target=target;
    }

    @Override
    public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
        //通过反射调用真实委托类对象的方法，并且返回
        Object result=method.invoke(target,args);
        return result;
    }
}
------
//静态工厂类
package dynamicProxy;

import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Proxy;

public class dynamicFactory {
    public static User getInstance(){
        User user=new UserImpl();
        InvocationHandler handler=new dynamicProxy(user);
        User proxy=null;
        proxy=(User) Proxy.newProxyInstance(
                user.getClass().getClassLoader(),
                user.getClass().getInterfaces(),
                handler
        );
        //最后返回代理实例
        return proxy;
    }
}

------
//客户类
package dynamicProxy;

public class Client {
    public static void main(String[] args){
        User proxy=dynamicFactory.getInstance();
        proxy.fun("Keane");
    }
}
------
//运行结果：
//运行了fun方法:Keane
```

### 优缺点：

> 
优点：接口中声明的所有方法都被转移到调用处理器一个集中的方法——InvocationHandler.invoke中处理 

  缺点：任然依赖接口











