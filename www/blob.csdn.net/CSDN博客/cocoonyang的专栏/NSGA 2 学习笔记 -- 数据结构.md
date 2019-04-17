# NSGA 2 学习笔记 -- 数据结构 - cocoonyang的专栏 - CSDN博客





2013年01月02日 02:07:07[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：2560











概述

NSGA2是一种基于非支配排序的遗传算法，可用于求解多目标优化问题[1]。

在NSGA2中，种群初始化后， 基于非支配排序方法，种群中的个体被分成多个前沿组。第一个前沿组中的个体是完全非支配个体，它们的rank值被赋为1。第二个前沿组中个体受第一个前沿组中的个体支配，它们的rank值被赋为2。其余前沿组中个体依次类推 。

NSGA2引入拥挤距离（crowding distance）作为评判个体与相邻个体间的远近的指标。拥挤距离越大说明种群里个体越分散。

NSGA2使用双锦标赛制的方式，根据基因个体的rank值和拥挤距离从种群中筛选个体。

NSGA2使用的设计变量分为两种：实数型设计变量和二进制型设计变量。在代码中，分别由nreal和nbin保存这两种设计变量的数量。二进制型设计变量可以用于处理离散型设计变量，用户需为每个二进制型设计变量设置二进制数的位数（相应于单个离散型设计变量的所有可取值的数量），用户同时要设置二进制型设计变量的上下限。NSGA2默认为离散型设计变量均布于二进制型设计变量上下限之间。  

下面首先介绍在NSGA2代码中使用的两个重要的数据结构。本文使用的NSGA2代码版本是： Revision 1.1.6 (08 July 2011)[2] 

NSGA2中，最重要的数据结构莫过于基因个体与基因种群，他们的定义在文件global.h中。



```cpp
typedef struct
{
    int rank;  /*Rank of the individual*/
    double constr_violation;
    double *xreal;  /*list of real variables*/
    int **gene;  /*bianry chromosome*/
    double *xbin;  /*list of decoded value of the chromosome */
    double *obj;  /* Objectives values*/
    double *constr;  /*Constraints values*/
    double crowd_dist;
} individual;
```
个体数据结构保存 优化模型的实例数据。



种群数据结构包含一个个体数据结构的数组指针ind。数组的长度有变量popsize设定。



```cpp
typedef struct
{
    individual *ind;
} population;
```


Revision 1.1.6 (08 July 2011)[2]版本的NSGA2代码是使用C语言编写的，在使用个体数据结构和种群数据结构时，涉及了计算机内存操作，因此， 文件allocate.c中的allocate_memory_pop（） 函数为种群数据结构申请内存空间。





```cpp
/* Function to allocate memory to a population */
void allocate_memory_pop (population *pop, int size)
{
    int i;
    pop->ind = (individual *)malloc(size*sizeof(individual));
    for (i=0; i<size; i++)
    {
        allocate_memory_ind (&(pop->ind[i]));
    }
    return;
}
```



其中的allocate_memory_ind（） 函数是为个体数据结构申请内存空间。



```cpp
/* Function to allocate memory to an individual */
void allocate_memory_ind (individual *ind)
{
    int j;
    if (nreal != 0)
    {
        ind->xreal = (double *)malloc(nreal*sizeof(double));
    }
    if (nbin != 0)
    {
        ind->xbin = (double *)malloc(nbin*sizeof(double));
        ind->gene = (int **)malloc(nbin*sizeof(int *));
        for (j=0; j<nbin; j++)
        {
            ind->gene[j] = (int *)malloc(nbits[j]*sizeof(int)); /* nbits[j]个整型数,每一个整型数代表一个二进制数位 */
        }
    }
    ind->obj = (double *)malloc(nobj*sizeof(double));
    if (ncon != 0)
    {
        ind->constr = (double *)malloc(ncon*sizeof(double));
    }
    return;
}
```













参考文献：

[1]   ARAVIND SESHADRI. A FAST ELITIST MULTIOBJECTIVE GENETIC ALGORITHM: NSGA-II.


[2]  NSGA Code. http://www.iitk.ac.in/kangal/codes.shtml




