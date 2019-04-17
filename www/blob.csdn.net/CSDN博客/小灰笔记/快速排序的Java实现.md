# 快速排序的Java实现 - 小灰笔记 - CSDN博客





2017年03月12日 20:19:56[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：179








       之前整理过一份学习笔记放到了博客上，但是算法是借用了他人的成果，是看了网上多篇文章之后又写出来的。写完的时候还遇到一点小问题，最终的纠正方式也是参考网络文章。今天再次整理一下相关的内容，主要是针对自己曾经有一点迷惑的地方进行思考总结。

       之前的代码：

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
array_data = {1,5,3,6,2,4,7,8,4,12,55,22,99,47,12,53};

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

       编译后的运行结果：

databefore sorted:

1, 5,3, 6, 2, 4, 7, 8, 4, 12, 55, 22, 99, 47, 12, 53, 

dataafter sorted:

1, 2, 3, 4, 4, 5, 6, 7, 8, 12, 12, 22, 47,53, 55, 99,

    总体的分治思想我清楚了，但是一直没有弄明白最后为什么要给array_data[high_index]进行检查后的赋值。后来自己做了一个简单的待排序演示之后一步步看到最后终于明白，道理其实很简单——最后while循环结束的条件应该是low_index和hingh_index重合的时候。而这次重合退出的时候，重合点的数据存储实际上是一个未就绪状态。而这个地方其实应该存储的是最初用于比较的基准数值。既然两个index重合，其实这里对哪一个进行赋值都能够达到最终的效果。接下来验证一下想法，修改代码如下：

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

array[low_index] =
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
array_data = {1,5,3,6,2,4,7,8,4,12,55,22,99,47,12,53};

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

       编译运行的结果如下：

databefore sorted:

1, 5,3, 6, 2, 4, 7, 8, 4, 12, 55, 22, 99, 47, 12, 53, 

dataafter sorted:

1, 2, 3, 4, 4, 5, 6, 7, 8, 12, 12, 22, 47,53, 55, 99,

    从上面的结果来看，最终两个程序的运行结果是一致的。也就是说，最后的赋值并不是因为之前这个数值被修改了需要恢复，而是重合点本身就是一种未就绪的状态。



