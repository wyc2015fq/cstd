# 第k大元素 - guoziqing506的博客 - CSDN博客





2016年04月27日 08:18:22[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：847
所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)









题目描述：在数组中找到第k大的元素

样例：给出数组 [9,3,2,4,8]，第三大的元素是 4；给出数组 [1,2,3,4,5]，第一大的元素是 5，第二大的元素是 4，第三大的元素是 3，以此类推

和上一道中位数的题（详见：[点击打开链接](http://blog.csdn.net/guoziqing506/article/details/51254794)）是一模一样的，需要注意的是，数组既然要按升序排列，那么第k大元素就是数组的倒数第k位，也就是索引为n - k的元素，还是按照快排的思想，通过一个分割函数找到每次枢轴应该排在的位置，比较这个位置和n - k的前后，如果恰好是n - k，直接返回；如果小于n
 - k，则说明需要在枢轴所在后面的数组中找寻；如果大于n - k，则说明需要在枢轴所在前面的数组中找寻。这个思路如果还有疑问，请移步我刚才给的链接。

那么代码几乎跟“中位数”的题是一样的，如下：



```python
class Solution:
    # @param k & A a integer and an array
    # @return ans a integer
    def kthLargestElement(self, k, A):
        n = len(A)
        if n == 0:
            return None
        begin, end = 0, n - 1
        index = self.partition(A, 0, n - 1)
        while index != n - k:
            if index > n - k:
                end = index - 1
                index = self.partition(A, begin, end)
            else:
                begin = index + 1
                index = self.partition(A, begin, end)
                
        return A[index]
        
            
    def partition(self, A, begin, end):
        if begin >= end:
            return begin
        pivot = A[begin]
        index = begin
        for i in range(begin + 1, end + 1):
            if A[i] <= pivot:
                index += 1
                A[i], A[index] = A[index], A[i]
        A[begin], A[index] = A[index], A[begin]
        return index
```
这两道题，包括前面排列颜色的问题，都是快排的应用，所以，有时候学习算法，确实需要理解它背后的原理，而不仅仅是会写出这一个问题的代码。




