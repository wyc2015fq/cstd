# A Magic Lamp（HDU-3183） - Alex_McAvoy的博客 - CSDN博客





2019年02月15日 18:48:15[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：43
个人分类：[HDU																[数据结构——RMQ](https://blog.csdn.net/u011815404/article/category/8671578)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Kiki likes traveling. One day she finds a magic lamp, unfortunately the genie in the lamp is not so kind. Kiki must answer a question, and then the genie will realize one of her dreams. 

The question is: give you an integer, you are allowed to delete exactly m digits. The left digits will form a new integer. You should make it minimum. 

You are not allowed to change the order of the digits. Now can you help Kiki to realize her dream? 

# Input

There are several test cases. 

Each test case will contain an integer you are given (which may at most contains 1000 digits.) and the integer m (if the integer contains n digits, m will not bigger then n). The given integer will not contain leading zero. 

# Output

For each case, output the minimum result you can get in one line. 

If the result contains leading zero, ignore it. 

# **Sample Input**

**178543 4 1000001 1100001 212345 254321 2**

# Sample Output

**1310123321**


题意：每组数据首先给出一个纯数字的串，然后再给出一个数字 m，现要从这个纯数串中删除 m 位，使得余下的串值最小，输出这个最小的值

思路：RMQ 找区间最小值

要注意处理前导 0，其次当两个值一样时，要输出后面的值

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
#define E 1e-6
#define INF 0x3f3f3f3f
#define N 10001
#define LL long long
const int MOD=20091226;
const int dx[]={-1,1,0,0};
const int dy[]={0,0,-1,1};
using namespace std;
char a[N];
char res[N];
int f[N][20];
int minn(int i,int j){
    if(a[i]<=a[j])
        return i;
    return j;
}
void initMin(int n)
{
    for(int i=0;i<n;i++)
        f[i][0]=i;
    for(int j=1;(1<<j)<=n;j++)
        for(int i=0;i+(1<<j)-1<n;i++)
            f[i][j]=minn(f[i][j-1],f[i+(1<<(j-1))][j-1]);
}
int getMax(int L,int R){
    int k=0;
    while((1<<(k+1))<=R-L+1)
        k++;
    return minn(f[L][k] , f[R-(1<<k)+1][k]);
}


int main(){
    int m;
    while(scanf("%s%d",a,&m)!=EOF){
        memset(res,'\0',sizeof(res));
        int n=strlen(a);
        initMin(n);

        int len=n-m;
        int i=0,cnt=0;
        while(len--){
            i=getMax(i,n-len-1);
            res[cnt++]=a[i++];
        }

        int lead=0;//前导0
        for(int i=0;i<cnt;i++){
            lead=i;
            if(res[i]!='0')
                break;
        }

        if(lead==cnt)
            printf("0\n");
        else
            printf("%s\n",res+lead);
    }
    return 0;
}
```






