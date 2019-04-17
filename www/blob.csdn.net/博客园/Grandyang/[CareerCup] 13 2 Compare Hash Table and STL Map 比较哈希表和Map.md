# [CareerCup] 13.2 Compare Hash Table and STL Map 比较哈希表和Map - Grandyang - 博客园







# [[CareerCup] 13.2 Compare Hash Table and STL Map 比较哈希表和Map](https://www.cnblogs.com/grandyang/p/4922286.html)







13.2 Compare and contrast a hash table and an STL map. How is a hash table implemented? If the number of inputs is small, which data structure options can be used instead of a hash table?



这道题让我们比较哈希表和STL中的map数据结构，在遇到这道题之前，我一直以为map就是c++中的哈希表呢，原来是不同的啊--!!!

哈希表是通过一个链表数组来实现的，链表中的每个节点包含了关键值key和数值value。数据不是有序存储的，是通过一个哈希函数来计算将要存进的位置index，查找或插入数据的时间复杂度是常数级的O(1)，这简直碉堡了。在哈希表中，我们要解决潜在的冲突问题，即两组数据要存进同一个位置，我们可以用链表来将相同位置的数据链起立，铁索连环啊~

而Map是通过在二叉树中基于键值key插入键值/数值对来实现的，无需处理冲突。

对于输入数据比较小的情况，我们也可以用Map或者是二叉树来实现，虽然时间复杂度是O(lgn)，但是由于输入数据小，其实也没啥太大差别。












