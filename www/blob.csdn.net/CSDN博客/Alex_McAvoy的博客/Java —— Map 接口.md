# Java —— Map 接口 - Alex_McAvoy的博客 - CSDN博客





2018年10月19日 19:46:24[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：41
个人分类：[Java学习笔记](https://blog.csdn.net/u011815404/article/category/8863016)

所属专栏：[Java 学习笔记](https://blog.csdn.net/column/details/34317.html)









# 【概述】

Map 为映射类型，与 Collection 并列存在，用于保存具有映射关系的数据：Key-Value

映射中存储的每个对象(Key-Value)，是用 Set 存放的，不允许重复；相应的键对象(Key)，也是用 Set 存放的，不允许重复，因此当 Key 为自定义类型时，需要重写 hashCode() 和 equals() 方法；对应的值对象(Value)，是用 Collection 存放的，是可重复的

Map 中的 Key 和 Value 可以是任何类型的数据，常用 String 类做 Key，通过指定的 Key 总能找到唯一确定的 Value

Map 接口常用的实现类有 HashMap、LinkedHashMap、TreeMap，可根据需要进行实例化。

```java
//利用HashMap类实例化集合
HashMap<类类型> 集合名 = new HashMap<类类型>();
//利用LinkedHashMap类实例化集合
LinkedHashMap<类类型> 集合名 = new LinkedHashMap<类类型>();
//利用TreeMap类实例化集合
TreeMap<类类型> 集合名 = new TreeMap<类类型>();
//当省略参数 <类类型> 时，是一原始类型，其中可存储任何类型的元素
Map 集合名 = new HashMap();
```

# 【HashMap 类】

HashMap 类实现了 Map 接口，由于 HashMap 类实现了 Map 集合，如果经常需要添加、删除、定位映射关系，可以使用 HashMap 类，不过在遍历集合时，得到的映射关系是无序的。

HashMap 中存储的键、值可以为 null，但只能存储一个键为 null 的情况，当向 HashMap 中存储元素时，会先调用 key 所在类的 hashCode() 方法，再调用 equals() 方法，判断两个 key 是否相同，若相同，则只能添加进后添加的元素，但自定义类时，需要重写 hashCode() 方法和 equals() 方法。

当重写 hashCode() 方法时，需要遵循以下两种原则：

1）不唯一原则：不必为每一个对象生成一个唯一的哈希码，只要通过 hashCode() 方法生成的哈希码能够利用 get() 方法得到利用 put() 方法添加的映射关系即可。

2）分散原则：生成哈希码的算法应尽量使哈希码的值分散一些，不要很多的哈希码值集中在一个范围内，有利于提高 HashMap 的性能。

# 【LinkedHashMap】

LinkedHashMap 继承自 HashMap，其使用链表维护添加进 Map 中的顺序，遍历时按添加顺序遍历，类似 LinkedHashSet 类。

# 【TreeMap】

TreeMap 不仅实现了 Map 接口，还实现了 Map 接口的子接口 java.util.SortedMap ，与 HashMap 不同的时，TreeMap 不允许键对象为 null。

其按照添加进 Map 中的元素的 Key(要求所有的 Key 都是同一类型对象) 的指定属性进行排序，其排序方式与 Set 集合的 TreeSet 相同，分为自然排序和定制排序两种。

在添加、删除、定位映射关系上，TreeMap 要比 HashMap 性能差，但其中的映射关系具有一定顺序。因此，若不需要一个有序的映射关系的集合，建议使用 HashMap；若需要进行有序的遍历输出，则使用 TreeMap，在这种情况下，可以先使用由 HashMap 实现的 Map 集合，当需要顺序输出时，再利用现有的 HashMap 实例，创建一完全相同映射关系的 TreeMap 类实例。

# 【HashTable】

HashTable 是一古老的 Map 实现类，同 List 接口的 Vector 类一样，由于线程安全等问题，现已过时、废弃。

但其子类 Properties 类仍被使用，常用来处理属性文件，其键值均为 String 类类型。

# 【Map 接口定义的方法】

## 1.添加删除操作

### 1）put(Object key,Object value) 方法

put(Object key,Object value) 方法用于向集合内添加指定的键值映射关系

```java
Map map=new HashMap();
map.put("aa",123);
map.put("bb",456);
map.put(123,"aa");
System.out.println(map);//运行结果：{aa=123, bb=456, 123=aa}
```

### 2）remove(Object key) 方法

remove(Object key) 用于删除集合中指定的映射，若集合中存在指定键，则删除该键值对并返回该键对应的值对象，若不存在，则返回 null

```java
Map map=new HashMap();
map.put("aa",123);
map.put("bb",456);
map.put(123,"aa");
System.out.println(map.remove(123));//结果：aa
System.out.println(map.remove(123));//结果：null
System.out.println(map);//结果：{aa=123, bb=456}
```

### 3）putAll(Map t) 方法

putAll(Map t) 方法将指定集合中的所有映射关系添加到当前集合中

```java
Map map1=new HashMap();
Map map2=new HashMap();
map1.put("aa",123);
map1.put("bb",456);
map2.put(111,"aa");
map2.put(222,"bb");
System.out.println(map1);//结果：{aa=123, bb=456}
System.out.println(map2);//结果：{222=bb, 111=aa}
		
map1.putAll(map2);
System.out.println(map1);//结果：{aa=123, bb=456, 222=bb, 111=aa}
```

### 4）clear() 方法

clear() 方法用于清空集合中的映射

```java
Map map=new HashMap();
map.put("aa",123);
map.put("bb",456);
map.put(123,"aa");
map.clear();
System.out.println(map);//结果：{}
```

## 2.元素查询操作

### 1）get(Object key) 方法

若存在指定的键对象，则返回与该键对象对应的值对象，否则返回 null

```java
Map map=new HashMap();
map.put("aa",123);
map.put("bb",456);
map.put(123,"aa");
		
System.out.println(map.get(123));//结果：aa
System.out.println(map.get(456));//结果：null
```

### 2）containsKey(Object key) 方法

containsKey(Object key) 方法用于判断当前集合中是否存在指定键的映射关系，若存在返回 true，否则返回 false

```java
Map map=new HashMap();
map.put("aa",123);
map.put("bb",456);
map.put(123,"aa");
		
System.out.println(map.containsKey("aa"));//结果：true
System.out.println(map.containsKey(456));//结果：false
```

### 3）containsValue(Object value) 方法

containsKey(Object key) 方法用于判断当前集合中是否存在指定值的映射关系，若存在返回 true，否则返回 false

```java
Map map=new HashMap();
map.put("aa",123);
map.put("bb",456);
map.put(123,"aa");
		
System.out.println(map.containsKey("bb"));//结果：false
System.out.println(map.containsKey(456));//结果：true
```

### 4）size() 方法

size() 方法用于查看集合中包含键值关系的个数

```java
Map map=new HashMap();
map.put("aa",123);
map.put("bb",456);
map.put(123,"aa");
System.out.println(map.size());//运行结果：3
```

### 5）isEmpty() 方法

isEmpty() 方法用于判断集合是否为空

```java
Map map=new HashMap();
map.put("aa",123);
map.put("bb",456);
map.put(123,"aa");
System.out.println(map.isEmpty());//运行结果：false
```

### 6）equals(Object obj) 方法

equals(Object obj) 方法常用于判断两个集合是否相等

```java
Map map1=new HashMap();
Map map2=new HashMap();
map1.put("aa",123);
map1.put("bb",456);
map1.put(123,"aa");
map2.put("aa",123);
map2.put("bb",456);
map2.put(123,"aa");
		
System.out.println(map1.equals(map2));//结果：true
```

## 3.元视图操作方法

### 1）keySet() 方法

keySet() 方法用于遍历集合中所有的 Key

```java
Map map=new HashMap();
map.put("aa",123);
map.put("bb",456);
map.put(111,"aa");
map.put(222,"bb");

Set set=map.keySet();
Iterator it=set.iterator();
while(it.hasNext())
	System.out.print(it.next()+" ");//结果：aa bb 222 111
```

### 2）values() 方法

values() 方法用于遍历集合中所有的 Value

```java
Map map=new HashMap();
map.put("aa",123);
map.put("bb",456);
map.put(111,"aa");
map.put(222,"bb");

Collection values=map.values();
Iterator it=values.iterator();
while(it.hasNext())
	System.out.print(it.next()+" ");//结果：123 456 bb aa
```

### 3）entrySet() 方法

entrySet() 方法用于遍历集合中所有的 Key-Value

```java
Map map=new HashMap();
map.put("aa",123);
map.put("bb",456);
map.put(111,"aa");
map.put(222,"bb");

Set set=map.entrySet();
Iterator it=set.iterator();
while(it.hasNext())
	System.out.print(it.next()+" ");
```

除使用 entrySet() 方法遍历集合中所有的 Key-Value 外，还可使用如下方法进行遍历

```java
Map map=new HashMap();
map.put("aa",123);
map.put("bb",456);
map.put(111,"aa");
map.put(222,"bb");

Set set=map.keySet();
for(Object obj:set)
	System.out.println(obj+"-->"+map.get(obj));
/* 运行结果：
 * aa-->123
 * bb-->456
 * 222-->bb
 * 111-->aa
 */
```





