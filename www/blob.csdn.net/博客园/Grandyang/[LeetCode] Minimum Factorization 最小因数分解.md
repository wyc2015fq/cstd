# [LeetCode] Minimum Factorization 最小因数分解 - Grandyang - 博客园







# [[LeetCode] Minimum Factorization 最小因数分解](https://www.cnblogs.com/grandyang/p/7076026.html)








Given a positive integer `a`, find the smallest positive integer `b` whose multiplication of each digit equals to `a`.

If there is no answer or the answer is not fit in 32-bit signed integer, then return 0.

Example 1
Input:
48 
Output:
68


Example 2
Input:
15
Output:
35


这道题给了我们一个数字，让我们进行因数分解，让我们找出因数组成的最小的数字。从题目中的例子可以看出，分解出的因数一定是个位数字，即范围是[2, 9]。那我们就可以从大到小开始找因数，首先查找9是否是因数，是要能整除a，就是其因数，如果是的话，就加入到结果res的开头，a自除以9，我们用while循环查找9，直到取出所有的9，然后取8，7，6...以此类推，如果a能成功的被分解的话，最后a的值应该为1，如果a值大于1，说明无法被分解，返回true。最后还要看我们结果res字符转为整型是否越界，越界的话还是返回0，参见代码如下：



解法一：

```
class Solution {
public:
    int smallestFactorization(int a) {
        if (a == 1) return 1;
        string res = "";
        for (int k = 9; k >= 2; --k) {
            while (a % k == 0) {
                res = to_string(k) + res;
                a /= k;
            }
        }
        if (a > 1) return 0;
        long long num = stoll(res);
        return num > INT_MAX ? 0 : num;
    }
};
```



下面这种方法跟上面解法思路很像，只是结果res没有用字符串，而是直接用的长整型，我们每次在更新完res的结果后，判断一次是否越整型的界，越了就直接返回0，其他部分和上面没有什么区别，参见代码如下：



解法二：

```
class Solution {
public:
    int smallestFactorization(int a) {
        if (a < 10) return a;
        long long res = 0, cnt = 1;
        for (int i = 9; i >= 2; --i) {
            while (a % i == 0) {
                res += cnt * i;
                if (res > INT_MAX) return 0;
                a /= i;
                cnt *= 10;
            }
        }
        return (a == 1) ? res : 0;
    }
};
```



参考资料： 

[https://discuss.leetcode.com/topic/92920/concise-c-solution-10-lines-3ms](https://discuss.leetcode.com/topic/92920/concise-c-solution-10-lines-3ms)

[https://discuss.leetcode.com/topic/92998/c-clean-code-7-line-3-solutions/2](https://discuss.leetcode.com/topic/92998/c-clean-code-7-line-3-solutions/2)




[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












