# Skywind Inside » 超越 STL-map/hash_map性能的DICT
## 超越 STL-map/hash_map性能的DICT
November 23rd, 2007[skywind](http://www.skywind.me/blog/archives/author/skywind)[Leave a comment](#respond)[Go to comments](#comments)
最近用纯C作了一个dict，根据试验，比 stl的 map快 2.3-2.5倍，比 hash_map快 1.3倍，主要用到的优化策略如下：
1. skiplist：
传统hash-bucket中，使用链表靠 next, 指针记录同一个 bucket中的各个节点，查找时候 需要一个一个的搜索，而改进以后的 bucket节点，使用 next, next4, next8三级指针，分别指向后一个节点，后四个节点，后八个节点，如此保证链表有序的情况下，如果要搜索，就直接考虑 next8的值和当前值大小，如果小于当前值，直接跳过八个节点，如果大于那再一次判断 next4与next。如此有序链表的遍历速度提高了 8倍。
2. 双hash：
使用两次hash，开辟一个长度为 64的 LRU数组，记录最近访问过的节点，使用 hash2来定位 LRU数组的位置，当搜索发生时，首先根据 hash2在 LRU数组中查找是否最近访问过该节点，如果访问过就直接返回，如果没有访问过则继续用 hash1在 bucket中查找，如果查找到的话，就按照 hash2的值覆盖一下 LRU中该位置的数据。
再加上一些内存管理方面的优化，以及一些编码方式的改进，对 key/value分别取 string或 int，四种情况下，平均比 stl的 map快 2.3-2.5倍，比 hash_map快 1.3倍。
