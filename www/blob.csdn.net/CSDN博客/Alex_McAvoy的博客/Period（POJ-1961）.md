# Period（POJ-1961） - Alex_McAvoy的博客 - CSDN博客





2019年02月26日 21:58:40[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：124
个人分类：[POJ																[字符串处理——单模式匹配(KMP)](https://blog.csdn.net/u011815404/article/category/8698890)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

For each prefix of a given string S with N characters (each character has an ASCII code between 97 and 126, inclusive), we want to know whether the prefix is a periodic string. That is, for each i (2 <= i <= N) we want to know the largest K > 1 (if there is one) such that the prefix of S with length i can be written as A K , that is A concatenated K times, for some string A. Of course, we also want to know the period K. 

# **Input**

The input file consists of several test cases. Each test case consists of two lines. The first one contains N (2 <= N <= 1 000 000) – the size of the string S. The second line contains the string S. The input file ends with a line, having the number zero on it. 

# Output

For each test case, output “Test case #” and the consecutive test case number on a single line; then, for each prefix with length i that has a period K > 1, output the prefix size i and the period K separated by a single space; the prefix sizes must be in increasing order. Print a blank line after each test case. 

# Sample Input

**3aaa12aabaabaabaab0**

# Sample Output

**Test case #12 23 3**

**Test case #22 26 29 312 4**


题意：每组给出一个长度为 n 的字符串，要求输出这个字符串每个前缀的最短循环节，简单来说，对于这个字符串的第 i 个前缀 ，求一个最大的整数，使得这个前缀是由某个字符串重复 k 次得到的，输出所有存在这个 k 值的 i 与对应的 k

例如：aabaabaabaab
- 当 i=2 时，前缀为 aa，是由于 a 重复两次得到的，故有 k=2
- 当 i=6 时，前缀为 aabaab，是由于 aab 重复两次得到的，故有 k=2
- 当 i=9 时，前缀为 aabaabaab，是由于 a 重复三次得到的，故有 k=3
- 当 i=12 时，前缀为 aabaabaabaab ，是由于 a 重复四次得到的，故有 k=4

思路：

本质上还是求最小循环节长度，使用 KMP 的 next 数组即可解决，对于每组数据的字符串，枚举前缀 i ，当 i%(i-next[i])=0 时即存在循环节，其个数 k=i/(i-next[i]) 

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
#define N 100501
#define LL long long
const int MOD=20091226;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;

int Next[N];
char p[N];
int n;
void getNext(){

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

    int Case=1;
    while(scanf("%d",&n)!=EOF&&n){
        scanf("%s",p);
        getNext();

        printf("Test case #%d\n",Case++);
        for(int i=1;i<=n;i++){
            int len=i-Next[i];
            if((i%len==0) && Next[i]>0)
                printf("%d %d\n",i,i/len);
        }
        printf("\n");
    }
    return 0;
}
```






