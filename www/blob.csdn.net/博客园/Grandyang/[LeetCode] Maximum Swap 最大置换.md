# [LeetCode] Maximum Swap 最大置换 - Grandyang - 博客园







# [[LeetCode] Maximum Swap 最大置换](https://www.cnblogs.com/grandyang/p/7583875.html)







Given a non-negative integer, you could swap two digits at most once to get the maximum valued number. Return the maximum valued number you could get.

Example 1:
Input: 2736
Output: 7236
Explanation: Swap the number 2 and the number 7.



Example 2:
Input: 9973
Output: 9973
Explanation: No swap.



Note:
- The given number is in the range [0, 108]



这道题给了我们一个数字，我们有一次机会可以置换该数字中的任意两位，让我们返回置换后的最大值，当然如果当前数字就是最大值，我们也可以选择不置换，直接返回原数。那么最简单粗暴的方法当然就是将所有可能的置换都进行一遍，然后更新结果res，取其中较大的数字，这样一定会得到置换后的最大数字，这里使用了整型数和字符串之间的相互转换，参见代码如下：



解法一：

```
class Solution {
public:
    int maximumSwap(int num) {
        string str = to_string(num);
        int res = num, n = str.size();
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                swap(str[i], str[j]);
                res = max(res, stoi(str));
                swap(str[i], str[j]);
            }
        }
        return res;
    }
};
```



下面这种解法是一种更优解，思路是这样的，由于我们希望置换后的数字最大，那么肯定最好的高位上的小数字和低位上的大数字进行置换，比如题目汇总的例子1。而如果高位上的都是大数字，像例子2那样，很有可能就不需要置换。所以我们需要找到每个数字右边的最大数字(包括其自身)，这样我们再从高位像低位遍历，如果某一位上的数字小于其右边的最大数字，说明需要调换，由于最大数字可能不止出现一次，我们希望能跟较低位的数字置换，这样置换后的数字最大，所以我们就从低位向高位遍历来找那个最大的数字，找到后进行调换即可。比如对于1993这个数：

1 9 9 3

9 9 9 3  (back数组)

**9** 9 **1** 3

我们建立好back数组后，从头遍历原数字，发现1比9小，于是从末尾往前找9，找到后一置换，就得到了9913。



解法二：

```
class Solution {
public:
    int maximumSwap(int num) {
        string res = to_string(num), back = res;
        for (int i = back.size() - 2; i >= 0; --i) {
            back[i] = max(back[i], back[i + 1]);
        }
        for (int i = 0; i < res.size(); ++i) {
            if (res[i] == back[i]) continue;
            for (int j = res.size() - 1; j > i; --j) {
                if (res[j] == back[i]) {
                    swap(res[i], res[j]);
                    return stoi(res);
                }
            }
        }
        return stoi(res);
    }
};
```



下面这种解法建了十个桶，分别代表数字0到9，每个桶存该数字出现的最后一个位置，也就是低位。这样我们从开头开始遍历数字上的每位上的数字，然后从大桶开始遍历，如果该大桶的数字对应的位置大于当前数字的位置，说明低位的数字要大于当前高位上的数字，那么直接交换这两个数字返回即可，其实核心思路跟上面的解法蛮像的，参见代码如下：



解法三：

```
class Solution {
public:
    int maximumSwap(int num) {
        string str = to_string(num);
        vector<int> buckets(10, 0);
        for (int i = 0; i < str.size(); ++i) {
            buckets[str[i] - '0'] = i;
        }
        for (int i = 0; i < str.size(); ++i) {
            for (int k = 9; k > str[i] - '0'; --k) {
                if (buckets[k] <= i) continue;
                swap(str[i], str[buckets[k]]);
                return stoi(str);
            }
        }
        return num;
    }
};
```



类似题目：

[Create Maximum Number](http://www.cnblogs.com/grandyang/p/5136749.html)



参考资料：

[https://discuss.leetcode.com/topic/102052/simple-c-using-std-string-and-std-stoi](https://discuss.leetcode.com/topic/102052/simple-c-using-std-string-and-std-stoi)

[https://discuss.leetcode.com/topic/102350/c-3ms-o-n-time-o-n-space-dp-solution](https://discuss.leetcode.com/topic/102350/c-3ms-o-n-time-o-n-space-dp-solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












