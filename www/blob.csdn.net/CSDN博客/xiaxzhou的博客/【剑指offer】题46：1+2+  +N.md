# 【剑指offer】题46：1+2+...+N - xiaxzhou的博客 - CSDN博客





2017年07月06日 13:05:02[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：204







- 利用&&逻辑短路

```
int Sum_Solution(int n)
{
    int k(0);
    n && (k = Sum_Solution(n - 1));
    return n + k;
}
```
- 利用构造函数和静态变量

```cpp
class sum{
public:
    static int s;
    static int k;
    sum(){ k++; s += k; };

};
int sum::s(0);
int sum::k(0);

int Sum_Solution(int n)
{
    vector<sum> tmp(n);
    return sum::s;
}

int main()
{
    cout<<Sum_Solution(5)<<endl;
    return 0;
}
```
- 利用模板：非类型模板参数

```cpp
template<int T> 
class sum_solve
{
public:
    int N = sum_solve<T - 1>().N+T;
};
template<>
class sum_solve<0>
{
public:
    int N = 0;
};


int main()
{
    sum_solve<5> s;
    cout << s.N << endl;
    return 0;
}
```



