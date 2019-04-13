
# java的List实现类 - 李昆鹏的博客 - CSDN博客


2018年03月18日 00:08:49[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：905


**-----------------------java的List实现类------------------------------**
**Synchronized****线程同步**
**ArrayList类**
**List接口的大小可变数组的实现。实现了所有可选列表操作，**
**并允许包括null在内的所有元素。除了实现List接口外，此类**
**还提供一些方法来操作内部用来储存列表的数组的大小。**
**是有序的集合，长度不固定。**
**不是线程安全的。**
**效率高。**
**LinkedList类**
**List****接口的链接列表实现。实现所有可选的列表操作，并且允许所有元素（包括****null****）。除了实现****List****接口外，****LinkedList****类还为在列表的开头及结尾****get****、****remove****和****insert****元素提供了统一的命名方法。这些操作允许将链接列表用作堆栈、****队列****或****双端队列****。**
**不是线程安全的。**
![](//img-blog.csdn.net/20180318000919464?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**堆栈：后进先出，先进后出。（Push进、pop出）**
**队列：先进先出，后进后出。**
|构造方法摘要
|
|LinkedList|()
|构造一个空列表。
|
|LinkedList|(|Collection|<? extends|E|> c)
|构造一个包含指定 collection 中的元素的列表，这些元素按其  collection 的迭代器返回的顺序排列。
|
|方法摘要
|
|boolean
|add|(|E|e)
|将指定元素添加到此列表的结尾。
|
|void
|add|(int index,|E|element)
|在此列表中指定的位置插入指定的元素。
|
|boolean
|addAll|(|Collection|<? extends|E|> c)
|添加指定 collection 中的所有元素到此列表的结尾，顺序是指定  collection 的迭代器返回这些元素的顺序。
|
|boolean
|addAll|(int index,|Collection|<? extends|E|> c)
|将指定 collection 中的所有元素从指定位置开始插入此列表。
|
|void
|addFirst|(|E|e)
|将指定元素插入此列表的开头。
|
|void
|addLast|(|E|e)
|将指定元素添加到此列表的结尾。
|
|void
|clear|()
|从此列表中移除所有元素。
|
|Object
|clone|()
|返回此|LinkedList|的浅表副本。
|
|boolean
|contains|(|Object|o)
|如果此列表包含指定元素，则返回|true|。
|
|Iterator|<|E|>
|descendingIterator|()
|返回以逆向顺序在此双端队列的元素上进行迭代的迭代器。
|
|E
|element|()
|获取但不移除此列表的头（第一个元素）。
|
|E
|get|(int index)
|返回此列表中指定位置处的元素。
|
|E
|getFirst|()
|返回此列表的第一个元素。
|
|E
|getLast|()
|返回此列表的最后一个元素。
|
|int
|indexOf|(|Object|o)
|返回此列表中首次出现的指定元素的索引，如果此列表中不包含该元素，则返回 -1。
|
|int
|lastIndexOf|(|Object|o)
|返回此列表中最后出现的指定元素的索引，如果此列表中不包含该元素，则返回 -1。
|
|ListIterator|<|E|>
|listIterator|(int index)
|返回此列表中的元素的列表迭代器（按适当顺序），从列表中指定位置开始。
|
|boolean
|offer|(|E|e)
|将指定元素添加到此列表的末尾（最后一个元素）。
|
|boolean
|offerFirst|(|E|e)
|在此列表的开头插入指定的元素。
|
|boolean
|offerLast|(|E|e)
|在此列表末尾插入指定的元素。
|
|E
|peek|()
|获取但不移除此列表的头（第一个元素）。
|
|E
|peekFirst|()
|获取但不移除此列表的第一个元素；如果此列表为空，则返回|null|。
|
|E
|peekLast|()
|获取但不移除此列表的最后一个元素；如果此列表为空，则返回|null|。
|
|E
|poll|()
|获取并移除此列表的头（第一个元素）
|
|E
|pollFirst|()
|获取并移除此列表的第一个元素；如果此列表为空，则返回|null|。
|
|E
|pollLast|()
|获取并移除此列表的最后一个元素；如果此列表为空，则返回|null|。
|
|E
|pop|()
|从此列表所表示的堆栈处弹出一个元素。
|
|void
|push|(|E|e)
|将元素推入此列表所表示的堆栈。
|
|E
|remove|()
|获取并移除此列表的头（第一个元素）。
|
|E
|remove|(int index)
|移除此列表中指定位置处的元素。
|
|boolean
|remove|(|Object|o)
|从此列表中移除首次出现的指定元素（如果存在）。
|
|E
|removeFirst|()
|移除并返回此列表的第一个元素。
|
|boolean
|removeFirstOccurrence|(|Object|o)
|从此列表中移除第一次出现的指定元素（从头部到尾部遍历列表时）。
|
|E
|removeLast|()
|移除并返回此列表的最后一个元素。
|
|boolean
|removeLastOccurrence|(|Object|o)
|从此列表中移除最后一次出现的指定元素（从头部到尾部遍历列表时）。
|
|E
|set|(int index,|E|element)
|将此列表中指定位置的元素替换为指定的元素。
|
|int
|size|()
|返回此列表的元素数。
|
|Object|[]
|toArray|()
|返回以适当顺序（从第一个元素到最后一个元素）包含此列表中所有元素的数组。
|
|<T> T[]
|
|toArray|(T[] a)
|返回以适当顺序（从第一个元素到最后一个元素）包含此列表中所有元素的数组；返回数组的运行时类型为指定数组的类型。
|
**例子：**
**public****static****void**main(String[]args) {
//创建一个List集合
LinkedListlist=**new**LinkedList<>();
list.add("松江");
list.add("流浪");
list.add("阿道夫");
System.**out**.println(list);
//把元素插入到列表的头
list.addFirst("百胜");
//把元素插入到列表的最尾端
list.addLast("吴昊");
System.**out**.println(list);
//获得头元素,没有移除
Objectobj=list.element();
System.**out**.println(obj);
System.**out**.println(list);
//出栈
Objectobj1=list.poll();
System.**out**.println(obj1);
System.**out**.println(list);
//加栈
list.push("武松");
System.**out**.println(list);
}
}
结果：
[松江,流浪,阿道夫]
[百胜,松江,流浪,阿道夫,吴昊]
百胜
[百胜,松江,流浪,阿道夫,吴昊]
百胜
[松江,流浪,阿道夫,吴昊]
[武松,松江,流浪,阿道夫,吴昊]
**------------------Vector****类****--------------------------**
**是线程安全的，是同步的。**

## java.util
## 类 Vector<E>
![](https://img-blog.csdn.net/20180324213629958?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
java.lang.Objectjava.util.AbstractCollection<E>java.util.AbstractList<E>**java.util.Vector<E>****所有已实现的接口：**
Serializable,Cloneable,Iterable<E>,Collection<E>,List<E>,RandomAccess
**直接已知子类：**
Stack
---

public class**Vector<E>**extendsAbstractList<E>implementsList<E>,RandomAccess,Cloneable,Serializable`Vector`类可以实现可增长的对象数组。与数组一样，它包含可以使用整数索引进行访问的组件。但是，`Vector`的大小可以根据需要增大或缩小，以适应创建`Vector`后进行添加或移除项的操作。
每个向量会试图通过维护`capacity`和`capacityIncrement`来优化存储管理。`capacity`始终至少应与向量的大小相等；这个值通常比后者大些，因为随着将组件添加到向量中，其存储将按`capacityIncrement`的大小增加存储块。应用程序可以在插入大量组件前增加向量的容量；这样就减少了增加的重分配的量。
由 Vector 的 iterator 和 listIterator 方法所返回的迭代器是*快速失败的*：如果在迭代器创建后的任意时间从结构上修改了向量（通过迭代器自身的 remove 或 add 方法之外的任何其他方式），则迭代器将抛出 ConcurrentModificationException。因此，面对并发的修改，迭代器很快就完全失败，而不是冒着在将来不确定的时间任意发生不确定行为的风险。Vector 的 elements 方法返回的 Enumeration*不是*快速失败的。
注意，迭代器的快速失败行为不能得到保证，一般来说，存在不同步的并发修改时，不可能作出任何坚决的保证。快速失败迭代器尽最大努力抛出`ConcurrentModificationException`。因此，编写依赖于此异常的程序的方式是错误的，正确做法是：*迭代器的快速失败行为应该仅用于检测 bug。*
从 Java 2 平台 v1.2 开始，此类改进为可以实现`List`接口，使它成为JavaCollections Framework的成员。与新 collection 实现不同，`Vector`是同步的。
**从以下版本开始：**
JDK1.0
**另请参见：**
`Collection`,`List`,`ArrayList`,`LinkedList`,序列化表格
---

|字段摘要
|
|protected  int
|capacityIncrement
|向量的大小大于其容量时，容量自动增加的量。
|
|protected  int
|elementCount
|Vector|对象中的有效组件数。
|
|protected|Object|[]
|elementData
|存储向量组件的数组缓冲区。
|
|从类 java.util.|AbstractList|继承的字段
|
|modCount
|
|构造方法摘要
|
|Vector|()
|构造一个空向量，使其内部数据数组的大小为|10|，其标准容量增量为零。
|
|Vector|(|Collection|<? extends|E|> c)
|构造一个包含指定 collection 中的元素的向量，这些元素按其  collection 的迭代器返回元素的顺序排列。
|
|Vector|(int initialCapacity)
|使用指定的初始容量和等于零的容量增量构造一个空向量。
|
|Vector|(int initialCapacity, int capacityIncrement)
|使用指定的初始容量和容量增量构造一个空的向量。
|
|方法摘要
|
|boolean
|add|(|E|e)
|将指定元素添加到此向量的末尾。
|
|void
|add|(int index,|E|element)
|在此向量的指定位置插入指定的元素。
|
|boolean
|addAll|(|Collection|<? extends|E|> c)
|将指定 Collection 中的所有元素添加到此向量的末尾，按照指定  collection 的迭代器所返回的顺序添加这些元素。
|
|boolean
|addAll|(int index,|Collection|<? extends|E|> c)
|在指定位置将指定 Collection 中的所有元素插入到此向量中。
|
|void
|addElement|(|E|obj)
|将指定的组件添加到此向量的末尾，将其大小增加  1。
|
|int
|capacity|()
|返回此向量的当前容量。
|
|void
|clear|()
|从此向量中移除所有元素。
|
|Object
|clone|()
|返回向量的一个副本。
|
|boolean
|contains|(|Object|o)
|如果此向量包含指定的元素，则返回|true|。
|
|boolean
|containsAll|(|Collection|<?> c)
|如果此向量包含指定 Collection 中的所有元素，则返回 true。
|
|void
|copyInto|(|Object|[] anArray)
|将此向量的组件复制到指定的数组中。
|
|E
|elementAt|(int index)
|返回指定索引处的组件。
|
|Enumeration|<|E|>
|elements|()
|返回此向量的组件的枚举。
|
|void
|ensureCapacity|(int minCapacity)
|增加此向量的容量（如有必要），以确保其至少能够保存最小容量参数指定的组件数。
|
|boolean
|equals|(|Object|o)
|比较指定对象与此向量的相等性。
|
|E
|firstElement|()
|返回此向量的第一个组件（位于索引|0|) 处的项）。
|
|E
|get|(int index)
|返回向量中指定位置的元素。
|
|int
|hashCode|()
|返回此向量的哈希码值。
|
|int
|indexOf|(|Object|o)
|返回此向量中第一次出现的指定元素的索引，如果此向量不包含该元素，则返回 -1。
|
|int
|indexOf|(|Object|o, int index)
|返回此向量中第一次出现的指定元素的索引，从|index|处正向搜索，如果未找到该元素，则返回 -1。
|
|void
|insertElementAt|(|E|obj, int index)
|将指定对象作为此向量中的组件插入到指定的|index|处。
|
|boolean
|isEmpty|()
|测试此向量是否不包含组件。
|
|E
|lastElement|()
|返回此向量的最后一个组件。
|
|int
|lastIndexOf|(|Object|o)
|返回此向量中最后一次出现的指定元素的索引；如果此向量不包含该元素，则返回 -1。
|
|int
|lastIndexOf|(|Object|o, int index)
|返回此向量中最后一次出现的指定元素的索引，从|index|处逆向搜索，如果未找到该元素，则返回 -1。
|
|E
|remove|(int index)
|移除此向量中指定位置的元素。
|
|boolean
|remove|(|Object|o)
|移除此向量中指定元素的第一个匹配项，如果向量不包含该元素，则元素保持不变。
|
|boolean
|removeAll|(|Collection|<?> c)
|从此向量中移除包含在指定 Collection  中的所有元素。
|
|void
|removeAllElements|()
|从此向量中移除全部组件，并将其大小设置为零。
|
|boolean
|removeElement|(|Object|obj)
|从此向量中移除变量的第一个（索引最小的）匹配项。
|
|void
|removeElementAt|(int index)
|删除指定索引处的组件。
|
|protected  void
|removeRange|(int fromIndex, int toIndex)
|从此 List 中移除其索引位于 fromIndex（包括）与 toIndex（不包括）之间的所有元素。
|
|boolean
|retainAll|(|Collection|<?> c)
|在此向量中仅保留包含在指定  Collection 中的元素。
|
|E
|set|(int index,|E|element)
|用指定的元素替换此向量中指定位置处的元素。
|
|void
|setElementAt|(|E|obj, int index)
|将此向量指定|index|处的组件设置为指定的对象。
|
|void
|setSize|(int newSize)
|设置此向量的大小。
|
|int
|size|()
|返回此向量中的组件数。
|
|List|<|E|>
|subList|(int fromIndex, int toIndex)
|返回此 List 的部分视图，元素范围为从 fromIndex（包括）到 toIndex（不包括）。
|
|Object|[]
|toArray|()
|返回一个数组，包含此向量中以恰当顺序存放的所有元素。
|
|<T> T[]
|
|toArray|(T[] a)
|返回一个数组，包含此向量中以恰当顺序存放的所有元素；返回数组的运行时类型为指定数组的类型。
|
|String
|toString|()
|返回此向量的字符串表示形式，其中包含每个元素的  String 表示形式。
|
|void
|trimToSize|()
|对此向量的容量进行微调，使其等于向量的当前大小。
|
**例子：**
**public****class**CollectionTest10 {
**public****static****void**main(String[]args) {
Vectorv=**new**Vector();
v.add("宋江");
v.add("白胜");
v.add("刘唐");
//获得集合中的每个元素，枚举
Enumerationenums=v.elements();
**while**(enums.hasMoreElements()){
//可以一边遍历一边添加
Objectobj=enums.nextElement();
**if**("白胜"==obj)
v.add("娇羞");
System.**out**.println(obj);
}
}
}
结果：
宋江
白胜
刘唐
娇羞


