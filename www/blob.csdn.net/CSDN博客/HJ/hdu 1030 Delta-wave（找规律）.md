# hdu 1030  Delta-wave（找规律） - HJ - CSDN博客
2016年07月18日 01:50:08[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：403
个人分类：[====ACM====																[----------数学----------																[递推公式&结论题																[---------OJ---------																[HDU](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308108)](https://blog.csdn.net/feizaoSYUACM/article/category/6130298)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
题目链接：[http://acm.hdu.edu.cn/showproblem.php?pid=1030](http://acm.hdu.edu.cn/showproblem.php?pid=1030)
解析：这个题目分析方法有点特别，题目大意是在这个数塔中，从一个数到另一个数可以穿过边不能穿过点，对于输入的2数，输出最少到达次数。
通过观察，数塔每层最后一位为层数的平方，对于任意的数n，我们可知其层数。数塔有正三角和倒三角，我们可以将数塔2个三角分为一组，由此，数塔左边第一组为：
```
1,3,2,6,5,11.。。。
```
第二组为：
```
4,8,7,13,12,。。。
```
右边第一组为：
```
1,3,4,8,9,。。
```
以此类推。
观察可知结果又三部分组成：
数塔层数、数塔所处左边的组数和数塔所处右边的组数
代码如下：
```cpp
#include<iostream>
#include<cmath>
using namespace std;
int Fabs(int a)
{
    return a>0?a:-a;
}
int main()
{
    int m,n,sum;
    while(cin>>m>>n)
    {
        int sm,sn,rm,rn,lm,ln;
        sm=sqrt(m);
        sn=sqrt(n);
        if(sm*sm!=m) sm++;           //确定m在level图中的哪一层 
        if(sn*sn!=n) sn++;
        rm=(m-(sm-1)*(sm-1)-1)/2+1; //确定m在right图中的哪一层
        rn=(n-(sn-1)*(sn-1)-1)/2+1;
        lm=(sm*sm-m)/2+1;          //确定m在left图中的哪一层
        ln=(sn*sn-n)/2+1;
        sum=Fabs(sm-sn)+Fabs(rm-rn)+Fabs(lm-ln);
        cout<<sum<<endl;
    }
    return 0;
}
```
