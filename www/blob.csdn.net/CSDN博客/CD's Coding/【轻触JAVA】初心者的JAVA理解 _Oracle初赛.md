# 【轻触JAVA】初心者的JAVA理解 _Oracle初赛 - CD's Coding - CSDN博客





2015年04月20日 12:39:10[糖果天王](https://me.csdn.net/okcd00)阅读数：806标签：[Oracle																[java																[初赛																[甲骨文																[2015](https://so.csdn.net/so/search/s.do?q=2015&t=blog)
个人分类：[Coding Competition](https://blog.csdn.net/okcd00/article/category/2133459)





啊啦，初赛也比完了，好厉害的样子我想说……

JAVA真的有太多太多需要学的，以前我一直觉得，啊C#和C++差不多，JAVA和C#差不多，所以JAVA不难……

事实证明我错了……各种好难好难因为基础不足都不知道，但也硬着头皮一点点做着题。

初赛虽然事故频出，但至少还是完整的结束了战斗，做个小小的总结吧。

由于比赛过程中网络问题颇于严重，截图将每道题发给同组的同学让其不用和时刻502的服务器作斗争，才有了如下回顾所用的素材。

就算单单因为为了给大家看看而敲了这么多字，也得给个好评不是嘛![得意](http://static.blog.csdn.net/xheditor/xheditor_emot/default/proud.gif)

【声明: 本人依照比赛中查阅资料获得的浅显理解进行说明，若有不对的地方，请在评论中指出指教，我将非常感谢并立马纠正错误![可怜](http://static.blog.csdn.net/xheditor/xheditor_emot/default/cute.gif)】

为了防止本人的理解失误导致误人子弟的现象，较多地放出直接运行看结果可以确定答案的题目（运行环境：Eclipse Standard/SDK Version: Luna Release (4.4.0)）

P.S. 编程题的两道（29 & 31）赛中评测获得【完全正确】，品质有保障。




### 02. 追踪程序流程



```java
package Test02;

public class WanWanMeiXiangDao 
{
    public static void main(String[] args) {
        try (DaChuiCareer daChui = new DaChuiCareer()) {
            daChui.happyLife();
            return;
        } catch (Exception e) {
            System.out.println(e.getClass().getSimpleName());
            return;
        } finally {
            System.out.println("story end");
            return;
        }
    }
    public static class DaChuiCareer implements AutoCloseable {
        public DaChuiCareer() {
            System.out.println("born");
        }
        public void happyLife() throws JoyfulException {
            System.out.println("happy living");
            throw new JoyfulException();
        }
        @Override
        public void close() throws UnexpectedException {
            System.out.println("unexpected");
            throw new UnexpectedException();
        }
    }
}
class JoyfulException extends Exception { }
class UnexpectedException extends Exception { }
```




输出为：

born

happy living

unexpected

JoyfulException

story end




### 03. 静态与非静态



```java
package Test03;

public class Test {

	void method() {
	    Object o = null;
	    try {
	        methodA();
	        methodB(o.toString());
	    } catch (InterruptedException | IOException e) {
	        System.out.println(e.getClass().getSimpleName());
	    }
	}

	void methodA() throws IOException {
	    throw new FileNotFoundException();
	}

	void methodB(String s) throws InterruptedException {
	    throw new IllegalArgumentException();
	}
	
	public static void main(String[] args) {
		method();
	}
}
```


结果：编译不通过

Exception in thread "main" java.lang.Error: Unresolved compilation problem: 
Cannot make a static reference to the non-static method method() from the type Test
at Test03.Test.main(Test.java:24)





### 05. 时刻注意数据范围



```java
package Test05;

class FruitNinjia {
	  public static void main(String [] args) {
	    System.out.println(new Ninjia().cut(300));
	  }
}
```

```java
package Test05;

class Ninjia {
	  String cut(short fruit) { return "piece of cake!"; }
	  String cut(short... fruits) { return "it is hard..."; }
	}
```
这里的 cut(short... fruits) ，传参的数据类型为short，要知道short只能最多到128，必然出错。

结果：编译出错

Exception in thread "main" java.lang.Error: Unresolved compilation problem: 
The method cut(short) in the type Ninjia is not applicable for the arguments (int)
at Test05.FruitNinjia.main(FruitNinjia.java:5)



### 07. 子函数不会更改传参值





```java
package Test07;

public class Test {
	public static void main(String[] args) {
		  int[] da1 = new int[]{1, 3, 5};
		  int[] da2 = new int[]{2, 4, 6};
		  System.out.print(da2[da1[0]]);
		  interchange(da1, da2);
		  System.out.print(da1[da2[0]]);
		}
		static void interchange(int[] da1, int[] da2) {
		  int[] dat = (int[]) da1.clone();
		  da1 = (int[]) da2.clone();
		  da2 = (int[]) dat.clone();
		}
}
```
虽然看起来interchange函数确实干了点什么事情，好像交换了da1和da2，但问题是，和C++中一样，在这个函数里改了并不会影响外部定义的这两个变量。

输出：45




### 08. INT型除法不要余数



```java
package Test08;

public class Test {
	public static void main(String[] args) {
		int i = (2 ^ 4 | 1 >> 5 + 3 << 3 % 2) / 2;
		if (2.5 == i) {
		  System.out.println("ok");
		} else {
		  System.out.println("fail");
		}
	}
}
```
这个，玩C++的大家应该很懂吧，int型如果有个除以2的话，是不会出现2.5这样的小数的，除以2等效于右移一位，余数是被扔掉的。

输出:fail




### 10. 同名函数调用





```java
package Test10;

public class MiniPad extends Pad {
	  public final void pad() {
	    System.out.println("Mini pad");
	  }
	  public static void main(String [] args) {
	    new MiniPad().pad();
	  }
	}
```

```java
package Test10;

	class Pad {
		  private final void pad() {
		    System.out.println("Big pad");
		  }
		}
```


当一个类的继承类中有着和自身同名的类的时候，调用哪个，这往往是常常被问到的问题。

不过这道题要知道Pad里的这个函数是private的……

输出：Mini Pad



### 11. Index Start With 0



```java
package Test11;


public class HolyDays {
	public enum Days { SUN, MON, TUE, WED };
	public static void main(String[] args) {
		for (Days d : Days.values());
     Days [] d = Days.values();
     System.out.println(d[3]);
   }
 }
```
并不是很嗯那个理解考察点在哪里，可能是为了让大家知道，Index是从0开始的，所以3其实是第4个。

输出：WED



### 12. 线程运行





```java
package Test12;

public class Sudo implements Runnable {
	  public static void main(String[] args) throws Exception {
	    Thread t = new Thread(new Sudo());
	    t.start();
	    System.out.print("Started");
	    t.join();
	    System.out.print("Complete");
	  }
	  public void run() {
	    for (int i = 0; i < 4; i++) {
	      System.out.print(i);
	    }
	  }
	}
```

Java Thread中， join() 方法主要是让调用改方法的thread完成run方法里面的东西后， 在执行join()方法后面的代码。


输出： Started0123Complete




13.



```java
package Test13;

public class Weibo {
	  private String content;
	  public String getContent() { return content; }
	  public void setContent(String d) { content = d; }
	  public static void modifyCont(Weibo weibo, String content) {
	    weibo = new Weibo();
	    weibo.setContent(content);
	  }
	  public static void main(String[] args) {
	    Weibo wb = new Weibo();
	    wb.setContent("javacontest");
	    Weibo wb2 = new Weibo();
	    wb2.setContent("jingkao");
	    modifyCont(wb, "sudocn");
	    System.out.println(wb.getContent());
	    System.out.println(wb2.getContent());
	  }
	}
```

单单只是读一读代码，翻译过来好像就是 javacontest/jingkao 被修改成了 sudocn/jingkao

这个似乎当时写错了，并不是很能理解，个人猜测可能是modifyCont操作并没有真正修改到值（有人能告诉我一下就非常感谢啦^_^）为啥Eclipse的输出是酱紫的：

输出：

javacontest

jingkao



### 14. 注意数据类型



```java
package Test14;

public class Java {
   Integer i;
   int x;
   public Java(int y) {
     x = i + y;
     System.out.println(x);
   }
   public static void main(String[] args) {
     new Java(new Integer(4));
   }
 }
```

错在了 x=i+y 这一行，需要注意的是，i是Integer类的实例，而y是int整型的数值，以及这个Integer的声明之后似乎还没有被赋值过。

结果：编译不通过

Exception in thread "main" java.lang.NullPointerException
at Test14.Java.<init>(Java.java:7)
at Test14.Java.main(Java.java:11)





### 15. 优先队列



```java
package Test15;

import java.util.PriorityQueue;

public class Test {
	
	public static void main(String[] args) 
	{
		 PriorityQueue<String> pq = new PriorityQueue<String>();
		 pq.add("2");
		 pq.add("4");
		 System.out.print(pq.peek() + " ");
		 pq.offer("1");
		 pq.add("3");
		 pq.remove("1");
		 System.out.print(pq.poll() + " ");
		 if (pq.remove("2")) System.out.print(pq.poll() + " ");
		 System.out.println(pq.poll() + " " + pq.peek());
	}
}
```

优先队列哎~ 这个我熟~~，优先队列的本质是堆，被当放入新的元素的时候它会自动排序。

这里的顺序应该是 2 / 24 / 打印顶部2 / 1是个过客 / 234 / 依次输出2 3 4

输出: 2 2 3 4


### 18. try...catch



```java
package Test18;

class OhMyException {
	  static String s = "> ";
	  public static void main(String[] args) {
	    try {
	      throw new Exception();
	    } catch (Exception e) {
	      try {
	        try { throw new Exception(); }
	        catch (Exception ex) { s += "eric "; }
	        throw new Exception();
	      } catch (Exception x) { s += "michael "; }
	      finally { s += "fyi "; }
	    } finally { s += "chao "; }
	    System.out.println(s);
	  }
	}
```
这是一个一环套一环，但是巧妙地全都用上了的try/catch机制示例

输出：> eric michael fyi chao 




### 20. 构造函数



```java
package Test20;

class Fat {
	  String name = "No name";
	  public Fat(String nm) { name = nm; }
}
```

```java
package Test20;

public class FatTest 
{
	  public static void main(String[] args) {
		    Steven s = new Steven("90kg");
		    System.out.println(s.weight);
	  }
}
```

```java
package Test20;

class Steven extends Fat 
{
	String weight = "80kg";
	public Steven(String w){ weight = w; }
}
```

Class Steven中 public Steven(String w){weight=w;}这句，Steven(String w)下有红波浪线，

编译错误：Implicit super constructor Fat() is undefined. Must explicitly invoke another constructor

隐式的父级构造函数fat()是未定义的。必须显式地调用另一个构造函数




### 21. 线程问题



```java
package Test21;

public class Thread2 implements Runnable {
	  public void run() {
	    System.out.println("run.");
	    throw new RuntimeException("Problem");
	  }
	  public static void main(String[] args) {
	    Thread t = new Thread(new Thread2());
	    t.start();
	    System.out.println("End of method.");
	  }
	}
```

存疑，这个部分理解得尚不透彻

输出：（似乎有多种情况，输出其中一种）

End of method.

run.

Exception in thread "Thread-0" java.lang.RuntimeException: Problem
at Test21.Thread2.run(Thread2.java:6)
at java.lang.Thread.run(Thread.java:745)



### 24. 又一个线程问题



```java
package Test24;

public class IncomprehensiveThread {
	  synchronized void a() { maybeBusy(); }
	  static synchronized void b() { maybeBusy(); }
	  
	  static void maybeBusy() {
	    try { Thread.sleep(1000); }
	    catch (InterruptedException e) {}
	  }
	  public static void main(String[] args) {
	    final IncomprehensiveThread x = new IncomprehensiveThread();
	    final IncomprehensiveThread y = new IncomprehensiveThread();
	    Runnable runnable = new Runnable() {
	      public void run() {
	        int option = (int) (Math.random() * 4);
	        switch (option) {
	          case 0: x.a(); break;
	          case 1: x.b(); break;
	          case 2: y.a(); break;
	          case 3: y.b(); break;
	    } } };
	    Thread thread1 = new Thread(runnable);
	    Thread thread2 = new Thread(runnable);
	    thread1.start();
	    thread2.start();
	} }
```
这个问题如果我没记错的话，问的是，x的a和b，与y的a和b，两两之间哪些不能同时运行。

存疑。



### 29. 编程题A



```java
package Test29;

public class Adder {
	public static int add(int a,int b){
		return a+b;
	}
	public static double add(double a,double b){
		double result = Math.round(10*(a+b))/10.0;
		return result;
	}
	public static String add(String a,String b){
		return a+b;
	}
}
```

实现AdderTest里用到的所有复用，由于要求不能抛出异常，则每个按照异常中所要求的写好函数即可。


### 30. 编程题B

这题的难点在于串式调用以及省略号传参，可惜不懂得如何串式调用……




### 31. 编程题C



```java
package Test31;

public class Person {
	String name;
	int age;
	
	public Person(String name, int age) {
		super();
		this.name = name;
		this.age = age;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public int getAge() {
		return age;
	}

	public void setAge(int age) {
		this.age = age;
	}
	 public boolean equals(Object o)   
	    {   
	        if (this == o)   
	        {   
	            return true;   
	        }   
	        if (o.getClass() == Person.class)   
	        {   
	            Person n = (Person)o;   
	            return n.name.equals(name)&&(n.age==age);   
	        }   
	        return false;   
	    } 
	 public int hashCode()   
	    {   
	       return (name+Integer.toString(age)).hashCode();   
	    }  
	  
	    public String toString()   
	    {   
	        return "Name[name=" + name + ", age=" + age + "]";   
	    }  
}
```
get/set无脑写上去，难点在哪里呢？ 对了，我们如果想用上哈希的话需要实现这三个： equals、hashCode 和 toString 方法。

考虑到这个这题就差不多了。](https://so.csdn.net/so/search/s.do?q=甲骨文&t=blog)](https://so.csdn.net/so/search/s.do?q=初赛&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=Oracle&t=blog)




