# Cable master（HDU-1551） - Alex_McAvoy的博客 - CSDN博客





2018年03月15日 20:36:02[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：89
个人分类：[HDU																[查找——二分查找](https://blog.csdn.net/u011815404/article/category/8116900)](https://blog.csdn.net/u011815404/article/category/7923257)








# Problem Description

    Inhabitants of the Wonderland have decided to hold a regional programming contest. The Judging Committee has volunteered and has promised to organize the most honest contest ever. It was decided to connect computers for the contestants using a star topology - i.e. connect them all to a single central hub. To organize a truly honest contest, the Head of the Judging Committee has decreed to place all contestants evenly around the hub on an equal distance from it.

    To buy network cables, the Judging Committee has contacted a local network solutions provider with a request to sell for them a specified number of cables with equal lengths. The Judging Committee wants the cables to be as long as possible to sit contestants as far from each other as possible.

    The Cable Master of the company was assigned to the task. He knows the length of each cable in the stock up to a centimeter, and he can cut them with a centimeter precision being told the length of the pieces he must cut. However, this time, the length is not known and the Cable Master is completely puzzled.

    You are to help the Cable Master, by writing a program that will determine the maximal possible length of a cable piece that can be cut from the cables in the stock, to get the specified number of pieces.

# Input

    The input consists of several testcases. The first line of each testcase contains two integer numbers N and K, separated by a space. 

    N (1 ≤ N ≤ 10000) is the number of cables in the stock, and K (1 ≤ K ≤ 10000) is the number of requested pieces. 

   The first line is followed by N lines with one number per line, that specify the length of each cable in the stock in meters. All cables are at least 1 centimeter and at most 100 kilometers in length. All lengths in the input are written with a centimeter precision, with exactly two digits after a decimal point.

    The input is ended by line containing two 0's.

# Output

     For each testcase write to the output the maximal length (in meters) of the pieces that Cable Master may cut from the cables in the stock to get the requested number of pieces. The number must be written with a centimeter precision, with exactly two digits after a decimal point.

    If it is not possible to cut the requested number of pieces each one being at least one centimeter long, then the output must contain the single number "0.00" (without quotes).

# Sample Input

**4 11**

**    8.02    7.43    4.57**

**    5.39    0 0**

# Sample Output

**    2.00**

————————————————————————————————————————————

思路：

    二分思想类似于Problem C分馅饼的思想，关键在于scanf的输入问题。

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cmath>
using namespace std;
double a[10100];

int main()
{
    int n,m;
    int i,j;
    int num,k;
    double l,r,mid;
    double max_cable;

   	while(scanf("%d%d",&n,&k)!=EOF&&(n||k))//输入线缆数n与划分段数k
      {
        r=0;
        for(i=0;i<n;i++)
            {
                scanf("%lf",&a[i]);//输入线缆长
                max_cable=max(max_cable,a[i]);
            }

        /*二分搜索*/
        l=0;r=max_cable;//设置初值
        while(r-l>1e-5)
        {
            mid=(l+r)/2; //设置中值
            num=0;//记录分割线缆的数量
            for(i=0;i<n;i++)//记录按mid分割每条线缆时的数量
                num+=(int)(a[i]/mid);
            if(num>=k) //若分割数大于需求数
                l=mid;
            else
                r=mid;
        }
        printf("%.2f\n",floor(r*100)/100);//精度控制
     }
    return 0;
}
```






