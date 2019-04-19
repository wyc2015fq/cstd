# java 集合类Array、List、Map区别和联系 - BlueSky - CSDN博客
2016年05月05日 11:09:11[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：364
个人分类：[java编程](https://blog.csdn.net/ASIA_kobe/article/category/2863525)
[Java](http://lib.csdn.net/base/17)集合类主要分为以下三类：
**第一类：Array、Arrays**
**第二类：Collection** ：List、Set
**第三类：Map** ：HashMap、HashTable
**一、Array ， Arrays**
Java所有“存储及随机访问一连串对象”的做法，array是最有效率的一种。
1、
效率高，但容量固定且无法动态改变。
array还有一个缺点是，无法判断其中实际存有多少元素，length只是告诉我们array的容量。
2、Java中有一个**Arrays类，专门用来操作array**。
    arrays中拥有一组static函数，
equals()：比较两个array是否相等。array拥有相同元素个数，且所有对应元素两两相等。
fill()：将值填入array中。
sort()：用来对array进行排序。
binarySearch()：在排好序的array中寻找元素。
System.arraycopy()：array的复制。
**二、Collection ， Map**
若撰写程序时不知道究竟需要多少对象，需要在空间不足时自动扩增容量，则需要使用容器类库，array不适用。
**1、Collection 和 Map 的区别**
容器内每个为之所存储的元素个数不同。
Collection类型者，每个位置只有一个元素。
Map类型者，持有 key-value pair，像个小型[数据库](http://lib.csdn.net/base/14)。
**2、各自旗下的子类关系**
**Collection**    --List ： 将以特定次序存储元素。所以取出来的顺序可能和放入顺序不同。
          --ArrayList / LinkedList / Vector
    --Set ： 不能含有重复的元素
          --HashSet / TreeSet
**Map**
    --HashMap
-- HashTable
-- TreeMap
**3、其他特征**
*  List，Set，Map将持有对象一律视为Object型别。
*  Collection、List、Set、Map都是接口，不能实例化。
   继承自它们的 ArrayList, Vector, HashTable, HashMap是具象class，这些才可被实例化。
*  vector容器确切知道它所持有的对象隶属什么型别。vector不进行边界检查。
**三、Collections**
**Collections是针对集合类的一个帮助类。**提供了一系列静态 方法实现对各种集合的搜索、排序、线程完全化等操作。
相当于对Array进行类似操作的类——Arrays。
如，Collections.max(Collection coll); 取coll中最大的元素。
    Collections.sort(List list); 对list中元素排序
**四、如何选择？**
**1、容器类和Array的区别、择取**
   *  容器类仅能持有对象引用（指向对象的指针），而不是将对象信息copy一份至数列某位置。
   *  一旦将对象置入容器内，便损失了该对象的型别信息。
**2、**
  *  在各种Lists中，最好的做法是以ArrayList作为缺省选择。当插入、删除频繁时，使用LinkedList()；
     Vector总是比ArrayList慢，所以要尽量避免使用。
  *  在各种Sets中，HashSet通常优于HashTree（插入、查找）。只有当需要产生一个经过排序的序列，才用TreeSet。
     HashTree存在的唯一理由：能够维护其内元素的排序状态。
  *  在各种Maps中
     HashMap用于快速查找。
  *  当元素个数固定，用Array，因为Array效率是最高的。
**结论：最常用的是ArrayList，HashSet，HashMap，Array。**
**注意：**
1、Collection没有get()方法 来取得某个元素。只能通过iterator()遍历元素。
2、Set 和Collection拥有一模一样的接口。
3、List ，可以通过get()方法来一次取出一个元素 。使用数字来选择一堆对象中的一个，get(0)...。(add/get)
4、一般使用ArrayList。用LinkedList构造堆栈stack、队列queue 。
5、Map用 put(k,v) / get(k) ，还可以使用containsKey()/containsValue()来检查其中是否含有某个key/value。
   HashMap会利用对象的hashCode来快速找到key。
***  hashing**       哈希码就是将对象的信息经过一些转变形成一个独一无二的int值，这个值存储在一个array中。
       我们都知道所有存储结构中，array查找速度是最快的。所以，可以加速查找。
       发生碰撞时，让array指向多个values。即，数组每个位置上又生成一个梿表。
**6、Map中元素，可以将key序列、value序列单独抽取出来。**使用keySet() 抽取key序列，将map中的所有keys生成一个Set。
使用values( ) 抽取value序列，将map中的所有values生成一个Collection。
为什么一个生成Set，一个生成Collection？那是因为，key总是独一无二的，value允许重复。
