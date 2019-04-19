# 51nod  1305 Pairwise Sum and Divide（数学） - HJ - CSDN博客
2017年05月05日 14:49:17[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：403
有这样一段程序，fun会对整数数组A进行求值，其中Floor表示向下取整：
fun(A) 
    sum = 0 
    for i = 1 to A.length 
        for j = i+1 to A.length 
            sum = sum + Floor((A[i]+A[j])/(A[i]*A[j]))  
    return sum
给出数组A，由你来计算fun(A)的结果。例如：A = {1, 4, 1}，fun(A) = [5/4] + [2/1] + [5/4] = 1 + 2 + 1 = 4。 
Input
第1行：1个数N，表示数组A的长度(1 <= N <= 100000)。 
第2 - N + 1行：每行1个数A[i]（1 <= A[i] <= 10^9)。
Output
输出fun(A)的计算结果。
Input示例
3 
1 4 1
Output示例
4
因为是对结果向下取整，你会发现，只有1跟2做了贡献，其余的数字结合都是0。数字1与其他数字结合得到1，与数字1结合得到2，数字2与数字2结合得到1，与其他数字结合得到0.
```cpp
#include<iostream>
using namespace std;
int main()
{
    int n;
    while(cin>>n)
    {
        long long sum1=0,sum2=0,x;
        for(int i=0;i<n;i++)
        {
            cin>>x;
            if(x==1)
              sum1++;
            else if(x==2)
              sum2++;
        }
        long long ans=sum1*(n-1)+sum2*(sum2-1)/2;
        cout<<ans<<endl;
    }
    return 0;
}
```
