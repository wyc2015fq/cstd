# HDOJ2035 人见人爱A^B - 江南烟雨 - CSDN博客
2011年03月08日 19:55:00[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：3127
题目：[http://acm.hdu.edu.cn/showproblem.php?pid=2035](http://acm.hdu.edu.cn/showproblem.php?pid=2035)
代码：
#include<stdio.h>
#include<stdlib.h>
int main(){
    int a,b,i,p;
    while(scanf("%d %d",&a,&b)!=EOF){
         if(a==0&&b==0){
               break;         
         }        
        //printf("a=%d,b=%d/n",a,b);   
         p=a;
         a=1;
         for(i=1;i<=b;i++){
               a=a%1000;
               a=a*p;            
         }
         printf("%d/n",a%1000);
    }
}
