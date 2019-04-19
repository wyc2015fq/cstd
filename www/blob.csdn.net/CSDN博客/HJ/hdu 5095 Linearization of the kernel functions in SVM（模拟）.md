# hdu  5095  Linearization of the kernel functions in SVM（模拟） - HJ - CSDN博客
2017年01月31日 03:07:51[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：344
个人分类：[====ACM====																[---------OJ---------](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Problem Description 
SVM（Support Vector Machine）is an important classification tool, which has a wide range of applications in cluster analysis, community division and so on. SVM The kernel functions used in SVM have many forms. Here we only discuss the function of the form f(x,y,z) = ax^2 + by^2 + cy^2 + dxy + eyz + fzx + gx + hy + iz + j. By introducing new variables p, q, r, u, v, w, the linearization of the function f(x,y,z) is realized by setting the correspondence x^2 <-> p, y^2 <-> q, z^2 <-> r, xy <-> u, yz <-> v, zx <-> w and the function f(x,y,z) = ax^2 + by^2 + cy^2 + dxy + eyz + fzx + gx + hy + iz + j can be written as g(p,q,r,u,v,w,x,y,z) = ap + bq + cr + du + ev + fw + gx + hy + iz + j, which is a linear function with 9 variables.
Now your task is to write a program to change f into g.
Input 
The input of the first line is an integer T, which is the number of test data (T<120). Then T data follows. For each data, there are 10 integer numbers on one line, which are the coefficients and constant a, b, c, d, e, f, g, h, i, j of the function f(x,y,z) = ax^2 + by^2 + cy^2 + dxy + eyz + fzx + gx + hy + iz + j.
Output 
For each input function, print its correspondent linear function with 9 variables in conventional way on one line.
Sample Input
2 
0 46 3 4 -5 -22 -8 -32 24 27 
2 31 -5 0 0 12 0 0 -49 12
Sample Output
46q+3r+4u-5v-22w-8x-32y+24z+27 
2p+31q-5r+12w-49z+12
Source 
2014上海全国邀请赛——题目重现（感谢上海大学提供题目） 
解题思路： 
需要注意三点即头一个数为正的话没有加号、序数为1或者-1的话不能有1因为有字母（没有字母譬如最后一个则要加上），序数为0跳过不加以考虑。
代码如下：
```cpp
#include<iostream>
#include<cstdio>
using namespace std;
int main()
{
    int t,a[15];
    char s[]={'p','q','r','u','v','w','x','y','z'};
    cin>>t;
    while(t--)
    {
        for(int i=0;i<10;i++)
          cin>>a[i];
        int blag=0;
        for(int i=0;i<9;i++)
        {
            if(a[i])      //非0 
            {
                if(blag)      //判断是一个数 
                {
                    if(a[i]>0)   
                    {
                        if(a[i]==1)  
                          printf("+");
                        else  
                          printf("+%d",a[i]);
                    }
                    else
                    {
                        if(a[i]==-1)
                          printf("-");
                        else
                          printf("%d",a[i]);
                    }
                }
                else     //如果不是第一个数 
                {
                    if(a[i]!=1)   //序号如果为1则不用输出1,因此只需要考虑其他情况  上同 
                    {
                        if(a[i]==-1)    //序号为-1不能有1只能保留'-' 
                          printf("-");
                        else
                          printf("%d",a[i]);
                    }
                }
                printf("%c",s[i]);
                blag=1;
            }
        }
        if(blag&&a[9]>0)     //最后一个需要拿出来单独考虑（因为后面没有字母，就算序号为1或者-1同样原样输出 
          printf("+%d\n",a[9]);
        else if(a[9])
          printf("%d\n",a[9]);
        else
          printf("\n");
    }
    return 0;
}
```
