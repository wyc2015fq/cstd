# [LeetCode] Smallest Good Base 最小的好基数 - Grandyang - 博客园







# [[LeetCode] Smallest Good Base 最小的好基数](https://www.cnblogs.com/grandyang/p/6620351.html)







For an integer n, we call k>=2 a ***good base*** of n, if all digits of n base k are 1.

Now given a string representing n, you should return the smallest good base of n in string format.

**Example 1:**

**Input:** "13"
**Output:** "3"
**Explanation:** 13 base 3 is 111.

**Example 2:**

**Input:** "4681"
**Output:** "8"
**Explanation:** 4681 base 8 is 11111.

**Example 3:**

**Input:** "1000000000000000000"
**Output:** "999999999999999999"
**Explanation:** 1000000000000000000 base 999999999999999999 is 11.

**Note:**

- The range of n is [3, 10^18].
- The string representing n is always valid and will not have leading zeros.




这道题让我们求最小的好基数，定义了一个大于等于2的基数k，如果可以把数字n转化为各位都是1的数，那么就称这个基数k是好基数。通过看题目中的三个例子，应该大致可以理解题意了吧。如果我们用k表示基数，m表示转为全1数字的位数，那么数字n就可以拆分为：

n = 1 + k + k^2 + k^3 + ... + k^(m-1)

这是一个等比数列，中学数学的内容吧，利用求和公式可以表示为 n = (k^m - 1) / (k - 1)。我们的目标是求最小的k，那么仔细观察这个式子，在n恒定的情况，k越小则m却大，那么就是说上面的等式越长越好。下面我们来分析m的取值范围，题目中给了n的范围，是[3, 10^18]。那么由于k至少为2，n至少为3，那么肯定至少有两项，则m>=2。那么m的上限该如何求？其实也不难，想要m最大，那么k就要最小，k最小是2，那么m最大只能为log2(n + 1)，数字n用二进制表示的时候可拆分出的项最多。但这道题要求变换后的数各位都是1，那么我们看题目中最后一个例子，可以发现，当k=n-1时，一定能变成11，所以实在找不到更小的情况下就返回n-1。

下面我们来确定k的范围，由于k至少为2，那么我们可以根据下面这个不等式来求k的上限：

n = 1 + k + k^2 + k^3 + ... + k^(m-1) > k^(m-1)

解出k < n^(1 / (m-1))，其实我们也可以可以通过n < k^m - 1 来求出k的准确的下限，但由于是二分查找法，下限直接使用2也没啥问题。分析到这里，那么解法应该已经跃然纸上了，我们遍历所有可能的m值，然后利用二分查找法来确定k的值，对每一个k值，我们通过联合m值算出总和sum，然后跟n来对比即可，参见代码如下：



```
class Solution {
public:
    string smallestGoodBase(string n) {
        long long num = stol(n);
        for (int i = log(num + 1) / log(2); i >= 2; --i) {
            long long left = 2, right = pow(num, 1.0 / (i - 1)) + 1;
            while (left < right) {
                long long mid = left + (right - left) / 2, sum = 0;
                for (int j = 0; j < i; ++j) {
                    sum = sum * mid + 1;
                }
                if (sum == num) return to_string(mid);
                else if (sum < num) left = mid + 1;
                else right = mid;
            }
        }
        return to_string(num - 1);
    }
};
```



参考资料：

[https://discuss.leetcode.com/topic/76425/concise-c-binary-search-solution](https://discuss.leetcode.com/topic/76425/concise-c-binary-search-solution)

[https://discuss.leetcode.com/topic/78148/java-o-logn-2-binary-search-solution](https://discuss.leetcode.com/topic/78148/java-o-logn-2-binary-search-solution)

[https://discuss.leetcode.com/topic/76347/3ms-ac-c-long-long-int-binary-search](https://discuss.leetcode.com/topic/76347/3ms-ac-c-long-long-int-binary-search)




[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












