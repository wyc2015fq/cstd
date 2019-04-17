# [LeetCode] Map Sum Pairs 映射配对之和 - Grandyang - 博客园







# [[LeetCode] Map Sum Pairs 映射配对之和](https://www.cnblogs.com/grandyang/p/7616525.html)








Implement a MapSum class with `insert`, and `sum` methods.

For the method `insert`, you'll be given a pair of (string, integer). The string represents the key and the integer represents the value. If the key already existed, then the original key-value pair will be overridden to the new one.

For the method `sum`, you'll be given a string representing the prefix, and you need to return the sum of all the pairs' value whose key starts with the prefix.

Example 1:
Input: insert("apple", 3), Output: Null
Input: sum("ap"), Output: 3
Input: insert("app", 2), Output: Null
Input: sum("ap"), Output: 5



这道题让我们实现一个MapSum类，里面有两个方法，insert和sum，其中inser就是插入一个键值对，而sum方法比较特别，是在找一个前缀，需要将所有有此前缀的单词的值累加起来返回。看到这种玩前缀的题，照理来说是要用前缀树来做的。但是博主一般想偷懒，不想新写一个结构或类，于是就使用map来代替前缀树啦。博主开始想到的方法是建立前缀和一个pair之间的映射，这里的pair的第一个值表示该词的值，第二个值表示将该词作为前缀的所有词的累加值，那么我们的sum函数就异常的简单了，直接将pair中的两个值相加即可。关键就是要在insert中把数据结构建好，构建的方法也不难，首先我们suppose原本这个key是有值的，我们更新的时候只需要加上它的差值即可，就算key不存在默认就是0，算差值也没问题。然后我们将first值更新为val，然后就是遍历其所有的前缀了，给每个前缀的second都加上diff即可，参见代码如下：



解法一：

```
class MapSum {
public:
    /** Initialize your data structure here. */
    MapSum() {}
    
    void insert(string key, int val) {
        int diff = val - m[key].first, n = key.size();
        m[key].first = val;
        for (int i = n - 1; i > 0; --i) {
            m[key.substr(0, i)].second += diff;
        }
    }
    
    int sum(string prefix) {
        return m[prefix].first + m[prefix].second;
    }

private:
    unordered_map<string, pair<int, int>> m;
};
```



下面这种方法是论坛上投票最高的方法，感觉很叼，用的是带排序的map，insert就是把单词加入map。在map里会按照字母顺序自动排序，然后在sum函数里，我们根据prefix来用二分查找快速定位到第一个不小于prefix的位置，然后向后遍历，向后遍历的都是以prefix为前缀的单词，如果我们发现某个单词不是以prefix为前缀了，直接break；否则就累加其val值，参见代码如下：



解法二：

```
class MapSum {
public:
    /** Initialize your data structure here. */
    MapSum() {}
    
    void insert(string key, int val) {
        m[key] = val;
    }
    
    int sum(string prefix) {
        int res = 0, n = prefix.size();
        for (auto it = m.lower_bound(prefix); it != m.end(); ++it) {
            if (it->first.substr(0, n) != prefix) break;
            res += it->second;
        }
        return res;
    }
    
private:
    map<string, int> m;
};
```



参考资料：

[https://discuss.leetcode.com/topic/103924/java-map-solution](https://discuss.leetcode.com/topic/103924/java-map-solution)

[https://discuss.leetcode.com/topic/104006/c-easy-solution-ordered-map](https://discuss.leetcode.com/topic/104006/c-easy-solution-ordered-map)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













