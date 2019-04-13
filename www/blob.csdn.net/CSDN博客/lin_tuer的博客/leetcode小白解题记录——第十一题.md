
# leetcode小白解题记录——第十一题 - lin_tuer的博客 - CSDN博客


2016年10月10日 20:46:47[lin_tuer](https://me.csdn.net/lin_tuer)阅读数：198


### 11. Container With Most Water
Givennnon-negative integersa1,a2,
 ...,an, where each represents a point at coordinate (i,ai).nvertical
 lines are drawn such that the two endpoints of lineiis at (i,ai) and (i,
 0). Find two lines, which together with x-axis forms a container, such that the container contains the most water.
Note: You may not slant the container.
[Subscribe](https://leetcode.com/subscribe/)to see which companies asked this question
看到一个比较直白简洁有效的解决方案——栈法~

》复杂度
时间 O(N) 空间 O(N)
》思路
最大盛水量取决于两边中较短的那条边，而且如果将较短的边换为更短边的话，盛水量只会变少。所以我们可以用两个头尾指针，计算出当前最大的盛水量后，将较短的边向中间移，因为我们想看看能不能把较短的边换长一点。这样一直计算到左边大于右边为止就行了。
》代码
```java
public class Solution {
    public int maxArea(int[] height) {
        int left = 0, right = height.length - 1, maxArea = 0;
        while(left < right){
            // 每次更新最大面积（盛水量）
            maxArea = Math.max(maxArea, Math.min(height[left], height[right]) * (right - left));
            // 如果左边较低，则将左边向中间移一点
            if(height[left] < height[right]){
                left++;
            // 如果右边较低，则将右边向中间移一点
            } else {
                right--;
            }
        }
        return maxArea;
    }
}
```




