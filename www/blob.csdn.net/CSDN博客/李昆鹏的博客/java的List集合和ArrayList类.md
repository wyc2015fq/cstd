
# java的List集合和ArrayList类 - 李昆鹏的博客 - CSDN博客


2018年03月17日 20:53:13[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：51


**-----------------------java的List集合--------------------------------**
**ArrayList类**
**List接口的大小可变数组的实现。实现了所有可选列表操作，**
**并允许包括null在内的所有元素。除了实现List接口外，此类**
**还提供一些方法来操作内部用来储存列表的数组的大小。**
**是有序的集合，长度不固定。**
**不是线程安全的。**
**效率高。**
**-----------------List类-------------------------**
## java.util
## 接口 List<E>
**所有超级接口：**
Collection<E>,Iterable<E>
**所有已知实现类：**
AbstractList,AbstractSequentialList,ArrayList,AttributeList,CopyOnWriteArrayList,LinkedList,RoleList,RoleUnresolvedList,Stack,Vector
---

public interface**List<E>**extendsCollection<E>有序的 collection（也称为*序列*）。此接口的用户可以对列表中每个元素的插入位置进行精确地控制。用户可以根据元素的整数索引（在列表中的位置）访问元素，并搜索列表中的元素。
与 set 不同，列表通常允许重复的元素。更确切地讲，列表通常允许满足e1.equals(e2)的元素对e1和e2，并且如果列表本身允许 null 元素的话，通常它们允许多个 null 元素。难免有人希望通过在用户尝试插入重复元素时抛出运行时异常的方法来禁止重复的列表，但我们希望这种用法越少越好。
List接口在iterator、add、remove、equals和hashCode方法的协定上加了一些其他约定，超过了Collection接口中指定的约定。为方便起见，这里也包括了其他继承方法的声明。
List接口提供了 4 种对列表元素进行定位（索引）访问方法。列表（像 Java 数组一样）是基于 0 的。注意，这些操作可能在和某些实现（例如LinkedList类）的索引值成比例的时间内执行。因此，如果调用者不知道实现，那么在列表元素上迭代通常优于用索引遍历列表。
List接口提供了特殊的迭代器，称为ListIterator，除了允许Iterator接口提供的正常操作外，该迭代器还允许元素插入和替换，以及双向访问。还提供了一个方法来获取从列表中指定位置开始的列表迭代器。
List接口提供了两种搜索指定对象的方法。从性能的观点来看，应该小心使用这些方法。在很多实现中，它们将执行高开销的线性搜索。
List接口提供了两种在列表的任意位置高效插入和移除多个元素的方法。
注意：尽管列表允许把自身作为元素包含在内，但建议要特别小心：在这样的列表上，equals和hashCode方法不再是定义良好的。
某些列表实现对列表可能包含的元素有限制。例如，某些实现禁止 null 元素，而某些实现则对元素的类型有限制。试图添加不合格的元素会抛出未经检查的异常，通常是NullPointerException或ClassCastException。试图查询不合格的元素是否存在可能会抛出异常，也可能简单地返回 false；某些实现会采用前一种行为，而某些则采用后者。概括地说，试图对不合格元素执行操作时，如果完成该操作后不会导致在列表中插入不合格的元素，则该操作可能抛出一个异常，也可能成功，这取决于实现的选择。此接口的规范中将这样的异常标记为“可选”。
此接口是JavaCollections Framework的成员。
**从以下版本开始：**
1.2
**另请参见：**
`Collection`,`Set`,`ArrayList`,`LinkedList`,`Vector`,`Arrays.asList(Object[])`,`Collections.nCopies(int, Object)`,`Collections.EMPTY_LIST`,`AbstractList`,`AbstractSequentialList`
---

|方法摘要
|
|boolean
|add|(|E|e)
|向列表的尾部添加指定的元素（可选操作）。
|
|void
|add|(int index,|E|element)
|在列表的指定位置插入指定元素（可选操作）。
|
|boolean
|addAll|(|Collection|<? extends|E|> c)
|添加指定|collection|中的所有元素到此列表的结尾，顺序是指定|collection|的迭代器返回这些元素的顺序（可选操作）。
|
|boolean
|addAll|(int index,|Collection|<? extends|E|> c)
|将指定|collection|中的所有元素都插入到列表中的指定位置（可选操作）。
|
|void
|clear|()
|从列表中移除所有元素（可选操作）。
|
|boolean
|contains|(|Object|o)
|如果列表包含指定的元素，则返回|true|。
|
|boolean
|containsAll|(|Collection|<?> c)
|如果列表包含指定 collection 的所有元素，则返回|true|。
|
|boolean
|equals|(|Object|o)
|比较指定的对象与列表是否相等。
|
|E
|get|(int index)
|返回列表中指定位置的元素。
|
|int
|hashCode|()
|返回列表的哈希码值。
|
|int
|indexOf|(|Object|o)
|返回此列表中第一次出现的指定元素的索引；如果此列表不包含该元素，则返回 -1。
|
|boolean
|isEmpty|()
|如果列表不包含元素，则返回|true|。
|
|Iterator|<|E|>
|iterator|()
|返回按适当顺序在列表的元素上进行迭代的迭代器。
|
|int
|lastIndexOf|(|Object|o)
|返回此列表中最后出现的指定元素的索引；如果列表不包含此元素，则返回 -1。
|
|ListIterator|<|E|>
|listIterator|()
|返回此列表元素的列表迭代器（按适当顺序）。
|
|ListIterator|<|E|>
|listIterator|(int index)
|返回列表中元素的列表迭代器（按适当顺序），从列表的指定位置开始。
|
|E
|remove|(int index)
|移除列表中指定位置的元素（可选操作）。
|
|boolean
|remove|(|Object|o)
|从此列表中移除第一次出现的指定元素（如果存在）（可选操作）。
|
|boolean
|removeAll|(|Collection|<?> c)
|从列表中移除指定|collection|中包含的其所有元素（可选操作）。
|
|boolean
|retainAll|(|Collection|<?> c)
|仅在列表中保留指定|collection|中所包含的元素（可选操作）。
|
|E
|set|(int index,|E|element)
|用指定元素替换列表中指定位置的元素（可选操作）。
|
|int
|size|()
|返回列表中的元素数。
|
|List|<|E|>
|subList|(int fromIndex, int toIndex)
|返回列表中指定的|fromIndex|（包括 ）和|toIndex|（不包括）之间的部分视图。
|
|Object|[]
|toArray|()
|返回按适当顺序包含列表中的所有元素的数组（从第一个元素到最后一个元素）。
|
|<T> T[]
|
|toArray|(T[] a)
|返回按适当顺序（从第一个元素到最后一个元素）包含列表中所有元素的数组；返回数组的运行时类型是指定数组的运行时类型。
|
例子：
**public****class**CollectionTest6 {
**public****static****void**main(String[]args) {
//创建一个List集合
Listlist=**new**ArrayList();
list.add("松江");
list.add("流浪");
list.add("阿道夫");
System.**out**.println(list);
//在指定的索引处添加，索引从0开始,后面的元素都向后移一位
list.add(1,"吴浩");
System.**out**.println(list);
Listlist1=**new**ArrayList();
list1.add("松江1");
list1.add("流浪1");
list1.add("阿道夫1");
//集合添加集合时可以添加自己本身
//list.addAll(list);
list.addAll(list1);
System.**out**.println(list);
list.addAll(2,list1);
System.**out**.println(list);
//获得集合的长度
**int**size=list.size();
System.**out**.println("list集合的长度是："+size);
//根据索引来获得指定的元素
Objectobj=list.get(6);
System.**out**.println(obj);
//遍历list
**for**(**int**i= 0;i<list.size();i++) {
obj=list.get(i);
System.**out**.print(obj+",");
}
}
}
结果：
[松江,流浪,阿道夫]
[松江,吴浩,流浪,阿道夫]
[松江,吴浩,流浪,阿道夫,松江1,流浪1,阿道夫1]
[松江,吴浩,松江1,流浪1,阿道夫1,流浪,阿道夫,松江1,流浪1,阿道夫1]
list集合的长度是：10
阿道夫
松江,吴浩,松江1,流浪1,阿道夫1,流浪,阿道夫,松江1,流浪1,阿道夫1,
**-----------list****的元素删除方法****---------------------**
**例子：**
**public****static****void**main(String[]args) {
//创建一个List集合
Listlist=**new**ArrayList();
list.add("松江");
list.add("流浪");
list.add("阿道夫");
System.**out**.println(list);
//根据指定元素内容删除
list.remove("松江");
System.**out**.println(list);
//根据索引删除元素
list.remove(1);
System.**out**.println(list);
//清空list
list.clear();
System.**out**.println(list);
}
}
结果：
[松江,流浪,阿道夫]
[流浪,阿道夫]
[流浪]
[]

**----------------ListIterator---------------------------**
## java.util
## 接口 ListIterator<E>
**所有超级接口：**
Iterator<E>
---

public interface**ListIterator<E>**extendsIterator<E>系列表迭代器，允许程序员按任一方向遍历列表、迭代期间修改列表，并获得迭代器在列表中的当前位置。ListIterator没有当前元素；它的*光标位置*始终位于调用previous()所返回的元素和调用next()所返回的元素之间。长度为n的列表的迭代器有n+1个可能的指针位置，如下面的插入符举例说明：
Element(0)   Element(1)   Element(2)   ... Element(n-1)cursor positions:  ^            ^            ^            ^                  ^注意，`remove()`和`set(Object)`方法*不是*根据光标位置定义的；它们是根据对调用`next()`或`previous()`所返回的最后一个元素的操作定义的。
此接口是JavaCollections Framework的成员。
**从以下版本开始：**
1.2
**另请参见：**
`Collection`,`List`,`Iterator`,`Enumeration`,`List.listIterator()`
---

|方法摘要
|
|void
|add|(|E|e)
|将指定的元素插入列表（可选操作）。
|
|boolean
|hasNext|()
|以正向遍历列表时，如果列表迭代器有多个元素，则返回|true|（换句话说，如果|next|返回一个元素而不是抛出异常，则返回|true|）。
|
|boolean
|hasPrevious|()
|如果以逆向遍历列表，列表迭代器有多个元素，则返回|true|。
|
|E
|next|()
|返回列表中的下一个元素。
|
|int
|nextIndex|()
|返回对|next|的后续调用所返回元素的索引。
|
|E
|previous|()
|返回列表中的前一个元素。
|
|int
|previousIndex|()
|返回对|previous|的后续调用所返回元素的索引。
|
|void
|remove|()
|从列表中移除由|next|或|previous|返回的最后一个元素（可选操作）。
|
|void
|set|(|E|e)
|用指定元素替换|next|或|previous|返回的最后一个元素（可选操作）。
|
例子：
**public****class**CollectionTest7 {
**public****static****void**main(String[]args) {
//创建一个List集合
Listlist=**new**ArrayList();
list.add("松江");
list.add("流浪");
list.add("阿道夫");
System.**out**.println(list);
//使用ListIterator对集合进行遍历
ListIteratorli=list.listIterator();
**while**(li.hasNext()){
Objectobj=li.next();
System.**out**.println(obj);
}
**boolean**isHasPre=li.hasPrevious();
System.**out**.println("是否有前一个元素"+isHasPre);
System.**out**.println("------------------");
//对list集合倒序遍历
**while**(li.hasPrevious()){
Objectobj=li.previous();
System.**out**.println(obj);
}
System.**out**.println("------------------");
//ListIterator的迭代器只能出遍历创建迭代器对象之前的list的状态内容
ListIteratorli1=list.listIterator();
**while**(li1.hasNext()){
Objectobj=li1.next();
//对集合进行添加
**if**("流浪"==obj) {
//通过这个迭代器可以在list中添加元素，不会出现并发问题
li1.add("百胜");
System.**out**.println(list);
}
//在遍历中无法打印集合添加的元素，只有遍历完了以后才能打印
System.**out**.println(obj);
}
System.**out**.println(list);
System.**out**.println("------------------");
//此方法在循环中添加元素是不会打印出来的
//intsize = list.size();
//以下方法在循环中添加元素可以打印出来
**for**(**int**i= 0;i<list.size();i++) {
Objectobj=list.get(i);
**if**("流浪"==obj) {
//list中添加元素
//当在List增加元素后，list.size()的长度也改变了
list.add("时代城");
}
System.**out**.print(obj+",");
}
}
}
结果：
[松江,流浪,阿道夫]
松江
流浪
阿道夫
是否有前一个元素true
------------------
阿道夫
流浪
松江
------------------
松江
[松江,流浪,百胜,阿道夫]
流浪
阿道夫
[松江,流浪,百胜,阿道夫]
------------------
松江,流浪,百胜,阿道夫,时代城,


