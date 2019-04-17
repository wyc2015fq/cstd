# (i++)+(++i)+(++i)+(++j)+(j++)问题 - Keith - CSDN博客





2014年02月23日 18:15:20[ke1th](https://me.csdn.net/u012436149)阅读数：786








```cpp
今天学习c++看到了volatile变量，，此关键字告诉编译器 “不知何时会改变”，防止编译器依据变量的稳定性做任何优化。而这时让我想到了那蛋疼的++ -- 问题，，所以进行了一个实验，，代码如下：
```

```cpp

```

```cpp
#include <iostream>

int main()
{
    volatile int i=0;   //用volatile 修饰i
    volatile int j=1;   // 用volatile 修饰 j

    int sum=0;
    sum=(i++)+(++i)+(++i)+(++j)+(j++);
    std::cout<<sum<<std::endl;
    return 0;
}
```


用 GNU GCC 编译器得出的结果是 7，，当不用volatile修饰变量时得到的结果是8，，

所以在编译器没有进行任何优化的情况下答案是7，，正确答案应该是7.



