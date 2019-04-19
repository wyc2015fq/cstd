# codeforces 676-C. Vasya and String（尺取） - HJ - CSDN博客
2017年07月16日 18:29:39[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：243
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
High school student Vasya got a string of length n as a birthday present. This string consists of letters ‘a’ and ‘b’ only. Vasya denotes beauty of the string as the maximum length of a substring (consecutive subsequence) consisting of equal letters.
Vasya can change no more than k characters of the original string. What is the maximum beauty of the string he can achieve? 
Input
The first line of the input contains two integers n and k (1 ≤ n ≤ 100 000, 0 ≤ k ≤ n) — the length of the string and the maximum number of characters to change.
The second line contains the string, consisting of letters ‘a’ and ‘b’ only. 
Output
Print the only integer — the maximum beauty of the string Vasya can achieve by changing no more than k characters. 
Examples 
Input
4 2 
abba
Output
4
Input
8 1 
aabaabaa
Output
5
Note
In the first sample, Vasya can obtain both strings “aaaa” and “bbbb”.
In the second sample, the optimal answer is obtained with the string “aaaaabaa” or with the string “aabaaaaa”.
题目大意： 
 一个字符串只包含a和b，可以改变其中k个字符。求最大的美丽值。
（美丽值定义为子串中所含字符均相同的的最长子串的长度）
解题思路： 
通过尺取，分别把k个字符全部改成a和全部改成b的最大美丽值求出来，然后比较一下大小即可。
```cpp
#include<iostream>
using namespace std;
#define maxn 100005
char s[maxn];
int n,k;
int cal(char x)
{
    int l=0,r=0,cnt=0,ans=0;
    while(l<n&&r<n)
    {
        while((s[r]==x||cnt<k)&&r<n)
        {
            if(s[r]!=x)
              cnt++;
            r++;
        }
        ans=max(ans,r-l);
        while(s[l]==x&&l<=r)
          l++;
        l++;
        cnt--;
    }
    return ans;
}
int main()
{
    while(cin>>n>>k)
    {
        cin>>s;
        int ans=max(cal('a'),cal('b'));
        cout<<ans<<endl;
    }
    return 0;
}
```
