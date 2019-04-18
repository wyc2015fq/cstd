# 现代 C++ 中的异步操作 - 文章 - 伯乐在线
原文出处： [忆先，2016-10-14](https://segmentfault.com/a/1190000007165195)
**背景**
老实说，C++11中对异步的支持不必Python和JavaScript弱。
实现（代码）


```
#include <iostream>
#include <vector>
#include <algorithm>
#include <future>
template <typename RAIter>
int parallel_sum(RAIter front, RAIter beg, RAIter end)
{
    auto len = end - beg;
    if (len < 1000)
    {
        char out[50];
        sprintf(out, "Calculate from %d to %d\n", beg - front, end - front);
        std::cout << out;
        return std::accumulate(beg, end, 0);
    }
    RAIter mid = beg + len / 2;
    auto handle_right = std::async(std::launch::async,
                             parallel_sum<RAIter>, front, mid, end);
    auto handle_left = std::async(std::launch::async,
                             parallel_sum<RAIter>, front, beg, mid);
    return handle_left.get() + handle_right.get();
}
int main()
{
    std::vector<int> v(20000, 1);
    std::cout << "The sum is " << parallel_sum(v.begin(), v.begin(), v.end()) << '\n';
}
```
** 输出**
可能一


```
Calculate from 8125 to 8750                                                                                                               
Calculate from 18125 to 18750                                                                                                             
Calculate from 10625 to 11250                                                                                                             
Calculate from 2500 to 3125                                                                                                               
Calculate from 13750 to 14375                                                                                                             
Calculate from 5000 to 5625                                                                                                               
Calculate from 1250 to 1875                                                                                                               
Calculate from 3750 to 4375                                                                                                               
Calculate from 16250 to 16875                                                                                                             
Calculate from 17500 to 18125                                                                                                             
Calculate from 10000 to 10625                                                                                                             
Calculate from 625 to 1250                                                                                                                
Calculate from 18750 to 19375                                                                                                             
Calculate from 15625 to 16250                                                                                                             
Calculate from 3125 to 3750                                                                                                               
Calculate from 12500 to 13125                                                                                                             
The sum is 20000
```
可能二


```
Calculate from 1875 to 2500                                                                                                               
Calculate from 3750 to 4375                                                                                                               
Calculate from 8750 to 9375                                                                                                               
Calculate from 7500 to 8125                                                                                                               
Calculate from 15000 to 15625                                                                                                             
Calculate from 10000 to 10625                                                                                                             
Calculate from 1250 to 1875                                                                                                               
Calculate from 6875 to 7500                                                                                                               
Calculate from 0 to 625                                                                                                                   
Calculate from 3125 to 3750                                                                                                               
Calculate from 5625 to 6250                                                                                                               
Calculate from 5000 to 5625                                                                                                               
Calculate from 6250 to 6875                                                                                                               
Calculate from 2500 to 3125                                                                                                               
The sum is 20000
```
可能….
…..
**注意**
如果在输出中间信息的时候不先现在字符串里输出会很乱，因为同一行cout输出不同项的操作不是原子的，中间可能会转到其他线程去。
