# [LeetCode] H-Index 求H指数 - Grandyang - 博客园







# [[LeetCode] H-Index 求H指数](https://www.cnblogs.com/grandyang/p/4781203.html)







Given an array of citations (each citation is a non-negative integer) of a researcher, write a function to compute the researcher's h-index.

According to the [definition of h-index on Wikipedia](https://en.wikipedia.org/wiki/H-index): "A scientist has index *h* if *h* of his/her *N* papers have at least*h* citations each, and the other *N − h* papers have no more than *h* citations each."

Example:
Input: `citations = [3,0,6,1,5]`
Output: 3 
Explanation: `[3,0,6,1,5] `means the researcher has `5` papers in total and each of them had 
             received `3, 0, 6, 1, 5` citations respectively. 
             Since the researcher has `3` papers with at least `3` citations each and the remaining 
             two with no more than `3` citations each, her h-index is `3`.
Note: If there are several possible values for *h*, the maximum one is taken as the h-index.



这道题让我们求[H指数](https://zh.wikipedia.org/wiki/H%E6%8C%87%E6%95%B0)，这个质数是用来衡量研究人员的学术水平的质数，定义为一个人的学术文章有n篇分别被引用了n次，那么H指数就是n。而且wiki上直接给出了算法，可以按照如下方法确定某人的H指数：1、将其发表的所有SCI论文按被引次数从高到低排序；2、从前往后查找排序后的列表，直到某篇论文的序号大于该论文被引次数。所得序号减一即为H指数。我也就没多想，直接按照上面的方法写出了代码：



```
class Solution {
public:
    int hIndex(vector<int>& citations) {
        sort(citations.begin(), citations.end(), greater<int>());
        for (int i = 0; i < citations.size(); ++i) {
            if (i >= citations[i]) return i;
        }
        return citations.size();
    }
};
```



类似题目：

[H-Index II](http://www.cnblogs.com/grandyang/p/4782695.html)



参考资料：

[https://leetcode.com/problems/h-index/](https://leetcode.com/problems/h-index/)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












