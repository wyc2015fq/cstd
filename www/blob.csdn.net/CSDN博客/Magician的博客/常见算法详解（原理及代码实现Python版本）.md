
# 常见算法详解（原理及代码实现Python版本） - Magician的博客 - CSDN博客


2018年02月14日 17:39:04[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：920



### 文章目录
[前言](#_2)
[1、冒泡排序](#1_7)
[2、选择排序](#2_68)
[3、插入排序](#3_120)
[4、希尔排序](#4_172)
[5、快速排序](#5_277)
[6、归并排序](#6_381)
[7、二分法查找](#7_484)
[总结](#_538)

## 前言
最近复习了下常见的算法，在这里手动再写一遍，权当加深自己的印象。代码实现用的是python3.6版本。
## 1、冒泡排序
**原理：**
冒泡排序（英语：Bubble Sort）是一种简单的排序算法。它重复地遍历要排序的数列，一次比较两个元素，如果他们的顺序错误就把他们交换过来。遍历数列的工作是重复地进行直到没有再需要交换，也就是说该数列已经排序完成。这个算法的名字由来是因为越小的元素会经由交换慢慢“浮”到数列的顶端。
**冒泡排序算法的运作如下：**
比较相邻的元素。如果第一个比第二个大（升序），就交换他们两个。
对每一对相邻元素作同样的工作，从开始第一对到结尾的最后一对。这步做完后，最后的元素会是最大的数。
针对所有的元素重复以上的步骤，除了最后一个。
持续每次对越来越少的元素重复上面的步骤，直到没有任何一对数字需要比较。
**冒泡排序的分析**
交换过程图示(第一次)：
![这里写图片描述](https://img-blog.csdn.net/20180213233258705?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
那么我们需要进行n-1次冒泡过程，每次对应的比较次数如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20180213233317416?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**时间复杂度**
最优时间复杂度：O(n) （表示遍历一次发现没有任何可以交换的元素，排序结束。）
最坏时间复杂度：O(n2)
稳定性：稳定
**总结：冒泡实质就是将列表中的数据依次两两比较，将较大值往列表的后排放，就像冒泡一样，直到所有的列表的前两个数据的大小也被确定。**
**代码实现：**
`#版本一：一般实现
def bubble_sort(alist):
#再写外层循环，需要执行n-1次
	n=len(alist)
	for j in range(0,n-1):
	#先写内层循环，两个数据两两比较大小，大的值往右移。
		for i in range(1,n-j)
			if alist[i]<alist[i-1]:
				alist[i],alist[i-1]=alist[i-1],alist[i]
				
版本二：优化版本。
#这里通过设置参数来判断之前的子列表数据是否有序，若有序则无需再进行重复的排序计算。
def bubble_sort(alist):
    n = len(alist)
#要进行n-1次比较排序
    for j in range(n-1):  
# [1, 2,3 ,4 ,6,5]则5和6顺序换过之后，就不用再两两比较排序了
        count = 0
        for i in range(0, n-1-j):
            # 班长从头走到尾，换一下位置count加1；
            if alist[i] > alist[i+1]:
                alist[i],alist[i+1] = alist[i+1], alist[i]
                count += 1
#count用于判断序列是否有序，若本身有序的话即count==0则说明之前的数据有序则无需再进行重复的排序计算直接结束程序
        if 0 == count:
            return
# i 0 ~ n-2   range(0, n-1) j=0
# i 0 ~ n-3   range(0, n-1-1) j=1
# i 0 ~ n-4   range(0, n-1-2) j=2
# j=n  i  range(0, n-1-j)
if __name__ == "__main__":
    li = [54, 26, 93, 17, 77, 31, 44, 55, 20]
    print(li)
    bubble_sort(li)
    print(li)`
## 2、选择排序
**原理：**
选择排序（Selection sort）是一种简单直观的排序算法。它的工作原理如下。首先在未排序序列中找到最小（大）元素，存放到排序序列的起始位置，然后，再从剩余未排序元素中继续寻找最小（大）元素，然后放到已排序序列的末尾。以此类推，直到所有元素均排序完毕。
选择排序的主要优点与数据移动有关。如果某个元素位于正确的最终位置上，则它不会被移动。选择排序每次交换一对元素，它们当中至少有一个将被移到其最终位置上，因此对n个元素的表进行排序总共进行至多n-1次交换。在所有的完全依靠交换去移动元素的排序方法中，选择排序属于非常好的一种。
**选择排序分析**
排序过程：
![这里写图片描述](https://img-blog.csdn.net/20180214152531256?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**时间复杂度**
最优时间复杂度：O(n2)
最坏时间复杂度：O(n2)
稳定性：不稳定（考虑升序每次选择最大的情况）
**总结：首先需要一个min坐标标记最小值，之后对右边的无序序列进行遍历选择操作，若找到比min值还小的数就交换位置<与冒泡的区别：冒泡两两比较将大的值往后移，选择是设定一个min的索引然后拿选定的最小值和右边的无序序列分别进行遍历比较，最终得到最小值与min索引指向的值进行交换。>**
**代码实现：**
`**版本一：一般实现**
def select_sort(alist):
	n=len(alist)
	#外层循环控制执行多少次下面的操作
	for j in range(n-1):
	     min = j
	     #先写内层循环用于拿指定的“最小”值（遍历右边的无序序列）和右边的无序序列分别进行两两比较，并将比“最小”值小的值和“最小”值互换位置。
	     for i in range(j+1, n):
	#min为最小值得索引指标
	         if alist[min] > alist[i]:
	             alist[min], alist[i] = alist[i], alist[min]
				
		
版本二：优化版本。（只是记录最小索引的位置信息等到一轮循环结束后再确定最终的最小值进行赋值操作）
def selection_sort(alist):
    n = len(alist)
    # 需要进行n-1次选择操作
    for i in range(n-1):
        # 记录最小位置
        min_index = i
        # 从i+1位置（即每次操作无序序列的第一个元素）到末尾遍历一遍选择出最小数据，然后进行交换。
        for j in range(i+1, n):
            if alist[j] < alist[min_index]:
            #只是记录最小索引的位置信息等到该循环结束后再确定最终的最小值
                min_index = j
        alist[i], alist[min_index] = alist[min_index], alist[i]
alist = [54,226,93,17,77,31,44,55,20]
selection_sort(alist)
print(alist)`
## 3、插入排序
**原理：**
插入排序（英语：Insertion Sort）是一种简单直观的排序算法。它的工作原理是通过构建有序序列，对于未排序数据，在已排序序列中从后向前扫描，找到相应位置并插入。插入排序在实现上，在从后向前扫描过程中，需要反复把已排序元素逐步向后挪位，为最新元素提供插入空间。
**插入排序分析**
![这里写图片描述](https://img-blog.csdn.net/20180214161059784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**时间复杂度**
最优时间复杂度：O(n) （升序排列，序列已经处于升序状态）
最坏时间复杂度：O(n2)
稳定性：稳定
**总结：从右边的无序序列中拿数据插入到左边的有序序列中。**
**代码实现：**
`**版本一：一般实现**
def insert_sort(alist):
		
		#再写外层循环,需要确定n-1个元素后整个队列才有序。
		for j in range(n-1)
			i=j
			#先写内层循环用于拿右边无序序列中的一个元素和左边的有序序列中的元素两两比较，插入到合适的位置。
			
			while i>=0:
				if alist[i]>alist[i+1]:
					alist[i],alist[i+1]=alist[i+1],alist[i]
					i-=1
		
版本二：优化版本。（当右边取得值比左边的最大值大时，直接插入左边然后break。不必进行重复的操做。）
def insert_sort(alist):
    # 从第二个位置，即下标为1的元素开始向前插入
#从右边的无序列表中取出多少个元素执行以下的过程
	n=len(alist)
    for j in range(1, n):
        # 从第i个元素开始向前比较，如果小于前一个元素，交换位置
		i=j
#从右边的无序序列中取出第一个元素，即i位置的元素，然后和左边的有序序列中的元素进行比较插入合适的位置。
        while i>0： 
            if alist[i] < alist[i-1]:
                alist[i], alist[i-1] = alist[i-1], alist[i]
#优化的过程：当右边取得值比左边的最大值大时，直接插入左边然后break。不必进行重复的操做。
			else:
				break
alist = [54,26,93,17,77,31,44,55,20]
insert_sort(alist)
print(alist)`
## 4、希尔排序
**原理：**
希尔排序(Shell Sort)是插入排序的一种。也称缩小增量排序，是直接插入排序算法的一种更高效的改进版本。希尔排序是非稳定排序算法。该方法因DL．Shell于1959年提出而得名。 希尔排序是把记录按下标的一定增量分组，对每组使用直接插入排序算法排序；随着增量逐渐减少，每组包含的关键词越来越多，当增量减至1时，整个文件恰被分成一组，算法便终止。
希尔排序过程
**希尔排序的基本思想是**：将数组列在一个表中并对列分别进行插入排序，重复这过程，不过每次用更长的列（步长更长了，列数更少了）来进行。最后整个表就只有一列了。将数组转换至表是为了更好地理解这算法，算法本身还是使用数组进行排序。
例如，假设有这样一组数[ 13 14 94 33 82 25 59 94 65 23 45 27 73 25 39 10 ]，如果我们以步长为5开始进行排序，我们可以通过将这列表放在有5列的表中来更好地描述算法，这样他们就应该看起来是这样(竖着的元素是步长组成)：
13 14 94 33 82
25 59 94 65 23
45 27 73 25 39
10
然后我们对每列进行排序：
10 14 73 25 23
13 27 94 33 39
25 59 94 65 82
45
将上述四行数字，依序接在一起时我们得到：[ 10 14 73 25 23 13 27 94 33 39 25 59 94 65 82 45 ]。这时10已经移至正确位置了，然后再以3为步长进行排序：
10 14 73
25 23 13
27 94 33
39 25 59
94 65 82
45
排序之后变为：
10 14 13
25 23 33
27 25 59
39 65 73
45 94 82
94
最后以1步长进行排序（此时就是简单的插入排序了）
**希尔排序的分析**
![这里写图片描述](https://img-blog.csdn.net/20180214163339328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**时间复杂度**
最优时间复杂度：根据步长序列的不同而不同
最坏时间复杂度：O(n2)
稳定想：不稳定
**总结：将一数列看成多组无序数列然后对每组数列进行插入排序，之后再缩短步长重新进行分组排序。这个算法的确挺麻烦，但和插入算法结合起来会容易记点**
**代码实现：**
`**版本一：一般实现**
def shell_sort(alist):
		
		n=len(alist)
	    grap = n // 2
	    while grap > 0:
			#grap，grap+1...
	        for j in range(grap, n):
	            i = j
	            #对每组中的各个元素分别进行比较初步排序
	            while i > 0:
	                if alist[i] < alist[i - grap]:
	                    alist[i], alist[i - grap] = alist[i - grap], alist[i]
	                    i -= grap
	        #缩短步长再次进行分组比较，每组中各个元素进行两两比较。
	        grap = grap // 2
			
版本二：优化版本。（同插入算法一样增加了break，避免对已排序的序列再次进行排序操作）
def shell_sort(alist):
    """希尔排序"""
    # n=9
    n = len(alist)
    # gap =4
    gap = n // 2
    # i = gap
    # for i in range(gap, n):
    #     # i = [gap, gap+1, gap+2, gap+3... n-1]
    #     while:
    #     if alist[i] < alist[i-gap]:
    #         alist[i], alist[i-gap] = alist[i-gap], alist[i]
    # gap变化到0之前，插入算法执行的次数
    while gap > 0:
        # shell算法，与普通的插入算法的区别就是gap步长
        for j in range(gap, n):
            # j = [gap, gap+1, gap+2, gap+3, ..., n-1]拿多少个元素进行操作
            i = j
# i 可以认为无序序列中要插入有序序列中元素的下标
            while i > 0:
                if alist[i] < alist[i-gap]:
                    alist[i], alist[i-gap] = alist[i-gap], alist[i]
                    i -= gap
                else:
                    break
        # 缩短gap步长
        gap //= 2

if __name__ == "__main__":
    li = [54, 26, 93, 17, 77, 31, 44, 55, 20]
    print(li)
    shell_sort(li)
    print(li)`
## 5、快速排序
**原理：**
快速排序（英语：Quicksort），又称划分交换排序（partition-exchange sort），通过一趟排序将要排序的数据分割成独立的两部分，其中一部分的所有数据都比另外一部分的所有数据都要小，然后再按此方法对这两部分数据分别进行快速排序，整个排序过程可以递归进行，以此达到整个数据变成有序序列。
**步骤为：**
从数列中挑出一个元素，称为"基准"（pivot），
重新排序数列，所有元素比基准值小的摆放在基准前面，所有元素比基准值大的摆在基准的后面（相同的数可以到任一边）。在这个分区结束之后，该基准就处于数列的中间位置。这个称为分区（partition）操作。
递归地（recursive）把小于基准值元素的子数列和大于基准值元素的子数列排序。
递归的最底部情形，是数列的大小是零或一，也就是永远都已经被排序好了。虽然一直递归下去，但是这个算法总会结束，因为在每次的迭代（iteration）中，它至少会把一个元素摆到它最后的位置去。
**快速排序的分析**
![这里写图片描述](https://img-blog.csdn.net/20180214165407537?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**时间复杂度**
最优时间复杂度：O(nlogn)
最坏时间复杂度：O(n2)
稳定性：不稳定
从一开始快速排序平均需要花费O(n log n)时间的描述并不明显。但是不难观察到的是分区运算，数组的元素都会在每次循环中走访过一次，使用O(n)的时间。在使用结合（concatenation）的版本中，这项运算也是O(n)。
在最好的情况，每次我们运行一次分区，我们会把一个数列分为两个几近相等的片段。这个意思就是每次递归调用处理一半大小的数列。因此，在到达大小为一的数列前，我们只要作log n次嵌套的调用。这个意思就是调用树的深度是O(log n)。但是在同一层次结构的两个程序调用中，不会处理到原来数列的相同部分；因此，程序调用的每一层次结构总共全部仅需要O(n)的时间（每个调用有某些共同的额外耗费，但是因为在每一层次结构仅仅只有O(n)个调用，这些被归纳在O(n)系数中）。结果是这个算法仅需使用O(n log n)时间。
**总结：最主要是采用递归的思想去解决问题**
**代码实现：**
`**版本一：一般实现**
def quick_sort(alist,start,end):
	
	if start>end:
        return
	#先写内层循环
	mind=alist[start]
	low=start
	high=end
	while low<high:
		if low<high and mind>alist[high]:
			alist[high],alist[low]=alist[low],alist[high]
			low+=1
		if low<high and mind<alist[low]:
			alist[high],alist[low]=alist[low],alist[high]
			high-=1
	alist[low]=mind
	quick_sort(alist,start,low-1)
	quick_sort(alist,low+1,end)
		
		
	
		
		
if __name__ == "__main__":
    li = [54, 26, 93, 17, 77, 31, 44, 55, 20]
    print(li)
    shell_sort(li)
    print(li)
**版本二：详细解释**
def quick_sort(alist, start, end):
    """快速排序"""
    # 注意这个条件：递归的退出条件即每个列表的元素为1
    if start >= end:
        return
    # 设定起始元素为要寻找位置的基准元素
    mid = alist[start]
    # low为序列左边的由左向右移动的游标
    low = start
    # high为序列右边的由右向左移动的游标
    high = end
    while low < high:
        # 如果low与high未重合，high指向的元素不比基准元素小，则high向左移动
        while low < high and alist[high] >= mid:
            high -= 1
        # 将high指向的元素放到low的位置上
        alist[low] = alist[high]
        # 如果low与high未重合，low指向的元素比基准元素小，则low向右移动
        while low < high and alist[low] < mid:
            low += 1
        # 将low指向的元素放到high的位置上
        alist[high] = alist[low]
    # 退出循环后，low与high重合，此时所指位置为基准元素的正确位置
    # 将基准元素放到该位置
    alist[low] = mid
    # 对基准元素左边的子序列进行快速排序
    quick_sort(alist, start, low-1)
    # 对基准元素右边的子序列进行快速排序
    quick_sort(alist, low+1, end)

alist = [54,26,93,17,77,31,44,55,20]
quick_sort(alist,0,len(alist)-1)
print(alist)
版本三：
def qsort(L):
    if len(L) <= 1: return L
    return qsort([lt for lt in L[1:] if lt < L[0]]) + L[0:1]+ \
           qsort([ge for ge in L[1:] if ge >= L[0]])
L = [3,14,2,12,9,33,99,35]
print (qsort(L))`
## 6、归并排序
**原理：**
归并排序是采用分治法的一个非常典型的应用。归并排序的思想就是先递归分解数组，再合并数组。
将数组分解最小之后，然后合并两个有序数组，基本思路是比较两个数组的最前面的数，谁小就先取谁，取了后相应的指针就往后移一位。然后再比较，直至一个数组为空，最后把另一个数组的剩余部分复制过来即可。
**归并排序的分析：**
先拆分后排序结合
![这里写图片描述](https://img-blog.csdn.net/20180214172028585?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**时间复杂度**
最优时间复杂度：O(nlogn)
最坏时间复杂度：O(nlogn)
稳定性：稳定
**总结：先将数列拆分成多个数列每个数列只有一个元素，然后将各个数列再两两结合成新的数列，结合的过程中进行排序（如A和B两个数列的第一个元素比较大小，小的插入到新的列表中的第一个元素的位置，大的紧随其后，然后再对两列的第二个元素进行上述操作）直到最终合成为一个序列然后再依次进行排序。**
**代码实现：**
`def merge_sort(alist):
    """归并排序"""
    n = len(alist)
    if n <= 1:
        return alist
    mid = n//2
    # left 采用归并排序后形成的有序的新的列表
    left_li = merge_sort(alist[:mid])
    # right 采用归并排序后形成的有序的新的列表
    right_li = merge_sort(alist[mid:])
    # 将两个有序的子序列合并为一个新的整体
    # merge(left, right)
    left_pointer, right_pointer = 0, 0
    result = []
#核心代码：拿左右两个队列中的每个元素分别进行比较，依次将最小值加入新的队列中
    while left_pointer < len(left_li) and right_pointer < len(right_li):
        if left_li[left_pointer] <=  right_li[right_pointer]:
            result.append(left_li[left_pointer])
            left_pointer += 1
        else:
            result.append(right_li[right_pointer])
            right_pointer += 1 
    result += left_li[left_pointer:]
    result += right_li[right_pointer:]
    return result

if __name__ == "__main__":
    li = [54, 26, 93, 17, 77, 31, 44, 55, 20]
    print(li)
    sorted_li = merge_sort(li)
    print(li)
    print(sorted_li)
#运行过程分析
    # merge_sort   [54, 26, 93, 17, 77, 31, 44, 55, 20]
    #
    # left_li = merge_sort [54, 26, 93, 17]
    #
    #     left_li = merge_sort [54, 26]
    #     left_li = [26, 54]
    #     left_li = merge_sort [54]    return [54]
    #     left_li = merge_sort [26]    return [26]
    #   返回两个列表后这两个返回值将作为merge_sort [54, 26]的函数体接着往下执行函数即：
    #              left_li = [54]
    #              right_li = [26]
    #              result = [26, 54]
    #              return result      merge_sort [54, 26]执行完毕接下来执行 right_li = merge_sort([93, 17])（得到最底层的结果后一步步往外面解递归嵌套）
    #
    #     right_li = merge_sort([93, 17])
    #
    #             left_li = merge_sort([93])
    #
    #                         return [93]
    #             left_li =[93]
    #
    #             right_li = merge_sort([17])
    #
    #                         return [17]
    #             right_li = [17]
    #
    #             result = [17, 93]
    #
    #             return result
    #
    #     right_li = [17, 93]
    #
    #     result = [17, 26, 54, 93]
    #
    #     return result
    #
    # left_li = [17, 26, 54, 93]
    #
    # right_li = merge_sort([77, 31, 44, 55, 20])
    #
    #
    # result = []
    # return result`
## 7、二分法查找
**原理：**
二分查找又称折半查找，优点是比较次数少，查找速度快，平均性能好；其缺点是要求待查表为有序表，且插入删除困难。因此，折半查找方法适用于不经常变动而查找频繁的有序列表。首先，假设表中元素是按升序排列，将表中间位置记录的关键字与查找关键字比较，如果两者相等，则查找成功；否则利用中间位置记 录将表分成前、后两个子表，如果中间位置记录的关键字大于查找关键字，则进一步查找前一子表，否则进一步查找后一子表。重复以上过程，直到找到满足条件的记录，使查找成功，或直到子表不存在为止，此时查找不成功。
**二分法查找分析：**
![这里写图片描述](https://img-blog.csdn.net/20180214173421847?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**时间复杂度**
最优时间复杂度：O(1)
最坏时间复杂度：O(logn)
**代码实现：**
`# coding:utf-8
# [17, 20, 26, 31, 44, 54, 55, 77, 93]
# mid = n/2
#
# [17, 20, 26, 31]
# mid = n/2

def binary_search(alist, item):
    """二分查找,递归"""
    n = len(alist)
    if n > 0:
        mid = n//2
        if alist[mid] == item:
            return True
        elif item < alist[mid]:
            return binary_search(alist[:mid], item)
        else:
            return binary_search(alist[mid+1:], item)
    return False

def binary_search_2(alist, item):
    """二分查找， 非递归"""
    n = len(alist)
    first = 0
    last = n-1
    while first <= last:
        mid = (first + last)//2
        if alist[mid] == item:
            return True
        elif item < alist[mid]:
            last = mid - 1
        else:
            first = mid + 1
    return False`
## 总结
各种算法时间复杂对对比：
![这里写图片描述](https://img-blog.csdn.net/20180214173716641?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

