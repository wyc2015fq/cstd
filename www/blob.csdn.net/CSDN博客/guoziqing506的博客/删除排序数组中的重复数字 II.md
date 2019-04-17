# 删除排序数组中的重复数字 II - guoziqing506的博客 - CSDN博客





2016年05月13日 17:12:53[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：551标签：[数组](https://so.csdn.net/so/search/s.do?q=数组&t=blog)
个人分类：[lintcode](https://blog.csdn.net/guoziqing506/article/category/6289140)

所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)









题目描述：跟进“删除重复数字”：如果可以允许出现两次重复将如何处理？

样例：给出数组A =[1,1,1,2,2,3]，你的函数应该返回长度5，此时A=[1,1,2,2,3]。

题目是上一道题“删除排序数组中的重复数字”（详见：[点击打开链接](http://blog.csdn.net/guoziqing506/article/details/51395796)）的延伸，现在要求可以保留出现两次的重复。

那也很简单，用一个整型变量count来统计出现的次数，大于2了，就删。基本思路和上一道题一模一样。

直接给代码：



```python
class Solution:
    """
    @param A: a list of integers
    @return an integer
    """
    def removeDuplicates(self, A):
        if len(A) <= 1:
            return len(A)
        temp = A[0]
        count, index = 1, 1
        while index != len(A):
            if A[index] == temp and count == 2:
                A.pop(index)
            elif A[index] == temp:
                count += 1
                index += 1
            else:
                temp = A[index]
                count = 1
                index += 1
        return index + 1
        # write your code here
```
还是那个需要注意的点：用index变量和while循环来完成对随时可删除数组的扫描，保证正确性。上一道题已经说过。




