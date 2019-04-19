# codeforces 165-C. Another Problem on Strings（计数+尺取） - HJ - CSDN博客
2017年07月16日 18:43:57[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：279
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
A string is binary, if it consists only of characters “0” and “1”.
String v is a substring of string w if it has a non-zero length and can be read starting from some position in string w. For example, string “010” has six substrings: “0”, “1”, “0”, “01”, “10”, “010”. Two substrings are considered different if their positions of occurrence are different. So, if some string occurs multiple times, we should consider it the number of times it occurs.
You are given a binary string s. Your task is to find the number of its substrings, containing exactly k characters “1”. 
Input
The first line contains the single integer k (0 ≤ k ≤ 106). The second line contains a non-empty binary string s. The length of s does not exceed 106 characters. 
Output
Print the single number — the number of substrings of the given string, containing exactly k characters “1”.
Please do not use the %lld specifier to read or write 64-bit integers in С++. It is preferred to use the cin, cout streams or the %I64d specifier. 
Examples 
Input
1 
1010
Output
6
Input
2 
01010
Output
4
Input
100 
01010
Output
0
Note
In the first sample the sought substrings are: “1”, “1”, “10”, “01”, “10”, “010”.
In the second sample the sought substrings are: “101”, “0101”, “1010”, “01010”.
题目大意： 
给一个只有0和1组成的字符串，问这个字符串中最多有多少个子串满足各字符的和等于k ？
解题思路：
当k=0的时候，每次统计相邻的0的个数res，累加res*（res+1）/2即可；
当k>0的时候，尺取统计每个满足条件的最小子串，在统计的过程中，左边的0的个数用a表示，右边的0的个数用b表示，然后每次累加a*b即可；
```cpp
#include<iostream>
#include<cstring>
using namespace std;
#define maxn 1000005
char s[maxn];
int main()
{
    int k;
    while(cin>>k)
    {
        s[0]='\0';
        cin>>s;
        long long len=strlen(s),a=0,b=0,l=0,r=0,cnt=0,blag=0,ans=0;
        if(k==0)             //需要分出来讨论 
        {
            for(int i=0;i<len;i++)
            {
                if(s[i]=='0')
                {
                    cnt++;              //计算相邻的'0'的个数 
                    blag=0;
                }
                else
                {
                    if(blag)
                      continue;
                    ans+=cnt*(cnt+1)/2;           //因为cnt个相邻的0构成的串有cnt*(cnt-1)/2个子串 
                    cnt=0;
                    blag=1;
                }
            }
            ans+=(s[len-1]-'0'==0)?cnt*(cnt+1)/2:0;         //累加最后没有记上去的cnt的值 
            cout<<ans<<endl;
            continue;
        }
        while(l<len&&r<len)
        {
            while(r<len&&cnt+s[r]-'0'<=k)    //每次寻找满足条件的子串s'   
            {
                cnt+=s[r]-'0';
                b+=(cnt==k)?1:0;         //并且统计子串s'到右边第一个'1'之间'0'的个数加一  用b表示 
                r++;
            }
            if(cnt<k)
              break;
            while(l<len&&l<=r)         //子串改变 l往前移动 直到碰见'1' 
            {
                a++;                     //统计两个'1'之间0的个数加一 
                if(s[l]=='1')
                  break;
                l++;
            }
            ans+=a*b;                   //每次累加a*b的值   
            a=b=0;
            cnt--;
            l++;
        }
        cout<<ans<<endl;
    }
    return 0;
}
```
