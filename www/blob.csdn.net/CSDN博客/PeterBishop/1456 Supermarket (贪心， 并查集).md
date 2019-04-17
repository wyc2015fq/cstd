# 1456 Supermarket (贪心， 并查集) - PeterBishop - CSDN博客





2018年12月09日 21:17:05[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：11
个人分类：[POJ](https://blog.csdn.net/qq_40061421/article/category/7589386)









```cpp
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<queue>
using namespace std;
 
const int N = 10005;
struct Node{
    int px, dx;
    friend bool operator<(const Node&a,const Node&b){
        return a.px>b.px;
    }
}arr[N];
 
int n;
 //简单的贪心就可以解决，当然是选择利润优先，然后把截止日期给占掉
//如果已经给占用了，那就往前推一天
int main(){
    while(~scanf("%d",&n)){
        int maxTime=0;
        for(int i=0; i<n; ++i){
            scanf("%d%d",&arr[i].px, &arr[i].dx);
            if(arr[i].dx>maxTime) maxTime = arr[i].dx;
        }
        sort(arr,arr+n);
        bool vis[N];
        memset(vis, 0, sizeof(vis));
        int ans = 0, pos=0;
        for(int i=0; i<n; ++i){
            if(!vis[arr[i].dx]){
                ans += arr[i].px;
                vis[arr[i].dx] = true;
            }
            else{
                for(int j=arr[i].dx-1; j>=1; --j)if(!vis[j]){
                    ans += arr[i].px;
                    vis[j] = true;
                    break;
                }
            }
        }
        printf("%d\n", ans);
    }
    return 0;
}
```

```cpp
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<queue>
using namespace std;
 
const int N = 10005;
struct Node{
    int px, dx;
    friend bool operator<(const Node&a,const Node&b){
        return a.px>b.px;
    }
}arr[N];
 
int f[N],n;
inline void init(int n){
    for(int i=0; i<=n; ++i)
        f[i]=i;
}
int find(int x){//找到没有被占用的最后那一天
/*假设一个产品a占用了一个日期后，那么如果下次又有一个产品b和产品a的截止日期是相同的，但是那个日期以被占用了，所以就要往前移动1天，那么就可以用并查集进行标记，在a占用了那个日期后，把a的截止日期指向前一个日期，这样的话，可以直接查找到他要占用到哪一个时间*/
    int i, j=x;
    while(j!=f[j]) j=f[j];
    while(x!=j){
        i=f[x]; f[x]=j; x=i;//把相关的都修改一下
    }
    return j;
}
 
int main(){
    while(~scanf("%d",&n)){
        int maxTime=0;
        for(int i=0; i<n; ++i){
            scanf("%d%d",&arr[i].px, &arr[i].dx);
            if(arr[i].dx>maxTime) maxTime = arr[i].dx;
        }
        init(maxTime);
        sort(arr,arr+n);
        int ans = 0;
        for(int i=0; i<n; ++i){
            int d = find(arr[i].dx);
            if(d>0){
                ans += arr[i].px;
                f[d] = d-1;
            }
        }
        printf("%d\n", ans);
    }
    return 0;
}
```







