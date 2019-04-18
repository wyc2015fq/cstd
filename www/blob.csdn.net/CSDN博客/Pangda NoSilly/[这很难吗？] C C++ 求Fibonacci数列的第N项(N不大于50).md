# [这很难吗？] C/C++ 求Fibonacci数列的第N项(N不大于50) - Pangda NoSilly - CSDN博客

2018年04月19日 13:10:26[hepangda](https://me.csdn.net/hepangda)阅读数：736


## 为什么写？

很好奇为什么这个东西有那么难吗？于是就花了点时间探索一下

## 若n未知或已知

### 使用写轮眼复制书上代码

顺便一说书上的代码十分的难看，好孩子们不要学。

```
#include<stdio.h>

void main()
{
    int i;
    long int f, f1=1, f2=1;
    printf("%10ld%10ld", f1, f2);
    for(i=3;i<=50;i++)
    {   f=f1+f2;
        printf("%10ld", f);
        f2=f;
    }
}
```

### 使用迭代

```
#include<stdio.h>

long long fibonacci(unsigned int n)
{
    long long rv[] = { 1, 1 };
    for (int i = 2; i < n; i++)
    {
        rv[1] += rv[0];
        rv[0] = rv[1] - rv[0];
    }
    return rv[1];
}

int main(int argc, char *argv[])
{
    unsigned int n;
    scanf("%u", &n);
    printf("%lld\n", fibonacci(n));
    return 0;
}
```

### 使用递归

```
#include<stdio.h>

long long fibonacci(unsigned int n)
{
    return (n <= 2) ? 1: fibonacci(n - 1) + fibonacci(n - 2);
}

int main(int argc, char *argv[])
{
    unsigned int n;
    scanf("%u", &n);
    printf("%lld\n", fibonacci(n));
    return 0;
}
```

考虑到这段代码在n较大时时间爆炸，于是可以简单的优化一下：

```
#include<stdio.h>

long long fibonacci(unsigned int n)
{
    static long long rv[51] = {0, 1, 1 };
    return (rv[n] != 0) ? rv[n]: (rv[n] = fibonacci(n - 1) + fibonacci(n - 2));
}

int main(int argc, char *argv[])
{
    unsigned int n;
    scanf("%u", &n);
    printf("%lld\n", fibonacci(n));
    return 0;
}
```

### 使用计算器、纸笔或者你的大脑

```
#include<stdio.h>

long long fibonacci[51] = {0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597,
                           2584, 4181, 6765, 10946, 17711, 28657, 46368, 75025, 121393, 196418, 317811,
                           514229, 832040, 1346269, 2178309, 3524578, 5702887, 9227465, 14930352, 
                           24157817, 39088169, 63245986, 102334155, 165580141, 267914296, 433494437,
                           701408733, 1134903170, 1836311903, 2971215073, 4807526976, 7778742049,
                           12586269025};

int main(int argc, char *argv[])
{
    unsigned int n;
    scanf("%u", &n);
    printf("%lld\n", fibonacci[n]);
    return 0;
}
```

## 若n已知

### 使用模板

```cpp
#include<iostream>

template<unsigned int V>
struct fibonacci {
    static const long long value = fibonacci<V - 1>::value + fibonacci<V - 2>::value;
};

template<>
struct fibonacci<0> {
    static const long long value = 0;
};

template<>
struct fibonacci<1> {
    static const long long value = 1;
};

int main(int argc, char *argv[])
{
    std::cout << "Fibonacci数列的第50项为: " << fibonacci<50>::value << std::endl;
    return 0;
}
```

