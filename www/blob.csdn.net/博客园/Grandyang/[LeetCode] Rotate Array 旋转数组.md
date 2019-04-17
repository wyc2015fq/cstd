# [LeetCode] Rotate Array 旋转数组 - Grandyang - 博客园







# [[LeetCode] Rotate Array 旋转数组](https://www.cnblogs.com/grandyang/p/4298711.html)







Given an array, rotate the array to the right by *k* steps, where *k* is non-negative.

Example 1:
Input: `[1,2,3,4,5,6,7]` and *k* = 3
Output: `[5,6,7,1,2,3,4]`
Explanation:
rotate 1 steps to the right: `[7,1,2,3,4,5,6]`
rotate 2 steps to the right: 
```
[6,7,1,2,3,4,5]
```
rotate 3 steps to the right: `[5,6,7,1,2,3,4]`
Example 2:
Input: `[-1,-100,3,99]` and *k* = 2
Output: [3,99,-1,-100]
Explanation: 
rotate 1 steps to the right: [99,-1,-100,3]
rotate 2 steps to the right: [3,99,-1,-100]

Note:
- Try to come up as many solutions as you can, there are at least 3 different ways to solve this problem.
- Could you do it in-place with O(1) extra space?

Credits:
Special thanks to [@Freezen](https://oj.leetcode.com/discuss/user/Freezen) for adding this problem and creating all test cases.



新题抢先刷，这道题标为Easy，应该不是很难，我们先来看一种O(n)的空间复杂度的方法，我们复制一个和nums一样的数组，然后利用映射关系i -> (i+k)%n来交换数字。代码如下：



解法一：

```
class Solution {
public:
    void rotate(vector<int>& nums, int k) {
        vector<int> t = nums;
        for (int i = 0; i < nums.size(); ++i) {
            nums[(i + k) % nums.size()] = t[i];
        }
    }
};
```



由于提示中要求我们空间复杂度为O(1)，所以我们不能用辅助数组，上面的思想还是可以使用的，但是写法就复杂的多，而且需要用到很多的辅助变量，我们还是要将 nums[idx] 上的数字移动到 nums[(idx+k) % n] 上去，为了防止数据覆盖丢失，我们需要用额外的变量来保存，这里用了pre和cur，其中cur初始化为了数组的第一个数字，然后当然需要变量 idx 标明当前在交换的位置，还需要一个变量 start，这个是为了防止陷入死循环的，初始化为0，一旦当 idx 变到了 strat 的位置，则 start 自增1，再赋值给 idx，这样 idx 的位置也改变了，可以继续进行交换了。举个例子，假如 [1, 2, 3, 4], K=2 的话，那么 idx=0，下一次变为 idx = (idx+k) % n = 2，再下一次又变成了 idx = (idx+k) % n = 0，此时明显 1 和 3 的位置还没有处理过，所以当我们发现 idx 和 start 相等，则二者均自增1，那么此时 idx=1，下一次变为 idx = (idx+k) % n = 3，就可以交换完所有的数字了。

因为长度为n的数组只需要更新n次，所以我们用一个for循环来处理n次。首先 pre 更新为 cur，然后计算新的 idx 的位置，然后将 nums[idx] 上的值先存到 cur 上，然后把 pre 赋值给 nums[idx]，这相当于把上一轮的 nums[idx] 赋给了新的一轮，完成了数字的交换，然后 if 语句判断是否会变到处理过的数字，参见上面一段的解释，我们用题目中的例子1来展示下面这种算法的nums的变化过程：

1 2 3 4 5 6 7 
1 2 3 1 5 6 7
1 2 3 1 5 6 4
1 2 7 1 5 6 4
1 2 7 1 5 3 4
1 6 7 1 5 3 4
1 6 7 1 2 3 4
5 6 7 1 2 3 4



解法二：

```
class Solution {
public:
    void rotate(vector<int>& nums, int k) {
        if (nums.empty() || (k %= nums.size()) == 0) return;
        int start = 0, idx = 0, pre = 0, cur = nums[0], n = nums.size();
        for (int i = 0; i < n; ++i) {
            pre = cur;
            idx = (idx + k) % n;
            cur = nums[idx];
            nums[idx] = pre;
            if (idx == start) {
                idx = ++start;
                cur = nums[idx];
            }
        }
    }
};
```



根据热心网友[waruzhi](http://www.cnblogs.com/waruzhi/)的留言，这道题其实还有种类似翻转字符的方法，思路是先把前n-k个数字翻转一下，再把后k个数字翻转一下，最后再把整个数组翻转一下：

1 2 3 4 5 6 7 
4 3 2 1 5 6 7 
4 3 2 1 7 6 5
5 6 7 1 2 3 4



解法三：

```
class Solution {
public:
    void rotate(vector<int>& nums, int k) {
        if (nums.empty() || (k %= nums.size()) == 0) return;
        int n = nums.size();
        reverse(nums.begin(), nums.begin() + n - k);
        reverse(nums.begin() + n - k, nums.end());
        reverse(nums.begin(), nums.end());
    }
};
```



由于旋转数组的操作也可以看做从数组的末尾取k个数组放入数组的开头，所以我们用STL的push_back和erase可以很容易的实现这些操作。



解法四：

```
class Solution {
public:
    void rotate(vector<int>& nums, int k) {
        if (nums.empty() || (k %= nums.size()) == 0) return;
        int n = nums.size();
        for (int i = 0; i < n - k; ++i) {
            nums.push_back(nums[0]);
            nums.erase(nums.begin());
        }
    }
};
```



下面这种方法其实还蛮独特的，通过不停的交换某两个数字的位置来实现旋转，根据[网上这个帖子](https://discuss.leetcode.com/topic/9406/3-lines-of-c-in-one-pass-using-swap)的思路改写而来，数组改变过程如下：

1 2 3 4 5 6 7 
5 2 3 4 1 6 7 
5 6 3 4 1 2 7
5 6 7 4 1 2 3
5 6 7 1 4 2 3
5 6 7 1 2 4 3
5 6 7 1 2 3 4



解法五：

```
class Solution {
public:
    void rotate(vector<int>& nums, int k) {
        if (nums.empty()) return;
        int n = nums.size(), start = 0;   
        while (n && (k %= n)) {
            for (int i = 0; i < k; ++i) {
                swap(nums[i + start], nums[n - k + i + start]);
            }
            n -= k;
            start += k;
        }
    }
};
```



参考资料：

[https://leetcode.com/problems/rotate-array/](https://leetcode.com/problems/rotate-array/)

[https://leetcode.com/problems/rotate-array/discuss/54250/Easy-to-read-Java-solution](https://leetcode.com/problems/rotate-array/discuss/54250/Easy-to-read-Java-solution)

[https://leetcode.com/problems/rotate-array/discuss/54277/Summary-of-C%2B%2B-solutions](https://leetcode.com/problems/rotate-array/discuss/54277/Summary-of-C%2B%2B-solutions)

[https://leetcode.com/problems/rotate-array/discuss/54438/My-c%2B%2B-solution-o(n)time-andand-o(1)space](https://leetcode.com/problems/rotate-array/discuss/54438/My-c%2B%2B-solution-o(n)time-andand-o(1)space)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












