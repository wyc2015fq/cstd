
# 492. Construct the Rectangle - OraYang的博客 - CSDN博客

2017年08月21日 16:33:39[OraYang](https://me.csdn.net/u010665216)阅读数：114标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
For a web developer, it is very important to know how to design a web page's size. So, given a specific rectangular web page’s area, your job by now is to design a rectangular web page, whose length L and width W satisfy the following requirements:
1. The area of the rectangular web page you designed must equal to the given target area.
2. The width W should not be larger than the length L, which means L >= W.
3. The difference between length L and width W should be as small as possible.You need to output the length L and the width W of the web page you designed in sequence.
Example:
Input:4Output:[2, 2]Explanation:The target area is 4, and all the possible ways to construct it are [1,4], [2,2], [4,1]. 
But according to requirement 2, [1,4] is illegal; according to requirement 3,  [4,1] is not optimal compared to [2,2]. So the length L is 2, and the width W is 2.
Note:
The given area won't exceed 10,000,000 and is a positive integer
The web page's width and length you designed must be positive integers.
思路：本题的思路是构造矩形，并不断比较长宽差，差距最小的保留输出
代码：

```python
class Solution {
public:
    vector<int> constructRectangle(int area) {
        vector<int> res;
        int size = sqrt(area);
        int diff,l,w;
        diff = area -1;
        l =area;
        w = 1;
        if(area%2)
        {
            for(int i =1;i<=area;i = i+2)
            {
                if(area%i==0)
                {
                    if(diff>abs(area/i - i)){
                    diff = area/i - i;
                    w = area / i < i?area / i:i;
                    l = area / i < i?i:area / i;                 
                    }
                }
            }
        }
        else
        {
            for(int j =2;j<=area;j = j+2){
                if(area%j==0)
                {
                    if(diff>abs(area/j - j)){
                    diff = area/j - j;
                    w = area / j < j?area / j:j;
                    l = area / j < j?j:area / j;                   
                    }
                }
            }
        }
        res.push_back(l);
        res.push_back(w);
        return res;
    }
};
```


