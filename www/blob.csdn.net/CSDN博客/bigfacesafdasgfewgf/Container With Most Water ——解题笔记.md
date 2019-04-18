# Container With Most Water ——解题笔记 - bigfacesafdasgfewgf - CSDN博客





2015年05月06日 17:48:26[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：385标签：[LeetCode																[container](https://so.csdn.net/so/search/s.do?q=container&t=blog)](https://so.csdn.net/so/search/s.do?q=LeetCode&t=blog)
个人分类：[LeetCode](https://blog.csdn.net/puqutogether/article/category/2832605)











   【题目】

Given n non-negative integers a1, a2, ..., an, where each represents a point at coordinate (i, ai). n vertical lines are drawn such that the two endpoints of line i is at (i, ai) and (i, 0). Find
 two lines, which together with x-axis forms a container, such that the container contains the most water.


Note: You may not slant the container.





    分析：

    如果我们遍历两次vector，一定可以得到结果，但是时间复杂度是O(n^2)。

**    换种思路，两个token，一个放在vector前面，一个放在后面。判断此时的面积是否大于max，然后需要移动两个token，如何移动呢？**

**    如果height[前]小于height[后]，那么我们把前面的token后移一位；**

**    如果height[前]大于等于height[后]，那么我们把后面的token前移一位；**

**    这样移动的原则是一定要尽量保留较长的边，以保证形成的容积尽可能的大。**




    代码：



```cpp
class Solution {
public:
    int maxArea(vector<int>& height) {
        int area = 0; 
        int len = height.size();
        int head = 0; 
        int tail = len - 1; 
        while(head < tail)
        {
            int tmp = (tail - head) * min(height[head], height[tail]);
            area = max(area, tmp);
            
            if(height[head] < height[tail])
                head++;
            else
                tail--;
        }
        return area;
    }
};
```


















