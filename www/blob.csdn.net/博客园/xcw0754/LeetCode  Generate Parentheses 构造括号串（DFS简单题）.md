# LeetCode    Generate Parentheses  构造括号串（DFS简单题） - xcw0754 - 博客园
# [LeetCode    Generate Parentheses  构造括号串（DFS简单题）](https://www.cnblogs.com/xcw0754/p/4926374.html)
题意：
　　产生n对合法括号的所有组合，用vector<string>返回。
思路：
　　递归和迭代都可以产生。复杂度都可以为O(2n*合法的括号组合数)，即每次产生出的括号序列都保证是合法的。
　　方法都是差不多的，就是记录当前产生的串中含有左括号的个数cnt，如果出现右括号，就将cnt--。当长度为2*n的串的cnt为0时，就是答案了，如果当前cnt比剩下未填的位数要小，则可以继续装“(”，否则不能再装。如果当前cnt>0，那么就能继续装“)”与其前面的左括号匹配（无需要管匹配到谁，总之能匹配）。
　　递归版本
```
1 class Solution {
 2 public:
 3     void DFS(vector<string>& ans,string t,int cnt,int n)
 4     {
 5         if(n==0)    ans.push_back(t);
 6         if(cnt<n)    DFS(ans,t+"(",cnt+1,n-1);
 7         if(cnt>0)    DFS(ans,t+")",cnt-1,n-1);
 8     }
 9 
10     vector<string> generateParenthesis(int n)
11     {
12         vector<string> ans;
13         DFS(ans,"",0,n*2);
14         return ans;
15     }
16 };
AC代码
```
　　迭代版本
```
1 vector<string> generateParenthesis(int n)
 2 {
 3     vector<string> ans[2];
 4     vector<int> cnt[2];//空间换时间
 5     int cur=0;
 6     ans[0].push_back("");
 7     cnt[0].push_back(0);
 8     for(int i=n<<1; i>0; i--)
 9     {
10         cur^=1;
11         ans[cur].clear();
12         cnt[cur].clear();
13         for(int j=0; j<ans[cur^1].size(); j++)
14         {
15             string &q=ans[cur^1][j];
16             int &c=cnt[cur^1][j];
17             if(c<i)
18             {
19                 ans[cur].push_back(q+"(");
20                 cnt[cur].push_back(c+1);
21             }
22             if(c>0)
23             {
24                 ans[cur].push_back(q+")");
25                 cnt[cur].push_back(c-1);
26             }    
27         }
28     }
29     return ans[cur];
30 }
AC代码
```

