# Java 泛型总结 - fuckluy的博客 - CSDN博客












2016年10月20日 17:50:37[Zcc?](https://me.csdn.net/fuckluy)阅读数：1977








# 

**如需转载，请注明原地址，谢谢**

# 什么是泛型

泛型实现了参数化类型的概念，主要目的是希望类或方法能够就最广泛的表达能力

### 泛型的优点和局限性

**优点：**1、在编译时提供安全检查


        2、消除强制类型转换（都是隐式的自动完成,运行时引用类都是Object，在从泛型容器中取出值时编译器进行类型转换）

             3、最大限度的重用代码

     4、泛型和设计模式的组合可以仿真潜在类型机制

潜在类型机制：一种代码组织和复用机制，有了它可以编写更复用的代码，简单的说就是程序不管是什么类型的类，只要有需要的方法就行，而JAVA不支持这种特性，比如下面的demo


```java
class SimpleQueue<T> implements Iterable<T> {
	LinkedList<T> list = new LinkedList<T>();

	public void add(T item) {
		list.add(item);
	}

	public T get() {
		return list.poll();// 获取并移除列表的第一个元素
	}

	@Override
	public Iterator<T> iterator() {

		return list.iterator();
	}

}

class Fill {
	// 主要想使用Collection的add方法向其中添加实例，但是这样又限制在了Collection的层次结构中，
	// 导致其他不再这个层次结构中但又具有add方法的类不能使用该方法，比如SimpleQueue
	public static <T> void fill(Collection<T> collection, Class<? extends T> class1Token, int size) {
		for (int i = 0; i < size; i++) {
			try {
				collection.add(class1Token.newInstance());// 向collection中添加实例
			} catch (Exception e) {
				throw new RuntimeException();// 如果捕获到异常就抛出运行时异常
			}
		}
	}
}

class A {
	@Override
	public String toString() {

		return getClass().getName().toString();
	}
}

class B extends A {
}

public class Test {
	public static void main(String[] args) {
		List<A> listA = new ArrayList<A>();
		Fill.fill(listA, A.class, 3);// 没问题
		Fill.fill(listA, B.class, 2);// 没问题

		/**
		 * SimpleQueue中也有add方法但是无法使用Fill.fill()方法，限制了代码的泛化性
		 */
		SimpleQueue<A> sq=new SimpleQueue<A>();
	//  Fill.fill(sq, A.class, 3);//编译通不过
		
	}
}
```


可以通过适配器模式仿真这样的潜在类型机制，比如下面的Demo


```java
package Num_6;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;

class SimpleQueue<T> implements Iterable<T> {
	LinkedList<T> list = new LinkedList<T>();

	public void add(T item) {
		list.add(item);
	}

	public T get() {
		return list.poll();// 获取并移除列表的第一个元素
	}

	@Override
	public Iterator<T> iterator() {

		return list.iterator();
	}

}

// 需要的接口
interface Addable<T> {
	void add(T t);
}

class A {
	@Override
	public String toString() {

		return getClass().getName().toString();
	}
}

class B extends A {
}

class SimpleQueueSon<T> extends SimpleQueue<T> implements Addable<T> {
	@Override
	public void add(T item) {
		super.add(item);
	}
}

/**
 * 最底层的适配器类
 * 
 * @author he
 *
 */

class AddableCollectionAdapter<T> implements Addable<T> {
	private Collection<T> collection;

	public AddableCollectionAdapter(Collection<T> collection) {
		this.collection = collection;
	}

	@Override
	public void add(T t) {
		collection.add(t);
	}
}
/**
 * 向外提供静态方法的类
 * @author he
 *
 */
class Adapter{
	public static<T> Addable<T> collectionAdapter(Collection<T> c){
		return new AddableCollectionAdapter<T>(c);
	}
}



class Fill2 {
	// 这样可以自己定制一些类，只要它实现了Addalbe接口，且add方法可行就能使用该方法，比如SimpleQueueSon
	public static <T> void fill(Addable<T> addable, Class<? extends T> class1Token, int size) {
		for (int i = 0; i < size; i++) {
			try {
				addable.add(class1Token.newInstance());// 向addable中添加实例
			} catch (Exception e) {
				throw new RuntimeException();// 如果捕获到异常就抛出运行时异常
			}
		}
	}
}

public class Test {
	public static void main(String[] args) {
		List<A> listA = new ArrayList<A>();
		 Fill2.fill(new AddableCollectionAdapter<A>(listA), A.class, 3);// 没问题
		 Fill2.fill(Adapter.collectionAdapter(listA), B.class, 2);// 没问题

		/**
		 * SimpleQueueSon实现了Addable接口因此可以使用fill()方法
		 */
		SimpleQueueSon<A> sq = new SimpleQueueSon<A>();
		 Fill2.fill(sq, A.class, 3);//没问题
		 Fill2.fill(sq, B.class,2);//没问题
		 for(A c:sq)
			 System.out.println(c);//Num_6.A  Num_6.A Num_6.A Num_6.B Num_6.B
	}
}
```
**说明：**关于第一个Demo中，只要SimpleQueue实现了Collection接口不就行了吗？这样当然可以，但是实现了Collection接口需要添加很多不需要的方法而继承属于Collection层次中的某个类也会使SimpleQueue继承很多不需要的方法，造成臃肿，按照Demo2中的办法可以更加的简洁，自由




**局限性：**1、由于擦除的原因无法和数组很好的混合使用

2、基本类型无法作为类型参数，需要使用包装类

3、由于擦除的原因，泛型不能用于显式地引用运行时类型操作中，比如  instanceof和new表达式------>> T var=new T();  //error

4、由于擦除的原因，重载不识别泛型，如
```java
//不能编译
class A<K,V>{
     void f(K x){}
     void f(V x){}
}
```
5、基类劫持了接口，比如


```java
class Fruit implements Comparable<Fruit>{

	@Override
	public int compareTo(Fruit o) {
		// TODO Auto-generated method stub
		return 0;
	}
}

//编译通不过
// class Apple extends Fruit implements Comparable<Apple>{}

//编译能通过
class Apple extends Fruit implements Comparable<Fruit>{}
```
6、异常的cath语句不能捕获泛型类型的异常


```java
interface A<T, E extends Exception> {
	void process(List<T> list) throws E; // 该方法向上抛出异常T
}

class Runner<T, E extends Exception> extends ArrayList<A<T, E>> {
	List<T> processAll() throws E {
		List<T> result = new ArrayList<T>();
		for (A<T, E> a : this)
			a.process(result);
		return result;
	}
}

class myException extends Exception {
}

class B implements A<Integer, myException> {

	@Override
	public void process(List<Integer> list) throws myException {
		throw new myException();//抛出异常
	}

}

public class Test {
	public static void main(String[] args) {

		try {
			Runner<Integer,myException> runner=new Runner<Integer,myException>();
			System.out.println(runner.processAll());
		} catch (myException e) {
			System.out.println("xxx");//异常未捕获
		}
	}
}
```



**注意：**1、对于一个泛型类在定义时不能使用无限制通配符? 和super关键字


```java
class Fruit {
}

class Apple extends Fruit {
}

// class A<?>{} //无法编译
// class A<? extends Fruit>{}//无法编译
// class A<T super Apple>{} //无法编译

class A<T extends Fruit> {
	void x(List<? super Apple> x) {
		System.out.println(x.get(0).getClass().getName());
	}
}

public class Test {
	public static void main(String[] args) {
		A<Fruit> a = new A<Fruit>();
		List<Apple> list = new ArrayList<Apple>();
		list.add(new Apple());
		a.x(list);// Num_6.Apple
		List<Fruit> list2 = new ArrayList<Fruit>();
		list2.add(new Fruit());
		a.x(list2);// Num_6.Fruit
	}
}
```



2、如果一个类不是泛型的，但在方法中有泛型的参数，则该方法必须是泛型方法（不管是不是静态方法，泛型方法见最后一条）


```java
class A {
	// void f(T x);//无法编译
	<T> void h(T x) {
	}
}
```







### 泛型的擦除：

容器运行时实际上是原生态，即List<Integer>--->List，这样设计是因为对以前版本的兼容（泛型1.5才加入）

在泛型代码内部，无法获得任何有关泛型参数类型的信息


```java
Class c1=new ArrayList<Integer>().getClass();
		Class c2=new ArrayList<String>().getClass();
		System.out.println(c1==c2);//true
		System.out.println(c1.getName());//java.util.ArrayList
```


我们可以利用泛型擦除的这种特性，实现一个泛型单例模式，创建不可变但又适合于许多不同类型的对象，Demo：


```java
/**
 * 该接口有一个方法接受和返回某个类型T的值
 * 
 * @author he
 *
 * @param <T>
 */
interface UnaryFunction<T> {
	T apply(T arg);
}

public class Test {

	private static UnaryFunction<Object> IDENTITY_FUNCTION = new UnaryFunction<Object>() {

		@Override
		public Object apply(Object arg) {
			return arg;
		}
	};

	// 由于编译器的类型推导可以知道T的准确类型
	@SuppressWarnings("unchecked")
	public static <T> UnaryFunction<T> identityFuction() {
		return (UnaryFunction<T>) IDENTITY_FUNCTION;
	}

	//只有一个UnaryFunction的实例
	public static void main(String[] args) {
		String s[] = { "A", "B", "C" };
		UnaryFunction<String> u1 = identityFuction();
		for (String t : s)
			System.out.println(u1.apply(t));// A B C

		Integer t[] = { 1, 2, 3 };
		UnaryFunction<Integer> u2 = identityFuction();
		for (int i : t)
			System.out.println(u2.apply(i));// 1 2 3

	}
}
```





### 泛型的边界： 

? ：无限制通配符

?  extends T ，T为上界

?   super   T ，T为下界

因为泛型的擦除，所以无界泛型参数调用的方法只能是那些可以用Object调用的方法，但是如果能够将这个参数限制为某个类型子集，那么就可以用子集调用方法（见三）







### 一、泛型和数组

**泛型和数组的区别：**

1、**数组是协变的**：如果类A是类B的子类型，那么A[ ]就是B[ ]的子类型。**泛型是不可变的**，对于任意的两个不同的类型A,B，LIist<A>既不是List<B>的子类型也不是List<B>的超类型

2、**数组是具体化的**，因此数组会在运行时才知道并检查它们的元素类型约束，**泛型则是通过擦除实现的**，因此泛型只是在编译时强化它们的类型信息，并在运行时擦除元素类型信息


因此数组提供了运行时的类型安全，但没有编译时的类型安全，泛型提供了编译时的安全但没有运行时的类型安全因此泛型和数组不能很好的混用


**但是！！！**说数组不能使用泛型是不准确的，声明一个泛型数组是没问题的，见下面的demo：


```java
package Num_6;

import java.lang.reflect.Array;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.Set;

public class GenericArray<T> {
	private T array[];//声明一个泛型数组未报错
	private int count;

	public GenericArray(int N) {
		array=(T[]) new Object[N];//转型，对于ArrayList内部就存在大量的像这样的转型
	}
	
	
	public void put(T item) {
		array[count++] = item;
		count++;
	}

	public T get(int index) {
		return array[index];
	}

	public T[] pre() {
		return array;
	}

	public static void main(String[] args) {
		GenericArray<Integer> g = new GenericArray<Integer>( 10);
		g.put(1);//未出错
		System.out.println(g.get(0));//未出错
	<span style="white-space:pre">	</span>Integer[] tt = g.pre();// 报错，类型转换错误
	}

}
```
上面的代码中“泛型数组”被创建了，这是因为泛型的擦除只会运行在编译期，因此可以直接创建一个Object数组并对其转型，上面的put和get方法是没问题的，但是当你想调用pre返回T[ ]数组时会出错（即使数组array确实是一个Integer类型的数组），还是因为擦除的原因，运行时是一个Object数组编译器并不知道实际的数组类型，如果将数组确切的类型信息传入构造器就可以从擦除中恢复出来，将上面代码中的构造器改为


```java
public GenericArray(Class<T> c, int N) {
		array = (T[]) Array.newInstance(c, N);
	}
```
则
```java
Integer[] tt = g.pre();//正确
```


### 

### 二、无限制通配符类型<?>

List<?> 和List的区别：


1、List<?>是安全的，List则不安全

2、可以将任何元素放入List中，不能将任何元素（除了null）放入List<?> 类型中


Demo:


```java
public void f(List<?> list){
		list.add(null);//可以
		//list.add(list.get(0));//编译通不过
		System.out.println(list.get(0).getClass().getName());
	}
```

```java
<span style="white-space:pre">	</span>        LinkedList<Integer> list = new LinkedList<Integer>();
		list.add(1);
		list.add(2);
		g.f(list);//java.lang.Integer
		LinkedList<String> list2 = new LinkedList<String>();
		list2.add("A");
		list2.add("B");
		g.f(list2);//java.lang.String
```

**补充部分：**如果有一个方法的参数类型使用了List<?> ，比如下面的一个方法


```java
//该方法交换list中index为i和j对应元素
	static void swap(List<?> list,int i,int j){
		
	}
```
显然list是不能直接添加非null的元素的，但是我们可以借助一个辅助方法对无限制通配符进行类型捕捉，辅助方法必须是泛型方法



```java
public class Test {
	//该方法交换list中index为i和j对应元素
	static void swap(List<?> list,int i,int j){
		swapHelper(list, i, j);
	}
	//辅助方法必须是泛型的
	private static <E> void swapHelper(List<E> list,int i,int j){
		E e =list.get(i);
		list.set(i, list.get(j));
		list.set(j, e);
	}
	
	public static void main(String[] args) {
		List<String> list=new ArrayList<String>();
		list.add(0, "A");
		list.add(1,"B");
		swap(list, 0, 1);
		System.out.println(list.get(0));//B
		System.out.println(list.get(1));//A
	}
		
}
```




### 三、有限制类型参数<E extends Number>

E必须具有类型Number或是Number的子类，这样就给擦除定了一个边界，效果如下

### 


```java
package Num_6;

class Hasf {
	public void f() {
		System.out.println("Hasf.f()");
	}
}

class HasfSon extends Hasf {
	public void f() {
		System.out.println("HasfSon.f()");
	}
}

//class Mani<T> {
//	private T obj;
//
//	public Mani(T x) {
//		obj = x;
//	}
//
//	// 此方法，无法编译，Java编译器无法将obj映射到Hasf拥有f()这一事实上
//	public void man() {
//		obj.f();
//	}
//}

class Mani<T extends Hasf> {
	private T obj;

	public Mani(T x) {
		obj = x;
	}

	// 可以编译，泛型T会擦除到Hasf
	public void man() {
		obj.f();
	}
}



public class Test {
	public static void main(String[] args) {
		Hasf hasf=new HasfSon();
		Mani<Hasf> mani=new Mani<Hasf>(hasf);//HasfSon.f()
		mani.man();
	}
}
```







### 四、递归类型限制<T extends Comparable<T>>

1、自限定类   class Self<T extends Self<T>>{ }，保证泛型必须是Self的子类


```java
class Fruit {}

class Self<T extends Self<T>>{}//只能使用Self的子类作为泛型，Self类也不行
class A extends Self<A>{} //A 不是泛型类，可以编译
class B extends Self<A>{}//B不是泛型类，可以编译
class C extends A{}

public class Test {
	public static void main(String[] args) {
	//	Self<Fruit> self=new Self<Fruit>();//无法编译
	//	Self<Self> self=new Self<Self>();//无法编译
		Self<A> self=new Self<A>();
		
		
	}
}
```
2、自限定方法   static <T extends Self<T>> T f(T arg){ }


```java
class Fruit {
}

class Self<T extends Self<T>> {
}// 只能使用Self的子类作为泛型，Self类也不行

class A extends Self<A> {
} // A 不是泛型类，可以编译

class B extends Self<A> {
}// B不是泛型类，可以编译

class C extends A {
}

public class Test {

	static <T extends Self<T>> T f(T arg) {
		return arg;
	}

	public static void main(String[] args) {
		// Self<Fruit> self=new Self<Fruit>();//无法编译
		// Self<Self> self=new Self<Self>();//无法编译
		Self<A> self = new Self<A>();
		A a=f(new A());
		//Fruit fruit=f(new Fruit());//无法编译
		//B b=f(new B());//无法编译
	}
}
```







### 五、有限制通配符类型<? extends Number>

Number 为上界，可以解释为具有任何从Number继承的类型的列表，通配符?的引用是明确类型，但是不能添加元素（除了null）

### 


```java
class Fruit {
}

class Apple extends Fruit {
}

class Orange extends Fruit {
}

public class Test {
	public static void main(String[] args) {
		//对于数组
		Fruit []f=new Apple[10];
		f[0]=new Apple();//数组f只能添加Apple类型或者Apple的子类型
		f[1]=new Fruit();//虽然编译时不报错，但运行时出错，java.lang.ArrayStoreException，因为数组是具体化的在运行时检查类型
		//对于泛型容器
		//List< Fruit> list = new ArrayList<Apple>();//无法通过编译
		List<? extends Fruit> list=new ArrayList<Apple>();//可以编译，但是只能添加null
		list.add(null);//可行
		//list.add(new Apple());//编译通不过
	}
}
```




****










### 六、超类通配符，<? super T> or <? super Number>  ，Number为下界 ，<T super Number>(错误)

这样可以添加Number类及其子类型，比如List<? super Apple>，表示的含义就是，List指定的泛型是Apple的超类，作为Apple的超类，自然能添加Apple类及其子类



```java
class Fruit {
}

class Apple extends Fruit {
}

class Apple2 extends Apple{
	
}

class Orange extends Fruit {
}

public class Test {
	public static void main(String[] args) {
	//	ArrayList<? super Fruit> list=new ArrayList<Apple>();//不能编译,因为Fruit为下界
		ArrayList<? super Apple> list=new ArrayList<Fruit>();//可以编译，使用super关键字，可以添加Apple类型以及其子类
		list.add(new Apple());
	//	list.add(new Fruit());//不能编译
		list.add(new Apple2());
		list.add(null);//可行
	}
}
```






**PESC规则：**PESC表示producer-extends，consumber-super，如果参数化类型T表示生产者就是使用<? extends T>，如果参数化泛型T表示消费者就使用<? super T>

生产者可以简单的理解为自己的东西给别人用，消费者简单的理解为使用别人的东西，如下面的Demo是一般的泛型方法


```java
// 该方法求list列表中的最大值
	static <T extends Comparable<T>> T max(List<T> list) {
		Iterator<T> i = list.iterator();
		T result = i.next();
		while (i.hasNext()) {
			T t = i.next();
			if (t.compareTo(result) > 0) {
				result = t;
			}
		}
		return result;
	}
```


下面的Demo是在上面的基础上运用的PESC规则，参数list是生产者，Comparable是消费者，**注意，Comparable永远都是消费者**


```java
// 该方法求list列表中的最大值
	static <T extends Comparable<? super T>> T max(List<? extends T> list) {
		Iterator<? extends T> i = list.iterator();
		T result = i.next();
		while (i.hasNext()) {
			T t = i.next();
			if (t.compareTo(result) > 0) {
				result = t;
			}
		}
		return result;
	}
```






### 七、泛型方法 static <T> void f(T x){}

泛型方法是在返回值前面写上泛型参数列表，是否拥有泛型方法与其所在的类是否是泛型无关，泛型方法的显著特点是无需知名类型参数的值，编译器通过类型推导就可知道类型参数的具体类型

对于static的方法而言，无法访问泛型类的类型参数，非static方法可以使用，请看下面的代码




```java
public class Test<T> {
	T age;

	static void h(T x){//编译通不过
		
	}
	
	static <T> void f(T x) {
		System.out.println(x.getClass().getName());
	}

	void g(T x) {
		System.out.println();
	}

}
```




# **利用泛型实现类型安全的异构容器**

异构容器：所有键都是不同类型的容器，将键参数化而不是将容器参数化

**局限性：**key可以是String，String[ ] ，但不能是List<String>，因为List<String>.class语法错误

```java
class Favorites {
	// Map的key是参数化的使用的是无限制通配符 ?，因此Map中可以添加元素
	private Map<Class<?>, Object> favorites = new HashMap<Class<?>, Object>();

	 
	/**
	 * type为类型令牌
	 * 这种写法 Map中的键和值的类型没有对于关系，不能保证每个值的类型都是键的类型
	 * 比如键为String类型，值为1,这样将导致在取出元素时出现类型转换异常，java.lang.ClassCastException
	 * 解决办法见方法 putFavorite2
	 * @param type
	 * @param instance
	 */
	public <T> void putFavorite(Class<?> type, T instance) {
		if (type == null) {
			throw new NullPointerException("type is null");
		}
		favorites.put(type, instance);
	}

	/**
	 * 这样无法将key和value类型不同的情况添加到容器中
	 * @param type
	 * @param instance
	 */
	public <T> void putFavorite2(Class<?> type, T instance) {
		if (type == null) {
			throw new NullPointerException("type is null");
		}
		favorites.put(type, type.cast(instance));//如果类型转换错误，直接抛出异常
	}
	
	
	
	
	public <T> T getFavorite(Class<T> type) {
		return type.cast(favorites.get(type));//将Object对象动态的转换成了Class对象所表示的类型
	}

}

public class Test {
	public static void main(String[] args) {
		Favorites f=new Favorites();
		f.putFavorite(Integer.class, 1);
		f.putFavorite(Character.class, 'A');
		f.putFavorite(Class.class,Favorites.class);
		f.putFavorite(String.class, 1);//键和值的类型不同，可以添加
		f.putFavorite(String[].class, new String[]{"A","B"});
	//	f.putFavorite2(String.class, 1);//将类型转换异常阻止在了添加时
		System.out.println(f.getFavorite(Character.class));//A
		System.out.println(f.getFavorite(String[].class));//返回内存地址，[Ljava.lang.String;@4e25154f
	//	System.out.println(f.getFavorite(String.class));//java.lang.ClassCastException
		
	//	f.putFavorite(List<Sting>.class, new ArrayList<String>());//无法添加，因为List<String>.class语法错误
		
	}

}
```

















