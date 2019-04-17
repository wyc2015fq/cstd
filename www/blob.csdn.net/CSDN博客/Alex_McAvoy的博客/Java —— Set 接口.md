# Java —— Set 接口 - Alex_McAvoy的博客 - CSDN博客





2018年10月19日 19:45:58[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：39
个人分类：[Java学习笔记](https://blog.csdn.net/u011815404/article/category/8863016)

所属专栏：[Java 学习笔记](https://blog.csdn.net/column/details/34317.html)









# 【概述】

Set 集合为集类型，集是最简单的一种集合，存放于集中的对象不按特定方式排序，只是简单地把对象加入集合中，类似向口袋中放东西。

Set 中的对象是不可重复的，存放的对象的访问和操作是通过对象的引用进行的，所以在集中不能存放重复对象。对象的存放也是无序的，但无序性不等于随机性，在这里无序性指的是元素在底层存储的位置是无序的，其是根据哈希值来确定存储位置。

由于 Set 的不可重复性、无序性，因此向其中添加自定义对象时，需要重写 equals() 方法、hashCode() 方法。

Set 接口的常见实现类有 HashSet、LinkedHashSet、TreeSet 在使用 Set 集合时，通常情况下，声明为 Set 类型，实例化时，根据实际需要，实例化为 HashSet、LinkedHashSet、TreeSet

```java
//利用HashSet类实例化集合
Set<类类型> 集合名 = new HashSet<类类型>();
//利用LinkedHashSet类实例化集合
Set<类类型> 集合名 = new LinkedHashSet<类类型>();
//利用TreeSet类实例化集合
Set<类类型> 集合名 = new TreeSet<类类型>();
//当省略参数 <类类型> 时，是一原始类型，其中可存储任何类型的元素
Set 集合名 = new HashSet();
```

# 【Set 存储元素机制】

向 Set 中存储元素时，使用了哈希算法。

当向 Set 中添加对象时，首先调用此对象所在类的 hashCode() 方法，计算此对象的哈希值，此哈希值决定了此对象在 Set 中的存储位置。

若计算出的位置之前没有对象存储，则这个对象直接存储到该位置；若此位置已有对象存储，再通过 equals() 方法比较这两个对象是否相同，如果相同，则后一对象不能添加到 Set 中。

hashCode() 方法的重写格式如下，其采用了迭代的方法，通过计算每个对象属性的哈希值，迭代后从而算出对象的哈希值。

```java
@Override
public int hashCode(){
    final int prime=31;
    int result=1;
    result=prime*result+( (age==null)?0:age.hashCode() );
    result=prime*result+( (name==null)?0:name.hashCode() );
    return result;
}
```

# 【HashSet 类】

由 HashSet 类实现的 Set 集合的优点是能快速定位集合中的元素，其是最常用的实例化类。

由于 Set 接口实现了 Collectoin 接口，因此 HashSet 类的常用方法都是 Collection 接口提供的所有常用方法。

# 【LinkedHashSet】

LinkedHashSet 是继承于 HashSet 类的，其本质上是使用链表，从而维护对象添加进集合的顺序，因此遍历集合中的元素时，是按照添加时的顺序遍历的，需要注意的是，虽然遍历时是有序的，但在底层存储仍是按照哈希值计算的。

LinkedHashSet 具备 HashSet 类快速定位集合中对象的优点，又让集合中的对象按插入的顺序保存，但当向其中加入新元素时，较为缓慢，因为其在插入元素的同时还在维护链表。

# 【TreeSet 类】

TreeSet 类最大特点是存储的对象必须是同一类型的，且在对象类型是 String 类、包装类时，遍历集合按照字典序获得对象。

TreeSet 类不仅实现了 Set 接口，还实现了 java.util 包下的 SortedSet 接口，从而保证在遍历集合时按照递增顺序获得对象。

当 TreeSet 存储自定义类对象时，有以下两种排序方式

## 1.自然排序

要求自定义类实现 java.lang 包下的 Comparabel 接口，并重写其 compareTo(Object obj) 方法，在此方法中，可以指明自定义类的排序方式。

```java
@Override
public int compareTo(Object obj) {    //重写compareTo()方法
	if(obj instanceof Person) {
		Person p=(Person)obj;
		return this.name.compareTo(p.name);
	}
	return 0;
}
```

要注意的是，向 TreeSet 中添加元素时，首先按照 compareTo() 方法进行比较，进而再是 hashCode() 方法以及 equals() 方法，若按照上述方法重写 compareTo() 方法，一旦返回 0，虽然仅是两个对象的属性值相同，但程序会认为两个对象是相同的，从而无法添加进 TreeSet 中，因此在使用自然排序时，要保证 compareTo()、hashCode()、equals() 方法一致。.

修改后的 compareTo() 方法如下

```java
@Override
public int compareTo(Object obj) {
	if(obj instanceof Person) {
		Person p=(Person)obj;
		int temp=this.name.compareTo(p.name);
		if(temp==0)
			return this.age.compareTo(p.age);
		else
			return temp;
	}
	return 0;
}
```

## 2.定制排序

当按照指定比较器递增排列时，可以通过比较器对由 TreeSet 类实现的 Set 集合中的对象进行排序。

其具体步骤为：创建一个实现了 Copmarator 接口的类对象，将对象作为形参传入 TreeSet 的构造器中，向 TreeSet 中添加 Comparable 接口中的 compare() 方法中涉及的类的对象。

使用定制排序时，要保证 compare()、hashCode()、equals() 方法一致。.

```java
//创建实现Copmarator接口的类对象
Comparator com=new Comparator() {    
	@Override
	public int compare(Object obj1,Object obj2) {
		if(obj1 instanceof Person && obj2 instanceof Person) {
			Person p1=(Person)obj1;
			Person p2=(Person)obj2;
			int temp=p1.name.compareTo(p2.name);
			if(temp==0)
				return p1.age.compareTo(p2.age);
			else
				return temp;
		}
		return 0;
	}
};

//将对象作为形参传入TreeSet构造器中
TreeSet set =new TreeSet(com);

//向TreeSet中添加Comparable接口中的compare()方法中涉及的类的对象
set.add(new Person("AA",123));
...
```





