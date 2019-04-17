# Power Strings（POJ-2406） - Alex_McAvoy的博客 - CSDN博客





2019年02月26日 18:11:06[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：26
个人分类：[HDU																[字符串处理——单模式匹配(KMP)](https://blog.csdn.net/u011815404/article/category/8698890)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Given two strings a and b we define a*b to be their concatenation. For example, if a = "abc" and b = "def" then a*b = "abcdef". If we think of concatenation as multiplication, exponentiation by a non-negative integer is defined in the normal way: a^0 = "" (the empty string) and a^(n+1) = a*(a^n).

# **Input**

Each test case is a line of input representing s, a string of printable characters. The length of s will be at least 1 and will not exceed 1 million characters. A line containing a period follows the last test case.

# Output

For each s you should print the largest n such that s = a^n for some string a.

# Sample Input

**abcdaaaaababab.**

# Sample Output

**143**


题意：每组数据给出一个字符串，要求输出这个字符串的循环节，输入以 . 为结束标记

思路：KMP 求循环节个数模版题

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
#define N 1000001
#define LL long long
const int MOD=20091226;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;
int next[N];
char p[N];
void getNext(){
    next[0]=-1;

    int len=strlen(p);
    int j=0;
    int k=-1;

    while(j<len) {
        if(k==-1||p[j]==p[k]) {
            k++;
            j++;
            if(p[j]==p[k])
                next[j]=next[k];
            else
                next[j]=k;
        }else{
            k=next[k];
        }
    }
}
int main(){

    while(scanf("%s",p)!=EOF){
        memset(next,0,sizeof(next));
        if(p[0]=='.')
            break;

        int n=strlen(p);
        getNext();

        int len=n-next[n];
        if(n%len==0)
            printf("%d\n",n/len);
        else
            printf("1\n");
    }
    return 0;
}
```






