# 快速排序算法的C语言实现 - 小灰笔记 - CSDN博客





2017年03月13日 22:40:54[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：497








       快速排序算法是对冒泡排序算法的改进，速度上有了很大提升。变种也非常多，今天就比较容易理解的一种方式进行C语言的代码实现示范。

       C语言代码如下：

#include"stdio.h"



int Partialize(int*array,int low_index,int high_index);

void QuickSort(int*array,int low_index,int high_index);



int test_array[] ={1,5,3,6,2,4};



int Partialize(int*array,int low_index,int     high_index)

{

       int base_value = array[low_index];



       while(high_index > low_index)

       {

              while((high_index >low_index)&&(array[high_index] >= base_value))

              {

                     high_index -= 1;

              }

              array[low_index]  = array[low_index] ^ array[high_index];

              array[high_index] = array[low_index]^ array[high_index];

              array[low_index]  = array[low_index] ^ array[high_index];



              while((high_index >low_index)&&(array[low_index] <= base_value))

              {

                     low_index += 1;

              }

              array[low_index]  = array[low_index] ^ array[high_index];

              array[high_index] =array[low_index] ^ array[high_index];

              array[low_index]  = array[low_index] ^ array[high_index];

       }

       array[high_index] = base_value;



       return high_index;

}



void QuickSort(int*array,int low_index,int high_index)

{

       int index = 0;



       if(low_index < high_index)

       {

              index =Partialize(array,low_index,high_index);

              QuickSort(array,low_index,index -1);

              QuickSort(array,index +1,high_index);      

       }

       else

       {

              /* nothing */

       }

}



int main(void)

{

       int i = 0;

       int array_length = 0;



       array_length =sizeof(test_array)/sizeof(int);



       printf("data beforesorted:\n");

       for(i = 0;i<array_length;i++)

       {

              printf("%d,",test_array[i]);

       }

       printf("\n");

       printf("data after sorted:\n");

       QuickSort(test_array,0,array_length - 1);



       for(i = 0;i<array_length;i++)

       {

              printf("%d,",test_array[i]);

       }

       return 0;

}



       编译运行结果：

E:\WorkSpace\01_编程语言\01_C语言\01_算法\01_排序>gcc quick_sort.c



E:\WorkSpace\01_编程语言\01_C语言\01_算法\01_排序>a

data beforesorted:

1, 5, 3, 6, 2, 4,

data after sorted:

1, 2, 3, 4, 5, 6,

E:\WorkSpace\01_编程语言\01_C语言\01_算法\01_排序>



       经过在Java学习过程中对这个算法的学习总结，现在总算是有一点熟练的感觉了。后期使用Python再实现一下试试看，毕竟如今Python也是我比较常用的一个工具。



