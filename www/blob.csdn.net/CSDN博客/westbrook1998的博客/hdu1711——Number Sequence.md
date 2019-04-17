# hdu1711——Number Sequence - westbrook1998的博客 - CSDN博客





2018年06月24日 20:57:48[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：45标签：[KMP模板题](https://so.csdn.net/so/search/s.do?q=KMP模板题&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
Given two sequences of numbers : a[1], a[2], …… , a[N], and b[1], b[2], …… , b[M] (1 <= M <= 10000, 1 <= N <= 1000000). Your task is to find a number K which make a[K] = b[1], a[K + 1] = b[2], …… , a[K + M - 1] = b[M]. If there are more than one K exist, output the smallest one.  

  Input 

  The first line of input is a number T which indicate the number of cases. Each case contains three lines. The first line is two numbers N and M (1 <= M <= 10000, 1 <= N <= 1000000). The second line contains N integers which indicate a[1], a[2], …… , a[N]. The third line contains M integers which indicate b[1], b[2], …… , b[M]. All integers are in the range of [-1000000, 1000000].  

  Output 

  For each test case, you should output one line which only contain K described above. If no such K exists, output -1 instead.  

  Sample Input 

  2 

  13 5 

  1 2 1 2 3 1 2 3 1 3 2 1 2 

  1 2 3 1 3 

  13 5 

  1 2 1 2 3 1 2 3 1 3 2 1 2 

  1 2 3 2 1 

  Sample Output 

  6 

  -1
KMP的裸题…干掉他 

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int MINN=10050;
const int MAXN=1000050;
int a[MAXN];
int b[MINN];
int Next[MINN];
void kmp_pre(int* pat,int l){
    int i=0;
    int j=-1;
    Next[0]=-1;
    while(i<l-1){
        if(j==-1 || pat[i]==pat[j]){
            i++;
            j++;
            Next[i]=j;
        }
        else{
            j=Next[j];
        }
    }
}
int kmp_search(int* str,int slen,int* pat,int plen){
    memset(Next,0,sizeof(Next));
    kmp_pre(pat,plen);
    int i=0;
    int j=0;
    while(i<slen && j<plen){
        if(j==-1 || str[i]==pat[j]){
            i++;
            j++;
        }
        else{
            j=Next[j];
        }
    }
    if(j==plen){
        return i-j+1;
    }
    else{
        return -1;
    }
}
int main(void){
    int t;
    scanf("%d",&t);
    int n,m;
    while(t--){
        scanf("%d%d",&n,&m);
        for(int i=0;i<n;i++){
            scanf("%d",&a[i]);
        }
        for(int i=0;i<m;i++){
            scanf("%d",&b[i]);
        }
        int res=kmp_search(a,n,b,m);
        printf("%d\n",res);
    }
    return 0;
}
```





