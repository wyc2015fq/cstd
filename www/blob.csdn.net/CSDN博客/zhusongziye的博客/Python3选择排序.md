# Python3选择排序 - zhusongziye的博客 - CSDN博客





2018年01月20日 14:13:26[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：280








### 概述


选择排序（Selection sort）是一种简单直观的排序算法。


它的工作原理是每一次从待排序的数据元素中选出最小（或最大）的一个元素，存放在序列的起始位置，直到全部待排序的数据元素排完。




### 基本过程


n个记录的文件的直接选择排序可经过n-1趟直接选择排序得到有序结果：
- 

在未排序序列中找到最小（大）元素，存放到排序序列的起始位置。

- 

再从剩余未排序元素中继续寻找最小（大）元素，然后放到已排序序列的末尾。

- 

以此类推，直到所有元素均排序完毕。





### 代码



# -*- coding：utf-8 -*-


import random


# 随机生成1-10之间无序序列整数数据
defgenerator():
    random_data =[]for i in range(0,10):
        random_data.append(random.randint(1,1000))return random_data

# 选择排序    
defselect_sort(data_list):
    length = len(data_list)for i in range(0, length):
        min = i        
        # 查找最小的元素for j in range(i+1, length):if data_list[j]< data_list[min]:# 找到最小的元素
                min = j        
        # 交换位置
        data_list[min], data_list[i]= data_list[i], data_list[min]return data_list

if __name__ =="__main__":print("开源优测-积微速成计划基本功")# 生成随机无序数据
    random_data = generator()# 打印无序数据print(random_data)# 插入排序
    length = len(random_data)
    sorted_data = select_sort(random_data)# 打印排序结果print(sorted_data)





