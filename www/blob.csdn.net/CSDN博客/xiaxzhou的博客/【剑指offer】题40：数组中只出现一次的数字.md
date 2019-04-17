# 【剑指offer】题40：数组中只出现一次的数字 - xiaxzhou的博客 - CSDN博客





2017年07月04日 22:10:58[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：95
个人分类：[剑指offer](https://blog.csdn.net/xiaxzhou/article/category/6949916)









```cpp
void FindNumsAppearOnce(vector<int> data, int* num1, int *num2)
{
    if (data.size()<2)
    {
        return;
    }
    int N(data[0]);
    for (auto i = 1; i < data.size();++i)
    {
        N = N^data[i];
    }
    int k(0);
    while (!((N>>k)&1))
    {
        k++;
    }
    int Ncopy(N);
    for (auto i = 0; i < data.size();++i)
    {
        if ((data[i]>>k)&1)
        {
            N = N^data[i];
        }
    }
    *num1 = N;
    *num2 = N^Ncopy;
}
```



