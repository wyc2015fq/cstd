# [LeetCode] LRU Cache 最近最少使用页面置换缓存器 - Grandyang - 博客园







# [[LeetCode] LRU Cache 最近最少使用页面置换缓存器](https://www.cnblogs.com/grandyang/p/4587511.html)







Design and implement a data structure for [Least Recently Used (LRU) cache](https://en.wikipedia.org/wiki/Cache_replacement_policies#LRU). It should support the following operations: `get` and `put`.

`get(key)` - Get the value (will always be positive) of the key if the key exists in the cache, otherwise return -1.
`put(key, value)` - Set or insert the value if the key is not already present. When the cache reached its capacity, it should invalidate the least recently used item before inserting a new item.

Follow up:
Could you do both operations in O(1) time complexity?

Example:
LRUCache cache = new LRUCache( 2 /* capacity */ );

cache.put(1, 1);
cache.put(2, 2);
cache.get(1);       // returns 1
cache.put(3, 3);    // evicts key 2
cache.get(2);       // returns -1 (not found)
cache.put(4, 4);    // evicts key 1
cache.get(1);       // returns -1 (not found)
cache.get(3);       // returns 3
cache.get(4);       // returns 4


这道题让我们实现一个LRU缓存器，LRU是Least Recently Used的简写，就是最近最少使用的意思。那么这个缓存器主要有两个成员函数，get和put，其中get函数是通过输入key来获得value，如果成功获得后，这对(key, value)升至缓存器中最常用的位置（顶部），如果key不存在，则返回-1。而put函数是插入一对新的(key, value)，如果原缓存器中有该key，则需要先删除掉原有的，将新的插入到缓存器的顶部。如果不存在，则直接插入到顶部。若加入新的值后缓存器超过了容量，则需要删掉一个最不常用的值，也就是底部的值。具体实现时我们需要三个私有变量，cap, l和m，其中cap是缓存器的容量大小，l是保存缓存器内容的列表，m是HashMap，保存关键值key和缓存器各项的迭代器之间映射，方便我们以O(1)的时间内找到目标项。

然后我们再来看get和put如何实现，get相对简单些，我们在HashMap中查找给定的key，若不存在直接返回-1。如果存在则将此项移到顶部，这里我们使用C++ STL中的函数splice，专门移动链表中的一个或若干个结点到某个特定的位置，这里我们就只移动key对应的迭代器到列表的开头，然后返回value。这里再解释一下为啥HashMap不用更新，因为HashMap的建立的是关键值key和缓存列表中的迭代器之间的映射，get函数是查询函数，如果关键值key不在HashMap，那么不需要更新。如果在，我们需要更新的是该key-value键值对儿对在缓存列表中的位置，而HashMap中还是这个key跟键值对儿的迭代器之间的映射，并不需要更新什么。

对于put，我们也是现在HashMap中查找给定的key，如果存在就删掉原有项，并在顶部插入新来项，然后判断是否溢出，若溢出则删掉底部项(最不常用项)。代码如下：



```
class LRUCache{
public:
    LRUCache(int capacity) {
        cap = capacity;
    }
    
    int get(int key) {
        auto it = m.find(key);
        if (it == m.end()) return -1;
        l.splice(l.begin(), l, it->second);
        return it->second->second;
    }
    
    void put(int key, int value) {
        auto it = m.find(key);
        if (it != m.end()) l.erase(it->second);
        l.push_front(make_pair(key, value));
        m[key] = l.begin();
        if (m.size() > cap) {
            int k = l.rbegin()->first;
            l.pop_back();
            m.erase(k);
        }
    }
    
private:
    int cap;
    list<pair<int, int>> l;
    unordered_map<int, list<pair<int, int>>::iterator> m;
};
```



类似题目：

[LFU Cache](http://www.cnblogs.com/grandyang/p/6258459.html)

[Design In-Memory File System](http://www.cnblogs.com/grandyang/p/6944331.html)

[Design Compressed String Iterator](http://www.cnblogs.com/grandyang/p/7026999.html)



参考资料：

[https://leetcode.com/problems/lru-cache/](https://leetcode.com/problems/lru-cache/)

[http://www.cnblogs.com/TenosDoIt/p/3417157.html](http://www.cnblogs.com/TenosDoIt/p/3417157.html)

[https://leetcode.com/problems/lru-cache/discuss/46285/unordered_map-list](https://leetcode.com/problems/lru-cache/discuss/46285/unordered_map-list)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












