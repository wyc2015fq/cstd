# 合并K个有序数组 - 数据之美的博客 - CSDN博客
2017年09月18日 20:58:52[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：450
目录[(?)](http://blog.csdn.net/u012328476/article/details/52522900#)[[-]](http://blog.csdn.net/u012328476/article/details/52522900#)
- [算法原理及实现](http://blog.csdn.net/u012328476/article/details/52522900#t0)
- [算法时间复杂度理解](http://blog.csdn.net/u012328476/article/details/52522900#t1)- [建堆的时间复杂度](http://blog.csdn.net/u012328476/article/details/52522900#t2)
- [排序的时间复杂度](http://blog.csdn.net/u012328476/article/details/52522900#t3)
给定K个有序数组，每个数组有n个元素，想把这些数组合并成一个有序数组。
## 算法原理及实现
一. 最简单的方法是创建一个n*k大小的数组，然后把所有数字拷贝进去，然后再进行时间复杂度为O(nlogn)排序算法，这样总体时间复杂度为O(nklognk)
二. 可以利用最小堆完成，时间复杂度是O(nklogk)，具体过程如下：
> 
- 创建一个大小为n*k的数组保存最后的结果
- 创建一个大小为k的最小堆，堆中元素为k个数组中的每个数组的第一个元素
- 重复下列步骤n*k次：
- 每次从堆中取出最小元素（堆顶元素），并将其存入输出数组中
- 用堆顶元素所在数组的下一元素将堆顶元素替换掉，
- 如果数组中元素被取光了，将堆顶元素替换为无穷大。每次替换堆顶元素后，重新调整堆
下面给出相关算法的C++实现代码
**[cpp]**[view
 plain](http://blog.csdn.net/u012328476/article/details/52522900#)[copy](http://blog.csdn.net/u012328476/article/details/52522900#)
- #include<iostream>
- #include<limits.h>
- usingnamespace std;  
- 
- #define n 4
- 
- // 定义最小堆节点
- struct MinHeapNode  
- {  
- int element; // 带排序元素
- int i; // 数组索引，element是从哪个数组取得
- int j; // 元素索引，下一个element的索引
- };  
- 
- // 定义一个最小节点之间的比较函数
- void swap(MinHeapNode *x, MinHeapNode *y);  
- 
- class MinHeap  
- {  
-     MinHeapNode *harr; // 定义一个指针，指向最小堆的堆顶元素
- int heap_size; // 堆的大小
- public:  
- // 构造函数（建堆）：构造一个指定大小的最小堆
-     MinHeap(MinHeapNode a[], int size);  
- 
- // 调整堆：调整给定根结点的子树结构
- void MinHeapify(int );  
- 
- // 给定节点的左子节点
- int left(int i) { return (2*i + 1); }  
- 
- // 给定节点的右子节点
- int right(int i) { return (2*i + 2); }  
- 
- // 获取根节点值（堆顶元素）
-     MinHeapNode getMin() { return harr[0]; }  
- 
- // 替换根节点（堆顶元素），重新调整堆
- void replaceMin(MinHeapNode x) { harr[0] = x;  MinHeapify(0); }  
- };  
**[cpp]**[view
 plain](http://blog.csdn.net/u012328476/article/details/52522900#)[copy](http://blog.csdn.net/u012328476/article/details/52522900#)
- int *mergeKArrays(int arr[][n], int k)  
- {  
- int *output = newint[n*k];  // 最后输出的数组，保存排序结果
- 
- // 创建一个大小为k的最小堆，堆中元素为k个数组中的每个数组的第一个元素
-     MinHeapNode *harr = new MinHeapNode[k];  
- for (int i = 0; i < k; i++)  
-     {  
-         harr[i].element = arr[i][0]; // 每个数组的第一个元素（每个数组的最小元素）
-         harr[i].i = i;  // 当前数组索引
-         harr[i].j = 1;  // 该数组下一个元素的索引（判断元素是否用完）
-     }  
-     MinHeap hp(harr, k); // 对上述大小为k的数组建堆
- 
- // 逐次取出堆顶元素，存入输出数组中，并将其替换为所在数组的下一元素
- for (int count = 0; count < n*k; count++)  
-     {  
- // 取堆顶，存结果
-         MinHeapNode root = hp.getMin();  
-         output[count] = root.element;  
- 
- // 替换堆顶
- if (root.j < n)  
-         {  
-             root.element = arr[root.i][root.j];  
-             root.j += 1;  
-         }  
- // 当前数组元素用完了，设堆顶为无穷大
- else root.element =  INT_MAX; //INT_MAX is for infinite
- 
- // 如果还有元素，就替换堆顶元素，调整堆结构
-         hp.replaceMin(root);  
-     }  
- 
- return output;  
- }  
- MinHeap::MinHeap(MinHeapNode a[], int size)  
- {  
-     heap_size = size;  
-     harr = a;  // store address of array
- int i = (heap_size - 1)/2;  
- while (i >= 0)  
-     {  
-         MinHeapify(i);  
-         i--;  
-     }  
- }  
- 
- // 递归的调整
- // This method assumes that the subtrees are already heapified
- void MinHeap::MinHeapify(int i)  
- {  
- int l = left(i);  
- int r = right(i);  
- int smallest = i;  
- if (l < heap_size && harr[l].element < harr[i].element)  
-         smallest = l;  
- if (r < heap_size && harr[r].element < harr[smallest].element)  
-         smallest = r;  
- if (smallest != i)  
-     {  
-         swap(&harr[i], &harr[smallest]);  
-         MinHeapify(smallest);  
-     }  
- }  
- void swap(MinHeapNode *x, MinHeapNode *y)  
- {  
-     MinHeapNode temp = *x;  *x = *y;  *y = temp;  
- }  
- 
- // 输出排序结果
- void printArray(int arr[], int size)  
- {  
- for (int i=0; i < size; i++)  
-        cout << arr[i] << " ";  
- }  
- 
- 
- int main()  
- {  
- 
- int arr[][n] =  {{2, 6, 12, 34},  
-                      {1, 9, 20, 1000},  
-                      {23, 34, 90, 2000}};  
- int k = sizeof(arr)/sizeof(arr[0]);  
- 
- int *output = mergeKArrays(arr, k);  
- 
-     cout << "Merged array is " << endl;  
-     printArray(output, n*k);  
- 
- return 0;  
- }  
## 算法时间复杂度理解
### 1. **建堆**的时间复杂度
堆排序有两种建堆方式，一种是知道了n个元素，进行建堆排序，另一种是逐次添加一个元素，一个n个，进行建堆排序
（1）第一种就是普通的堆排序：建堆＋调整。
> 
建堆是从最后一个非叶子节点开始，总共要遍历n/2个元素，每次调整需要的时间为O(logn),所以大致整体的运行时间复杂度为O(nlogn).但是如果更进一步分析，堆排序的实际情况会更理想一些，因为是从最后一个非叶子节点开始排序的，就算要调整，最多需要一次，不需要logn这么多次。
建堆过程实际是从完全二叉树最后一个非叶子节点开始，将其与子节点进行比较和交换。对于每个非叶子节点来说，最多进行2次比较和交换操作，因此整个建堆的时间复杂度为O(n)
（2）第二种是插入建堆：它的大致步骤如下：
> 
a. 首先增加堆的长度，在最末尾的地方加入最新插入的元素
> 
b. 比较当前元素和它的父结点值，如果比父结点值大，则交换两个元素，否则返回
> 
c. 重复步骤b
> 
最优情况就是n。
最坏的情况下，每次新增加一个元素都需要调整到它的根结点。而这个长度为logn。那么它的时间复杂度为O(nlogn)：
![](https://img-blog.csdn.net/20160913113536557?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
### 2. 排序的时间复杂度
完全二叉树的第i个节点到根节点的距离为  floor(logn) + 1,所以第i次取堆顶记录重建堆需要用O(logi)时间，且需要取n-1次堆顶元素，所以调整堆的时间复杂度为O(nlogn)
所以总体而言，堆排序的时间复杂度为O(nlogn)，因为堆排序对原始记录的排序状态不敏感，因此它的最好最坏和平均时间复杂度都是O(nlogn)。
