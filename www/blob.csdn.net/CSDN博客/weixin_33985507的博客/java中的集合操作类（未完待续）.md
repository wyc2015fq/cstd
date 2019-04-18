# java中的集合操作类（未完待续） - weixin_33985507的博客 - CSDN博客
2017年08月01日 17:14:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2

# 申明：
> 
实习生的肤浅理解，如发现有错误之处。还望大牛们多多指点
# 废话
> 
事实上我写java的后台操作，我每次都会遇到一条语句：List<XXXXX> list = new ArrayList<XXXXX>();
可是我只不过了解，list这个类是一个可变长用来存储的对象实例的类。我甚至认为这个List对象能够理解成数组。可是却又与java中咱们正常理解的数组非常多的不同。比方说。他的长度能够随着须要自己主动增长，比方说，实例化一个List类就和咱们声明数组的时候是不一样的。
今天的实习生活过的有点枯燥，我就打开了eclipse。看同事们写的代码，无意间又看到了这句话，所以决定学习一下这一类的操作——java中的容器类（集合类）
# 正文
> 
先偷个图来吧，我不会告诉你我是在百度百科里面偷的
> 
![](https://img-blog.csdn.net/20140826201041029?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjQ1MzYxOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
怎样。我们能够看出。java中的集合操作主要有两大类：Collection集合与Map映射
## Collection
Collection是一个顶层的接口。他是java集合的一个抽象，在此基础上派生出了两个子接口。分别为：List与Set
> 
> 
### List
> 
> 
List接口。特点有序的，所存储的对象能够为null。而且同意反复，这点与set有非常大差别，Set是不同意反复的。List非常类似于数组，对于List对象的訪问能够通过类似于数组下标去訪问，可是是通过get(int index)方法进行訪问的，同一时候事实上际长度也非常类似于数组，能够通过size()返回事实上际长度。除了具有Collection接口必备的iterator()方法外，List还提供一个listIterator()方法。返回一个
 ListIterator接口，和标准的Iterator接口相比，ListIterator多了一些add()之类的方法，同意加入，删除，设定元素，还能向前或向后遍历。
list常见方法：
`**[add]()**([E]() e)` ：向列表的尾部加入指定的元素
`**[add]()**(int index,[E]() element)` ： 在列表的指定位置插入指定元素
`**[addAll]()**([Collection]()<?`
` extends[E]()> c)`
`` ：加入指定
 collection 中的全部元素到此列表的结尾，顺序是指定                                                                      collection 的迭代器返回这些元素的顺序
`**[addAll]()**(int index,[Collection]()<?`
```
extends
[E]()> c)
```
`` ：
                          将指定 collection 中的全部元素都插入到列表中的指定位置与数组相比較。list的查询效率                             高。可是插入和删除的效率较低，由于插入和删除会引起其它元素的位置变化
`**[clear]()**()` ：从列表中移除全部元素
`**[contains]()**([Object]() o)` ：假设列表包括指定的元素。则返回true
`**[containsAll]()**([Collection]()<?> c)` ：假设列表包括指定 collection
 的全部元素。则返回true
`**[get]()**(int index)` ： 返回列表中指定位置的元素
`**[indexOf]()**([Object]() o)` ：返回此列表中第一次出现的指定元素的索引。假设此列表不包括该元素。则返回 -1
`**[iterator]()**()` ： 返回按适当顺序在列表的元素上进行迭代的迭代器
`**[lastIndexOf]()**([Object]() o)` ：返回此列表中最后出现的指定元素的索引。假设列表不包括此元素。则返回 -1
`**[listIterator]()**()` ：返回此列表元素的列表迭代器（按适当顺序）
`**[listIterator]()**(int index)` ：返回列表中元素的列表迭代器（按适当顺序），从列表的指定位置開始
`**[remove]()**(int index)` ： 移除列表中指定位置的元素
`**[remove]()**([Object]() o)` ：从此列表中移除第一次出现的指定元素
`**[removeAll]()**([Collection]()<?`
`> c)`
`` ： 从列表中移除指定 collection 中包括的其全部元素
`**[retainAll]()**([Collection]()<?> c)` ： 仅在列表中保留指定 collection 中所包括的元素
`**[set]()**(int index,[E]() element)` ：用指定元素替换列表中指定位置的元素
`**[size]()**()` ：返回列表中的元素数
`**[subList]()**(int fromIndex,int toIndex)`返回列表中指定的fromIndex（包含 ）和
toIndex（不包含）之间的部分视图
`**[toArray]()**()` ：返回按适当顺序包括列表中的全部元素的数组
具体方法信息还是參考一下API里面的吧
实现list接口的主要有：ArrayList、LinkedList、Vector和Stack
### ArrayList
通过名字就能知道这是个啥了吧，灰常类似于数组
ArrayList实现了可变大小的数组。
它同意全部元素，包含null。
ArrayList没有同步，仅有Vector实现了同步(线程安全)。每一个ArrayList实例都有一个容量（Capacity）。即用于存储元素的数组的大小。默认值好像是10。
这个容量可随着不断加入新元素而自己主动添加，可是增长算法并未定义。当须要插入大量元素时。在插入前能够调用ensureCapacity方法（设定长度最小值）来添加ArrayList的容量以提高插入效率。和LinkedList一样。ArrayList也是非同步的（unsynchronized）
#### 案例：
```java
package test;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
public class Test {
	public static int flg = 0;
	public int self = 0;
	public Test() {
		++flg;
		self = flg;
		System.out.println("已经初始化了:" + flg + "个实例");
	}
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		/*
		 * 加入范式Test表示这里存储Test类型，而且能够获得详细值之后，能够訪问对象的方法和属性 比方下述：list.get(i).flg
		 * 若不加入范式，则无法訪问该self属性或者其它方法
		 */
		List<Test> list = new ArrayList<Test>();
		for (int i = 0; i < 5; i++) {
			list.add(new Test());
			System.out.println(list.get(i).self);
		}
		list.add(new Test());
		System.out.println(list.get(list.size() - 1).self);
		list.addAll(list);
		System.out.println(list.size());
		list.add(0, null);
		System.out.println(list.get(0));
		// 使用迭代器去遍历，注意给迭代器加入一个泛型
		Iterator<Test> myiterator = list.listIterator();
		Test test;
		while (myiterator.hasNext()) {
			test = myiterator.next();
			if (test != null)
				System.out.println(test.self);
			else
				System.out.println(test);
		}
	}
}
```
> 
> 
### LinkedList
LinkedList实现了List接口，同意null元素。
此外LinkedList提供额外的get。remove，insert方法在 LinkedList的首部或尾部。这些操作使LinkedList可被用作堆栈（stack），队列（queue）或双向队列（deque）,该类实现了Deque接口。提供了add和poll方法实现FIFO先进先出的队列操作。一起其它的堆栈和双向队列操作。
注意LinkedList没有同步方法。假设多个线程同一时候訪问一个List，则必须自己实现訪问同步。一种解决方法是在创建List时构造一个同步的List： 
List list = Collections.synchronizedList(new LinkedList(...));
案例：
> 
> 
基础操作
```java
<span style="font-size:14px;">package test;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
public class Test {
	public static int flg = 0;
	public int self = 0;
	public Test() {
		++flg;
		self = flg;
		System.out.println("已经初始化了:" + flg + "个实例");
	}
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		/*
		 * 加入范式Test表示这里存储Test类型，而且能够获得具体值之后，能够訪问对象的方法和属性 比方下述：list.get(i).flg
		 * 若不加入范式，则无法訪问该self属性或者其它方法
		 */
		List<Test> list = new LinkedList<Test>();
		int i = 0;
		Test test = new Test();
		list.add(0, test);
		while (i < 5) {
			list.add(new Test());
			i++;
		}
		System.out.println("indexOf :" + list.indexOf(test));
		System.out.println("lastIndexOf :" + list.lastIndexOf(test));
		System.out.println("contants :" + list.contains(test));
		System.out.println("contants :" + list.contains(new Test()));
		// 注意迭代器的使用。给迭代器指定正确的泛型，才干够訪问具体的属性和方法
		Iterator<Test> iterator = list.listIterator();
		while (iterator.hasNext()) {
			System.out.println(iterator.next().self);
		}
	}
}</span>
```
> 
> 
至于大家都说了LinkedList能够当做队列或者堆栈，从表面上，LinkedList实现了Deque等接口，确实能够这么去用了。可是我在实践的过程中。我得到的LinkedList的实例，不知道怎么情况，没有api里面给出的poll方法，所以我这里没有做关于这个的案例的，可是模拟还是非常easy的
> 
> 
### Vector
Vector类似于ArrayList，可是确实同步的，也就是说他是线程安全的，换句话说就是假设Vector创建了一个迭代器。那么这个迭代器正在使用的时候，还有一个线程改变了这个Vector的状态。那么就会产生异常
使用方法接近ArrayList。參照上述以及API就可以
### Stack
栈这个东西。在数据结构里面学过的，曾经非常熟悉。但是如今我就陌生了。大概的知道这个有一个特性：LIFO后进先出。在java中Stack是基于Vector的，除了主要的方法push()和pop()方法，java的Stack还提供了一个peek()获取栈顶元素值的方法，empty方法推断是否为空。search方法寻找某个元素在栈中的位置等
既然实现了List接口。那么就会有list的一些操作方法
简单案例：
```java
package test;
import java.util.Stack;
public class Test {
	public static int flg = 0;
	public int self = 0;
	public Test() {
		++flg;
		self = flg;
		System.out.println("已经初始化了:" + flg + "个实例");
	}
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		Stack stack = new Stack();
		for (int i = 0; i < 5; i++)
			stack.push(i);
		System.out.println(stack.peek());
		System.out.println(stack.size());
		int len = stack.size();
		for (int i = 0; i < len; i++)
			System.out.println(stack.pop());
		System.out.println(stack.size());
	}
}
```
> 
> 
### Set
> 
> 
Set是一种不可反复的Collection接口，set的检索效率相对较低，而与list对照。插入和删除的效率较高，不会引起其它元素位置的变化，Set是无序的。Set和List最大的差别是Set不同意反复
我们重点要记住这家伙不能反复
Set不同意反复的原因是，他是基于Map实现的，而Map中的key是不同意反复的，Set中使用的不过Map中的key
Set常见的方法。具体情况请參考Java语言的API
[add]()([E]() e) ：假设 set 中尚未存在指定的元素。则加入此元素
[addAll]()([Collection]()<? extends[E]()> c) ：假设 set 中没有指定
 collection 中的全部元素，则将其加入到此 set 中
[clear]()() ：移除此 set 中的全部元素
[contains]()([Object]() o) ： 假设 set 包括指定的元素。则返回true
[containsAll]()([Collection]()<?
> c) ：假设此 set 包括指定 collection 的全部元素。则返回true
[equals]()([Object]() o) ：比較指定对象与此 set 的相等性
[isEmpty]()() ：假设 set 不包括元素，则返回true
[iterator]()() ：返回在此 set 中的元素上进行迭代的迭代器
[remove]()([Object]() o) ： 假设 set 中存在指定的元素，则将其移除
[removeAll]()([Collection]()<?> c) ：移除 set 中那些包括在指定 collection 中的元素
[retainAll]()([Collection]()<?
> c) ： 仅保留 set 中那些包括在指定 collection 中的元素
[size]()() ：返回 set 中的元素数
[toArray]()() ：返回一个包括 set 中全部元素的数组
比較蛋疼，set中是没有get方法获取元素对象的，仅仅能通过迭代器来訪问数据
我们常见的Set实现类有：TreeSet、HashSet和LinkedHashSet
### TreeSet
TreeSet竟然是有序的，默认是自然序列的。排序方式能够使用自带的。也能够使用自己定义，在创建TreeSet的时候，传递一个自己定义的排序规则对象就可以，另外既然它是有序的。有是基于TreeMap实现的。那么TreeSet中的元素对象必须实现Comparable接口。可是假设是字符串对象的话。呵呵，不须要了，String本身就已经实现了Comparable接口了
非常显然。在測试过程中没有发现get方法，看到了iterator方法，那么也就意味着，仅仅能通过迭代器訪问具体数据了
简单案例：
须要实现Comparable接口（String不须要。String已经实现了，只是你能够又一次写一个。用自定义的）
```java
package test;
import java.util.Iterator;
import java.util.Set;
import java.util.TreeSet;
public class Test implements Comparable<Test> {
	public static int flg = 0;
	public int self = 0;
	public Test() {
		++flg;
		self = flg;
		System.out.println("已经初始化了:" + flg + "个实例");
	}
	@Override
	public int compareTo(Test o) {
		// TODO Auto-generated method stub
		if (this.self < o.self)
			return -1;
		else if (this.self > o.self)
			return 1;
		else
			return 0;
	}
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		Set set = new TreeSet();
		for (int i = 0; i < 5; i++)
			set.add(new Test());
		System.out.println(set.size());
		Iterator<Test> it = set.iterator();
		while (it.hasNext()) {
			System.out.println(it.next().self);
		}
	}
}
```
> 
> 
### HashSet
和TreeSet一样仅仅能通过迭代器来訪问数据，HashSet是基于HashMap实现的，可是是无序的，经常使用的方法和Set接口的相近，记住一点，訪问必须通过迭代器，迭代的顺序不能保证是和插入的顺序一致，这点与LinkedHashSet正好是相反的
案例：有待补充
### LinkedHashSet
> 
> 
LinkedHashSet是一个链表结构。与上述一致，通过迭代器遍历，但遍历的顺序和插入的顺序一致
> 
> 
案例：有待补充
> 
## **Map**
> 
> 
（事实上我接触Map应该是从学习Hibernate课堂上。老师说过。学java的时候，老师讲到基本循环结构就结束课程了，要命，呵呵）
> 
Map是一个顶层接口。主要是处理映射（key/value）类型，用于存储键值对的
> 
Map接口提供了三种Collection视图，同意以键集、值集或者键值对映射关系集的方式来訪问某个映射的内容，迭代的顺序定义为迭代器在collection视图上返回的元素顺序，有些Map实现能够确保顺序。比方TreeMap，但有些是不能够的。比方HashMap
> 
构造方法：
> 
构造方法一般有两种：一种是无參数，还有一种是带一个Map类型的參数
> 
经常用法：put方法，加入键值对，这里对于Map的介绍。还是以样例为主吧。
> 
实现Map接口的主要有TreeMap、HashMap等
### TreeMap
基于红黑树（Red-Black tree）的 [NavigableMap]() 平衡二叉树实现。
该映射依据其键的[自然顺序]()进行排序，或者依据创建映射时提供的
[Comparator]() 进行排序。详细取决于使用的构造方法。
构造函数：
[TreeMap]()() ：使用键的自然顺序构造一个新的、空的树映射
[TreeMap]()([Comparator]()<? super
[K]()> comparator) 
            构造一个新的、空的树映射，该映射依据给定比較器进行排序
[TreeMap]()([Map]()<?
 extends
[K](),? extends [V]()> m) 
           构造一个与给定映射具有同样映射关系的新的树映射。该映射依据其键的自然顺序 进行排序
[TreeMap]()([SortedMap]()<[K](),? extends
[V]()> m) 
           构造一个与指定有序映射具有同样映射关系和同样排序顺序的新的树映射
经常用法
[]()[Map.Entry]()<[K](),[V]()>  ceilingEntry([K]() key)
          返回一个键-值映射关系实体，与大于等于给定键的最小键关联。假设不存在这种键。则返回 null
[get]()([Object]() key) ：返回指定键所映射的值，假设对于该键而言，此映射不包括不论什么映射关系，则返回 null
[keySet]()() ：返回此映射包括的键的
[Set]() 视图
[navigableKeySet]()() ：返回此映射中所包括键的
[NavigableSet]() 视图
[values]()() ：返回此映射包括的值的
[Collection]() 视图
[put]()([K]() key,
[V]() value) ：将指定值与此映射中的指定键进行关联
[putAll]()([Map]()<?
 extends
[K](),? extends [V]()> map) ：将指定映射中的全部映射关系拷贝到此映射中
[remove]()([Object]() key) ：假设此 TreeMap 中存在该键的映射关系。则将其删除
```
```
### HashMap
基于哈希表的 Map 接口的实现。
此实现提供全部可选的映射操作，并同意使用 null 值和 null 键。（除了非同步和同意使用 null 之外，HashMap 类与 Hashtable 大致同样。
）此类不保证映射的顺序，特别是它不保证该顺序恒久不变。
此实现假定哈希函数将元素适当地分布在各桶之间，可为基本操作（get 和 put）提供稳定的性能。
迭代 collection 视图所需的时间与 HashMap 实例的“容量”（桶的数量）及其大小（键-值映射关系数）成比例。所以。假设迭代性能非常重要，则不要将初始容量设置得太高（或将载入因子设置得太低）
具体信息，还是API比較全，晚上看累了。不想写了
> 
> 
