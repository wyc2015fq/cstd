# CGlib的动态代理使用示例 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年10月13日 23:36:49[boonya](https://me.csdn.net/boonya)阅读数：7189标签：[Java																[cglib																[proxy																[asm](https://so.csdn.net/so/search/s.do?q=asm&t=blog)
个人分类：[Java](https://blog.csdn.net/boonya/article/category/878447)





**1、cglib简述**



           Cglib是一个优秀的动态代理框架，它的底层使用ASM在内存中动态的生成被代理类的子类，使用CGLIB即使代理类没有实现任何接口也可以实现动态代理功能。CGLIB具有简单易用，它的运行速度要远远快于JDK的Proxy动态代理：

使用CGLIB需要导入以下两个jar文件：

    $1:    asm.jar – CGLIB的底层实现

    $2:   cglib.jar – CGLIB的核心jar包。

**CGLIB的核心类：**

    net.sf.cglib.proxy.Enhancer – 主要的增强类

    net.sf.cglib.proxy.MethodInterceptor – 主要的方法拦截类，它是Callback接口的子接口，需要用户实现

    net.sf.cglib.proxy.MethodProxy – JDK的java.lang.reflect.Method类的代理类，可以方便的实现对源对象方法的调用,如使用：

    Object o = methodProxy.invokeSuper(proxy, args);//虽然第一个参数是被代理对象，也不会出现死循环的问题。
**2、简单的cglib代理使用**
前述：Person 类和Dog类都有一个方法叫做avtive(); Dog的avtive()方法来实现自IAnimal接口。



```java
package com.boonya.proxy.cglib;

import java.lang.reflect.Method;
import net.sf.cglib.proxy.Enhancer;
import net.sf.cglib.proxy.MethodInterceptor;
import net.sf.cglib.proxy.MethodProxy;

public class CglibObjectProxy {

	@SuppressWarnings("rawtypes")
	public static Object ceateProxtObject(final Object object,Class clazz) {
		// 声明增加类实例
		Enhancer en = new Enhancer();
		// 设置被代理类字节码，CGLIB根据字节码生成被代理类的子类
		en.setSuperclass(clazz);
		// 设置回调函数，即一个方法拦截
		en.setCallback(new MethodInterceptor() {

			@Override
			public Object intercept(Object arg0, Method method, Object[] args,
					MethodProxy arg3) throws Throwable {
				
				// 注意参数object,仍然为外部声明的源对象，且Method为JDK的Method反射
				Object o = method.invoke(object, args);

				return o;
			}
		});
		return en.create();
	}
	
	public static void main(String[] args) {
		// 未实现接口的类的代理
		Person proxyPerson=(Person) CglibObjectProxy.ceateProxtObject(new Person(),Person.class);
		proxyPerson.active("Talk with sb.");
		// 实现接口的类的代理
		IAnimal proxyDog=(IAnimal) CglibObjectProxy.ceateProxtObject(new Dog(),Dog.class);
		proxyDog.active("Dog lying ...........");
		
	}

}
```
**3、优化封装cglib的使用对象**




```java
package com.boonya.proxy.cglib;

import java.lang.reflect.Method;
import net.sf.cglib.proxy.Enhancer;
import net.sf.cglib.proxy.MethodInterceptor;
import net.sf.cglib.proxy.MethodProxy;

public class CglibProxy implements MethodInterceptor {

	private Object srcTarget;

	private CglibProxy(Object o) {
		this.srcTarget = o;
	}

	@SuppressWarnings("unchecked")
	public static <T> T proxyTarget(T t) {
		Enhancer en = new Enhancer();
		en.setSuperclass(t.getClass());
		en.setCallback(new CglibProxy(t));
		T tt = (T) en.create();
		return tt;
	}

	@Override
	public Object intercept(Object obj, Method method, Object[] args,
			MethodProxy proxy) throws Throwable {

		Object o = method.invoke(srcTarget, args);
		return o;
	}

	public static void main(String[] args) {
		// 未实现接口的类的代理
		Person person = CglibProxy.proxyTarget(new Person());
		person.active("Tom speaking ........");
		// 实现接口的类的代理
		IAnimal dog = CglibProxy.proxyTarget(new Dog());
		dog.active("Dog is running.........");
	}

}
```


参考地址：[http://www.open-open.com/lib/view/open1335662371030.html](http://www.open-open.com/lib/view/open1335662371030.html)](https://so.csdn.net/so/search/s.do?q=proxy&t=blog)](https://so.csdn.net/so/search/s.do?q=cglib&t=blog)](https://so.csdn.net/so/search/s.do?q=Java&t=blog)




