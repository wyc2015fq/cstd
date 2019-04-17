# Seek the Name, Seek the Fame（POJ-2752） - Alex_McAvoy的博客 - CSDN博客





2019年02月27日 09:33:31[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：21
个人分类：[POJ																[字符串处理——单模式匹配(KMP)](https://blog.csdn.net/u011815404/article/category/8698890)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

The little cat is so famous, that many couples tramp over hill and dale to Byteland, and asked the little cat to give names to their newly-born babies. They seek the name, and at the same time seek the fame. In order to escape from such boring job, the innovative little cat works out an easy but fantastic algorithm: 

Step1. Connect the father's name and the mother's name, to a new string S. 

Step2. Find a proper prefix-suffix string of S (which is not only the prefix, but also the suffix of S). 

Example: Father='ala', Mother='la', we have S = 'ala'+'la' = 'alala'. Potential prefix-suffix strings of S are {'a', 'ala', 'alala'}. Given the string S, could you help the little cat to write a program to calculate the length of possible prefix-suffix strings of S? (He might thank you by giving your baby a name:) 

# **Input**

The input contains a number of test cases. Each test case occupies a single line that contains the string S described above. 

Restrictions: Only lowercase letters may appear in the input. 1 <= Length of S <= 400000. 

# Output

For each test case, output a single line with integer numbers in increasing order, denoting the possible length of the new baby's name.

# Sample Input

**ababcababababcababaaaaa**

# Sample Output

**2 4 9 181 2 3 4 5**


题意：给出一个字符串，问字符串中存在多少子串，使得这些子串既是字符串的前缀，又是字符串的后缀，要求从小到大依次输出这些子串的长度。

思路：根据 next 数组的定义，可以得到前缀 next[len] 长度的子串与后缀 next[len] 长度的子串相同，因此求出 len 处的 next 值，然后向下递归即可得到答案

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
#define N 400001
#define LL long long
const int MOD=20091226;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;

int next[N];
char p[N];
int sum[N];
void getNext(){

    next[0]=-1;

    int len=strlen(p);
    int j=0;
    int k=-1;

    while(j<len){
        if(k==-1||p[j]==p[k]) {
            k++;
            j++;
            next[j]=k;
        }else{
            k=next[k];
        }
    }
}
int main(){

    while(scanf("%s",p)!=EOF){
        int len=strlen(p);
        getNext();

        int i=len;
        int cnt=0;
        while(i){//递归求出next值
            sum[cnt++]=next[i];
            i=next[i];
        }
        for(int i=cnt-2;i>=0;i--)
            printf("%d ",sum[i]);
        printf("%d\n",len);
    }
    return 0;
}
```






