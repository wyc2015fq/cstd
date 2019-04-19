# Qt容器类详解 - fanyun的博客 - CSDN博客
2016年12月25日 22:08:47[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：1738

QT不仅支持C++的STL模板库，同时自己也定义了一套容器类和与之操作的[**算法**](http://lib.csdn.net/base/datastructure)类，使用QT定义的这一套库，可以使在各个平台的表现相同。QT的这些容器被设计为更轻便，更安全和更容易使用。容器类是隐含共享(implicitly)的，可重入(reentrant)的和线程安全的。为容器遍历提供了两种迭代器（[**Java**](http://lib.csdn.net/base/javaee)-style，STL-style），并且QT提供了foreach关键字，更加方便元素遍历。
**一．连续容器：**
QVector<T>  
它是QT里最常见的容器类，它对应STL中的Vector<T>容器，是动态数组，提供快速的索引存取。
QList<T>  QLinkList<T>  QVarLengthArray<T>提供类似的功能。
可以参考帮助文档，查看QList的具体使用方法，以下列出部分使用示例：
1.创建各种类型的vector：
  QVector<int> integerVector;
  QVector<QString> stringVector;
2.创建一定数目项的vector
  QVector<QString> vector(200);
3.带初始化的创建vector
  QVector<QString> vector(200, "Pass");
也可以使用fill赋值：
  QVector<QString> vector(3);
  vector.fill("Yes");
  // vector: ["Yes", "Yes", "Yes"]
  vector.fill("oh", 5);
  // vector: ["oh", "oh", "oh", "oh", "oh"]
4.QVector像c++的Vector一样提供[]下标操作，并从0项开始。还提供at()的只读操作,at()比[]更快，因为它不会导致深度复制。
  if (vector[0] == "Liz")
  vector[0] = "Elizabeth";
  for (int i = 0; i < vector.size(); ++i) {
    if (vector.at(i) == "Alfonso")
        cout << "Found Alfonso at position " << i << endl;
  }
5.你可以使用indexOf，lastIndexOf来查询获取某项值的索引：
  QVector<QString> vector;
  vector << "A" << "B" << "C" << "B" << "A";
  vector.indexOf("B");            // returns 1
  vector.indexOf("B", 1);         // returns 1
  vector.indexOf("B", 2);         // returns 3
  vector.indexOf("X");            // returns -1
  vector.lastIndexOf("B");        // returns 3
  vector.lastIndexOf("B", 3);     // returns 3
  vector.lastIndexOf("B", 2);     // returns 1
  vector.lastIndexOf("X");        // returns -1
也可以用contains()查看是否包含某元素，返回bool值。
6.通过append，operator<<，prepend，insert添加元素。（对于较大的vector来说，在开头和中间插入项都是相当耗时的。这种情况更适合使用QLinkedList<T>）
  QVector<QString> vector(0);
  vector.append("one");
  vector.append("two");
  vector.append("three");
  // vector: ["one", "two", "three"]
  QVector<QString> vector(0);
  vector << “one” << “two” << “three”;
  // vector: ["one", "two", "three"]
  QVector<QString> vector;
  vector.prepend("one");
  vector.prepend("two");
  vector.prepend("three");
  // vector: ["three", "two", "one"]
  QVector<QString> vector;
  vector << "alpha" << "beta" << "delta";
  vector.insert(2, "gamma");
  // vector: ["alpha", "beta", "gamma", "delta"]
7.size() resize() isEmpty() capacity()等和容器大小相关操作。
8.相关转化：toList()  toStdVector()  
  QVector<double> vect;
  vect << "red" << "green" << "blue" << "black"; 
  QList<double> list = vect.toList();
  // list: ["red", "green", "blue", "black"]
  QVector<double> vector;
  vector << 1.2 << 0.5 << 3.14;
  std::vector<double> stdvector = vector.toStdVector();
（以下容器操作函数的使用将不再累述，和查阅帮助文档，并且和QVector的使用方法是一样的。）
QLinkedList<T>
前面提到，它适合随机插入项，其原因是它的链式结构。他提供了常量时间的插入删除，却不能提供快速的随机存取操作。不提供[]操作，它的遍历元素是通过迭代器完成的。
QList<T>
它是个数组列表，结合了上面两种结构的优点，它支持随机存取，在它的任意一端插入和删除都是非常快速的并且对于千项以上的列表，在中间插入和删除也是很快的。学过[**数据结构**](http://lib.csdn.net/base/datastructure)的都会清楚这三者的结构区别。如果非要每个项元素都相邻那就只能用QVector。
QString<t>
它是QList<QString>的子类，它为字符串操作提供了更通用的操作。
QStack<T> QQueue<T>
他们是栈和队列结构的实现，QStack提供pop() push() swap() top()操作，它继承自QVector<T>
QQueue<T>提供dequeue() enqueue() head() swap操作。继承自QList<T>。
**二．关联容器**
QSet<T>
它提供一个键值对集合，可以快速的进行查找，
QMap<Key, T> QMultiMap<Key, T>
QMap是一个以升序键顺序存储键值对的数据结构，QMultiMap是QMap基础上提供可以存储多值的maps，这样就是说一个键对应多个值了。
下面是创建一个QString-int的maps
  QMap<QString, int> map;
可以这样插入值
  map["one"] = 1;
  map["three"] = 3;
  map["seven"] = 7;
也可以这样：
  map.insert("twelve", 12);
查询一个值使用[] 或者value(“**”)
  int num1 = map["thirteen"];
  int num2 = map.value("thirteen");
查询是否存在一个值：
  if (map.contains("TIMEOUT"))
      timeout = map.value("TIMEOUT");
一般推荐使用contains() value()而不是[]。
QHash<Key, T> QMultiHash<Key, T>
QHash<Key, T>是个在哈希表中存储键值对的结构。它的接口几乎和QMap相同，但它提供了更快的查找功能。
QHash为它的内部哈希表自动分配最初的存储区域，并在有项被插入或者删除时重新划分所分配的区域大小。也可以调用reserve()或者squeeze()来指定或者压缩希望存储到哈希表的项的数目，以进行性能调整。通常的做法是利用我们预期的最大的项的数目来调用reserve()，然后插入数据，最后如果有多出的项，则调用squeeze()以使内存减到最小。
**三．迭代器**
对于每种容器都有两种风格的迭代器——java风格和STL风格。Java风格的更易于使用而以很少量性能作为了代价，而STL风格的可以结合STL的算法从而更加强大。
这里我们主讲QList和QMap的迭代器为例。
Java-Style：
Java风格的迭代器分为两种：只读迭代器，读写迭代器。只读迭代器就是Q*Iterator<T> (例如QVectorIterator<T>)，而读写迭代器则像QMutable*Iterator<T>这种（例如：QMutableVectorIterator<T>）。
|Containers|Read-only iterator|Read-write iterator|
|----|----|----|
|[QList](http://blog.csdn.net/xuguangsoft/article/details/8514921)<T>, [QQueue](http://blog.csdn.net/xuguangsoft/article/details/8514921)<T>|[QListIterator](http://blog.csdn.net/xuguangsoft/article/details/8514921)<T>|[QMutableListIterator](http://blog.csdn.net/xuguangsoft/article/details/8514921)<T>|
|[QLinkedList](http://blog.csdn.net/xuguangsoft/article/details/8514921)<T>|[QLinkedListIterator](http://blog.csdn.net/xuguangsoft/article/details/8514921)<T>|[QMutableLinkedListIterator](http://blog.csdn.net/xuguangsoft/article/details/8514921)<T>|
|[QVector](http://blog.csdn.net/xuguangsoft/article/details/8514921)<T>, [QStack](http://blog.csdn.net/xuguangsoft/article/details/8514921)<T>|[QVectorIterator](http://blog.csdn.net/xuguangsoft/article/details/8514921)<T>|[QMutableVectorIterator](http://blog.csdn.net/xuguangsoft/article/details/8514921)<T>|
|[QSet](http://blog.csdn.net/xuguangsoft/article/details/8514921)<T>|[QSetIterator](http://blog.csdn.net/xuguangsoft/article/details/8514921)<T>|[QMutableSetIterator](http://blog.csdn.net/xuguangsoft/article/details/8514921)<T>|
|[QMap](http://blog.csdn.net/xuguangsoft/article/details/8514921)<Key, T>, [QMultiMap](http://blog.csdn.net/xuguangsoft/article/details/8514921)<Key, T>|[QMapIterator](http://blog.csdn.net/xuguangsoft/article/details/8514921)<Key, T>|[QMutableMapIterator](http://blog.csdn.net/xuguangsoft/article/details/8514921)<Key, T>|
|[QHash](http://blog.csdn.net/xuguangsoft/article/details/8514921)<Key, T>, [QMultiHash](http://blog.csdn.net/xuguangsoft/article/details/8514921)<Key, T>|[QHashIterator](http://blog.csdn.net/xuguangsoft/article/details/8514921)<Key, T>|[QMutableHashIterator](http://blog.csdn.net/xuguangsoft/article/details/8514921)<Key, T>|
Java风格迭代器的有效位置：
下面是一个典型的使用例子：  
  QList<QString> list;
  list << "A" << "B" << "C" << "D";
  QListIterator<QString> i(list);
  while (i.hasNext())
      qDebug() << i.next();
下面展示如何向后遍历
  QListIterator<QString> i(list);
  i.toBack();
  while (i.hasPrevious())
      qDebug() << i.previous();
如果左边有项那么hasPrevious()将返回true。previous()返回迭代器左边的项并且往前移一个位置。可以看如图：
下表是QListIterator 的API及说明
|Function|Behavior|
|----|----|
|[toFront()](http://blog.csdn.net/xuguangsoft/article/details/8514921)|迭代器移到最前，第一项的前|
|[toBack()](http://blog.csdn.net/xuguangsoft/article/details/8514921)|迭代器移到最后，最后一项的后面|
|[hasNext()](http://blog.csdn.net/xuguangsoft/article/details/8514921)|如果不是list的最后，就返回true|
|[next()](http://blog.csdn.net/xuguangsoft/article/details/8514921)|返回下一项，并迭代器向后移一位|
|[peekNext()](http://blog.csdn.net/xuguangsoft/article/details/8514921)|返回下一项，迭代器并不移动|
|[hasPrevious()](http://blog.csdn.net/xuguangsoft/article/details/8514921)|如果不是list的最前，就返回true|
|[previous()](http://blog.csdn.net/xuguangsoft/article/details/8514921)|返回前一项，并迭代器向后移一位|
|[peekPrevious()](http://blog.csdn.net/xuguangsoft/article/details/8514921)|返回前一项，迭代器并不移动|
下面是Mutable iterator读写迭代器使用说明：
QList<int>移除基数项：
  QMutableListIterator<int> i(list);
  while (i.hasNext()) {
      if (i.next() % 2 != 0)
          i.remove();
  }
下面是QMap的迭代器示例，用法和前面是类似的：
  QMap<QString, QString> map;
  map.insert("Paris", "France");
  map.insert("Guatemala City", "Guatemala");
  map.insert("Mexico City", "Mexico");
  map.insert("Moscow", "Russia");
  ...
  QMutableMapIterator<QString, QString> i(map);
  while (i.hasNext()) {
      if (i.next().key().endsWith("City"))
          i.remove();
  }
  QMap<int, QWidget *> map;
  QHash<int, QWidget *> hash;
  QMapIterator<int, QWidget *> i(map);
  while (i.hasNext()) {
      i.next();
      hash.insert(i.key(), i.value());
  }
STL-Style:
STL风格是迭代器不仅支持Qt的通用算法，还兼容STL的。
和java风格的类似，它也有两种风格的迭代器，只读的（const_iterator）和读写的（iterator）。
|Containers|Read-only iterator|Read-write iterator|
|----|----|----|
|[QList](http://blog.csdn.net/xuguangsoft/article/details/8514921)<T>, [QQueue](http://blog.csdn.net/xuguangsoft/article/details/8514921)<T>|[QList](http://blog.csdn.net/xuguangsoft/article/details/8514921)<T>::const_iterator|[QList](http://blog.csdn.net/xuguangsoft/article/details/8514921)<T>::iterator|
|[QLinkedList](http://blog.csdn.net/xuguangsoft/article/details/8514921)<T>|[QLinkedList](http://blog.csdn.net/xuguangsoft/article/details/8514921)<T>::const_iterator|[QLinkedList](http://blog.csdn.net/xuguangsoft/article/details/8514921)<T>::iterator|
|[QVector](http://blog.csdn.net/xuguangsoft/article/details/8514921)<T>, [QStack](http://blog.csdn.net/xuguangsoft/article/details/8514921)<T>|[QVector](http://blog.csdn.net/xuguangsoft/article/details/8514921)<T>::const_iterator|[QVector](http://blog.csdn.net/xuguangsoft/article/details/8514921)<T>::iterator|
|[QSet](http://blog.csdn.net/xuguangsoft/article/details/8514921)<T>|[QSet](http://blog.csdn.net/xuguangsoft/article/details/8514921)<T>::const_iterator|[QSet](http://blog.csdn.net/xuguangsoft/article/details/8514921)<T>::iterator|
|[QMap](http://blog.csdn.net/xuguangsoft/article/details/8514921)<Key, T>, [QMultiMap](http://blog.csdn.net/xuguangsoft/article/details/8514921)<Key, T>|[QMap](http://blog.csdn.net/xuguangsoft/article/details/8514921)<Key, T>::const_iterator|[QMap](http://blog.csdn.net/xuguangsoft/article/details/8514921)<Key, T>::iterator|
|[QHash](http://blog.csdn.net/xuguangsoft/article/details/8514921)<Key, T>, [QMultiHash](http://blog.csdn.net/xuguangsoft/article/details/8514921)<Key, T>|[QHash](http://blog.csdn.net/xuguangsoft/article/details/8514921)<Key, T>::const_iterator|[QHash](http://blog.csdn.net/xuguangsoft/article/details/8514921)<Key, T>::iterator|
用过c++ STL库的就对此很容易上手。下面是QListIterator的例子：
  QList<QString> list;
  list << "A" << "B" << "C" << "D";
  QList<QString>::iterator i;
  for (i = list.begin(); i != list.end(); ++i)
      *i = (*i).toLower();
STL风格的迭代器可允许的位置与java风格的有所不同
遍历需要我们自己增加缩减迭代器，例如：
  QList<QString>::const_iterator i;
  for (i = list.constBegin(); i != list.constEnd(); ++i)
      qDebug() << *i;
QList<QString> list;
  list << "A" << "B" << "C" << "D";
  QList<QString>::iterator i = list.end();
  while (i != list.begin()) {
      --i;
      *i = (*i).toLower();
  }
下面是QMap的例子：
  QMap<int, int> map;
  ...
  QMap<int, int>::const_iterator i;
  for (i = map.constBegin(); i != map.constEnd(); ++i)
      qDebug() << i.key() << ":" << i.value();
**四．Foreach关键字**
下面这个用foreach去遍历QLinkedList<QString>
  QLinkedList<QString> list;
  ...
  QString str;
  foreach (str, list)
      qDebug() << str;
可以再循环里使用break
  QLinkedList<QString> list;
  ...
  foreach (const QString &str, list) {
      if (str.isEmpty())
          break;
      qDebug() << str;
  }
QMap 和 QHash 中，如果你想遍历键和值，你可以用iterators(更快)，或者这样写：
  QMap<QString, int> map;
  ...
  foreach (const QString &str, map.keys())
      qDebug() << str << ":" << map.value(str);
或者：
  QMultiMap<QString, int> map;
  ...
  foreach (const QString &str, map.uniqueKeys()) {
      foreach (int i, map.values(str))
          qDebug() << str << ":" << i;
  }
**五．类容器类（Container-Like Class）**
QVarLengthArray<T>
C++不支持在栈内存中提供可变长度的数组，例如下面：
int myfunc(int n)
{
    int table[n + 1];  // WRONG
    ...
    return table[n];
}
只能在堆内存中实现：
int myfunc(int n)
{
    int *table = new int[n + 1];
    ...
    int ret = table[n];
    delete[] table;
    return ret;
}
但是如果myfunc在应用程序内循环中调用非常频繁，那么堆内存分配将会变得缓慢，这种情况，QT为我们提供了QVarLengthArray来解决。
int myfunc(int n)
{
    QVarLengthArray<int, 1024> array(n + 1);
    ...
    return array[n];
}
值得注意的是，1.它的API是低水平的(low-level)的，他没有提供迭代器，和QVector的功能函数。2.如果值是基本类型，它将不会初始化内存。3.QVector使用隐含共享作为内存的优化，QVarLengthArray并没有提供，然而，它因为减少了经常性消费而显得性能稍微好些，特别是在紧凑的循环里。总的来说，它是为了方便用户使用在很少部分情况。
QCache<Key, T>
提供一个cache去存储Key-T键值对的对象。例如：
  QCache<int, Employee> cache;
插入对象到cache
  Employee *employee = new Employee;
  employee->setId(37);
  employee->setName("Richard Schmit");
  ...
  cache.insert(employee->id(), employee);
QCache的好处是自动获取的对象的拥有权（ownership）。你可以指定插入对象的花费，totalCost() maxCost()。maxCost()默认是100。
  QCache<int, MyDataStructure> cache(5000);
QContiguousCache<T>
QContiguousCache是一个提供连续Cache存储器的模板类。和QCache不同的是，它要求一个约束——相邻（Contiguous）。这有利于用户交互界面最普遍的数据需求。这样的约束使它比QCache消耗更少的内存和处理器周期。
简单的使用QContiguousCache的方式是使用append() prepend()
  MyRecord record(int row) const
  {
      Q_ASSERT(row >= 0 && row < count());
      while(row > cache.lastIndex())
          cache.append(slowFetchRecord(cache.lastIndex()+1));
      while(row < cache.firstIndex())
          cache.prepend(slowFetchRecord(cache.firstIndex()-1));
      return cache.at(row);
  }
可以查看文档中Contiguous Cache Example的例子。
QPair<T1, T2>
这个在STL中也是有的（pair）用来储存键值对。它用得更多的是做为函数的返回值。
看下面这个例子：存储一个QString键double值的QPair
  QPair<QString, double> pair;
使用first second来修改值
  pair.first = "pi";
  pair.second = 3.14159265358979323846;
**六．算法复杂性比较**
Constant time: O(1). 常数时间复杂度
Logarithmic time: O(log n). 对数时间复杂度
Linear time: O(n). 线性时间复杂度
Linear-logarithmic time: O(n log n). 线性对数时间复杂度
Quadratic time: O(n²). 平方时间复杂度
 顺序容器类操作时间复杂度比较：
|Index lookup|Insertion|Prepending|Appending| |
|----|----|----|----|----|
|[QLinkedList](http://blog.csdn.net/xuguangsoft/article/details/8514921)<T>|O(n)|O(1)|O(1)|O(1)|
|[QList](http://blog.csdn.net/xuguangsoft/article/details/8514921)<T>|O(1)|O(n)|Amort. O(1)|Amort. O(1)|
|[QVector](http://blog.csdn.net/xuguangsoft/article/details/8514921)<T>|O(1)|O(n)|O(n)|Amort. O(1)|
关联容器时间复杂度比较：
|Key lookup|Insertion| | | |
|----|----|----|----|----|
|Average|Worst case|Average|Worst case| |
|[QMap](http://blog.csdn.net/xuguangsoft/article/details/8514921)<Key, T>|O(log n)|O(log n)|O(log n)|O(log n)|
|[QMultiMap](http://blog.csdn.net/xuguangsoft/article/details/8514921)<Key, T>|O(log n)|O(log n)|O(log n)|O(log n)|
|[QHash](http://blog.csdn.net/xuguangsoft/article/details/8514921)<Key, T>|Amort. O(1)|O(n)|Amort. O(1)|O(n)|
|[QSet](http://blog.csdn.net/xuguangsoft/article/details/8514921)<Key>|Amort. O(1)|O(n)|Amort. O(1)|O(n)|
