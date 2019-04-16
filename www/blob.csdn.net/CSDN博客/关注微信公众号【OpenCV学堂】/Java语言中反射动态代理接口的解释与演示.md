# Java语言中反射动态代理接口的解释与演示 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2015年11月19日 16:59:25[gloomyfish](https://me.csdn.net/jia20003)阅读数：4131








**Java语言中反射动态代理接口的解释与演示**


Java在JDK1.3的时候引入了动态代理机制、可以运用在框架编程与平台编程时候捕获事件、审核数据、日志等功能实现，首先看一下设计模式的UML图解：

![](https://img-blog.csdn.net/20151119164704099?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




当你调用一个接口API时候，实际实现类继承该接口，调用时候经过proxy实现。

在Java中动态代理实现的两个关键接口类与class类分别如下：

java.lang.reflect.Proxy 

java.lang.reflect.InvocationHandler

我们下面就通过InvocationHandler接口来实现动态代理过程，通过Proxy接口创建

一个代理类，然后测试完整的程序。要实现演示Demo需要如下几步：

一：首先定义我们自己的POJO对象接口类IExample与IUser



```java
package com.example.pojo;

public interface IExample {
	
	public void setName(String name);
	
	public String getName();
	
	public void setDesc(String description);
	
	public String getDesc();
	
}
```

```java
package com.example.pojo;

public interface IUser {
	
	public void setUserID(String userID);
	
	public String getUserID();
	
	public void setUserName(String userName);
	
	public String getUserName();

}
```
二：实现我们自己InvocationHandler接口，其中map我用来存储POJO对象的数据，这样做的好处是POJO接口无需再创建实现类，只有定义接口就可以通过代理直接使用该类，这在实际项目开发中非常有用。




```java
package com.example.reflection;

import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;
import java.util.HashMap;
import java.util.Map;

public class MyProxyView implements InvocationHandler {
	private Map<Object, Object> map = null;

	public static Object newInstance(Class[] interfaces) {
		return Proxy.newProxyInstance(MyProxyView.class.getClassLoader(),
				interfaces, new MyProxyView());
	}

	private MyProxyView() {
		this.map = new HashMap<Object, Object>();
	}

	@Override
	public Object invoke(Object proxy, Method method, Object[] args)
			throws Throwable {
		Object result = null;
		String methodName = method.getName();
		if (methodName.startsWith("get")) {
			String name = methodName.substring(methodName.indexOf("get") + 3);
			return map.get(name);
		} else if (methodName.startsWith("set")) {
			String name = methodName.substring(methodName.indexOf("set") + 3);
			map.put(name, args[0]);
			return null;
		} else if (methodName.startsWith("is")) {
			String name = methodName.substring(methodName.indexOf("is") + 2);
			return (map.get(name));
		}
		return result;
	}

}
```



三：通过Proxy方法初始化代理得到POJO对象，运行与测试：



```java
package com.example.reflection;

import com.example.pojo.IExample;
import com.example.pojo.IUser;

public class TextProxy {
	
	public static void main(String[] args)
	{
		IExample example = (IExample)MyProxyView.newInstance(new Class[]{IExample.class});
		
		IUser user = (IUser)MyProxyView.newInstance(new Class[]{IUser.class});
		
		// aduit bean 1
		example.setName("my example");
		example.setDesc("my proxy example");
		// aduit bean 2
		user.setUserID("jia20003");
		user.setUserName("gloomyfish");
		
		System.out.println("exmaple name : " + example.getName());
		System.out.println("exmaple desc : " + example.getDesc());
		System.out.println();
		System.out.println("user ID : " + user.getUserID());
		System.out.println("user name : " + user.getUserName());
	}

}
```
**四：运行结果如下：**




```
exmaple name : my example
exmaple desc : my proxy example

user ID : jia20003
user name : gloomyfish
```



Java动态代理方式对框架编程非常重要无论是在Web端还是桌面端

而真正把这种技术发扬光大的则是spring框架。



