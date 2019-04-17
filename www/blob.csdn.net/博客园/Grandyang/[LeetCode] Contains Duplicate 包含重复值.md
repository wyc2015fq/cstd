# [LeetCode] Contains Duplicate 包含重复值 - Grandyang - 博客园







# [[LeetCode] Contains Duplicate 包含重复值](https://www.cnblogs.com/grandyang/p/4537029.html)







Given an array of integers, find if the array contains any duplicates. Your function should return true if any value appears at least twice in the array, and it should return false if every element is distinct.



这道题不算难题，就是使用一个哈希表，遍历整个数组，如果哈希表里存在，返回false，如果不存在，则将其放入哈希表中，代码如下：

 解法一

```
class Solution {
public:
    bool containsDuplicate(vector<int>& nums) {
        unordered_map<int, int> m;
        for (int i = 0; i < nums.size(); ++i) {
            if (m.find(nums[i]) != m.end()) return true;
            ++m[nums[i]];
        }
        return false;
    }
};
```



这题还有另一种解法，就是先将数组排个序，然后再比较相邻两个数字是否相等，时间复杂度取决于排序方法，代码如下：

解法二

```
class Solution {
public:
    bool containsDuplicate(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        for (int i = 1; i < nums.size(); ++i) {
            if (nums[i] == nums[i - 1]) return true;
        }
        return false;
    }
};
```



相似题目：

[Contains Duplicate II 包含重复值之二](http://www.cnblogs.com/grandyang/p/4539680.html)

[Contains Duplicate III 包含重复值之三](http://www.cnblogs.com/grandyang/p/4545261.html)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












