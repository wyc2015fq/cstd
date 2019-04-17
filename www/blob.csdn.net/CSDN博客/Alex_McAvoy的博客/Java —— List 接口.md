# Java —— List 接口 - Alex_McAvoy的博客 - CSDN博客





2018年10月18日 23:48:56[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：44
个人分类：[Java学习笔记](https://blog.csdn.net/u011815404/article/category/8863016)

所属专栏：[Java 学习笔记](https://blog.csdn.net/column/details/34317.html)









# 【概述】

List 集合为列表类型，主要特征是以线性方式存储对象，通过对象的索引来操作对象，因此 List 集合可视为一动态数组。

List 接口的常用实现类有 ArrayList、LinkedList、Vector，在使用 List 集合时，通常情况下，声明为 List 类型，实例化时，根据实际需要，实例化为 ArrayList、LinkedList、Vector

```java
//利用ArrayList类实例化集合
List<类类型> 集合名 = new ArrayList<类类型>();
//利用ArrayList类实例化集合
List<类类型> 集合名 = new LinkedList<类类型>();
//利用Vector类实例化集合
List<类类型> 集合名 = new Vector<类类型>();
//当省略参数 <类类型> 时，是一原始类型，其中可存储任何类型的元素
List 集合名 = new ArrayList();
```

# 【三个实现类的特征】

## 1.ArrayList 类

ArrayList 类实现了 List 接口，采用了数组结构来保存对象。

数组结构的优点是便于对集合进行快速随机访问，若经常需要根据索引位置来访问集合中的对象，使用 ArrayList 类实现的 List 集合的效率较高。

数组结构的缺点是对指定索引位置插入、删除对象时速度较慢，且插入、删除对象的索引位置越小效率越低（插入时，指定索引位置后的元素全部后移一位；删除时，指定索引位置的元素全部前移一位），因此，若经常需要对指定索引位置插入、删除对象时，使用 ArrayList 类实现的 List 集合的效率较低。

## 2.LinkedList 类

LinkedList 类实现了 List 接口，采用了链表结构来保存对象。

链表结构的优点是便于向集合中插入、删除对象，当插入、删除集合时，只需要简单地修改链接位置，省去移动对象的操作，因此，若经常需要向集合中插入、删除对象，使用 LinkedList 类实现的 List 集合效率较高。

链表结构的缺点是随机访问对象的速度较慢，若经常需要随机访问集合中的对象，使用 LinkedList 类实现的 List 集合效率较低。

LinkedList 类定义的常用方法：
- addFirst(Object obj)：将指定对象插入到列表的开头
- addLast(Object obj)：将指定对象插入到列表的结尾
- getFirst()：获得列表开头的对象
- getLast()：获得列表结尾的对象
- removeFirst()：移除列表开头的对象
- removeLast()：移除列表结尾的对象

## 3.Vector类

Vector 类是一古老的实现类，由于线程安全等问题，现已过时、废弃

# 【Lits 接口通用方法】

List 集合包含 List 接口以及 List 接口的所有实现类，由于 List 接口继承了 Collection 接口，因此 List 接口具有 Collection 接口提供的所有常用方法，又因为 List 是列表类型，因此，List 接口还提供了一些适合于自身的常用方法。

## 1.add(int index,Object obj) 方法

向当前集合的指定索引位置添加指定对象，其他对象索引位置后移一位，索引从 0 开始

```java
List<Integer> list = new ArrayList<Integer>();
list.add(123);
list.add(456);
list.add(789);
		
list.add(0,123);
System.out.println(list);//结果：[123, 123, 456, 789]
```

## 2.addAll(int index,Collection coll) 方法

向当前集合的指定索引位置添加指定集合的所有位置，其他对象索引位置后移

```java
List<Integer> list1 = new ArrayList<Integer>();
List<Integer> list2 = new ArrayList<Integer>();

list1.add(123);
list1.add(456);
list1.add(789);
list2.add(1);
list2.add(2);

System.out.println(list1);//结果：[123, 456, 789]
list1.addAll(1,list2);
System.out.println(list1);//结果：[123, 1, 2, 456, 789]
```

## 3.remove(int index) 方法

清除当前集合中指定索引位置的对象

```java
List<Integer> list = new ArrayList<Integer>();
list.add(123);
list.add(456);
list.add(789);

System.out.println(list);//结果：[123, 456, 789]
list.remove(1);
System.out.println(list);//结果：[123, 789]
```

## 4.set(int index,Object obj) 方法

将当前集合的指定索引位置的对象替换为指定对象

```java
List<Integer> list = new ArrayList<Integer>();
list.add(123);
list.add(456);
list.add(789);

System.out.println(list);//结果：[123, 456, 789]
list.set(1,111);
System.out.println(list);//结果：[123, 111, 789]
```

## 5.get(int index) 方法

获取当前集合中指定索引位置的对象

```java
List<Integer> list = new ArrayList<Integer>();
list.add(123);
list.add(456);
list.add(789);

int res=list.get(1);
System.out.println(res);//结果：456
```

## 6.indexOf(Object obj) 方法

获取指定对象的索引位置。当存在多个时，返回第一个的索引位置；当不存在时，返回-1

```java
List<Integer> list = new ArrayList<Integer>();
list.add(123);
list.add(123);
list.add(789);

int res1=list.indexOf(123);
int res2=list.indexOf(111);

System.out.println(res1);//结果：0
System.out.println(res2);//结果：-1
```

## 7.lastIndexOf(Object obj) 方法

获取指定对象的索引位置。当存在多个时，返回最后一个的索引位置；当不存在时，返回-1

```java
List<Integer> list = new ArrayList<Integer>();
list.add(123);
list.add(456);
list.add(789);

int res1=list.lastIndexOf(123);
int res2=list.indexOf(111);

System.out.println(res1);//结果：1
System.out.println(res2);//结果：-1
```

## 8.subList(int fromIndex,int toIndex) 方法

截取当前集合从起始索引位置（fromIndex，包含）到终止索引位置（toIndex，不包含）的对象，重新生成一个 List 集合并返回

```java
List<Integer> list = new ArrayList<Integer>();
list.add(123);
list.add(456);
list.add(789);

list.subList(0,2);
System.out.println(list);//结果：[123, 456]
```

## 9.listIterator() 方法

获得一个包含当前集合所有对象的 ListIterator 型实例

```java
List<Integer> list = new ArrayList<Integer>();
list.add(123);
list.add(456);
list.add(789);

ListIterator<Integer> lit = list.listIterator();
while(lit.hasNext()){
	System.out.println(lit.next());
}
```

## 10.listIterator(int index) 方法

获取一个包含当前集合从指定位置到最后的 ListIterator 型实例

```java
List<Integer> list = new ArrayList<Integer>();
list.add(123);
list.add(456);
list.add(789);

ListIterator<Integer> lit = list.listIterator(1);
while(lit.hasNext()){
	System.out.println(lit.next());
}
```

关于 ListIterator 接口：[点击这里](https://blog.csdn.net/u011815404/article/details/83187117)



