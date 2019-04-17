# [LeetCode] 3Sum 三数之和 - Grandyang - 博客园







# [[LeetCode] 3Sum 三数之和](https://www.cnblogs.com/grandyang/p/4481576.html)







Given an array *S* of *n* integers, are there elements *a*, *b*, *c* in *S* such that *a* + *b* + *c* = 0? Find all unique triplets in the array which gives the sum of zero.

Note:
- Elements in a triplet (*a*,*b*,*c*) must be in non-descending order. (ie, *a* ≤ *b* ≤ *c*)
- The solution set must not contain duplicate triplets.


    For example, given array S = {-1 0 1 2 -1 -4},

    A solution set is:
    (-1, 0, 1)
    (-1, -1, 2)


这道题让我们求三数之和，比之前那道[Two Sum](http://www.cnblogs.com/grandyang/p/4130379.html)要复杂一些，博主考虑过先fix一个数，然后另外两个数使用[Two Sum](http://www.cnblogs.com/grandyang/p/4130379.html)那种HashMap的解法，但是会有重复结果出现，就算使用set来去除重复也不行，会TLE，看来此题并不是考我们[Two Sum](http://www.cnblogs.com/grandyang/p/4130379.html)的解法。那么我们来分析一下这道题的特点，要我们找出三个数且和为0，那么除了三个数全是0的情况之外，肯定会有负数和正数，我们还是要先fix一个数，然后去找另外两个数，我们只要找到两个数且和为第一个fix数的相反数就行了，既然另外两个数不能使用[Two Sum](http://www.cnblogs.com/grandyang/p/4130379.html)的那种解法来找，如果能更有效的定位呢？我们肯定不希望遍历所有两个数的组合吧，所以如果数组是有序的，那么我们就可以用双指针以线性时间复杂度来遍历所有满足题意的两个数组合。

我们对原数组进行排序，然后开始遍历排序后的数组，这里注意不是遍历到最后一个停止，而是到倒数第三个就可以了。这里我们可以先做个剪枝优化，就是当遍历到正数的时候就break，为啥呢，因为我们的数组现在是有序的了，如果第一个要fix的数就是正数了，那么后面的数字就都是正数，就永远不会出现和为0的情况了。然后我们还要加上重复就跳过的处理，处理方法是从第二个数开始，如果和前面的数字相等，就跳过，因为我们不想把相同的数字fix两次。对于遍历到的数，用0减去这个fix的数得到一个target，然后只需要再之后找到两个数之和等于target即可。我们用两个指针分别指向fix数字之后开始的数组首尾两个数，如果两个数和正好为target，则将这两个数和fix的数一起存入结果中。然后就是跳过重复数字的步骤了，两个指针都需要检测重复数字。如果两数之和小于target，则我们将左边那个指针i右移一位，使得指向的数字增大一些。同理，如果两数之和大于target，则我们将右边那个指针j左移一位，使得指向的数字减小一些，代码如下：



解法一：

```
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        vector<vector<int>> res;
        sort(nums.begin(), nums.end());
        if (nums.empty() || nums.back() < 0 || nums.front() > 0) return {};
        for (int k = 0; k < nums.size(); ++k) {
            if (nums[k] > 0) break;
            if (k > 0 && nums[k] == nums[k - 1]) continue;
            int target = 0 - nums[k];
            int i = k + 1, j = nums.size() - 1;
            while (i < j) {
                if (nums[i] + nums[j] == target) {
                    res.push_back({nums[k], nums[i], nums[j]});
                    while (i < j && nums[i] == nums[i + 1]) ++i;
                    while (i < j && nums[j] == nums[j - 1]) --j;
                    ++i; --j;
                } else if (nums[i] + nums[j] < target) ++i;
                else --j;
            }
        }
        return res;
    }
};
```



或者我们也可以利用set的不能包含重复项的特点来防止重复项的产生，那么我们就不需要检测数字是否被fix过两次，不过个人觉得还是前面那种解法更好一些，参见代码如下：



解法二：

```
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        set<vector<int>> res;
        sort(nums.begin(), nums.end());
        if (nums.empty() || nums.back() < 0 || nums.front() > 0) return {};
        for (int k = 0; k < nums.size(); ++k) {
            if (nums[k] > 0) break;
            int target = 0 - nums[k];
            int i = k + 1, j = nums.size() - 1;
            while (i < j) {
                if (nums[i] + nums[j] == target) {
                    res.insert({nums[k], nums[i], nums[j]});
                    while (i < j && nums[i] == nums[i + 1]) ++i;
                    while (i < j && nums[j] == nums[j - 1]) --j;
                    ++i; --j;
                } else if (nums[i] + nums[j] < target) ++i;
                else --j;
            }
        }
        return vector<vector<int>>(res.begin(), res.end());
    }
};
```



类似题目：

[Two Sum](http://www.cnblogs.com/grandyang/p/4130379.html)

[3Sum Smaller](http://www.cnblogs.com/grandyang/p/5235086.html)

[3Sum Closest](http://www.cnblogs.com/grandyang/p/4510984.html)

[4Sum](http://www.cnblogs.com/grandyang/p/4515925.html)



参考资料：

[https://leetcode.com/problems/3sum/](https://leetcode.com/problems/3sum/)

[https://leetcode.com/problems/3sum/discuss/7380/Concise-O(N2)-Java-solution](https://leetcode.com/problems/3sum/discuss/7380/Concise-O(N2)-Java-solution)

[https://leetcode.com/problems/3sum/discuss/7373/Share-my-simple-java-solution](https://leetcode.com/problems/3sum/discuss/7373/Share-my-simple-java-solution)

[http://www.lifeincode.net/programming/leetcode-two-sum-3-sum-3-sum-closest-and-4-sum-java/](http://www.lifeincode.net/programming/leetcode-two-sum-3-sum-3-sum-closest-and-4-sum-java/)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












