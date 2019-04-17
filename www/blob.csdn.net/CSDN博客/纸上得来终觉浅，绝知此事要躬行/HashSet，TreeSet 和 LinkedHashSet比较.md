# HashSet，TreeSet 和 LinkedHashSet比较 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年04月09日 21:36:57[boonya](https://me.csdn.net/boonya)阅读数：574










原文出处： [泥沙砖瓦浆木匠-Jeff_Li](http://www.bysocket.com/?p=195)

## 一、Set回顾


一个不包括重复元素（包括可变对象）的Collection，是一种无序的集合。Set不包含满 a.equals(b) 的元素对a和b，并且最多有一个null。


泥瓦匠的记忆宫殿：


1、不允许包含相同元素


2、判断对象是否相同，根据equals方法

![](http://incdn1.b0.upaiyun.com/2015/04/b4450c48c83c37d33ca062828083a3e9.jpg)

## 二、HashSet


一个按着Hash算法来存储集合中的元素，其元素值可以是NULL。它不能保证元素的排列顺序。同样，HashSet是不同步的，如果需要多线程访问它的话，可以用 Collections.synchronizedSet 方法来包装它：



```
```java
Set
 s = Collections.synchronizedSet(
```

```java
new
```

```java
HashSet(...));
```
```




同上一节一样，用迭代器的时候，也要注意 并发修改异常ConcurrentModificationException。



要注意的地方是，HashSet集合判断两个元素相等不单单是equals方法，并且必须hashCode()方法返回值也要相等。看下面的例子：



```
```java
import
```

```java
java.util.HashSet;
```



```java

```



```java
class
```

```java
EuqalsObj
```



```java
{
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
equals(Object obj)
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
true
```

```java
;
```



```java

```

```java
}
```



```java
}
```



```java

```



```java
class
```

```java
HashCodeObj
```



```java
{
```



```java

```

```java
public
```

```java
int
```

```java
hashCode()
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
1
```

```java
;
```



```java

```

```java
}
```



```java
}
```



```java

```



```java
class
```

```java
HashSetObj
```



```java
{
```



```java

```

```java
public
```

```java
int
```

```java
hashCode()
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
2
```

```java
;
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
boolean
```

```java
equals(Object obj)
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
true
```

```java
;
```



```java

```

```java
}
```



```java
}
```



```java

```



```java
public
```

```java
class
```

```java
HashSetTest
```



```java
{
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
HashSet
 objs =
```

```java
new
```

```java
HashSet();
```



```java

```

```java
objs.add(
```

```java
new
```

```java
EuqalsObj());
```



```java

```

```java
objs.add(
```

```java
new
```

```java
EuqalsObj());
```



```java

```

```java
objs.add(
```

```java
new
```

```java
HashCodeObj());
```



```java

```

```java
objs.add(
```

```java
new
```

```java
HashCodeObj());
```



```java

```

```java
objs.add(
```

```java
new
```

```java
HashSetObj());
```



```java

```

```java
objs.add(
```

```java
new
```

```java
HashSetObj());
```



```java

```



```java

```

```java
System.out.println(
```

```java
"HashSet
 Elements:"
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
"\t"
```

```java
+ objs +
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



```java
}
```
```




Run 一下，控制台如下输出：



```
```
HashSet
 Elements:
```


`    ``[HashCodeObj@``1`
```
,
 HashCodeObj@
```
`1`
```
,
 HashSetObj@
```
`2`
```
,
 EuqalsObj@1471cb25, EuqalsObj@3acff49f]
```
```




泥瓦匠根据结果，一一到来。首先，排列顺序不定。


HashSetObj 类满足我们刚刚的要求，所以集合中只有一个且它的HashCode值为2。


HashCodeObj 类虽然它们HashCode值为1，但是他们不相等。（其实当HashCode值一样，这个存储位置会采用链式结构保存两个HashCodeObj对象。）


同样,EqualsObj 类他们相等，但是他们HashCode值不等，分别为1471cb25、3acff49f。


因此，用HashSet添加可变对象，要注意当对象有可能修改后和其他对象矛盾，这样我们无法从HashSet找到准确我们需要的对象。

## 三、LinkedHashList


HashSet的子类，也同样有HashCode值来决定元素位置。但是它使用链表维护元素的次序。记住两个字：有序。


有序的妙用，复制。比如泥瓦匠实现一个HashSet无序添加，然后复制一个一样次序的HashSet来。代码如下：



```
```java
package
```

```java
com.sedion.bysocket.collection;
```



```java

```



```java
import
```

```java
java.util.HashSet;
```



```java
import
```

```java
java.util.LinkedHashSet;
```



```java
import
```

```java
java.util.Set;
```



```java

```



```java
public
```

```java
class
```

```java
LinkedHashListTest
```



```java
{
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
/*
 复制HashSet */
```



```java

```

```java
Set
 h1 =
```

```java
new
```

```java
HashSet<String>();
```



```java

```

```java
h1.add(
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
h1.add(
```

```java
"Queue"
```

```java
);
```



```java

```

```java
h1.add(
```

```java
"Set"
```

```java
);
```



```java

```

```java
h1.add(
```

```java
"Map"
```

```java
);
```



```java

```



```java

```

```java
System.out.println(
```

```java
"HashSet
 Elements:"
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
"\t"
```

```java
+ h1 +
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
Set
 h2 = copy(h1);
```



```java

```

```java
System.out.println(
```

```java
"HashSet
 Elements After Copy:"
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
"\t"
```

```java
+ h2 +
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



```java

```



```java

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
Set copy(Set set)
```



```java

```

```java
{
```



```java

```

```java
Set
 setCopy =
```

```java
new
```

```java
LinkedHashSet(set);
```



```java

```

```java
return
```

```java
setCopy;
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




Run 一下，控制台输出：



```
```
HashSet
 Elements:
```


`    `
```
[Map,
 Queue, Set, List]
```



```
HashSet
 Elements After Copy:
```


`    `
```
[Map,
 Queue, Set, List]
```
```




可见，每个数据结构都有它存在的理由。

## 四、TreeSet


TreeSet使用树结构实现（红黑树），集合中的元素进行排序，但是添加、删除和包含的算法复杂度为O（log（n））。


举个例子吧，首先我们定义一个Bird类。（鸟是泥瓦匠最喜欢的动物）



```
```java
class
```

```java
Bird
```



```java
{
```



```java

```

```java
int
```

```java
size;
```



```java

```



```java

```

```java
public
```

```java
Bird(
```

```java
int
```

```java
s)
```



```java

```

```java
{
```



```java

```

```java
size
 = s;
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
size +
```

```java
""
```

```java
;
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




然后用TreeSet添加Bird类。



```
```java
public
```

```java
class
```

```java
TreeSetTest
```



```java
{
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
TreeSet<Bird>
 bSet =
```

```java
new
```

```java
TreeSet<Bird>();
```



```java

```

```java
bSet.add(
```

```java
new
```

```java
Bird(
```

```java
1
```

```java
));
```



```java

```

```java
bSet.add(
```

```java
new
```

```java
Bird(
```

```java
3
```

```java
));
```



```java

```

```java
bSet.add(
```

```java
new
```

```java
Bird(
```

```java
2
```

```java
));
```



```java

```



```java

```

```java
Iterator<Bird>
 iter = bSet.iterator();
```



```java

```



```java

```

```java
while
```

```java
(iter.hasNext())
```



```java

```

```java
{
```



```java

```

```java
Bird
 bird = (Bird) iter.next();
```



```java

```

```java
System.out.println(bird);
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
}
```
```




Run一下,控制台输出如下：



```
```
Exception
```
`in`
```
thread
```
`"main"`
```
java.lang.ClassCastException: Bird cannot be cast to java.lang.Comparable
```


`    `
```
at
 java.util.TreeMap.compare(Unknown Source)
```


`    `
```
at
 java.util.TreeMap.put(Unknown Source)
```


`    `
```
at
 java.util.TreeSet.add(Unknown Source)
```


`    `
```
at
 com.sedion.bysocket.collection.TreeSetTest.main(TreeSetTest.java:
```
`29``)`
```




答案很明显，TreeSet是排序的。所以Bird需要实现Comparable此接口。


java.lang.Comparable此接口强行对实现它的每个类的对象进行整体排序。这种排序被称为类的自然排序，类的 compareTo 方法被称为它的自然比较方法。


修改Bird如下：



```
```java
class
```

```java
Bird
```

```java
implements
```

```java
Comparable<Bird>
```



```java
{
```



```java

```

```java
int
```

```java
size;
```



```java

```



```java

```

```java
public
```

```java
Bird(
```

```java
int
```

```java
s)
```



```java

```

```java
{
```



```java

```

```java
size
 = s;
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
size +
```

```java
"号鸟"
```

```java
;
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
@Override
```



```java

```

```java
public
```

```java
int
```

```java
compareTo(Bird o)
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
size - o.size;
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




再次Run一下：



```
`1``号鸟`

`2``号鸟`

`3``号鸟`
```




五、性能测试比较


针对上面三种Set集合，我们对它们的Add方法进行性能测试：



```
```java
import
```

```java
java.util.HashSet;
```



```java
import
```

```java
java.util.LinkedHashSet;
```



```java
import
```

```java
java.util.Random;
```



```java
import
```

```java
java.util.TreeSet;
```



```java

```



```java
class
```

```java
Bird
```

```java
implements
```

```java
Comparable<Bird>
```



```java
{
```



```java

```

```java
int
```

```java
size;
```



```java

```



```java

```

```java
public
```

```java
Bird(
```

```java
int
```

```java
s)
```



```java

```

```java
{
```



```java

```

```java
size
 = s;
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
size +
```

```java
"号鸟"
```

```java
;
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
@Override
```



```java

```

```java
public
```

```java
int
```

```java
compareTo(Bird o)
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
size - o.size;
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
public
```

```java
class
```

```java
Set
```



```java
{
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
Random
 r =
```

```java
new
```

```java
Random();
```



```java

```



```java

```

```java
HashSet<Bird>
 hashSet =
```

```java
new
```

```java
HashSet<Bird>();
```



```java

```

```java
TreeSet<Bird>
 treeSet =
```

```java
new
```

```java
TreeSet<Bird>();
```



```java

```

```java
LinkedHashSet<Bird>
 linkedSet =
```

```java
new
```

```java
LinkedHashSet<Bird>();
```



```java

```



```java

```

```java
//
 start time
```



```java

```

```java
long
```

```java
startTime = System.nanoTime();
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
1000
```

```java
;
 i++) {
```



```java

```

```java
int
```

```java
x = r.nextInt(
```

```java
1000
```

```java
-
```

```java
10
```

```java
)
 +
```

```java
10
```

```java
;
```



```java

```

```java
hashSet.add(
```

```java
new
```

```java
Bird(x));
```



```java

```

```java
}
```



```java

```

```java
//
 end time
```



```java

```

```java
long
```

```java
endTime = System.nanoTime();
```



```java

```

```java
long
```

```java
duration = endTime - startTime;
```



```java

```

```java
System.out.println(
```

```java
"HashSet:
 "
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
 start time
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
1000
```

```java
;
 i++) {
```



```java

```

```java
int
```

```java
x = r.nextInt(
```

```java
1000
```

```java
-
```

```java
10
```

```java
)
 +
```

```java
10
```

```java
;
```



```java

```

```java
treeSet.add(
```

```java
new
```

```java
Bird(x));
```



```java

```

```java
}
```



```java

```

```java
//
 end time
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
"TreeSet:
 "
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
 start time
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
1000
```

```java
;
 i++) {
```



```java

```

```java
int
```

```java
x = r.nextInt(
```

```java
1000
```

```java
-
```

```java
10
```

```java
)
 +
```

```java
10
```

```java
;
```



```java

```

```java
linkedSet.add(
```

```java
new
```

```java
Bird(x));
```



```java

```

```java
}
```



```java

```

```java
//
 end time
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
"LinkedHashSet:
 "
```

```java
+ duration);
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




Run一下，可以在控制台中看出：



```
```
HashSet:
```
`2610998`


```
TreeSet:
```
`3195378`


```
LinkedHashSet:
```
`2673782`
```




可见，TreeSet因为需要进行比较，所以性能比较差。

## 六、总结


HashSet：equlas hashcode


LinkedHashSet：链式结构


TreeSet：比较，Comparable接口，性能较差


[](http://www.jiathis.com/share?uid=1745061)



