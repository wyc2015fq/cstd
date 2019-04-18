# SPOJ  SORTBIT  Sorted bit squence （数位DP，入门） - xcw0754 - 博客园
# [SPOJ  SORTBIT  Sorted bit squence （数位DP，入门）](https://www.cnblogs.com/xcw0754/p/4852036.html)
题意：
　　给出一个范围[m,n]，按照二进制表示中的1的个数从小到大排序，若1的个数相同，则按照十进制大小排序。求排序后的第k个数。注意：m*n>=0。
思路：
　　也是看论文的。一开始也能想到是这种解法，枚举0~31个1，逐步缩小第k个数的范围（其实就是找到第k个数应该有几个1），然后二分答案，直到找到第k个数。
　　我只是在找第k个数时不是二分答案，而是想直接从最高位往低位走，判断左子树中满足条件的数的数量，然后控制往下一位应该是0还是1（即往树的哪一个孩子方向走，直到根）。其实也是二分思想。
　　这题明显只有两个范围:[-INF,0]或者[0,INF]，要特别注意n=0或者m=0的情况，有可能第k个数就是0，否则，是不是0就没有什么影响了。
```
1 //#include <bits/stdc++.h>
  2 #include <iostream>
  3 #include <cstdio>
  4 #include <cstring>
  5 #include <cmath>
  6 #include <map>
  7 #include <algorithm>
  8 #include <vector>
  9 #include <iostream>
 10 #define pii pair<int,int>
 11 #define INF 0x7f3f3f3f
 12 #define LL long long
 13 using namespace std;
 14 const double PI  = acos(-1.0);
 15 const int N=35; //注意大小
 16 
 17 int f[N][N], bit[N], m, n, k;;
 18 void pre_cal()  //预处理组合数
 19 {
 20     f[0][0]=1;
 21     for(int i=1; i<N; i++) //位数
 22     {
 23         f[i][0]=f[i][i]=1;
 24         for(int j=1; j<i; j++) //多少个1
 25         {
 26             f[i][j]=f[i-1][j]+f[i-1][j-1];
 27         }
 28     }
 29 }
 30 
 31 int cal(int n,int k,int b)
 32 {
 33     memset(bit, 0, sizeof(bit));
 34     int len=0, cnt=0, ans=0;
 35     while(n)    //转成b进制
 36     {
 37         bit[++len]=n%b;
 38         n/=b;
 39     }
 40     for(int i=len; i>0; i--)
 41     {
 42         if( bit[i]==1 )
 43         {
 44             ans+=f[i-1][k-cnt]; //统计左边的
 45             if(++cnt>k)   break;  //已超
 46         }
 47     }
 48     if(cnt==k)  ans++;
 49     return ans;
 50 }
 51 
 52 
 53 int get_ans(int m,int n,int k)
 54 {
 55     int i, num;
 56     for(i=0; i<=31; i++)    //枚举位数
 57     {
 58         num=cal(n,i,2)-cal(m-1,i,2);
 59         if(k-num<=0)    break;
 60         else   k-=num;
 61     }
 62     int L=m,R=n;
 63     while( L<R )            //二分答案
 64     {
 65         int mid=R-(R-L+1)/2;
 66         num=cal(mid,i,2)-cal(m-1,i,2);
 67         if( num<k ) L=mid+1;
 68         else        R=mid;  //如果等于，也是继续缩小范围的
 69     }
 70     return R;
 71 }
 72 
 73 
 74 int main()
 75 {
 76     //freopen("input.txt","r",stdin);
 77     pre_cal();
 78     int t;cin>>t;
 79     while(t--)
 80     {
 81         scanf("%d%d%d",&m,&n,&k);
 82         if(m<0)
 83         {
 84             m^=(1<<31); //改为正
 85             if(n==0)    //上界为0
 86             {
 87                 n--;
 88                 n^=(1<<31);
 89                 if(get_ans(m,n,k-1)==n) printf("0\n");
 90                 else cout<<(get_ans(m,n,k)^(1<<31))<<endl;
 91             }
 92             else
 93             {
 94                 n^=(1<<31);
 95                 cout<<(get_ans(m,n,k)^(1<<31))<<endl;  //恢复负值
 96             }
 97         }
 98         else
 99         {
100             if(m==0&&k==1)   {printf("0\n");continue;}
101             else if(m==0)    m++,k--;
102             cout<<get_ans(m,n,k)<<endl;
103         }
104     }
105     return 0;
106 }
AC代码
```

