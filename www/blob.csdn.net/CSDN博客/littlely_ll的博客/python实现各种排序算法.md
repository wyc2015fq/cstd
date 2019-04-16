# python实现各种排序算法 - littlely_ll的博客 - CSDN博客





2017年09月25日 20:41:26[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：673








# python排序算法



```python
# -*-encoding: utf-8 -*-
# param: a    a list which to be sort

class MultiSort(object):
    # 交换函数
    def exch(self, a, i, j):
        temp = a[i]
        a[i] = a[j]
        a[j] = temp

    # 插入排序
    def insertionSort(self, a):
        N = len(a)
        if N == 1:
            return a
        for i in range(1, N):
            for j in range(1, i + 1)[::-1]:
                if a[j] < a[j - 1]:
                    self.exch(a, j, j - 1)
                    # print a
        return a

    # 选择排序
    def selectionSort(self, a):
        N = len(a)
        if N == 1:
            return a
        for i in range(N):
            mins = i
            for j in range(i + 1, N):
                if (a[mins] > a[j]):
                    mins = j
            self.exch(i, mins)
            # print a
        return a

    # 希尔排序
    def shellSort(self, a):
        N = len(a)
        h = 1
        if N == 1:
            return a
        while h < (N // 3):
            h = 3 * h + 1
        while h >= 1:
            for i in range(h, N):
                for j in range(i, 0, -h):
                    if j >= h:
                        if a[j] < a[j - h]:
                            self.exch(a, j, j - h)
                            # print self.a
                    j -= h
            h //= 3
        return a

    # 归并排序：__merge与__mSort为辅助函数
    def __merge(self, a, lo, mid, hi):
        from copy import deepcopy
        i = lo
        j = mid + 1
        aux = deepcopy(a)
        for k in range(lo, hi + 1):
            if i > mid:
                a[k] = aux[j]
                j += 1
            elif j > hi:
                a[k] = aux[i]
                i += 1
            elif aux[j] < aux[i]:
                a[k] = aux[j]
                j += 1
            else:
                a[k] = aux[i]
                i += 1
            print a

    def __mSort(self, a, lo, hi):
        if hi <= lo:
            return
        mid = lo + (hi - lo) // 2
        self.__mSort(a, lo, mid)
        self.__mSort(a, mid + 1, hi)
        self.__merge(a, lo, mid, hi)

    def mergeSort(self, a):
        self.__mSort(a, 0, len(a) - 1)
        return a

    # 快速排序：辅助函数__partition和__qSort
    def __partition(self, a, lo, hi):
        i = lo
        j = hi + 1
        v = a[lo]
        while True:
            while True:
                i += 1
                if a[i] < v:
                    if i == hi:
                        break
                else: break
            while True:
                j -= 1
                if v < a[j]:
                    if j == lo:
                        break
                else: break
            if i >= j: break
            self.exch(a, i, j)
        self.exch(a, lo, j)
        return j

    def __qSort(self, a, lo, hi):
        if hi <= lo: return
        j = self.__partition(a, lo, hi)
        self.__qSort(a, lo, j-1)
        self.__qSort(a, j+1, hi)

    def quickSort(self, a):
        import random
        random.shuffle(a)
        self.__qSort(a, 0, len(a)-1)
        return a
	
	# 冒泡排序
    def bubbleSort(self, a):
        length = len(a)
        for i in range(length-1):
            # print "第%d趟:" % (i+1)
            isSorted = True
            for j in range(length-1-i):
                if a[j] > a[j+1]:
                    self.exch(a, j, j+1)
                    isSorted = False
                # print "第%d次:" % (j + 1), a
            if isSorted:
                break
        return a



if __name__ == "__main__":
    import numpy as np

    # a = np.random.randint(1, 100000, size=100, dtype=np.int64)
    a = [49, 38, 65, 97, 26, 13, 27, 49, 55, 4]
    # print a
    b = MultiSort()
    # b.insertionSort(a)
    # b.selectionSort(a)
    # b.shellSort(a)
    print "========================"
    print b.bubbleSort(a)
    print "========================"
```





