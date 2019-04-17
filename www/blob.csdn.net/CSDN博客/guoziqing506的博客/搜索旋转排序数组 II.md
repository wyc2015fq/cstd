# 搜索旋转排序数组 II - guoziqing506的博客 - CSDN博客





2016年04月14日 09:35:32[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：1005
所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)









题目描述：跟进“搜索旋转排序数组”，假如有重复元素又将如何？是否会影响运行时间复杂度？如何影响？为何会影响？写出一个函数判断给定的目标值是否出现在数组中。

样例：给出[3,4,4,5,7,0,1,2]和target=4，返回 true


这是上一道搜索旋转排序数组的升级版，详见：[点击打开链接](http://blog.csdn.net/guoziqing506/article/details/51139999)，只不过考虑了有重复元素的情况。

还记得我们之前做过一道求旋转排序数组中最小值的问题吧，先回顾一下，寻找一个存在重复元素的旋转排序数组最小值是怎么做的。我这里只把这一道题的代码抄在下面：



```python
class Solution:
    # @param num: a rotated sorted array
    # @return: the minimum number in the array
    def findMin(self, num):
        left, right = 0, len(num) - 1
        while left < right and num[left] >= num[right]:
            mid = (left + right) // 2
            # mid指在second中
            if num[left] < num[mid]:
                left = mid + 1
            # mid指在first中
            elif num[left] > num[mid]:
                right = mid
            # num[left] = num[mid]时的情况，非常特殊
            else:
                left = left + 1
        return num[left]
        # write your code here
```
至于具体每一步怎么思考的，详见：[点击打开链接](http://blog.csdn.net/guoziqing506/article/details/51073496)，不再重复了。那么再看这道题，其实就是“寻找旋转排序数组最小值”和“搜索旋转排序数组”的结合。如果你不会这两道题，那么请先学习我刚才给的两个链接，再看这道题。
因为有重复元素的存在，所以不能通过"A[left] <= A[mid]"这样的形式来判断mid左侧一直到left是排好序的，还是右侧一直到right是排好序的，但是我们发现:

1. 如果A[left] < A[mid]，则肯定mid左侧一直到left是排好序的；

2. 同理，如果A[left] > A[mid]，则肯定mid右侧一直到right是排好序的；

3. 对于A[left] = A[mid]的情况不好判断，则令left + 1，再看看。

这里，再解释一下，为啥A[left] = A[mid]的情况不好判断，例如下面这两个旋转排序数组：

1. [3,3,3,3,1,2,3]，此时A[mid] = A[left] = 3，左侧[3,3,3,3]是排好序的

2. [3,1,2,3,3,3,3]，此时A[mid] = A[left] = 3，右侧[3,3,3,3]是排好序的

所以，这里有个比较特殊的办法，就是令left + 1，上面代码中的第16行也是这么处理的。

好了，终于可以给出本节的代码了：



```python
class Solution:
    """
    @param A : an integer ratated sorted array and duplicates are allowed
    @param target : an integer to be searched
    @return : a boolean
    """
    def search(self, A, target):
        left, right = 0, len(A) - 1
        while left <= right:
            mid = (left + right) // 2
            if A[mid] == target:
                return True
            if A[mid] > A[left]:
                if target >= A[left] and target < A[mid]:
                    right = mid - 1
                else:
                    left = mid + 1
            elif A[mid] < A[left]:
                if target > A[mid] and target <= A[right]:
                    left = mid + 1
                else:
                    right = mid - 1
            else:
                left = left + 1
        return False
        # write your code here
```












