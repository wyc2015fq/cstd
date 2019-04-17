# 堆排序的C语言实现以及和快排的比较 - Machine Learning with Peppa - CSDN博客





2018年03月05日 22:29:25[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：145
所属专栏：[C语言入门](https://blog.csdn.net/column/details/18792.html)









昨天看了堆排序的文章，自己根据模板实现了一下，并且在原本的基础上做了一些改动，主要思想还是二叉树的排序和堆的生成，具体的算法不再详谈。

有趣的是，之前看见有人说堆排序和快排同为不稳定排序，其效率却相差较大的原因是**堆排序浪费的开销**太多，我一直不太懂。直到自己实现了一遍才明白为什么：

每次交换头尾结点之后，除了堆顶结点外的所有元素还要再做一次元素排序，一共做了N-1次，虽然每次的数量减少，且总的开销是线性的，不影响复杂度上界，但常数因子绝对是很大的。这也是为什么快排在数据量大时明显优于堆排序的原因了。

好了，不多废话，代码如下，C99测试通过

```cpp
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

//给定父节点的索引，得到左子节点的索引
#ifndef LeftChild(i)
#define LeftChild(i) (2*(i)+1)
#endif // LeftChild

//交换两个元素的位置
void _swap(int *num1, int *num2)
{
    int tmp = *num1;
    *num1 = *num2;
    *num2 = tmp;
}

//元素下沉方法
void PercDown(int A[], int i, int N)
{
    //子节点的索引号
    int child;
    //存储当前父节点元素的临时变量
    //(注：每一个节点都可以看作是其子树的根节点)

    for (; LeftChild(i) < N; i = child)
    {
        child = LeftChild(i);
        //挑选出左、右子节点中较大者
        if (child != N-1 && A[child+1]>A[child])
        {

            child++;
        }

        //比较当前父节点与较大子节点
        if (A[i] < A[child])
        {
            //交换当前父节点处的元素值与较大子节点的元素值
            /**tmp= A[i];
            A[i] = A[child];
            A[child] = tmp;
            **/
            _swap(&A[i], &A[child]);
        }
        else
        {
            break;
        }

    }
}

//堆排序
void HeapSort(int A[], int N)
{
    int i;
    //步骤一：创建大根堆
    for (i  = N/2;  i>=0; i--)
    {
        PercDown(A, i, N);

    }
    //步骤二：调整大根堆
    for ( i = N-1; i > 0; i--)
    {
         //首尾交换
        _swap(&A[0], &A[i]);
        //元素下沉
        PercDown(A, 0, i);
    }
}


//主函数
void main()
{
    //int A[6] = {4,5,3,2,6,1};

    register int i;
    int A[6];
    printf("Please enter the number to sort:\n");
    for(i = 0; i < 6; i++)
    {
        scanf("%d",&A[i]);
    }
    HeapSort(A, 6);
    printf("The rank is:\n");

    for(i = 0; i < 6; i++)
    {
        printf("%d\n",A[i]);
    }
}
```

测试结果：

![](https://img-blog.csdn.net/20180305222747682?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzk1MjE1NTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




