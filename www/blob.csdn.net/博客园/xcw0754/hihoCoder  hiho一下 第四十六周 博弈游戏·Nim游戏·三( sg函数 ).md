# hihoCoder   hiho一下 第四十六周  博弈游戏·Nim游戏·三( sg函数 ) - xcw0754 - 博客园
# [hihoCoder   hiho一下 第四十六周  博弈游戏·Nim游戏·三( sg函数 )](https://www.cnblogs.com/xcw0754/p/4515129.html)
题意:
　　给出几堆石子数量，每次可以取走一堆中任意数量的石头，也可以将一堆分成两堆，而不取。最后取走者胜。
思路：
　　先规矩地计算出sg值，再对每个数量查SG值就可以了。最后求异或和。和不为0的就是必赢。
SG打表
```
1 #include <bits/stdc++.h>
 2 using namespace std;
 3 const int N=105, limit=20004;
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
23 
24     while(~scanf("%d",&n))
25     {
26         for(int i=0; i<n; i++)    scanf("%d",&a[i]);
27         int ans=0;
28         for(int i=0; i<n; i++)    ans^=sg[a[i]];
29         if(ans)    printf("Alice\n");
30         else    printf("Bob\n");
31     }
32     return 0;
33 }
AC代码
```
打表后找规律：
```
1 #include <stdio.h>
 2 int n, a, ans;
 3 int main(){
 4     scanf("%d",&n);
 5     while(n--){
 6         scanf("%d",&a);
 7         if(a%4==0)    a--;
 8         else if(a%4==3)    a++;
 9         ans^=a;
10     }
11     if(ans)    printf("Alice\n");
12     else    printf("Bob\n");
13     return 0;
14 }
AC代码
```

