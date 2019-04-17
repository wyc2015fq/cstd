# python 小练习12 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)




给你一个整数数列a1,a2,a3,...,an,请你修改（不能删除，只能修改）最少的数字，使得数列严格单调递增。
数列存储在列表L中，你可以直接使用L，L的长度小于100000。
注意：必须保证修改后的数列依然是整数序列，不能修改成小数。
例如：L=[1,3,2],则输出1
这个题目呢，就是去找一个最长递增的子序列（就是不用去修改的最大个数），且这个子序需要满足一个条件就是两个元素之间的差值要比它们在数组L中的位置的下标差值要大，因为只有这样它们之间的元素才可以通过变化，成严格的递增的序列 =的，然后將我们数组的长度去减去这个最长递增的子序列的长度就是答案了。

举个例子说明，我们有一个[1,5,6,3,4,5,2,7,8]的序列，我们找到了它的最长子序列应该是[3,4,5,7,8]，这个子序列的值在原数组中保持不变，而其他数据也就可以通过修改变成严格递增的

怎么找这个序列呢，我们可以直接找一个数组的最大子序列的大小就可以了，利用动态规划，取M[i]表示以L[i]结尾的最大子序列的大小，我们有它的大小应该是在1（自己成立一个新的序列）或是它的前面的

的最大子序列的中满足条件的最大值，条件呢就是要满足元素之差大于元素下标之差，也就是{ M[i] = max{M[:j]} + 1, L[i] - L[k] >= i - k, k表示遍历M的下标}

代码如下

```
def dp(L):
    size = len(L)
    rsl = [0] * size
    rsl[0] = 1
    for i in range(1, size):
        for j in range(i):
            if L[i] - L[j] >= i - j and rsl[i] < rsl[j] + 1:  # 动态规划得到结果
                rsl[i] = rsl[j] + 1
        if rsl[i] == 0:  # 如果没有满足条件的，自成一个序列
            rsl[i] = 1

    return max(rsl)


L = [1,5,6,3,4,5,2,7,8]
print(len(L) - dp(L))
```













