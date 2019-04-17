# 【剑指offer】题65：滑动窗口最大值 - xiaxzhou的博客 - CSDN博客





2017年09月03日 16:10:32[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：245








使用双向队列保存当前可能成为最大值的数值

```cpp
vector<int> maxInWindows(const vector<int>& num, unsigned int size)
{
    vector<int> max_vec;

    if (num.size()<size||size<1)
        return max_vec;

    deque<int> index;

    for (auto i = 0; i < num.size();++i)
    {
        int begin = i - size + 1;

        if (!index.empty()&& begin>index.front() )//注意判断是否为空
        {
            index.pop_front();
        }
        while (!index.empty()&&num[index.back()]<=num[i])//注意判断是否为空
        {
            index.pop_back();
        }
        index.push_back(i);

        if (begin>=0)
        {
            max_vec.push_back(num[index.front()]);
        }
    }
    return max_vec;
}
```



