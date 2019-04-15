# Java 枚举总结 - fuckluy的博客 - CSDN博客












2016年11月02日 17:42:22[Zcc?](https://me.csdn.net/fuckluy)阅读数：539











**如需转载，请注明原地址，谢谢**

## 枚举类型（enum type）

枚举类型是指由一组固定的常量组成合法值的类型，是引用类型，在Java1.5中加入，

public abstract class **Enum<E extends Enum<E>> **extends [Object]()implements [Comparable]()<E>, [Serializable]()
这是所有Java语言枚举类的公共基类，由我前面写的泛型总结可知，枚举类中使用的泛型参数必须是Enum类的子类，由基类可知，枚举类继承了Object类并实现可Comparable接口和Serializable接口，这样枚举类能够实现元素比较和序列化操作




**几点说明：**

1、enum的成员实例要求是唯一的

2、enum的本质是int值

3、java中与enum相关联的集合容器

EnumSet：EnumSet中的元素必须来自一个enum，EnumSet的基础是long其中一位表示enum中的一个实例（但是enum可以超过64个元素，应该是使用了自动扩容机制）

EnumMap：特殊的Map，要求键来自一个enum，EnumMap的内部由数组实现，因此EnumMap速度很快（说个题外话，HashMap的内部实现是数组+红黑树，通过散列函数获取数组的index，数组中的每个元素都代表一个红黑树，这样解决了散列冲突的问题，而红黑树的主要是在模仿2-3树，这样相比起普通的1-2树就大大的降低了树的高度，2-3树是一棵完美平衡树）

4、编译器不允许enum实例当作class类型，因此enum的实例不能作为参数的类型







### enum的创建与常用方法

下面的Demo展示了enum的创建以及一些常用方法


```java
package Num_6;


interface A {
	public void f();
}

//一个类无法显示的继承java.lang.Enum
//class B extends Enum<Enum<E>>{}

//真实的含义 final class C extends Enum<C>{},由编译器完成
enum C {
}

/**
 * 与class，interface，一样，一个java类中只能有一个被public修饰的class 或 interface或enum。
 * 创建enum时，编译器会为你生成一个相关的类，这个类继承自java.lang.Enum,这些由编译器完成且enum被修饰为final
 * 因此该enum不能在extends其他的类但可以implements接口
 *
 * @author he
 *
 */
enum Suit implements A {
	CLUB, DIAMOND, HEART, SPADE;

	@Override
	public void f() {
		System.out.println("实现了接口A");
	}
}

public class Test {
	public static void main(String[] args) {
		Suit suit=Suit.CLUB;
		suit.f();//实现了接口A
		suit.values(); //可用
		Enum en=Suit.CLUB;
	//	en.values(); 此方法不可用，编译通不过
		/**
		 * enum常用方法
		 */
		//Suit.values()方法返回一个数组Suit[]
		for(Suit s:Suit.values()){
			System.out.println(s+"    ordinal   "+s.ordinal());//返回序数，及在声明时的index，从0开始
			System.out.println(s.compareTo(Suit.CLUB));//比较大小,enum的本质是int值
			System.out.println(s==Suit.CLUB);//可以用==比较Suit的实例
			System.out.println(s.getDeclaringClass());//返回s所在enum的类名
			System.out.println(s.name());//返回s的名字
			System.out.println("----------------------------------");
		}
	}
}
```




从上面的Demo中看到，使用了**values（）方法**，但是你去查看API时API中并没有values()方法，这是因为values()方法是由编译器添加的static方法，同时还添加了valueOf()方法，该valuesOf()方法只有一个参数，因此当enum向上转型为Enum时values（）方法就不可用，下面的Demo使用反射证实该结论


```java
public static void main(String[] args) {
		try {
			Class<?> s=Class.forName("Num_6.Suit");//定义的enum，包名+类名
			Class<?> f=Class.forName("java.lang.Enum");//Enum基类
			List<String> list=new ArrayList<String>();	
			for(Method m:s.getMethods())
				list.add(m.getName());
			for(Method x:f.getMethods())
				if(list.contains(x.getName())){
					list.remove(x.getName());
				}
			System.out.println(list);//<span style="color:#ff0000;">values</span>,f--因为实现接口A，<span style="color:#ff0000;">valueOf</span>
		} catch (ClassNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}		
	}
```

**枚举类型背后的思想：**通过 public static final 域为每个枚举常量导出实例的类。没有可以访问的构造器，枚举类型是真正的final，因为客户端不能创建枚举类型的实例，不能对其扩展，可参考这篇文章中的反编译代码：[点击打开链接](http://pf-miles.iteye.com/blog/187155)





### 向enum中添加方法

如果打算定义自己的方法，则enum实例序列的最后一个要添加分号，且必须先定义enum实例，如果方法或属性定义在了实例之前则出错


```java
public enum Test {
	// private int x; 编译出错
	A("#A"), B("#B"), C("#C");
	private String desc;

	// 私有化构造器,即使不私有化，一但enum的定义结束，编译器就不允许我们再使用其构造器创建任何实例了
	private Test(String description) {
		this.desc = description;
	}

	public String get() {
		return desc;
	}

	/**
	 * 可以使用泛型方法
	 * @param x
	 */
	public <T>void h(T x){
		System.out.println(x.getClass().getName());
	}
	
	@Override
	public String toString() {
		return name() + "-" + ordinal();
	}

	public static void main(String[] args) {
		Test x=Test.A;
		x.h(1);//java.lang.Integer
		
		for (Test t : Test.values()) {
			System.out.println(t.get());
			System.out.println(t);
		}
		/**
		 * 输出结果： 
		 * #A A-0 
		 * #B B-1 
		 * #C C-2
		 */

	}

}
```





### 使用接口组织枚举

因为无法从enum继承子类，但是又需要对enum进行更细致的分类，这时可以在一个接口的内部，创建实现该接口的枚举，以此将元素进行分组


```java
interface Fruit{
	enum Apple implements Fruit{
		GREEN_APPLE,
		RED_APPLE
	}
	
	enum Grape implements Fruit{
		RED_GRAPE,
		VIOLET_GRAPE
	}
}
```




### enum的多态性

为一个enum定义一个抽象方法，每个实例去具体实现该方法并返回不同的结果，对于一般的方法，enum实例可以覆盖该方法


```java
package Num_6;

public enum Test {
	A {
		@Override
		String getInfo() {
			return "A";
		}
	},
	B {
		@Override
		String getInfo() {
			// TODO Auto-generated method stub
			return "B";
		}

		@Override
		void f() {
			System.out.println("ttttt");// 覆盖了f()方法
		}
	};
	abstract String getInfo();

	void f() {
		System.out.println("ffff");
	}

	public static void main(String[] args) {
		for (Test t : values()) {
			System.out.println("getInfo: " + t.getInfo());
			t.f();
		}
	}
	/**
	 * 输出结果： 
	 * getInfo: A 
	 * ffff 
	 * getInfo: B 
	 * ttttt
	 * 
	 */
}
```























