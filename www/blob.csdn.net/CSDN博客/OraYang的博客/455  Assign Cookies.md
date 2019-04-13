
# 455. Assign Cookies - OraYang的博客 - CSDN博客

2017年08月19日 09:12:55[OraYang](https://me.csdn.net/u010665216)阅读数：138标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Assume you are an awesome parent and want to give your children some cookies. But, you should give each child at most one cookie. Each child i has a greed factor gi,
 which is the minimum size of a cookie that the child will be content with; and each cookie j has a size sj. If sj>=
 gi, we can assign the cookie j to the child i, and the child i will be content. Your goal is to maximize the number of your content children and
 output the maximum number.
Note:
You may assume the greed factor is always positive.
You cannot assign more than one cookie to one child.
Example 1:
Input:[1,2,3], [1,1]Output:1Explanation:You have 3 children and 2 cookies. The greed factors of 3 children are 1, 2, 3. 
And even though you have 2 cookies, since their size is both 1, you could only make the child whose greed factor is 1 content.
You need to output 1.
Example 2:
Input:[1,2], [1,2,3]Output:2Explanation:You have 2 children and 3 cookies. The greed factors of 2 children are 1, 2. 
You have 3 cookies and their sizes are big enough to gratify all of the children, 
You need to output 2.思路：本题就是两个数字，逐步比较，分为三个步骤：
step1:给两个数组排序g,s
step2:给每个数组各维护一个指针g_p,s_p，遍历
step3:满足条件，count加1，否则，继续循环
代码：

```python
class Solution {
public:
    int findContentChildren(vector<int>& g, vector<int>& s) {
        sort(g.begin(),g.end());
        sort(s.begin(),s.end());
        int count =0;
        for(int i =0,j=0;i<g.size()&&j<s.size();){
            if(g[i]<=s[j])
            {
                count++;
                i++;
                j++;
                continue;
            }
            else if(g[i]>s[j])
            {
                j++;
                continue;
            }
            
        }
        return count;
    }
};
```



