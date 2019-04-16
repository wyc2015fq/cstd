# POINTERS ON C【C和指针】 - 紫芝的博客 - CSDN博客





2018年11月10日 18:11:17[紫芝](https://me.csdn.net/qq_40507857)阅读数：4545








```cpp
#include<iostream>
#include<algorithm>
#include<cstdio>
#include<cstring>
#include<math.h>
#include<set>
using namespace std;
const int  maxn = 500+10;
#define MUL(x,y) ++x*++y

int f1(int x){
    return x+5;
}
int f2(int x){
    return 2*x-1;
}

int func(int x){
    int countx=0;
    while(x){
        printf("%d\t%d\n",countx,x);
        countx++;
        x=x&(x-1);
    }
    return countx;
}

int func(int x,int y){
    return x+y;
}
int main(){
    //选择1
    char s[]="123",*p=NULL;
    p=s;
    printf("%c%c%c\n",*p++,*p++,*p++);//321

    //2
    int i=0,a=1,b=2,c=3;
    i=++a||++b||++c;//++a之后a=2，返回为真（i=1），后边不再执行
    printf("%d %d %d %d\n",i,a,b,c);//1 2 2 3

    //3
    a=1,b=2,c=3;
    printf("%d\n",MUL(a+b,b+c));
    printf("%d  %d  %d\n",a,b,c);
    //printf("%d\n",MUL(a,b));
    //printf("%d\n",++a*++b);

    //4
    int u=010,v=0x10,w=10;
    printf("%d,%d,%d\n",u,v,w);

    //5
    int (*p1)(int),(*p2)(int),(*t)(int),y1,y2;
    p1=f1;  p2=f2;
    y1=p2(p1(1));
    t=p1;   p1=p2;  p2=t;
    y2=p2(p1(2));
    printf("%d,%d\n",y1,y2);

    //6
    printf("%d\n",func(9981));

    //7
    char *S="\ta\018bc";//\0表示后面的字符是八进制数
    int num=0;
    for(;*S!='\0';S++)
        printf("%d  ",*S);//6个字符：\t,a,\01,8,b,c
        printf("\n");

    //8
    int A=1,B=2,C=3,D=4,E=5;
    printf("%d\n",func((A+B,B+C,C+A),(D+E)));//func(C+A,D+E)=13,逗号表达式
    return 0;
}
```





