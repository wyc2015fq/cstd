# 【剑指offer】题44：扑克牌的顺子 - xiaxzhou的博客 - CSDN博客





2017年07月05日 16:51:22[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：109








```cpp
bool IsContinuous(vector<int> numbers)
{
    if (numbers.size()<5)
    {
        return false;
    }
    sort(numbers.begin(), numbers.end());
    int min_, max_;
    auto iter = find_if(numbers.begin(), numbers.end(), [](const int & lhs){ return lhs != 0; });
    min_ = *iter;
    max_ = numbers.back();
    if (max_-min_>4)
    {
        return false;
    }
    for (; iter != numbers.end()-1;iter++)
    {
        if (*iter==*(iter+1))
        {
            return false;
        }
    }
    return true;
}
```



