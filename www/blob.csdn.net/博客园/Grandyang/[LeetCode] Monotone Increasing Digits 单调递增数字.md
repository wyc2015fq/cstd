# [LeetCode] Monotone Increasing Digits 单调递增数字 - Grandyang - 博客园







# [[LeetCode] Monotone Increasing Digits 单调递增数字](https://www.cnblogs.com/grandyang/p/8068326.html)








Given a non-negative integer `N`, find the largest number that is less than or equal to `N` with monotone increasing digits.

(Recall that an integer has *monotone increasing digits* if and only if each pair of adjacent digits `x` and `y` satisfy `x <= y`.)



Example 1:
Input: N = 10
Output: 9



Example 2:
Input: N = 1234
Output: 1234



Example 3:
Input: N = 332
Output: 299



Note: `N` is an integer in the range `[0, 10^9]`.




这道题给了我们一个非负数，让我们求一个数字小于等于给定数字，且该数字各位上的数字是单调递增的。那么我们就来分析题目中给的几个例子吧，首先如果是10的话，我们知道1大于0，所以不是单调自增的，那么返回的数就是9。第二个例子是1234，各位上已经满足单调自增的条件了，返回原数即可。第三个例子是332，我们发现最后一位2小于之前的3，那么此时我们将前面位减1，先变成322，再往前看，还是小于前面的3，那么我们再将前面位减1，就变成了222，此时222不是最大的单调递增数，我们可以将后面两位变成9，于是乎就有了299，小于给定的332，符合题意。如果给定的数字是232，那么就会得到229，我们可以发现规律，要找到从后往前遍历的最后一个值升高的位置，让前一位减1，并把当前位以及后面的所有位都变成9，就可以得到最大的单调递增数啦。

我们用j表示最后一个值升高的位置，具体来说应该是其前一位的值大，初始化为总位数n，然后从后往前遍历，因为每次要和前一位比较，为防止越界，应遍历到第二个数停止，如果当前位大于等于前一位，符合单调递增，直接跳过；否则就将前一位自减1，j赋值为当前位i，循环结束后，从j位到末尾的位数都改为9即可，参见代码如下：



```
class Solution {
public:
    int monotoneIncreasingDigits(int N) {
        string str = to_string(N);
        int n = str.size(), j = n;
        for (int i = n - 1; i > 0; --i) {
            if (str[i] >= str[i - 1]) continue;
            --str[i - 1];
            j = i;
        }        
        for (int i = j; i < n; ++i) {
            str[i] = '9';
        }
        return stoi(str);
    }
};
```



类似题目：

[Remove K Digits](http://www.cnblogs.com/grandyang/p/5883736.html)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












