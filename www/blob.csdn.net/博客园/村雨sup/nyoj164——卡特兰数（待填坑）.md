# nyoj164——卡特兰数（待填坑） - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





题意：将1~2n个数按照顺时针排列好，用一条线将两个数字连接起来要求：线之间不能有交点，同一个点只允许被连一次。

最后问给出一个n，有多少种方式满足条件。

卡特兰数（列）：

令h(0)=1,h(1)=1，catalan数满足递推式：h(n)= h(0)*h(n-1)+h(1)*h(n-2) + ... + h(n-1)h(0) (n>=2)
例如：h(2)=h(0)*h(1)+h(1)*h(0)=1*1+1*1=2               h(3)=h(0)*h(2)+h(1)*h(1)+h(2)*h(0)=1*2+1*1+2*1=5
另类递推式：        h(n)=h(n-1)*(4*n-2)/(n+1);
递推关系的解为： h(n)=C(2n,n)/(n+1) (n=0,1,2,...)
递推关系的另类解为：    h(n)=c(2n,n)-c(2n,n-1)(n=0,1,2,...)

一些方面的应用：

1. 括号化:矩阵连乘:P=a1×a2×a3×……×an，依据乘法结合律，不改变其顺序，只用括号表示成对的乘积，试问有几种括号化的方案？(h(n-1)种)
2.一个栈(无穷大)的进栈序列为1，2，3，…，n，有多少个不同的出栈序列?
3.给定n个点求能组成的二叉树所有总数。
4. 凸多边形三角划分（任意两顶点之间的连线必能相交），求有多少中分割的方法（类似：在圆上选择2n个点,将这些点成对连接起来使得所得到的n条线段不相交的方法数） 
5. n层阶梯切割为n个矩形的切割方法总数

代码

```
#include<bits/stdc++.h>
using namespace std;
typedef long long  ll;
const int maxn = 1000000;
const int moder = 1000000;
int a[105][100];
void catalan()
//求卡特兰数,a[i][j]存储的是第i个逆序（高位在后）的卡特兰数（从0开始），且未对高位0进行处理
{
    int i, j, len, carry, temp;
    a[1][0] = 1;
    len = 1;
    for(i = 2; i <= 100; i++)
    {
        for(j = 0; j < len; j++) //乘法
            a[i][j] = a[i-1][j]*(4*(i-1)+2);
        carry = 0;
        for(j = 0; j < len; j++) //处理相乘结果
        {
            temp = a[i][j] + carry;
            a[i][j] = temp % 10;
            carry = temp / 10;
        }
        while(carry) //进位处理
        {
            a[i][len++] = carry % 10;
            carry /= 10;
        }
        carry = 0;
        for(j = len-1; j >= 0; j--) //除法
        {
            temp = carry*10 + a[i][j];
            a[i][j] = temp/(i+1);
            carry = temp%(i+1);
        }
    }
}
int main()
{
    int n;
    catalan() ;
    while(scanf("%d",&n) ,n != -1)
    {
        int flag = 0 ;
        for(int i = 99;i >= 0;i--)//处理高位
        {
            if(a[n][i] != 0)
                flag = 1;
            if(flag)
                printf("%d",a[n][i]);
        }
        printf("\n");
    }
    return 0;
}
```

————不是很懂











