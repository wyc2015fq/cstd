# Hashpmap的原理，HashMap怎样保证key的唯一性 - z69183787的专栏 - CSDN博客
2017年09月13日 14:42:29[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1151
1.    HashMap概述
HashMap是基于哈希表的Map接口的非同步实现。此实现提供所有可选的映射操作，并允许使用null值和null键。此类不保证映射的顺序，特别是它不保证该顺序恒久不变。
2.    HashMap的数据结构
在java编程语言中，最基本的结构就是两种，一个是数组，另外一个是模拟指针（引用），所有的数据结构都可以用这两个基本结构来构造的，HashMap也不例外。HashMap实际上是一个“链表散列”的数据结构，即数组和链表的结合体。
![](http://blog.chinaunix.net/attachment/201203/22/11775320_1332399870gIGw.jpg)
3.            HashMap的存取
    HashMap的功能是通过“键(key)”能够快速的找到“值”。下面我们分析下HashMap存数据的基本流程：
    1、 当调用put(key,value)时，首先获取key的hashcode，int hash = key.hashCode();
    2、 再把hash通过一下运算得到一个int h.
hash ^= (hash >>> 20) ^ (hash >>> 12);
int h = hash ^ (hash >>> 7) ^ (hash >>> 4);
为什么要经过这样的运算呢？这就是HashMap的高明之处。先看个例子，一个十进制数32768(二进制1000 0000 0000 0000)，经过上述公式运算之后的结果是35080(二进制1000 1001 0000 1000)。看出来了吗？或许这样还看不出什么，再举个数字61440(二进制1111
 0000 0000 0000)，运算结果是65263(二进制1111 1110 1110 1111)，现在应该很明显了，它的目的是让“1”变的均匀一点，散列的本意就是要尽量均匀分布。那这样有什么意义呢？看第3步。
    3、 得到h之后，把h与HashMap的承载量（HashMap的默认承载量length是16，可以自动变长。在构造HashMap的时候也可以指定一个长 度。这个承载量就是上图所描述的数组的长度。）进行逻辑与运算，即 h & (length-1)，这样得到的结果就是一个比length小的正数，我们把这个值叫做index。其实这个index就是索引将要插入的值在数组中的 位置。第2步那个算法的意义就是希望能够得出均匀的index，这是HashTable的改进，HashTable中的算法只是把key的 hashcode与length相除取余，即hash
 % length，这样有可能会造成index分布不均匀。还有一点需要说明，HashMap的键可以为null，它的值是放在数组的第一个位置。
    4、 我们用table[index]表示已经找到的元素需要存储的位置。先判断该位置上有没有元素（这个元素是HashMap内部定义的一个类Entity， 基本结构它包含三个类，key，value和指向下一个Entity的next）,没有的话就创建一个Entity<K,V>对象，在 table[index]位置上插入，这样插入结束；如果有的话，通过链表的遍历方式去逐个遍历，看看有没有已经存在的key，有的话用新的value替
 换老的value；如果没有，则在table[index]插入该Entity，把原来在table[index]位置上的Entity赋值给新的 Entity的next，这样插入结束。
总结：keyàhashcodeàhàindexà遍历链表à插入
4.            扩展问题
要同时复写equals方法和hashCode方法。
按照散列函数的定义，如果两个对象相同，即obj1.equals(obj2)=true，则它们的hashCode必须相同，但如果两个对象不同，则它们的hashCode不一定不同。
如果两个不同对象的hashCode相同，这种现象称为冲突，冲突会导致操作哈希表的时间开销增大，所以尽量定义好的hashCode()方法，能加快哈希表的操作。
如果相同的对象有不同的hashCode，对哈希表的操作会出现意想不到的结果（期待的get方法返回null），
再回头看看前面提到的为什么覆盖了equals方法之后一定要覆盖hashCode方法，很简单，比如，String a = new String(“abc”);String b = new String(“abc”);如果不覆盖hashCode的话，那么a和b的hashCode就会不同，把这两个类当做key存到HashMap中的话就
 会出现问题，就会和key的唯一性相矛盾。
原文出处：http://blog.chinaunix.net/uid-11775320-id-3143919.html
