# M爷的线段树 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





# M爷的线段树 - BUCTOJ 3305

一个长度为n的数列A。
修改m次，每次给区间[L,R]中的每一个数加X。
查询k次，每次查询第i个元素的值并输出。
1<=n<=

**Input**
n m k


**Output**
k行,每行一个整数代表答案

**Sample Input**
2 1 2
4 5
1 2 3
1 2
1 2 1
0
1 1 1
1 1 -1
1
-1 -1 -1

**Sample Output**
7
8
0

#### **思路:**

一种操作技巧,可以说是前缀,要使[l,r]区间的值都加x,可以用一个add数组记录它的两端位置add[l]=x,add[r+1]=-x, 在最后都修改后遍历一次add[i]的值即为最终它比最初加的值
**注意:输入不要用cin,会超时.**



```
#include<bits/stdc++.h>
using namespace std;
int b[100005];
int main()
{
    int n,m,k;
    scanf("%d%d%d",&n,&m,&k);
    while(n != -1 || m != -1 || k != -1)
    {
        int a[100005];
        for(int i=0;i < n;i++)
            scanf("%d",&a[i]);
        memset(b,0, sizeof(b));
        while(m--)
        {
            int l,r,x;
            scanf("%d%d%d",&l,&r,&x);
            b[l-1] += x;
            b[r] += -x;
        }
        for(int i=0;i < n;i++)
        {
            b[i+1] += b[i];
            a[i] += b[i];
        }
        while(k--)
        {
            int q;
            scanf("%d",&q);
            printf("%d\n",a[q-1]);
        }
        scanf("%d%d%d",&n,&m,&k);
    }
    return 0;
}
```

可以说很gay了











