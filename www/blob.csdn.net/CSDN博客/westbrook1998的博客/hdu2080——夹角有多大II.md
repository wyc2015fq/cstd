# hdu2080——夹角有多大II - westbrook1998的博客 - CSDN博客





2018年05月11日 14:34:26[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：129标签：[算法																[几何题																[数学题																[简单题](https://so.csdn.net/so/search/s.do?q=简单题&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)





> 
这次xhd面临的问题是这样的：在一个平面内有两个点，求两个点分别和原点的连线的夹角的大小。  

  注：夹角的范围[0，180]，两个点不会在圆心出现。  

  Input 

  输入数据的第一行是一个数据T，表示有T组数据。  

  每组数据有四个实数x1,y1,x2,y2分别表示两个点的坐标,这些实数的范围是[-10000,10000]。  

  Output 

  对于每组输入数据，输出夹角的大小精确到小数点后两位。  

  Sample Input 

  2 

  1 1 2 2 

  1 1 1 0 

  Sample Output 

  0.00 

  45.00
一道比较简单的数学题，几何题，要注意的就是`acos`这个函数返回值是弧度，要求角度需要`*180/PI`，`PI`最好自己定义一个，用`M_PI`不知道为何编译错误

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cmath>
using namespace std;
const double PI=3.1415926535;
double getLineLen(double x1,double y1,double x2,double y2){
    return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

int main(void){
    int t;
    scanf("%d",&t);
    while(t--){
        double x1,y1,x2,y2;
        scanf("%lf%lf%lf%lf",&x1,&y1,&x2,&y2);
        double lines[3];
        double xl=getLineLen(x1,y1,0.0,0.0);
        double yl=getLineLen(x2,y2,0.0,0.0);
        double zl=getLineLen(x1,y1,x2,y2);
        double ans=(xl*xl+yl*yl-zl*zl)/(2*xl*yl);
        printf("%.2lf\n",acos(ans)*180/PI);
    }
    return 0;
}
```](https://so.csdn.net/so/search/s.do?q=数学题&t=blog)](https://so.csdn.net/so/search/s.do?q=几何题&t=blog)](https://so.csdn.net/so/search/s.do?q=算法&t=blog)




