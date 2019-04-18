# NSGA 2 学习笔记 -- 快速排序 - cocoonyang的专栏 - CSDN博客





2013年01月02日 02:48:48[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：887








NSGA 2 使用了随机快速排序算法。

C. A. R. Hoare在1962年为了改进冒泡排序算法提出快速排序（Quicksort）算法。其基本思想是递归、分治。其时间复杂度为Ο(n log n)，最差时间复杂度为Ο(n^2)。


算法的基本流程为：首先根据队列中元素的大小分割为两组，其中一组的所有元素都比另一组中的元素小；然后再分别对这两组数据重复执行分割操作。


快速排序伪代码如下：


QUICKSORT(A，left，right)

    if left >= right return;

    choose one element as Pivot


    遍历队列中所有元素，将小于Pivot放到第一组，将不小于Pivot的元素放到第二组。

    分别对这两组应用QUICKSORT排序。

网上有很多关于快速排序算法的分析资料。这里仅以一个小例子演示一下分组操作。

现有一组随机数据如下所示：

202
392171 277443933

对这组数据队列执行分割操作，首先设定一个基准，一般选择队首或队尾（这里选取队尾元素）：

pivot = 33

然后逐一遍历队列元素，操作过程如下所示：




```
theVector[0] = 20
exchange 0 with 0
20 2 39 21 71 2 77 44 39 33

theVector[1] = 2
exchange 1 with 1
20 2 39 21 71 2 77 44 39 33

theVector[3] = 21
exchange 2 with 3
20 2 21 39 71 2 77 44 39 33 

theVector[5] = 2
exchange 3 with 5
20 2 21 2 71 39 77 44 39 33
```


基于Hoare的快速排序算法，又有研究人员从基准选择、分组数量等方面对快速排序算法加改进。其中影响快速排序算法效率的重要因素是基准的选择。如果基准选择不好，会使新生成的两个队列中元素的数量差别较大，达不到分治的目的。一般的快速排序算法是取队首（或队尾）元素作为排序基准。当目标队列是有序的情况下，每次分组都会得到最差的分组结果。一种常见的改进方法是在队列中随机选取一个元素作为基准，是为随机快速排序算法。

NSGA 2 中使用就是随机快速排序算法。具体代码如下：




```cpp
/* Actual implementation of the randomized quick sort used to sort a population based on crowding distance */
void q_sort_dist(population *pop, int *dist, int left, int right)
{
    int index;
    int temp;
    int i, j;
    double pivot;
    if (left<right)
    {
        index = rnd (left, right);
        temp = dist[right];
        dist[right] = dist[index];
        dist[index] = temp;
        pivot = pop->ind[dist[right]].crowd_dist;
        i = left-1;
        for (j=left; j<right; j++)
        {
            if (pop->ind[dist[j]].crowd_dist <= pivot)
            {
                i+=1;
                temp = dist[j];
                dist[j] = dist[i];
                dist[i] = temp;
            }
        }
        index=i+1;
        temp = dist[index];
        dist[index] = dist[right];
        dist[right] = temp;
        q_sort_dist (pop, dist, left, index-1);
        q_sort_dist (pop, dist, index+1, right);
    }
    return;
}
```








