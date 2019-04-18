# 局部最大值 高原数组 python - YZXnuaa的博客 - CSDN博客
2018年04月12日 17:49:18[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：380

# 题目
         给定一个无重复元素的数组A[0…N-1]，求找到一个该数组的局部最大值。
         规定：在数组边界外的值无穷小。
# 思想
         遍历一遍得全局最大值，它显然是局部最大值，但可否有更快的办法
         定义：若子数组Array[from,…,to] 满足Array[from]＞Array[from-1]且Array[to]＞Array[to+1]，则称该数组为“高原数组”。
         于是：如果from和to退化为一个点，那这个点就是局部极大值点。于是我们就不停的切切切把高原数组切成一个点。
# 算法描述
                  1，求中点mid=(left+right)/2
                  2，该点导数下降，丢弃后半段，令right=mid
                  3，该点导数上升，丢弃后半段，令right=mid丢弃前半段，令left=mid+1
         重复上述过程，递归直至left==right
# 时间复杂度
         时间复杂度为O(logN)。
# !/usr/bin/python
# -*- coding: UTF-8 -*-
import random
import numpy as np
random.seed(0)
result = [random.randint(1, 100) for i in range(100)]
print(result)
while True:
    print('1')
    mid = int((len(result)-1)/2)
    if result[mid] > result[mid+1]:
        result = result[:mid]
    elif result[mid] < result[mid+1]:
        result = result[(mid+1):]
    else:
        print('The %s answer is %s' % (mid, result[mid]))
        break
