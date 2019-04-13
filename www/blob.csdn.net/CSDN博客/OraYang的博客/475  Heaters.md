
# 475. Heaters - OraYang的博客 - CSDN博客

2017年08月19日 17:08:27[OraYang](https://me.csdn.net/u010665216)阅读数：145标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Winter is coming! Your first job during the contest is to design a standard heater with fixed warm radius to warm all the houses.
Now, you are given positions of houses and heaters on a horizontal line, find out minimum radius of heaters so that all houses could be covered by those heaters.
So, your input will be the positions of houses and heaters seperately, and your expected output will be the minimum radius standard of heaters.
Note:
Numbers of houses and heaters you are given are non-negative and will not exceed 25000.
Positions of houses and heaters you are given are non-negative and will not exceed 10^9.
As long as a house is in the heaters' warm radius range, it can be warmed.
All the heaters follow your radius standard and the warm radius will the same.
Example 1:
Input:[1,2,3],[2]Output:1Explanation:The only heater was placed in the position 2, and if we use the radius 1 standard, then all the houses can be warmed.
Example 2:
Input:[1,2,3,4],[1,4]Output:1Explanation:The two heater was placed in the position 1 and 4. We need to use radius 1 standard, then all the houses can思路：本题先排序，然后给两个指针，按照顺序找到每个屋子距离最近的加热器，记录其位置差，所有的位置差里面最长的那一个就是最小的加热器半径了
代码：

```python
class Solution {
public:
    int findRadius(vector<int>& houses, vector<int>& heaters) {
        sort(houses.begin(),houses.end());
        sort(heaters.begin(),heaters.end());
        int n = houses.size();
        int m = heaters.size();
        int radius = 0;
        int j = 0;
        for(int i = 0 ;i< n ;i++){
            while( j < m - 1 && (abs(heaters[j] - houses[i]) >= abs(heaters[j+1] - houses[i])))
                j ++;
            radius = max(radius,abs(heaters[j] - houses[i]));
        }
        return radius;
    }
};
```


