# 跳跃游戏 II - guoziqing506的博客 - CSDN博客





2016年05月23日 17:15:17[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：2318
所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)









题目描述：给出一个非负整数数组，你最初定位在数组的第一个位置。数组中的每个元素代表你在那个位置可以跳跃的最大长度。你的目标是使用最少的跳跃次数到达数组的最后一个位置。

样例：给出数组A = [2,3,1,1,4]，最少到达数组最后一个位置的跳跃次数是2(从数组下标0跳一步到数组下标1，然后跳3步到数组的最后一个位置，一共跳跃2次)

跟上一道题：“跳跃游戏”（详见：[点击打开链接](http://blog.csdn.net/guoziqing506/article/details/51459069)）基本逻辑是一样的，都是贪心法的应用。其实这道题当中，贪心法用得更明显了。

为了能够以最少的步数跳到最后，则每次获得能达到的最远距离后（比如，样例中，数组的第一个元素是2，那么最远能达到元素1（第一个）的那个位置），我们可以在当前位置到当前位置所能达到的最远位置中寻找最大值，这个最大值所在的位置，就是我们下一步要跳到的位置。拿样例来说，从2开始，第一步之后，调到3，发现第二步再跳的话能直接到最后了，得到结果。

逻辑很简单，代码写起来有点麻烦，不过不难，细心就能写对：



```python
class Solution:
    # @param A, a list of integers
    # @return an integer
    def jump(self, A):
        n = len(A)
        cur, far = 0, 0
        step = 0
        far = A[0]
        while cur < n:
            # 能跳到最后了，直接返回
            if far >= n - 1:
                return step + 1
            # 初始化next_step
            next_step = cur
            # 把此时能跳到的最远位置用temp记录下来
            temp = far
            # 寻找最大值
            while cur <= temp:
                if A[cur] + cur > far:
                    next_step = cur
                    far = A[cur] + cur
                cur += 1
            # 跳了一步
            step += 1
            # 从下一个位置起跳
            cur = next_step
        # write your code here
```


其中，next_step代表下一步要开始的位置，far表示能达到的最远距离，step记录跳跃的步数






