# HDU  3032  Nim or not Nim? （Nim，sg函数） - xcw0754 - 博客园
# [HDU  3032  Nim or not Nim? （Nim，sg函数）](https://www.cnblogs.com/xcw0754/p/4515109.html)
题意:给出几堆石子数量，每次可以取走一堆中任意数量的石头，也可以将一堆分成两堆，而不取。最后取走者胜。
思路：石子数量很大，不能直接算，sg打表找出规律：正常情况下a[i]=i，但是有例外的，就是i%4=0和i%4=3的sg值是交换了的，所以要算某个状态的sg值时，若模4为0，则进行自减，若模4为3则进行自加，这样就得到了sg值。最后再求全部异或和。若0，则先手输。否则先手胜。
```
1 #include <bits/stdc++.h>
 2 using namespace std;
 3 const int N=105, limit=1004;
 4 int a[N],n,sg[limit]={0,1,2};
 5 bool B[limit];
 6 int main()
 7 {
 8     //freopen("input.txt", "r", stdin);
 9     for(int i=3; i<limit; i++)
10     {
11         memset(B,0,sizeof(B));
12         for(int j=0; j<i; j++)    B[sg[j]]=1;
13         int up=i/2;
14         for(int j=1; j<=up; j++)
15             B[sg[j]^sg[i-j]]=1;
16         for(int j=0;; j++)
17             if(!B[j])
18             {
19                 sg[i]=j;
20                 break;
21             }
22     }
23     for(int i=0; i<100; i++)
24         cout<<sg[i]<<" ";
25     cout<<endl;
26     return 0;
27 }
打表代码
```
```
1 #include <stdio.h>
 2 int a, n, t, ans, i;
 3 int main()
 4 {
 5     scanf("%d",&t);
 6     while(t--)
 7     {
 8         scanf("%d",&n);ans=0;
 9         while(n--)
10         {
11             scanf("%d",&a);
12             if(a%4==0)    --a;
13             else if(a%4==3)    ++a;
14             ans^=a;
15         }
16         if(ans)    printf("Alice\n");
17         else    printf("Bob\n");
18     }
19     return 0;
20 }
AC代码
```

