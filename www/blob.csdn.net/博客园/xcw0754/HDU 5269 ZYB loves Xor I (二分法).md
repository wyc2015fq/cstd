# HDU 5269 ZYB loves Xor I (二分法) - xcw0754 - 博客园
# [HDU 5269 ZYB loves Xor I (二分法)](https://www.cnblogs.com/xcw0754/p/4574384.html)
题意：
　　给出一个序列，对每两个数求异或结果后取最低位的1出来作为一个数，然后求这些数字的和。比如：｛a，b，c｝，结果是lowbit(a^b)+lowbit(a^c)+lowbit(b^a)+lowbit(b^c)+lowbit(c^a)+lowbit(c^b)。若不剔除结果为0的，应该有n*n个数的和作为结果。
思路：
　　试考虑二分法。
　　观察到可能的取值 lowbit[a]=1，2，4，8....。也就是说最多有29种，结果就是ans=C1*1+C2*2+C3*4+C4*8...。C为个数。可以从这方面下手。
　　首先序列seq有n个元素，以二进制第1位的不同，将序列分成左右两个集合。那么结果lowbit[]=1的系数C1就是左边个数*右边个数。现在左边集合中的元素的第1位都是0了。接下来要做的就是递归分别处理左边和右边的集合。而a^a肯定是0，不用考虑，也就是数本身不考虑，相等的数也不考虑。主要是复杂度的分析：当序列为自然数序列时，DFS的过程形成了一棵满二叉树，有2*n-1个节点；当数比较集中时，递归层数较多，复杂度O(nlogn)。
```
1 #include <bits/stdc++.h>
 2 #define LL long long
 3 using namespace std;
 4 const int N=50005;
 5 const int mod=998244353;
 6 int ans;
 7 void cal(deque<int> &tmp, int num)
 8 {
 9     deque<int> tmp0,tmp1;
10     int q=0;
11     while(!tmp.empty())
12     {
13         q=tmp.front();
14         tmp.pop_front();
15         if((q&(1<<num))>0)    tmp1.push_back(q);
16         else    tmp0.push_back(q);
17     }
18 
19     ans= (ans+(1<<num)*tmp1.size()*tmp0.size())%mod;
20     if(num>=28)    return;     //大于29位的认同为相同
21     if(!tmp1.empty()&&tmp1.size()>1)
22         cal(tmp1,num+1);
23     if(!tmp0.empty()&&tmp0.size()>1)
24         cal(tmp0,num+1);
25 
26 }
27 
28 int main()
29 {
30     //freopen("input.txt", "r", stdin);
31     int t, j=0, n, a;
32     deque<int> que;
33     cin>>t;
34     while(t--)
35     {
36         que.clear();
37         scanf("%d",&n);
38         for(int i=0; i<n; i++)
39         {
40             scanf("%d",&a);
41             que.push_back(a);
42         }
43         ans=0;
44         cal(que, 0);
45         printf("Case #%d: %d\n",++j,ans*2%mod);
46     }
47     return 0;
48 }
49 
50 AC代码
AC代码
```

