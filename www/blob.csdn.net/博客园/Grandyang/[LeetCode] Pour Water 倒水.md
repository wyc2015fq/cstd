# [LeetCode] Pour Water 倒水 - Grandyang - 博客园







# [[LeetCode] Pour Water 倒水](https://www.cnblogs.com/grandyang/p/8460541.html)







We are given an elevation map, `heights[i]` representing the height of the terrain at that index. The width at each index is 1. After `V` units of water fall at index `K`, how much water is at each index?

Water first drops at index `K` and rests on top of the highest terrain or water at that index. Then, it flows according to the following rules:
- If the droplet would eventually fall by moving left, then move left.
- Otherwise, if the droplet would eventually fall by moving right, then move right.
- Otherwise, rise at it's current position.

Here, "eventually fall" means that the droplet will eventually be at a lower level if it moves in that direction. Also, "level" means the height of the terrain plus any water in that column.



We can assume there's infinitely high terrain on the two sides out of bounds of the array. Also, there could not be partial water being spread out evenly on more than 1 grid block - each unit of water has to be in exactly one block.



Example 1:
Input: heights = [2,1,1,2,1,2,2], V = 4, K = 3
Output: [2,2,2,3,2,2,2]
Explanation:
#       #
#       #
##  # ###
#########
 0123456    <- index

The first drop of water lands at index K = 3:

#       #
#   w   #
##  # ###
#########
 0123456    

When moving left or right, the water can only move to the same level or a lower level.
(By level, we mean the total height of the terrain plus any water in that column.)
Since moving left will eventually make it fall, it moves left.
(A droplet "made to fall" means go to a lower height than it was at previously.)

#       #
#       #
## w# ###
#########
 0123456    

Since moving left will not make it fall, it stays in place.  The next droplet falls:

#       #
#   w   #
## w# ###
#########
 0123456  

Since the new droplet moving left will eventually make it fall, it moves left.
Notice that the droplet still preferred to move left,
even though it could move right (and moving right makes it fall quicker.)

#       #
#  w    #
## w# ###
#########
 0123456  

#       #
#       #
##ww# ###
#########
 0123456  

After those steps, the third droplet falls.
Since moving left would not eventually make it fall, it tries to move right.
Since moving right would eventually make it fall, it moves right.

#       #
#   w   #
##ww# ###
#########
 0123456  

#       #
#       #
##ww#w###
#########
 0123456  

Finally, the fourth droplet falls.
Since moving left would not eventually make it fall, it tries to move right.
Since moving right would not eventually make it fall, it stays in place:

#       #
#   w   #
##ww#w###
#########
 0123456  

The final answer is [2,2,2,3,2,2,2]:

    #    
 ####### 
 ####### 
 0123456 



Example 2:
Input: heights = [1,2,3,4], V = 2, K = 2
Output: [2,3,3,4]
Explanation:
The last droplet settles at index 1, since moving further left would not cause it to eventually fall to a lower height.



Example 3:
Input: heights = [3,1,3], V = 5, K = 1
Output: [4,4,4]



Note:
- `heights` will have length in `[1, 100]` and contain integers in `[0, 99]`.
- `V` will be in range `[0, 2000]`.
- `K` will be in range `[0, heights.length - 1]`.



这道题说有不同高度的地面，每次都位置K有水滴落下，水滴落下后移动的方向有如下的规则：

1. 如果水滴向左移动后最终停止的位置低于落下的位置，则向左移动。

2. 否则若水滴向右移动后最终停止的位置低于落下的位置，则向右移动。

3. 否则停在原来的位置。

水滴停止后，原来的位置高度就增加1，让我们返回最后地面的高度。我们首先来分析题目中的例子1:

```
#       #
#   4   #
##21#3###
#########
```

我们可以观察出，如果左边到的位置低的话，就先去左边，即便右边能到同样低的位置，也还是左边优先。但是这个例子没能说明，如果右边的位置更低的话，是去右边呢，还是左边，看下面这个例子：

```
54#     #
32###   ###
1##### #####
#############
```

红色表示水滴落下的位置，我们可以看到第五滴水没有去右边更低的地方，而是去了左边的位置，这说明，左边有至高优先权，只要左边的最终位置低于水滴落下的位置，一定会去左边。

还有就是，去一个方向最终要落到一个局部最低点，请看下面这个例子：

```
#
###1# #
##### #
#######
```

我们可以看到，水滴去了右边第一个局部最低点，而再右边的全局最低点是无法到达的。如果都是一样的高度的话，落在离水滴落下起始位置最近的点，请看下下面这两个例子：

```
#
###1#
#####
```

```
#1
#####
#####
```

第一个例子中水滴去了局部最低点，第二个例子中由于右边的高度都相同，水滴去了最靠近起始位置的点。

那么分析到这里，我想思路应该比较明晰了吧。首先我们尝试向左走，找到第一个局部最低点，停止条件是左边的高度大于当前高度，但是为了防止出现大家高度都一样而需要停止在靠近起始点位置的情况，我们来一个回滚操作，就是只要和右边的高度一样，就一直往右滚。同样，在尝试向右走，找第一个局部最低点，停止条件是右边的高度大于当前高度，但是为了防止出现大家高度都一样而需要停止在靠近起始点位置的情况，我们也来一个回滚操作，就是只要和左边的高度一样，就一直往左滚。那么此时我们来做比较，如果左边的局部最低点小于雨滴落下位置的高度，那么左边局部最低点高度自增1。否则如果右边的局部最低点高度小于雨滴落下位置的高度，则右边局部最低点高度自增1。如果左右高度都一样，则雨滴落下位置的高度自增1，参见代码如下：



```
class Solution {
public:
    vector<int> pourWater(vector<int>& heights, int V, int K) {
        for (int i = 0; i < V; ++i) {
            int l = K, r = K, n = heights.size();
            while (l > 0 && heights[l] >= heights[l - 1]) --l;
            while (l < K && heights[l] == heights[l + 1]) ++l;
            while (r < n - 1 && heights[r] >= heights[r + 1]) ++r;
            while (r > K && heights[r] == heights[r - 1]) --r;
            (heights[l] < heights[K]) ? ++heights[l] : ++heights[r];
        }
        return heights;
    }
};
```



类似题目：

[Trapping Rain Water](http://www.cnblogs.com/grandyang/p/4402392.html)



参考资料：

[https://leetcode.com/problems/pour-water/discuss/116670/My-8-Lines-C++-Solution](https://leetcode.com/problems/pour-water/discuss/116670/My-8-Lines-C++-Solution)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












