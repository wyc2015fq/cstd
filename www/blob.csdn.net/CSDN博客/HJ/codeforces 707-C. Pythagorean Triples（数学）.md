# codeforces 707-C. Pythagorean Triples（数学） - HJ - CSDN博客
2017年07月11日 11:15:12[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：205
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
Katya studies in a fifth grade. Recently her class studied right triangles and the Pythagorean theorem. It appeared, that there are triples of positive integers such that you can construct a right triangle with segments of lengths corresponding to triple. Such triples are called Pythagorean triples.
For example, triples (3, 4, 5), (5, 12, 13) and (6, 8, 10) are Pythagorean triples.
Here Katya wondered if she can specify the length of some side of right triangle and find any Pythagorean triple corresponding to such length? Note that the side which length is specified can be a cathetus as well as hypotenuse.
Katya had no problems with completing this task. Will you do the same? 
Input
The only line of the input contains single integer n (1 ≤ n ≤ 109) — the length of some side of a right triangle. 
Output
Print two integers m and k (1 ≤ m, k ≤ 1018), such that n, m and k form a Pythagorean triple, in the only line.
In case if there is no any Pythagorean triple containing integer n, print  - 1 in the only line. If there are many answers, print any of them. 
Examples 
Input
3
Output
4 5
Input
6
Output
8 10
Input
1
Output
-1
Input
17
Output
144 145
Input
67
Output
2244 2245
题目大意： 
给出一个数，求出另外两个勾股数
解题方法：
设a*a+n*n=c*c  则有 n*n=c*c-a*a=(c-a)*(c+a)
令A=c+a  B=c-a  则有 n*n=A*B  
其中c=(A+B)/2  a=(A-B)/2
因此只要n*n的其中两个因子A和B满足(A+B)/2和(A-B)/2都是大于0的整数即可。
实际上，只需要作如下分类：
当n*n为偶数的时候，A=n*n/2, B=2 
当n*n为奇数的时候，A=n*n , B=1
因此得到 a,c
```cpp
#include<iostream>
using namespace std;
int main()
{
    long long n;
    while(cin>>n)
    {
        long long ans1,ans2;
        if(n<=2)
          cout<<-1<<endl;
        else
        {
            if(n*n%2)
            {
                ans1=(n*n-1)/2;
                ans2=(n*n+1)/2;
            }
            else
            {
                ans1=(n*n/2-2)/2;
                ans2=(n*n/2+2)/2;
            }
            cout<<ans1<<" "<<ans2<<endl;
        }
    }
    return 0;
}
```
