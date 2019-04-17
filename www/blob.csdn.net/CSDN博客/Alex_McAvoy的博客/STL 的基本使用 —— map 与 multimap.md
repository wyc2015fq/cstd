# STL 的基本使用 —— map 与 multimap - Alex_McAvoy的博客 - CSDN博客





2019年01月13日 20:42:37[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：69








# 【概述】

map 和 multimap 是映射数据容器，两者均定义与 <map> 头文件中，其所有元素都是 pair，pair 的第一个元素被视为键值，第二个元素为实值。

他们是基于某一类型 Key 的键集的存在，提供对 TYPE 类型的数据进行快速和高效的检索，具体实现采用了一颗红黑树的平衡二叉树的数据结构，这颗树具有对数据自动排序的功能，所以在 map 内部所有的数据 Key 都是有序的。

map 和 multimap 对象包涵了键和各个键有关的值，键和值的数据类型可以是不相同的。

两者的不同在于，map 不允许有重复键值，而 multimap 允许重复键值。

# 【定义】

定义：**map <type1, type2> mapName、multimap <type1, type2> multimapName**
- map<type1,type2> m：产生一个空的 map/multimap，其中不含任何元素
- map<type1,type2> m(op)：以 op 为排序准则，产生一个空的 map/multimap
- map<type1,type2> m1(m2)：产生某个 map/multimap 的副本，所有元素均被复制
- map<type1,type2> m(beg, end)：以迭代器区间 [beg; end] 内的元素产生一个 map/multimap
- map<type1,type2> m(beg, end, op)：：以 op 为排序准则，以迭代器区间 [beg; end] 内的元素生成一个map/multimap

# 【基本操作】

对于 map<type> m 的基本操作有：
- m.size()：返回容器大小
- m.empty()：返回容器是否为空
- m.clear()：清空容器
- m.count(key)：返回键值等于 key 的元素的个数
- m.erase(elem)：移除键值为 elem 的所有元素，返回个数，对 map 来说非 0 即 1
-  m[key] = value：查找的时候若没有键值为 key 的元素，则插入一个键值为 key 的新元素，实值为默认值(一般为 0)
- m.insert(elem)：用于插入元素，其有三种形式
	
1）运用 value_type 插入：

	      map<string, float> m;

	      m.insert(map<string, float>:: value_type ("Robin", 22.3));

	2）运用 pair<>：

	      m.insert(pair<string, float>("Robin", 22.3));

	3）运用 make_pair()：

	      m.insert(make_pair("Robin", 22.3));


# 【迭代器操作】

对于 map<type> m 的迭代器 it 的操作有：
- m.begin()：返回一个双向迭代器，指向第一个元素
- m.end()：返回一个双向迭代器，指向最后一个元素的下一个位置
- m.erase(it)：移除迭代器 it 所指位置上的元素，并返回指向下一个元素对的迭代器
- m.lower_bound(key)：返回键值等于 key 的元素的第一个可安插的位置
- m.upper_bound(key)：返回键值等于key的元素的最后一个可安插的位置



