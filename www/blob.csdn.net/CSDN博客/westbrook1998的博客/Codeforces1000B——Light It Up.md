# Codeforces1000B——Light It Up - westbrook1998的博客 - CSDN博客





2018年08月10日 18:38:25[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：41标签：[思维																[模拟](https://so.csdn.net/so/search/s.do?q=模拟&t=blog)](https://so.csdn.net/so/search/s.do?q=思维&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
Recently, you bought a brand new smart lamp with programming features. At first, you set up a schedule to the lamp. Every day it will turn power on at moment 0 and turn power off at moment M. Moreover, the lamp allows you to set a program of switching its state (states are “lights on” and “lights off”). Unfortunately, some program is already installed into the lamp. 

  The lamp allows only good programs. Good program can be represented as a non-empty array a, where 0


```cpp
#include <cstdio>
#include <algorithm>
using namespace std;
const int MAXN=100005;
int n,m;
int a[MAXN];
int on[MAXN];
int off[MAXN];
int main(void){
    scanf("%d%d",&n,&m);
    for(int i=1;i<=n;i++){
        scanf("%d",&a[i]);
    }
    a[n+1]=m;
    for(int i=1;i<=n+1;i++){
        if(i%2){
            on[i]=on[i-1]+a[i]-a[i-1];
            off[i]=off[i-1];
        }
        else{
            on[i]=on[i-1];
            off[i]=off[i-1]+a[i]-a[i-1];
        }
    }
    int ans=on[n+1];
    for(int i=1;i<=n;i++){
        int tmp=0;
        //这个点左或右可以插入
        if(a[i]-a[i-1]>1 || a[i+1]-a[i]>1){
            if(i%2){
                //插入在i点的左边 （前面开的时间少了1）
                tmp=on[i]-1+off[n+1]-off[i];
                //插入在i点的右边  （后面关的时间多了1）
                //tmp=on[i]+off[n+1]-off[i]-1;
            }
            else{
                //插入在i点的左边  （前面关的时间少了1）
                tmp=on[i]+1+off[n+1]-off[i];
                //插入在i点的右边 (后面关的时间少了1,即开的时间多了1)
                //tmp=on[i]+off[n+1]-off[i]+1;
            }
        }
        ans=max(ans,tmp);
    }
    printf("%d\n",ans);
    return 0;
}
```




