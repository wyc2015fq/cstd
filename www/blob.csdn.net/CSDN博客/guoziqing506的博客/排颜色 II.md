# 排颜色 II - guoziqing506的博客 - CSDN博客





2016年04月23日 21:47:38[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：665
所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)









题目描述：给定一个有n个对象（包括k种不同的颜色，并按照1到k进行编号）的数组，将对象进行分类使相同颜色的对象相邻，并按照1,2，...k的顺序进行排序。

样例：给出colors=[3, 2, 2, 1, 4]，k=4, 你的代码应该在原地操作使得数组变成[1, 2, 2, 3, 4]


与上一道颜色排列的问题（详见：[点击打开链接](http://blog.csdn.net/guoziqing506/article/details/51227695)）不同之处在于现在有多个元素，所以直接进行快速排序即可，是有重复元素的快速排序。（这里，我实在觉得此处给的参数k没什么大用，如果谁有比我给的更好的方法，用到k了，请不吝赐教）

快速排序的方法之前详细讨论过了（[点击打开链接](http://blog.csdn.net/guoziqing506/article/details/50952002)），不再赘述，直接给出代码：



```python
class Solution:
    """
    @param colors: A list of integer
    @param k: An integer
    @return: nothing
    """
    def sortColors2(self, colors, k):
        if k == 1 or len(colors) == 0:
            return colors
        end = len(colors) - 1
        begin = 0
        self.helper(colors, begin, end)
        return colors


    def helper(self, colors, begin, end):
        if begin >= end:
            return
        pivot = colors[begin]
        index = begin
        for cur in range(begin + 1, end + 1):
            if colors[cur] <= pivot:
                index += 1
                colors[cur], colors[index] = colors[index], colors[cur]
        colors[begin], colors[index] = colors[index], colors[begin]
        self.helper(colors, begin, index - 1)
        self.helper(colors, index + 1, end)
        # write your code here
```
其中，helper函数是快排，用begin和end代表首尾位置，这种形参设置的方法之前已经说过很多




