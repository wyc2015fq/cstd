# 136  Ugly Numbers （priority_queue模拟 拓展） - PeterBishop - CSDN博客





2018年07月11日 11:26:53[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：18
个人分类：[UVA](https://blog.csdn.net/qq_40061421/article/category/7793583)









```cpp
#include <iostream>
#include <set>
#include <queue>
#define LL long long
using namespace std;
 //丑数都可以用1*x 2*x 3*x 5*x表示，要做的就是用priority_queue来入队自动优先值排序再出队（就是优先值最小的），对最小的重复操作

//这里的greater表示优先队列的规则,值越小的优先级越大
priority_queue<LL, vector<LL>,greater<LL> > pq;
//集合具有元素唯一性.其他特性这里用不到
set<LL> s;
 
int main()
{
    pq.push(1);
    int number=1;
    while(number<1500)      //此循环执行1499次
    {
        number++;
        LL t=pq.top();      //取优先队列队首元素(即最小值)
        pq.pop();           //队首出列,更新队首元素
        //队首的倍数在集合中找不到时才把这个数压进队列,以及加入集合
        if(!s.count(t*2)){pq.push(t*2);s.insert(t*2);}
        if(!s.count(t*3)){pq.push(t*3);s.insert(t*3);}
        if(!s.count(t*5)){pq.push(t*5);s.insert(t*5);}
    }
    //已经有1499个队首出列了,那么接下来这个队首就是第1500个丑数了
    cout << "The 1500'th ugly number is " << pq.top() << ".\n";
    return 0;
}
```
[丑数的拓展这篇写的很好，转载](https://www.jianshu.com/p/8c188508c8f2)


