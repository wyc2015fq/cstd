# hdu1425——sort - westbrook1998的博客 - CSDN博客





2018年06月18日 16:46:18[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：36标签：[水题](https://so.csdn.net/so/search/s.do?q=水题&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
给你n个整数，请按从大到小的顺序输出其中前m大的数。  

  Input 

  每组测试数据有两行，第一行有两个数n,m(0
```cpp
#include <cstdio>
#include <algorithm>
using namespace std;
const int MAXN=1000050;
int a[MAXN];
bool cmp(int a,int b){
    return a>b;
}
int main(void){
    int n,m;
    while(~scanf("%d%d",&n,&m)){
        int t;
        for(int i=0;i<n;i++){
            scanf("%d",&t);
            a[t+500000]=1;
        }
        for(int i=MAXN;i>=0;i--){
            if(a[i]==1 && m>0){
                m--;
                if(m>0){
                    printf("%d ",i-500000);
                }
                else{
                    printf("%d\n",i-500000);
                }
            }
        }
    }
    return 0;
}
```





