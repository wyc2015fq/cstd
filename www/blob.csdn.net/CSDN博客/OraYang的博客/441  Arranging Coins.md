
# 441. Arranging Coins - OraYang的博客 - CSDN博客

2017年08月16日 08:33:45[OraYang](https://me.csdn.net/u010665216)阅读数：181标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
You have a total ofncoins that you want to form in a staircase shape, where everyk-th row must have exactlykcoins.
Givenn, find the total number offullstaircase rows that can be formed.
nis a non-negative integer and fits within the range of a 32-bit signed integer.
Example 1:
n = 5
The coins can form the following rows:
¤
¤ ¤
¤ ¤
Because the 3rd row is incomplete, we return 2.
Example 2:
n = 8
The coins can form the following rows:
¤
¤ ¤
¤ ¤ ¤
¤ ¤
Because the 4th row is incomplete, we return 3.思路：本题塔型数据求和问题，思路比较简单，单独是先一个求和函数,利用递归实现，为了防止数据溢出
代码：

```python
class Solution {
public:
    int arrangeCoins(int n) {
        long num = n;
        for(int i = sqrt(2*num);i>=0;i--)
        {
            if(sum(i)<=num)
                return i;
        }
        return 0;
    }
private:
    int sum(int n){
        if(n==0)
            return 0;
        return sum(n-1)+n;
        
    }
};
```


