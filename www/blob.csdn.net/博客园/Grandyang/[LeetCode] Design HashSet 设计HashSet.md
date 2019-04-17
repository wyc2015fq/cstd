# [LeetCode] Design HashSet 设计HashSet - Grandyang - 博客园







# [[LeetCode] Design HashSet 设计HashSet](https://www.cnblogs.com/grandyang/p/9966807.html)







Design a HashSet without using any built-in hash table libraries.

To be specific, your design should include these functions:
- `add(value)`: Insert a value into the HashSet. 
- `contains(value)` : Return whether the value exists in the HashSet or not.
- `remove(value)`: Remove a value in the HashSet. If the value does not exist in the HashSet, do nothing.


Example:
MyHashSet hashSet = new MyHashSet();
hashSet.add(1);         
hashSet.add(2);         
hashSet.contains(1);    // returns true
hashSet.contains(3);    // returns false (not found)
hashSet.add(2);          
hashSet.contains(2);    // returns true
hashSet.remove(2);          
hashSet.contains(2);    // returns false (already removed)


Note:
- All values will be in the range of `[0, 1000000]`.
- The number of operations will be in the range of `[1, 10000]`.
- Please do not use the built-in HashSet library.



这道题让我们设计HashSet，不能用自带的哈希表的数据结构，而且要我们主要实现添加，删除，以及判断是否存在，这三个函数。我们都知道HashSet最有用的地方就是其能够在常数的时间内判断某个元素是否存在，这都得归功于哈希表的作用。但是现在不让我们用了，但我们还是得保证其常数级的查找效率，那么就用空间来换时间吧。既然题目中说了数字的范围不会超过1000000，那么我们就申请这么大空间的数组，这样对于在HashSet中的数字，我们就将其标记为1，不在或者删除了的就标记为0，检测的时候就看其值是否为1即可，参见代码如下：



解法一：

```
class MyHashSet {
public:
    /** Initialize your data structure here. */
    MyHashSet() {
        data.resize(1000000, 0);
    }
    
    void add(int key) {
        data[key] = 1;
    }
    
    void remove(int key) {
        data[key] = 0;
    }
    
    /** Returns true if this set contains the specified element */
    bool contains(int key) {
        return data[key] == 1;
    }
    
private:
    vector<int> data;
};
```



我们可以来适度的优化一下空间复杂度，由于存入HashSet的数字也许不会跨度很大，那么直接就申请长度为1000000的数组可能会有些浪费，那么我们其实可以使用1000个长度为1000的数组来代替，那么就要用个二维数组啦，实际上开始我们只申请了1000个空数组，对于每个要处理的元素，我们首先对1000取余，得到的值就当作哈希值，对应我们申请的那1000个空数组的位置，在加入元素时，一旦计算出了哈希值，我们将对应的空数组resize为长度1000，然后根据哈希值和key/1000来确定具体的加入位置。移除数字一样的，先计算出哈希值，如果对应的数组不为空的话，找到对应的位置并赋值为0。不过大家也可以看出来，我们在加入元素时会开辟1000的新空间，但是删除这个元素时，并没有检测这1000个位置是否均为0，是的话应该删除这1000个新空间。但是这样可能会使得删除函数变慢一些，参见代码如下： 



解法二：

```
class MyHashSet {
public:
    /** Initialize your data structure here. */
    MyHashSet() {
        data.resize(1000, vector<int>());
    }
    
    void add(int key) {
        int hashKey = key % 1000;
        if (data[hashKey].empty()) {
            data[hashKey].resize(1000);
        } 
        data[hashKey][key / 1000] = 1;
    }
    
    void remove(int key) {
        int hashKey = key % 1000;
        if (!data[hashKey].empty()) {
            data[hashKey][key / 1000] = 0;
        } 
    }
    
    /** Returns true if this set contains the specified element */
    bool contains(int key) {
        int hashKey = key % 1000;
        return !data[hashKey].empty() && data[hashKey][key / 1000];
    }
    
private:
    vector<vector<int>> data;
};
```



类似题目：

[Design HashMap](https://www.cnblogs.com/grandyang/p/9972764.html)



参考资料：

[https://leetcode.com/problems/design-hashset/](https://leetcode.com/problems/design-hashset/)

[https://leetcode.com/problems/design-hashset/discuss/185826/C%2B%2B-solution](https://leetcode.com/problems/design-hashset/discuss/185826/C%2B%2B-solution)

[https://leetcode.com/problems/design-hashset/discuss/193132/Solution-without-boolean-array](https://leetcode.com/problems/design-hashset/discuss/193132/Solution-without-boolean-array)

[https://leetcode.com/problems/design-hashset/discuss/143434/Beats-100-Real-Java-Solution-(Not-boolean-array)](https://leetcode.com/problems/design-hashset/discuss/143434/Beats-100-Real-Java-Solution-(Not-boolean-array))



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












