# Java —— 集合工具类(Collections 类) - Alex_McAvoy的博客 - CSDN博客





2018年10月23日 14:30:35[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：39
个人分类：[Java学习笔记](https://blog.csdn.net/u011815404/article/category/8863016)

所属专栏：[Java 学习笔记](https://blog.csdn.net/column/details/34317.html)









# 【概述】

Collections 类是一个操作 List、Set、Map 等集合的工具类，其提供了一系列静态方法对集合元素进行排序、查询、修改等操作，还提供了对集合对象设置不可变、对集合对象实现同步控制等方法。

# 【排序操作】

## 1.reverse(List list) 方法

reverse(List list) 方法用于反转 List 内元素顺序

```java
List list=new ArrayList();
list.add(123);
list.add(45);
list.add(111);
list.add(12);
System.out.println(list);//结果：[123, 45, 111, 12]

Collections.reverse(list);
System.out.println(list);//结果：[12, 111, 45, 123]
```

## 2.shuffle(List list) 方法

shuffle(List list) 方法用于对 List 内元素进行随机排序

```java
List list=new ArrayList();
list.add(123);
list.add(45);
list.add(111);
list.add(12);
System.out.println(list);//结果：[123, 45, 111, 12]

Collections.shuffle(list);
System.out.println(list);//结果：[111, 45, 12, 123]  每次结果都不同
```

## 3.sort(List list) 方法

sort(List list) 方法根据元素的自然顺序对指定 List 集合元素按升序排序

```java
List list=new ArrayList();
list.add(123);
list.add(45);
list.add(111);
list.add(12);
System.out.println(list);//结果：[123, 45, 111, 12]

Collections.sort(list);
System.out.println(list);//结果：[12, 45, 111, 123]
```

## 4.sort(List list,Comparator) 方法

sort(List list,Comparator) 方法根据指定的 Comparator 产生的顺序对 List 集合元素进行排序

```java
List list=new ArrayList();
list.add(123);
list.add(45);
list.add(111);
list.add(12);
System.out.println(list);//结果为：[123, 45, 111, 12]

Collections.sort(list,new Comparator<Integer>() {
    @Override
    public int compare(Integer a,Integer b) {
	    if(a<b)
	    	return 1;
	    else if(a>b)
	    	return -1;
	    else
		return 0;
    }
});
System.out.println(list);//结果为：[123, 111, 45, 12]
```

## 5.swap(List list,int a,int b) 方法

swap(List list,int a,int b) 方法将指定 list 集合中的 i 处元素和 j 处元素进行交换

```java
List list=new ArrayList();
list.add(123);
list.add(45);
list.add(111);
list.add(12);
System.out.println(list);//结果：[123, 45, 111, 12]

Collections.swap(list,0,2);
System.out.println(list);//结果：[111, 45, 123, 12]
```

# 【查找替换操作】

## 1.max(Collection) 方法

max(Collection) 方法根据元素自然顺序，返回给定集合中的最大元素

```java
List list=new ArrayList();
list.add(123);
list.add(45);
list.add(111);
list.add(12);
		
Object obj=Collections.max(list);
System.out.println(obj);//结果：123
```

除 max(Collection) 方法外，还有 max(Collection,Comparator) 方法根据 Comparator 指定的顺序，返回给定集合中的最大元素

## 2.min(Collection) 方法

min(Collection) 方法根据元素自然顺序，返回给定集合中的最小元素

```java
List list=new ArrayList();
list.add(123);
list.add(45);
list.add(111);
list.add(12);
		
Object obj=Collections.min(list);
System.out.println(obj);//结果：12
```

除 min(Collection) 方法外，min(Collecttion,Comparator) 方法根据 Comparator 指定的顺序，返回给定集合中的最小元素

## 3.frequency(Collection,Object) 方法

frequency(Collection,Object) 方法返回指定集合中指定元素出现的次数

```java
List list=new ArrayList();
list.add(123);
list.add(45);
list.add(111);
list.add(12);
		
int cnt=Collections.frequency(list,123);
System.out.println(cnt);//结果：1
```

## 4.copy(List dest,List src) 方法

copy(List dest,List src) 方法将 src 中的内容复制到 dest 中，前提是 src 的长度要大于 dest 的长度

```java
List list=new ArrayList();
list.add(123);
list.add(45);
list.add(111);
list.add(12);
		
List newList=new ArrayList();
newList.add(1);
newList.add(2);
newList.add(3);
Collections.copy(list,newList);
		
System.out.println(list);//结果：[1, 2, 3, 12]
System.out.println(newList);//结果：[1, 2, 3]
```

## 5.replaceAll(List list,Object oldVal,Object newVal) 方法

replaceAll(List list,Object oldVal,Object newVal) 方法使用新值 newVal 替换当前 List 对象的所有旧值 oldVal

```java
List list=new ArrayList();
list.add(1);
list.add(2);
list.add(3);
list.add(1);
list.add(2);
list.add(2);
		
System.out.println(list);//结果：[1, 2, 3, 1, 2, 2]
Collections.replaceAll(list,2,22);
System.out.println(list);//结果：[1, 22, 3, 1, 22, 22]
```

# 【同步控制】

Collections 类中提供了多个 synchronizedXXX() 方法，该方法可使将指定集合包装成线程同步的集合，从而解决多线程并发访问集合时的线程安全问题。

例如：

```java
List list=new ArrayList();
list.add(1);
list.add(2);
list.add(3);
list.add(1);
list.add(2);
list.add(2);
		
List newList=Collections.synchronizedList(list);//线程安全
```





