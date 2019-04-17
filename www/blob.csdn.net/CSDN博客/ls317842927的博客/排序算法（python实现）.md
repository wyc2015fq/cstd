# 排序算法（python实现） - ls317842927的博客 - CSDN博客





2017年02月23日 19:48:48[ls317842927](https://me.csdn.net/ls317842927)阅读数：183








![这里写图片描述](https://img-blog.csdn.net/20170224203919749?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHMzMTc4NDI5Mjc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
1、冒泡排序
![这里写图片描述](https://img-blog.csdn.net/20170223194750413?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHMzMTc4NDI5Mjc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```python
class BubbleSort:
    def bubbleSort(self, A, n):     
        flag=1
        for i in range(n-1):  #i: 0~(n-2)  range函数不包括最后一个元素
            if flag:                
                flag=0
                for j in range(n-1,i,-1):   #j: (n-1)~i+1 逆序
                    if A[j]<A[j-1]:
                        A[j],A[j-1]=A[j-1],A[j] #互换元素
                        flag=1 #如果进行交换，设置flag=1
            else:
                break  #flag=0时表示没有进行交换，之后的数字有序，无需再排序，退出循环
        return A
```

2、快速排序

快速排序是冒泡排序的改进。快速排序是根据“基准点”跨越式的交换，而冒泡排序是相邻元素两两交换。快速排序使得元素移动的间距变大，总的比较和移动次数会减少。

```python
# -*- coding:utf-8 -*-

class QuickSort:
    def quickSort(self, A, n):
        self.Qsort(A,0,n-1)
        return A  
    def Qsort(self,A,low,high):
        if low <high:         
            point=self.partition(A,low,high)  #每回可以固定一个pivot的位置

            self.Qsort(A,low,point-1)    #再递归时刨除pivot
            self.Qsort(A,point+1,high)


    def partition(self, A, low,high):
        pivot=A[low]
        while low < high:   #low high每操作一次后，再检查low < high
            while low<high and A[high]>=pivot:  #内部while也要检查low<high，当走到high==low时停止
                high-=1
            A[low]=A[high]  #一定要先从high开始
            while low<high and A[low]<=pivot:
                low+=1
            A[high]=A[low]

        A[low]=pivot  #此时low==high，写A[high]=pivot也行
        return low
```

最差的情况就是每一次取到的元素就是数组中最小/最大的，这种情况其实就是冒泡排序了(每一次都排好一个元素的顺序)，这种情况时间复杂度就是冒泡排序的时间复杂度。 

首先就地快速排序使用的空间是O(1)的，也就是个常数级；而真正消耗空间的就是递归调用了，因为每次递归就要保持一些数据； 

最优的情况下空间复杂度为：O(logn)  ；每一次都平分数组的情况 

最差的情况下空间复杂度为：O( n )      ；退化为冒泡排序的情况
快速排序的优化

1、优化选取基准点 

解决方法：三数取中法，即从序列前中后三部分找出三个数，排序后取出中间的作为基准点。确保基准点不会取到最小值和最大值。 

2、优化小数组时排序方案 

当待排序记录个数很少时，使用直接插入排序。 

3、优化递归操作 
![这里写图片描述](https://img-blog.csdn.net/20170224225939051?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHMzMTc4NDI5Mjc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

修改 Qsort，变为一个递归。
```python
def Qsort(self,A,low,high):
        while low < high:
            point=self.partition(A,low,high)           
            self.Qsort(A,low,point-1) #处理完左一半，low没有用了
            low=point+1
            #令low=point+1后，利用while循环再调用partition(A,point+1,high) 
            #等于实现Qsort(A,point+1，high)
```

扩大尾递归优势，让递归函数只去处理小部分的数组。

```python
def Qsort(self,A,low,high):
        while low < high:
            point=self.partition(A,low,high)
            if (point-low < high-point): #左边长度小于右边长度
                self.Qsort(A,low,point-1) #对较小长度先进行递归
                low=point+1
            else:
                self.Qsort(A,point+1,high)
                high=point-1
```

3、选择排序

```python
# -*- coding:utf-8 -*-

class SelectionSort:
    def selectionSort(self, A, n):
        for i in range(n-1):
            min=i
            for j in range(i+1,n):
                if A[j]<A[min]:
                    min=j
            if min != i:  #不相等时才交换
                A[i],A[min]=A[min],A[i]
        return A
```

4、堆排序

在n个记录里找到最小的记录需要比较n-1次（选择排序）。选择排序和冒泡排序一样，每一次选择最小记录时都没有利用之前的比较结果，都要比较n-1次，导致$O(n^{2})$。

堆排序可以很好地利用上一次的比较结果。根结点（亦称为堆顶）的关键字是堆里所有结点关键字中最大者，称为大顶堆。大顶堆要求根节点的关键字既大于或等于左子树的关键字值，又大于或等于右子树的关键字值，且要求是完全二叉树。 
![这里写图片描述](https://img-blog.csdn.net/20170226161329713?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHMzMTc4NDI5Mjc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170226162041644?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHMzMTc4NDI5Mjc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170226162300052?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHMzMTc4NDI5Mjc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

上述ppt中，数组下标从1开始，下面代码中数组下标从0开始。
```python
# -*- coding:utf-8 -*-

class HeapSort:
    def heapSort(self, A, n):

        for i in range(n//2-1, -1, -1):  #n/2-1 ~ 0
            self.HeapAdjust(A, i, n);  #从最后一个有叶子的结点A[n/2-1]往前至A[0]，一个个调整成大顶堆。n为待排序的元素个数

        for i in range(n-1, 0, -1):  #n-1 ~ 1
            A[0], A[i] = A[i], A[0]   #从大顶堆中，将第一个元素和最后一个元素交换，此时最后一个位置上的元素已经排序好了，只需要将前n-1个元素排序
            self.HeapAdjust(A, 0, i) #大hh顶堆中，只有A[0]变了，所以只调整0结点，其余已经是大顶堆了。i=n-1为待排序的元素个数
        return A

    def HeapAdjust(self, A, s, n):  #s为双亲结点，n为待排序的元素个数
        tmp = A[s]  #保存双亲结点的值

        i = 2*s+1  #s=0时，左孩子i是2*s+1
        while(i<n):  #左孩子下标 i <= 数组最后一个元素下标 n-1
            #把while(i<n)写成for i in range(2*s+1,n,2*i+1): 亲测不行,因为步长i未定义就使用了
            if i+1<n and A[i+1] > A[i]:  #如果存在右孩子 并且 右孩子大于左孩子
                i+=1  #i指向右孩子（即较大的孩子）

            if A[i] <= tmp:  #如果较大的孩子也没有根结点大，则退出
                break

            A[s] = A[i]  #否则，较大的孩子赋值给双亲结点
            s = i  #双亲位置来到较大的孩子的位置

            i = 2*s+1  #来到较大的孩子的左孩子，接着和tmp比较  i=2*i+1也可以

        A[s] = tmp
```

5、直接插入排序
![这里写图片描述](https://img-blog.csdn.net/20170223203847407?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHMzMTc4NDI5Mjc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```python
# -*- coding:utf-8 -*-

class InsertionSort:
    def insertionSort(self, A, n):
        for i in range(1,n):
            tmp=A[i]
            j=i-1
            while j>=0 and A[j]>tmp:
                A[j+1]=A[j]
                j-=1
            A[j+1]=tmp  
        return A
```

6、希尔排序

直接插入排序在**记录基本有序**时或者**记录数较少**时效率很高。 

希尔排序使待排序的记录个数减少，且基本有序。做法是将记录分组，对每个子序列（待排序的记录个数减少）进行插入排序，当gap=1时，记录已经基本有序。 

代码是在直接插入排序（间隔为1）中修改间隔为gap。 
![这里写图片描述](https://img-blog.csdn.net/20170224202809241?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHMzMTc4NDI5Mjc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```python
# -*- coding:utf-8 -*-

class ShellSort:
    def shellSort(self, A, n):
        gap=n
        while gap >=1:
            gap=gap//3+1
            for i in range(1,n,gap):
                tmp=A[i]
                j=i-gap
                while j>=0 and A[j]>tmp:
                    A[j+gap]=A[j]
                    j-=gap
                A[j+gap]=tmp  
            if gap==1:  #进行完gap=1的排序后停止，否则死循环
                break
        return A
```

7、归并排序
![这里写图片描述](https://img-blog.csdn.net/20170223211117847?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHMzMTc4NDI5Mjc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```
class MergeSort:
    def mergeSort(self, A, n):
        if n>1:  #数组A元素至少两个
            mid =n//2
            left=self.mergeSort(A[:mid],len(A[:mid]))  #左一半排序     #注意！！切片没有取到最后的mid下标，即A[0]~A[mid-1]
            right=self.mergeSort(A[mid:],len(A[mid:]))  #右一半排序     #从mid开始，不能从mid+1开始，即A[mid]~A[n-1]
            return self.merge(left,right)   #将两个升序列表left和right合并，并返回   勿忘返回
        else:  #如果只有一个元素则直接返回数组
            return A 

    def merge(self,left,right):   #将两个升序列表合并
        i,j=0,0
        result=[]
        while i<len(left) and j<len(right):
            if left[i]<=right[j]:
                result.append(left[i])
                i+=1
            else:
                result.append(right[j])
                j+=1
        #如果没有数组剩下，说明i==len(left)==j==len(right)
        #如果最后有一个数组剩下
        if i<len(left):  #此时,j==len(right)
            result+=left[i:]
        if j<len(right): #此时,i<==len(left)
            result+=right[j:]
        return result
```

归并的空间复杂度就是那个临时的数组和递归时压入栈的数据占用的空间：n + logn；所以空间复杂度为: O(n)。 

我看到网上很多blog分享空间复杂度只有O(1)的归并排序法；因为传统的归并排序所消耗的空间主要是在归并函数（把两个有序的函数合并成一个有序的函数），所以如果要让时间复杂度为 O(1)  ，那么也只能在归并函数中做文章了。其主要思想就是借助于快速排序（其实就是相当于归并函数被快速排序函数替换了）；这样的方法虽然可以减少内存的消耗，但是却会在时间上带来损失，因为这样时间复杂度却变成了  O(n^2)  了；所以这种方法并不是一个两全其美的idea。

线性时间排序
8、计数排序

```python
# -*- coding:utf-8 -*-

class CountingSort:
    def countingSort(self, A, n):
        k=max(A) #待排序数组的最大值
        b=[0 for i in range(n)] #用于存放排序好的数组
        c=[0 for i in range(k+1)] #用于计数 c[0~ k]

        for i in A:  
            c[i]+=1  # c[i]存放元素i的个数
        for i in range(1,k+1):
            c[i]=c[i-1]+c[i]   # c[i]是小于等于元素i的元素个数（包括元素i自己）

        for i in A: 
            b[c[i]-1]=i  #c[i]-1说明元素i之前有几个数（去掉元素i自己），把元素i放到该位置上
            c[i]-=1  #把元素i放在正确的位置后，去掉此元素，即计数减一

        return b
```

空间复杂度是：O( n )，因为b数组。

9、基数排序

```python
# -*- coding:utf-8 -*-

class RadixSort:
    def radixSort(self, A, n):

        for k in range(4):  #最大四位数
            s=[[] for i in range(10)]  #0~9  10个桶
            for i in A:
                s[int(i/10**k)%10].append(i)  #每个元素i按顺序放入s

            A=[]
            for i in range(10):
                A+=s[i]   #将s中的元素全放到A里

        return A
```

![这里写图片描述](https://img-blog.csdn.net/20170307092643272?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHMzMTc4NDI5Mjc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)










