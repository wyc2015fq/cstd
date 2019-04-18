# 【算法学习】堆排序（Heap Sorting） - 江南烟雨 - CSDN博客
2012年05月25日 20:33:12[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：3907
堆排序引入了另外一种算法设计技术：利用某种数据结构（在此算法中为“堆”）来管理算法执行中的信息。
## **一、堆**
### 堆简介
我们通常使用的堆的二叉堆，它是一种数组对象，可以被视为一棵完全二叉树。树中的每个节点与数组中的节点相对应。如下图所示：
![](https://img-my.csdn.net/uploads/201205/25/1337943557_1222.jpg)
表示堆的数组通常由两个属性：数组中元素的个数length[A]，存放在A中的堆的元素的个数heap-size[A]。也就是说存放在A中的一些元素可能不属于对应的堆。因此：
heap-size[A] <= length[A]。
给定了某个节点的下标i，其父节点、左儿子以及右儿子可以很容易计算出来：
注意：下标均以1开始，而不是0
父节点：PARENT(i) = floor(i / 2)（向下取整）
左儿子：LEFT(i) = 2 * i
右儿子：RIGHT(i) = 2 * i + 1
### **堆的分类**
二叉堆通常分为大根堆和小根堆。
在大根堆中，对于以某个节点为根的子树，其各节点的值都不大于其根节点的值，即A[PARENT(i)] >= A[i]
小根堆则正好相反。
在堆排序中我们使用的是大根堆（从小到大排列）。小根堆通常在[STL优先级队列](http://blog.csdn.net/xiajun07061225/article/details/8556786)中使用。
### **堆的高度**
节点在堆中的高度被定义为此节点到叶子的最长简单下降路径中边的数目。
堆的高度即根节点的高度。
## **二、堆的调整**
很多时候，一棵二叉树不满足大根堆的性质，我们需要采用某种算法进行调整以使其变为大根堆。下面的函数MaxHeapify将会实现此功能。我们假定以某个节点i的左儿子节点和右儿子节点为根的子树都是大根堆，但是A[i]可能小于其子节点的值，这样就违背了大根堆的性质。
### 算法大致思想：
首先找出i节点和其左右子节点共3个节点中值最大的节点，如果不是i，则将i与值最大的节点互换。这样确保了根i处的值是最大的。然后调整以刚才与i互换的子节点为根的子树，递归调用算法MaxHeapify。
### 算法CPP代码实现：
```cpp
//得到父节点索引
int getParent(int i)
{
	return (int)floor((float)i / 2);
}
//得到左子树索引
int getLeftSon(int i)
{
	return (2 * i);
}
//得到右子树索引
int getRightSon(int i)
{
	return (2 * i + 1);
}
//调整以某个节点i为根节点的子树为大根堆
void MaxHeapify(int A[],int i,int HeapSize)
{
	int left = getLeftSon(i);
	int right = getRightSon(i);
	int largest = i;//记录值最大的元素的索引
	if (left <= HeapSize && A[left] > A[i])
	{
		largest = left;
	}
	if (right <= HeapSize && A[right] > A[largest])
	{
		largest = right;
	}
	if (largest != i)//此子树不满足大根堆的性质，需要进行调整
	{
		//进行交换
		int temp = A[i];
		A[i] = A[largest];
		A[largest] = temp;
		
		MaxHeapify(A,largest,HeapSize);//递归调用，继续调整子树
	}
}
```
### 调整堆示例
![](https://img-my.csdn.net/uploads/201303/23/1364004875_5090.jpg)
### 时间复杂度的分析：
调整A[i]、A[left]、A[right]的时间为常量时间。
下面分析递归调整以i的某个子节点为根的子树所需的时间。
假设树的节点个数为n，则i节点的子树节点个数最多为2n/3(在最底层刚好半满的时候，由完全二叉树性质可得),
推导过程：
树总的节点个数
n = pow(2,0) + pow(2,1) + ... + pow(2,h - 1 - 1) + 1 / 2 * pow(2,h - 1),其中h为树的高度(根为第一层)
= 3 * pow(2,h - 2) - 1
假设根节点的左儿子所对应的子树节点数大于右子树的节点，其高度应为h - 1，节点数：
pow(2,0) + pow(2,1) + ... + pow(2,h - 1 - 1)  = pow(2,h - 1) - 1
结合上面的式子可达到，子树的最大节点数为(2 * n - 1) / 3.
这样，调整堆的时间复杂度的推算为：
T(n) <= T(2n / 3) + O(1)
求解递归式得到：T(n) = O(lgn)。或者是O(h)，h为树的高度。
## **三、建堆**
### 建堆思想
我们可以自底向上地调用上述调整堆的方法MaxHeapify来将一个数组变成最大堆。
注意：子数组A[floor(n/2) + 1]...A[n]是树的叶子。显然叶子可以看成只有一个元素的大根堆，不用调整。只需从非叶子节点自后向前依次调整即可。
### 建堆示例
下图为建堆的一个示例：
![](https://img-my.csdn.net/uploads/201303/23/1364006115_6861.jpg)
![](https://img-my.csdn.net/uploads/201303/23/1364006078_1699.jpg)
最后一个非叶节点是2，因此从2开始往前依次调用调整函数。
### C++代码实现：
```cpp
//建堆
void buildMaxHeap(int A[],int HeapSize)
{
	for (int i = (int)floor((float)HeapSize / 2);i > 0;--i)
	{
		MaxHeapify(A,i,HeapSize);
	}
	cout << "建成的大根堆：" << endl;
	printHeap(A,HeapSize);
}
```
时间复杂度分析
一个有n个元素堆的高度为floor(lgn)，并且在任意高度h上之多有ceil(n/pow(2,h+1))个节点。
这样，时间复杂度推算为：
![](https://img-my.csdn.net/uploads/201303/23/1364008961_4326.jpg)
而右边的和式可以如下计算：
由式子：
![](https://img-my.csdn.net/uploads/201303/23/1364008971_3113.jpg)
两端求导并乘以x可得：
，因此有：
![](https://img-my.csdn.net/uploads/201303/23/1364008987_5425.jpg)
于是，建堆的时间复杂度：
![](https://img-my.csdn.net/uploads/201303/23/1364008997_7299.jpg)
即可以在线性时间内将一个无序数组建成大根堆。
## **四、堆排序**
### 算法思想
首先是将无序数组建成大根堆，然后通过把最大元素即根与A[n]互换使得最大元素到达正确位置。
然后将节点n从堆中去掉，原来根的子女仍然是大根堆，但是新的根元素可能违反了大根堆的规则，必须重新调整A[1,...,n - 1]为大根堆。这样重复进行。直至堆的大小变为1才结束。
堆排序示例
![](https://img-my.csdn.net/uploads/201303/23/1364006612_7910.jpg)
![](https://img-my.csdn.net/uploads/201303/23/1364006866_6274.jpg)
![](https://img-my.csdn.net/uploads/201303/23/1364006935_2004.jpg)
![](https://img-my.csdn.net/uploads/201303/23/1364006981_3934.jpg)
![](https://img-my.csdn.net/uploads/201303/23/1364007042_9705.jpg)
![](https://img-my.csdn.net/uploads/201303/23/1364007105_2335.jpg)
![](https://img-my.csdn.net/uploads/201303/23/1364007199_8337.jpg)
![](https://img-my.csdn.net/uploads/201303/23/1364007234_5369.jpg)
![](https://img-my.csdn.net/uploads/201303/23/1364007275_5673.jpg)
![](https://img-my.csdn.net/uploads/201303/23/1364007308_1632.jpg)
### C++代码实现：
```cpp
//堆排序
void heapSort(int A[],int HeapSize)
{
	buildMaxHeap(A,HeapSize);
	for (int i = HeapSize;i > 0;--i)
	{
		int temp = A[1];
		A[1] = A[i];
		A[i] = temp;
		MaxHeapify(A,1,i - 1);
	}
}
```
### 时间复杂度分析
堆排序的时间复杂度是O(nlgn)。其中建堆的时间是O(n)。n-1次调整堆，每次时间代价是O(logn)。
而且是一种原地排序算法，即在任何时刻数组中只有常数个元素存储在输入数组以外。
程序实例：
```cpp
#include <iostream>
#include <cmath>
using namespace std;
//注意：下表都以1开始，而不是0
//得到父节点索引
int getParent(int i)
{
	return i>>1;
}
//得到左子树索引
int getLeftSon(int i)
{
	return i<<1;
}
//得到右子树索引
int getRightSon(int i)
{
	return ((i<<1) + 1);
}
//调整以某个节点i为根节点的子树为大根堆
void MaxHeapify(int A[],int i,int HeapSize)
{
	int left = getLeftSon(i);
	int right = getRightSon(i);
	int largest = i;//记录值最大的元素的索引
	if (left <= HeapSize && A[left] > A[i])
	{
		largest = left;
	}
	if (right <= HeapSize && A[right] > A[largest])
	{
		largest = right;
	}
	if (largest != i)//此子树不满足大根堆的性质，需要进行调整
	{
		//进行交换
		int temp = A[i];
		A[i] = A[largest];
		A[largest] = temp;
		
		MaxHeapify(A,largest,HeapSize);//递归调用，继续调整子树
	}
}
//输出数组元素
void printHeap(int A[],int HeapSize)
{
	for(int i = 1;i <= HeapSize;++i)
	{
		cout << A[i] << " ";
	}
	cout << endl;
}
//建堆
void buildMaxHeap(int A[],int HeapSize)
{
	for (int i = (int)floor((float)HeapSize / 2);i > 0;--i)
	{
		MaxHeapify(A,i,HeapSize);
	}
	cout << "建成的大根堆：" << endl;
	printHeap(A,HeapSize);
}
//堆排序
void heapSort(int A[],int HeapSize)
{
	buildMaxHeap(A,HeapSize);
	for (int i = HeapSize;i > 0;--i)
	{
		int temp = A[1];
		A[1] = A[i];
		A[i] = temp;
		MaxHeapify(A,1,i - 1);
	}
}
int main()
{
	const int length = 11;
	//堆元素下班从1开始
	int A[length] = {0,4,1,3,2,16,9,10,14,8,7};
	int HeapSize = length - 1;
	heapSort(A,HeapSize);
	cout << "堆排序之后：" << endl;
	printHeap(A,HeapSize);
}
```
运行结果：
![](https://img-my.csdn.net/uploads/201205/25/1337949402_6639.jpg)
## 五、堆排序的应用
堆排序[应用](http://www.51projob.com/a/shujujiegou/20120823/404.html)广泛：
（1）STL中[优先级队列](http://blog.csdn.net/xiajun07061225/article/details/8556786)。
（2）寻找第K大的数或者最大的K个数。具体可以参考《编程之美》。
比如海量数据处理中，从1亿个数中找出最大的1000个。
在构建哈夫曼树时，也可以使用这种策略来提高效率。我们需要每次从集合中选择两个最小的元素，然后将元素相加，合并为一个新的元素。我们可以堆来提高效率。
