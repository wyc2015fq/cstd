# 【剑指offer】题33：数组排成最小的数 - xiaxzhou的博客 - CSDN博客





2017年07月04日 09:36:17[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：132








```cpp
bool func(const int lhs,const int rhs)
{
    long long lh(lhs), rh(rhs);
    int lk(0), rk(0);
    while (lh)
    {
        lk++;
        lh = lh / 10;
    }
    while (rh)
    {
        rk++;
        rh = rh / 10;
    }
    lh = lhs;rh = rhs;
    if (lh+rh*pow(10,lk)>rh+lh*pow(10,rk))
    {
        return true;
    }
    else
    {
        return false;
    }
}

string PrintMinNumber(vector<int> numbers)
{
    sort(numbers.begin(), numbers.end(), func);
    string str;
    for (int i = numbers.size()-1; i >= 0;--i)
    {
        int tmp = numbers[i];
        while (tmp)
        {
            int k = tmp % 10;
            tmp = tmp / 10;
            str.insert(str.begin(),'0' + k);
        }
    }
    return str ;
}
int main()
{
    vector<int> vec{ 3,32,321 };
    PrintMinNumber(vec);
#ifdef debug_
#else
#endif
    return 0;
}
```



