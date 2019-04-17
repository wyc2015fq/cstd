# hdu1392——Surround the Trees - westbrook1998的博客 - CSDN博客





2018年11月06日 12:43:56[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：38








求凸包的周长，模板题，卡了很久，最后发现是如果n=2的时候没有特判

代码：

```
#include <bits/stdc++.h>
using namespace std;
const int N=1050;
struct Point{
    int x,y;
}p[N],s[N];
int top,n;
int cross(Point a,Point b,Point c){
    return (b.x-a.x)*(c.y-a.y)-(b.y-a.y)*(c.x-a.x);
}
double dis(Point a,Point b){
    return sqrt((double)(a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}
bool cmp(Point a,Point b){
    int t=cross(p[0],a,b);
    if(t>0 || t==0 && dis(p[0],a)<dis(p[0],b)){
        return true;
    }else{
        return false;
    }
}
void init(int n){
    scanf("%d%d",&p[0].x,&p[0].y);
    Point p0=Point{p[0].x,p[0].y};
    int k=0;
    for(int i=1;i<n;i++){
        scanf("%d%d",&p[i].x,&p[i].y);
        if(p[i].y<p0.y || p[i].y==p0.y && p[i].x<p0.x){
            p0=Point{p[i].x,p[i].y};
            k=i;
        }
    }
    p[k]=p[0];
    p[0]=p0;
    sort(p+1,p+n,cmp);
}
void graham(int n){
    if(n==1){
        top=0;
        s[0]=p[0];
    }else if(n==2){
        top=1;
        s[0]=p[0];
        s[1]=p[1];
    }else{
        top=1;
        s[0]=p[0];
        s[1]=p[1];
        for(int i=2;i<n;i++){
            while(top>0 && cross(s[top-1],s[top],p[i])<=0){
                top--;
            }
            s[++top]=p[i];
        }
    }
}
int main(void){
    while(~scanf("%d",&n) && n){
        init(n);
        graham(n);
        //printf("%d\n",top);
        double ans=0;
        for(int i=1;i<=top;i++){
            ans+=dis(s[i],s[i-1]);
            //printf("%d %d %d %d %lf\n",s[i].x,s[i].y,s[i-1].x,s[i-1].y,dis(s[i],s[i-1]));
        }
        ans+=dis(s[0],s[top]);
        if(n==2){
            ans/=2;
        }
        printf("%.2lf\n",ans);
    }
    return 0;
}
```



