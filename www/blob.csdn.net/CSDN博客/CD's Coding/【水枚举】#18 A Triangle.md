# 【水枚举】#18 A. Triangle - CD's Coding - CSDN博客





2014年05月09日 01:05:35[糖果天王](https://me.csdn.net/okcd00)阅读数：621
所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)


















At a geometry lesson Bob learnt that a triangle is called right-angled if it is nondegenerate and one of its angles is right. Bob decided to draw such a triangle immediately: on a sheet of paper he drew three points with integer coordinates, and joined them
 with segments of straight lines, then he showed the triangle to Peter. Peter said that Bob's triangle is not right-angled, but is almost right-angled: the triangle itself is not right-angled,
 but it is possible to move one of the points exactly by distance 1 so, that all the coordinates remain integer, and the triangle become right-angled. Bob asks you to help him and find out if Peter tricks him. By the given coordinates of the triangle you should
 find out if it is right-angled, almost right-angled, or neither of these.




Input


The first input line contains 6 space-separated integers *x*1, *y*1, *x*2, *y*2, *x*3, *y*3 —
 coordinates of the triangle's vertices. All the coordinates are integer and don't exceed 100 in absolute value. It's guaranteed that the triangle is nondegenerate, i.e. its total area is not zero.




Output


If the given triangle is right-angled, output RIGHT, if it is almost right-angled, output ALMOST,
 and if it is neither of these, output NEITHER.




Sample test(s)




input
0 0 2 0 0 1




output
RIGHT




input
2 3 4 5 6 6




output
NEITHER




input
-1 0 2 0 0 1




output
ALMOST












这道题是问呢，在格子阵上有三个点，他们能不能构成直角三角形，能的话呢就是Right，现在不能但是其中一个点移动一格就Right了的情况呢就是ALMOST（差一点），否则就是NEITHER，其实就简单了，不能的话枚举三个点周围的四个点共12个，枚举，回答，完毕。



关于判定，依然三角形两边平方和等于第三边平方和，而如果这个边不是水平、竖直的呢，就用这个边投影的水平竖直边平方和代替~ 也是勾股定理~~~

一眼就看完了，是个水题哟~




C++:



```cpp
#include<iostream>  
#include<algorithm>
using namespace std;  
//http://blog.csdn.net/okcd00/article/details/25349317

#define Jx(a) ((i==a)?tx:x1[a])
#define Jy(a) ((i==a)?ty:y1[a])
int m[4][2]={{0,1},{1,0},{0,-1},{-1,0}};  

bool isR(int x1,int y1,int x2,int y2,int x3,int y3)  
{  
    int a,b,c;  
    a=(x1-x2)*(x1-x2)+(y1-y2)*(y1-y2);  
    b=(x1-x3)*(x1-x3)+(y1-y3)*(y1-y3);  
    c=(x3-x2)*(x3-x2)+(y3-y2)*(y3-y2);  
    if(a==0||b==0||c==0) return 0;  
    if(a+b==c||a+c==b||b+c==a) return 1;  
    else return 0;  
}  

int main()  
{  
    int x1[4],y1[4];  
    for(int i=0;i<3;i++) cin>>x1[i]>>y1[i];  
    if(isR(x1[0],y1[0],x1[1],y1[1],x1[2],y1[2]))  
    {  
        cout<<"RIGHT"<<endl;  
        return 0;  
    }  
    int tx,ty;  
    for(int i=0;i<=3;i++)  
    {  
		for(int j=0;j<=3;j++)  
		{  
			tx=x1[i]+m[j][0];  
			ty=y1[i]+m[j][1]; 
			if( isR(Jx(0),Jy(0),Jx(1),Jy(1),Jx(2),Jy(2)) )
			{
				cout<<"ALMOST"<<endl;
				return 0;
			}
				
			/***** The Complete Version Below *****
			if(i==0 && isR(tx,ty,x1[1],y1[1],x1[2],y1[2]))  
			{  
				cout<<"ALMOST"<<endl;  
				return 0;  
			}  
			if(i==1 && isR(x1[0],y1[0],tx,ty,x1[2],y1[2]))  
			{  
				cout<<"ALMOST"<<endl;  
				return 0;  
			}  
			if(i==2 && isR(x1[0],y1[0],x1[1],y1[1],tx,ty))  
			{  
				cout<<"ALMOST"<<endl;  
				return 0;  
			}         
			 ***** The Complete Version Above *****/
		}  
        if(i==3) cout<<"NEITHER"<<endl;  
    }  
    return 0;  
}
```








