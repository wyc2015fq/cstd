# 【浅尝JAVA】那些JAVA我所不知道的_Oracle模考 - CD's Coding - CSDN博客





2015年04月08日 00:28:49[糖果天王](https://me.csdn.net/okcd00)阅读数：643标签：[甲骨文																[oracle																[java																[class																[类](https://so.csdn.net/so/search/s.do?q=类&t=blog)
个人分类：[Coding Competition](https://blog.csdn.net/okcd00/article/category/2133459)








竞赛考查参赛选手在Java技术和程序设计方面的综合技能，竞赛内容大纲如下：
- 
程序的构建，包括编译、运行、打包和文档生成等。
- 
语言基础，包括语法、变量、数组、流程控制、方法、包、类、枚举、接口等。
- 
面向对象特性，包括继承、封装、多态、抽象、访问控制。
- 
相关运行机制，包括异常、断言、垃圾收集等。
- 
常用工具库，包括线程及并发控制、文件及IO处理、泛型和集合类等。
- 
常用的类和接口，包括Object、字符串、序列化、比较、克隆等。


竞赛题型包含客观题（单选、多选、填空）和编程题，采用自动评判系统客观评分，以保证竞赛的公平性和公正性。对于编程题，竞赛系统会对选手的答案进行即时评判，并将评判结果提示给参赛选手。选手可以在交卷前重复提交编程题的答案。




甲骨文Oracle的JAVA比赛，嘛，我去报了个名参与下玩玩看，今天试了试模考，模拟一把看看感觉。

然后发现……哭……这都是啥啦……和Ｃ＋＋差别好大，好可怕……

暂记录过程中一些记忆较为深刻的部分于此，便于以后的查询以及进一步学习。



```java
package Test_3;

public class ComplexCalc 
{
	public int value;
	public void calc() {value += 5;}
}

public class MoreComplexCalc extends ComplexCalc
{
	public void calc() 
	{
		value -=2;
	}
	public void calc(int multi)
	{
		calc();
		super.calc();
		value *= multi;
	}
	public static void main(String[] args)
	{
		MoreComplexCalc calc = new MoreComplexCalc();
		calc.calc(3);
		System.out.println("Oh it is: "+ calc.value);
	}
}
```
输出为：　

Oh it is: 9


这里的 clac.clac(3)，经历如下过程：

(0-2+5)*3=9






```java
package Test_4;

public class Test 
{
	private int a;
	public int b;
	protected int c;
	int d;
	public static void main(String[] args)
	{
		Test test = new Test();
		int a = test.a++;
		int b = test.b--;
		int c = test.c++;
		int d = test.d--;
		System.out.println(a+"-"+b+"-"+c+"-"+d);
	}
}
```
输出为：

０－０－０－０

因为，改动的是test里的，对当前abcd没有改变






```java
package Test_5;
import java.util.Collection;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.Set;
public class TestSet 
{
	enum Num {ONE,THREE,TWO}
	public static void main(String[] args)
	{
		Collection coll = new LinkedList();
		coll.add(Num.THREE);
		coll.add(Num.ONE);
		coll.add(Num.THREE);
		coll.add(Num.TWO);
		coll.add(Num.TWO);
		Set set = new HashSet(coll);
		System.out.println(set);
		System.out.println(coll);
	}

}
```

输出为：

[TWO, THREE, ONE]

[THREE, ONE, THREE, TWO, TWO]







```java
package Test_6;

public class Test 
{
	public static void main(String[] args)
	{
		String[] elements = {"for","tea","too"};
		String first = (elements.length>0)?elements[0]:null;
		System.out.println(first);
	}
}
```


输出为：

ｆｏｒ






```java
package Test_8;

public class JavaContest 
{
	public static void main(String[] args) throws Exception
	{
		Thread.sleep(3000);
		long n1=12_3_45___789;
		double n2=0b11001_001_0_0_11;
		float n3=0_1_2_3;
		System.out.println(n3);
	}
}
```
输出为：

（等待１秒后）

12345789

3219.0

83.0


_作为连接用，不会对数字造成影响](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)](https://so.csdn.net/so/search/s.do?q=甲骨文&t=blog)




