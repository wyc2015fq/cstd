# Python3冒泡排序 - zhusongziye的博客 - CSDN博客





2018年01月20日 14:08:48[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：237








### 概述


冒泡排序（Bubble Sort），是一种计算机科学领域的较简单的排序算法。


它重复地走访过要排序的数列，一次比较两个元素，如果他们的顺序错误就把他们交换过来。


走访数列的工作是重复地进行直到没有再需要交换，也就是说该数列已经排序完成。




### 算法原理


冒泡排序算法的运作如下：
- 

比较相邻的元素。如果第一个比第二个大，就交换他们两个。

- 

对每一对相邻元素作同样的工作，从开始第一对到结尾的最后一对。在这一点，最后的元素应该会是最大的数。

- 

针对所有的元素重复以上的步骤，除了最后一个。

- 

持续每次对越来越少的元素重复上面的步骤，直到没有任何一对数字需要比较。





### 代码



# -*- coding：utf-8 -*-

__author__ ="苦叶子"

import random


# 随机生成1-1000之间无序序列整数数据

defgenerator():
    random_data =[]for i in range(0,10):
        random_data.append(random.randint(1,1000))return random_data

# 冒泡排序
defbubble_sort(data_list):# 序列长度
    lenght = len(data_list)for i in range(0, lenght):for j in range(i+1, lenght):if data_list[i]> data_list[j]:
                data_list[i], data_list[j]= data_list[j], data_list[i]return data_list

if __name__ =="__main__":print("开源优测-积微速成计划基本功提升")# 生成随机无序数据
    random_data = generator()# 打印无序数据print(random_data)# 插入排序
    sorted_data = bubble_sort(random_data)# 打印排序结果print(sorted_data)


