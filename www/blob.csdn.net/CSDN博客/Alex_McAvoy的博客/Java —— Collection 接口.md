# Java —— Collection 接口 - Alex_McAvoy的博客 - CSDN博客





2018年10月18日 22:45:00[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：39
个人分类：[Java学习笔记](https://blog.csdn.net/u011815404/article/category/8863016)

所属专栏：[Java 学习笔记](https://blog.csdn.net/column/details/34317.html)









# 【概述】

Collection 接口是 List 接口和 Set 接口的父接口，通常情况下不被直接使用，但 Collection 接口定义了一些通用方法，通过这些方法可以实现对集合的基本操作，由于 List 接口和 Set 接口实现了 Collection 接口，因此这些方法对 List 接口和 Set 接口是通用的。

# 【Collection 接口的15个通用方法】

## 1.对集合的基础操作

### 1）add(Object obj) 方法

该方法用于将指定对象添加到当前集合中

```java
//创建集合
Collection coll = new ArrayList();

//添加对象到集合中
coll.add(123);
coll.add(456);
coll.add("abc");
coll.add("ABC");

//打印集合元素
System.out.println(coll); //运行结果：[123, 456, abc, ABC]
```

### 2）remove(Object obj) 方法

该方法用于删除集合中的指定元素，若删除成功则返回 true，否则返回 false

```java
Collection coll = new ArrayList();
coll.add(123);
coll.add(456);
System.out.println(coll);//结果为：[123, 456]

System.out.println(coll.remove(123));//结果为：true
System.out.println(coll);//结果为：[456]
System.out.println(coll.remove("abc"));//结果为：false
System.out.println(coll);//结果为：[456]
```

### 3）size() 方法

该方法用于获取集合中元素的个数

```java
Collection coll = new ArrayList();
coll.add(123);
coll.add(456);
System.out.println(coll.size());//结果为：2
```

### 4）contains(Object obj) 方法

该方法用于判断是否包含指定元素，若包含则返回 true，否则返回 false

判断依据：元素所在类的 equals() 方法

```java
Collection coll = new ArrayList();
coll.add(123);
coll.add(456);
		
Boolean res1=coll.contains(123);
Boolean res2=coll.contains("abc");
System.out.println(res1);//结果为：true
System.out.println(res2);//结果为：false
```

### 5）clear() 方法

该方法用于清空集合元素

```java
Collection coll = new ArrayList();
coll.add(123);
coll.add(456);
System.out.println(coll);//结果为：[123,456]
		
coll.clear();
System.out.println(coll);//结果为：[]
```

### 6）isEmpty() 方法

该方法用于判断当前集合是否为空，若为空则返回 true，若不为空则返回 false

```java
Collection coll = new ArrayList();
coll.add(123);
coll.add(456);
		
Boolean res1=coll.isEmpty();
coll.clear();
Boolean res2=coll.isEmpty();

System.out.println(res1);//结果为：false
System.out.println(res2);//结果为：true
```

### 7）equals(Object obj) 方法

该方法用于判断指定对象与当前对象是否为同一对象，若是则返回 true，否则返回 false，常用于判断两集合是否相等

```java
Collection coll1 = new ArrayList();
Collection coll2 = new ArrayList();
Collection coll3 = new ArrayList();
coll1.add(123);
coll1.add(456);
coll2.add(123);
coll2.add(456);
coll3.add(123);
coll3.add("abc");

Boolean res1=coll1.equals(coll2);
Boolean res2=coll1.equals(coll3);
	
System.out.println(res1);//结果为：true
System.out.println(res2);//结果为：false
```

### 8）iterator() 方法

用来序列化当前集合中的所有对象，返回一个 Iterator 接口实现类的实例，进而实现集合的遍历。

```java
Collection<Person> persons = new ArrayList<Person>();
Iterator<Person> iterator = persons.iterator();//获取当前集合的一个iterator实例
while(iterator.hasNext()){//当前集合序列不为空时 
    System.out.println(iterator.next());//输出下一个元素
}
```

关于 Iterator 接口：[点击这里](https://blog.csdn.net/u011815404/article/details/83187117)

## 2.对整个集合的操作

### 1）addAll(Collection coll) 方法

该方法用于将指定集合中的所有对象添加到当前集合中

```cpp
//创建集合
Collection coll1 = new ArrayList();
Collection coll2 = new ArrayList();

//分别添加对象至两集合中
coll1.add(123);
coll1.add(456);
coll2.add("abc");
coll2.add("ABC");

//分别输出两集合元素
System.out.println(coll1);//结果为：[123, 456]
System.out.println(coll2);//结果为：[abc, ABC]

//将集合coll2加入集合coll1中
coll1.addAll(coll2);

System.out.println(coll1);//结果为：[123, 456, abc, ABC]
```

### 2）removeAll(Collection coll) 方法

该方法用于从当前集合中删除包含在指定集合中的元素，即求两集合的差集

```java
Collection coll1 = new ArrayList();
Collection coll2 = new ArrayList();
coll1.add(123);
coll1.add(456);
coll1.add(789);
coll2.add(123);
coll2.add(456);
			
System.out.println(coll1);//结果为：[123, 456, 789]
System.out.println(coll2);//结果为：[123, 456]

coll1.removeAll(coll2);
System.out.println(coll1);//结果为：[789]
```

### 3）containsAll(Collection coll) 方法

判断当前集合中是否包含指定集合的所有元素，若全部包含则返回 true，否则返回 false

判断依据：元素所在类的 equals() 方法

若存入集合的元素是自定义类对象时，自定义类需要重写 equals() 方法

```java
Collection coll1 = new ArrayList();
Collection coll2 = new ArrayList();
Collection coll3 = new ArrayList();
coll1.add(123);
coll1.add(456);
coll2.add(123);
coll2.add("abc");
coll3.add(123);
coll3.add(456);

Boolean res1=coll1.containsAll(coll2);
Boolean res2=coll1.containsAll(coll3);
		
System.out.println(res1);//结果为：false
System.out.println(res2);//结果为：true
```

### 4）retainAll(Collection coll) 方法

该方法用于返回当前集合与指定集合共有的元素，即求两集合的交集

```cpp
Collection coll1 = new ArrayList();
Collection coll2 = new ArrayList();	
coll1.add(123);
coll1.add(456);
coll2.add(123);
coll2.add("abc");

coll1.retainAll(coll2);
System.out.println(coll1);//结果为：123
```

## 3.对数组的操作

### 1）toArray() 方法

将集合转化为 Object 型数组（一般来说，由于集合中各种类型元素均有存放，因此转为 Object 类型）

```java
Collection coll = new ArrayList();
coll.add(123);
coll.add(456);
coll.add("abc");

Object[] obj=coll.toArray();
for(int i=0;i<obj.length;i++)
	System.out.print(obj[i]+" ");
```

### 2）toArray(T[] t) 方法

toArray(T[] a) 方法需要传入一个数组作为参数，并通过泛型 T 的方式定义参数，所返回的数组类型就是调用集合的泛型，所以自己无需再转型，但是根据传入数组的长度    与集合的实际长度的关系，会有不同的处理；

① 数组长度不小于集合长度，直接拷贝，不会产生新的数组对象；

② 数组长度小于集合长度，会创建一个与集合长度相同的新数组，将集合的数据拷贝到新数组并将新数组的引用返回。

常用写法如下：

```java
List<Integer> list = new ArrayList<Integer>();
list.add(12);
list.add(12);
list.add(111);
list.add(45);
		
Integer[] a = list.toArray(new Integer[list.size()]);    
for(int i:a)
	System.out.print(i+" ");
//运行结果：12 12 111 45
```

## 4.hashCode() 方法

该方法常用于 Set 集合中，其根据集合中的每个元素的哈希值计算集合的哈希总值，哈希值决定了每个元素底层存储位置。

关于 Set 集合与哈希值：[点击这里](https://blog.csdn.net/u011815404/article/details/83187111)



