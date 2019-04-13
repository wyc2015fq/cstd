
# 204. Count Primes - OraYang的博客 - CSDN博客

2017年07月30日 15:40:41[OraYang](https://me.csdn.net/u010665216)阅读数：180标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Description:
Count the number of prime numbers less than a non-negative number,n.
思路：本题主要采用埃拉托斯特尼筛法（详情请戳[sieve of Eratosthenes](https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes)）
代码：

```python
class Solution {
public:
    int countPrimes(int n) {
        vector<bool> num(n - 1, true);
        num[0] = false;
        int res = 0, limit = sqrt(n);
        for (int i = 2; i <= limit; ++i) {
            if (num[i - 1]) {
                for (int j = i * i; j < n; j += i) {
                    num[j - 1] = false;
                }
            }
        }
        for (int j = 0; j < n - 1; ++j) {
            if (num[j]) ++res;
        }
        return res;
    }
};
```



