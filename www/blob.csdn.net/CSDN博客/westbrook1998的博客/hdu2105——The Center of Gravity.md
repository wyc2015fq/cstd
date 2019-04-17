# hdu2105——The Center of Gravity - westbrook1998的博客 - CSDN博客





2018年05月13日 14:15:15[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：48








> 
Everyone know the story that how Newton discovered the Universal Gravitation. One day, Newton walked  

  leisurely, suddenly, an apple hit his head. Then Newton discovered the Universal Gravitation.From then  

  on,people have sovled many problems by the the theory of the Universal Gravitation. What’s more, wo also  

  have known every object has its Center of Gravity.  

  Now,you have been given the coordinates of three points of a triangle. Can you calculate the center  

  of gravity of the triangle? 

  Input 

  The first line is an integer n,which is the number of test cases.  

  Then n lines follow. Each line has 6 numbers x1,y1,x2,y2,x3,y3,which are the coordinates of three points.  

  The input is terminated by n = 0. 

  Output 

  For each case, print the coordinate, accurate up to 1 decimal places. 

  Sample Input 

  2 

  1.0 2.0 3.0 4.0 5.0 2.0 

  1.0 1.0 4.0 1.0 1.0 5.0 

  0 

  Sample Output 

  3.0 2.7 

  2.0 2.3
简单到不能再简单的一道题了

代码：

```cpp
#include <cstdio>
using namespace std;
int main(void){
    int t;
    while(~scanf("%d",&t)){
        if(0==t){
            break;
        }
        while(t--){
            double x1,y1,x2,y2,x3,y3;
            scanf("%lf%lf%lf%lf%lf%lf",&x1,&y1,&x2,&y2,&x3,&y3);
            printf("%.1lf %.1lf\n",(x1+x2+x3)/3,(y1+y2+y3)/3);
        }
    }
    return 0;
}
```





