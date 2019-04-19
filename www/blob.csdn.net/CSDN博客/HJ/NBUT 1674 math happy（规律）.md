# NBUT  1674  math happy（规律） - HJ - CSDN博客
2016年08月19日 19:58:03[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：329
个人分类：[====ACM====																[----------数学----------																[递推公式&结论题																[---------OJ---------																[杂题](https://blog.csdn.net/feizaoSYUACM/article/category/6684121)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308108)](https://blog.csdn.net/feizaoSYUACM/article/category/6130298)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Description
计算1^1+2^2+3^3…….+n^n的和的个位数
Input 
输入整数 T （T个case）
输入整型 n （n < 10^18)
Output 
输出算式的答案的个位数
Sample Input
3 
1 
2 
3
Sample Output
1 
5 
2
解析：很明显这个题是个规律的题。首先应该会很容易看出来这个值是100个循环一次，并且所有数字都跟它的最右位数字有关，因此有两种做法：
（1）对最右位数字找规律
```
#include<iostream>
using namespace std;
int main()
{
    long long n,k,i;
    cin>>k;
    while(k--)
    {
        cin>>n;
        int sum=0;
        n=n%100;
        for(i=1;i<=n;i++)
        {
            int p=i%100,q=i%10;
            if(q==5||q==6||q==0||q==1||q==9)     //只存在一种可能值，而且这个值就是它本身  
               sum=(sum+q)%10;
            if(q==4)                //只存在一种可能值，这个值为6
               sum=(sum+6)%10;
            if(q==8||q==2)           //存在两种可能值，判断的根据为该数对100取模是否能被4整除  
            {
               if(p%4==0) sum=(sum+6)%10;       //如果能被4整除则这个值为6
               else sum=(sum+4)%10;   //否则这个值为4
            }
            if(q==3)             //存在两种可能值，判断的根据为该数对100取模减一能否被4整除
            {
               if((p-1)%4==0) sum=(sum+3)%10;   //如果能则这个值为3
               else sum=(sum+7)%10;    //否则这个值为7
            }
            if(q==7)           //同上
            {
               if((p-1)%4==0) sum=(sum+7)%10;  
               else sum=(sum+3)%10;
            }
        }
        cout<<sum<<endl;
    }
    return 0;
}
```
（2）另外运行一个程序对100以内的数据打表，直接利用得到的结果
```cpp
#include<iostream>
using namespace std;
int sum[100]={1,4,7,6,5,6,3,6,9,0,1,6,3,6,5,6,7,4,9,0,1,4,7,6,5,6,3,6,9,0,1,6,3,6,5,6,7,4,9,0,1,4,7,6,5,6,3,6,9,0,1,6,3,6,5,6,7,4,9,0,1,4,7,6,5,6,3,6,9,0,1,6,3,6,5,6,7,4,9,01,4,7,6,5,6,3,6,9,0,1,6,3,6,5,6,7,4,9,0};     //通过打表得到值，直接复制粘贴过来进行调用
int ans[100];
int main()
{
    ans[0]=1;
    for(int i=1;i<100;i++)
    {
        ans[i]=ans[i-1]+sum[i];
    }
    for(int i=0;i<100;i++)ans[i]%=10;
    int T;
    cin>>T;
    while(T--)
    {
        long long n;
        cin>>n;
        if(n<=100)
            cout<<ans[n-1]<<endl;
        else cout<<ans[n%100]<<endl;
    }
    return 0;
}
```
或者可以把100个数据直接在主函数里面跑一遍，每一跑一个数据对10取一次模，并且保存到数组里面
```cpp
#include<iostream>
using namespace std;
int main()
{
    int sum=1,s=0;
    int a[101];
    for(int i=1;i<=100;i++)   //把100个数据跑一遍，得到的结果保存到数组里
    {
         sum=1;
         for(int k=1;k<=i;k++)
         {
            sum=sum*(i)%10;
         }
         s+=sum;
         a[i]=s%10;
    }
    a[0]=0;
    int T;
    cin>>T;
    while(T--)
    {
        long long n;
        cin>>n;
        if(n<100)
            cout<<a[n]<<endl;
        else cout<<a[n%100]<<endl;   //输出对100取模的值所对应的数组下标的数据值
    }
    return 0;
}
```
