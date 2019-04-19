# Java TreeMap / HashMap 区别 - Simple 专栏 - CSDN博客
2012年01月19日 16:18:45[Simple_Zz](https://me.csdn.net/love284969214)阅读数：391
所属专栏：[Java 浅谈](https://blog.csdn.net/column/details/27397.html)
一般情况下我们选用HashMap，因为HashMap的键值对在取出时是随机的，其依据键的hashCode和键的equals方法存取数据，具有很快的访问速度，所以在Map中插入、删除及索引元素时其是效率最高的实现。而TreeMap的键值对在取出时是排过序的，所以效率会低点。
- HashMap：基于哈希表实现。使用HashMap要求添加的键类明确定义了hashCode()和equals()(可以重写hashCode()和equals()),为了优化HashMap空间的使用，可以调优初始容量和负载因子。
- TreeMap：基于红黑树实现。TreeMap没有调优选项，因为该树总处于平衡状态。
- HashMap：适用于在Map中插入、删除和定位元素。
- TreeMap：适用于按自然顺序或自定义顺序遍历键(key)。
- HashMap通常比TreeMap快一点，建议多使用HashMap，在需要排序的Map时候才用TreeMap。
- HashMap的结果是没有排序的。
- TreeMap实现**SortMap**接口，能够把它保存的记录根据键排序，默认是按键值的升序排序，也可以指定排序的比较器，当用Iterator遍历**TreeMap**时，得到的记录是排过序的。
- HashMap里面存入的键值对在取出的时候是随机的，它根据键的HashCode值存储数据，根据键可以直接获取它的值，具有很快的访问速度。在Map中插入、删除和定位元素，HashMap是最好的选择。
- TreeMap取出来的是排序后的键值对。但如果您要按自然顺序或自定义顺序遍历键，那么TreeMap会更好。
||ThreeMap|HashMap|
|----|----|----|
|继承|AbstractMap|AbstractMap|
|实现|SortMap接口，基于红黑树|基于哈希散列表实现|
|存储|默认按键的升序排序|随机存储|
|遍历|Iterator遍历是排序的|Iterator遍历是随机的|
|性能损耗|插入、删除|基本无|
|键值对|键、值都不能为null|键、值均能为null|
|安全|非并发安全Map|非并发安全Map|
|效率|低|高|
