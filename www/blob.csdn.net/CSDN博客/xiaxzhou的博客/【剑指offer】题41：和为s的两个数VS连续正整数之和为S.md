# 【剑指offer】题41：和为s的两个数VS连续正整数之和为S - xiaxzhou的博客 - CSDN博客





2017年07月04日 22:49:51[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：94
个人分类：[剑指offer](https://blog.csdn.net/xiaxzhou/article/category/6949916)









```cpp
vector<int> FindNumbersWithSum(vector<int> array, int sum)
{
    int multi(999999);
    vector<int> vec(2);
    int left(0), right(array.size() - 1);
    while (left<right)
    {
        int tmp = array[left] + array[right];
        if (tmp<sum)
        {
            left++;
        }else if (tmp>sum)
        {
            right--;
        }
        else
        {
            if (multi>(array[left]+array[right]))
            {
                vec[0] = array[left];
                vec[1] = array[right];
                multi = array[left] + array[right];
            }
            left++;
            right--;
        }
    }
    if (multi ==999999)
    {
        vec.clear();
    }
    return vec;
}
```

```cpp
vector<vector<int> > FindContinuousSequence(int sum)
{
    vector<vector<int>> vec;
    if (sum<3)
    {
        return vec;
    }
    int left(1), right(2);
    int tmp(3);
    while (left<right)
    {
        if (sum>tmp)
        {
            tmp += (++right);
        }
        else if (sum<tmp)
        {
            tmp -= (left++);
        }
        else
        {
            vector<int> v;
            for (auto i = left; i <= right;++i)
            {
                v.push_back(i);
            }
            vec.push_back(v);
            tmp -= (left++);
        }
    }
    return vec;
}
```



