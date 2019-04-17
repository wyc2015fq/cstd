# 【剑指offer】题64：数据流中的中位数 - xiaxzhou的博客 - CSDN博客





2017年09月03日 14:55:51[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：172








STL适配器：

heap、priority_queue 默认是 vector 实现的最大堆

stack，queue默认是 dequeue 实现的

**注意top，pop时，需判断空的情况！**

试用一个最大堆和一个最小堆，保持两个最大堆中所有数小于最小堆，且保证两个堆元素个数相差不超过1。

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <queue>
#include <functional>
using namespace std;

//stl中，栈默认使用双端队列实现，堆和优先队列默认使用vector实现
priority_queue<int, vector<int>, less<int>> min_que;
priority_queue<int, vector<int>, greater<int>> max_que;

void Insert(int num)
{
    if (min_que.size() < max_que.size())
    {
        min_que.push(num);
    }
    else
    {
        max_que.push(num);
    }
    if ((!min_que.empty() && !max_que.empty()) && min_que.top() > max_que.top())//注意判断是否为空
    {
        int tmp1 = min_que.top();
        int tmp2 = max_que.top();
        min_que.pop();
        max_que.pop();
        min_que.push(tmp2);
        max_que.push(tmp1);
    }
}

double GetMedian()
{
    //注意判断为空的情况
    if (min_que.empty()&&max_que.empty())
    {
        return -1;
    }
    if (min_que.empty())
    {
        return max_que.top();
    }
    else if (max_que.empty())
    {
        return min_que.top();
    }

    if (min_que.size() == max_que.size())
    {
        return ((double)(min_que.top() + max_que.top()) / 2);
    }
    else if (min_que.size() < max_que.size())
    {
        return max_que.top();
    }
    else
    {
        return min_que.top();
    }
}

int main()
{
    cout<<GetMedian()<<endl;
    for (auto i = 0; i < 100; i++)
    {
        Insert(i);
        cout << GetMedian() << endl;
    }
    return 0;
}
```



