# The Cow Lexicon（POJ-3267） - Alex_McAvoy的博客 - CSDN博客





2018年06月24日 22:45:25[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：40
个人分类：[POJ																[动态规划——区间 DP](https://blog.csdn.net/u011815404/article/category/7813197)](https://blog.csdn.net/u011815404/article/category/7504500)








# Problem Description

Few know that the cows have their own dictionary with W (1 ≤ W ≤ 600) words, each containing no more 25 of the characters 'a'..'z'. Their cowmunication system, based on mooing, is not very accurate; sometimes they hear words that do not make any sense. For instance, Bessie once received a message that said "browndcodw". As it turns out, the intended message was "browncow" and the two letter "d"s were noise from other parts of the barnyard.

The cows want you to help them decipher a received message (also containing only characters in the range 'a'..'z') of length L (2 ≤ L ≤ 300) characters that is a bit garbled. In particular, they know that the message has some extra letters, and they want you to determine the smallest number of letters that must be removed to make the message a sequence of words from the dictionary.



# **Input**

Line 1: Two space-separated integers, respectively: W and L 

Line 2: L characters (followed by a newline, of course): the received message 

Lines 3..W+2: The cows' dictionary, one word per line

# Output

Line 1: a single integer that is the smallest number of characters that need to be removed to make the message a sequence of dictionary words.


# Sample Input

**6 10**

**browndcodwcowmilkwhiteblackbrownfarmer**

# Sample Output

**2**

——————————————————————————————————————————————————————————————————————————————————————————

题意：给出一个长度为l的字符串，与w个词，问最少删掉字符串中的几个字母后可使字符串全部由所给的单词组成。



思路：



用 f[i]=n 来表示字符串删掉n个字母后剩下的串可由单词构成，易得：

若不存在单词可以与字符串str中str[i]开头的字符串匹配，则有：f[i]=f[i+1]+1

若存在单词可以匹配，且str[t-1]为可以匹配的字符串的最后一个字符，匹配的单词的长度为n，则有：f[i]=min(f[i+1]+1,f[t]+t-i-n)

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<string>
#include<cstdlib>
#include<queue>
#include<set>
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 1001
#define MOD 123
#define E 1e-6
using namespace std;
string str;
string word[N];
int f[N];
int main()
{


    int w,l;
    scanf("%d%d",&w,&l);
        cin>>str;
    for(int i=0;i<w;i++)
        cin>>word[i];

    f[l]=0;
    for(int i=l-1;i>=0;i--)
    {
        f[i]=f[i+1]+1;
        for(int j=0;j<w;j++)
        {
            if(word[j].length()<=l-i && word[j].at(0)==str[i])//当第j个单词的首字母与字符串中匹配时
            {
                int t=i;
                int n=0;
                while(t<l)
                {
                    if(word[j].at(n)==str[t++])//匹配单词第n个字母是否与字符串匹配
                        n++;
                    if(n==word[j].length())
                    {
                        f[i]=min(f[t]+t-i-n,f[i]);
                        break;
                    }
                }
            }
        }
    }
    printf("%d\n",f[0]);
}
```






