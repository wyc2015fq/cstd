# codeforces-755D-PolandBall and Polygon（数学题） - HJ - CSDN博客
2017年01月17日 03:22:26[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：695
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
PolandBall has such a convex polygon with n veritces that no three of its diagonals intersect at the same point. PolandBall decided to improve it and draw some red segments.
He chose a number k such that gcd(n, k) = 1. Vertices of the polygon are numbered from 1 to n in a clockwise way. PolandBall repeats the following process n times, starting from the vertex 1:
Assume you’ve ended last operation in vertex x (consider x = 1 if it is the first operation). Draw a new segment from vertex x to k-th next vertex in clockwise direction. This is a vertex x + k or x + k - n depending on which of these is a valid index of polygon’s vertex.
Your task is to calculate number of polygon’s sections after each drawing. A section is a clear area inside the polygon bounded with drawn diagonals or the polygon’s sides. 
Input
There are only two numbers in the input: n and k (5 ≤ n ≤ 106, 2 ≤ k ≤ n - 2, gcd(n, k) = 1). 
Output
You should print n values separated by spaces. The i-th value should represent number of polygon’s sections after drawing first i lines. 
Examples 
Input
5 2
Output
2 3 5 8 11 
Input
10 3
Output
2 3 4 6 9 12 16 21 26 31 
Note
The greatest common divisor (gcd) of two integers a and b is the largest positive integer that divides both a and b without a remainder.
For the first sample testcase, you should output “2 3 5 8 11”. Pictures below correspond to situations after drawing lines. 
![这里写图片描述](http://codeforces.com/predownloaded/33/de/33de87d500c2f587988d5c0f86e1af173bf4227e.png)
![这里写图片描述](http://codeforces.com/predownloaded/5c/d0/5cd059d6a34bc4d34e9980ea56b9394f6ea929b6.png)
![这里写图片描述](http://codeforces.com/predownloaded/1a/0f/1a0f27678edfb803f7a60c2f7ea25feaf3b80f9a.png)
![这里写图片描述](http://codeforces.com/predownloaded/ab/0d/ab0dbd4291753344fda575c0f2cbe238e5304626.png)
![这里写图片描述](http://codeforces.com/predownloaded/cb/d0/cbd0c456c1bf9e00bb08ac4ae776710eb9d9348f.png)
![这里写图片描述](http://codeforces.com/predownloaded/98/d5/98d5cd46decf854ba7abe0a70ee796fe0e01fe2a.png)
重要提示： 
    对递增的数据进行取整n，用数组保存，可以找出规律。
```cpp
#include<iostream>
using namespace std;
long long b[1000005],c[1000005],d[1000005];
int main()
{
    long long n,k,i,j;
    while(cin>>n>>k)
    {
        k=k<=n/2?k:n-k;    //先做出判断，k对于n前后具有对称性 
        b[0]=1,c[0]=0;
        for(i=1;i<=n;i++)
        {
            b[i]=b[i-1]+k;       //把递增的数据存在b[]中 
            c[i]=(b[i]-1)/n;     //把数据取整n的结果保存在c[]中 
        }
        int blag=0,t=0;    //blag用来标记在c[i]==c[i-1]的条件下c[i-1]==c[i-2]？，t用来累加d[i]数据 
        d[0]=1;
        for(i=1;i<=n;i++)
        {
            if(c[i]==c[i-1])
            {
                if(!blag)    //假如c[i]==c[i-1]并且c[i-1]!=c[i-2] 
                {
                    blag=1;
                    t++;     //t要多累加一次
                }
            }
            else
            {
                if(blag)   //如果出现了c[i]!=c[i-1]，blag初始化为0，方便进行下一次判断 
                  blag=0;
                t++;
            }
            d[i]=d[i-1]+t;   //每次累加的数据用d[i]来保存  
        }
        for(i=1;i<n;i++)
          cout<<d[i]<<" ";
        cout<<n*k+1<<endl;
    }
    return 0;
}
```
