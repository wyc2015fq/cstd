# 快速排序算法C++实现[评注版] - 心纯净，行致远 - CSDN博客





2018年12月11日 14:16:59[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：52
个人分类：[【算法】](https://blog.csdn.net/zhanshen112/article/category/7916677)









经常看到有人在网上发快速排序的算法，通常情况下这些人是在准备找工作，或者看<算法导论>这本书，而在他们发布的代码通常是差不多的版本，估计也是网上copy一下，自己改改，跑过了就算了，但是通常这样玩根本没有太大作用，如果到一家公司，给你一台不能上网的笔记本，20分钟，你是根本写不出来快速排序的算法的，当然除了那些死记硬背的兄弟。

说说我写这篇文章的目的吧，记得有一天我想重新看看<算法导论>，看到快速排序我觉得很简单，于是按奈不住，想动手写写，可是写完了，在测试有些数据的时候总也过不去，于是我就想在网上找找按照<算法导论>的提示逻辑写成的快速排序，但是很是失望，网上差不多都是同一个版本，而且不是我想要的，于是有了本文。

为了让本文自成体系，先看看什么是快速排序，快速排序是一种排序算法。在平均状况下，排序n 个项目要Ο(n log n)次比较。在最坏状况下则需要Ο(n2)次比较，但这种况并不常见。事实上，快速排序通常明显比其他Ο(n log n) 演算法更快，因为它的内部循环（inner loop）可以在大部分的架构上很有效率地被实作出来，且在大部分真实世界的资料，可以决定设计的选择，减少所需时间的二次方项之可能性。



首先让我们来看看<算法导论>上面的算法逻辑

QUICKSORT(A, p, r)//快速排序算法

 if (p < r )

{

q = PARTITION(A, p, r)//分成左右两半，一半不大于A[r], 一半不小于A[r]

QUICKSORT(A, p, q-1)//递归左半

QUICKSORT(A, q+1, r) //递归右半

}



PARTITION(A, p, r)

x = A[r]//选择最后一个元素作为比较元素

i = p – 1//这个慢速移动下标必须设定为比最小下表p小1，否则两个元素的序列比如2，1无法交换

**for **j = p **to **r-1//遍历每个元素

{

**if (**A[j] <= x)//比较

**{**

i = i + 1//移动慢速下标

Exchange A[i] with A[j ]//交换

}

}

Exchange A[i+1] with A[r]//交换



**return **i + 1//返回分割点





一次完整的比较过程如下图：

![](https://images.cnblogs.com/cnblogs_com/pugang/QuickSort.JPG)



算法导论快速排序逻辑C++实现

//Data swop function

void Swap(int &p,int &q)                          

{                                                      

     int temp = p;

       p=q;

       q=temp;

} 



//Partition function

int Partition(int ArrayInput[],int nLow,int nHigh)                

{                                                 



    int nTemp=ArrayInput[nHigh];   

    int i = nLow-1, j=nLow;  

       for(; j<nHigh; j++)

       {

              if( ArrayInput[j]<=nTemp )

              {

                     i++;

                     if(i !=j )

                     {

                           Swap(ArrayInput[i], ArrayInput[j]);

                     }

              }

       }



       Swap(ArrayInput[i+1],ArrayInput[nHigh]);



    return (i+1);                                        

}



//Quick sort

void Quick_sort(int ArrayInput[],int nLow,int nHigh)            

{                                                                                                       

    if(nLow < nHigh)                                        

    {                                                

        int nIndex=Partition(ArrayInput , nLow, nHigh);                         

        Quick_sort(ArrayInput , nLow, nIndex-1);                           

        Quick_sort(ArrayInput , nIndex+1, nHigh);                           

    }                                                 

}



**总结**

本文对<算法导论>的快速排序算法实现的关键点进行了详细的阐述，另外，本文给出了严格按照，<算法导论>快速排序算法逻辑实现的C++快速排序算法，希望对大家有所帮助。



