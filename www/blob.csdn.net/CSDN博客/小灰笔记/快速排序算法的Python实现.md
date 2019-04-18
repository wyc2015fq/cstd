# 快速排序算法的Python实现 - 小灰笔记 - CSDN博客





2017年03月13日 22:56:36[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：425








       网络上多次看到8大排序算法的Python实现这样的文章，我也不知道收藏了多少次了，但是似乎从来没有看完过。虽说，这些基本的排序算法我掌握的并不是很熟练，有些甚至可以说不会。

       趁着最近几天学习Java又接触到了相应的排序算法，赶紧把快速排序算法给补了一下。接下来，用Python再进行一次练手。

       代码如下：

#!/usr/bin/python



defPartialize(array,low_index,high_index):

       base_value = array[low_index]



       while(low_index < high_index):

             while((low_index < high_index)and (array[high_index] >= base_value)):

                     high_index -= 1

              array[low_index],array[high_index]= array[high_index],array[low_index]

              while((low_index < high_index)and (array[low_index] <= base_value)):

                     low_index += 1

              array[low_index],array[high_index]= array[high_index],array[low_index]

       array[high_index] = base_value

       return high_index



defQuickSort(array,low_index,high_index):

       if low_index < high_index:

              index =Partialize(array,low_index,high_index)

              QuickSort(array,low_index,index -1)

              QuickSort(array,index +1,high_index)



test_data =[1,5,3,6,2,4]

print("databefore sorted:")

print(test_data)

QuickSort(test_data,0,len(test_data)- 1)

print("dataafter sorted:")

print(test_data)

       运行结果如下;

data beforesorted:

[1, 5, 3, 6, 2, 4]

data after sorted:

[1, 2, 3, 4, 5, 6]

       相比C或者是Java，Python的代码确实是简洁好多。其实，由于脚本语言的特性，Python的代码兼容性应该也是最好的，可以处理各种数据。而C或者是Java如果想要兼容多种数据类型的处理就还需要进行更多的工作。



