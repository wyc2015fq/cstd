# 数组与矩阵---找到无序数组中最小的k个数 - miner_zhu的博客 - CSDN博客





2018年07月24日 10:21:15[miner_zhu](https://me.csdn.net/miner_zhu)阅读数：53标签：[python																[BFPRT算法](https://so.csdn.net/so/search/s.do?q=BFPRT算法&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[常用算法python实现](https://blog.csdn.net/miner_zhu/article/category/7800703)








**【题目】**

　　给定一个无序的整型数组arr，找到其中最小的k个数。

**【要求】**

　　如果数组arr的长度为N，排序之后自然可以得到最小的k个数，此时时间复杂度与排序算法的时间复杂度相同，为O(NlogN)。本题要求读者实现时间复杂度为O(Nlogk)和O(N)的方法。

**【基本思路】**

**O(Nlogk)的方法**。思路很简单，就是一直维护一个大小为k的大根堆，这个堆表示目前选出的k个最小的数。接下来遍历整个数组，遍历过程中看当前数是否比堆顶元素小，如果是的话，将堆顶元素替换成当前的数，然后从堆顶向下调整堆；否则，不进行任何操作。遍历结束后，堆中的k个数就是答案。

**下面是使用python3.5实现的代码：**

```python
def getMinKNumsByHeap(arr, k):
    def heapInsert(heap, value, i):
        heap[i] = value
        parent = (i-1) // 2
        while parent >= 0:
            if arr[parent] < value:
                arr[i] = arr[parent]
                i = parent
                parent = (parent-1) // 2
            else:
                break
        arr[i] = value

    def heapify(heap, index):
        n = len(heap)
        child = 2 * index + 1
        tmp = heap[index]
        while child < n:
            if child < n-1 and heap[child] < heap[child+1]:
                child += 1
            if heap[child] > tmp:
                heap[index] = heap[child]
                index = child
                child = 2 * child + 1
            else:
                break
        heap[index] = tmp


    if arr == None or len(arr) == 0 or k < 1 or k > len(arr):
        return arr
    heap = [0 for i in range(k)]
    for i in range(k):
        heapInsert(heap, arr[i], i)
    for i in range(k, len(arr)):
        if arr[i] < heap[0]:
            heap[0] = arr[i]
            heapify(heap, 0)
    return heap
```

**O(N)的方法**。需要用到一个经典的算法——**BFPRT 算法**。该算法解决的问题是，在时间复杂度O(N)内，从无序数组中找到第k个最小的数。显然，如果我们找到第k个最小的数，只需要再遍历一次数组即可找到最小的k个数。 

　　假设BFPRT算法的函数是selct(arr, k)，表示在arr中找到第k个最小的数并返回。具体算法过程如下：
- 
将arr中的n个元素5个5个分成一组，不够5个的单独分成一组

- 
对每个组进行组内排序（插入排序）。排序后找到每个组的中位数，如果组的元素个数是偶数，这里规定找下中位数

- 步骤2中一定会找到n/5个中位数，让这些中位数组成一个新的数组，假设为res。递归调用select(res, len(res)/2)，意义是找到这个新数组的中位数，即res中第res/2小的数 

- 假设步骤3中递归调用select后，返回的数是x。根据这个x划分整个arr数组(partition过程）：比x小的都放在x的左边，比x大的都放在x的右边。假设划分完后，x在arr中的位置记为 i 

- 如果i == k-1，说明x为整个数组中第k个小的数，直接返回；如果i > k-1，说明第k个最小的数在x左边，调用select函数在左半区寻找；如果i < k-1，说明第k个最小的数在x右边，调用select函数在右半区寻找

在整个实现的过程中，对BFPRT做了一点改进，改进的地方是当中位数的中位数x出现多次时，我们返回的是一个下标范围而不是一个值。这样可以减少递归调用的次数。

**下面是使用python3.5实现的代码：**

```python
def getMinKNumsByBFPRT(arr, k):
    if arr == None or len(arr) == 0 or k < 1 or k > len(arr):
        return arr
    minKth = getMinKthByBFPRT(arr, k)
    res = []
    for i in range(len(arr)):
        if arr[i] < minKth:
            res.append(arr[i])
    for i in range(len(res), k):
        res.append(minKth)
    return res


    def getMinKthByBFPRT(arr, k):
        copyArr = [arr[i] for i in range(len(arr))]
        return select(copyArr, 0, len(copyArr)-1, k-1)

    def select(arr, begin, end, index):
        if begin == end:
            return arr[begin]
        pivot = medianOfMedian(arr, begin, end)
        pivotRange = partition(arr, begin, end, pivot)    #返回一个大小为2的数组，表示pivot这个数 \
        在数组中的范围，因为pivot可能出现了不止一次
        if index >= pivotRange[0] and index <= pivotRange[1]:
            return arr[index]
        elif index < pivotRange[0]:
            return select(arr, begin, pivotRange[0]-1, index)
        else:
            return select(arr, pivotRange[1]+1, end, index)

    def medianOfMedian(arr, begin, end):
        num = end - begin + 1
        offset = 0 if num % 5 == 0 else 1
        res = [0 for i in range(num//5 + offset)]
        for i in range(len(res)):
            start = begin + i * 5
            last = start + 4 if start + 4 <= end else end
            res[i] = median(arr, start, last)
        return select(res, 0, len(res) - 1, len(res) // 2)

    def median(arr, begin, end):
        insertSorts(arr, begin, end)
        mid = (begin + end) // 2
        return arr[mid] if (end - begin + 1) % 2 == 1 else arr[mid+1]

    def insertSorts(arr, begin, end):
        for i in range(begin+1, end+1):
            tmp = arr[i]
            j = i
            while j > begin and tmp < arr[j-1]:
                arr[j] = arr[j-1]
                j -= 1
            arr[j] = tmp

    def partition(arr, begin, end, pivot):
        small = begin - 1
        cur = begin 
        big = end + 1
        while cur != big:
            if arr[cur] == pivot:
                cur += 1
            elif arr[cur] < pivot:
                arr[small+1], arr[cur] = arr[cur], arr[small+1]
                small += 1
                cur += 1
            else:
                arr[cur], arr[big-1] = arr[big-1], arr[cur]
                big -= 1
        ran = []
        ran.append(small+1)
        ran.append(big-1)
        return ran
```






