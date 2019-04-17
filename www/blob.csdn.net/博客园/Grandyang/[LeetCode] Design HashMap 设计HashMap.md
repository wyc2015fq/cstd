# [LeetCode] Design HashMap 设计HashMap - Grandyang - 博客园







# [[LeetCode] Design HashMap 设计HashMap](https://www.cnblogs.com/grandyang/p/9972764.html)







Design a HashMap without using any built-in hash table libraries.

To be specific, your design should include these functions:
- `put(key, value)` : Insert a (key, value) pair into the HashMap. If the value already exists in the HashMap, update the value.
- `get(key)`: Returns the value to which the specified key is mapped, or -1 if this map contains no mapping for the key.
- `remove(key)` : Remove the mapping for the value key if this map contains the mapping for the key.


Example:
MyHashMap hashMap = new MyHashMap();
hashMap.put(1, 1);          
hashMap.put(2, 2);         
hashMap.get(1);            // returns 1
hashMap.get(3);            // returns -1 (not found)
hashMap.put(2, 1);          // update the existing value
hashMap.get(2);            // returns 1 
hashMap.remove(2);          // remove the mapping for 2
hashMap.get(2);            // returns -1 (not found) 


Note:
- All keys and values will be in the range of `[0, 1000000]`.
- The number of operations will be in the range of `[1, 10000]`.
- Please do not use the built-in HashMap library.



这道题让我们设计一个HashMap的数据结构，不能使用自带的哈希表，跟之前那道[Design HashSet](https://www.cnblogs.com/grandyang/p/9966807.html)很类似，之前那道的两种解法在这里也是行得通的，既然题目中说了数字的范围不会超过1000000，那么我们就申请这么大空间的数组，只需将数组的初始化值改为-1即可。空间足够大了，我们就可以直接建立映射，移除时就将映射值重置为-1，由于默认值是-1，所以获取映射值就可以直接去，参见代码如下：



解法一：

```
class MyHashMap {
public:
    /** Initialize your data structure here. */
    MyHashMap() {
        data.resize(1000000, -1);
    }
    
    /** value will always be non-negative. */
    void put(int key, int value) {
        data[key] = value;
    }
    
    /** Returns the value to which the specified key is mapped, or -1 if this map contains no mapping for the key */
    int get(int key) {
        return data[key];
    }
    
    /** Removes the mapping of the specified value key if this map contains a mapping for the key */
    void remove(int key) {
        data[key] = -1;
    }

private:
    vector<int> data;
};
```



我们可以来适度的优化一下空间复杂度，由于存入HashMap的映射对儿也许不会跨度很大，那么直接就申请长度为1000000的数组可能会有些浪费，那么我们其实可以使用1000个长度为1000的数组来代替，那么就要用个二维数组啦，实际上开始我们只申请了1000个空数组，对于每个要处理的元素，我们首先对1000取余，得到的值就当作哈希值，对应我们申请的那1000个空数组的位置，在建立映射时，一旦计算出了哈希值，我们将对应的空数组resize为长度1000，然后根据哈希值和key/1000来确定具体的加入映射值的位置。获取映射值时，计算出哈希值，若对应的数组不为空，直接返回对应的位置上的值。移除映射值一样的，先计算出哈希值，如果对应的数组不为空的话，找到对应的位置并重置为-1。参见代码如下：



解法二：

```
class MyHashMap {
public:
    /** Initialize your data structure here. */
    MyHashMap() {
        data.resize(1000, vector<int>());
    }
    
    /** value will always be non-negative. */
    void put(int key, int value) {
        int hashKey = key % 1000;
        if (data[hashKey].empty()) {
            data[hashKey].resize(1000, -1);
        } 
        data[hashKey][key / 1000] = value;
    }
    
    /** Returns the value to which the specified key is mapped, or -1 if this map contains no mapping for the key */
    int get(int key) {
        int hashKey = key % 1000;
        if (!data[hashKey].empty()) {
            return data[hashKey][key / 1000];
        } 
        return -1;
    }
    
    /** Removes the mapping of the specified value key if this map contains a mapping for the key */
    void remove(int key) {
        int hashKey = key % 1000;
        if (!data[hashKey].empty()) {
            data[hashKey][key / 1000] = -1;
        } 
    }

private:
    vector<vector<int>> data;
};
```



类似题目：

[Design HashSet](https://www.cnblogs.com/grandyang/p/9966807.html)



参考资料：

[https://leetcode.com/problems/design-hashmap](https://leetcode.com/problems/design-hashmap)

[https://leetcode.com/problems/design-hashmap/discuss/152746/Java-Solution](https://leetcode.com/problems/design-hashmap/discuss/152746/Java-Solution)

[https://leetcode.com/problems/design-hashmap/discuss/184764/Easy-C%2B%2B-Solution-beats-98.01(52-msec)-using-array-of-vectors](https://leetcode.com/problems/design-hashmap/discuss/184764/Easy-C%2B%2B-Solution-beats-98.01(52-msec)-using-array-of-vectors)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












