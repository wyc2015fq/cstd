# 用STL对一组数组进行排序和去重 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
#include <iostream>
#include<cmath>
#include "algorithm"
#include "cstdio"
#include "stack"
using namespace std;
bool cmp(const int a, const int b)
{
    return a<b;
}

int main()
{
    int n;
    cin >> n;
    int a[n];
    for(int i=0;i<n;i++)
    {
        cin >> a[i];
    }
    sort(a,a+n);
    int m = unique(a,a+n)-a;//地址的差值正好为数组中数据的个数
    cout << m <<endl;
    for(int i=0;i<m;i++)
    {
        cout << a[i] << " ";
    }
    cout << endl;

    return 0;
}
```

第一：sort不加函数就是按从小到大的顺序进行快排。

第二：unique一般都是用于sort后的去重，实际上是把重复的元素丢到了最后面，返回的是没有重复元素数组的最后一个元素的地址。











