# LeetCode Pascal's Triangle Pascal三角形 - xcw0754 - 博客园
# [LeetCode Pascal's Triangle Pascal三角形](https://www.cnblogs.com/xcw0754/p/4456858.html)
![](https://images0.cnblogs.com/blog2015/641737/201504/252226095622422.png)
题意：给一个数字，返回一个二维数组，包含一个三角形。
思路：n=0、1、2都是特例，特别处理。3行以上的的头尾都是1，其他都是依靠上一行的两个数。具体了解Pascal三角形原理。
```
1 class Solution {
 2 public:
 3     vector<vector<int> > generate(int numRows) {
 4         vector<vector<int> > ans;
 5         if(!numRows) return ans;
 6         vector<int> tmp;
 7         tmp.push_back(1);ans.push_back(tmp);if(numRows==1) return ans;
 8         tmp.push_back(1);ans.push_back(tmp);if(numRows==2) return ans;
 9     
10         for(int i=2; i<numRows; i++)
11         {
12             tmp.clear();
13             tmp.push_back(1);
14             for(int j=1; j<i; j++)
15             {
16                 tmp.push_back( ans[i-1][j-1]+ans[i-1][j] );
17             }
18             tmp.push_back(1);
19             ans.push_back(tmp);
20         }
21         return ans;
22     }
23 };
Pascal's Triangle
```

