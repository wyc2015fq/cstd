# 程序设计之Java设计模式(单例模式) - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年09月27日 10:36:44[boonya](https://me.csdn.net/boonya)阅读数：935








**1、普通单例模式**

此类缺陷：无法应对多线程问题



```java
package com.boonya.pattern.singleton;
/**
 *   单例示例
 * <li>文件名称: Singleton</li> 
 * <li>文件描述: $单例示例类</li> 
 * <li>内容摘要: 单例的创建</li>
 * <li>完成日期：2013-9-27</li> 
 * <li>示例编写:  BOONYACHENGDU@GMAIL.COM</li>
 * <li>应用场景：频繁使用的对象且值不变，一如组件之类的对象</li>
 */
public class Singleton
{
	/**
	 * 无论是否调用都会实例化
	 */
    private  static  Singleton singleton=new Singleton();
	
	/**
	 * 确保外部不能实例化，即使用new关键字无效
	 */
	private Singleton(){
		System.out.println("singleton constructor");
	}
	
	/**
	 * 必须确保同步，否则多线程情况下值不统一 但 synchronized 关键字使用会降低系统性能
	 */
	public static  /*synchronized*/  Singleton  getInstance(){
		return singleton;
	}

}
```


**2、懒加载单例模式**
此类缺陷：使用多线程时系统性能明显下降



```java
package com.boonya.pattern.singleton;
/**
 *   单例示例
 * <li>文件名称: LazySingleton</li> 
 * <li>文件描述: $单例示例类</li> 
 * <li>内容摘要: 单例的创建</li>
 * <li>完成日期：2013-9-27</li> 
 * <li>示例编写:  BOONYACHENGDU@GMAIL.COM</li>
 * <li>应用场景：频繁使用的对象且值不变，一如组件之类的对象</li>
 */
public class LazySingleton
{
	/**
	 * 调用的时候再实例化
	 */
	private  static  LazySingleton singleton=null;
	
	/**
	 * 确保外部不能实例化，即使用new关键字无效
	 */
	private LazySingleton(){
		System.out.println("singleton constructor");
	}
	
	/**
	 * 必须确保同步，否则多线程情况下值不统一(线程执行先后可能判断为null)
	 */
	public static  synchronized  LazySingleton  getInstance(){
		if(singleton==null){
			singleton=new LazySingleton();
		}
		return singleton;
	}

}
```

**3、加强(plus)单例**

此类缺陷：使用反射机制时，会强行调用单例类的私有构造器，生成多个实例



```java
package com.boonya.pattern.singleton;
/**
 *   单例示例
 * <li>文件名称: PlusSingleton</li> 
 * <li>文件描述: $单例示例类</li> 
 * <li>内容摘要: 单例的创建</li>
 * <li>完成日期：2013-9-27</li> 
 * <li>示例编写:  BOONYACHENGDU@GMAIL.COM</li>
 * <li>应用场景：频繁使用的对象且值不变，一如组件之类的对象</li>
 */
public class PlusSingleton
{
	/**
	 * 确保外部不能实例化，即使用new关键字无效
	 */
	private  PlusSingleton(){
		System.out.println("singleton constructor"); 
	}
	
	/**
	 * 内部类JVM初始换调用
	 */
	private static class SingletonObject{
		private  static  PlusSingleton singleton=new PlusSingleton();
	}
	
	/**
	 * 不需要考虑多线程问题
	 */
	public static PlusSingleton  getInstance(){
		return SingletonObject.singleton;
	}

}
```

**4、序列化单例**


此类缺陷：序列化和反序列化都有可能会破坏单例



```java
package com.boonya.pattern.singleton;

import java.io.Serializable;
/**
 *   单例示例
 * <li>文件名称: Singleton</li> 
 * <li>文件描述: $单例示例类</li> 
 * <li>内容摘要: 单例的创建</li>
 * <li>完成日期：2013-9-27</li> 
 * <li>示例编写:  BOONYACHENGDU@GMAIL.COM</li>
 * <li>应用场景：频繁使用的对象且值不变，一如组件之类的对象   序列化操作需慎重</li>
 */
@SuppressWarnings("serial")
public class SerializableSingleton implements Serializable
{
	private  String context;
	
	public String getContext()
	{
		return context;
	}

	public void setContext(String context)
	{
		this.context = context;
	}

	/**
	 * 无论是否调用都会实例化
	 */
    private  static  SerializableSingleton instance=new SerializableSingleton();
	
	/**
	 * 确保外部不能实例化，即使用new关键字无效
	 */
	private SerializableSingleton(){
		System.out.println("singleton constructor");
		context="SerializableSingleton";
	}
	
	/**
	 * 获取单例对象
	 */
	public static  SerializableSingleton  getInstance(){
		return instance;
	}
	
	/**
	 * 阻止生成新的实例，总是返回当前对象(此方法不可去掉否则序列化和反序列化会出错)
	 */
	private  Object  readResolve(){
		return instance;
	}

}
```

**5、性能和注意事项**


     (1)、频繁使用的对象且值不变，一如组件之类的对象建议使用单例。此处建议使用懒加载方式，使用的时候再实例化，降低JVM初始化static 时new实例化对象消耗性能，减少GC机制的执行。

    (2)、考虑是否会在多线程中使用到，多线程中使用单例消耗性能严重。

    (3)、使用单例时，尽量不要使用序列化操作，危险难以预测，最好是具体问题具体分析。





