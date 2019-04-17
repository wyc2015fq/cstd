# 【剑指offer】题31：最大连续数组之和 - xiaxzhou的博客 - CSDN博客





2017年07月03日 15:57:53[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：90
个人分类：[剑指offer](https://blog.csdn.net/xiaxzhou/article/category/6949916)









```cpp
int FindGreatestSumOfSubArray(vector<int> array)
{
    assert(array.size()>0);
    if(array.size()==1)
    {return array[0];}
    int my_max(array[0]);
    for (auto i = 1; i < array.size();++i)
    {
        if (array[i-1]>0)
        {
            array[i] = array[i] + array[i - 1];
        }
        my_max = max(my_max, array[i]);
    }
    return my_max;
}
```



