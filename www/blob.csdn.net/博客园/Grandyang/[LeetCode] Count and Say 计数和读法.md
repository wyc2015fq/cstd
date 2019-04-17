# [LeetCode] Count and Say 计数和读法 - Grandyang - 博客园







# [[LeetCode] Count and Say 计数和读法](https://www.cnblogs.com/grandyang/p/4086299.html)







The count-and-say sequence is the sequence of integers beginning as follows:
`1, 11, 21, 1211, 111221, ...`

`1` is read off as `"one 1"` or `11`.
`11` is read off as `"two 1s"` or `21`.
`21` is read off as `"one 2`, then `one 1"` or `1211`.

Given an integer *n*, generate the *n*th sequence.

Note: The sequence of integers will be represented as a string.



这道计数和读法问题还是第一次遇到，看似挺复杂，其实仔细一看，算法很简单，就是对于前一个数，找出相同元素的个数，把个数和该元素存到新的string里。代码如下：



```
class Solution {
public:
    string countAndSay(int n) {
        if (n <= 0) return "";
        string res = "1";
        while (--n) {
            string cur = "";
            for (int i = 0; i < res.size(); ++i) {
                int cnt = 1;
                while (i + 1 < res.size() && res[i] == res[i + 1]) {
                    ++cnt;
                    ++i;
                }
                cur += to_string(cnt) + res[i];
            }
            res = cur;
        }
        return res;
    }
};
```



博主出于好奇打印出了前12个数字，发现一个很有意思的现象，不管打印到后面多少位，出现的数字只是由1,2和3组成，网上也有人发现了并分析了原因 ([http://www.cnblogs.com/TenosDoIt/p/3776356.html](http://www.cnblogs.com/TenosDoIt/p/3776356.html))，前十二个数字如下：



```
1
1 1
2 1
1 2 1 1
1 1 1 2 2 1
3 1 2 2 1 1
1 3 1 1 2 2 2 1
1 1 1 3 2 1 3 2 1 1
3 1 1 3 1 2 1 1 1 3 1 2 2 1
1 3 2 1 1 3 1 1 1 2 3 1 1 3 1 1 2 2 1 1
1 1 1 3 1 2 2 1 1 3 3 1 1 2 1 3 2 1 1 3 2 1 2 2 2 1
3 1 1 3 1 1 2 2 2 1 2 3 2 1 1 2 1 1 1 3 1 2 2 1 1 3 1 2 1 1 3 2 1 1
```



参考资料：

[https://discuss.leetcode.com/topic/20195/c-solution-easy-understand](https://discuss.leetcode.com/topic/20195/c-solution-easy-understand)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












