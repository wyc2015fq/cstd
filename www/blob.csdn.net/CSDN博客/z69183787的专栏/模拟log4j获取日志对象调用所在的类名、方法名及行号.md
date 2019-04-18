# 模拟log4j获取日志对象调用所在的类名、方法名及行号 - z69183787的专栏 - CSDN博客
2017年08月29日 16:57:00[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2164
[http://www.cnblogs.com/pangblog/p/3397899.html](http://www.cnblogs.com/pangblog/p/3397899.html)
当我们在记录日志时，每个类中会定义一个日志对象，然后利用这个对象去写日志，那么我们在处理日志时，如何能才能记录日志对象所在的类、方法和行号呢？log4j中已经实现了该功能，那么它是怎么实现的呢？
    其实我们可以这样，在要写日志的代码时获得当前的线程信息，这样我们就可以获得上个线程的信息了（即对象写日志所在类的信息）。
    先看以下列子。
新建Location类和Test类：
      Location:
```java
public class Location {
	public void getInfo(){
		String location="";
		StackTraceElement[] stacks = Thread.currentThread().getStackTrace(); 
		location = "类名："+stacks[2].getClassName() + "\n函数名：" + stacks[2].getMethodName()
		+ "\n文件名：" + stacks[2].getFileName() + "\n行号："
		+ stacks[2].getLineNumber() + "";
		System.out.println(location);
	}
}
```
Test:
```java
public class Test {
	public static void main(String[] args) {
		Location l = new Location();
		l.getInfo();
	}
}
```
执行Test中的main函数，得到以下结果：
```java
类名：thread.Test
函数名：main
文件名：Test.java
行号：10
```
是不是输出了Test类中调用的信息呢？那么有很多人就问了，为什么location类中调用的是stacts[2]而不是stacts[0]或其他的呢？
针对这个问题，我们可以将stacts数组里面的东西遍历输出一下就知道了：
```java
StackTraceElement[] stacks = Thread.currentThread().getStackTrace(); 
		for(int i=0;i<stacks.length;i++){
			location = i+"  at "+stacks[i].getClassName() + "." + stacks[i].getMethodName()
			+ "(" + stacks[i].getFileName() + ":"
			+ stacks[i].getLineNumber() + ")";
			System.out.println(location);
		}
```
再次执行，输出结果如下：
```java
0  at java.lang.Thread.getStackTrace(Thread.java:1436)
1  at thread.Location.getInfo(Location.java:6)
2  at thread.Test.main(Test.java:7)
```
那么这就好理解了，线程是以栈形式存放的，
分析一下StackTraceElement[] stacks = Thread.currentThread().getStackTrace();
此代码中会创建2个线程，调用Thread.currentThread().getStackTrace()时底层会创建一个线程，我们调用它也会创建一个线程，然后test调用getInfo函数时会创建一个线程，这样总共就三个线程了，程序执行顺序是先test类中调用getInfo方法，然后getInfo方法中调用StackTraceElement[] stacks = Thread.currentThread().getStackTrace();调用StackTraceElement[]
 stacks = Thread.currentThread().getStackTrace();时底层还会创建一个线程，根据栈的原理先进后出规则，他们的排队顺序就是上面输出的结果了。
