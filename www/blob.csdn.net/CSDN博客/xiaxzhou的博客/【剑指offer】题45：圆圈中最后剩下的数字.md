# 【剑指offer】题45：圆圈中最后剩下的数字 - xiaxzhou的博客 - CSDN博客





2017年07月06日 12:22:10[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：110








```
int LastRemaining_Solution(int n, int m)
{
    if (n<=1||m<1)
    {
        return -1;
    }
    vector<int> vec(n);
    int index(-1);
    int count(n-1);
    while (count)
    {
        int k(m);
        while (k)
        {
            index++;
            index = index%n;
            if (vec[index]!=-1)
            {
                k--;
            }
        }
        vec[index] = -1;
        count--;
    }
    for (auto i = 0; i < vec.size();++i)
    {
        if (!vec[i])
        {
            return vec[i];
        }
    }
    return -1;
}
```



