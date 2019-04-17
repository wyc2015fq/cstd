# ArrayList 、LinkedList和Vector比较 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年04月09日 20:21:17[boonya](https://me.csdn.net/boonya)阅读数：490
个人分类：[Java Practice](https://blog.csdn.net/boonya/article/category/3089055)











原文出处： [泥沙砖瓦浆木匠-Jeff_Li](http://www.bysocket.com/?p=169)

## 一、List回顾


序列（List），有序的Collection，正如它的名字一样，是一个有序的元素列表。确切的讲，列表通常允许满足 e1.equals(e2) 的元素对 e1 和 e2，并且如果列表本身允许
 null 元素的话，通常它们允许多个 null 元素。实现List的有：ArrayList、LinkedList、Vector、Stack等。值得一提的是，Vector在JDK1.1的时候就有了，而List在JDK1.2的时候出现，待会我们会聊到ArrayList和Vector的区别。

## 二、ArrayList vs. Vector


ArrayList是一个可调整大小的数组实现的序列。随着元素增加，其大小会动态的增加。此类在Iterator或ListIterator迭代中，调用容器自身的remove和add方法进行修改，会抛出ConcurrentModificationException并发修改异常。


注意，此实现不是同步的。如果多个线程同时访问一个 ArrayList 实例，而其中至少一个线程从结构上修改了列表，那么它必须 保持外部同步。（结构上的修改是指任何添加或删除一个或多个元素的操作，或者显式调整底层数组的大小；仅仅设置元素的值不是结构上的修改。）这一般通过对自然封装该列表的对象进行同步操作来完成。如果不存在这样的对象，则应该使用 `Collections.synchronizedList` 方法将该列表“包装”起来。这最好在创建时完成，以防止意外对列表进行不同步的访问：

List list = Collections.synchronizedList(new ArrayList(…)); 

下面演示下相关ArrayList例子。


ArrayList基本方法代码：



```
```java
@SuppressWarnings
```

```java
({
```

```java
"rawtypes"
```

```java
,
```

```java
"unchecked"
```

```java
})
```



```java

```

```java
public
```

```java
static
```

```java
void
```

```java
listMethods()
```



```java

```

```java
{
```



```java

```



```java

```

```java
List
 a1 =
```

```java
new
```

```java
ArrayList<String>();
```



```java

```



```java

```

```java
a1.add(
```

```java
"List01"
```

```java
);
```



```java

```

```java
a1.add(
```

```java
"List03"
```

```java
);
```



```java

```

```java
a1.add(
```

```java
"List04"
```

```java
);
```



```java

```

```java
System.out.print(
```

```java
"原来集合：\n\t"
```

```java
+a1+
```

```java
"\n"
```

```java
);
```



```java

```



```java

```

```java
a1.add(
```

```java
1
```

```java
,
```

```java
"List02"
```

```java
);
```



```java

```

```java
System.out.print(
```

```java
"指定角标1插入：\n\t"
```

```java
+a1+
```

```java
"\n"
```

```java
);
```



```java

```



```java

```

```java
a1.remove(
```

```java
2
```

```java
);
```



```java

```

```java
System.out.print(
```

```java
"指定角标2删除：\n\t"
```

```java
+a1+
```

```java
"\n"
```

```java
);
```



```java

```



```java

```

```java
System.out.print(
```

```java
"指定角标2查询：\n\t"
```

```java
+a1.get(
```

```java
2
```

```java
)+
```

```java
"\n"
```

```java
);
```



```java

```



```java

```

```java
Iterator
 i1 = a1.iterator();
```



```java

```

```java
System.out.println(
```

```java
"用迭代器查询全部元素："
```

```java
);
```



```java

```

```java
while
```

```java
(i1.hasNext())
```



```java

```

```java
{
```



```java

```

```java
System.out.print(i1.next()+
```

```java
","
```

```java
);
```



```java

```

```java
}
```



```java

```

```java
}
```
```




可以从控制台可以看出：



```
```java
原来集合：
```



```java

```

```java
[List01,
 List03, List04]
```



```java
指定角标
```

```java
1
```

```java
插入：
```



```java

```

```java
[List01,
 List02, List03, List04]
```



```java
指定角标
```

```java
2
```

```java
删除：
```



```java

```

```java
[List01,
 List02, List04]
```



```java
指定角标
```

```java
2
```

```java
查询：
```



```java

```

```java
List04
```



```java
用迭代器查询全部元素：
```



```java
List01,List02,List04
```
```




在上面我们可以根据角标来增加(add)、删除(remove)、获取(get)列表里面元素。ArrayList提供了Iterator迭代器来遍历序列。值得注意的是，迭代器的就相当于一个指针指向角标，next()方法就相当于指针往后移一位。所以切记，用迭代器中一次循环用一次next()。


下面演示下在ConcurrentModificationException的出现，及处理方案。泥瓦匠用Iterator演示这个异常的出现：



```
```java
@SuppressWarnings
```

```java
({
 “unchecked”, “rawtypes” })
```



```java

```

```java
public
```

```java
static
```

```java
void
```

```java
iteratorTest()
```



```java

```

```java
{
```



```java

```

```java
List
 a1 =
```

```java
new
```

```java
ArrayList<String>();
```



```java

```



```java

```

```java
a1.add(“List01″);
```



```java

```

```java
a1.add(“List02″);
```



```java

```

```java
a1.add(“List04″);
```



```java

```

```java
a1.add(“List05″);
```



```java

```



```java

```

```java
Iterator
 i1 = a1.iterator();
```



```java

```

```java
while
```

```java
(i1.hasNext())
```



```java

```

```java
{
```



```java

```

```java
Object
 obj = i1.next();
```



```java

```

```java
if
```

```java
(obj.equals(“List02″))
```



```java

```

```java
a1.add(“List03″);
```



```java

```

```java
}
```



```java

```



```java

```

```java
System.out.print(“集合：\n\t”+a1+”\n”);
```



```java

```

```java
}
```
```




运行，我们可以在控制台看到：

![](http://incdn1.b0.upaiyun.com/2015/04/545bccf41d58f5db5ba6e023c826a5a5.png)


怎么解决的，先看清楚这个问题。问题描述很清楚，在创建迭代器之后，除非通过迭代器自身的 remove 或 add 方法从结构上对列表进行修改，否则在任何时间以任何方式对列表进行修改，迭代器都会抛出`ConcurrentModificationException`。


因此我们应该这样修改代码，用ListIterator迭代器提供方法：



```
```java
@SuppressWarnings
```

```java
({
```

```java
"unchecked"
```

```java
,
```

```java
"rawtypes"
```

```java
})
```



```java

```

```java
public
```

```java
static
```

```java
void
```

```java
listIterator()
```



```java

```

```java
{
```



```java

```



```java

```

```java
List
 a1 =
```

```java
new
```

```java
ArrayList<String>();
```



```java

```



```java

```

```java
a1.add(
```

```java
"List01"
```

```java
);
```



```java

```

```java
a1.add(
```

```java
"List"
```

```java
);
```



```java

```

```java
a1.add(
```

```java
"List03"
```

```java
);
```



```java

```

```java
a1.add(
```

```java
"List04"
```

```java
);
```



```java

```



```java

```

```java
ListIterator
 l1 = a1.listIterator();
```



```java

```

```java
while
```

```java
(l1.hasNext())
```



```java

```

```java
{
```



```java

```

```java
Object
 obj = l1.next();
```



```java

```

```java
if
```

```java
(obj.equals(
```

```java
"List"
```

```java
))
```



```java

```

```java
{
```



```java

```

```java
l1.remove();
```



```java

```

```java
l1.add(
```

```java
"List02"
```

```java
);
```



```java

```

```java
}
```



```java

```

```java
}
```



```java

```

```java
System.out.print(
```

```java
"集合：\n\t"
```

```java
+a1+
```

```java
"\n"
```

```java
);
```



```java

```

```java
}
```
```




运行下，我们可以看到：



```
`集合：`

`    `
```
[List01,
 List02, List03, List04]
```
```




这样，我们成功解决了这个并发修改异常。把其中‘List’元素删除，新增了一个‘List02’的元素。


Vector非常类似ArrayList。早在JDK1.1的时候就出现了，以前没有所谓的List接口，现在此类被改进为实现List接口。但与新的Collection不同的是，Vector是同步的。泥瓦匠想说的是Vector，在像查询的性能上会比ArrayList开销大。下面演示下Vector的基本例子：



```
```java
@SuppressWarnings
```

```java
({
```

```java
"unchecked"
```

```java
,
```

```java
"rawtypes"
```

```java
})
```



```java

```

```java
public
```

```java
static
```

```java
void
```

```java
vectorMethods()
```



```java

```

```java
{
```



```java

```

```java
Vector
 v1 =
```

```java
new
```

```java
Vector<String>();
```



```java

```



```java

```

```java
v1.add(
```

```java
"Vector001"
```

```java
);
```



```java

```

```java
v1.add(
```

```java
"Vector002"
```

```java
);
```



```java

```

```java
v1.add(
```

```java
"Vector003"
```

```java
);
```



```java

```

```java
v1.add(
```

```java
"Vector004"
```

```java
);
```



```java

```

```java
v1.add(
```

```java
"Vector005"
```

```java
);
```



```java

```



```java

```

```java
Enumeration
 e1 =v1.elements();
```



```java

```

```java
while
```

```java
(e1.hasMoreElements())
```



```java

```

```java
{
```



```java

```

```java
Object
 object = e1.nextElement();
```



```java

```

```java
System.out.println(object);
```



```java

```

```java
}
```



```java

```

```java
}
```
```




从方法上看几乎没差别，同样注意的是：此接口的功能与 Iterator 接口的功能是重复的。此外，Iterator 接口添加了一个可选的移除操作，并使用较短的方法名。新的实现应该优先考虑使用 Iterator 接口而不是 Enumeration 接口。

## 三、LinkedList及其与ArrayList性能比


LinkedList与ArrayList一样实现List接口，LinkedList是List接口链表的实现。基于链表实现的方式使得LinkedList在插入和删除时更优于ArrayList，而随机访问则比ArrayList逊色些。LinkedList实现所有可选的列表操作，并允许所有的元素包括null。除了实现 List 接口外，LinkedList 类还为在列表的开头及结尾 get、remove 和 insert 元素提供了统一的命名方法。这些操作允许将链接列表用作堆栈、队列或双端队列。


LinkedList和ArrayList的方法时间复杂度总结如下图所示。

![](http://incdn1.b0.upaiyun.com/2015/04/a0d27afd721a8edd5576297b0430019c.png)


表中，添加add()指添加元素的方法，remove()是指除去（int index）角标。ArrayList具有O（N）的任意指数时间复杂度的添加/删除，但O（1）的操作列表的末尾。链表的O（n）的任意指数时间复杂度的添加/删除，但O（1）操作端/列表的开始。


泥瓦匠用代码验证下这个结论：



```
```java
public
```

```java
static
```

```java
void
```

```java
testPerBtwnArlAndLkl()
```



```java

```

```java
{
```



```java

```

```java
ArrayList<Integer>
 arrayList   =
```

```java
new
```

```java
ArrayList<Integer>();
```



```java

```

```java
LinkedList<Integer>
 linkedList =
```

```java
new
```

```java
LinkedList<Integer>();
```



```java

```



```java

```

```java
//
 ArrayList add
```



```java

```

```java
long
```

```java
startTime  = System.nanoTime();
```



```java

```

```java
long
```

```java
endTime;
```



```java

```

```java
long
```

```java
duration;
```



```java

```



```java

```

```java
for
```

```java
(
```

```java
int
```

```java
i =
```

```java
0
```

```java
;
 i <
```

```java
100000
```

```java
;
 i++) {
```



```java

```

```java
arrayList.add(i);
```



```java

```

```java
}
```



```java

```

```java
endTime
 = System.nanoTime();
```



```java

```

```java
duration
 = endTime - startTime;
```



```java

```

```java
System.out.println(
```

```java
"ArrayList
 add:  "
```

```java
+ duration);
```



```java

```



```java

```

```java
//
 LinkedList add
```



```java

```

```java
startTime
 = System.nanoTime();
```



```java

```



```java

```

```java
for
```

```java
(
```

```java
int
```

```java
i =
```

```java
0
```

```java
;
 i <
```

```java
100000
```

```java
;
 i++) {
```



```java

```

```java
linkedList.add(i);
```



```java

```

```java
}
```



```java

```

```java
endTime
 = System.nanoTime();
```



```java

```

```java
duration
 = endTime - startTime;
```



```java

```

```java
System.out.println(
```

```java
"LinkedList
 add: "
```

```java
+ duration);
```



```java

```



```java

```

```java
//
 ArrayList get
```



```java

```

```java
startTime
 = System.nanoTime();
```



```java

```



```java

```

```java
for
```

```java
(
```

```java
int
```

```java
i =
```

```java
0
```

```java
;
 i <
```

```java
10000
```

```java
;
 i++) {
```



```java

```

```java
arrayList.get(i);
```



```java

```

```java
}
```



```java

```

```java
endTime
 = System.nanoTime();
```



```java

```

```java
duration
 = endTime - startTime;
```



```java

```

```java
System.out.println(
```

```java
"ArrayList
 get:  "
```

```java
+ duration);
```



```java

```



```java

```

```java
//
 LinkedList get
```



```java

```

```java
startTime
 = System.nanoTime();
```



```java

```



```java

```

```java
for
```

```java
(
```

```java
int
```

```java
i =
```

```java
0
```

```java
;
 i <
```

```java
10000
```

```java
;
 i++) {
```



```java

```

```java
linkedList.get(i);
```



```java

```

```java
}
```



```java

```

```java
endTime
 = System.nanoTime();
```



```java

```

```java
duration
 = endTime - startTime;
```



```java

```

```java
System.out.println(
```

```java
"LinkedList
 get: "
```

```java
+ duration);
```



```java

```



```java

```

```java
//
 ArrayList remove
```



```java

```

```java
startTime
 = System.nanoTime();
```



```java

```



```java

```

```java
for
```

```java
(
```

```java
int
```

```java
i =
```

```java
9999
```

```java
;
 i >=
```

```java
0
```

```java
;
 i--) {
```



```java

```

```java
arrayList.remove(i);
```



```java

```

```java
}
```



```java

```

```java
endTime
 = System.nanoTime();
```



```java

```

```java
duration
 = endTime - startTime;
```



```java

```

```java
System.out.println(
```

```java
"ArrayList
 remove:  "
```

```java
+ duration);
```



```java

```



```java

```

```java
//
 LinkedList remove
```



```java

```

```java
startTime
 = System.nanoTime();
```



```java

```



```java

```

```java
for
```

```java
(
```

```java
int
```

```java
i =
```

```java
9999
```

```java
;
 i >=
```

```java
0
```

```java
;
 i--) {
```



```java

```

```java
linkedList.remove(i);
```



```java

```

```java
}
```



```java

```

```java
endTime
 = System.nanoTime();
```



```java

```

```java
duration
 = endTime - startTime;
```



```java

```

```java
System.out.println(
```

```java
"LinkedList
 remove: "
```

```java
+ duration);
```



```java

```

```java
}
```
```




控制台输出如下：



```
```
ArrayList
 add:
```
`16904776`


```
LinkedList
 add:
```
`12015418`


```
ArrayList
```
`get`
```
:
```
`1304593`


```
LinkedList
```
`get`
```
:
```
`108950741`


```
ArrayList
 remove:
```
`787388127`


```
LinkedList
 remove:
```
`128145950`
```




对比下的话，其性能差距很明显。LinkedList在添加和删除中性能快，但在获取中性能差。从复杂度和测试结果，我们应该懂得平时在添加或者删除操作频繁的地方，选择LinkedList时考虑：


1、没有大量的元素的随机访问


2、添加/删除操作


自然我下面用LinedList实现一个数据结构–栈。泥瓦匠留给大家LinkedList的一些方法自己消化下。



```
```java
package
```

```java
com.sedion.bysocket.collection;
```



```java
import
```

```java
java.util.LinkedList;
```



```java

```



```java
/**
```



```java

```

```java
*
 用LinkedList实现栈
```



```java

```

```java
*
 队列和栈区别：队列先进先出，栈先进后出。
```



```java

```

```java
*/
```



```java
public
```

```java
class
```

```java
Stack<T>
```



```java
{
```



```java

```

```java
private
```

```java
LinkedList<T> storage =
```

```java
new
```

```java
LinkedList<T>();
```



```java

```



```java

```

```java
/**
 入栈 */
```



```java

```

```java
public
```

```java
void
```

```java
push(T v)
```



```java

```

```java
{
```



```java

```

```java
storage.addFirst(v);
```



```java

```

```java
}
```



```java

```



```java

```

```java
/**
 出栈，但不删除 */
```



```java

```

```java
public
```

```java
T peek()
```



```java

```

```java
{
```



```java

```

```java
return
```

```java
storage.getFirst();
```



```java

```

```java
}
```



```java

```



```java

```

```java
/**
 出栈，删除 */
```



```java

```

```java
public
```

```java
T pop()
```



```java

```

```java
{
```



```java

```

```java
return
```

```java
storage.removeFirst();
```



```java

```

```java
}
```



```java

```



```java

```

```java
/**
 栈是否为空 */
```



```java

```

```java
public
```

```java
boolean
```

```java
empty()
```



```java

```

```java
{
```



```java

```

```java
return
```

```java
storage.isEmpty();
```



```java

```

```java
}
```



```java

```



```java

```

```java
/**
 输出栈元素 */
```



```java

```

```java
public
```

```java
String toString()
```



```java

```

```java
{
```



```java

```

```java
return
```

```java
storage.toString();
```



```java

```

```java
}
```



```java

```



```java

```

```java
public
```

```java
static
```

```java
void
```

```java
main(String[] args)
```



```java

```

```java
{
```



```java

```

```java
Stack
 stack=
```

```java
new
```

```java
Stack<String>();
```



```java

```

```java
stack.push(
```

```java
"a"
```

```java
);
```



```java

```

```java
stack.push(
```

```java
"b"
```

```java
);
```



```java

```

```java
stack.push(
```

```java
"c"
```

```java
);
```



```java

```

```java
System.out.println(stack.toString());
```



```java

```

```java
Object
 obj=stack.peek();
```



```java

```

```java
System.out.println(obj+
```

```java
"--"
```

```java
+stack.toString());
```



```java

```

```java
obj=stack.pop();
```



```java

```

```java
System.out.println(obj+
```

```java
"--"
```

```java
+stack.toString());
```



```java

```

```java
System.out.println(stack.empty());
```



```java

```

```java
}
```



```java
}
```
```



## 四、总结


泥瓦匠总结如下：


Vector和ArrayList


1、vector是线程同步的，所以它也是线程安全的，而arraylist是线程异步的，是不安全的。


2、记住并发修改异常 java.util.ConcurrentModificationException ，优先考虑ArrayList，除非你在使用多线程所需。


Aarraylist和Linkedlist

1、对于随机访问get和set，ArrayList觉得优于LinkedList，LinkedList要移动指针。

2、于新增和删除操作add和remove，LinedList比较占优势，ArrayList要移动数据。

3、单条数据插入或删除，ArrayList的速度反而优于LinkedList.若是批量随机的插入删除数据，LinkedList的速度大大优于ArrayList. 因为ArrayList每插入一条数据，要移动插入点及之后的所有数据。
[](http://www.jiathis.com/share?uid=1745061)





