# [LeetCode] 4Sum 四数之和 - Grandyang - 博客园







# [[LeetCode] 4Sum 四数之和](https://www.cnblogs.com/grandyang/p/4515925.html)







Given an array *S* of *n* integers, are there elements *a*, *b*, *c*, and *d* in *S* such that *a* + *b* + *c* + *d* = target? Find all unique quadruplets in the array which gives the sum of target.

**Note:**

- Elements in a quadruplet (*a*,*b*,*c*,*d*) must be in non-descending order. (ie, *a* ≤ *b* ≤ *c* ≤ *d*)
- The solution set must not contain duplicate quadruplets.
    For example, given array S = {1 0 -1 0 -2 2}, and target = 0.

    A solution set is:
    (-1,  0, 0, 1)
    (-2, -1, 1, 2)
    (-2,  0, 0, 2)



LeetCode中关于数字之和还有其他几道，分别是 [Two Sum](http://www.cnblogs.com/grandyang/p/4130379.html) ，[3Sum](http://www.cnblogs.com/grandyang/p/4481576.html) ，[3Sum Closest](http://www.cnblogs.com/grandyang/p/4510984.html) ，虽然难度在递增，但是整体的套路都是一样的，在这里为了避免重复项，我们使用了STL中的set，其特点是不能有重复，如果新加入的数在set中原本就存在的话，插入操作就会失败，这样能很好的避免的重复项的存在。此题的O(n^3)解法的思路跟 [3Sum](http://www.cnblogs.com/grandyang/p/4481576.html) 基本没啥区别，就是多加了一层for循环，其他的都一样，代码如下：



解法一：

```
class Solution {
public:
    vector<vector<int>> fourSum(vector<int> &nums, int target) {
        set<vector<int>> res;
        sort(nums.begin(), nums.end());
        for (int i = 0; i < int(nums.size() - 3); ++i) {
            for (int j = i + 1; j < int(nums.size() - 2); ++j) {
                if (j > i + 1 && nums[j] == nums[j - 1]) continue;
                int left = j + 1, right = nums.size() - 1;
                while (left < right) {
                    int sum = nums[i] + nums[j] + nums[left] + nums[right];
                    if (sum == target) {
                        vector<int> out{nums[i], nums[j], nums[left], nums[right]};
                        res.insert(out);
                        ++left; --right;
                    } else if (sum < target) ++left;
                    else --right;
                }
            }
        }
        return vector<vector<int>>(res.begin(), res.end());
    }
};
```



但是毕竟用set来进行去重复的处理还是有些取巧，可能在Java中就不能这么做，那么我们还是来看一种比较正统的做法吧，手动进行去重复处理。主要可以进行的有三个地方，首先在两个for循环下可以各放一个，因为一旦当前的数字跟上面处理过的数字相同了，那么找下来肯定还是重复的。之后就是当sum等于target的时候了，我们在将四个数字加入结果res之后，left和right都需要去重复处理，分别像各自的方面遍历即可，参见代码如下：



解法二：

```
class Solution {
public:
    vector<vector<int>> fourSum(vector<int> &nums, int target) {
        vector<vector<int>> res;
        int n = nums.size();
        sort(nums.begin(), nums.end());
        for (int i = 0; i < n - 3; ++i) {
            if (i > 0 && nums[i] == nums[i - 1]) continue;
            for (int j = i + 1; j < n - 2; ++j) {
                if (j > i + 1 && nums[j] == nums[j - 1]) continue;
                int left = j + 1, right = n - 1;
                while (left < right) {
                    int sum = nums[i] + nums[j] + nums[left] + nums[right];
                    if (sum == target) {
                        vector<int> out{nums[i], nums[j], nums[left], nums[right]};
                        res.push_back(out);
                        while (left < right && nums[left] == nums[left + 1]) ++left;
                        while (left < right && nums[right] == nums[right - 1]) --right;
                        ++left; --right;
                    } else if (sum < target) ++left;
                    else --right;
                }
            }
        }
        return res;
    }
};
```



类似题目：

[Two Sum](http://www.cnblogs.com/grandyang/p/4130379.html)

[3Sum](http://www.cnblogs.com/grandyang/p/4481576.html)

[4Sum II](http://www.cnblogs.com/grandyang/p/6073317.html)



参考资料：

[https://leetcode.com/problems/4sum/](https://leetcode.com/problems/4sum/)

[https://leetcode.com/problems/4sum/discuss/8549/My-16ms-c%2B%2B-code](https://leetcode.com/problems/4sum/discuss/8549/My-16ms-c%2B%2B-code)

[https://leetcode.com/problems/4sum/discuss/8575/Clean-accepted-java-O(n3)-solution-based-on-3sum](https://leetcode.com/problems/4sum/discuss/8575/Clean-accepted-java-O(n3)-solution-based-on-3sum)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












