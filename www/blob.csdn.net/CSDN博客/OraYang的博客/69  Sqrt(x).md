
# 69. Sqrt(x) - OraYang的博客 - CSDN博客

2017年07月15日 09:22:35[OraYang](https://me.csdn.net/u010665216)阅读数：223标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Implement`int sqrt(int x)`.
Compute and return the square root ofx.
思路：牛顿迭代法，公式：，最终公式：**x**i+1**=x**i**- (x**i2**- n) / (2x**i**) = x**i**- x**i**/ 2 + n / (2x**i**) = x**i**/ 2 + n / 2x**i**= (x**i**+ n/x**i**) / 2，**参考链接：https://en.wikipedia.org/wiki/Newton%27s_method
代码：

```python
class Solution {
public:
    int mySqrt(int x) {
        if(x==0)
            return 0;
        double res = 1;
        double last = 0;
        while(res!=last)
        {
            last = res;//每次迭代update last value
            res = (res + x/res)/2;
            
        }
        return int(res);
    }
};
```


