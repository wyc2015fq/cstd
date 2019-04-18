# LeetCode Permutations （全排列） - xcw0754 - 博客园
# [LeetCode Permutations （全排列）](https://www.cnblogs.com/xcw0754/p/4931626.html)
题意：
　　给出n个元素，请产生出所有的全排列。
思路：
　　注意到可能会有相同的排列出现，比如 {2，2}。还有可能是乱序列（大部分情况下都是无所谓的）。
　　递归（1）：产生的过多的多余vector。
```
1 class Solution {
 2 public:
 3     void recursion(vector<int> num, int i,  vector<vector<int> > &res) 
 4     {
 5         if (i+1==num.size())    res.push_back(num);
 6         else
 7         {
 8             for (int j=i; j<num.size(); j++)
 9             {
10                 if(j!=i&&num[i]==num[j])    continue;    
11                 swap(num[i], num[j]);
12                 recursion(num, i+1, res);
13             }
14         }
15     }
16     vector<vector<int> > permute(vector<int> &num) 
17     {
18         vector<vector<int> >res;
19         recursion(num, 0, res);
20         return res;
21     }
22 };
AC代码
```
　　 递归（2）：只要保证每次交换后都能换回来，必定能恢复到原来的样子，所以不需要产生过多的多余vector。
```
1 class Solution {
 2     vector<vector<int> > ans;     
 3 public:
 4 
 5     void DFS(vector<int>& num,int pos)
 6     {
 7         if(pos+1==num.size())    ans.push_back(num);
 8         else
 9         {
10             for(int i=pos; i<num.size(); i++)
11             {
12                 swap(num[i],num[pos]);
13                 DFS(num,pos+1);
14             swap(num[i],num[pos]);//换回来
15             }
16         }
17     }
18 
19     vector<vector<int> > permute(vector<int> &num)
20     {
21         if(!num.empty())    DFS(num,0);
22         return ans;
23     }
24 };
AC代码
```
　　迭代法：类似于BFS，由于不会有重复的数字，所以一个个数来分配。当分配一个数时，只要其前面不会有相同的数，就可以插入到末尾。速度也慢了很多。
```
1 class Solution {
 2     deque<vector<int> > que;    
 3 public:
 4 
 5     bool isok(vector<int>& num,int a)
 6     {
 7         for(int i=0; i+1<num.size(); i++)
 8             if(num[i]==a)    return false;
 9         return true;
10     }
11 
12 
13     vector<vector<int> > permute(vector<int> &num)
14     {
15         que.push_back(vector<int>());
16         for(int i=0; i<num.size(); i++)
17         {
18             int siz=que.size();
19             for(int j=0; j<siz; j++)
20             {
21                 vector<int> tmp(que.front());
22                 que.pop_front();
23                 tmp.push_back(0);
24                 for(int k=0; k<num.size(); k++)
25                 {
26                     if(isok(tmp,num[k]))
27                     {
28                         tmp[i]=num[k];
29                         que.push_back(tmp);
30                     }
31                 }
32             }
33         }
34         return vector<vector<int> >(que.begin(),que.end());
35     }
36 };
AC代码
```

