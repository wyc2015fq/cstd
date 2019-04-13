
# java的set集合和子类HashSet - 李昆鹏的博客 - CSDN博客


2018年03月25日 06:13:43[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：748


**-----------------------java的set集合和子类HashSet----------------**
**set****特点：**
**1.****无序性**
**2.****不能重复**
**3.****允许有null****但是只能有一个**

**--------------------hashSet------------------**
此类实现Set接口，由哈希表（实际上是一个HashMap实列）支持。它不保证set的迭代顺序，特别是它不保证该顺序
恒久不变。此类允许使用null元素。
HashSet的唯一性：
在HashSet做添加的时候会逐个来判断当前集合中的对象和要添加的对象的比较
通过以下的条件判断两个对象是否相等
l**e.hash == hash&&((k == e.hash) == key || key.equals(k))**
l  hash值必须相等并且两个对象的地址值相等或者equals返回ture
![](https://img-blog.csdn.net/20180325061217364?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**List****集合是有序的，可以存储null****和相同的元素。**
**一个不包含重复元素的 collection****。更确切地讲，set 不包含满足**`e1.equals(e2)`**的元素对**`e1`**和**`e2`**，并且最多包含一个 null****元素。**
**Set****里面的方法绝大多数都是继承于collection****，所以set基本学完。**
**Set****的特点：**
**元素不可重复性**
**元素无序的（跟添加的顺序无关）**
**-------------- Set****的重复性---------------**
**Set****在添加元素的时候会拿这个元素和集合中的每一个元素做比较，如果重复了就不添加，**
**如果没有重复就添加。**
**判断两个对象是否相等必须重写equals****和hashCde方法，**
**这两个方法都符合相等的条件，才认为是相等的。**
例子：
/**
* set特点：
* 1.无序性
* 2.不能重复
* 3.允许有null但是只能有一个
***@****作者**李昆鹏
*
*/
**public****class**SetDemo {
**public****static****void**main(String[]args) {
//创建一个Set集合
Set<String>set=**new**HashSet<String>();
set.add("曹操");
set.add("刘备");
set.add("孙权");
set.add("曹操");
set.add("曹操");
set.add(**null**);
set.add(**null**);
System.**out**.println(set);
//创建一个Person类型Set集合
//判断两个对象是否相等必须重写equals和hashCde方法，
//这两个方法都符合相等的条件，才认为是相等的。
Set<Person>set1=**new**HashSet<Person>();
set1.add(**new**Person(1, 26,"诸葛亮"));
set1.add(**new**Person(2, 26,"庞统"));
set1.add(**new**Person(1, 26,"诸葛亮"));
System.**out**.println(set1);
}
}
结果：
[null,孙权,刘备, 曹操]
[Person [id=1, age=26, name=诸葛亮], Person [id=2,age=26, name=庞统]]

**-------------------------HashSet----------------------------**
**特点：**
**1.元素唯一性**
**2.无序性**
**3.允许null存在一个**
**4.不是线程安全（效率高）**
**例子：**
**package**cn.tx.set;
**public****class**Person {
**private****int**id;
**private****int**age;
**private**Stringname;

**public**Person(**int**id,**int**age, Stringname) {
**super**();
**this**.id=id;
**this**.age=age;
**this**.name=name;
}

@Override
**public**String toString() {
**return**"Person[id="+id+", age="+age+",name="+name+"]";
}

/**
*重写hashCode方法
*/
@Override
**public****int**hashCode() {
**return**1;
}
/**
*重写equals方法
*/
@Override
**public****boolean**equals(Objectobj) {
//判断是否是Person的实列
**if**(!(obj**instanceof**Person)){
**return****false**;
}
//把传递过来的对象转换成子类
Personperson=(Person)obj;
//如果两个person的所有属性都相等我们认为是同一个person
**if**(**this**.id==person.id&&
**this**.name!=**null**&&person.name!=**null**&&
**this**.name.equals(person.name)&&
**this**.age==person.age
){
**return****true**;
}**else**{
**return****false**;
}
}
**publi c****int**getId() {
**return**id;
}
**public****void**setId(**int**id) {
**this**.id=id;
}
**publi c****int**getAge(){
**return**age;
}
**public****void**setAge(**int**age) {
**this**.age=age;
}
**public**String getName() {
**return**name;
}
**public****void**setName(Stringname) {
**this**.name=name;
}
}
**-------------------------------------------**
**public****class**SetDemo2 {
**public****static****void**main(String[]args) {
//创建一个Person类型Set集合
//判断两个对象是否相等必须重写equals和hashCde方法，
//这两个方法都符合相等的条件，才认为是相等的。
Set<Person>set=**new**HashSet<Person>();
set.add(**new**Person(1, 26,"诸葛亮"));
set.add(**new**Person(2, 26,"庞统"));
set.add(**new**Person(3, 26,"诸葛亮"));
Iterator<Person>iter=set.iterator();
//set集合会出现ConcurrentModficationException
//如果添加相同的元素不会抛异常，做其他的操作会抛出异常
**while**(iter.hasNext()) {
Personp=iter.next();
**if**(p.getId()==1)
set.add(**new**Person(4, 26,"诸葛亮"));
System.**out**.println(p);
}
System.**out**.println(set);
}
}
结果：
Person [id=1, age=26, name=诸葛亮]
Exception in thread "main"java.util.ConcurrentModificationException
atjava.util.HashMap$HashIterator.nextNode(Unknown Source)
atjava.util.HashMap$KeyIterator.next(Unknown Source)
at cn.tx.set.SetDemo2.main(SetDemo2.java:29)



