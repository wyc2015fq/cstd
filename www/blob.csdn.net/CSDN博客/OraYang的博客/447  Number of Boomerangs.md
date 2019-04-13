
# 447. Number of Boomerangs - OraYang的博客 - CSDN博客

2017年08月16日 11:54:51[OraYang](https://me.csdn.net/u010665216)阅读数：105所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Givennpoints in the plane that are all pairwise distinct, a "boomerang" is a tuple of points`(i,
 j, k)`such that the distance between`i`and`j`equals
 the distance between`i`and`k`(the
 order of the tuple matters).
Find the number of boomerangs. You may assume thatnwill be at most500and coordinates of points are all in the range[-10000, 10000](inclusive).
Example:
Input:[[0,0],[1,0],[2,0]]Output:2Explanation:The two boomerangs are[[1,0],[0,0],[2,0]]and[[1,0],[2,0],[0,0]]思路：本题思路很简单，实现两个函数，一个是求距离的函数
```python
sumq(pair<int,int> p1,pair<int,int> p2)
```
一个是求个数的函数
```python
sumn(map<long,int> res)
```
通过两个嵌套for循环来实现遍历代码：

```python
class Solution {
public:
    int numberOfBoomerangs(vector<pair<int, int>>& points) {
        map<long,int> res;
        int sum = 0;
        for(int i =0;i<=points.size()-1;i++)
        {
            for(int j =0;j<=points.size()-1;j++)
            {
                if(j!=i)
                    res[(sumq(points[i],points[j]))]++;
            }
            sum+=sumn(res);
            res.clear();        
        }
    return sum;
        
    }
private:
    long sumq(pair<int,int> p1,pair<int,int> p2){
        long distance = (p1.first-p2.first)*(p1.first-p2.first)+(p1.second-p2.second)*(p1.second-p2.second);
        return distance;
    }
    int sumn(map<long,int> res){
        map<long,int>::iterator itr = res.begin();
        int count =0;
        while(itr!=res.end())
        {
            if(itr->second>1)
                count+=itr->second*(itr->second-1);
            itr++;
        }
        return count;
        
    }
    
};
```


