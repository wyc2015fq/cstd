
# java的集合工具类Collections - 李昆鹏的博客 - CSDN博客


2018年03月25日 11:26:55[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：89标签：[java																](https://so.csdn.net/so/search/s.do?q=java&t=blog)个人分类：[JAVASE																](https://blog.csdn.net/weixin_41547486/article/category/7484968)


**-----------------------java的集合工具类Collections------------------**
**工具类****Collections**
**面试题：**
**问：****Collection****和****Collection****的区别；**
**前者是集合的接口**
**后者操作集合的工具类**

## java.util
## 类 Collections
java.lang.Object**java.util.Collections**
---

public class**Collections**extendsObject此类完全由在 collection 上进行操作或返回collection 的静态方法组成。它包含在 collection 上操作的多态算法，即“包装器”，包装器返回由指定collection 支持的新 collection，以及少数其他内容。
如果为此类的方法所提供的 collection 或类对象为 null，则这些方法都将抛出NullPointerException。
此类中所含多态算法的文档通常包括对*实现*的简短描述。应该将这类描述视为*实现注意事项*，而不是*规范*的一部分。实现者应该可以随意使用其他算法替代，只要遵循规范本身即可。（例如，sort使用的算法不一定是合并排序算法，但它必须是*稳定的*。）
此类中包含的“破坏性”算法，即可修改其所操作的 collection 的算法，该算法被指定在 collection 不支持适当的可变基元（比如set方法）时抛出UnsupportedOperationException。如果调用不会对collection 产生任何影响，那么这些算法可能（但不要求）抛出此异常。例如，在已经排序的、不可修改列表上调用sort方法可能会（也可能不会）抛出UnsupportedOperationException。
此类是JavaCollections Framework的成员。
**从以下版本开始：**
1.2
**另请参见：**
`Collection`,`Set`,`List`,`Map`
---

|字段摘要
|
|static|List
|EMPTY_LIST
|空的列表（不可变的）。
|
|static|Map
|EMPTY_MAP
|空的映射（不可变的）。
|
|static|Set
|EMPTY_SET
|空的 set（不可变的）。
|
|方法摘要
|
|static
|<T> boolean
|
|addAll|(|Collection|<? super  T> c, T... elements)
|将所有指定元素添加到指定 collection  中。
|
|static
|<T>|Queue|<T>
|
|asLifoQueue|(|Deque|<T> deque)
|以后进先出 (Lifo)|Queue|的形式返回某个|Deque|的视图。
|
|static
|<T> int
|
|binarySearch|(|List|<? extends|Comparable|<? super  T>> list, T key)
|使用二分搜索法搜索指定列表，以获得指定对象。
|
|static
|<T> int
|
|binarySearch|(|List|<? extends T> list,  T key,|Comparator|<? super  T> c)
|使用二分搜索法搜索指定列表，以获得指定对象。
|
|static
|<E>|Collection|<E>
|
|checkedCollection|(|Collection|<E> c,|Class|<E> type)
|返回指定 collection 的一个动态类型安全视图。
|
|static
|<E>|List|<E>
|
|checkedList|(|List|<E> list,|Class|<E> type)
|返回指定列表的一个动态类型安全视图。
|
|static
|<K,V>|Map|<K,V>
|
|checkedMap|(|Map|<K,V> m,|Class|<K> keyType,|Class|<V> valueType)
|返回指定映射的一个动态类型安全视图。
|
|static
|<E>|Set|<E>
|
|checkedSet|(|Set|<E> s,|Class|<E> type)
|返回指定 set 的一个动态类型安全视图。
|
|static
|<K,V>|SortedMap|<K,V>
|
|checkedSortedMap|(|SortedMap|<K,V> m,|Class|<K> keyType,|Class|<V> valueType)
|返回指定有序映射的一个动态类型安全视图。
|
|static
|<E>|SortedSet|<E>
|
|checkedSortedSet|(|SortedSet|<E> s,|Class|<E> type)
|返回指定有序 set 的一个动态类型安全视图。
|
|static
|<T> void
|
|copy|(|List|<? super T> dest,|List|<? extends T> src)
|将所有元素从一个列表复制到另一个列表。
|
|static boolean
|disjoint|(|Collection|<?> c1,|Collection|<?> c2)
|如果两个指定 collection 中没有相同的元素，则返回|true|。
|
|static
|<T>|List|<T>
|
|emptyList|()
|返回空的列表（不可变的）。
|
|static
|<K,V>|Map|<K,V>
|
|emptyMap|()
|返回空的映射（不可变的）。
|
|static
|<T>|Set|<T>
|
|emptySet|()
|返回空的 set（不可变的）。
|
|static
|<T>|Enumeration|<T>
|
|enumeration|(|Collection|<T> c)
|返回一个指定 collection 上的枚举。
|
|static
|<T> void
|
|fill|(|List|<? super T> list,  T obj)
|使用指定元素替换指定列表中的所有元素。
|
|static int
|frequency|(|Collection|<?> c,|Object|o)
|返回指定 collection 中等于指定对象的元素数。
|
|static int
|indexOfSubList|(|List|<?> source,|List|<?> target)
|返回指定源列表中第一次出现指定目标列表的起始位置；如果没有出现这样的列表，则返回 -1。
|
|static int
|lastIndexOfSubList|(|List|<?> source,|List|<?> target)
|返回指定源列表中最后一次出现指定目标列表的起始位置；如果没有出现这样的列表，则返回 -1。
|
|static
|<T>|ArrayList|<T>
|
|list|(|Enumeration|<T> e)
|返回一个数组列表，它按返回顺序包含指定枚举返回的元素。
|
|static
|<T extends|Object|&|Comparable|<? super T>>
|T
|
|max|(|Collection|<? extends  T> coll)
|根据元素的|自然顺序|，返回给定  collection 的最大元素。
|
|static
|<T> T
|
|max|(|Collection|<? extends  T> coll,|Comparator|<? super  T> comp)
|根据指定比较器产生的顺序，返回给定  collection 的最大元素。
|
|static
|<T extends|Object|&|Comparable|<? super T>>
|T
|
|min|(|Collection|<? extends  T> coll)
|根据元素的|自然顺序|返回给定 collection 的最小元素。
|
|static
|<T> T
|
|min|(|Collection|<? extends  T> coll,|Comparator|<? super  T> comp)
|根据指定比较器产生的顺序，返回给定  collection 的最小元素。
|
|static
|<T>|List|<T>
|
|nCopies|(int n, T o)
|返回由指定对象的|n|个副本组成的不可变列表。
|
|static
|<E>|Set|<E>
|
|newSetFromMap|(|Map|<E,|Boolean|> map)
|返回指定映射支持的 set。
|
|static
|<T> boolean
|
|replaceAll|(|List|<T> list,  T oldVal, T newVal)
|使用另一个值替换列表中出现的所有某一指定值。
|
|static void
|reverse|(|List|<?> list)
|反转指定列表中元素的顺序。
|
|static
|<T>|Comparator|<T>
|
|reverseOrder|()
|返回一个比较器，它强行逆转实现了|Comparable|接口的对象 collection 的|自然顺序|。
|
|static
|<T>|Comparator|<T>
|
|reverseOrder|(|Comparator|<T> cmp)
|返回一个比较器，它强行逆转指定比较器的顺序。
|
|static void
|rotate|(|List|<?> list,  int distance)
|根据指定的距离轮换指定列表中的元素。
|
|static void
|shuffle|(|List|<?> list)
|使用默认随机源对指定列表进行置换。
|
|static void
|shuffle|(|List|<?> list,|Random|rnd)
|使用指定的随机源对指定列表进行置换。
|
|static
|<T>|Set|<T>
|
|singleton|(T o)
|返回一个只包含指定对象的不可变 set。
|
|static
|<T>|List|<T>
|
|singletonList|(T o)
|返回一个只包含指定对象的不可变列表。
|
|static
|<K,V>|Map|<K,V>
|
|singletonMap|(K key, V value)
|返回一个不可变的映射，它只将指定键映射到指定值。
|
|static
|<T extends|Comparable|<? super T>>
|void
|
|sort|(|List|<T> list)
|根据元素的|自然顺序|对指定列表按升序进行排序。
|
|static
|<T> void
|
|sort|(|List|<T> list,|Comparator|<? super  T> c)
|根据指定比较器产生的顺序对指定列表进行排序。
|
|static void
|swap|(|List|<?> list,  int i, int j)
|在指定列表的指定位置处交换元素。
|
|static
|<T>|Collection|<T>
|
|synchronizedCollection|(|Collection|<T> c)
|返回指定 collection 支持的同步（线程安全的）collection。
|
|static
|<T>|List|<T>
|
|synchronizedList|(|List|<T> list)
|返回指定列表支持的同步（线程安全的）列表。
|
|static
|<K,V>|Map|<K,V>
|
|synchronizedMap|(|Map|<K,V> m)
|返回由指定映射支持的同步（线程安全的）映射。
|
|static
|<T>|Set|<T>
|
|synchronizedSet|(|Set|<T> s)
|返回指定 set 支持的同步（线程安全的）set。
|
|static
|<K,V>|SortedMap|<K,V>
|
|synchronizedSortedMap|(|SortedMap|<K,V> m)
|返回指定有序映射支持的同步（线程安全的）有序映射。
|
|static
|<T>|SortedSet|<T>
|
|synchronizedSortedSet|(|SortedSet|<T> s)
|返回指定有序 set 支持的同步（线程安全的）有序 set。
|
|static
|<T>|Collection|<T>
|
|unmodifiableCollection|(|Collection|<? extends  T> c)
|返回指定 collection 的不可修改视图。
|
|static
|<T>|List|<T>
|
|unmodifiableList|(|List|<? extends T> list)
|返回指定列表的不可修改视图。
|
|static
|<K,V>|Map|<K,V>
|
|unmodifiableMap|(|Map|<? extends K,? extends  V> m)
|返回指定映射的不可修改视图。
|
|static
|<T>|Set|<T>
|
|unmodifiableSet|(|Set|<? extends T> s)
|返回指定 set 的不可修改视图。
|
|static
|<K,V>|SortedMap|<K,V>
|
|unmodifiableSortedMap|(|SortedMap|<K,? extends  V> m)
|返回指定有序映射的不可修改视图。
|
|static
|<T>|SortedSet|<T>
|
|unmodifiableSortedSet|(|SortedSet|<T> s)
|返回指定有序 set 的不可修改视图。
|
**自制外部比较器**
**public****class**EmpComparator**implements**Comparator<Emp> {
@Override
**public****int**compare(Empe1, Empe2) {
**int**val=e1.getId() -e2.getId();
**if**(val== 0) {
val=e1.getAge()-e2.getAge();
}
**return**val;
}
}
--------------------------------------
例子：
**public****class**CollectionsDemo {
**public****static****void**main(String[]args) {
List<String>list=**new**ArrayList<String>();
list.add("李昆鹏1");
list.add("李昆鹏2");
list.add("李昆鹏3");
list.add("李昆鹏8");
list.add("李昆鹏4");
list.add("李昆鹏5");
System.**out**.println(list);
//集合的反转
Collections.*reverse*(list);
System.**out**.println(list);
//随机打乱集合
Collections.*shuffle*(list);
System.**out**.println(list);
//对集合进行排序
Collections.*sort*(list);
System.**out**.println(list);
List<Student>list1=**new**ArrayList<Student>();
list1.add(**new**Student(5,"李昆鹏5", 11));
list1.add(**new**Student(3,"李昆鹏3", 33));
list1.add(**new**Student(2,"李昆鹏2", 22));
System.**out**.println(list1);
//对student集合进行排序
Collections.*sort*(list1);
System.**out**.println(list1);
//把list变成同步的（线程安全的）；
Collections.*synchronizedList*(list);
//对Emp进行排序,使用外部比较器来做排序
List<Emp>list2=**new**ArrayList<Emp>();
list2.add(**new**Emp(5,55));
list2.add(**new**Emp(3,33));
list2.add(**new**Emp(88,88));
System.**out**.println(list2);
Collections.*sort*(list2,**new**EmpComparator());
System.**out**.println(list2);
}
}
**结果：**
[李昆鹏1,李昆鹏2,李昆鹏3,李昆鹏8,李昆鹏4,李昆鹏5]
[李昆鹏5,李昆鹏4,李昆鹏8,李昆鹏3,李昆鹏2,李昆鹏1]
[李昆鹏5,李昆鹏1,李昆鹏4,李昆鹏8,李昆鹏3,李昆鹏2]
[李昆鹏1,李昆鹏2,李昆鹏3,李昆鹏4,李昆鹏5,李昆鹏8]
[Student [stuId=5,name=李昆鹏5, age=11], Student [stuId=3, name=李昆鹏3, age=33], Student [stuId=2, name=李昆鹏2, age=22]]
[Student [stuId=2,name=李昆鹏2, age=22], Student [stuId=3, name=李昆鹏3, age=33], Student [stuId=5, name=李昆鹏5, age=11]]
[Emp [id=5, age=55],Emp [id=3, age=33], Emp [id=88, age=88]]
[Emp [id=3, age=33],Emp [id=5, age=55], Emp [id=88, age=88]]


