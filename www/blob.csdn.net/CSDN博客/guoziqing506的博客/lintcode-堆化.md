# lintcode - 堆化 - guoziqing506的博客 - CSDN博客





2016年09月17日 18:39:53[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：659
所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)









题目描述：给出一个整数数组，堆化操作就是把它变成一个最小堆数组。对于堆数组A，A[0]是堆的根，并对于每个A[i]，A [i * 2 + 1]是A[i]的左儿子并且A[i * 2 + 2]是A[i]的右儿子。

样例：给出 [3,2,1,4,5]，返回[1,2,3,4,5] 或者任何一个合法的堆数组




有关堆数组的实现，我在另一篇博文中有详细的论述：[堆的实现与优先队列](http://blog.csdn.net/guoziqing506/article/details/52372469)。所以，讲解此处略，只是给出答案代码：






```python
class Solution:
    # @param A: Given an integer array
    # @return: void
    def heapify(self, A):
        n = len(A)
        for i in range(n):
            cur = i
            par = (i - 1) // 2
            while par >= 0 and A[par] > A[cur]:
                A[par], A[cur] = A[cur], A[par]
                cur = par
                par = (cur - 1) // 2
```

其实，lintcode的题目的描述中没有说清楚如下要求：对数组A原地更新，并且函数的返回值为None







