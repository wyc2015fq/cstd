# codeforces-757-B Bash's Big Day（简单题） - HJ - CSDN博客
2017年01月25日 18:00:13[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：555
个人分类：[====ACM====																[codeforce](https://blog.csdn.net/feizaoSYUACM/article/category/6353637)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
Bash has set out on a journey to become the greatest Pokemon master. To get his first Pokemon, he went to Professor Zulu’s Lab. Since Bash is Professor Zulu’s favourite student, Zulu allows him to take as many Pokemon from his lab as he pleases.
But Zulu warns him that a group of k > 1 Pokemon with strengths {s1, s2, s3, …, sk} tend to fight among each other if gcd(s1, s2, s3, …, sk) = 1 (see notes for gcd definition).
Bash, being smart, does not want his Pokemon to fight among each other. However, he also wants to maximize the number of Pokemon he takes from the lab. Can you help Bash find out the maximum number of Pokemon he can take?
Note: A Pokemon cannot fight with itself. 
Input
The input consists of two lines.
The first line contains an integer n (1 ≤ n ≤ 105), the number of Pokemon in the lab.
The next line contains n space separated integers, where the i-th of them denotes si (1 ≤ si ≤ 105), the strength of the i-th Pokemon. 
Output
Print single integer — the maximum number of Pokemons Bash can take. 
Examples 
Input
3 
2 3 4
Output
2
Input
5 
2 3 4 6 7
Output
3
Note
gcd (greatest common divisor) of positive integers set {a1, a2, …, an} is the maximum positive integer that divides all the integers {a1, a2, …, an}.
In the first sample, we can take Pokemons with strengths {2, 4} since gcd(2, 4) = 2.
In the second sample, we can take Pokemons with strengths {2, 4, 6}, and there is no larger group with gcd ≠ 1.
题目大意： 
    给出一组数，求出含有公约数非1个数最多的结果
解题思路： 
   结合筛法求素数，因为数字也不是很大，可以用数组下标存数，下标值累加个数。
```cpp
#include<iostream>
#include<cstring>
using namespace std;
int s[100005],b[100005];
int main()
{
    int n;
    while(cin>>n)
    {
        memset(b,0,sizeof(b));
        int maxn=0,ans=1;
        for(int i=0;i<n;i++)
        {
            cin>>s[i];
            b[s[i]]++;
            maxn=max(maxn,s[i]);
        }
        for(int i=2;i<=maxn;i++)
        {
            int sum=0;
            for(int j=i;j<=maxn;j+=i)   //类似于筛法求和
              sum+=b[j];
            ans=max(ans,sum);
        }
        cout<<ans<<endl;
    }
    return 0;
}
```
