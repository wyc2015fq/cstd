# HDU 5340 Three Palindromes （Manacher） - xcw0754 - 博客园
# [HDU 5340 Three Palindromes （Manacher）](https://www.cnblogs.com/xcw0754/p/4694857.html)
题意：
　　判断是否能将字符串S分成三段非空回文串。
思路：
　　先预处理出前缀回文串和后缀回文串的位置，将位置分别装入两个集合中，O(n)。
　　针对每个前缀回文串的终点位置，挑出不相交的后缀回文串，对中间那段进行暴力匹配即可。只有20个case，不会超时的。
　　具体的算法参考[HIHOCODER HIHO一下 第一周 #1032 : 最长回文子串 (特殊处理)](http://www.cnblogs.com/xcw0754/p/4442652.html)
```
1 //#include <bits/stdc++.h>
  2 #include <cstdio>
  3 #include <cstring>
  4 #include <map>
  5 #include <algorithm>
  6 #include <set>
  7 #include <string>
  8 #include <iostream>
  9 #include <deque>
 10 #include <vector>
 11 #define INF 0x7f7f7f7f
 12 #define pii pair<int,int>
 13 #define LL unsigned long long
 14 using namespace std;
 15 const int N=20100;
 16 int len;            //原串长
 17 char str[N*2];        //接收原来的串
 18 char s[N*2];
 19 int P[N*2];         //保存关于长度的信息（回文长度的一半再加1）
 20 vector<int> vect[2];
 21 int cal(int q)
 22 {
 23     int id=1, mx=1, max1=1;
 24     P[0]=1;
 25     P[1]=1;
 26     for(int i=2; s[i]!='\0'; i++)        //考虑以i为中心的回文串
 27     {
 28         P[i] =i>mx? 1: min( P[2*id-i],mx-i);
 29         while(s[i+P[i]]==s[i-P[i]])     //在这比对
 30         {
 31             if(i-P[i]==1)    vect[q].push_back(i+P[i]); //已经匹配
 32             P[i]++;
 33         }
 34 
 35 
 36         if(i+P[i]>mx)       //更新id和mx的位置
 37         {
 38             id=i;
 39             mx=i+P[i];
 40         }
 41         if(P[i]-1>max1)     //更新最大值
 42             max1=P[i]-1;
 43     }
 44     return max1;
 45 }
 46 
 47 
 48 
 49 bool ok()
 50 {
 51     len=strlen(str);
 52     sort(vect[0].begin(), vect[0].end());
 53     sort(vect[1].begin(), vect[1].end());
 54 
 55     for(int i=0; i<vect[0].size(); i++)//这里虽然穷举了所有可能，但通常都很少
 56     {
 57         for(int j=0; j<vect[1].size(); j++)
 58         {
 59             int l=vect[0][i]+1;
 60             int r=len-vect[1][j]-1;
 61             if(l>r)    break;
 62 
 63             while( l<r && str[l]==str[r] )
 64                 l++,r--;
 65             if(l==r && str[l]==str[r])
 66                 return true;
 67 
 68         }
 69     }
 70     return false;
 71 }
 72 
 73 int main()
 74 {
 75     //freopen("input.txt", "r", stdin);
 76     int t;
 77     cin>>t;
 78     while(t--)
 79     {
 80         scanf("%s",str);
 81         len=strlen(str);
 82         vect[0].clear();
 83         vect[1].clear();
 84         if(len==3)///特别处理
 85         {
 86             puts("YES");
 87             continue;
 88         }
 89 
 90         s[0]='$';
 91         s[1]='#';
 92         int i=0, j=2;
 93         for(; i<len; i++)
 94         {
 95             s[j++]=str[i];
 96             s[j++]='#';
 97         }
 98         s[j]='\0';
 99         strcpy(str,s);//先copy出一份拷贝
100         memset(P, 0, sizeof(P));
101         cal(0);
102 
103         reverse(s,s+2*len+3);//反置过来，求后缀回文
104         s[0]='$';
105         s[2*len+2]='\0';
106         memset(P, 0, sizeof(P));
107         cal(1);
108 
109         if(ok())puts("Yes");
110         else    puts("No");
111     }
112     return 0;
113 }
AC代码
```

