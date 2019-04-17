# 【面经笔记】堆排序与topk问题 - xiaxzhou的博客 - CSDN博客





2017年07月11日 11:53:03[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：635








### 最大堆的数组实现

#### 维持堆函数：

**h为新元素位置，假设h的两个孩子树已经是最大堆**

1、 如果D[h]为D[h]、左孩子、右孩子中的最大值，则结束调整。 

2、 否则，将X[h]与孩子中的最大值互换，更新h值，返回上一步

```cpp
void KeepHeap(int *  array, int len, int h)
{
    int lagest = h;
    int leftchild = (h << 1) + 1;//注意移位操作符的优先级低
    int rightchild = (h << 1) + 2;

    if (leftchild < len && array[h] < array[leftchild])
    {
        lagest = leftchild;
    }
    if (rightchild < len && array[lagest] < array[rightchild])
    {
        lagest = rightchild;
    }
    if (lagest == h)
    {
        return;
    }
    else
    {
        std::swap(array[h], array[lagest]);
        KeepHeap(array, len, lagest);
    }
}
```

> 
二叉树使用数组实现时，节点下标的关系：


如果从下标从1开始存储，则编号为i的结点的主要关系为： 

双亲：下取整 （i/2） 

左孩子：2i 

右孩子：2i+1
如果从下标从0开始存储，则编号为i的结点的主要关系为： 

双亲：下取整 （(i-1)/2） 

左孩子：2i+1 

右孩子：2i+2
#### 建堆函数：

自底向上建堆，先对左右子树建堆，然后**对根节点使用维持堆函数**

```cpp
void MakeHeap(int *  array, int len)
{
    if (array==NULL || len <= 1)
    {
        return;
    }
    int k = (len - 1) >> 1;
    for (auto i = k; i >= 0; --i)//利用keepheap建堆
    {
        KeepHeap(array, len, i);
    }
}
```

#### 建堆的时间复杂度是多少？

O（n）

#### 堆弹出函数：

将根节点与数组最后一个元素互换后，减少堆节点数，重新维护堆

```
void Pop(int *  array,int len)
{
    std::swap(array[len - 1], array[0]);
    KeepHeap(array,len-1,0);
}
```

#### 堆排序：

先建堆，然后逐个弹出

```
void HeapSort(int * array,int len)
{
    MakeHeap(array, len);
    for (auto i = len; i > 1;--i)
    {
        Pop(array, i);
    }
}
```

topk问题：

先建立k大小的最小堆

然后每次将新对象值与堆顶比较，如果比堆顶大，则用新值替换堆顶，并重新维持堆





