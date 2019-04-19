# ACM之路——算法模板（数学） - HJ - CSDN博客
2016年08月13日 15:55:05[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：1607
在说正事之前，先说说题外话，进集训队也有将近八个月，感觉这就是一个靠数学吃饭的地方，另外还需要努力，每一道题目你解决它所花的时间跟你的进步是成正比的，路还有那么长，不过今年就要参加省赛了，剩下的时间不多，赶紧补题去！！！
数学在比赛中占的比重比较大，主要是数论、公式、递推、组合、概率、矩阵、博弈等等，目前更新一部分的模板，另外一部分会马上更新，请大家不要着急。
一、简单数论、特殊公式
（1）素数判定
```
bool prime(int x)         
{
    for(int i=2;i*i<=maxn;i++)    //降低时间复杂度
      if(x%i==0)
        return false;
    return true; 
}
```
（2）筛法素数打表
```cpp
void getprime(int n)      
{
    int k=0,ans[maxn];
    memset(ans,0,sizeof(ans));
    ans[0]=ans[1]=1;
    for(int i=2;i<=n;i++)
    {
        if(!ans[i])             
          for(int j=i*i;j<=n;j+=i)
            ans[j]=1;              //标记变量
    }
    for(int i=2;i<=n;i++)
      if(!ans[i])                //值为0的都是素数
        ans[k++]=i;
    ans[k]='\0';
    for(int i=0;ans[i]!='\0';i++)
      cout<<ans[i]<<" ";
    cout<<endl; 
}
```
（3）唯一分解定理
```cpp
void W(int n)         
{
    int k=0，s[maxn];
    for(int i=2;i<=n;i++)
    {
        if(n%i==0)
        {
            while(n%i==0)           //一定能保证i为素数
              n/=i;
            s[k++]=i;
        }
    }
    for(int i=0;i<k;i++)
      cout<<s[k]<<" ";
    cout<<endl;
}
```
（4）欧拉公式
```
N(n)=n*(1-1/p1)*(1-1/p2)···（其中p1、p2为能被n整除的素因子）
```
```
void euler_phi(int n)         
{
    int sum=n;
    for(int i=2;i<=n;i++)
    {
        if(n%i==0)
        {
            sum=sum/i*(i-1);
            while(n%i==0)
              n/=i;
        }
    }
    if(n>1)
      sum=sum/n*(n-1);
    cout<<sum<<endl;
}
```
（5）GCD
```
int gcd(int a,int b)         
{
    return b==0?a:gcd(b,a%b);    //不断递归
}
```
（6）扩展GCD（求出x,y，满足Ax+By=gcd(A,B) ）
```
int extend_gcd(int a,int b,int &x,int &y)         
{
    if(b==0)
    {
        x=1;
        y=0;
        return a;
    }
    int r=extend_gcd(b,a%b,y,x);  
    y-=x*(a/b);
    return r;
}
```
（7）中国剩余定理（中国剩余定理   求出方程组x=a(i)(mod m(i))(0<=i
```
int CRT(int a[],int m[],int n)           
{
    int M=1;
    for(int i=0;i<n;i++)
      M*=m[i];
    int ret=0;
    for(int i=0;i<n;i++)
    {
        int x,y;
        int tm=M/m[i];
        extend_gcd(tm,m[i],x,y);
        ret=(ret+tm*x*a[i])%M;
    }
    return (ret+M)%M;
}
```
（8）进制转换（x进制的数转换成y进制 ）
```
string transform(int x,int y,string s)   
{
    string res="";
    int sum=0;
    for(int i=0;i<s.length();i++)
    {
        if(s[i]=='-')
          continue;
        if(s[i]>='0'&&s[i]<='9')      //十进制以内
          sum=sum*x+s[i]-'0';
        else
          sum=sum*x+s[i]-'A'+10;      //十进制之外
    }
    while(sum)
    {
        char tmp=sum%y;
        sum/=y;
        if(tmp<=9)
          tmp+='0';
        else
          tmp=tmp-10+'A';
        res=tmp+res;
    }
    if(res.length()==0)
      res="0";
    if(s[0]=='-') 
      res='-'+res;
    return res;
}
```
（9）快速幂取模（求a的i次幂对n的模 ）
```
void quickpow_mod(int a,int i,int n)         
{
    int k=a%n,sum=1;
    while(i)
    {
        if(i&1)
          sum=sum*k%i;
        k=k*k%a;
        n>>=1;
    }
    cout<<sum<<endl;
}
```
（10）星期计算（蔡勒公式   给定一个日期，求出这一天是星期几，返回ans，表示是星期(ans+1) ）
```
int whatday(int day,int mon,int year)   
{
    int ans;
    if(mon==1||mon==2)
    {
        mon+=12;
        year--;
    }
    if((year<1752)||(year==1752&&mon<9)||(year==1752&&mon==9&&day<3))   //当日期在1752年9月3日之前
      ans=(day+2*mon+3*(mon+1)/5+year+year/4+5)%7;
    else  
      ans=(day+2*mon+3*(mon+1)/5+year+year/4-year/100+year/400)%7;
    return ans;
}
```
（11）闰年判断
```
bool Isleap(int year)           
{
    if(year%400==0||year%100&&year%4==0)
      return true;
    return false;
}
```
（12）日期计算
```
int leap(int y)           
{
    if(!y)
      return 0;
    return y/4-y/100+y/400;
}
int  calc(int day,int mon,int year)       
{
    int res=(year-1)*day+leap(year-1);
    int s[12]={31,28,31,30,31,30,31,31,30,31,30,31};
    for(int i=1;i<mon;i++)
      res+=s[i];
    if(Isleap(year)&&mon>2)
      res++;
    res+=day;
    return res;
} 
void count_day(int da,int ma,int ya,int db,int mb,int yb)  
{
    int resa=calc(da,ma,ya);
    int resb=calc(db,mb,yb);
    cout<<abs(resa-resb)<<endl; 
}
```
二、常见递推，特殊计数
（13）Catalan数（ 1,1,2,3,5,14,42,132,429,1430,4862,16796）
```
f(n)=f(2)*f(n-1)+f(3)*f(n-2)+...+f(n-1)*f(2)   边界是f[2]=f[3]=1;
```
```
void Catalan(int n)        
{
    long long count=1;
    for(int i=1,j=2*n;i<=n;i++,j--)
      count=count*j/i;
    cout<<count/(n+1)<<endl;
}
```
（14）第二类Stirling数、Bell数（结果b[n]对c取模 ）（1,2,5,15,52,203,877,140,147,975,当n=100时,b[n]=751）
```
#define c 1000
long long a[2005][2005]={1};
long long b[2005];
void Bell(int n,int c)           
{
    for(int i=1;i<=2000;i++)
    {
        a[i][0]=0;
        a[i][i]=1;
        for(int j=1;j<i;j++)
          a[i][j]=(a[i-1][j-1]+j*a[i-1][j])%c;
    }
    for(int i=1;i<=2000;i++)
    {
        b[i]=0;
        for(int j=0;j<=i;j++)
          b[i]=(b[i]+a[i][j])%c;
    } 
    cout<<b[n]<<endl;
}
```
（15）错排计数（给出n，求出1~n的错排个数占n个数全排列的百分比 
```
错排公式：Dn=(1-1+1/2!-...+[(-1)^n]*1/n!)*n!)
```
```
void Cuopai(int n)       
{
    double sum,sign=1.0;
    if(n==1)
      sum=0;
    else
    {
        sum=0;
        long long k=1;
        for(int i=2;i<=n;i++)
        {
            k*=i;
            sum=sum+sign/(k*1.0);
            sign=-sign;
        }
        sum*=100;
    }
    printf(".2%%lf",sum);
}
```
（16）全排列（字符串S字母的全排列输出 (可转化应用于数组)）
```cpp
void Pailie(char s[])         
{
    int len=strlen(s);
    sort(s,s+len);
    do
    {
        cout<<s<<endl;
    }while(next_permutation(s,s+len));
}
```
（17）三角形计数（从1~n任意选择三个数，并且保证三边长能够组成三角形）（0,1,3,7,13,22,34,50,70,95,125,161,203,252,308,372,444）
```
f(n)=f(n-1)+((n-1)*(n-2)/2-(n-1)/2)/2  边界是f[3]=0;
```
```cpp
void TC(int n)
{
    long long a[maxn];
    a[3]=0;
    for(i=4;i<maxn;i++)
      a[i]=a[i-1]+((i-1)*(i-2)/2-(i-1)/2)/2;
    cout<<a[n]<<endl;
}
```
（18）折线分割平面（0,5,14,27,44,65,90,119,152,189,230,275,324,377,434）
```
f(n)=f(n-1)+4*(n-1))+1   边界f[1]=0;
```
```cpp
void ZX(int n)       
{
    long long a[maxn];
    a[1]=0;
    for(int i=2;i<10005;i++)
      a[i]=a[i-1]+4*(i-1)+1;
    cout<<a[n]<<endl;
}
```
（19）三色问题（3,6,6,18,50,66,126,258,510,1026,2046,4098,8190,16386,32766）
```
f(n)=f(n-1)+f(n-2)*2   边界f[0]=0;
```
```cpp
void RPG(int n)        
{
    long long s[55]; 
    s[0]=0;s[1]=3,s[2]=6;s[3]=6;
    for(int i=4;i<=50;i++)
      s[i]=s[i-1]+s[i-2]*2;
    cout<<s[n]<<endl;
}
```
（20）骨牌铺方格I（两层）（1,2,3,5,8,13,21,34,55,89,144,233,377,610,987）
```
f(n)=f(n-1)+f(n-2)   边界f[0]=0;
```
```cpp
void GPI(int n)          
{
    long long s[50];
    s[0]=0,s[1]=1,s[2]=2;
    for(int i=3;i<50;i++)
      s[i]=s[i-1]+s[i-2];
    cout<<s[n]<<endl;
}
```
（21）骨牌铺方格II（三层）（3,11,41,153,571,2131,7953,29681,110771）
```
f(n)=3*f(n-2)+2*(f(n-4)+...+f(2));   (n为偶数，且f(0)=1,f[2]=3)
```
```
void GPII(int n)       
{
    int a[17],sum=0;
    a[0]=0,a[1]=1;
    for(int i=2;i<=16;i++)
    {
        sum=sum+a[i-2];
        a[i]=3*a[i-1]+2*sum;
    }
    if(n%2==1)
      cout<<0<<endl;
    else
      cout<<a[n/2+1]<<endl; 
}
```
（22）切西瓜问题（2,3,5,8,12,17,23,30,38,47,57,68）
```
F(n)=(n*n-n+4)/2;
```
```
int QXG(int n)    
{
    return (n*n-n+4)/2;
}
```
