# LeetCode Pascal's Triangle II （杨辉三角） - xcw0754 - 博客园
# [LeetCode Pascal's Triangle II （杨辉三角）](https://www.cnblogs.com/xcw0754/p/4638714.html)
题意：给出杨辉三角的层数k，返回最后一层。k=0时就是只有一个数字1。
思路：滚动数组计算前一半出来，返回时再复制另一半。简单但是每一句都挺长的。
0ms的版本：
```
1 class Solution {
 2 public:
 3     vector<int> getRow(int rowIndex) {
 4         if(rowIndex==0) return vector<int>(1,1);    //0和1特殊处理
 5         if(rowIndex==1) return vector<int>(2,1);
 6         vector<int> ans[2];
 7         ans[0].push_back(1);    //只需要处理一半，另一半在返回时复制。
 8         for(int i=2; i<=rowIndex; i++)
 9         {
10             ans[~i&1].clear();              //滚动数组
11             ans[~i&1].push_back(1);         //这是必须的
12             int j=1;
13             for(; j<ans[i&1].size(); j++)    ans[~i&1].push_back( ans[i&1][j-1]+ans[i&1][j]);
14 
15             if(i%2==0)    ans[~i&1].push_back( ans[i&1][j-1]+ans[i&1][j-1] );   //k为偶数时，里面有奇数个呢。
16         }
17 
18 
19         ans[rowIndex&1].clear();
20         ans[rowIndex&1].insert( ans[rowIndex&1].end(),ans[~rowIndex&1].begin(), ans[~rowIndex&1].end()  );
21 
22         if(!(rowIndex&1))
23             ans[rowIndex&1].insert( ans[rowIndex&1].end(),ans[~rowIndex&1].rbegin()+1, ans[~rowIndex&1].rend() );//总数为奇数个，最后1个不要复制进去。
24         else
25             ans[rowIndex&1].insert( ans[rowIndex&1].end(), ans[~rowIndex&1].rbegin(), ans[~rowIndex&1].rend() );//偶数个，全复制。
26         return ans[rowIndex&1];
27     }
28 };
AC代码
```
简洁但4ms的版本：
```
class Solution {
public:
    vector<int> getRow(int rowIndex) {
        vector<int> ans(rowIndex+1,1);
        for(int i=1; i<rowIndex; i++)   //正在产生第i+2行。
        {
            for(int j=i; j>0; j--)      //必须从右开始，不然前面行就被覆盖了。
            {
                ans[j]+=ans[j-1];
            }
        }
        return ans;
    }
};
AC代码
```

