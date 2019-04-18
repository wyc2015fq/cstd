# 用c++画空心圆 - writeeee的专栏 - CSDN博客
2015年07月29日 22:52:40[writeeee](https://me.csdn.net/writeeee)阅读数：3920标签：[空心圆-c](https://so.csdn.net/so/search/s.do?q=空心圆-c&t=blog)
个人分类：[c++](https://blog.csdn.net/writeeee/article/category/5634855)
```cpp
#include <STDIO.H>
#include <iostream>
#include <MATH.H>
using namespace std;
int main()
{
    int c1=80;
    int r=10;
    cin>>r;
    int r2=r*r;
    int i=0,j=0;
    for (j=c1;j>-c1;j--)
    {
        double v=2.2*sqrt(r2-j*j);
        double v1=c1-v;
        double v2=c1+v;
        if (j*j>r2)
        {
            printf("\n");
            continue;
        }
        for (i=0;i<v1;i++)
        {
            printf("%c",' ');
        }
        printf("%c",'*');
        for (i=v1;i<c1;i++)
        {
            printf("%c",' ');
        }
        //================
        for (i=c1;i<v2;i++)
        {
            printf("%c",' ');
        }
        printf("%c",'*');
        for (i=v2;i<c1+r;i++)
        {
            printf("%c",' ');
        }
        printf("\n");
    }
    return 0;
}
```
