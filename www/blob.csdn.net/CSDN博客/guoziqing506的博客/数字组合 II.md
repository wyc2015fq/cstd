# 数字组合 II - guoziqing506的博客 - CSDN博客





2016年07月18日 13:45:25[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：901
所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)









题目描述：给出一组候选数字(C)和目标数字(T),找出C中所有的组合，使组合中数字的和为T。C中每个数字在每个组合中只能使用一次。

样例：给出一个例子，候选数字集合为[10,1,6,7,2,1,5] 和目标数字 8 , 解集为：[[1,7],[1,2,5],[2,6],[1,1,6]]




跟上一题“数字组合”（详见：[点击打开链接](http://blog.csdn.net/guoziqing506/article/details/51918246)）其实是一样的，只不过此时不允许列表中的数字被重复利用了。那么我们稍微修改一下代码就行。我假设你已经完全搞懂了上一题的原理，所以本题我将省略讲解部分。




代码如下：



```python
class Solution:
    """
    @param candidates: Given the candidate numbers
    @param target: Given the target number
    @return: All the combinations that sum to target
    """
    def combinationSum2(self, candidates, target):
        path = []
        result = []
        begin = 0
        # 先排序，为了后面的去重做准备
        candidates.sort()
        self.helper(candidates, target, begin, path, result)
        return result

    def helper(self, candidates, target, begin, path, result):
        # 符合条件，加入结果列表
        if sum(path) == target:
            result.append(path[:])
            return
        # 剪枝
        if sum(path) > target:
            return

        # 注意此时用while循环，而应避免使用for循环
        n = len(candidates)
        while begin < n:
            path.append(candidates[begin])
            self.helper(candidates, target, begin + 1, path, result)
            path.pop()
            # 因为已经排序，所以可以通过如下的while循环找到下一个不同的元素作为新一轮的开始，防止重复
            while begin < n - 1 and candidates[begin] == candidates[begin + 1]:
                begin += 1
            begin += 1
        # write your code here
```




helper()函数中，用begin控制开始搜寻的位置，因为不能使用重复，所以，我们令每次循环结束后，begin += 1. 

需要注意的是，此时尽量应该使用while循环，而应避免使用for循环。比如下面这个例子：



```python
begin = 0
a = [1, 2, 3, 4, 5]
for i in a[begin:]:
	print(i)
	begin += 2
```
输出的结果还是是1， 2， 3， 4， 5，并没有随着begin的变化发生改变。所以如果把helper()函数中的第一个while循环写成for循环就很容易出现错误。我的建议是：对于处理数组中，对下标操作比较细的问题，统一使用while循环，而尽量避免使用for循环。






