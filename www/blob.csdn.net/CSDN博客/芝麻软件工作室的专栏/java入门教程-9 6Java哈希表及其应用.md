
# java入门教程-9.6Java哈希表及其应用 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月27日 09:47:42[seven-soft](https://me.csdn.net/softn)阅读数：469


哈希表也称为散列表，是用来存储群体对象的集合类结构。
## 什么是哈希表
数组和向量都可以存储对象，但对象的存储位置是随机的，也就是说对象本身与其存储位置之间没有必然的联系。当要查找一个对象时，只能以某种顺序（如顺序查找或二分查找）与各个元素进行比较，当数组或向量中的元素数量很多时，查找的效率会明显的降低。
一种有效的存储方式，是不与其他元素进行比较，一次存取便能得到所需要的记录。这就需要在对象的存储位置和对象的关键属性（设为 k）之间建立一个特定的对应关系（设为 f），使每个对象与一个唯一的存储位置相对应。在查找时，只要根据待查对象的关键属性
 k 计算f(k)的值即可。如果此对象在集合中，则必定在存储位置 f(k)上，因此不需要与集合中的其他元素进行比较。称这种对应关系 f 为哈希（hash）方法，按照这种思想建立的表为哈希表。
Java 使用哈希表类（Hashtable）来实现哈希表，以下是与哈希表相关的一些概念：容量（Capacity）：Hashtable 的容量不是固定的，随对象的加入其容量也可以自动增长。
关键字（Key）：每个存储的对象都需要有一个关键字，key 可以是对象本身，也可以是对象的一部分（如某个属性）。要求在一个 Hashtable 中的所有关键字都是唯一的。
哈希码（Hash Code）：若要将对象存储到 Hashtable 上，就需要将其关键字 key 映射到一个整型数据，成为 key 的哈希码。
项（Item）：Hashtable 中的每一项都有两个域，分别是关键字域 key 和值域 value（存储的对象）。Key 和 value 都可以是任意的 Object 类型的对象，但不能为空。
装填因子（Load Factor）：装填因子表示为哈希表的装满程度，其值等于元素数比上哈希表的长度。
## 哈希表的使用
哈希表类主要有三种形式的构造方法：
Hashtable(); //默认构造函数，初始容量为 101，最大填充因子 0.75
Hashtable(int capacity);
Hashtable(int capacity,float loadFactor)
哈希表类的主要方法如表 8-6 所示。
表 8-6 哈希表定义的常见方法|方法|功能|
|void clear()|重新设置并清空哈希表|
|boolean contains(Object value)|确定哈希表内是否包含了给定的对象，若有返回 true，否则返回 false|
|boolean containsKey(Object key)|确定哈希表内是否包含了给定的关键字，若有返回 true，否则返回 false|
|boolean isEmpty()|确认哈希表是否为空，若是返回 true，否则返回 false|
|Object get(Object key)|获取对应关键字的对象，若不存在返回 null|
|void rehash()|再哈希，扩充哈希表使之可以保存更多的元素，当哈希表达到饱和时，系统自动调用此方法|
|Object put(Object key,Object value)|用给定的关键字把对象保存到哈希表中，此处的关键字和元素均不可为空|
|Object remove(Object key)|从哈希表中删除与给定关键字相对应的对象，若该对象不存在返回 null|
|int size()|返回哈希表的大小|
|String toString()|将哈希表内容转换为字符串|
哈希表的创建也可以通过 new 操作符实现。其语句为：
HashTable has=new HashTable();
【例 8-12】哈希表的遍历。
//********** ep8_12.java **********
import java.util.*;
class ep8_12{
public static void main(String args[]){
Hashtable has=new Hashtable();
has.put("one",new Integer(1));
has.put("two",new Integer(2));
has.put("three",new Integer(3));
has.put("four",new Double(12.3));
Set s=has.keySet();
for(Iterator<String> i=s.iterator();i.hasNext();){
System.out.println(has.get(i.next()));
}
}
}
运行结果：
2
1
3
12.3

