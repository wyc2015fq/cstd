# leetcode-146. LRU Cache LRU缓存机制 - 别说话写代码的博客 - CSDN博客





2019年02月20日 23:13:00[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：12标签：[LRU Cache																[LRU																[LRU缓存机制																[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=LRU缓存机制&t=blog)



|Design and implement a data structure for [Least Recently Used (LRU) cache](https://en.wikipedia.org/wiki/Cache_replacement_policies#LRU). It should support the following operations: `get` and `put`.`get(key)` - Get the value (will always be positive) of the key if the key exists in the cache, otherwise return -1.`put(key, value)` - Set or insert the value if the key is not already present. When the cache reached its capacity, it should invalidate the least recently used item before inserting a new item.**Follow up:**			Could you do both operations in **O(1)** time complexity?**Example:**LRUCache cache = new LRUCache( 2 /* capacity */ );cache.put(1, 1);cache.put(2, 2);cache.get(1);       // returns 1cache.put(3, 3);    // evicts key 2cache.get(2);       // returns -1 (not found)cache.put(4, 4);    // evicts key 1cache.get(1);       // returns -1 (not found)cache.get(3);       // returns 3cache.get(4);       // returns 4|运用你所掌握的数据结构，设计和实现一个  [LRU (最近最少使用) 缓存机制](https://baike.baidu.com/item/LRU)。它应该支持以下操作： 获取数据 `get` 和 写入数据 `put` 。获取数据 `get(key)` - 如果密钥 (key) 存在于缓存中，则获取密钥的值（总是正数），否则返回 -1。			写入数据 `put(key, value)` - 如果密钥不存在，则写入其数据值。当缓存容量达到上限时，它应该在写入新数据之前删除最近最少使用的数据值，从而为新的数据值留出空间。**进阶:**你是否可以在 **O(1)** 时间复杂度内完成这两种操作？**示例:**LRUCache cache = new LRUCache( 2 /* 缓存容量 */ );cache.put(1, 1);cache.put(2, 2);cache.get(1);       // 返回  1cache.put(3, 3);    // 该操作会使得密钥 2 作废cache.get(2);       // 返回 -1 (未找到)cache.put(4, 4);    // 该操作会使得密钥 1 作废cache.get(1);       // 返回 -1 (未找到)cache.get(3);       // 返回  3cache.get(4);       // 返回  4|
|----|----|

思路：线性容器存储key- value值，我们可以选择list，这样调整节点 只需要O（1）。而用数组需要O(N)。然后用hash 表保存key与 对应链表之间的映射 ，可以以O（1）的时间查找到 位置并将其插入链表 首节点。

如过来了get 请求，先判断key是否在hash表中，若在则将 链表中节点 的位置移动到 链表首，否则返回-1.

set请求，先判断key是否在hash 表中，若在则将key对应的value值更新，并将其移动到链表首部。若不在，则在链表和hash表中都插入，如果添加后的长度大于缓存容量size时，删除链表最后一个节点，并在hash表中删除

```cpp
class LRUCache {
public:
    LRUCache(int capacity):size(capacity) {}
    
    int get(int key) {
        auto it = hash.find(key);
        if(it == hash.end()) return -1;
        cache.splice(cache.begin(),cache,it->second);
        return it->second->second;
    }
    
    void put(int key, int value) {
        auto it = hash.find(key);
        if(it != hash.end())
        {
            it->second->second = value;
            return cache.splice(cache.begin(),cache,it->second);
        }
        cache.insert(cache.begin(),make_pair(key,value));
        hash[key] = cache.begin();
        if(cache.size() > size)
        {
            hash.erase(cache.back().first); //按照key删除
            cache.pop_back();
        }
    }
private:
    unordered_map<int,list<pair<int,int>>::iterator> hash; 
    list<pair<int,int>> cache;
    int size;
};

/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache obj = new LRUCache(capacity);
 * int param_1 = obj.get(key);
 * obj.put(key,value);
 */
```](https://so.csdn.net/so/search/s.do?q=LRU&t=blog)](https://so.csdn.net/so/search/s.do?q=LRU Cache&t=blog)




