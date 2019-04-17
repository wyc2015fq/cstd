# 【剑指offer】题12：打印1到最大的n位数 - xiaxzhou的博客 - CSDN博客





2017年06月16日 17:36:00[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：177







- 方法1：使用vector模拟大数

```cpp
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <stack>

using namespace std;
#define debug_

class BigNum
{
public:
    BigNum(int n_):n(n_){ vec.resize(n); };
    void increase();
    bool iszero();
    void print();
private:
    vector<int> vec;
    int n;
};

void BigNum::increase()
{
    for (auto i = n - 1; i > 0;--i)
    {
         if (vec[i]<9)
         {
             vec[i]++;
             break;
         }
         else
         {
             vec[i] = 0;
         }
    }
}

bool BigNum::iszero()
{
    for (auto i = 0; i < n;++i)
    {
        if (vec[i]!=0)
        {
            return false;
        }
    }
    return true;
}

void BigNum::print()
{
    bool flag = false;
    for (auto i = 0; i < n;++i)
    {
        if (vec[i]!=0)
        {
            flag = true;
        }
        if (flag)
        {
            cout << vec[i];
        }
    }
    cout << endl;
}

void my_print(int n)
{
    if (n<=0)
    {
        return;
    }
    BigNum num(n);
    num.increase();
    while (!num.iszero())
    {
        num.print();
        num.increase();
    }
}

int main()
{
#ifdef debug_
#else
#endif
    my_print(36);

    return 0;
}
```

实际上是个求全排列的问题，递归求全排列：

```cpp
void print_core(vector<char>& vec, int k)
{
    if ( k == vec.size())
    {
        bool flag = false;
        for (auto i = 0; i < k;i++)
        {
            if (vec[i]!=0)
            {
                flag = true;
            }
            if (flag)
            {
                cout << (short)vec[i];
            }
        }
        cout << endl;
        return;
    }

    for (auto i = 0; i < 10;++i)
    { 
        vec[k] = i;
        print_core(vec, k + 1);
    }
}

void my_print(int n)
{
    if (n<=0)
    {
        return;
    }
    vector<char> vec;
    vec.resize(n);
    print_core(vec, 0);
}

int main()
{
    my_print(6);
    return 0;
}
```



