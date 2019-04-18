# 如何在C++中动态分配二维数组 - ljx0305的专栏 - CSDN博客
2008年11月09日 22:16:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：700
                这个问题应该是我以前在CSDN蹭分时回答次数比较多的一个问题了，我的回答一般是三种方法：(1)用vector的vector，(2)先分配一个指针 数组，然后让里面每一个指针再指向一个数组，这个做法的好处是访问数组元素时比较直观，可以用a[x][y]这样的写法，缺点是它相当于C#中的一个锯齿 数组，内存空间不连续。(3)直接分配一个x*y大小的一维数组，这样保证空间是连续的，但访问数组元素不直观。对于我这个“经典”回答，我那时还一直是 挺得意的，至少从蹭分的角度来看，这样回答还是很有效的。 
　　今天在ChinaUnix论坛闲逛时看到一个贴子，再次证明了我在C++方面才疏学浅。
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void **darray_new(int row, int col, int size)
{
        void **arr;
        arr = (void **) malloc(sizeof(void *) * row + size * row * col);
        if (arr != NULL)
        {
                void *head;
                head = (void *) arr + sizeof(void *) * row;
                memset(arr, 0, sizeof(void *) * row + size * row * col);
                while (row--)
                        arr[row] = head + size * row * col;
        }
        return arr;
}
void darray_free(void **arr)
{
        if (arr != NULL)
                free(arr);
}
嗯，连续分配内存，而且可以用a[x][y]的方式来访问！可谓二维数组动态分配的绝妙方法！这段程序是C的，似乎要改成支持对象分配的C++版也不是什么难事（不过估计得用上placement new吧，嗯，需要再思考一下……）。
2007-06-13 12:38 补充:
　　经过试验，C++版出炉了：）关键点还是在于placement new和显示的析构函数调用，用于保证对象可以正常的构造和析构。
　　这个实现也还是有不少缺点的，比如，数组的大小必须记住，才能保证析构所有对象。不过这点可以通过改进分配方法算法，把数组大小也用一点空间保存起来。
　　另一个缺点是，从语法上看，很容易让人误把darray_new返回的指针以为是数据区的起始地址，从而可能导致一些逻辑错误。
#include <iostream>
#include <cstdlib>
#include <new>
template <typename T>
T **darray_new(int row, int col)
{
    int size = sizeof(T);
    void **arr = (void **) malloc(sizeof(void *) * row + size * row * col);
    if (arr != NULL)
    {
        unsigned char * head;
        head = (unsigned char *) arr + sizeof(void *) * row;
        for (int i = 0; i < row; ++i)
        {
            arr[i] =  head + size * i * col;
            for (int j = 0; j < col; ++j)
                new (head + size * (i * col + j)) T;
        }
    }
    return (T**) arr;
}
template <typename T>
void darray_free(T **arr, int row, int col)
{
    for (int i = 0; i < row; ++i)
        for (int j = 0; j < col; ++j)
            arr[i][j].~T();
    if (arr != NULL)
        free((void **)arr);
}
2007-06-13 21:00补充
本文仅为技术层面的讨论，实践中考虑用boost::multi_array之类的现成的解决方案可能会更有效。
引用：[http://blog.csdn.net/lifanxi/archive/2007/06/13/1649840.aspx](http://blog.csdn.net/lifanxi/archive/2007/06/13/1649840.aspx)
