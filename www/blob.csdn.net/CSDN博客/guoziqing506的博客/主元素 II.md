# 主元素 II - guoziqing506的博客 - CSDN博客





2016年05月23日 19:00:58[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：1445
所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)









题目描述：给定一个整型数组，找到主元素，它在数组中的出现次数严格大于数组元素个数的三分之一。

样例：给出数组[1,2,1,2,1,3,3] 返回 1

之前，有果一道“主元素”的问题（详见：[点击打开链接](http://blog.csdn.net/guoziqing506/article/details/51434299)），我们用的是“消去法”，因为主元素的个数大于元素总数的二分之一，可以保存只有一个元素的hash表，通过消去法解决。




现在条件变了，变成大于三分之一了。其实，这个题目还有一个变形，那就是严格大于1/k的主元素搜寻。其实道理是一样的。我们的基本做法如下：

首先，先要搞清楚这个问题在数学上的原理。既然主元素的判定标准是个数大于数组长度的1/k，那换句话说，就是满足如下的公式：

x/n > 1/k

其中，我假设x为主元素的个数，n为数组长度。然后，将这个公式化简，得到下面这个式子：

x - 1 > n/k - 1  ->  (x - 1) / (n - k) > 1/ k

这个式子说明了一个很重要的问题：当主元素的个数减1后，如果整个数组的长度也减去k，是不会影响主元素的“地位”的。




于是，我们可以按照以下步骤设计算法：

1. 遍历数组，建立一个键为数组中元素，值为当前元素出现次数的hash表同时根据遍历结果，更新hash表

2. 当hash表中键值对的个数小于k时，继续步骤1，更新即可；而如果hash表中键值对的个数等于k，则对现在hash表中的所有键的值减1，也就是一共减去了k，这样，相当于是用新遍历到的元素持续冲击现有的hash表。每次冲击的结果是，将新元素消掉，同时整个数组元素总数减了k。可以想象，如果新的元素不是主元素，那消掉也无所谓，而如果新的元素是主元素，那也没有影响它的地位。

这样的持续冲击，势必会出现至少一个键所对的值为0，我们剔除值为0的键。需要注意的是，这样做的结果使得hash表的长度永远是小于k的。

3. 持续进行以上两步，直到所有数组元素全部被遍历完。




最后，对现在得到的这个hash表的值归0，然后再遍历一遍数组，统计现在hash表中的元素的个数，返回个数最多的那个元素，也就是主元素。




现在分析一下为什么这样做是对的，分两种情况讨论即可：

1. 数组中不同元素的个数小于k，那hash表的长度始终达不到k，主元素一定被保存在hash表的键中，最后遍历一遍数组之后，就会“浮出水面”

2. 数组中不同元素的个数大于或等于k，那又可分为两种情况：

    (1) 当hash表中的键的个数达到k时，主元素恰好在hash表中，那么根据上面的公式知道，对所有键所对的值减1，不会影响主元素的“地位”

    (2) 当hash表中的键的个数达到k时，主元素不在hash表中，那么减去别的元素，也不影响主元素的“地位”，而且，最终，主元素一定会被保存在hash表中。




到此，可以给出代码了：



```python
class Solution:
    """
    @param nums: A list of integers
    @return: The majority number occurs more than 1/3
    """
    def majorityNumber(self, nums):

        hash_table = {}

        for i in nums:
            if i not in hash_table:
                hash_table[i] = 1
            else:
                hash_table[i] += 1

            if len(hash_table) == 3:
                for key in hash_table:
                    hash_table[key] -= 1
                temp = {}
                for key in hash_table:
                    if hash_table[key] != 0:
                        temp[key] = hash_table[key]
                hash_table = temp

        for key in hash_table:
            hash_table[key] = 0

        for i in nums:
            if i in hash_table:
                hash_table[i] += 1

        return max(hash_table.items(), key=lambda x: x[1])[0]
        # write your code here
```
去年给出的答案有点问题，虽然lintcode上通过，但其实是错误的，感谢有心的读者指正。现已更新，欢迎批评~         


Date: 2017-3-3







