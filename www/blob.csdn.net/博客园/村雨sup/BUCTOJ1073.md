# BUCTOJ1073 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
#include "iostream"
#include "algorithm"
using namespace std;
const int N = 7;
struct Time
{
    int a;
    int b;
    int c;
    int d;
};

bool cmp(const Time &A,const Time &B)
{
    if(A.c > B.c)
        return 1;
    if(A.c == B.c)
        return A.d < B.d;
    else
        return 0;

}


int main()
{
    Time time[N];
    int i;
    for( i=0;i<7;i++)
    {
        cin >> time[i].a >>time[i].b;
        time[i].c = time[i].a + time[i].b;
        time[i].d = i;
    }
    sort(time,time+7,cmp);

    if(time[0].c > 8)
        cout << time[0].d+1;
    else
        cout << "0";




    return 0;

}
```

第一：如何在排序的时候保留原先的下标

第二：如何在排序时增加当相等时的判断条件











