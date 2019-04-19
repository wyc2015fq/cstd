# hdu 6330 Problem L. Visual Cube（模拟） - HJ - CSDN博客
2018年08月11日 19:07:21[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：42
[http://acm.hdu.edu.cn/showproblem.php?pid=6330](http://acm.hdu.edu.cn/showproblem.php?pid=6330)
练练手的模拟，做了将近两个小时，最后才发现原来长宽高搞错了。
做法：先解决左上和右下的两个直角三角形，然后解决顶边和正面两面，剩下的就填补一下就行了。
```
#include<iostream>
#include<cstring>
#include<cstdio>
using namespace std;
char m[1005][1005],p[]={'+','-','/','.','|'};
void print1(char c1,char c2,int len,int i,int j)
{
    for(int k=0;k<len;k++)
      m[i][j++]=(k%2)?c2:c1; 
}
int main()
{
    int t,a,b,c;
    cin>>t;
    while(t--)
    {
        cin>>a>>b>>c;
        memset(m,'a',sizeof(m));
        for(int i=0,cnt=2*b;i<2*b;i++,cnt--)
        {
            print1(p[3],p[3],cnt,i,0);    //左上角的直角三角形的'.' 
            i%2?print1(p[2],p[3],2*a,i,cnt):print1(p[0],p[1],2*a,i,cnt);  //长方体的顶盖 
            print1(p[3],p[3],cnt,2*b+2*c-i,2*a+2*b-cnt+1);  //右下角的直角三角形的'.' 
        }
        for(int i=2*b;i<2*b+2*c+1;i++)   //长方体的正面 
          i%2?print1(p[4],p[3],2*a,i,0):print1(p[0],p[1],2*a,i,0); 
        for(int i=0;i<=2*b+2*c;i++)
          for(int j=0;j<=2*a+2*b;j++)
            if(m[i][j]=='a')     //剩下的就分四种情况填补填补 
              i%2?(m[i][j]=(j%2)?p[2]:p[4]):(m[i][j]=(j%2)?p[3]:p[0]);
        for(int i=0;i<=2*b+2*c;i++)   //最后输出 
        {
            for(int j=0;j<=2*a+2*b;j++)
              cout<<m[i][j];
            cout<<endl;
        }
    }
    return 0;
}
```
