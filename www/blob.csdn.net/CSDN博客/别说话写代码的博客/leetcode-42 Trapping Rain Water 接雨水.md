# leetcode-42. Trapping Rain Water 接雨水 - 别说话写代码的博客 - CSDN博客





2019年01月09日 22:49:41[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：32
所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)








|Given *n* non-negative integers representing an elevation map where the width of each bar is 1, compute how much water it is able to trap after raining.![](https://assets.leetcode.com/uploads/2018/10/22/rainwatertrap.png)			The above elevation map is represented by array [0,1,0,2,1,0,1,3,2,1,2,1]. In this case, 6 units of rain water (blue section) are being trapped. **Thanks Marcos**for contributing this image!**Example:****Input:** [0,1,0,2,1,0,1,3,2,1,2,1]**Output:** 6|给定 *n* 个非负整数表示每个宽度为 1 的柱子的高度图，计算按此排列的柱子，下雨之后能接多少雨水。![](https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2018/10/22/rainwatertrap.png)上面是由数组 [0,1,0,2,1,0,1,3,2,1,2,1] 表示的高度图，在这种情况下，可以接 6 个单位的雨水（蓝色部分表示雨水）。 **感谢 Marcos** 贡献此图。**示例:****输入:** [0,1,0,2,1,0,1,3,2,1,2,1]**输出:** 6|
|----|----|

思路：这道题目 类似于[直方图中的 最大矩形](http://www.cnblogs.com/grandyang/p/4322653.html)和[盛水最多的容器](https://blog.csdn.net/qq_21997625/article/details/85056185)。**第一种方法**就是，i和j分别从前往后和从后往前遍历直到i>=j，看他们对应的 值哪个小，若height[i]<=height[j]，那就从左往右如果遇到的 高度小于当前的 高度，加上height[i]与当前高度的差；若height[i]>height[j]，那就从右往左如果遇到高度小于当前高度，则res加上height[j]与挡圈高度的差。最终res就是雨水容量。比如题目中从前往后height[2]<height[1],那res+=1,(注释height[1]-height[2]),接着从后往前height[9]<height[10],res+=1,(注释height[10]-height[9]),接着从前往后height[4]<height[3],res+=1,（注释，height[3]-height[4]）,接着继续Height[5]<height[3],res+=2,（注释，height[3]-height[5]）,接着继续往后height[6]<height[3],res+=1，（注释，height[3]-height[6]）。下面是第一种代码实现

**第二种方法**是使用DP。

**第三种是**用栈。

二三种方法参考：[http://www.cnblogs.com/grandyang/p/4402392.html](http://www.cnblogs.com/grandyang/p/4402392.html)

```cpp
class Solution {
public:
    int trap(vector<int>& height) {
        int res=0,i=0,j=height.size()-1;
        while(i<j)
        {
            int small=min(height[i],height[j]);
            if(small==height[i])
            {
                ++i;
                while(i<j && height[i]<small)
                    res += small - height[i++];
            }else
            {
                --j;
                while(i<j && height[j]<small)
                    res +=small - height[j--];
            }
        }return res;
    }
};
```





