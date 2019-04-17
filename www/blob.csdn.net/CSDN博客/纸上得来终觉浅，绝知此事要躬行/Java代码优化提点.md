# Java代码优化提点 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年10月30日 11:04:16[boonya](https://me.csdn.net/boonya)阅读数：10903








**优化原则**

代码优化是指对程序代码进行等价（指不改变程序的运行结果）变换。程序代码可以是中间代码，也可以是目标代码。等价的含义是使得变换后的代码运行结果与变换前代码运行结果相同。优化的含义是最终生成的目标代码更加轻松自如（运行时间更短、占用空间更小）地工作，时空效率优化。
代码调优属于性能优化的范畴，制定代码优化的目标是什么才不会偏离优化的本质。如果开始编码的时候就将大多数的时间花在优化代码上面，而忘记了软件开发的初衷（帮助用户解决问题）就得不偿失了。如果优化部分的程序对程序的性能影响不大，那就需要考虑有没有优化的必要的，如果要优化是什么时候优化，这个需要明确下来。优化过的代码或多或少会对系统性能产生一些影响，而且是最直接快速有效的方式之一。
优化是要朝着好的方向发展而不是过度地优化，过度优化会降低程序可读性，会给后期维护人员增加难度系数。编写出优美而性能高效的代码程序是每个程序员都应该致力于钻研寻求的代码解决之道。程序员编写出可读性强、凝练简洁、执行高效的代码有助于提升系统的整体性能，笔者认为能那写出符合以上编码标准的程序是高效程序员的基本素养。

基于API对程序进行优化。例如编写一个Java程序，程序员需要了解Java API 所提供的方法和对象的使用，熟悉对象和方法的应用场景之后做出一个权衡(一般Java API会提供一个接口的不同实现)。程序员需要灵活地使用Java API提供的常用数据类型和工具方法，而要编写出高效的程序就需要去了解相应使用的数据结构或工具方法的工作机制，知道每种数据结构的利弊才能做出合理的选择。编写出符合Java API 应用规范的程序是代码规范的基础，也是代码优化的前提条件。

**认识优化**

以下内容来自百度百科：[http://baike.baidu.com/link?url=D84XufyCzBwe2HTLBclo6HgZX7wAt9wHVT67TqyYtdnEFeZH4X5xpo_9oGK50LeUOEPL--NsuD9LtEs9fp4H8q](http://baike.baidu.com/link?url=D84XufyCzBwe2HTLBclo6HgZX7wAt9wHVT67TqyYtdnEFeZH4X5xpo_9oGK50LeUOEPL--NsuD9LtEs9fp4H8q)

**1、明确优化目标**

       在优化工作开始的时候，你还尚未明确优化内容和目的，那么你很容易陷入误区。在一开始，你就应该清楚地了解你要达到的效果，以及其他优化相关的各种问题。这些目标需要明确指出（至少精通技术的项目经理可以理解和表达它），接下来，在整个优化过程中，你需要坚持这些目标。

　  在实际的项目开发中，经常会存在各种各样的变数。可能一开始时要优化这一方面，随后你可能会发现需要优化另一方面。这种情况下，你需要清晰地了解这些变化，并确保团队中的每个人都明白目标已经发生了变化。

**2、选择正确的优化指标**

      选择正确的指标，是优化的一个重要组成部分，你需要按照这些指标来测量优化工作的进展情况。如果指标选择不恰当，或者完全错误，你所做的努力有可能白费了。即使指标正确，也必须有一些辨别。在某些情况下，将最多的努力投入到运行消耗时间最多的那部分代码中，这是实用的策略。但也要记住，Unix/Linux内核的大部分时间花费在了空循环上。需要注意的是，如果你轻易选择了一个很容易达到的指标，这作用不大，因为没有真正解决问题。你有必要选择一个更复杂的、更接近你的目标的指标。

**3、找到优化的关键点**

      这是有效优化的关键。找到项目中与你的目标（性能、资源或其他）相背的地方，并将你的努力和时间用在那里。举一个典型的例子，一个Web项目速度比较慢，开发者在优化时将大部分精力放在了数据库优化上，最终发现真正的问题是网络连接慢。另外，不要分心于容易实现的问题。这些问题尽管很容易解决，但可能不是必要的，或与你的目标不相符。容易优化并不意味着值得你花费工夫。

**4、优化层次越高越好**

　  在一般情况下，优化的层次越高，就会越有效。根据这个标准，最好的优化是找到一个更有效的算法。举个例子，在一个软件开发项目中，有一个重要的应用程序性能较差，于是开发团队开始着手优化，但性能并没有提升太多，之后，项目人员交替，新的开发人员在检查代码时发现，性能问题的核心是由于在表中使用了冒泡排序算法，导致成千上万项的增加。

　　尽管如此，高层次的优化也不是“银弹”。一些基本技术，如将所有东西移到循环语句外，也可以产生一些优化的效果。通常情况下，大量低层次的优化可以产生等同于一个高层次优化的效果。还需要注意的是，高层次优化，会减少一些代码块，那么你之前对这些代码块所做的优化就没有任何意义了，因此，刚开始就应该考虑高层次的优化。

**5、不要过早优化**

　　在项目早期就进行优化，会导致你的代码难以阅读，或者会影响运行。另一方面，在项目后期，你可能会发现之前所做的优化没有起到任何作用，白白浪费了时间和精力。正确的方式是，你应该将项目开发和优化当作两个独立的步骤来做。


**6、性能分析指导优化**

　　　你往往会认为你已经知道哪里需要优化，这是不可取的，尤其是在复杂的软件系统中，性能分析数据应该是第一位的，最后才是直觉。优化的一个有效的策略是，你要根据所做工作对优化效果的影响来进行排序。在开始工作之前找到影响最大的“路障”，然后再处理小的“路障”。


**7、优化并非万能**

　　　优化最重要的规则之一是，你无法优化一切，甚至无法同时优化两个问题。比如，优化了速度，可能会增加资源利用；优化了存储的利用率，可能会使其他地方放慢。你需要权衡一下，哪个更符合你的优化目标。


**优化实践**

**1、try{}catch(){}使用**

   一般特别重要的数据操作都需要捕获异常，以便程序员及时知道哪里产生了错误，但频繁地try{}catch(){}会降低系统的性能。
【情景一】查询中不要使用try{}catch(){}语句

   一般查询中不需要使用try{}catch(){}语句的最好将try{}catch(){}去掉。在CRUD(create、read、update、delete)操作中，读取数据（R{read}）是一种很常见的操作方式，不会对数据本身构成任何影响，所以没必要使用try{}catch(){}语句。单个的try{}catch(){}语句使用虽然对程序虽然不会造成什么影响但是最好不要使用，没有什么实际意义。

   假设一个查询的时间是3s，程序模拟实现：

Java测试代码


```java
public void tryCatch()
	{
		long before = System.currentTimeMillis();
		try
		{
			new Thread().sleep(3000);
		} catch (Exception e)
		{
			e.printStackTrace();
		}
		long after = System.currentTimeMillis();
		System.out.println("单个try catch 耗时:" + (after - before) + "ms");
	}
```

打印结果：

单个try catch 耗时:3000ms



【情景二】循环中不要使用try{}catch(){}语句

   try{}catch(){}语句是一个比较耗时的处理过程，在for循环中循环体的长度越长需要try{}catch(){}的次数也就越多。  

Java测试代码


```java
public void tryCatchOperateInLoop(long times)
	{
		long before = System.currentTimeMillis();
		for (int i = 0; i < times; i++)
		{
			try
			{
				new Thread();
			} catch (Exception e)
			{
				e.printStackTrace();
			}
		}
		long after = System.currentTimeMillis();
		System.out.println("执行try catch 循环耗时:" + (after - before) + "ms");
	}

	public void noTryCatchOperateInLoop(long times)
	{
		long before = System.currentTimeMillis();
		for (int i = 0; i < times; i++)
		{
			new Thread();
		}
		long after = System.currentTimeMillis();
		System.out.println("非try catch 循环耗时:" + (after - before) + "ms");
	}
```

测试结果：

循环10000次，执行try catch 循环耗时:32ms ；非try catch 循环耗时:15ms。



【情景三】try{}catch(){}异常处理

　　try{}catch(){}语句是为了捕获可能会发生的异常而使用的，不要对于所有的异常捕获都使用Exception，最好是捕获对应的异常类型。异常捕获了就需要做相应的处理，而大多数人并未处理，更多的是打印一句e.printStackTrace(); 就了事了，这样做是不好的。

　　异常捕获的作用就是在异常发生后做一些补救措施，使程序得以继续运行下去。而简单的一句e.printStackTrace(); 不算是对异常进行处理，反而更多带来的是程序无法再执行下去的恶果。

处理日志方式：

　　1、打印错误日志

　　2、调用错误业务逻辑处理函数

　　3、跳转错误提示页面
【情景四】try{}catch(){}的finally中销毁对象或释放资源

　　finally块代码无论try{}catch(){}是否发生异常都会被访问，所以在这里可以做一些资源的释放、对象的销毁工作，减少不必要的资源占用。

　　Java代码示例：


```java
try
		{
			os.write(byteBuffer);
		} catch (Exception e)
		{
			e.printStackTrace();
		} finally
		{
			os.close();// 释放资源
			os = null;
			destfile = null;
		}
```

以上代码是文件写入的一部分。
【情景五】对外提供的接口有异常则抛出

     在接口调用中，如果实现中可能会有异常产生需要将异常处理或者抛出异常，这样可以方便开发人员调试定位问题；如果接口实现者隐而不报，那么定位问题将是件麻烦的事情。

Java代码：





```java
package com.boonya.test;
public interface IObjectService {

    public void printFiles() throws Exception;

}
```



**2、减少使用new关键字创建对象**


       在Java程序中，对象的创建和销毁是一个重量级的操作，会增加系统性能开销降低系统性能，而且并不能保证对象在不使用的时候就马上执行GC操作了。此外，初始化一个对象会默认盗用其无参构造函数，大多数情况下我们并不想初始化对象时就调用其构造方法。


【情景一】避免在循环中使用new关键字创建对象

  下面以一个例子来加以说明：

Java测试代码


```java
public void noTryCatchOperateInLoop(long times)
	{
		long before = System.currentTimeMillis();
		for (int i = 0; i < times; i++)
		{
			new Object();
		}
		long after = System.currentTimeMillis();
		System.out.println("循环耗时:" + (after - before) + "ms");
	}
```

测试结果：

以10000次为测试标准，结果是：循环耗时:16ms；如果将new Object();注释掉，打印的结果是：循环耗时:0ms；由此看出对象的创建是一笔不小的开销。
【情景二】有必要创建对象时才进行创建

   在某些情况下我们根本不需要创建多余的变量或者对象，但是却时常忍不住会或无意中这么做了。

Java测试代码

```java
public String judgeStatus(int flag)
	{
		String s = new String("成功");
		if (flag == 0)
		{
			s = "失败";
		}
		return s;
	}
```

其实此处的s是不需要创建的，因为开发者习惯使用数据类型来返回对象或者值。上述代码完全可以这样写：

```java
public String judgeStatus(int flag)
	{
		if (flag == 0)
		{
			return "失败";
		}
		return "成功";
	}
```

另外在对象作用范围声明对象：

```java
public Object test(int flag)
	{
		if (flag == 1)
		{
			return new Object();
		}
		return null;
	}
```



**3、乘除法是否可以使用移位运算**

【情景一】乘数或除数是否是2的平方根

Java测试代码


```java
// 左移运算 times是2的幂
public long numberLeftCalculate(long number, int times)
{
	return number << times;
}

// 右移运算 times是2的幂
public long numberRightCalculate(long number, int times)
{
	return number >> times;
}
```

Java中许多算法使用的乘除计算基本上都会用到移位运算，这样大大缩短了计算时间，提高了算法的运算效率。
【情景二】乘除运算在循环中反复出现

     众所周知，移位运算是操作二进制数据最快速的方法，而计算机能识别的就是二进制数据0和1，其计算只是将二进制数后面向左或右添加或减去若干个0而不需要进行计算，所以读取运算的效率非常高。而十进制数据运算，计算机会将十进制数（至少会转化两次）转化成可以识别的二进制数，然后再进行二进制数之间的乘法或除法计算。

Java测试代码





```java
public void perplexingLoop(int j)
	{
		int s = 0;
		for (int i = 0; i <= j; i++)
		{
			int a = i * 8;
			int b = a / 4;
			// 复杂的a、b乘除关系计算
			// .................
			s += a + b;
		}
		System.out.println("s1=" + s);
	}

	public void perplexingLoop2(int j)
	{
		int s = 0;
		for (int i = 0; i <= j; i++)
		{
			int a = i << 3;
			int b = a >> 2;
			// 复杂的a、b乘除关系计算
			// .................

			s += a + b;
		}
		System.out.println("s2=" + s);
	}
```



分别执行1000000次循环

测试结果：s1=663067456，s2=663067456，这里可能看不出什么差别，但是for循环中的乘除运算一多那就能体现出来了。

**4、注意循环的写法**

【情景一】减少循环次数变量的读取

事先要确定一个循环的次数，以后就不再需要计算它的次数了

Java测试代码


```java
for (int i = 0; i < list.size(); i++)
{
	//每次都会去判定i是否到list.size()的边界
}
```

不要每次都去进行边界大小计算，最好写成如下方式：

```java
for (int i = 0,j=list.size(); i < j; i++)
{
       //j是局部变量只初始化一次
}
```
【情景二】循环中嵌套循环将小循环写在外层

   大循环嵌套小循环，外层循环次数太多造成循环之间不断的切换，也会增加系统开销。

Java测试代码

```java
public void complexLoop()
	{
		long before = System.currentTimeMillis();
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 1000000; j++)
			{
				new String();
			}
		}
		long after = System.currentTimeMillis();
		System.out.println("小嵌大循环耗时:" + (after - before) + "ms");

		long before2 = System.currentTimeMillis();
		for (int i = 0; i < 1000000; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				new String();
			}
		}
		long after2 = System.currentTimeMillis();
		System.out.println("大嵌小循环耗时:" + (after2 - before2) + "ms");
	}
```

测试结果：小嵌大循环耗时:266ms，大嵌小循环耗时:312ms



【情景三】不要在循环中频繁声明对象

  前面[2、【情景二】]已经说到，不要随意地创建对象，这个会降低程序执行的效率，在循环中也是如此。


```java
// 方式一
	public List<Map<String, Object>> createMapList()
	{
		List<Map<String, Object>> maplist = new ArrayList<Map<String, Object>>();
		for (int i = 0, j = 1000000; i < j; i++)
		{
			Map<String, Object> map = new HashMap<String, Object>();// 每循环一次划出一块堆空间来存储该对象，并在栈中建立该对象的一个引用
			map.put("ms" + 1, "hello_" + i);
			maplist.add(map);
		}
		return maplist;
	}

	// 方式二
	public List<Map<String, Object>> createMapList2()
	{
		List<Map<String, Object>> maplist = new ArrayList<Map<String, Object>>();
		Map<String, Object> map;
		for (int i = 0, j = 1000000; i < j; i++)
		{
			map = new HashMap<String, Object>();// 不再每次都做声明了
			map.put("ms" + 1, "hello_" + i);
			maplist.add(map);
		}
		return maplist;
	}
```

测试for长度100万次：

方法一耗时：4141ms

方法一耗时：4078ms

结论：方式二比方式一更加高效。



【情景四】循环中不要调用同步方法

只要跟同步挂上钩都是耗时的操作，如果同步方法执行的时间复杂度是1，那么在循环中调用同步方法的时间复杂度就是n，所以建议不要这么做。

Java代码：


```java
private static boolean isUsingLock = false;

	public synchronized void operate()
	{
		isUsingLock = true;
		// do somthing
		isUsingLock = false;
	}

	public void dowork(int n)
	{
		for (int i = 0; i < n; i++)
		{
			this.operate();
		}
	}
```

这样做效率肯定低下，除非是该方法需要同步。
【情景五】尽量避免循环中执行查询操作

     查询也是一个导致系统性能降低的原因，尤其是频繁地访问数据库操作更是如此。类似上面的情况，如果同步方法执行的时间复杂度是1，那么在循环中调用同步方法的时间复杂度就是n，跟循环中套用循环效果是基本一致的。

Java代码：

```java
public void queryData(int n)
	{
		for (int i = 0; i < n; i++)
		{
			// database query
		}
	}
```



除非是跨表查询一般不使用此方式。


**5、实现字符串拼接**


【情景一】String

     不可变字符数据类的拼接，每次使用“+=”时都相当于引入了一个中间的临时变量，所以这种方式最值得诟病。

Java代码


```java
public void exampleString(int arg1, int arg2, int arg3)
	{
		String sb = new String("参数名称: ");
		if (arg1 == 0)
		{
			sb += "  arg1xxxxx0";
		} else
		{
			sb += "  arg1xxxxx-1";
		}
		if (arg2 == 0)
		{
			sb += "  arg2xxxxx0";
		} else
		{
			sb += "  arg2xxxxx-1";
		}
		if (arg3 == 0)
		{
			sb += "  arg3xxxxx0";
		} else
		{
			sb += "  arg3xxxxx-1";
		}
		System.out.println(sb);
	}
```

多一次添加就会多声明一个临时的隐藏变量。
【情景二】StringBuilder

  可变字符串对象，可以确保当前线程只有一个StringBuffer对象在实现字符串长度的修改变化，但此对象却不是线程安全的。


```java
public void exampleStringBuilder(int arg1, int arg2, int arg3)
	{
		StringBuilder sb = new StringBuilder("参数名称: ");
		if (arg1 == 0)
		{
			sb.append("  arg1xxxxx0");
		} else
		{
			sb.append("  arg1xxxxx-1");
		}
		if (arg2 == 0)
		{
			sb.append("  arg2xxxxx0");
		} else
		{
			sb.append("  arg2xxxxx-1");
		}
		if (arg3 == 0)
		{
			sb.append("  arg3xxxxx0");
		} else
		{
			sb.append("  arg3xxxxx-1");
		}
		System.out.println(sb.toString());
	}
```

在效率上它比StringBuffer的效率更高，因为不需要做同步。



【情景三】StringBuffer

     可变字符串对象，此对象是线程安全的，可以确保当前线程只有一个StringBuffer对象在实现字符串长度的修改变化。

Java代码


```java
public void exampleStringBuffer(int arg1, int arg2, int arg3)
	{
		StringBuffer sb = new StringBuffer("参数名称: ");
		if (arg1 == 0)
		{
			sb.append("  arg1xxxxx0");
		} else
		{
			sb.append("  arg1xxxxx-1");
		}
		if (arg2 == 0)
		{
			sb.append("  arg2xxxxx0");
		} else
		{
			sb.append("  arg2xxxxx-1");
		}
		if (arg3 == 0)
		{
			sb.append("  arg3xxxxx0");
		} else
		{
			sb.append("  arg3xxxxx-1");
		}
		System.out.println(sb.toString());
	}
```

在数据库SQL语句拼接时经常会用到，所以一般字符串拼接都使用StringBuffer为妙 。
【情景四】单个字符相加用’’代替””

     前面已经讲述：String字符串相加不是个明智之选，这里谈单个字符的拼接；由于’’作用的是char类型的数据所以比String要适合；显然，StringBuffer在这里使用就是一种浪费了。

Java代码

```java
// 例子：
	public void method(String s)
	{
		String string = s + "d";
		string = "abc" + "d";
		System.out.println(string);
	}

	// 更正： 将一个字符的字符串替换成' '
	public void method2(String s)
	{
		String string = s + 'd';
		string = "abc" + 'd';
		System.out.println(string);
	}
```



**6、提升HashMap遍历效率**

     Hashmap实现了Map接口，其底层是用Entry对象来操作key和value的，Map的遍历实就是利用Hash值取出相应的Entry做比较得到结果。

Java测试代码




```java
public void foreachHashMap()
	{
		Map<String, String[]> paraMap = new HashMap<String, String[]>();
		for (int i = 0; i < 1000000; i++)
		{
			paraMap.put("test" + i, new String[]
			{ "s" + i });
		}
		String[] values;
		// 第一个循环
		long before = System.currentTimeMillis();
		Set<String> appFieldDefIds = paraMap.keySet();
		for (String appFieldDefId : appFieldDefIds)
		{
			values = paraMap.get(appFieldDefId);
			// System.out.println(values[0]);
		}
		long after = System.currentTimeMillis();
		System.out.println("HashMap遍历:" + (after - before) + "ms");

		// 第二个循环
		long before2 = System.currentTimeMillis();
		for (Entry<String, String[]> entry : paraMap.entrySet())
		{
			values = entry.getValue();
			// System.out.println(values[0]);
		}
		long after2 = System.currentTimeMillis();
		System.out.println("HashMap遍历:" + (after2 - before2) + "ms");
	}
```



测试结果：HashMap遍历:93ms，HashMap遍历:63ms

当循环次数是10000次时，打印：

HashMap遍历:15ms

HashMap遍历:0ms


就方法本身而言，使用第一个循环相当于两次进入HashMap的Entry中

而第二个循环取得Entry的值之后直接取value，效率比第一个循环高。


**7、  尽量使用局部变量**

       尽量使用局部变量，调用方法时传递的参数以及在调用中创建的临时变量都保存在栈（Stack）中，速度较快。其他变量，如静态变量、实例变量等，都在堆（Heap）中创建，速度较慢。

Java测试代码



```java
public static int increment_no;

	public void forLoop()
	{
		long before = System.currentTimeMillis();
		for (int i = 0; i < 10000000; i++)
		{
			increment_no += i;
		}
		long after = System.currentTimeMillis();
		System.out.println("变量测试:" + increment_no + " " + (after - before) + "ms");
	}

	public void forLoop2()
	{
		int no = 0;
		long before = System.currentTimeMillis();
		for (int i = 0; i < 10000000; i++)
		{
			no += i;
		}
		long after = System.currentTimeMillis();
		System.out.println("变量测试:" + no + " " + (after - before) + "ms");
	}
```



测试结果：

变量测试:-2014260032 31ms

变量测试:-2014260032 0ms


**8、尽量实现对象重用**

   系统要花时间生成对象，以后还需花时间对这些对象进行垃圾回收和处理。因此，生成过多的对象将会给程序的性能带来很大的影响。


【情景一】字符串对象重用

  字符串对象的重用，就是减少字符串变量的声明，而有些声明是不可见的，需要注意。

Java代码


```java
// String 是不可变字符，str "+="相当于 String temp=array[0];str=str+temp;
	public String getString(String array[])
	{
		String str = "";
		for (int i = 0; i < array.length; i++)
		{
			str += array[i];
		}
		return str;
	}

	// StringBuffer是可变字符，字符串拼接在一个对象上操作
	public String getString2(String array[])
	{
		StringBuffer sb = new StringBuffer();
		for (int i = 0; i < array.length; i++)
		{
			sb.append(array[i]);
		}
		return sb.toString();
	}
```

这里的String类的”+=”操作就是一种中间变量的声明方式实现的，而StringBuffer则相当于一个容器，字符串添加就是在不断地向容器中添加而不需要其他容器来辅助实现。有此可见，字符串对象重用也可以使用StringBuffer 来实现。



【情景二】声明对象的重用

   对象的创建和销毁(垃圾收集机制GC)都是会以消耗系统性能为代价的.

Java代码


```java
public List<Object> getObjectList()
	{
		List<Object> list = new ArrayList<Object>();
		for (int i = 0; i < 100; i++)
		{
			Object obj = new Object();
			list.add(obj);
		}
		return list;
	}

	public List<Object> getObjectList2()
	{
		List<Object> list = new ArrayList<Object>();
		Object obj;
		for (int i = 0; i < 100; i++)
		{
			obj = new Object();
			list.add(obj);
		}
		return list;
	}
```

以上代码中getObjectList()的Object的创建就是一种不明智的做法。



【情景三】减少对象初始化

     默认情况下，调用类的构造函数时， Java会把变量初始化成确定的值：所有的字符串变量被设置成null，整数变量（byte、short、int、long）设置成0，float和double变量设置成0.0，逻辑值设置成false。

     这里以单例模式为例，减少对象的频繁创建带来的系统性能消耗。

Java代码


```java
public class PlusSingleton
{
	/**
	 * 确保外部不能实例化，即使用new关键字无效
	 */
	private PlusSingleton()
	{
		System.out.println("singleton constructor");
	}

	/**
	 * 内部类JVM初始换调用
	 */
	private static class SingletonObject
	{
		private static PlusSingleton singleton = new PlusSingleton();
	}

	/**
	 * 不需要考虑多线程问题
	 */
	public static PlusSingleton getInstance()
	{
		return SingletonObject.singleton;
	}

}
```

如果不是单例模式，而是使用继承的子类频繁初始化，那么他会频繁地调用父类的构造函数，这种方式也是不推荐的。



【情景四】合理使用继承和抽象

    如果一个类是另一个类的扩展或延伸，那么可以考虑是否可以用继承，另外还需要权衡使用继承后这个对象是否受制于父类，是否降低了灵活性。继承只是继承父类的特性而不能了解父类所有的东西，比如私有的函数、属性等。


Java代码：




```java
public class Father
{
	private String name;
	private int age;

	public String getName()
	{
		return name;
	}

	public void setName(String name)
	{
		this.name = name;
	}

	public int getAge()
	{
		return age;
	}

	public void setAge(int age)
	{
		this.age = age;
	}

	@SuppressWarnings("unused")
	private void playCard()
	{
		System.out.println("打牌");
	}
}

public class Son extends Father
{
	public void work()
	{
		System.out.println("工作");
	}

}
```



       儿子和父亲虽然都有名字和年龄，但儿子并不知道父亲会打牌、父亲也不知道儿子能工作。如果轻率地改变父亲的私有方法那么儿子就会学会打牌了，儿子如果以打牌为乐、是不是会忘了工作呢?所以，继承也并非什么都好，子类会被父类影响那么最好慎用继承。显然，儿子可以拒绝在工作的时候玩牌，但是就需要增加工作时不能玩牌的逻辑处理了。这时候就可以考虑使用抽象类来实现了，不在需要使用继承受父类的影响了。


Java代码：


```java
public abstract class AbstractClass
{
	private String name;
	private int age;

	public String getName()
	{
		return name;
	}

	public void setName(String name)
	{
		this.name = name;
	}

	public int getAge()
	{
		return age;
	}

	public void setAge(int age)
	{
		this.age = age;
	}

	public abstract void playCard();

}
```



现在Father、Son继承这个抽象类实现自己的玩牌规则就行了：


```java
public class Father extends AbstractClass
{
	public void playCard()
	{
		System.out.println("打牌,没完没了的打");
	}
}

public class Son extends AbstractClass
{
	public void work()
	{
		System.out.println("工作");
	}

	public void playCard()
	{
		System.out.println("玩牌规则：工作时间不能玩牌，只能工作");

	}
}
```

以前儿子见不惯父亲只会打牌，儿子以此为耻给自己定下了打牌的规矩，从这里看出Father和Son已经断绝父子关系了。



**9、将闲置的资源进行释放**

      Java 编程过程中，进行数据库连接、I/O流操作时务必小心，在使用完毕后，即使关闭以释放资源。因为对这些大对象的操作会造成系统大的开销，稍有不慎，会导致严重的后果。


【情景一】记得断开数据库访问链接

     数据库操作是一个重量级的操作，虽然可以建立数据库连接池，如果建立的连接使用后没有被释放，那么数据库连接池中将很快没有空闲的连接提供使用了，所以及时回收数据库资源是一种避免线程池一直处于无可用连接的最佳方式。系统资源占用过多会导致程序很慢，造成假死状况的发生。为此，还是需要有一个严谨的编码习惯来避免这些问题的发生。

Java代码：




```java
/**
	 * 释放资源占用
	 * 
	 * @param con
	 *            数据库链接
	 * @param stmt
	 *            数据库声明操作
	 * @param res
	 *            数据集
	 */
	public static void setItFree(Connection con, Statement stmt, ResultSet res)
	{
		try
		{
			if (res != null)
			{
				res.close();
			}
			res = null;
		} catch (Exception e)
		{
			e.printStackTrace();
		}

		try
		{
			if (stmt != null)
			{
				stmt.close();
			}
			stmt = null;
		} catch (Exception e)
		{
			e.printStackTrace();
		}

		try
		{
			if (con != null)
			{
				con.close();
			}
			con = null;
		} catch (Exception e)
		{
			e.printStackTrace();
		}
	}
```



 以上是JDBC数据库访问资源的释放，在Hibernate中没必要这么做了，因为Hibernate已经帮我们做了这些工作了。


【情景二】记得关闭I/O输入输出流

    文件输入输出操作，需要关闭输入、输出的流，如果不关闭则会占用CPU资源降低系统性能。

Java代码：




```java
public void fileOperate() throws IOException
	{
		// 获取输入流
		File srcfile = new File("D:\\myfiles\\my.txt");
		InputStream is = new FileInputStream(srcfile);
		// 获取输出流
		File destfile = new File("D:\\myfiles\\test.txt");
		OutputStream os = new FileOutputStream(destfile);

		byte[] byteBuffer = null;
		try
		{
			byteBuffer = new byte[1024];
			while (is.read(byteBuffer) != -1)
			{
				System.out.println(byteBuffer.length);
			}
		} catch (Exception e)
		{
			e.printStackTrace();
		} finally
		{
			is.close();// 释放资源
			is = null;
			srcfile = null;
		}

		try
		{
			os.write(byteBuffer);
		} catch (Exception e)
		{
			e.printStackTrace();
		} finally
		{
			os.close();// 释放资源
			os = null;
			destfile = null;
		}
	}
```

建议文件输出完成后就将资源一并释放。



【情景三】对象使用完成后进行置空

     由于JVM的有其自身的GC机制，不需要程序开发者的过多考虑，从一定程度上减轻了开发者负担，但同时也遗漏了隐患，过分的创建对象会消耗系统的大量内存，严重时会导致内存泄露，因此，保证过期对象的及时回收具有重要意义。JVM回收垃圾的条件是：对象不在被引用；然而，JVM的GC并非十分的机智，即使对象满足了垃圾回收的条件也不一定会被立即回收。所以，建议我们在对象使用完毕，应手动置成null。

Java代码：




```java
public Map<String, Object> getObjects()
	{
		Map<String, Object> map = new HashMap<String, Object>();
		List<Object> list = new ArrayList<Object>();
		Object obj;
		for (int i = 0; i < 100; i++)
		{
			obj = new Object();
			list.add(obj);
		}
		map.put("objects", list);
		list = null;
		return map;
	}
```

这里清理了list占用的那块内存。



**10、ArrayList、Vector、LinkedList的使用**

  List接口一共有三个实现类，分别是ArrayList、Vector和LinkedList。List用于存放多个元素，能够维护元素的次序，并且允许元素的重复。


【情景一】ArrayList实现随机查找和遍历

　　ArrayList是最常用的List实现类，内部是通过数组实现的，它允许对元素进行快速随机访问。数组的缺点是每个元素之间不能有间隔，当数组大小不满足时需要增加存储能力，就要讲已经有数组的数据复制到新的存储空间中。当从ArrayList的中间位置插入或者删除元素时，需要对数组进行复制、移动、代价比较高。因此，它适合随机查找和遍历，不适合插入和删除。

Java代码：


```java
public void myArrayList(ArrayList<String> list)
	{
		for (int i = 0, j = list.size(); i < j; i++)
		{
			System.out.println(list.get(i));
		}
	}
```
【情景二】Vector实现数组同步操作

　　Vector与ArrayList一样，也是通过数组实现的，不同的是它支持线程的同步，即某一时刻只有一个线程能够写Vector，避免多线程同时写而引起的不一致性，但实现同步需要很高的花费，因此，访问它比访问ArrayList慢。

Java代码：

```java
public void myVectorList(Vector<String> list, String s)
	{
		for (int i = 0, j = list.size(); i < j; i++)
		{
			if (list.get(i).equals(s))
			{
				// 修改值
				list.add(i, i + "update");
			}
		}
	}
```



【情景三】LinkedList数据的动态插入和删除

　　LinkedList是用链表结构存储数据的，很适合数据的动态插入和删除，随机访问和遍历速度比较慢。另外，他还提供了List接口中没有定义的方法，专门用于操作表头和表尾元素，可以当作堆栈、队列和双向队列使用。

Java代码：


```java
public void myLinkedList(LinkedList<String> list, String s)
	{
		for (int i = 0, j = list.size() - 10; i < j; i++)
		{
			list.pop();
		}

		if (list.size() == 0)
		{
			list.add(0, "ss");
		} else
		{
			list.removeAll(list);
		}

	}
```



**11、synchronized的使用**

【情景一】方法的同步

    方法同步：一旦有一个线程在访问该方法，方法将陷入等待或阻塞，只有用户线程方法解锁后才能被其他线程调用。

Java代码：


```java
private static boolean isUsingLock = false;

public synchronized void operate()
{
	isUsingLock = true;
	// do somthing
	isUsingLock = false;
}
```

在操作串行的流程处理时需要。
【情景二】代码块同步

    此同步是为了保证一个线程中特殊的对象操作而存在的，比如多个用户有权限操作同一公共资源的时候就需要使用该方式实现资源的同步操作。

```java
private static String name = "";

public void operate2()
{
	synchronized (name)
	{
		if (name.equals(""))
		{
			name = "boonya";
		}
	}
}
```



**12、三元表达式代替if else**

简化逻辑判断，是代码更加清晰、一目了然。


```java
public String express(boolean flag)
{
    return flag ? "成功" : "失败";
}
```



**13、StringTokenizer代替substring()和indexOf()**

    StringTokenizer是java中object类的一个子类，继承自 Enumeration（ public interface Enumeration<E>实现 Enumeration（列举） 接口的对象，它生成一系列元素，一次生成一个。）接口。

　　StringTokenizer 可以理解为String的字符标记对象，此对象可以实现字符串的截取操作，避免了字符串容易导致StringIndexOutOfBoundsException的问题。其构造器有下面三种形式： 

·public StringTokenizer(String str,String delim,boolean returnTokens); 

·public StringTokenizer(String str,String delim); 

·public StringTokenizer(String str); 其中，str为需分析的字符串，delim为定界符，Tokens描述是否将定界符作为一个token(令牌)。 


Java代码示例：


```java
public void stringTokenizerSubstring()
{
	String str = "boonya@sina.com";
	String s1 = new StringTokenizer(str).nextToken("@");
	System.out.println(s1);
}

public void stringTokenizerIndexof()
{
	String str = "boonya@sina.com";
	String s1 = new StringTokenizer(str, "@", true).nextToken();
	System.out.println(s1);
}
```

结果打印的都是:boonya



**14、System.arraycopy()实现数组值的拷贝**

两个数组间的复制建议不要使用循环，Java提供了一个数组拷贝的方法。

Java代码：


```java
public void arrayCopy()
{
	int[] array1 = new int[10];
	for (int i = 0; i < array1.length; i++)
	{
		array1[i] = i;
	}
	int[] array2 = new int[10];
	System.arraycopy(array1, 0, array2, 0, 10);

	for (int i = 0; i < array2.length; i++)
	{
		System.out.print(array2[i] + ",");
	}
}
```

打印结果：0,1,2,3,4,5,6,7,8,9,

注意：虽然List内部是基于数组的实现，但是这里取不能使用System.arraycopy(list1, 0, list2, 0, n);虽然代码编写时通过了编译，但是会有运行时异常抛出。



**15、instanceof判断接口而不是接口的实现类**

基于接口的设计通常是件好事，因为它允许有不同的实现，而又保持灵活。只要可能，对一个对象进行instanceof操作，以判断它是否某一接口要比是否某一个类要快。

Java代码：


```java
public interface IObject
{

}

public class ObjectImpl implements IObject
{

}

public void instanceOf(Object obj)
{
	// better
	if (obj instanceof IObject)
	{

	}
	// worse
	if (obj instanceof ObjectImpl)
	{

	}
}
```

所以，我们判断接口而不是普通类或接口的实现类。



**16、将类中不变的实例静态化**

如果类中的变量不会随他的实例而变化，就可以定义为静态变量，从而使他所有的实例都共享这个变量。

Java代码：


```java
public class AlarmDao extends BaseDao<Alarm>
{
     private static SortUtil<Alarm> sortUtil = new SortUtil<Alarm>();
}
```



**17、减少不必要的数据库查询操作**

 如果在一次查询中有两个或两个以上的相同查询操作，可以考虑将第一次查询的结果缓存起来，在此次查询中以后的查询读取缓存对象就行了。

请看下面的草图，优化之前：


![](https://img-blog.csdn.net/20131030110821875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


优化后：

![](https://img-blog.csdn.net/20131030110900109?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


缓存解决方案有很多比如：Cache、Cache4j、Memcached、以及Hibernate提供的Hcache等等.


**18、尽量指定类的final修饰符**

       带有final修饰符的类是不可派生的。在Java核心API中，有许多应用final的例子，例如java.lang.String。为String类指定final防止了人们覆盖length()方法。另外，如果指定一个类为final，则该类所有的方法都是final。Java编译器会寻找机会内联（inline）[内联函数就是指函数在被调用的地方直接展开，编译器在调用时不用像一般函数那样，参数压栈，返回时参数出栈以及资源释放等，这样提高了程序执行速度。 ]所有的final方法（这和具体的编译器实现有关）。此举能够使性能平均提高50%。

Java代码：

//表示此行为无可更改

public final void  doWork(){
//doSomething

}

   一般实体对象的属性字段的getter/setter方法变成”final”的都是可行的，因为基本上没人去修改这些方法。


**19、仅查找单个字符用charAt()代替startsWith()**

用一个字符作为参数调用startsWith()也会工作的很好，但从性能角度上来看，调用用String API无疑是不合时宜的!

Java代码：


```java
public void findCharExists()
{
	String str = "boonya@sina.com";
	// startsWith
	if (str.startsWith("b"))
	{
		System.out.println("存在b");
	}

	// charAt
	if ('b' == str.charAt(0))
	{
		System.out.println("存在b");
	}

	// charAt
	if ('@' == str.charAt(6))
	{
		System.out.println("存在@");
	}
}
```



**20、transient修饰字段规避序列化持久化操作**

    字段描述中添加transient说明一个属性是临时的，不会被序列化,也不会被持久化。Java的serialization提供了一种持久化对象实例的机制。当持久化对象时，可能有一个特殊的对象数据成员，我们不想用serialization机制来保存它。为了在一个特定对象的一个域上关闭serialization，可以在这个域前加上关键字transient。transient是Java语言的关键字，用来表示一个域不是该对象串行化的一部分。当一个对象被串行化的时候，transient型变量的值不包括在串行化的表示中，然而非transient型的变量是被包括进去的。 


Java代码：


```java
import java.io.Serializable;

public class Person implements Serializable
{
	private static final long serialVersionUID = -36370127660133224L;
	// 姓名
	private String name;
	// 邮箱
	private String email;
	// 身份证(临时字段，此信息来自独家档案,不需要串行操作)
	private transient String idCard;

	public String getName()
	{
		return name;
	}

	public void setName(String name)
	{
		this.name = name;
	}

	public String getEmail()
	{
		return email;
	}

	public void setEmail(String email)
	{
		this.email = email;
	}

	public String getIdCard()
	{
		return idCard;
	}

	public void setIdCard(String idCard)
	{
		this.idCard = idCard;
	}
}
```

在Hibernate中，也是支持此特性的。Hibernate annotation 的实体类中如果存在数据库中没有的字段,需要在这个字段中加上@Transient注释，不会被映射，也就不会被持久化。

Java代码：

```java
@Transient
private String factoryDeviceId;// <非数据库字段>设备编号
```



**21、清理程序中多余的代码**

【情景一】删除不必要的测试代码

     删除代码中的测试方法，在项目部署后对于用户该代码就不再使用了，所以没有必要保留。

Java代码：


```java
public static void main(String[] args) throws IOException
{
   ObjectReuse or = new ObjectReuse();
   or.arrayCopy();
}
```

    该方法属于类的静态方法，在类加载初始化的时候就会被加载入内存中，同时对于此类而言，在其他地方该main函数都是可见的，所以应该去掉。

Java代码：

```java
try
{
	ObjectReuse.main(new String[]
	{ "a", "b", "c" });
} catch (IOException e)
{
	e.printStackTrace();
}
```



【情景二】删除无效代码行

       无效的代码行也会给编译器带来工作量，所以建议去掉。

Java代码：


```java
private double toPercent(int y, int z)
	{
		// String result = "";// 接受百分比的值
		double baiy = y * 1.0;
		double baiz = z * 1.0;
		double fen = (baiy / baiz) * 100;
		// NumberFormat nf = NumberFormat.getPercentInstance(); //注释掉的也是一种方法
		// nf.setMinimumFractionDigits( 2 ); //保留到小数点后几位
		// DecimalFormat df1 = new DecimalFormat("##.00%"); // ##.00%
		// // 百分比格式，后面不足2位的用0补齐
		// result = nf.format(fen);
		return fen;
	}

	// 清理后的Java代码：
	private double toPercent(int y, int z)
	{
		double baiy = y * 1.0;
		double baiz = z * 1.0;
		double fen = (baiy / baiz) * 100;
		return fen;
	}
```

编译器会判断“//”作用的行是否结束，“/* */”注释同理。



【情景三】删除不规范的注释

有些注释根本没有实际的意义，那就果断地删除它。


```java
/*
	 * (非 Javadoc)
	 * <p>Title:根据参数查询DeviceMember</p>
	 * <p>Description: </p>
	 * @param dm
	 * @param page
	 * @param rows
	 * @return
	 * @see
	 * com.kvt.lbs.system.service.IDeviceService#getDeviceMemberList(com.kvt
	 * .lbs.system.entity.DeviceMember, int, int)
	 */
	@Override
	public Map<String, Object> getDeviceMemberList(DeviceMember dm, int page, int rows, String sortName, String sortOrder)
	{
		return deviceMemberDao.getDeviceMember(dm, page, rows, sortName, sortOrder);
	}
```

    看到这种不好的注释到处泛滥，连基本的参数说明都省略掉了真的很令人头疼。要么直接删除掉要么将注释填写完整。删除很简单也不会耗费多少时间，但这里以补充完整的注释加以说明。

Java代码：





```java
/**
 * 根据传入参数查询DeviceMember数据
 * 
 * @param dm
 *            设备对象
 * @param page
 *            分页索引
 * @param rows
 *            每页条数
 * @param sortName
 *            排序字段
 * @param sortOrder
 *            排序类型
 * @return Map<String, Object> 设备分页数据属性对象
 */
public Map<String, Object> getDeviceMemberList(DeviceMember dm, int page, int rows, String sortName, String sortOrder);
```

如果要写注释就要写清楚，不要让阅读代码或使用该方法的人去猜参数的含义。



**22、Session管理**

【情景一】避免JSP页面的Session频繁创建

     一个常见的误解是以为session在有客户端访问时就被创建，然而事实是直到某server端程序调用HttpServletRequest.getSession(true)这样的语句时才被创建，注意如果JSP没有显示的使用 <%@page session="false"%> 关闭session，则JSP文件在编译成Servlet时将会自动加上这样一条语句HttpSession session = HttpServletRequest.getSession(true);这也是JSP中隐含的session对象的来历。由于session会消耗内存资源，因此，如果不打算使用session，应该在所有的JSP中关闭它。

对于那些无需跟踪会话状态的页面，关闭自动创建的会话可以节省一些资源。使用如下page指令：

 example:                  <%@ page session="false"%>

设置为false并不是禁用会话跟踪，它只是阻止jsp页面为不拥有会话的用户创建新会话。

对于不需要会话跟踪的页面那就设置为false；当设置为false时session对象是不可访问的。


【情景二】及时清理不需要的Session

     为了清除不再活动的会话，许多应用服务器都有默认的会话超时时间，一般为30分钟。当应用服务器需要保存更多会话时，如果内存容量不足，操作系统会把部分内存数据转移到磁盘，应用服务器也可能根据“最近最频繁使用”（Most Recently Used）算法把部分不活跃的会话转储到磁盘，甚至可能抛出“内存不足”异常。在大规模系统中，串行化会话的代价是很昂贵的。当会话不再需要时，应当及时调用HttpSession.invalidate()方法清除会话。HttpSession.invalidate()方法通常可以在应用的退出页面调用。

  Java代码：


```java
@RequestMapping("/out")
public String out(HttpServletRequest req, HttpServletResponse resp)
{
	isystemLogService.saveLog(req, "用户注销退出", "用户注销退出", "用户注销退出");
	req.getSession().removeAttribute("user");
	req.getSession().invalidate();
	Log.getLogger(this.getClass()).info("用户注销成功！");
	return "redirect:../";
}
```



**23、使用单元测试代替main方法**

　　对程序中关键细节的功能添加单元测试，这些单元测试可以保留在系统中，提供给测试人员使用、方便统计单元测试覆盖率等等；降低程序在测试人员测试前出错的频率；如使用Junit等测试工具来实现。

Java代码：


```java
public class UserTest
{
	private IUserService userService;

	@Before
	public void initInterface()
	{
		userService = new UserService();
	}

	@After
	public void freeSources()
	{
		userService = null;
	}

	@Test
    public void add()
    {
        User user = new User("boonya", "male", 23, "Java programmer");
        boolean success = userService.add(user);
        System.out.println(success ? "添加成功" : "添加失败");
　　}
}
```



注：有些地方可能欠考虑希望大家指出不妥之处。



