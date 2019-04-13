
# java的Map的子类 - 李昆鹏的博客 - CSDN博客


2018年04月03日 13:57:01[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：96


**-----------------------java的Map的子类-----------------------**
Map的实现类HashMap
HashMap的特点
1.    HashMap是map的实现类
2.    允许多个null值和一个null键
3.    HashMap中的元素没有顺序（跟添加的顺序无关）
4.    HashMap不是线程安全的。
**public****class**MapDemo4 {
**public****static****void**main(String[]args) {
//定义一个map
Map<String,String>map=**new**HashMap<String,String>();
//放入键值对
map.put("鼓上骚","时迁");
map.put("行者","武松");
map.put("及时雨","松江");
map.put("付宝义","宋江");
System.**out**.println(map);
//输出{付宝义=宋江,行者=武松,及时雨=松江,鼓上骚=时迁}
//由此看出hashMap添加的顺序是没有规律的，并且它不是线程安全的
//把map变成线程同步
map= Collections.*synchronizedMap*(map);
-----------------------------------------------------------------------------
Map的实现类TreeMap
TreeMap可以参考TreeSet，TreeMap可以支持Map的排序。
该映射根据其键的自然顺序进行排序，或者根据创建映射时提供的Comparator进行排序，具体取决于使用的构造方法
TreeMap也不是线程安全的
TreeMap的特点：
1.可以按着key来做排序
2.Key不能为null，key不能重复，值可以有多个null
3.    不是线程安全的

**public****class**MapDemo5 {
**public****static****void**main(String[]args) {
//定义一个TreeMap
Map<String,Person>map=**new**TreeMap<String,Person>();
//TreeMap是根据key的comparator来进行排序的
map.put("b",**new**Person(1,"张三", 23));
map.put("a",**new**Person(2,"李四", 23));
//TreeMap中key是不能为null的
//map.put(null, new Person(2, "李四", 23));
System.**out**.println(map);
//输出{a=Person [id=2, name=李四, age=23], b=Person [id=1, name=张三, age=23]}
//如果我们想按Person来进行排序就把Person和String的位置调换
//定义一个TreeMap
Map<Person,String>map1=**new**TreeMap<Person,String>();
//TreeMap是根据key的comparator来进行排序的
//注意：我们必须重写Person类的compareTo方法来进行排序
//而Person类必须实现Comparable接口，泛型是Person
map1.put(**new**Person(5,"张三", 23),"b");
map1.put(**new**Person(2,"王五", 25),"a");
map1.put(**new**Person(4,"李四", 21),"h");
System.**out**.println(map1);
//输出{Person [id=2, name=王五, age=25]=a, Person [id=4, name=李四, age=21]=h, Person[id=5, name=张三, age=23]=b}
}
}
--------------------------------------------------------------------------------------------------------------
Map的实现类Hashtable
Hashtable的特点
1.    Hashtable是map的实现类
2.不允许任何null值和null键
3.    HashTable中的元素没有顺序（跟添加的顺序无关）
4.Hashtable是线程安全的
HashMap和Hashtable的区别
1.都是键值对集合
2.HashMap允许有一个null键和多个null值，HashTable不允许有null键和null值
3.HashMap不是线程安全的，Hashtable是线程安全的
**public****class**MapDemo6 {
**public****static****void**main(String[]args) {
//定义一个Hashtable
Map<String,String>map=**new**Hashtable<String,String>();
//放入键值对
map.put("鼓上骚","时迁");
map.put("行者","武松");
map.put("及时雨","松江");
map.put("付宝义","宋江");
//Hashtablekey和值都不能为null
//map.put(null, "宋江");
//map.put("付宝义", null);
System.**out**.println(map);
//输出{付宝义=宋江,行者=武松,及时雨=松江,鼓上骚=时迁}
}
}
------------------------------------------------------
Map的实现类LinkedHashMap
LinkedHashMap的特点
1.    LinkedHashMap是Map的实现类。
2.    允许多个null值和一个null键。
3.LinkedHashMap有顺序（添加的顺序）
4.    LinkedHashMap不是线程安全的
**public****class**MapDemo7 {
**public****static****void**main(String[]args) {
//定义一个LinkedHashMap
Map<String,String>map=**new**LinkedHashMap<String,String>();
//放入键值对
map.put("鼓上骚","时迁");
map.put("行者","武松");
map.put("及时雨","松江");
map.put("付宝义","宋江");
System.**out**.println(map);
//输出{鼓上骚=时迁,行者=武松,及时雨=松江,付宝义=宋江}
}
}


