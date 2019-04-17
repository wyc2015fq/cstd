# Period II（FZU-1901） - Alex_McAvoy的博客 - CSDN博客





2019年02月27日 11:18:56[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：59
个人分类：[字符串处理——单模式匹配(KMP)																[其它 OJ](https://blog.csdn.net/u011815404/article/category/8820652)](https://blog.csdn.net/u011815404/article/category/8698890)








> 
# Problem Description

For each prefix with length P of a given string S,if

S[i]=S[i+P] for i in [0..SIZE(S)-p-1],

then the prefix is a “period” of S. We want to all the periodic prefixs.

# **Input**

Input contains multiple cases.

The first line contains an integer T representing the number of cases. Then following T cases.

Each test case contains a string S (1 <= SIZE(S) <= 1000000),represents the title.S consists of lowercase ,uppercase letter.

# Output

For each test case, first output one line containing "Case #x: y", where x is the case number (starting from 1) and y is the number of periodic prefixs.Then output the lengths of the periodic prefixs in ascending order.

# Sample Input

**4oooacmacmacmacmacmafzufzufzufstostootssto**

# Sample Output

**Case #1: 31 2 3Case #2: 63 6 9 12 15 16Case #3: 43 6 9 10Case #4: 29 12**


题意：t 组数据，每组给出一个字符串，对于字符串的所有前缀，若存在循环节，输出这个字符串的符合条件的个数与这个字符串的长度

思路：思路与 [Seek the Name, Seek the Fame（POJ-2752）](https://blog.csdn.net/u011815404/article/details/87966382)类似，但输出的不是循环节的长度，而是前缀子串的长度，此外需要统计符合条件的子串个数，可利用队列来统计输出

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
    int t;
    scanf("%d",&t);

    int Case=1;
    while(t--){
        scanf("%s",p);
        int len=strlen(p);
        getNext();

        int i=len;
        queue<int> Q;
        while(i){
            i=Next[i];
            Q.push(i);
        }

        printf("Case #%d: %d\n",Case++,Q.size());
        printf("%d",len-Q.front());
        Q.pop();
        while(!Q.empty()){
            printf(" %d",len-Q.front());
            Q.pop();
        }
        printf("\n");
    }
    return 0;
}
```






