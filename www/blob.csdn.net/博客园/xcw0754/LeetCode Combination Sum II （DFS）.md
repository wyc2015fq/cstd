# LeetCode Combination Sum II （DFS） - xcw0754 - 博客园
# [LeetCode Combination Sum II （DFS）](https://www.cnblogs.com/xcw0754/p/4970397.html)
题意：
　　在集合candidates中选出任意多个元素，使得他们的和为target，返回所有的组合，以升序排列。
思路：
　　难点在于如何去重，比如集合{1，1，2}，target=3，那么只有一个组合就是1+2=3，而不是两个。
　　DFS解决，每次考虑candidates[i]取还是不取，但是这样子还是会产生重复，这里只需要一个技巧就可以使得没有重复出现。如果当前元素已经被考虑过取了，那么在考虑不取的时候，i后面的与candidates[i]相同的都要跳过。观察一下可以发现，如果有一串相同的数字出现的话，只会考虑取到前面的那几个元素而已。
```
1 class Solution {
 2 public:
 3     vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
 4         n=target;
 5         sort(candidates.begin(),candidates.end());
 6         DFS(0,0,candidates,tmp);
 7         return ans;
 8     }
 9 
10     void DFS(int pos,int sum,const vector<int>& old,vector<int>& num)
11     {
12         if(sum==n)
13         {
14             ans.push_back(num);
15             return ;
16         }
17         if(sum>=n||pos>=old.size())    return;
18         for(;pos<old.size(); pos++)
19         {
20             num.push_back(old[pos]);
21             DFS(pos+1,sum+old[pos],old,num);
22             num.pop_back();
23             while(pos<old.size()&&old[pos]==old[pos+1]) pos++;
24         }
25     }
26 private:
27     vector<vector<int>> ans;
28     vector<int> tmp;
29     int n;
30 };
AC代码
```

