# 寻找旋转排序数组中的最小值 II - guoziqing506的博客 - CSDN博客





2016年04月06日 11:42:23[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：579
个人分类：[lintcode](https://blog.csdn.net/guoziqing506/article/category/6289140)

所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)









题目描述：假设一个旋转排序的数组其起始位置是未知的（比如0 1 2 4 5 6 7 可能变成是4 5 6 7 0 1 2）。你需要找到其中最小的元素。数组中可能存在重复的元素。

样例：给出[4,4,5,6,7,0,1,2]  返回 0




好了，跟上一道题“寻找旋转排序数组中的最小值”（详见：[点击打开链接](http://blog.csdn.net/guoziqing506/article/details/51058549)）相比，其实求取的东西本质上没有发生变化，我们还是按照上次讲的逻辑（具体看链接）将数组分成[first, second]，旋转后变成[second, first]，不论数组里面怎么出现重复的数字，要找的还是first的第一个元素。因此，我们可以尝试修改之前的代码，看能否实现这里的“有重复数字”的要求。




先来看看之前的代码是怎么写的：



```python
class Solution:
    # @param num: a rotated sorted array
    # @return: the minimum number in the array
    def findMin(self, num):
        left, right = 0, len(num) - 1
        while left < right and num[left] > num[right]:
            mid = (left + right) // 2
            # mid指在second中
            if num[left] <= num[mid]:
                left = mid + 1
            # mid指在first中
            elif num[left] > num[mid]:
                right = mid
        return num[left]
        # write your code here
```






下面，我们看看，在逻辑上，当数组中出现重复的元素之后，这段代码还是否成立。left和right是两个指针，不用多说了。while循环的条件要求两个指针始终分别指向前后两段，不能指向同一元素，这在有重复元素时，也对，需要注意的是，因为有重复元素，所以，我们要num[left] >= num[right]而不是之前的“>”。




循环里面，先算mid，通过left和mid所指向元素的大小，判断mid所指的元素在first还是second，在没有重复元素的时候，num[left] <= num[mid]证明mid指向的元素在second里面，那么当有重复元素的时候呢？




我们发现，有问题！看看这个数组：[1, 3, 3, 3, 3, 3, 3]，分成first = [1, 3, 3, 3, 3]和second = [3, 3]，旋转后变成[3, 3, 1, 3, 3, 3, 3]，此时，循环的第一步中满足num[left]
 <= num[mid]，而mid指向的元素却在first中。所以，num[left] = num[mid]是一种非常特殊的情况，因为有重复元素的存在，导致num[left] = num[mid]时不一定是说left = mid（也有可能是他们指向了相等的值而已）那么，我们该怎么办呢，可以把这种情况单独提出来，处理时，“小心翼翼”地令left
 = left + 1，比如上边这个例子，left+1后再求mid就满足num[left] > num[mid]了。也就是说，这样做就能像之前那样不断逼近了。




当然，此处不能保证left一定在second中，但是一旦left“越界”，指向了first中的元素，while循环的条件也会终止循环（除非first中都是同一个元素，那样也不影响计算结果）。




而如果num[left] > num[mid]，那么mid所指就一定在first中了。这样，可以写出修改后的程序：



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
这个题不用太在意，玩的是个小技巧。关键是理解上一道题对二分法的应用。






