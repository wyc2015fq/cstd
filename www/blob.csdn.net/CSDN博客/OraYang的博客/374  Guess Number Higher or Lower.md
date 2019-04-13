
# 374. Guess Number Higher or Lower - OraYang的博客 - CSDN博客

2017年08月08日 09:55:33[OraYang](https://me.csdn.net/u010665216)阅读数：104标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
We are playing the Guess Game. The game is as follows:
I pick a number from1ton. You have to guess which number I picked.
Every time you guess wrong, I'll tell you whether the number is higher or lower.
You call a pre-defined API`guess(int num)`which returns 3 possible results (`-1`,`1`,
 or`0`):
-1 : My number is lower
 1 : My number is higher
 0 : Congrats! You got it!Example:
n = 10, I pick 6.
Return 6.思路：本题很简单，用二分查找法
代码：

```python
// Forward declaration of guess API.
// @param num, your guess
// @return -1 if my number is lower, 1 if my number is higher, otherwise return 0
int guess(int num);
class Solution {
public:
    int guessNumber(int n) {
        //simple binary search : low = 1; high = n
        int low = 1;
        while(low <= n){
            int mid = low + (n-low) / 2;
            int res = guess(mid);
            if(res == 0)
                return mid;
            else if(res == -1)
                n = mid - 1;
            else
                low = mid + 1;
        }
        return -1;
    }
};
```


