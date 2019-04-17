# 【剑指offer】题38：字数在数组中出现的次数 - xiaxzhou的博客 - CSDN博客





2017年07月04日 20:50:28[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：132
个人分类：[剑指offer																[二分查找](https://blog.csdn.net/xiaxzhou/article/category/6958605)](https://blog.csdn.net/xiaxzhou/article/category/6949916)








二分查找 

上界 

下界
```cpp
int GetLowBound(vector<int>& data, int left, int right, int k)
{
    while (left+1<right)
    {
        int mid = left + ((right - left) >> 1);
        if (data[mid]>=k)
        {
            right = mid;
        }
        else
        {
            left = mid;
        }
    }
    if (data[left] == k)
    {
        return left;
    }
    else if (data[right] == k)
    {
        return right;
    }
    else
    {
        return -1;
    }
}
int GetHighBound(vector<int>& data, int left, int right, int k)
{
    while (left + 1 < right)
    {
        int mid = left + ((right - left) >> 1);
        if (data[mid] > k)
        {
            right = mid;
        }
        else
        {
            left = mid;
        }
    }
    if (data[right] == k)
    {
        return right;
    }
    else if (data[left] == k)
    {
        return left;
    }
    else
    {
        return -1;
    }
}
int GetNumberOfK(vector<int>& data, int k)
{
    if (data.size()==0)
    {
        return 0;
    }
    int left = GetLowBound(data, 0, data.size() - 1, k);
    int right = GetHighBound(data, 0, data.size() - 1, k);
    if (left == -1)
    {
        return 0;
    }
    return right - left + 1;
}
```





