# Codeforces1065C——Make It Equal - westbrook1998的博客 - CSDN博客





2018年10月13日 15:47:53[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：98








> 
题目省略


思路不清晰，后面想复杂了，然后就tle了

题意是给一个数列，代表每个建筑的高度，然后水平一刀切下去，每次拿走的不能超过k个，然后要切多少次才能切平

首先读取每一个高度的时候记录下最大和最小的，最小的其实就是切平的位置，所以以下的都不用管了，然后先记录每一个高度出现的次数，然后用前缀和就可以记录每一层的块的个数了，比如高度为4的有2个，那么第四层的块的个数就等于2，而第三层的块的个数就等于高度为3的个数加上第四层块的个数，因为能到第四层肯定能到第三层

然后就On遍历这个前缀和数组，记录临时和，如果小于k就继续加，直到大于k就切一刀，然后临时和变成这一层的块的个数
代码：

```
#include <cstdio>
#include <algorithm>
using namespace std;
const int N=2*1e5+50;
const int INF=0x3f3f3f3f;
int a[N];
int h[N];
int num[N];
int cnt[N];
int pre[N];
int n,k;
int main(void){
    int maxH=0;
    int minH=INF;
    scanf("%d%d",&n,&k);
    for(int i=0;i<n;i++){
        scanf("%d",&h[i]);
        maxH=max(maxH,h[i]);
        minH=min(minH,h[i]);
        num[h[i]-1]++;
    }
    //特判
    if(minH==maxH){
        printf("0\n");
        return 0;
    }
    num[maxH]=0;
    //前缀和思想求出每一层的个数（需要删除的层）
    for(int i=maxH-1;i>=minH;i--){
        cnt[i]=cnt[i+1]+num[i];
    }
    int ans=1;
    int tmp=0;
    for(int i=1;i<=maxH;i++){
        if(tmp+cnt[i]<=k){
            tmp+=cnt[i];
        }
        else{
            ans++;
            tmp=cnt[i];
        }
    }
    printf("%d\n",ans);
    return 0;
}
```




