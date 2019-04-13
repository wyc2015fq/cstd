
# 661. Image Smoother - OraYang的博客 - CSDN博客

2017年08月31日 22:42:31[OraYang](https://me.csdn.net/u010665216)阅读数：717所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given a 2D integer matrix M representing the gray scale of an image, you need to design a smoother to make the gray scale of each cell becomes the average gray scale (rounding down) of all the 8 surrounding cells and itself. If a cell has less than 8 surrounding
 cells, then use as many as you can.
Example 1:
Input:[[1,1,1],
 [1,0,1],
 [1,1,1]]Output:[[0, 0, 0],
 [0, 0, 0],
 [0, 0, 0]]Explanation:For the point (0,0), (0,2), (2,0), (2,2): floor(3/4) = floor(0.75) = 0
For the point (0,1), (1,0), (1,2), (2,1): floor(5/6) = floor(0.83333333) = 0
For the point (1,1): floor(8/9) = floor(0.88888889) = 0
Note:
The value in the given matrix is in the range of [0, 255].
The length and width of the given matrix are in the range of [1, 150].
思路：本题思路很简单，咱们是用最简单易懂的思路：额外构建两个二维数组，一个数组保留原数组的值不变，在四周包裹上0（用来统计每次计算的分子），一个数组是原数组所有值替换为1，再在四周包裹上0（用来统计每次计算的分母）
代码：

```python
class Solution {
public:
    vector<vector<int>> imageSmoother(vector<vector<int>>& M) {
        
        vector<vector<int>> res = M;
        vector<int> a(M[0].size(),1);
        vector<vector<int>> b(M.size(),a);     
        for(int i=0;i<M.size();i++)
        {
            M[i].insert(M[i].begin(),0);
            M[i].push_back(0);
            b[i].insert(b[i].begin(),0);
            b[i].push_back(0);          
        }
        vector<int> temp(M[0].size(),0);
        M.insert(M.begin(),temp);
        M.push_back(temp);
        b.insert(b.begin(),temp);
        b.push_back(temp);
        for(int i =1;i<M.size()-1;i++)
        {
            for(int j=1;j<M[0].size()-1;j++)
            {
                res[i-1][j-1] = floor((M[i][j]+M[i][j-1]+M[i][j+1]+M[i-1][j]+M[i-1][j-1]+M[i-1][j+1]+M[i+1][j]+M[i+1][j-1]+M[i+1][j+1])/(b[i][j]+b[i][j-1]+b[i][j+1]+b[i-1][j]+b[i-1][j-1]+b[i-1][j+1]+b[i+1][j]+b[i+1][j-1]+b[i+1][j+1]));
            }
        }
        return res;
    }
};
```


