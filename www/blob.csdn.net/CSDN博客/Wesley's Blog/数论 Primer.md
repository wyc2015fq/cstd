
# 数论 Primer - Wesley's Blog - CSDN博客


2018年08月12日 13:49:38[WesleyWang97](https://me.csdn.net/yinanmo5569)阅读数：37标签：[ACM																](https://so.csdn.net/so/search/s.do?q=ACM&t=blog)[编程题																](https://so.csdn.net/so/search/s.do?q=编程题&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=ACM&t=blog)个人分类：[编程题																](https://blog.csdn.net/yinanmo5569/article/category/7929888)


几个重要的数论模型：
# 欧几里得定理（辗转相除法）
`int gcd(int a, int b){
    return b==0?a:gcd(b, a%b);
}`
# 扩展欧几里得（求ax+by = gcd(a,b)的特解)
`void e_gcd(LL a, LL b, LL &d, LL &x, LL &y){
    if(b==0){
        x = 1; y = 0; d = a;
    } 
    else{
        e_gcd(b, a%b, d, y, x);
        y-= x*(a/b);
    }
}`
# 中国剩余定理
同余方程组
`x ≡ a1(mod m1)
x ≡ a2(mod m2)
... ...
x ≡ ak(mod mk)`方程组所有的解的集合就是：
x1 = N1*a1 + N2*a2 + … + Nk*ak 其中 Ni mod mi = 1，Ni = ai * ti , 可用欧几里得扩展定理求 ti. 其中M = m1*m2*m3····*mn;
`//互质版 
　　#include <iostream>  
    using namespace std;  
    //参数可为负数的扩展欧几里德定理  
    void exOJLD(int a, int b, int &x, int &y){  
        //根据欧几里德定理  
        if(b == 0){//任意数与0的最大公约数为其本身。  
            x = 1;  
            y = 0;  
        }else{  
            int x1, y1;  
            exOJLD(b, a%b, x1, y1);  
            if(a*b < 0){//异号取反  
                x = - y1;  
                y = a/b*y1 - x1;  
            }else{//同号  
                x = y1;  
                y = x1 - a/b* y1;  
            }  
        }  
    }  
    //剩余定理  
    int calSYDL(int a[], int m[], int k){  
        int N[k];//这个可以删除  
        int mm = 1;//最小公倍数  
        int result = 0;  
        for(int i = 0; i < k; i++){  
            mm *= m[i];  
        }  
        for(int j = 0; j < k; j++){  
            int L, J;  
            exOJLD(mm/m[j], -m[j], L, J);  
            N[j] = m[j] * J + 1;//1  
            N[j] = mm/m[j] * L;//2 【注】1和2这两个值应该是相等的。  
            result += N[j]*a[j];  
        }  
        return (result % mm + mm) % mm;//落在(0, mm)之间，这么写是为了防止result初始为负数，本例中不可能为负可以直接 写成：return result%mm;即可。  
    }  

    int main(){  
        int a[3] = {2, 3, 2};  
        int m[3] = {3, 5, 7};      
        cout<<"结果:"<<calSYDL(a, m, 3)<<endl;  
    }`不互质版
`中国剩余定理（不互质） 
*/  
#include <iostream>  
#include <cstdio>  
#include <cstring>  
using namespace std;  
typedef long long LL;  
LL Mod;  
LL gcd(LL a, LL b)  
{  
    if(b==0)  
        return a;  
    return gcd(b,a%b);  
}  
LL Extend_Euclid(LL a, LL b, LL&x, LL& y)  
{  
    if(b==0)  
    {  
        x=1,y=0;  
        return a;  
    }  
    LL d = Extend_Euclid(b,a%b,x,y);  
    LL t = x;  
    x = y;  
    y = t - a/b*y;  
    return d;  
}  
//a在模n乘法下的逆元，没有则返回-1  
LL inv(LL a, LL n)  
{  
    LL x,y;  
    LL t = Extend_Euclid(a,n,x,y);  
    if(t != 1)  
        return -1;  
    return (x%n+n)%n;  
}  
//将两个方程合并为一个  
bool merge(LL a1, LL n1, LL a2, LL n2, LL& a3, LL& n3)  
{  
    LL d = gcd(n1,n2);  
    LL c = a2-a1;  
    if(c%d)  
        return false;  
    c = (c%n2+n2)%n2;  
    c /= d;  
    n1 /= d;  
    n2 /= d;  
    c *= inv(n1,n2);  
    c %= n2;  
    c *= n1*d;  
    c += a1;  
    n3 = n1*n2*d;  
    a3 = (c%n3+n3)%n3;  
    return true;  
}  
//求模线性方程组x=ai(mod ni),ni可以不互质  
LL China_Reminder2(int len, LL* a, LL* n)  
{  
    LL a1=a[0],n1=n[0];  
    LL a2,n2;  
    for(int i = 1; i < len; i++)  
    {  
        LL aa,nn;  
        a2 = a[i],n2=n[i];  
        if(!merge(a1,n1,a2,n2,aa,nn))  
            return -1;  
        a1 = aa;  
        n1 = nn;  
    }  
    Mod = n1;  
    return (a1%n1+n1)%n1;  
}  
LL a[1000],b[1000];  
int main()  
{  
    int i;  
    int k;  
    while(scanf("%d",&k)!=EOF)  
    {  
        for(i = 0; i < k; i++)  
            scanf("%I64d %I64d",&a[i],&b[i]);  
        printf("%I64d\n",China_Reminder2(k,b,a));  
    }  
    return 0;  
}`
# 欧拉函数（求一个数前面的所有与这个数互质的数的个数）
Euler函数表达通式：`euler(x)=x(1-1/p1)(1-1/p2)(1-1/p3)(1-1/p4)…(1-1/pn)`,其中p1,p2……pn为x的所有素因数，x是不为0的整数。euler(1)=1（唯一和1互质的数就是1本身）。   Euler函数有几个性质：
如果q,p互质，则Euler（p*q） = Euler（p）*Euler（q）；
如果 a = p^k，则Euler（a） = p^k - p^k-1;
//直接求解欧拉函数  
int euler(int n){ //返回euler(n)   
     int res=n,a=n;  
     for(int i=2;i*i<=a;i++){  
         if(a%i==0){  
             res=res/i*(i-1);//先进行除法是为了防止中间数据的溢出   
             while(a%i==0) a/=i;  
         }  
     }  
     if(a>1) res=res/a*(a-1);  
     return res;  
}  
 //线性筛选欧拉函数O(n)用到了一下性质：
//(1) 若(N%a==0 && (N/a)%a==0) 则有:E(N)=E(N/a)*a;
//(2) 若(N%a==0 && (N/a)%a!=0) 则有:E(N)=E(N/a)*(a-1);   
//注意：如果范围过大 可能不适宜开数组来做
int euler[maxN], vis[maxN], prime[maxN/5], e[maxN], cnt = 0;
void make_euler(){
    memset(vis, 0, sizeof(vis));
    euler[1] = 1;
    for(int i=2; i<maxN ; ++i){
        if(vis[i] == 0){
            prime[cnt++] = i;
            euler[i] = i-1;
        }
        for(int j=0 ; j<cnt && i*prime[j] < maxN; ++j){
            vis[i*prime[j]] = 1;
            if( i%prime[j] == 0){
                euler[i*prime[j]] = euler[i] *prime[j];
                break;
            }
            else euler[i*prime[j]] = euler[i] *(prime[j]-1);
        }
    }
}
# 求N以前N的约数个数
约数个数的性质，对于一个数N，`N=p1^a1 + p2^a2 + ... + pn^an`。其中p1 ,p2, p3… pn是N的质因数，a1 ,a2, a2,…an为相应的指数，则
`div_num[N]=(p1+1)*(p2+1)*(p3+1)* ... *(pn+1);`结合这个算法的特点，在程序中如下运用：
对于div_num：
`(1)如果i|prime[j] 那么 div_num[i*prime[j]]=div_sum[i]/(e[i]+1)*(e[i]+2)                  //最小素因子次数加1
(2)否则 div_num[i*prime[j]]=div_num[i]*div_num[prime[j]]                                     //满足积性函数条件`对于e：
(1)如果i|pr[j]  e[i*pr[j]]=e[i]+1; //最小素因子次数加1
(2)否则 e[i*pr[j]]=1;              //pr[j]为1次
`#include<string.h>  
#include<iostream>
#define M 100000  
using namespace std;
int prime[M/3],e[M],div_num[M];           // e[i]表示第i个素数因子的个数  
bool flag[M];  
void get_prime()  
{  
    int i,j,k;  
    memset(flag,false,sizeof(flag));  
    k=0;  
    for(i=2;i<M;i++){  
        if(!flag[i]){                              
            prime[k++]=i;  
            e[i]=1;  
            div_num[i]=2;                       //素数的约数个数为2  
        }  
        for(j=0;j<k&&i*prime[j]<M;j++){  
            flag[i*prime[j]]=true;              
                if(i%prime[j]==0){  
                    div_num[i*prime[j]]=div_num[i]/(e[i]+1)*(e[i]+2);  
                    e[i*prime[j]]=e[i]+1;  
                    break;  
                }  
                else{  
                    div_num[i*prime[j]]=div_num[i]*div_num[prime[j]];  
                    e[i*prime[j]]=1;  
                }  
        }
    }  
}`
# 莫比乌斯函数
一个讲得比较清楚的PPT：[http://wenku.baidu.com/link?url=UARIPTGHjN78vIzedWT2iwICudBIbsuZ5WMrYwJJjp2P5x7hUvtvSoVKiW7a92GiiF7aCJu1FYid2eB5iM9Wh-hW2Bfd1UfJgrstX7nZnrm](http://wenku.baidu.com/link?url=UARIPTGHjN78vIzedWT2iwICudBIbsuZ5WMrYwJJjp2P5x7hUvtvSoVKiW7a92GiiF7aCJu1FYid2eB5iM9Wh-hW2Bfd1UfJgrstX7nZnrm)
线性筛打表莫比乌斯函数：
`int mob[maxN], vis[maxN], prime[maxN], cnt=0;
void make_mobius(){
    mob[1] = 1;
    memset(vis, 0, sizeof(vis));
    for(int i = 2; i<maxN ; ++i){
        if(!vis[i]){
            mob[i] = -1;
            prime[cnt++] = i;
        }
        for(int j= 0; j<cnt && i*prime[j] < maxN ; ++j){
            vis[i*prime[j]] = 1;
            if(i%prime[j] == 0){
                mob[i*prime[j]] = 0;
                break;
            }
            else mob[i*prime[j]] = -mob[i];
        }
    }
}`

