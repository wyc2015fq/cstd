# 【剑指offer】题34：丑数 - xiaxzhou的博客 - CSDN博客





2017年07月04日 12:02:22[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：137








```cpp
long long GetUglyNumber_Solution(int index)
{
    if (index<1)
        return 0;
    vector<long long > vec;
    vec.resize(index);
    vec[0] = 1;
    int index2(0), index3(0), index5(0);
    for (auto i = 1; i < vec.size(); ++i)
    {
        long long ugly2 = vec[index2] * 2;
        long long ugly3 = vec[index3] * 3;
        long long ugly5 = vec[index5] * 5;

        vec[i] = min(ugly5,min(ugly3,ugly2));

        while(vec[index2]*2 <= vec[i])
        {
            index2++;
        }
        while(vec[index3] * 3 <= vec[i])
        {
            index3++;
        }
        while(vec[index5] * 5 <= vec[i])
        {
            index5++;
        }
    }
    return vec[index-1];
}
```



