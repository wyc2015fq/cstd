# NSGA 2 学习笔记 -- crowding distance - cocoonyang的专栏 - CSDN博客





2013年01月02日 02:08:47[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：3989








NSGA-II基于Pareto占优关系对种群中的个体分级，在每一级中计算每个个体在种群中的密度指标，根据占优关系和密度指标对种群中全部个体排序，然后选择相对较好的个体进入下一代迭代循环。常用的密度指标评估方法有拥挤距离(crowding distance)，k-最近邻(k-nearest neighbor), epsilon占优(epsilon-domination) 、网格(gading)等。NSGA-II中使用的是拥挤距离。拥挤距离是描述基因个体与其相邻个体之间拥挤度的指标，拥挤距离越大表明种群中个体的分布越分散（“The
 crowding distance is a measure of how close an individual is to its neighbors. Large average crowding distance will result in better diversity in the population.”）[1]。


计算拥挤距离的函数assign_crowding_distance_list 在源文件crowddist.c中定义。它接受三个参数，分别是：

    一个population 指针pop，指针pop指向一个种群；

    一个list 指针lst，指针lst指向一个前沿队列； 

    一个int型参数 front_size，front_size保存前沿队列中元素的数量。

在数据结构list 的定义中，

```cpp
typedef struct lists
{
    int index;
    struct lists *parent;
    struct lists *child;
} list;
```


每一个list 队列中的元素保存一个整型数。在函数assign_crowding_distance_list这里，队列*lst保存的是前沿队列中基因个体在种群中的序列号。在此，指针lst的功用是作为前沿队列的目录，以此在种群中检索各前沿队列中的基因个体。



函数assign_crowding_distance_list （）具体实现代码如下所示：



```cpp
/* Routine to compute crowding distance based on ojbective function values when the population in in the form of a list */
void assign_crowding_distance_list (population *pop, list *lst, int front_size)
{
    int **obj_array;
    int *dist;
    int i, j;
    list *temp;
    temp = lst;
    if (front_size==1)
    {
        pop->ind[lst->index].crowd_dist = INF;
        return;
    }
    if (front_size==2)
    {
        pop->ind[lst->index].crowd_dist = INF;
        pop->ind[lst->child->index].crowd_dist = INF;
        return;
    }
    obj_array = (int **)malloc(nobj*sizeof(int*));
    dist = (int *)malloc(front_size*sizeof(int));
    for (i=0; i<nobj; i++)
    {
        obj_array[i] = (int *)malloc(front_size*sizeof(int));
    }
    for (j=0; j<front_size; j++)
    {
        dist[j] = temp->index;
        temp = temp->child;
    }
    assign_crowding_distance (pop, dist, obj_array, front_size);
    free (dist);
    for (i=0; i<nobj; i++)
    {
        free (obj_array[i]);
    }
    free (obj_array);
    return;
}
```


文献[1] 中给出了计算拥挤距离的算法，但是具体的代码实现却和文献中的描述有些差异。在代码中，算法的实现如下所述：


    1）如果前沿中个体数量不超过3个，就将每个个体的拥挤距离设置为INF（根据[1]中所述对队列边界上的个体 做特殊处理，它们的拥挤距离均被赋值为无穷）。

    2）创建规模为nobj*front_size的obj_array矩阵, 长度为front_size的dist数组，为它们申请内存。


    3）初始化dist数组： 将队列 lst中各元素的index赋给dist数组。


    4）调用函数assign_crowding_distance（）计算 拥挤距离。

    5）释放内存

第1，2，4，5步都是C语言编程中的寻常套路，下面继续跟进到函数assign_crowding_distance()中。这个函数接受四个参数：

    一个population 指针pop ---------  种群对象；


   一个int型指针dist --------  前沿队列中各元素的index值，相当于前沿队列的替身；


   一个int指针型指针obj_array   --  一个矩阵；


  一个int型 参数front_size --  前沿的规模，保存前沿队列中元素的数量。



```cpp
/* Routine to compute crowding distances */
void assign_crowding_distance (population *pop, int *dist, int **obj_array, int front_size)
{
    int i, j;
    for (i=0; i<nobj; i++)
    {
        for (j=0; j<front_size; j++)
        {
            obj_array[i][j] = dist[j];
        }
        quicksort_front_obj (pop, i, obj_array[i], front_size);
    }
    for (j=0; j<front_size; j++)
    {
        pop->ind[dist[j]].crowd_dist = 0.0;
    }
    for (i=0; i<nobj; i++)
    {
        pop->ind[obj_array[i][0]].crowd_dist = INF;
    }
    for (i=0; i<nobj; i++)
    {
        for (j=1; j<front_size-1; j++)
        {
            if (pop->ind[obj_array[i][j]].crowd_dist != INF)
            {
                if (pop->ind[obj_array[i][front_size-1]].obj[i] == pop->ind[obj_array[i][0]].obj[i])
                {
                    pop->ind[obj_array[i][j]].crowd_dist += 0.0;
                }
                else
                {
                    pop->ind[obj_array[i][j]].crowd_dist += (pop->ind[obj_array[i][j+1]].obj[i] - pop->ind[obj_array[i][j-1]].obj[i])/(pop->ind[obj_array[i][front_size-1]].obj[i] - pop->ind[obj_array[i][0]].obj[i]);
                }
            }
        }
    }
    for (j=0; j<front_size; j++)
    {
        if (pop->ind[dist[j]].crowd_dist != INF)
        {
            pop->ind[dist[j]].crowd_dist = (pop->ind[dist[j]].crowd_dist)/nobj;
        }
    }
    return;
}
```

算法的实现如下所述：

  1）根据每个目标函数值调用q_sort_front_obj函数对前沿队列中的个体进行排序，并将排序结果保存在obj_array 矩阵中。

  2）（“initialize the distance to be zero for all the individuals i.e. Fi(dj) = 0, where j corresponds to the jth individual in front Fi.”[1]）。

  3）将排序后位于前沿队列左边界位置的个体的拥挤距离设置为INF（根据[1]中所述对队列边界上的个体 做特殊处理，它们的拥挤距离均被赋值为无穷）。

  4）计算每个个体的拥挤距离。

  5）将排序后位于前沿队列右边界位置的个体的拥挤距离设置为INF。



拥挤距离背后的基本思想是寻找前沿队列中每个个体的之间的欧氏距离。一个个体就是一个以目标函数值为坐标的m维空间（m为目标函数的数量）点（"The basic idea behind the crowing distance is finding the euclidian distance between each individual in a front based on their m objectives in the m dimensional hyper space. The individuals
 in the boundary are always selected since they have infinite distance assignment."）[1]。

计算公式如下：

    I(dk) = I(dk) + ( I(k+1).m  - I(k-1).m ) / ( f_max_m - f_min_m )   


其中，位于前沿边界上个体的拥挤距离设为无穷大。






















参考文献

[1]   ARAVIND SESHADRI. A FAST ELITIST MULTIOBJECTIVE GENETIC ALGORITHM: NSGA-II




﻿﻿



