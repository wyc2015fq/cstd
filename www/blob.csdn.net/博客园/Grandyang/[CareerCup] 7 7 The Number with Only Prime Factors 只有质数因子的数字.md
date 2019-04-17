# [CareerCup] 7.7 The Number with Only Prime Factors 只有质数因子的数字 - Grandyang - 博客园







# [[CareerCup] 7.7 The Number with Only Prime Factors 只有质数因子的数字](https://www.cnblogs.com/grandyang/p/4780868.html)







7.7 Design an algorithm to find the kth number such that the only prime factors are 3,5, and 7.



这道题跟之前LeetCode的那道[Ugly Number II 丑陋数之二](http://www.cnblogs.com/grandyang/p/4743837.html)基本没有啥区别，具体讲解可参见那篇，代码如下：



```
class Solution {
public:
    int getKthMagicNumber(int k) {
        vector<int> res(1, 1);
        int i3 = 0, i5 = 0, i7 = 0;
        while (res.size() < k) {
            int m3 = res[i3] * 3, m5 = res[i5] * 5, m7 = res[i7] * 7;
            int mn = min(m3, min(m5, m7));
            if (mn == m3) ++i3;
            if (mn == m5) ++i5;
            if (mn == m7) ++i7;
            res.push_back(mn);
        }
        return res.back();
    }
};
```














