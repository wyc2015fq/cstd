# Count the string（HDU-3336） - Alex_McAvoy的博客 - CSDN博客





2019年02月27日 18:57:07[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：33
个人分类：[HDU																[字符串处理——单模式匹配(KMP)](https://blog.csdn.net/u011815404/article/category/8698890)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

It is well known that AekdyCoin is good at string problems as well as number theory problems. When given a string s, we can write down all the non-empty prefixes of this string. For example: 

s: "abab" 

The prefixes are: "a", "ab", "aba", "abab" 

For each prefix, we can count the times it matches in s. So we can see that prefix "a" matches twice, "ab" matches twice too, "aba" matches once, and "abab" matches once. Now you are asked to calculate the sum of the match times for all the prefixes. For "abab", it is 2 + 2 + 1 + 1 = 6. 

The answer may be very large, so output the answer mod 10007. 

# **Input**

The first line is a single integer T, indicating the number of test cases. 

For each case, the first line is an integer n (1 <= n <= 200000), which is the length of string s. A line follows giving the string s. The characters in the strings are all lower-case letters. 

# Output

For each case, output only one number: the sum of the match times for all the prefixes of s mod 10007.

# Sample Input

**14abab**

# Sample Output

**6**


题意：给出 t 组数据，每组给出一个长度为 n 的字符串，对于这个字符串的每个前缀，求其在这个字符串中的匹配次数之和，最后结果 mod 10007

思路：

关键是对 next 数组的理解，next[i] 记录当前位置之前前缀后缀相同的个数，计算出 next 数组之后枚举加和，最后再加上 n 个自身的一个匹配即可

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>
#include<ctime>
#include<algorithm>
#include<utility>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-9
#define INF 0x3f3f3f3f
#define LL long long
const int MOD=10007;
const int N=200000+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;

int Next[N];
char str[N];
int num[N];
void getNext(char p[]){
    Next[0]=-1;
    int len=strlen(p);
    int j=0;
    int k=-1;

    while(j<len){
        if(k==-1||p[j]==p[k]) {
            k++;
            j++;
            Next[j]=k;
        }else{
            k=Next[k];
        }
    }
}

int main(){
    int t;
    scanf("%d",&t);
    while(t--){
        memset(num,0,sizeof(num));
        int n;
        scanf("%d",&n);
        scanf("%s",str);
        getNext(str);

        for(int i=1;i<=n;i++)//统计next[i]对应的次数
            num[Next[i]]++;

        int res=0;
        for(int i=1;i<=n;i++)
            if(num[i]>0)
                res=(res+num[i])%MOD;

        res=(res+n)%MOD;
        printf("%d\n",res);
    }
    return 0;
}
```






