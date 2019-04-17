# 【剑指offer】题51：数组中重复的数字 - xiaxzhou的博客 - CSDN博客





2017年07月06日 16:14:44[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：115








```cpp
bool duplicate(int numbers[], int length, int* duplication) 
{
    if (numbers==NULL||length<2||duplication==NULL)
    {
        return false;
    }
    vector<int> vec(length);
    for (auto i = 0; i < length;++i)
    {
        if (vec[numbers[i]] != 0)
        {
            *duplication = numbers[i];
            return true;
        }
        else
        {
            vec[numbers[i]] = 1;
        }
    }
    return false;
}
```

```
bool duplicate(int numbers[], int length, int* duplication) 
{
    if (numbers==NULL||length<2||duplication==NULL)
    {
        return false;
    }
    for (auto i = 0; i < length;)
    {
        if (numbers[i] != i)
        {
            int j = numbers[i];
            if (numbers[i] == numbers[j])
            {
                *duplication = numbers[i];
                return true;
            }
            else
            {
                std::swap(numbers[i], numbers[j]);
            }
        }
        else
        {
            i++;
        }
    }
    return false;
}
```



