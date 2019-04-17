# 并查集模板（C++版） - westbrook1998的博客 - CSDN博客





2018年11月04日 13:24:30[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：42标签：[并查集](https://so.csdn.net/so/search/s.do?q=并查集&t=blog)
个人分类：[C++算法模板](https://blog.csdn.net/westbrook1998/article/category/8243225)









最简单的并查集

hdu1232

```
#include <algorithm>
#include <cstdio>
#include <cstring>
using namespace std;
const int N=1050;
int p[N];
int n,m;
void init(){
    for(int i=1;i<=n;i++){
        p[i]=i;
    }
}
int find(int x){
    return p[x]==x ? x : p[x]=find(p[x]);
}
void unit(int a,int b){
    p[find(a)]=find(b);
}
bool isUnit(int a,int b){
    return find(a)==find(b);
}
int main(void){
    while(~scanf("%d",&n) && n){
        scanf("%d",&m);
        init();
        int u,v;
		//n个节点如果都不连通，需要加n-1条边
        int res=n-1;
        for(int i=0;i<m;i++){
            scanf("%d%d",&u,&v);
			//如果有两个节点连通的话，需要的边就少了一条
            if(!isUnit(u,v)){
                res--;
				//再把这两个点连通
                unit(u,v);
            }
        }
        printf("%d\n",res);
    }
    return 0;
}
```



