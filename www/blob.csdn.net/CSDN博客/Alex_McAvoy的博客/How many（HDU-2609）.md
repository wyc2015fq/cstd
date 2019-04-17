# How many（HDU-2609） - Alex_McAvoy的博客 - CSDN博客





2019年02月27日 20:06:34[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：19
个人分类：[HDU																[字符串处理——最大最小表示法](https://blog.csdn.net/u011815404/article/category/8706084)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Give you n ( n < 10000) necklaces ,the length of necklace will not large than 100,tell me 

How many kinds of necklaces total have.(if two necklaces can equal by rotating ,we say the two necklaces are some). 

For example 0110 express a necklace, you can rotate it. 0110 -> 1100 -> 1001 -> 0011->0110. 

# **Input**

The input contains multiple test cases. 

Each test case include: first one integers n. (2<=n<=10000) 

Next n lines follow. Each line has a equal length character string. (string only include '0','1'). 

# Output

 For each test case output a integer , how many different necklaces.

# Sample Input

**4011011001001001141010010110000001**

# Sample Output

**12**


题意：每组数据给出 n 个可循环字符串，问有多少种不同的字符串

思路：用最小表示法或最大表示法将所有字符串同化，然后放入 set 容器判重

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
int minmumRepresentation(char *str){//最小表示法
    int len=strlen(str);
    int i=0;//指向字符串最小的位置
    int j=1;//比较指针
    int k=0;//str[i]与str[j]相等时一次移动几个
    while(i<len&&j<len&&k<len){
        int temp=str[(i+k)%len]-str[(j+k)%len];//比较值的长度
        if(temp==0)
            k++;
        else{
            if(temp>0)//维护i
                i=i+k+1;
            else//维护j
                j=j+k+1;
            if(i==j)//相等时比较指针后移
                j++;
            k=0;
        }
    }
    return i<j?i:j;//返回i、j中较小的那个
}
char str[N],temp[N];
int main(){
    int n;
    while(scanf("%d",&n)!=EOF){
        set<string> S;
        for(int i=0;i<n;i++){
            scanf("%s",str);
            int index=minmumRepresentation(str);//最小表示法的索引
            int len=strlen(str);
            for(int j=0;j<len;j++)
                temp[j]=str[(index+j)%len];
            temp[len]='\0';
            S.insert(temp);
        }
        printf("%d\n",S.size());
    }
    return 0;
}
```






