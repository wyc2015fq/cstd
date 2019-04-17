# 责任链模式分析Java示例 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年06月05日 16:38:34[boonya](https://me.csdn.net/boonya)阅读数：420








#### 责任链定义

**百度百科**：责任链模式是一种设计模式。在责任链模式里，很多对象由每一个对象对其下家的引用而连接起来形成一条链。请求在这个链上传递，直到链上的某一个对象决定处理此请求。发出这个请求的客户端并不知道链上的哪一个对象最终处理这个请求，这使得系统可以在不影响客户端的情况下动态地重新组织和分配责任。

**维基百科**：责任链模式在面向对象程式设计里是一种软件设计模式，它包含了一些命令对象和一系列的处理对象。每一个处理对象决定它能处理哪些命令对象，它也知道如何将它不能处理的命令对象传递给该链中的下一个处理对象。该模式还描述了往该处理链的末尾添加新的处理对象的方法。

#### 责任链分析及实现

代码引用的是一位小哥的（[责任链模式妙用](https://www.cnblogs.com/nesger/p/9138536.html)），说的比较清楚明白，为了便于自己理解我进行了一点小小的改动。首先，我们理清自己的责任链，建立一个公共的模型，这个模型抽象出我们每个责任链做事情的方法，每个点的实现不一样就行了，各个点干的事情不一样而已。

##### 抽象一个责任对象（当然可以是接口）

```java
interface IResponsibility {
	// 处理逻辑的方法
	void doSomething(String input, IResponsibility responsibility);
}
```

注：之所以要在做任务的方法上传入接口或者是抽象类，是为了让渡流转业务。


##### 实现实例化责任对象（实现接口）

A责任点

```java
public class AResponsibility implements IResponsibility{
	@Override
	public void doSomething(String input, IResponsibility responsibility) {
		if ("A".equals(input)) {
			// TODO do something
			System.out.println("A does something");
			return;
		}
		//当前没法处理，回调回去，让下一个去处理
		responsibility.doSomething(input, responsibility);
	}
}
```

B责任点

```java
public class BResponsibility implements IResponsibility{
	@Override
	public void doSomething(String input, IResponsibility responsibility) {
		if ("B".equals(input)) {
			// TODO do something
			System.out.println("B does something");
			return;
		}
		//当前没法处理，回调回去，让下一个去处理
		responsibility.doSomething(input, responsibility);
	}
}
```

C责任点

```java
public class CResponsibility implements IResponsibility{
	@Override
	public void doSomething(String input, IResponsibility responsibility) {
		if ("C".equals(input)) {
			// TODO do something
			System.out.println("C does something");
			return;
		}
		//当前没法处理，回调回去，让下一个去处理
		responsibility.doSomething(input, responsibility);
	}
}
```

..........

责任链一直可以自行扩展下去。

#### 编写管理责任的链表

将责任链各个点用链表的形式保存起来，形成一个完整的责任链模型。

```java
public class ResponsibilityChain implements IResponsibility {

	// 完整责任链列表
	private List<IResponsibility> list= new ArrayList<IResponsibility>();

        // 索引
	private int index = 0;

	// 添加责任对象
	public ResponsibilityChain add(IResponsibility responsibility) {
		list.add(responsibility);
		return this;
	}

	@Override
	public void doSomething(String input, IResponsibility responsibility) {
		// 所有遍历完了，直接返回
		if (index == list.size()) return;
		// 获取当前责任对象
		IResponsibility currentResponsibility = list.get(index);
		// 修改索引值，以便下次回调获取下个节点，达到遍历效果
		index++;
		// 调用当前责任对象处理方法
		currentResponsibility .doSomething(input, this);
	}

}
```

#### 测试责任链对象

```java
public class MyTest{

	public static void main(String[] args) {
		String input = "A";
		ResponsibilityChain chain= new ResponsibilityChain ();
		chain.add(new AResponsibility()).add(new BResponsibility()).add(new CResponsibility());
		chain.doSomething(input, chain);
	}

}
```
注：add方法是连写，关键语句是方法内的return this。














