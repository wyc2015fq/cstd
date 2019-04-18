# Java反射机制和动态代理实例 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年12月09日 09:19:21[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：1962
个人分类：[Java](https://blog.csdn.net/fjssharpsword/article/category/6480440)









反射机制是Java语言的一个重要特性，允许用户动态获取类的信息和动态调用对象的方法。

通过给定类的名字，通过反射机制就可以获取类的所有信息。

JAVA反射机制是在运行状态中，对于任意一个类，都能够知道这个类的所有属性和方法；对于任意一个对象，都能够调用它的任意一个方法；这种动态获取的信息以及动态调用对象的方法的功能称为java语言的反射机制。

Java反射机制主要提供了以下功能： 在运行时判断任意一个对象所属的类；在运行时构造任意一个类的对象；在运行时判断任意一个类所具有的成员变量和方法；在运行时调用任意一个对象的方法；生成动态代理。

参考案例如下：



```java
package com.reflect;

public interface CalculatorProtocol {
	//定义个接口协议
	public int add(int a,int b);//两个数相加
	public int subtract(int a,int b);//两个数相减
}
```

```java
package com.reflect;

public class Server implements CalculatorProtocol{
	public int add(int a,int b){
		return a+b;
	}
	public int subtract(int a,int b){
		return a-b;
	}
}
```


```java
package com.reflect;

import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;

public class CalculatorHandler implements InvocationHandler, net.sf.cglib.proxy.InvocationHandler{
	//实现调用处理器接口
	private Object objOriginal;
	
	public CalculatorHandler(Object obj){
		this.objOriginal=obj;
	}
	
	public Object invoke(Object proxy,Method method,Object[] args) throws Throwable{
		//此处添加预处理
		Object result=method.invoke(this.objOriginal, args);
		//此处添加后续处理
		return result;
	}
}
```


```java
package com.reflect;

import java.lang.reflect.InvocationHandler;

import net.sf.cglib.proxy.Proxy;

public class DynamicProxyExample {
	public static void main(String[] args){
		CalculatorProtocol server = new Server();//创建server
		InvocationHandler handler=new CalculatorHandler(server);
		CalculatorProtocol client=(CalculatorProtocol)Proxy.newProxyInstance(
				server.getClass().getClassLoader(),
				server.getClass().getInterfaces(), 
				(net.sf.cglib.proxy.InvocationHandler) handler);
		//创建client
		int r=client.add(5, 3);
		System.out.println("5+3="+r);
		r=client.subtract(5, 3);
		System.out.println("5-3="+r);
	}
}
```









