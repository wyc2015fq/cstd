# lintcode - 平面列表 - guoziqing506的博客 - CSDN博客





2016年09月05日 16:45:53[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：1664
所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)









题目描述：给定一个列表，该列表中的每个要素要么是个列表，要么是整数。将其变成一个只包含整数的简单列表。

样例：给定 [1,2,[1,2]]，返回 [1,2,1,2]。给定 [4,[3,[2,[1]]]]，返回 [4,3,2,1]。




用递归的思路很容易解决，之前我已经说过太多类似的问题，不再赘述。需要注意的一点是递归“触底”的条件是当这个给出的列表本身是整数。直接给出代码：



```python
class Solution(object):

    # @param nestedList a list, each element in the list 
    # can be a list or integer, for example [1,2,[1,2]]
    # @return {int[]} a list of integer
    def flatten(self, nestedList):

        if isinstance(nestedList, int):
            return [nestedList]

        result = []

        for i in nestedList:
            result.extend(self.flatten(i))
            
        return result
        # Write your code here
```

若是用非递归解决这个问题会稍微复杂一点。但是好在有队列这种数据结构，可以帮助我们解决问题。


思路是这样，将原先的列表，也就是上面代码中的参数nestedList看做一个队列，当然，nestedList的头就是队列的头，尾就是队列的尾。然后依次从队列中删除元素，若删除出来的元素为整数，则直接加入结果列表result；若不是整数，做法则有点特殊：

将这个元素（肯定是个列表了）中所有子元素依次插到队列的前头，当然，还要保持顺序不变。例如，现在删除出来的元素是[1, [2, 3]] 则将1, [2, 3]这两个元素按顺序插入队列的前端，这样做，相当于是去除了最外层的列表。然后接着删除队列首，持续上面的过程。




这样，就相当于是逐个处理，最终将所有元素的列表全部去除，并且还保持了元素顺序。

代码如下：



```python
class Solution(object):

    # @param nestedList a list, each element in the list
    # can be a list or integer, for example [1,2,[1,2]]
    # @return {int[]} a list of integer
    def flatten(self, nestedList):

        if isinstance(nestedList, int):
            return [nestedList]

        result = []

        while len(nestedList) != 0:
            temp = nestedList.pop(0)
            if isinstance(temp, int):
                result.append(temp)
            else:
                for i in temp[::-1]:
                    nestedList.insert(0, i)
        return result
        # Write your code here
```










