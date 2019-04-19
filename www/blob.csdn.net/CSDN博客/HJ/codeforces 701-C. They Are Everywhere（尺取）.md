# codeforces 701-C. They Are Everywhere（尺取） - HJ - CSDN博客
2017年07月12日 10:59:19[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：239
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
Sergei B., the young coach of Pokemons, has found the big house which consists of n flats ordered in a row from left to right. It is possible to enter each flat from the street. It is possible to go out from each flat. Also, each flat is connected with the flat to the left and the flat to the right. Flat number 1 is only connected with the flat number 2 and the flat number n is only connected with the flat number n - 1.
There is exactly one Pokemon of some type in each of these flats. Sergei B. asked residents of the house to let him enter their flats in order to catch Pokemons. After consulting the residents of the house decided to let Sergei B. enter one flat from the street, visit several flats and then go out from some flat. But they won’t let him visit the same flat more than once.
Sergei B. was very pleased, and now he wants to visit as few flats as possible in order to collect Pokemons of all types that appear in this house. Your task is to help him and determine this minimum number of flats he has to visit. 
Input
The first line contains the integer n (1 ≤ n ≤ 100 000) — the number of flats in the house.
The second line contains the row s with the length n, it consists of uppercase and lowercase letters of English alphabet, the i-th letter equals the type of Pokemon, which is in the flat number i. 
Output
Print the minimum number of flats which Sergei B. should visit in order to catch Pokemons of all types which there are in the house. 
Examples 
Input
3 
AaA
Output
2
Input
7 
bcAAcbc
Output
3
Input
6 
aaBCCe
Output
5
Note
In the first test Sergei B. can begin, for example, from the flat number 1 and end in the flat number 2.
In the second test Sergei B. can begin, for example, from the flat number 4 and end in the flat number 6.
In the third test Sergei B. must begin from the flat number 2 and end in the flat number 6.
题目大意： 
在一个只含有大小写字母的字符串中找一个最短的包含其所有字母种类的子串，输出最小长度。（注：大小写不相同）
解题思路： 
常规的尺取题目。只需要先预处理转化一下大小写字母，以方便标记就行了。
```cpp
#include<iostream>
#include<cstring>
#include<set>
using namespace std;
#define maxn 1000005
int a[80],s1[maxn];
char s[maxn];
int main()
{
    int n;
    while(cin>>n)
    {
        set<int> m;
        memset(s,0,sizeof(s));
        memset(a,0,sizeof(a));
        cin>>s;
        int cnt,l=0,r=0,ans=n,num=0;
        for(int i=0;i<n;i++)
        {
            if(s[i]>='a'&&s[i]<='z')
              s1[i]=s[i]-'a';
            else if(s[i]>='A'&&s[i]<='Z')
              s1[i]=s[i]-'A'+26;
            m.insert(s1[i]);
        }
        cnt=m.size();
        while(l<n)
        {
            while(r<n&&num<cnt)
            {
                if(a[s1[r]]==0)
                  num++;
                a[s1[r]]++;
                r++;
            }
            if(num<cnt)
              break;
            ans=min(ans,r-l);
            a[s1[l]]--;
            if(a[s1[l]]==0)
              num--;
            l++;
        }
        cout<<ans<<endl;
    }
    return 0;
}
```
