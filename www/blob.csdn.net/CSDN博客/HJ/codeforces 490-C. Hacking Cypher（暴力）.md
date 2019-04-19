# codeforces 490-C. Hacking Cypher（暴力） - HJ - CSDN博客
2017年07月16日 18:49:54[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：184
个人分类：[====ACM====																[---------OJ---------																[codeforce](https://blog.csdn.net/feizaoSYUACM/article/category/6353637)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
Polycarpus participates in a competition for hacking into a new secure messenger. He’s almost won.
Having carefully studied the interaction protocol, Polycarpus came to the conclusion that the secret key can be obtained if he properly cuts the public key of the application into two parts. The public key is a long integer which may consist of even a million digits!
Polycarpus needs to find such a way to cut the public key into two nonempty parts, that the first (left) part is divisible by a as a separate number, and the second (right) part is divisible by b as a separate number. Both parts should be positive integers that have no leading zeros. Polycarpus knows values a and b.
Help Polycarpus and find any suitable method to cut the public key. 
Input
The first line of the input contains the public key of the messenger — an integer without leading zeroes, its length is in range from 1 to 10^6 digits. The second line contains a pair of space-separated positive integers a, b (1 ≤ a, b ≤ 10^8). 
Output
In the first line print “YES” (without the quotes), if the method satisfying conditions above exists. In this case, next print two lines — the left and right parts after the cut. These two parts, being concatenated, must be exactly identical to the public key. The left part must be divisible by a, and the right part must be divisible by b. The two parts must be positive integers having no leading zeros. If there are several answers, print any of them.
If there is no answer, print in a single line “NO” (without the quotes). 
Examples 
Input
116401024 
97 1024
Output
YES 
11640 
1024
Input
284254589153928171911281811000 
1009 1000
Output
YES 
2842545891539 
28171911281811000
Input
120 
12 1
Output
NO
题目大意： 
把一个数分割两部分，要求前半部分能被a整除，后半部分能被b整除。请求出这样的前半部分和后半部分。
解题思路： 
暴力O(|s|)循环一遍，用A[i]表示前i位数%a的结果，B[j]表示后j位数%b的结果
```cpp
#include<iostream>
#include<cstring>  
using namespace std;  
typedef long long LL;  
#define maxn 1000005
char s[maxn];  
LL A[maxn],B[maxn];  
int main()  
{  
    LL a,b;    
    while(cin>>s>>a>>b)
    {  
        LL p=1,l=strlen(s),k,blag=0;    
        A[0]=(s[0]-'0')%a,B[l-1]=(s[l-1]-'0')%b; 
        for(int i=1;i<l;i++) 
          A[i]=(A[i-1]*10+s[i]-'0')%a;  
        for(int i=l-2;i>=0;i--)
        {  
            p=p*10%b;  
            B[i]=(B[i+1]+(s[i]-'0')*p)%b;  
        }   
        for(int i=0;i<l;++i)  
          if(A[i]==0&&i+1<l&&s[i+1]!='0'&&B[i+1]==0)          //满足前i个数%a等于0 后面的数%b也等于0 
          {  
              k=i;
              blag=1;
              break;  
          }
        if(blag)
        {  
            cout<<"YES"<<endl;  
            for(int i=0;i<=k;i++) 
              cout<<s[i]; 
            cout<<endl;
            for(int i=k+1;i<l;i++)
              cout<<s[i];
            cout<<endl;  
        }  
        else
          cout<<"NO"<<endl;
    }
    return 0;  
}
```
