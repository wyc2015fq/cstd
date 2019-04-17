# codeforces 1110E——Magic Stones - westbrook1998的博客 - CSDN博客





2019年02月08日 15:23:56[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：139








题意是给两个数组c和t，然后对c数组进行变换，可以把$c_i$换成$c_{i+1}+c_{i-1}-c_i$，问c数组能不能变换成t数组

题解的做法就是分别求两个数组的差分数组，然后比较这两个差分数组是否相同，如果相同，再看c数组和t数组第一个数是不是相同，是的话说明c数组可以变换到t数组

证明过程：

定义$$d_i=c_{i+1}−c_i $$
$$d′_{j−1}=c′_j−c_{j−1}=(c_{j+1}+c_{j−1}−c_j)−c_{j−1}=c_{j+1}−c_j=d_j; \\d′_j=c_{j+1}−c′_j=c_{j+1}−(c_{j+1}+c_{j−1}−c_j)=c_j−c_{j−1}=d_{j−1}.$$

可以看出每一次变换都会导致差分数组的两个相邻元素位置交换，但是大小是不变的。

然后还要判断第一个元素是否相等。。。想不出怎么解释
```cpp
#include <bits/stdc++.h>
using namespace std;
const int N=2e5+50;
int n;
int c[N],t[N],dc[N],dt[N];
int main(void){
    scanf("%d",&n);
    for(int i=0;i<n;i++){
        scanf("%d",&c[i]);
        if(i!=0){
            dc[i]=abs(c[i]-c[i-1]);
        }
    }
    for(int i=0;i<n;i++){
        scanf("%d",&t[i]);
        if(i!=0){
            dt[i]=abs(t[i]-t[i-1]);
        }
    }
    sort(dc+1,dc+n);
    sort(dt+1,dt+n);
    // for(int i=1;i<n;i++){
    //     printf("%d ",dc[i]);
    // }
    // printf("\n");
    // for(int i=1;i<n;i++){
    //     printf("%d ",dt[i]);
    // }
    // printf("\n");
    bool flag=true;
    for(int i=1;i<n;i++){
        if(dc[i]!=dt[i]){
            flag=false;
            break;
        }
    }
    
    if(flag && c[0]==t[0]){
        printf("YES\n");
    }else{
        printf("NO\n");
    }
    return 0;
}
```




