# leetcode-11:Container With Most Water盛最多水的容器 - 别说话写代码的博客 - CSDN博客





2018年12月17日 21:02:11[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：33标签：[盛最多水的容器																[Container With Most Water																[leetcode 11](https://so.csdn.net/so/search/s.do?q=leetcode 11&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=Container With Most Water&t=blog)




题目：
|Given *n* non-negative integers *a1*, *a2*, ..., *an *, where each represents a point at coordinate (*i*, *ai*). *n* vertical lines are drawn such that the two endpoints of line *i* is at (*i*, *ai*) and (*i*, 0). Find two lines, which together with x-axis forms a container, such that the container contains the most water.**Note: **You may not slant the container and *n* is at least 2.![](https://img-blog.csdnimg.cn/20181217210212657)The above vertical lines are represented by array [1,8,6,2,5,4,8,3,7]. In this case, the max area of water (blue section) the container can contain is 49.**Example:****Input:** [1,8,6,2,5,4,8,3,7]**Output:** 49|给定 *n* 个非负整数 *a*1，*a*2，...，*a*n，每个数代表坐标中的一个点 (*i*, *ai*) 。在坐标内画 *n* 条垂直线，垂直线 *i* 的两个端点分别为 (*i*, *ai*) 和 (*i*, 0)。找出其中的两条线，使得它们与 *x* 轴共同构成的容器可以容纳最多的水。**说明：**你不能倾斜容器，且 *n* 的值至少为 2。![](https://img-blog.csdnimg.cn/20181217210212760)图中垂直线代表输入数组 [1,8,6,2,5,4,8,3,7]。在此情况下，容器能够容纳水（表示为蓝色部分）的最大值为 49。**示例:****输入:** [1,8,6,2,5,4,8,3,7]**输出:** 49|
|----|----|

 思路：求最大，可以从两边向中间算。令i从左边开始，j从右边开始。height[i]和height[j]越接近越好，每次左右两端舍弃小的那一端就行。

```cpp
class Solution {
public:
    int maxArea(vector<int>& height) {
        int j=height.size()-1,i=0,ret=0;
        while(i<j)
        {
            ret = max(ret,min(height[i],height[j])*(j-i));
            height[j]>height[i]?i++:j--;    
        }return ret;
    }
};
```](https://so.csdn.net/so/search/s.do?q=盛最多水的容器&t=blog)




