# Java类加载顺序 - 零度的博客专栏 - CSDN博客
2016年04月12日 16:01:13[零度anngle](https://me.csdn.net/zmx729618)阅读数：298
        关于类的static成员变量初始化、static初始化块、实例的普通成员变量初始化、实例的普通初始化块以及构造函数的执行顺序，我一直不是十分明确地知道，今天专门花了几个小时的时间参考网上资料设计出了比较明了的测试代码，有代码有结果有真相。总体而言，static部分执行早于普通初始化块早于构造函数，如果一个类继承了某个父类，则父类的static部分最先执行。测试代码设计思路：有三个主线类B、C和D，其中D继承C，C继承B，这三个类中均包含static块、普通初始化块和无参的构造方法；有两个辅助类E和F，B中包含E类和F类的成员变量，F类成员变量是static类型，E类的成员变量是普通类型；程序运行入口在A.java中，A中的main函数只用来创建D类的实例，其代码列表如下。
```
package zmx.calss.loadorder;
public class F {
	
	public F() {
		System.out.println("执行F的构造方法");
	}
	
	public void funcOfF(){
		
	    System.out.println("执行F的方法");
	}
}
```
```
package zmx.calss.loadorder;
public class E {
	
	public E() {
		System.out.println("执行E的构造方法");
	}
	
	public void funcOfE(){
		
	    System.out.println("执行E的方法");
	}
}
```
```
package zmx.calss.loadorder;
public class D extends C{
	
	public String sd1=getSd1();
	public static String sd=getSd();
	      static{
	          System.out.println("执行D的static块(D继承C)");
	         
	     }
         {
	         System.out.println("执行D实例的普通初始化块");
	     }
	     D(){
	         System.out.println("执行D的构造函数(D继承C);父类B的实例成员变量sb的值为："+sb+";本类D的static成员变量sd的值为："+sd+";本类D的实例成员变量sd1的值是："+sd1);
	    }
	     
	     static public String getSd()
	     {
	         System.out.println("初始化D的static成员变量sd");
	         return "sd";
	     }
	     public String getSd1()
	    {
	        System.out.println("初始化D的实例成员变量sd1");
	         return "sd1";
	     }
}
```
```
package zmx.calss.loadorder;
public class C extends B{
	
     static{
	         System.out.println("执行C的static块(C继承B)");
     }
     
     {
	         System.out.println("执行C的普通初始化块");
	 }
     
     C(){
	         System.out.println("执行C的构造函数(C继承B)");
	 }
}
```
```
package zmx.calss.loadorder;
public class B {
	E e = new E();
	static F f = new F();
	public String sb = getSb();
	static {
		System.out.println("执行B类的static块(B包含E类的成员变量,包含静态F类成员变量)");
		f.funcOfF();
	}
	{
		System.out.println("执行B实例的普通初始化块");
	}
	B() {
		System.out.println("执行B类的构造函数(B包含E类的成员变量,包含静态F类成员变量)");
		e.funcOfE();
	}
	public String getSb() {
		System.out.println("初始化B的实例成员变量sb");
		return "sb";
	}
}
```
```
package zmx.calss.loadorder;
public class A {
	
	public static void main(String[] args) {
		System.out.println("运行A中的main函数，创建D类实例");
		D d=new D();
	}
}
```
程序执行结果：
运行A中的main函数，创建D类实例
执行F的构造方法
执行B类的static块(B包含E类的成员变量,包含静态F类成员变量)
执行F的方法
执行C的static块(C继承B)
初始化D的static成员变量sd
执行D的static块(D继承C)
执行E的构造方法
初始化B的实例成员变量sb
执行B实例的普通初始化块
执行B类的构造函数(B包含E类的成员变量,包含静态F类成员变量)
执行E的方法
执行C的普通初始化块
执行C的构造函数(C继承B)
初始化D的实例成员变量sd1
执行D实例的普通初始化块
执行D的构造函数(D继承C);父类B的实例成员变量sb的值为：sb;本类D的static成员变量sd的值为：sd;本类D的实例成员变量sd1的值是：sd1
分析：由输出结果可知，
　　(1) 整体上先执行static部分(1至7行)后执行非static部分(8至17行)。
　　(2) 在static部分中先运行父类的后运行子类的。第2行执行F的构造函数是因为B中包含F类的static(静态)变量，在B.java的第6行调用了F的构造函数初始化static变量；第4行执行F的funcOfF是因为在B的　　　　       　 static初始化块中调用了这个函数；之后依次执行C和D的static部分。
　　(3) 在非static部分中也是先运行父类的后运行子类的，对于同一个类，其成员变量的初始化和普通初始化块的执行优先于构造函数。
　　对于同一个类，其成员变量初始化一定优先于初始化块的执行吗？对于非static的情况，我们将D.java中的第5行移动到普通初始化块的后面，如下面所示，
```
package zmx.calss.loadorder;
public class D extends C{
	
	
	public static String sd=getSd();
	      static{
	          System.out.println("执行D的static块(D继承C)");
	         
	     }
         {
	         System.out.println("执行D实例的普通初始化块");
	     }
         
         public String sd1=getSd1();
         
	     D(){
	         System.out.println("执行D的构造函数(D继承C);父类B的实例成员变量sb的值为："+sb+";本类D的static成员变量sd的值为："+sd+";本类D的实例成员变量sd1的值是："+sd1);
	    }
	     
	     static public String getSd()
	     {
	         System.out.println("初始化D的static成员变量sd");
	         return "sd";
	     }
	     public String getSd1()
	    {
	        System.out.println("初始化D的实例成员变量sd1");
	         return "sd1";
	     }
}
```
程序运行结果如下：
运行A中的main函数，创建D类实例
执行F的构造方法
执行B类的static块(B包含E类的成员变量,包含静态F类成员变量)
执行F的方法
执行C的static块(C继承B)
初始化D的static成员变量sd
执行D的static块(D继承C)
执行E的构造方法
初始化B的实例成员变量sb
执行B实例的普通初始化块
执行B类的构造函数(B包含E类的成员变量,包含静态F类成员变量)
执行E的方法
执行C的普通初始化块
执行C的构造函数(C继承B)
执行D实例的普通初始化块
初始化D的实例成员变量sd1
执行D的构造函数(D继承C);父类B的实例成员变量sb的值为：sb;本类D的static成员变量sd的值为：sd;本类D的实例成员变量sd1的值是：sd1
       该结果的第15和16行的输出结果与改动之前的输出结果正好相反，可见成员变量的初始化与普通初始化块的执行顺序不是固定的，它与源代码中赋值语句及普通初始化块的放置顺序相关：成员变量的赋值语句在前则赋值先执行，普通初始化块在前则初始化块先执行。对于static成员变量和static初始化块的执行顺序也是类似的，修改B.java中的第6行的位置即可看到类似的输出结果，此处不再赘述。
总结：
       1、 class类的执行的大致顺序如下，
(1) 在一个不存在继承的类中：初始化static变量,执行static初始化快-->初始化普通成员变量(如果有赋值语句),执行普通初始化块-->构造方法
　　(2)在一个存在继承的类中：初始化父类static成员变量,运行父类static初始化块-->初始化子类static成员变量,运行子类static初始化块-->初始化父类实例成员变量(如果有赋值语句),执行父类普通初始化块-->父类构造方法-->初始化子类实例成员变量(如果有赋值语句)及普通初始化块-->子类构造方法。
　　注意：其中变量初始化(赋值)和初始化块的执行与相关语句在源码中的放置顺序一致，不过变量声明会最先执行，参考[http://www.189works.com/article-52232-1.html](http://www.189works.com/article-52232-1.html)。
     2、  static成员变量可以再定义的时候初始化也可以在static块中初始化，static块可以出现多次，当编译成.class文件时会将多个static块的内容合并；实例成员变量可以再定义时初始化也可以在普通初始化块或构造函数中初始化。
　基本数据类型的成员变量要在初始化后再使用，引用数据类型的成员变量在实例化后才能被使用。
      3、 类的加载时机：
  (1) 用new创建该类的实例时；
  (2) 使用java.lang.reflect进行反射调用的时候；
  (3) 之前没有加载该类，之后加载该类的子类的时候；
  (4) 当虚拟机启动时，初始化main函数所在的类。
       4、JVM载入类时会执行static块，创建一个实例时会运行构造方法。static块和static成员变量都是属于类而非实例的；构造函数和普通成员变量是属于一个实例的。类的初始化(包括static块的运行和static成员变量的赋值)只运行一次，多次创建某个类的实例只会运行一次该类的static()块，但会运行多次其构造函数。将之前的A.java中第9行的注释去掉后再运行即可说明情况。
