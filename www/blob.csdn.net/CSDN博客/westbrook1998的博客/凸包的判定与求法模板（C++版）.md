# 凸包的判定与求法模板（C++版） - westbrook1998的博客 - CSDN博客





2018年11月06日 18:35:07[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：52








凸包的判定方法：将点按逆时针排列，依次枚举三个点$a,b,c$求向量$(a,b)$和$(b,c)$，的叉乘，也就是判断$(a,b)$和$(b,c)$的夹角，如果叉乘小于0，即sin夹角大于180度，即不是凸包

注意到这里是求$(a,b)$和$(b,c)$的叉乘，而在下面的凸包求法中是求$(a,b)$和$(a,b)$的叉乘

```
#include <bits/stdc++.h>
using namespace std;
const int N=1e6+50;
struct Point{
    int x,y;
}p[N];
int n;
int cross(Point a,Point b,Point c,Point d){
    return (b.x-a.x)*(d.y-c.y)-(b.y-a.y)*(d.x-c.x);
}
int main(void){
    while(~scanf("%d",&n) && n){
        for(int i=0;i<n;i++){
            scanf("%d%d",&p[i].x,&p[i].y);
        }
        p[n]=p[0];
        p[n+1]=p[1];
        bool flag=true;
        for(int i=0;i<n;i++){
            if(cross(p[i],p[i+1],p[i+1],p[i+2])<0){
                flag=false;
                break;
            }
        }
        if(flag){
            printf("convex\n");
        }else{
            printf("concave\n");
        }
    }
    return 0;
}
```

凸包的求法（Graham）：从初始点开始逆时针扫描，维护一个栈，每次栈顶$a$和栈顶-1的$b$两个点与当前点$c$的两个向量$(a,c)$和$(b,c)$，如果叉乘小于等于0，就出栈一个点，直到栈空或者栈顶两个点满足条件，这时候再把当前点$c$加进去，最后栈中的点就是凸包上的点（逆时针）

这题是求凸包的周长，注意叉乘函数和上面的不同（两向量同起点a）

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
            p0=p[i];
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
        double ans=0;
        for(int i=1;i<=top;i++){
            ans+=dis(s[i],s[i-1]);
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



