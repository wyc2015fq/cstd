# ACdream   1071  神奇的%系列一 - HJ - CSDN博客
2016年08月19日 20:11:49[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：216
Problem Description
在计算机的世界里，%不是百分比，而是除法取余哟！
比如： 
  4 % 2 = 0 
  5 % 3 = 2
给你 2 ≤ N ≤ 100000 个数，a[1],a[2]…a[i]…a[n] (1 ≤ a[i] ≤ 100000)。
问有几个组合 (a[i], a[j])，(i != j, a[i] > a[j])，使得 a[i] % a[j] != 0。 
Input
输入有多组数据。(<= 30)
对于每组数据：
第一行：N（表示 N 个数）
第二行：N 个元素 a[i] 
Output
输出有几个组合 (a[i],a[j])，使得 a[i] % a[j] != 0 
Sample Input
3 
1 1 1 
4 
1 2 3 4 
5 
1 2 2 4 6
Sample Output
0 
2 
1
解析：本题需要采用对立方法来做，先把所有的组合数即C（n，2）计算出来，然后对应的依次减去互相不能整除的个数，并且利用倍数的关系减少循环次数，减少时间复杂度，代码如下：
```
#include<iostream>
#include<cstring>
using namespace std;
long long a[1000005],count[1000005],n;
int main()
{ 
    while(cin>>n)
    {
        long long i,j,sum;
        memset(count,0,sizeof(count));
        for(i=1;i<=n;i++)
        {
            cin>>a[i];
            count[a[i]]++;    //数组count保存数据
        } 
        sum=n*(n-1)/2;    //先计算好C(n,2)的总数
        for(i=1;i<100000;i++)
        {
            if(count[i]==0)
              continue;
            sum=sum-(count[i]*(count[i]-1))/2;  //每次减去相对应的不能被整除的数目
            for(j=i+i;j<100000;j=j+i)
              sum-=count[i]*count[j];    //每次选择一个数就相应的把这个数的倍数的不能被整除的数目也减去，减少循环次数
        }
        cout<<sum<<endl;
    }
    return 0;
}
```
