
# 【算法导论】【排序】—— 计数排序（counting sort） - Zhang's Wikipedia - CSDN博客


2018年07月29日 11:57:00[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：838


计数排序的特点：
需要额外的数组以存储：
中间过程数据（记为数组 C），数组 C 的下标是待排序序列的元素值，下标对应的值为出现的次数；
排序后的序列（记为 B），计数排序仅获取原始待排序序列的值，对原始序列不做 in-place 处理；
计数排序首先统计原始序列各个数（按顺序，也就是索引）出现的次数，需要获取原始序列的最大值，作为计数数组的区间长度；
defcounting_sort(A, k):\# k = max(A) + 1C = [0]*kforiinA:\# 统计 A 中各个数出现的次数，C 的下标为 A 的元素值C[i] +=1foriinrange(1, len(C)):            
        C[i] += C[i-1]\# 不断累加\# print('C: ', C)          \# C 的下标为 A 的元素值， C 下标对应的值为此元素值应在排序后的序列中的位置B = [0]*(len(A)+1)foriinA[::-1]:
        B[C[i]] = i\# C 下标对应的值为此元素值应在排序后的序列中的位置C[i] -=1\# 原始序列中重复出现的数字在排序过程中不断地向前排；returnBif__name__ =='__main__':
    A = [2,5,3,0,2,3,0,3]
    k = max(A)+1B = counting_sort(A, k)
    print('B: ', B)

