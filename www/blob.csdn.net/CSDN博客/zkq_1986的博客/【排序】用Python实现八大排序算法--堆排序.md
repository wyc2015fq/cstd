# 【排序】用Python实现八大排序算法--堆排序 - zkq_1986的博客 - CSDN博客





2017年12月26日 10:41:27[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：149









#获得大顶堆

def adjustHeap(ls,start,end):

    while start*2 <= end:

        j = start * 2

        if j<end and ls[j] < ls[j+1]:  #让j指向大的孩子

            j+=1

        if ls[start] < ls[j]:

            ls[start],ls[j]=ls[j],ls[start]

            start = j

        else:

            break



def heapSort(ls):

    n = len(ls)

    ls.insert(0, 0) #第一位不用

    for i in range(n//2,0,-1): #初始化堆，使得每个节点符合堆的要求

        adjustHeap(ls, i,n)

    while n > 1:

        ls[1],ls[n] = ls[n],ls[1] #将第一个最大元素沉到底部，从而获得最后的升序

        n -= 1

        adjustHeap(ls,1,n)

    return ls[1:]



ls=[3,1,100,1,20,4,70,39,48]

res=heapSort(ls)

print(res)
            


