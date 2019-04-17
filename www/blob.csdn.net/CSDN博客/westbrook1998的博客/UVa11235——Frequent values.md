# UVa11235——Frequent values - westbrook1998的博客 - CSDN博客





2018年05月25日 15:02:20[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：35标签：[RMQ																[区间最值](https://so.csdn.net/so/search/s.do?q=区间最值&t=blog)](https://so.csdn.net/so/search/s.do?q=RMQ&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
You are given a sequence of n integers a1, a2, … , an in non-decreasing order. In addition to that, you 

  are given several queries consisting of indices i and j (1 ≤ i ≤ j ≤ n). For each query, determine the 

  most frequent value among the integers ai 

  , … , aj . 

  Input 

  The input consists of several test cases. Each test case starts with a line containing two integers n and 

  q (1 ≤ n, q ≤ 100000). The next line contains n integers a1, … , an (−100000 ≤ ai ≤ 100000, for each 

  i ∈ {1, …, n}) separated by spaces. You can assume that for each i ∈ {1, … , n − 1}: ai ≤ ai+1. The 

  following q lines contain one query each, consisting of two integers i and j (1 ≤ i ≤ j ≤ n), which 

  indicate the boundary indices for the query. 

  The last test case is followed by a line containing a single ‘0’. 

  Output 

  For each query, print one line with one integer: The number of occurrences of the most frequent value 

  within the given range. 

  Note: A naive algorithm may not run in time! 

  Sample Input 

  10 3 

  -1 -1 1 1 1 1 3 10 10 10 

  2 3 

  1 10 

  5 10 

  0 

  Sample Output 

  1 

  4 

  3
一个不递减的数列，多次查询一段区间内出现次数最大的数字的次数 

好难，做了很久

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
int n,q,r,l,R,L,res;
const int MAXN=100050;
//保存原数列
int a[MAXN];
//RMQ的数组，d[i][j]表示从i开始，长度为2^j的一段元素中的最大值，这些元素是Count数组
int d[MAXN][20];
//记录第i个段(即相同数字)的个数
//Count[i]=j 表示第i个段元素个数为j
int Count[MAXN];
//num[i]=k 表示原数组(a)的i位置的元素属于第k个段
int num[MAXN];
//left[L]=i 表示第L个段的左端点为原数组的i位置
int left[MAXN];
//right[R]=j 表示第R个段的右端点为原数组的j位置
int right[MAXN];
//value[i]=j 表示第i个段的数字是j
int value[MAXN];

//RMQ预处理Count数组
void RMQ_init(int n){
    for(int i=1;i<=n;i++){
        d[i][0]=Count[i];
    }
    for(int j=1;(1<<j)<=n;j++){
        for(int i=0;i+(1<<j)-1<n;i++){
            d[i][j]=max(d[i][j-1],d[i+(1<<(j-1))][j-1]);
        }
    }
    /*
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            printf("%d ",d[i][j]);
        }
        printf("\n");
    }
    */
}

//RMQ查询
int RMQ(int L,int R){
    //特判
    if(L>R){
        return 0;
    }
    int k=0;
    while(1<<(k+1)<=R-L+1){
        k++;
    }
    return max(d[L][k],d[R-(1<<k)+1][k]);
}
int main(void){
    while(~scanf("%d",&n)){
        if(0==n){
            break;
        }
        memset(a,0,sizeof(a));
        memset(d,0,sizeof(d));  
        memset(Count,0,sizeof(Count));
        memset(num,0,sizeof(num));
        memset(left,0,sizeof(left));
        memset(right,0,sizeof(right));
        memset(value,0,sizeof(value));
        scanf("%d",&q);
        a[0]=-1*MAXN;
        //区间索引
        int k=0;
        for(int i=1;i<=n;i++){
            scanf("%d",&a[i]);
            //直接边输入边维护几个数组
            if(a[i]==a[i-1]){
                value[k]==a[i];
                Count[k]++;
                num[i]=k;
                right[k]++;
            }
            else{
                k++;
                value[k]=a[i];
                Count[k]=1;
                num[i]=k;
                left[k]=i;  
                right[k]=i;
            }
        }
        RMQ_init(k);
        /*
        printf("value\n");
        for(int i=1;i<=n;i++){
            printf("%d ",value[i]);
        }
        printf("\n");
        printf("Count\n");
        for(int i=1;i<=n;i++){
            printf("%d ",Count[i]);
        }
        printf("\n");
        printf("num\n");
        for(int i=1;i<=n;i++){
            printf("%d ",num[i]);
        }
        printf("\n");
        printf("left\n");
        for(int i=1;i<=n;i++){
            printf("%d ",left[i]);
        }
        printf("\n");
        printf("right\n");
        for(int i=1;i<=n;i++){
            printf("%d ",right[i]);
        }
        printf("\n");
        */
        for(int i=0;i<q;i++){
            scanf("%d%d",&l,&r);
            L=num[l];
            R=num[r];
            //L和R在同一段
            if(L==R){
                res=r-l+1;
            }
            else{
                /*
                printf("L %d R %d\n",L,R);
                printf("l %d r %d\n",l,r);
                printf("%d\n",RMQ(L,R));
                printf("%d %d\n",right[L]-l+1,r-left[R]+1);
                */
                res=max(RMQ(L+1,R-1),max(right[L]-l+1,r-left[R]+1));
            }
            printf("%d\n",res);
        }
    }
    return 0;
}
```






