# LeetCode Bulls and Cows （简单题） - xcw0754 - 博客园
# [LeetCode Bulls and Cows （简单题）](https://www.cnblogs.com/xcw0754/p/4934083.html)
题意：
　　给出两个数字，输出（1）有多少位是相同的（2）有多少位不在正确的位置上。
思路：
　　扫一遍，统计相同的，并且将两串中不同的数的出现次数分别统计起来，取小者之和就是第2个答案了。
```
1 class Solution {
 2 public:
 3     string getHint(string secret, string guess) {
 4         const int N=10;
 5         int *cnt1=new int[N];memset(cnt1,0,sizeof(int)*N);
 6         int *cnt2=new int[N];memset(cnt2,0,sizeof(int)*N);
 7            int    right=0, wrong=0;
 8         for(int i=0; i<secret.size(); i++)
 9         {
10             if(secret[i]==guess[i])    right++;
11             else    
12             {
13                 cnt1[secret[i]-'0']++;
14                 cnt2[guess[i]-'0']++;
15             }
16         }
17         for(int i=0; i<N; i++)    
18             wrong+=min(cnt1[i],cnt2[i]);
19         return to_string(right)+"A"+to_string(wrong)+"B";
20     }
21 };
AC代码
```

