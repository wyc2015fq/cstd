# 牛客网——F求最大值 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





链接：[https://www.nowcoder.net/acm/contest/29/F](https://www.nowcoder.net/acm/contest/29/F)
来源：牛客网




时间限制：C/C++ 1秒，其他语言2秒

空间限制：C/C++ 32768K，其他语言65536K

 64bit IO Format: %lld



## 题目描述


给出一个序列，你的任务是求序列中 （a[j]-a[i]）/（j-i）【1<=i<j<=n】的最大值

## 输入描述:
本题包含多组输入，每组输入第一行一个数字n，表示序列的长度。
然后接下来一行输入n个数，表示原先序列的样子。
数据范围：
3<=n<=200000
-1000000000<=a[i]<=1000000000
## 输出描述:
每组数据输出一行一个浮点数，保留两位小数，表示所求的最大值。

示例1



## 输入

5
2 4 6 8 10



## 输出

2.00




## 备注:
输入只有整形。
```
#include <stdio.h>
int main()
{
    int n,i,j,k,a[200200];
    double re,t;
    while(scanf("%d",&n)!=EOF)
    {
        for(k=0;k<n;k++)
            scanf("%d",&a[k]);
        re=1.0*(a[1]-a[0]);
        for(k=1;k+1<n;k++)
            if(re<(t=1.0*(a[k+1]-a[k])))
                re=t;
        printf("%.2f\n",re);
    }
    return 0;
}
```

看过类似的题，发现都是这样，只取相邻的i,j 有点疑惑。












