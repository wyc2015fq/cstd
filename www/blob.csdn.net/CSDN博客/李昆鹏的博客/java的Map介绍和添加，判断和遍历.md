
# java的Map介绍和添加，判断和遍历 - 李昆鹏的博客 - CSDN博客


2018年04月03日 10:41:48[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：50标签：[java																](https://so.csdn.net/so/search/s.do?q=java&t=blog)个人分类：[JAVASE																](https://blog.csdn.net/weixin_41547486/article/category/7484968)


**-----------------------java的Map介绍和添加，判断和遍历-----------------------**
**java.util**
**接口 Map<K,V>**
**类型参数：**
K - 此映射所维护的键的类型
V - 映射值的类型
**所有已知子接口：**
Bindings,ConcurrentMap<K,V>,ConcurrentNavigableMap<K,V>,LogicalMessageContext,MessageContext,NavigableMap<K,V>,SOAPMessageContext,SortedMap<K,V>
**所有已知实现类：**
AbstractMap,Attributes,AuthProvider,ConcurrentHashMap,ConcurrentSkipListMap,EnumMap,HashMap,Hashtable,IdentityHashMap,LinkedHashMap,PrinterStateReasons,Properties,Provider,RenderingHints,SimpleBindings,TabularDataSupport,TreeMap,UIDefaults,WeakHashMap
---

publicinterface**Map<K,V>**
将键映射到值的对象。一个映射不能包含重复的键；每个键最多只能映射到一个值。
此接口取代 Dictionary 类，后者完全是一个抽象类，而不是一个接口。
Map 接口提供三种*collection 视图*，允许以键集、值集或键-值映射关系集的形式查看某个映射的内容。映射*顺序*定义为迭代器在映射的collection 视图上返回其元素的顺序。某些映射实现可明确保证其顺序，如 TreeMap 类；另一些映射实现则不保证顺序，如 HashMap 类。
注：将可变对象用作映射键时必须格外小心。当对象是映射中某个键时，如果以影响 equals 比较的方式更改了对象的值，则映射的行为将是不确定的。此项禁止的一种特殊情况是不允许某个映射将自身作为一个键包含。虽然允许某个映射将自身作为值包含，但请格外小心：在这样的映射上equals 和 hashCode 方法的定义将不再是明确的。
所有通用的映射实现类应该提供两个“标准的”构造方法：一个 void（无参数）构造方法，用于创建空映射；一个是带有单个Map 类型参数的构造方法，用于创建一个与其参数具有相同键-值映射关系的新映射。实际上，后一个构造方法允许用户复制任意映射，生成所需类的一个等价映射。尽管无法强制执行此建议（因为接口不能包含构造方法），但是 JDK 中所有通用的映射实现都遵从它。
此接口中包含的“破坏”方法可修改其操作的映射，如果此映射不支持该操作，这些方法将抛出 UnsupportedOperationException。如果是这样，那么在调用对映射无效时，这些方法可以（但不要求）抛出 UnsupportedOperationException。例如，如果某个不可修改的映射（其映射关系是“重叠”的）为空，则对该映射调用putAll(Map)方法时，可以（但不要求）抛出异常。
某些映射实现对可能包含的键和值有所限制。例如，某些实现禁止 null 键和值，另一些则对其键的类型有限制。尝试插入不合格的键或值将抛出一个未经检查的异常，通常是 NullPointerException 或 ClassCastException。试图查询是否存在不合格的键或值可能抛出异常，或者返回 false；某些实现将表现出前一种行为，而另一些则表现后一种。一般来说，试图对不合格的键或值执行操作且该操作的完成不会导致不合格的元素被插入映射中时，将可能抛出一个异常，也可能操作成功，这取决于实现本身。这样的异常在此接口的规范中标记为“可选”。
此接口是JavaCollections Framework的成员。
CollectionsFramework 接口中的很多方法是根据equals方法定义的。例如，containsKey(Object key)方法的规范中写道：“当且仅当此映射包含针对满足 (key==null ?k==null : key.equals(k)) 的键 k 的映射关系时，返回 true”。*不*应将此规范解释为：调用具有非空参数key 的 Map.containsKey 将导致对任意的键 k 调用 key.equals(k)。实现可随意进行优化，以避免调用 equals，例如，可首先比较两个键的哈希码（Object.hashCode()规范保证哈希码不相等的两个对象不会相等）。一般来说，只要实现者认为合适，各种 Collections Framework 接口的实现可随意利用底层Object方法的指定行为。
**从以下版本开始：**
1.2
**另请参见：**
HashMap,TreeMap,Hashtable,SortedMap,Collection,Set
---

|嵌套类摘要
|
|static interface
|Map.Entry|<|K|,|V|>
|映射项（键-值对）。
|
|方法摘要
|
|void
|clear|()
|从此映射中移除所有映射关系（可选操作）。
|
|boolean
|containsKey|(|Object|key)
|如果此映射包含指定键的映射关系，则返回  true。
|
|boolean
|containsValue|(|Object|value)
|如果此映射将一个或多个键映射到指定值，则返回  true。
|
|Set|<|Map.Entry|<|K|,|V|>>
|entrySet|()
|返回此映射中包含的映射关系的|Set|视图。
|
|boolean
|equals|(|Object|o)
|比较指定的对象与此映射是否相等。
|
|V
|get|(|Object|key)
|返回指定键所映射的值；如果此映射不包含该键的映射关系，则返回  null。
|
|int
|hashCode|()
|返回此映射的哈希码值。
|
|boolean
|isEmpty|()
|如果此映射未包含键-值映射关系，则返回 true。
|
|Set|<|K|>
|keySet|()
|返回此映射中包含的键的|Set|视图。
|
|V
|put|(|K|key,|V|value)
|将指定的值与此映射中的指定键关联（可选操作）。
|
|void
|putAll|(|Map|<? extends|K|,? extends|V|> m)
|从指定映射中将所有映射关系复制到此映射中（可选操作）。
|
|V
|remove|(|Object|key)
|如果存在一个键的映射关系，则将其从此映射中移除（可选操作）。
|
|int
|size|()
|返回此映射中的键-值映射关系数。
|
|Collection|<|V|>
|values|()
|返回此映射中包含的值的|Collection|视图。
|

Map就是键值对的集合
Map的常用实现类：
HashMap
TreeMap
HashTable
linkedHashMap
Map就是键值对的集合的借口抽象
![](https://img-blog.csdn.net/20180403103715168?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
我们可以通过Key的值来获得相应的值，将键映射到值的对象。
一个映射不能包含重复的键，每个键最多只能映射到一个值，
键只允许有一个空值，值可以有多个空值。
注意：Map集合和之前学过的Collection接口无关
Map的特点
1.  数据成对出现
2.  数据的键是唯一的
3.  一个键只能对应一个值
4.  值可以重复
5.  键允许有一个位空，值可以多个空
面试题
问：map集合和Collection集合的区别？
答：1.map集合是键值对的集合，Collection是单一出现的数据的集合。
2.map的键是唯一的，而Collection的子接口list集合中的元素是可以重复的，set是唯一的。
3.map是夫妻对，Collection是光棍。
![](https://img-blog.csdn.net/20180403103725200?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
hashMap常用方法的例子：

**public****class**MapDemo {
**public****static****void**main(String[]args) {
//定义一个map
Map<String,String>map=**new**HashMap<String,String>();
//放入键值对
map.put("鼓上骚","时迁");
map.put("行者","武松");
//keyd的键是不能重复的，如果有相同的键会选择最后面一个
map.put("行者","武大郎");
//值是可以重复的
map.put("及时雨","松江");
map.put("付宝义","江");
map.put("付宝义","松江");
//键允许有一个是空的，就算有两个空的也只会显示一个，最后面一个
map.put(**null**,"宋江");
map.put(**null**,"江");

//值可以有多个是空的
map.put("赤发鬼",**null**);
map.put("黑旋风",**null**);
System.**out**.println(map);
//再定义一个Map集合
Map<String,String>map1=**new**HashMap<String,String>();
map1.put("花和尚","鲁智深");
map1.put("青面兽","杨志");
//把一个Map内容加入到另一个Map中
map.putAll(map1);
System.**out**.println(map);
//清空map
//map.clear();
//根据key删除指定的值
map.remove("青面兽");
System.**out**.println(map);
//根据key获得相应的值，如果没有相应的key就返回null
Stringvalue=map.get("行者");
System.**out**.println(value);
//拿到map中所有的键
Set<String>set=map.keySet();
System.**out**.println(set);
//根据key的集合来获得每一个key对应的value
**for**(Stringkey:set) {
Stringvalue1=map.get(key);
System.**out**.print(value1+"、");
}
System.**out**.println();
//输出
//     {null=江,赤发鬼=null,黑旋风=null,付宝义=松江,行者=武大郎,及时雨=松江,鼓上骚=时迁}
//     {null=江,赤发鬼=null,黑旋风=null,付宝义=松江,青面兽=杨志,行者=武大郎,花和尚=鲁智深,及时雨=松江,鼓上骚=时迁}
//     {null=江,赤发鬼=null,黑旋风=null,付宝义=松江,行者=武大郎,花和尚=鲁智深,及时雨=松江,鼓上骚=时迁}
//武大郎
//     [null,赤发鬼,黑旋风,付宝义,行者,花和尚,及时雨,鼓上骚]
//江、null、null、松江、武大郎、鲁智深、松江、时迁、
}
}
------------------------------------------------------
创建一个Person类
**public****class**Person {
**private****int**id;
**private**Stringname;
**private****int**age;
**public****int**getId() {
**return**id;
}
**public****void**setId(**int**id) {
**this**.id=id;
}
**public**Person(**int**id, Stringname,**int**age) {
**super**();
**this**.id=id;
**this**.name=name;
**this**.age=age;
}
@Override
**public**String  toString() {
**return**"Person [id="+id+",name="+name+", age="+age+"]";
}
**public**String  getName() {
**return**name;
}
**public****void**setName(Stringname) {
**this**.name=name;
}
**public****int**getAge() {
**return**age;
}
**public****void**setAge(**int**age) {
**this**.age=age;
}
}
----------------------------------
使用map集合存储Person类

public  class MapDemo2 {
public static void main(String[] args) {
//创建一个以字符串作为key，Person对象作为值得集合
Map<String,Person> map = newHashMap<String,Person>();
map.put("key1", newPerson(1,"张三",23));
map.put("key2", newPerson(2,"李四",25));
System.out.println(map);
//输出{key1=Person [id=1,name=张三, age=23], key2=Person [id=2, name=李四, age=25]}
//也可以将Person对象作为key，字符串作为值得集合
Map<Person,String> map1 = newHashMap<Person,String>();
map1.put(new Person(1,"张三",23),"key1");
map1.put( new Person(2,"李四",25),"key2");
System.out.println(map1);
//输出{Person [id=2, name=李四,age=25]=key2, Person [id=1, name=张三, age=23]=key1}
}
}
-------------------------------------------------------
Map集合的遍历和判断
package cn.tx.map;
import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;
public class MapDemo3 {
public static void main(String[] args) {
//创建一个以字符串作为key，Person对象作为值得集合
Map<String,Person>  map = new HashMap<String,Person>();
map.put("key1",new Person(1,"张三",23));
map.put("key2",new Person(2,"李四",25));
//判断是否包含指定的key
boolean  isCon = map.containsKey("key1");
System.out.println(isCon);
//输出true
//判断是否包含指定的值
boolean  isConVal = map.containsValue(new Person(1,"张三",23));
System.out.println(isConVal);
//输出false
//如果值是对象需要判断值是否相等要在Person类里重写equals方法和hashCode方法。
isConVal =map.containsValue(new Person(1,"张三",23));
System.out.println(isConVal);
//重写后输出true
//判断是否为空
boolean  isEmpty = map.isEmpty();
System.out.println(isEmpty);
//输出false
//使用KeySet方法遍历map集合
Set<String>  keys = map.keySet();
//使用增强for循环遍历
for (String  key : keys) {
Person p= map.get(key);
System.out.println(p);
//输出
//Person[id=1, name=张三, age=23]
//Person[id=2, name=李四, age=25]
}
//获得键值对set集合
Set<Entry<String,Person>>   entry = map.entrySet();
for(Entry<String,Person>  sp : entry) {
//获得每一项中的key和value
String  key = sp.getKey();
Person  value = sp.getValue();
System.out.println("键：" + key + "值：" + value);
//输出
//键：key1值：Person [id=1, name=张三,age=23]
//键：key2值：Person [id=2, name=李四,age=25]

}

}
}


