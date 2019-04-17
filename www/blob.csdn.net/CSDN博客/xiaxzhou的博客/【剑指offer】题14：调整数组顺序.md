# 【剑指offer】题14：调整数组顺序 - xiaxzhou的博客 - CSDN博客





2017年06月22日 20:12:57[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：163








> 
输入一个整数数组，实现一个函数来调整该数组中数字的顺序，使得所有的奇数位于数组的前半部分，所有的偶数位于位于数组的后半部分，并保证奇数和奇数，偶数和偶数之间的相对位置不变

- **函数指针**的声明
- 将函数指针作为参数传递
- 使用**位操作**判断奇偶性

```cpp
bool isodd(int p)
{
    return (p & 1 != 0);
}

void func(vector<int>& array,bool (*pfun)(int))
{
    if (array.size()<2)
    {
        return;
    }
    int count(0);
    for (auto i = 0; i < array.size(); ++i)
    {
        if (!pfun(array[i]))
        {
            count++;
        }
        else
        {
            for (auto j = 0; j < count;++j)
            {
                std::swap(array[i - j], array[i - j - 1]);
            }
        }
    }
}
```



