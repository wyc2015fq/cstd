# 在VC上可以运行的烟花代码~ - qq_37385726的博客 - CSDN博客





2017年02月01日 16:12:55[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：6658标签：[C语言																[烟花代码																[VC](https://so.csdn.net/so/search/s.do?q=VC&t=blog)
个人分类：[C语言](https://blog.csdn.net/qq_37385726/article/category/6697679)





                
# define PI 3.14

#include<graphics.h>

#include<stdio.h>

#include<stdlib.h>

#include<math.h>

#include<time.h>

#include<conio.h>

#include<dos.h>

#include<string.h>

void star(int x,int y);

void drawstar();

void Putstar(void);

void starflower();

int main()

{
int gdriver=DETECT;
int gmode=0;

initgraph(&gdriver,&gmode,"c:\\tc20\\bgi");

drawstar();
starflower();
getch();
closegraph();
return 0;

}

void star(int x,int y)

{
int i,a;
int n=5;
int x1[5],y1[5],x2[5],y2[5];
setcolor(YELLOW);
for(i=0;i<5;i++)
{
x1[i]=x+n*cos(i*72*PI/180);
y1[i]=y+n*sin(i*72*PI/180);
x2[i]=x+n/2*cos(i*72*PI/180+PI/5);
y2[i]=y+n/2*sin(i*72*PI/180+PI/5);
}
for(i=0;i<5;i++)
{
a=i+1;
if(a>4) a=0;
line(x1[i],y1[i],x2[i],y2[i]);
line(x2[i],y2[i],x1[a],y1[a]);
}

}

void Putstar(void)

{
int seed=1858;
int i,dotx,doty,h,w,color,maxcolor;
w=getmaxx();
h=getmaxy();
srand(seed);
for(i=0;i<100;++i)
{dotx=i+rand()%(w-1);
doty=1+rand()%(h-1);
color=rand()%(h-1);
setcolor(color);
putpixel(dotx,doty,color);
circle(dotx+1,doty+1,1);
}
srand(seed);

}

void drawstar()

{
int a[]={70,280,230,440,140,110,180,90,500,360};
int b[]={50,27,88,99,100,37,67,98,60,78},i;
setfillstyle(1,14);
for(i=0;i<10;i++)
{
star(a[i],b[i]);
floodfill(a[i],b[i],YELLOW);
}
Putstar();

}

void starflower() 

{
int i=0,j,n=60,n1=2;
int x=200,y=200,size=100;
int cover=0;
int delay1=20;
int wid,hei;
int px,py;
int color=9;
while(!kbhit())
{
if(i<size)
{
for(j=0;j<n;j++)
{
px=x+i*cos(j*360/n*PI/180);
py=y+i*sin(j*360/n*PI/180);
putpixel(px,py,rand()%16);
putpixel(px-1,py,color);
putpixel(px,py+1,color);
putpixel(px+1,py-1,YELLOW);
putpixel(px,py-1,YELLOW);
putpixel(px+1,py,RED);
putpixel(px+1,py+1,RED);
}
}
if(i>size&&cover<size)
{
setcolor(BLACK);
circle(x,y,cover++);
delay1=20;
}
if(cover==size)
{
i=0;
x=50+rand()%550;
y=rand()%400;
cover=0;
color=rand()%16;
size=50+rand()%250;
delay1=40;
drawstar();

}
i+=n1;
Sleep(delay1);
}

}](https://so.csdn.net/so/search/s.do?q=烟花代码&t=blog)](https://so.csdn.net/so/search/s.do?q=C语言&t=blog)




