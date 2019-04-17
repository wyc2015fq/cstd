# 【剑指offer】题52：构建乘积数组 - xiaxzhou的博客 - CSDN博客





2017年07月06日 19:25:29[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：106








注意反序遍历数组时

> 
auto i = vec.size()-1;   


i 为无符号整形

```cpp
vector<int> multiply(const vector<int>& A)
{
    vector<int> re(A.size());
    if (A.size()<=1)
    {
        return re;
    }
    vector<int> C(A.size(),1);
    vector<int> D(A.size(),1);
    for (auto i = 1; i < A.size();++i)
    {
        C[i] = C[i - 1] * A[i - 1];
    }
    for (int i = A.size()-2; i >= 0;--i)//auto -> unsigned int
    {
        D[i] = D[i + 1] * A[i + 1];
    }
    for (auto i = 0; i < A.size();++i)
    {
        re[i] = C[i] * D[i];
    }
    return re;
}
```



