# Java学习（六） 单例设计模式 - HJ - CSDN博客
2018年02月08日 13:34:49[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：94
个人分类：[Java](https://blog.csdn.net/feizaoSYUACM/article/category/7349915)
所属专栏：[Java学习](https://blog.csdn.net/column/details/19570.html)
单例设计模式
1.定义：保证一个类在内存中只有一个对象。
2.单例设计模式的步骤：
（1）饿汉单例设计模式：
①私有化构造函数；
②声明本类的引用类型变量，并且使用该变量指向本类对象；
③提供一个公共静态的方法获取本类的对象；
（2）懒汉单例设计模式：
①私有化构造函数；
②声明本类的引用类型变量，但是不要创建对象；
③提供公共静态的方法获取本类的对象，获取之前先判断是否已经创建了本类对象，如果已经创建了，那么直接返回对象即可；如果还没有创建，那么先创建本类的对象，然后再返回；
注意：推荐使用饿汉单例模式。因为懒汉单例模式会存在线程安全问题，目前还不能保证一类在内存中只有一个对象。
代码样例：
（1）饿汉单例设计模式 
```java
class Single{
	//声明本类的引用类型变量，并且使用该变量指向本类对象
	private static	Single s = new Single();
	//私有化构造函数
	private Single(){}
	//提供一个公共静态的方法获取本类的对象
	public	static  Single getInstance(){
		return s;
	}
}
class Test{
	public static Test t=new Test();
	private Test{}
	public static Test getInstance(){
		return t;
	}
}
class Test{
	public static Test t;
	private Test{}
	public static Test getInstance(){
		if(t==null){
			t=new Test();
		}
		return t;
	}
}
```
 （2）懒汉单例设计模式
```java
class Single2{
	//声明本类的引用类型变量，不创建本类的对象
	private static Single2 s;
	//私有化了构造函数
	private Single2(){}
	//
	public static Single2 getInstance(){
		if(s==null){
			s = new Single2();
		}
		return s;
	}
}
```
