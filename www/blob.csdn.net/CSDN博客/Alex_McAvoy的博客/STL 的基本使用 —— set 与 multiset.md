# STL 的基本使用 —— set 与 multiset - Alex_McAvoy的博客 - CSDN博客





2019年01月13日 20:20:37[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：50








# 【概述】

set 和 multiset 是集合数据容器，通过链表进行组织，具体实现采用了红黑树的平衡二叉树的数据结构，两者均定义在 <set> 头文件中，其会根据特定的规则，自动将元素排序。

两者不同在于，set 不允许元素重复，而 multiset 允许元素重复。

在插入操作和删除操作上比 vector 快，但查找或添加末尾的元素时会有些慢。

# 【定义】

定义： **set<type> setName、multiset<type> multisetName**
- set<type> s：产生一个空的 set/multiset，其中不含任何元素
- set<type> s(op)：以 op 为排序准则，生成一个 set/multiset
- set<type> s1(s2)：产生某个 set/multiset 的副本，所有元素均被复制
- set<type> s(beg, end)：以迭代器的区间 [beg; end] 内的元素产生一个 set/multiset
- set<type> s(beg, end, op)：以 op 为排序准则，利用迭代器的区间 [beg; end] 内的元素生成一个 set/multiset

默认情况下，由小到大排序，若想按照自己的方式排序，一般重载小于号

```cpp
struct type{
    int x,y;
    bool operator < (const type &a) const{
        if(x!=a.x)
            return x<a.x;
        return y<a.y;
    }
}
set<type> s;
```

# 【基本操作】

对于 set<type> s 的基本操作有：
- s.size()：返回容器大小
- s.empty()：返回容器是否为空。
- s.clear()：清空容器
- s.count(elem)：返回元素值为 elem 的元素的个数
- s.insert(elem) ：加入一个 elem 副本，返回新元素位置
- s.erase(elem)：移除与 elem 元素相等的所有元素，返回被移除的元素个数

# 【迭代器操作】

对于 set<type> s 的迭代器 it 的操作有：
- s.begin()：返回一个双向迭代器，指向第一个元素
- s.end()：返回一个双向迭代器，指向最后一个元素的下一 个位置
- s.erase(it)：移除迭代器 it 所指位置上的元素，无返回值
- s.lower_bound(elem)：返回 elem 的第一个可安插的位置，即元素值 >= elem 的第一个元素位置
- s.upper_bound(elem)：返回 elem 的最后一个可安插的位置，即元素值 > elem 的第一个元素的位置









