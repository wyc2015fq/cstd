# Leetcode 11. Container With Most Water - zqjackking的博客 - CSDN博客





2017年02月20日 15:34:11[zqjackking](https://me.csdn.net/zqjackking)阅读数：272标签：[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[深度学习](https://blog.csdn.net/zqjackking/article/category/6555105)









Given n non-negative integers a1, a2, …, an, where each represents a point at coordinate (i, ai). n vertical lines are drawn such that the two endpoints of line i is at (i, ai) and (i, 0). Find two lines, which together with x-axis forms a container, such that the container contains the most water. 

一开始用了穷举法，时间超了。 

后来看到了大神的代码才恍然大悟。
```
int area = 0;
int i = 0, j = height.size() - 1;
while (i < j) 
{
int h = min(height[i], height[j]);
area = max(area, (j - i) * h);
if(height[i]<=h)
    i++;
else
    j--;
}
return area;
```

其思路：先从两边（即最大宽度着手），然后再向中间合拢。因为容器的高度总是取决于短的那一边，所以需要短的那一边做出改变。现在我们来证明下这种方法的合理性。(现在假设下标从1开始） 

首先，我们算出Area1=min(an,a1)*(n-1),这也是宽度为n的唯一情况。 

接下来，当宽度为n-1的时候有两种情况。我们假设a1




