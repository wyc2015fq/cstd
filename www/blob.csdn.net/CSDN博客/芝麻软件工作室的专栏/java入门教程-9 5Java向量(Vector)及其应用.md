
# java入门教程-9.5Java向量(Vector)及其应用 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月27日 09:46:46[seven-soft](https://me.csdn.net/softn)阅读数：215个人分类：[Java																](https://blog.csdn.net/softn/article/category/6242590)



Vector（向量）是 java.util 包中的一个类，该类实现了类似动态数组的功能。
向量和数组相似，都可以保存一组数据（数据列表）。但是数组的大小是固定的，一旦指定，就不能改变，而向量却提供了一种类似于“动态数组”的功能，向量与数组的重要区别之一就是向量的容量是可变的。
可以在向量的任意位置插入不同类型的对象，无需考虑对象的类型，也无需考虑向量的容量。
向量和数组分别适用于不同的场合，一般来说，下列场合更适合于使用向量：如果需要频繁进行对象的插入和删除工作，或者因为需要处理的对象数目不定。
列表成员全部都是对象，或者可以方便的用对象表示。
需要很快确定列表内是否存在某一特定对象，并且希望很快了解到对象的存放位置。
向量作为一种对象提供了比数组更多的方法，但需要注意的是，向量只能存储对象，不能直接存储简单数据类型，因此下列场合适用于使用数组：所需处理的对象数目大致可以确定。
所需处理的是简单数据类型。
## 向量的使用
向量必须要先创建后使用，向量的大小是向量中元素的个数，向量的容量是被分配用来存储元素的内存大小，其大小总是大于向量的大小。下面是 Vector 的构造方法：Vector();//①创建空向量，初始大小为 10
Vector(intinitialCapacity);//②创建初始容量为 capacity 的空向量
Vector(intinitialCapacity,intcapacityIncrement);//③创建初始容量为 initialCapacity，增量为 capacityIncrement 的空向量

使用第①种方式系统会自动对向量进行管理。
使用第②种方式，会创建一个初始容量（即向量可存储数据的大小）为 initialCapacity 的空向量，当真正存放的数据超过该容量时，系统会自动扩充容量，每次增加一倍。
使用第③中方式，会创建一个初始容量为 initialCapacity 的空向量，当真正存放的数据超过该容量时，系统每次会自动扩充 capacityIncrement。如果 capacityIncrement 为0，那么每次增加一倍，。
通过分配多于所需的内存空间，向量减少了必须的内存分配的数目。这样能够有效地减少分配所消耗的时间，每次分配的额外空间数目将由创建向量时指定的增量所决定。
除了构造方法外，向量类还提供了三个属性变量，分别为：protectedintcapacityIncrement;//当向量大小不足时，所用的增量大小
protectedintelementCount;//向量的元素个数
protectedObjectelementData[];//向量成员数据所用的缓冲

一旦创建了Vector类的实例，就可以用其方法来执行插入、删除以及查找对象等操作，向量类提供了极为丰富的方法，下表给出了一些常用的方法：
|方法|功能|
|void addElement(Object element)|将给定对象 element 增加到向量末尾|
|int capacity()|返回向量容量|
|boolean contains(Object element)|若向量中包含了 element 返回 true，否则返回 false|
|void copyInto(Object Array[])|将向量元素复制到指定数组|
|synchronized Object elementAt(int index)|返回指定下标的元素，若下标非法，抛出 ArrayIndexOutOfBoundsExecption 异常|
|void ensureCapacity(int size)|将向量的最小容量设为 size|
|synchronized Object firstElement()|返回向量的第一个元素，若向量为空，抛出 NoSuchElementException 异常|
|int indexOf(Object element)|返回 element 的下标，若对象不存在返回-1|
|int indexOf (Object element,int start)|从指定位置（start）开始搜索向量，返回对象所对应的下标值，若未找到返回-1|
|void insertElementAt (Object obj,int index)|将给定的对象插入到指定的下标处|
|boolean isEmpty()|若向量不包括任何元素，返回 true，否则返回 false|
|synchronized Object lastElement()|返回向量的最后一个元素，若向量为空，抛出 NoSuchElementException 异常|
|int lastIndexOf(Object element)|从向量末尾向前搜索向量，返回对象的下标值|
|int lastIndexOf(Object element,int start)|从指定位置开始向前搜索向量，返回给定对象的下标值，若未找到返回-1|
|void removeAllElements()|删除向量中的所有对象，向量变成空向量|
|boolean removeElement(Object element)|从向量中删除指定对象 element，若给定的对象在向量中保存多次，则只删除其第一个实例，如果删除成功，返回 true，如果没发现对象，则返回 false|
|void removeElementAt(int index)|删除由 index 指定位置处的元素|
|void setElementAt(Object obj,int index)|将给定对象存放到给定下标处，该下标处的原有对象丢失|
|void setSize(int size)|将向量中的元素个数设为 size，如果新的长度小于原来的长度，元素将丢失，若新的长度大于原来的长度，则在其后增加 null 元素|
|int size()|返回向量中当前元素的个数|
|String toString()|将向量转换成字符串|
|void trimToSize()|将向量的容量设为与当前拥有的元素个数相等|
与数组相同，向量对象也可以通过 new 操作符实现。其语句为：
Vector vector=new Vector();

