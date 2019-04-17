# lintcode - 最接近的三数之和 - guoziqing506的博客 - CSDN博客





2016年09月06日 14:39:26[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：1847
所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)









题目描述：给一个包含 n 个整数的数组 S, 找到和与给定整数 target 最接近的三元组，返回这三个数的和。

样例：例如 S = [-1, 2, 1, -4] and target = 1. 和最接近 1 的三元组是 -1 + 2 + 1 = 2.




和之前“三数之和”的套路是一致的（详见：[点击打开链接](http://blog.csdn.net/guoziqing506/article/details/51779230)）。简单来说，就是先控制一个位置固定，然后对其余两个位置“左右夹逼”，再将这个固定的位置移动，然后“左右夹逼”。。。以此类推即可。




所以，本题的思路是这样的：先设定三个指针left, mid, right。初始时，left为数组最左端的元素，且固定不变，mid = left + 1, right为数组最右端的元素。然后对 left, mid, right三个指针所指向的值求和，记为sum_val

设置一个值min_gap记录当前的最小差，令abs(sum_val - target)的值与min_gap比较，若比min_gap小，则说明获得了更接近的三数之和，更新min_gap；若比min_gap大，则有两种可能性：

1. sum_val - target > 0，说明现在的三数之和大了，则令right指针左移

2. sum_val - target < 0，说明现在的三数之和小了，则令mid指针右移

此循环终止的条件是mid >= right




以上工作完成后，将 left 右移，令mid = left + 1继续上面的工作。




代码如下：



```python
class Solution:
    """
    @param numbers: Give an array numbers of n integer
    @param target : An integer
    @return : return the sum of the three integers, the sum closest target.
    """
    def threeSumClosest(self, numbers, target):

        numbers.sort()

        result = sum(numbers[:3])
        min_gap = abs(result - target)
        n = len(numbers)

        for left in range(n):

            right = n - 1
            mid = left + 1

            while mid < right:
                sum_val = numbers[left] + numbers[right] + numbers[mid]
                temp = abs(sum_val - target)
                if temp < min_gap:
                    min_gap = temp
                    result = sum_val
                elif sum_val > target:
                    right -= 1
                else:
                    mid += 1
        return result
        # write your code here
```







