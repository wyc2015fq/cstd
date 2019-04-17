# 三种常用排序算法（冒泡、选择、快速）的Java实现 - 小灰笔记 - CSDN博客





2017年03月12日 01:25:15[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：1420








       学习Java有一阵子了，现在还处于比较初级的水平，能够把简单的程序写对就不错了，更不用谈现在能够拿Java做什么了。

       学完了两段网络视频课程，接下来找本书简单看看。只要有了一个初步的认识，接下来的东西应该可以更加顺利一些。学习编程最好的方法就是练手了，现在还不知道有什么比较好的东西可以尝试，不过跑不出一般教程的排序算法还是可以尝试一下的。

1，冒泡排序算法

**package**bubble_sort;



**publicclass** bubble_sort {



**publicstaticvoid** main(String[] 
args) {

// **TODO** Auto-generated method stub

**int**[]
array_data = {1,5,3,6,2,4,7,8,4,12,55,22,99,47,12};

**int**
i,j;

**int**
array_length;



array_length = 
array_data.length;



        System.***out***.println("data before sorted:");

/* display numbers before sorted. */

**for**(i = 0;i<array_length;i++)

        {

           System.***out***.print(array_data[i] +
",");

        }



**for**(i = 0;i<array_length;i++)

        {

**for**(j =
i;j<array_length;j++)

           {

**if**(array_data[j] <
array_data[i])

               {

array_data[i] =
array_data[i] ^
array_data[j];

array_data[j] =
array_data[i] ^
array_data[j];

array_data[i] =
array_data[i] ^
array_data[j];

               }

           }

        }



        System.***out***.println();

        System.***out***.println("data after sorted:");

/* display numbers before sorted. */

**for**(i = 0;i<array_length;i++)

        {

           System.***out***.print(array_data[i] +
",");

        }

    }

}



程序运行结果;

databefore sorted:

1, 5,3, 6, 2, 4, 7, 8, 4, 12, 55, 22, 99, 47, 12, 

dataafter sorted:

1, 2, 3, 4, 4, 5, 6, 7, 8, 12, 12, 22, 47,55, 99,

    工作以来，遇到的软件问题都不是很复杂。如果需要排序算法，基本加单冒泡一下就可以了。我处理的数据量通常不大，这种简单有效的方式还是不错的。之所以先尝试这种算法，没别的，就是因为这种简单的算法使用已经成为了习惯。UNIX的编程思想不也推崇这种理念嘛——简单胜于机巧！

2，选择排序

代码：

**package**SelectSortPack;



**publicclass** SelectSort {



**publicstaticvoid** main(String[] 
args) {

// **TODO** Auto-generated method stub

**int**[]
array_data = {1,5,3,6,2,4,7,8,4,12,55,22,99,47,12};

**int**
i,j,array_length,index_min;



        System.***out***.println("data before sorted:");



array_length = 
array_data.length;



**for**(i = 0;i<
array_length;i++)

        {

           System.***out***.print(array_data[i] +
",");

        }



        System.***out***.println();



/* sort function */

**for**(i = 0;i <
array_length;i++)

        {

index_min = 
i;

**for**(j =
i + 1;j < 
array_length;j++)

           {

**if**(array_data[j] <
array_data[index_min])

               {

index_min =
j;

               }

           }

**if**(index_min !=
i)

           {

array_data[i] =
array_data[i] ^
array_data[index_min];

array_data[index_min] =
array_data[i] ^
array_data[index_min];

array_data[i] =
array_data[i] ^
array_data[index_min];

           }

        }

        System.***out***.println("data after sorted:");



**for**(i = 0;i<
array_length;i++)

        {

           System.***out***.print(array_data[i] +
",");

        }

    }



}



程序运行结果：

databefore sorted:

1, 5,3, 6, 2, 4, 7, 8, 4, 12, 55, 22, 99, 47, 12, 

dataafter sorted:

1, 2,3, 4, 4, 5, 6, 7, 8, 12, 12, 22, 47, 55, 99,

       自我感觉，选择排序和冒泡排序的设计思想差不多。不同的地方在于：选择排序是通过比较后标记数组的索引号来确定最终的交换对象。

3，快速排序

代码;

**package**QuickSortPack;



**publicclass** QuickSortClass {



**publicstaticint** Partialize(**int**[]
array,**int**low_index,**int**high_index)

    {

**int**
base_value;



base_value = 
array[low_index];



**while**(high_index >
low_index)

        {

**while**((array[high_index] >=
array[low_index])&&(high_index >
low_index))

           {

high_index -= 1;

           }

array[low_index]  = array[low_index] ^
array[high_index];

array[high_index] = 
array[low_index] ^
array[high_index];

array[low_index]  = array[low_index] ^
array[high_index];



**while**((array[low_index] <=
array[high_index])&&(high_index >
low_index))

           {

low_index += 1;

           }

array[low_index]  = array[low_index] ^
array[high_index];

array[high_index] = 
array[low_index] ^
array[high_index];

array[low_index]  = array[low_index] ^
array[high_index];

        }

array[high_index] =
base_value;

/* return the index of base_value */

**return**high_index;

    }



**publicstaticvoid** SortArray(**int**[]
array,**int**low_index,**int**high_index)

    {

**if**(low_index >=
high_index)

        {

**return**;

        }

**int**
base_value_index = *Partialize*(array,low_index,high_index);

*SortArray*(array,low_index,base_value_index - 1);

*SortArray*(array,base_value_index + 1,high_index);

    }



**publicstaticvoid** main(String[] 
args) {

// **TODO** Auto-generated method stub

**int**[]
array_data = {1,5,3,6,2,4,7,8,4,12,55,22,99,47,12};

**int**
i,array_length;



array_length = 
array_data.length;



/* display raw data */

        System.***out***.println("data before sorted:");

**for**(i = 0;i <
array_length;i++)

        {

           System.***out***.print(array_data[i] +
",");

        }



        System.***out***.println();



*SortArray*(array_data,0,array_length - 1);



/* display result data */

        System.***out***.println("data after sorted:");

**for**(i = 0;i <
array_length;i++)

        {

           System.***out***.print(array_data[i] +
",");

        }

    }



}



结果：

databefore sorted:

1, 5,3, 6, 2, 4, 7, 8, 4, 12, 55, 22, 99, 47, 12, 

dataafter sorted:

1, 2,3, 4, 4, 5, 6, 7, 8, 12, 12, 22, 47, 55, 99,

       关于快速排序算法，我查找资料重新看了好久。在好多资料以及文章面前，我有点无法确认什么叫做快速排序算法了。最终，在维基百科上找到了算是比较详细的描述。其实，快速排序算法也有很多变种，如果把多种变种混合起来理解有时候会有一些麻烦。最后，在算法实现的时候我还是挑选了一个比较容易懂的描述方式来实现。不过，这个算法的效率似乎并不是最出众的了，没有充分发挥快速排序算法的优势。

       简单对我所用的排序算法做一个小结：其实，这个跟我了解的归并排序还是有一点相似的地方的，都是对处理的数据对象进行分割处理。不同的是，快速排序是直接操作数组对象，不需要额外的临时存储空间。这样，也少了合并阶段的麻烦。

       大致的思路：首先以第一个数组元素为基准，从数组的最后面开始向前扫描。如果后面有更小的数值就与基准元素交换。接着，暂时不管扫描过得数据，再从前面扫描一次，同时做同样的操作。这样，当从前往后扫描的扫面点与从后往前的扫描点重合或者交叉时，一次分组就完成了。然后分别对交叉点左右两边的两组元素进行类似的递归操作，最后完成所有的排序。



