# [LeetCode] Contains Duplicate II 包含重复值之二 - Grandyang - 博客园







# [[LeetCode] Contains Duplicate II 包含重复值之二](https://www.cnblogs.com/grandyang/p/4539680.html)







Given an array of integers and an integer k, return true if and only if there are two distinct indices i and j in the array such that nums[i] = nums[j] and the difference between i and j is at most k. (Old Version)

Given an array of integers and an integer *k*, find out whether there are two distinct indices *i* and *j* in the array such that **nums[i] = nums[j]** and the difference between *i* and *j* is at most *k*. (New Version)



这道题是之前那道[Contains Duplicate 包含重复值](http://www.cnblogs.com/grandyang/p/4537029.html)的延伸，不同之处在于那道题只要我们判断下数组中是否有重复值，而这道题限制了数组中只许有一组重复的数字，而且他们坐标差不能超过k。那么我们首先需要一个哈希表，来记录每个数字和其坐标的映射，然后我们需要一个变量d来记录第一次出现重复数字的坐标差。由于题目要求只能有一组重复的数字，所以我们在遇到重复数字时，首先判断d是否已经存了值，如果d已经有值了，说明之前有过了重复数字，则直接返回false即可。如果没有，则此时给d附上值。在网上看到有些解法在这里就直接判断d和k的关系然后返回结果了，其实这样是不对的。因为题目要求只能有一组重复数，就是说如果后面又出现了重复数，就没法继续判断了。所以正确的做法应该是扫描完整个数组后在判断，先看d有没有存入结果，如果没有，则说明没出现过重复数， 返回false即可。如果d有值，再跟k比较，返回对应的结果。OJ的test case没有包含所有的情况，比如当nums = [1, 2, 3, 1, 3], k = 3时，实际上应该返回false，但是有些返回true的算法也能通过OJ。个人认为正确的解法应该入下：



```
// Wrong Soulution
class Solution {
public:
    bool containsNearbyDuplicate(vector<int>& nums, int k) {
        unordered_map<int, int> m;
        int d = 0;
        for (int i = 0; i < nums.size(); ++i) {
            if (m.find(nums[i]) != m.end()) {
                if (d > 0) return false;
                d = i - m[nums[i]];
            }
            m[nums[i]] = i;
        }
        return d == 0 ? false : d <= k;
    }
};
```



坑爹啊，题目要求变了，那么就没啥歧义了，正确解法如下：



```
class Solution {
public:
    bool containsNearbyDuplicate(vector<int>& nums, int k) {
        unordered_map<int, int> m;
        for (int i = 0; i < nums.size(); ++i) {
            if (m.find(nums[i]) != m.end() && i - m[nums[i]] <= k) return true;
            else m[nums[i]] = i;
        }
        return false;
    }
};
```



相似题目：

[Contains Duplicate 包含重复值](http://www.cnblogs.com/grandyang/p/4537029.html)

[Contains Duplicate III 包含重复值之三](http://www.cnblogs.com/grandyang/p/4545261.html)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












