# 求前K小的数和第K小的数 - bigfacesafdasgfewgf - CSDN博客





2015年01月28日 10:11:30[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：590标签：[快排																[快速选择																[算法](https://so.csdn.net/so/search/s.do?q=算法&t=blog)
个人分类：[Algorithm Probelm																[Data Structure](https://blog.csdn.net/puqutogether/article/category/2595131)](https://blog.csdn.net/puqutogether/article/category/2812137)





**求前K小的数和第K小的数**

# 1. 前K小的数    （堆排序实现）



```cpp
#include<iostream>
#include<algorithm>
#include <string>
using namespace std; 

// 调整堆函数
void HeapAdjust(int *a, int i, int size)  // a：数组；i：节点index；size：树的规模
{
	int lchild = 2 * i + 1;   // 节点的左孩子编号
	int rchild = 2 * i + 2;   // 节点的右孩子编号
	int max = i;  //节点编号备份
	if(i <= size/2 - 1)
	{
		if(lchild <= size && a[lchild] > a[max])  // 左孩子比节点值要大
			max = lchild; 
		if(rchild <= size && a[rchild] > a[max])  // 右孩子比节点值要大
			max = rchild; 
		if(max != i)  // 如果上面发生了交换，也就是如果上面有if成立了
		{
			swap(a[i], a[max]);   // 首先要交换二者的真实值，因为上面只是index的交换
			HeapAdjust(a, max, size);  // 其次，需要判断交换之后还需要重新调整吗
		}
	}
}

//建立堆函数
void BuildHeap(int *a, int size)  
{
	int i; 
	for(i = size/2 - 1; i >= 0; i--)  // 非叶子节点最大序号值为size/2，需要遍历所有的节点
	{
		HeapAdjust(a, i, size);   // 调整
	}
}

// 堆排序函数
void HeapSort(int *a, int size)
{
	int i; 
	BuildHeap(a, size); 
	for(i = size; i >= 0; i--)
	{
		swap(a[0], a[i]);  //  交换堆顶和最后一个元素，每次将剩余元素中的最大者放到最后面
		HeapAdjust(a, 0, i-1);  // 重新调整堆顶节点成为大顶堆，这个时候就要除出最后一个元素，范围是1~n-1
	}
}

int main()
{
	int a[] = {100, 16, 20, 3, 11, 17, 8, 2, 5, 7};
	int b[10]; 
	int size = 10; 
	int k = 5;  // 找出最小的k=5个数
	memcpy(b, a, sizeof(b)); 
	
	BuildHeap(b, k); 
	
	for(int i = k; i < size; i++)
	{
		cout<<b[0]<<' '<<b[1]<<' '<<b[2]<<' '<<b[3]<<' '<<b[4]<<endl; 
		if(a[i] < b[0])
		{
			cout<<"swap: "<<a[i]<<endl; 
			b[0] = a[i]; 
			cout<<b[0]<<' '<<b[1]<<' '<<b[2]<<' '<<b[3]<<' '<<b[4]<<endl; 
			BuildHeap(b, k); 
		} 
		
		cout<<endl; 
	}
	for(int j = 0; j < k; j++)
		cout<<b[j]<<endl; 


	return 0; 


}
```





# 2. 第K小的数（快速选择和双向扫描的快排版本）





```cpp
#include<iostream>
#include<time.h>
using namespace std; 

int kthElem(int a[], int low, int high, int k)
{
	int pivot = a[low];  // 第一个数为主元
	int low_temp = low; 
	int high_temp = high; 
	while(low < high)
	{
		while(low < high && a[high] >= pivot)  // 如果最后一个数比主元大，保留
			--high; 
		a[low] = a[high]; 
		while(low < high && a[low] < pivot)
			++low; 
		a[high] = a[low]; 
	}
	a[low] = pivot; 

	if(low == k-1)
		return a[low]; 
	else if(low > k-1)
		return kthElem(a, low_temp, low-1, k); 
	else 
		return kthElem(a, low+1, high_temp, k); 
}

int main()
{
	for(int num = 5000; num < 500001; num *= 10)
	{
		int *array = new int[num]; 
		int j = num/10; 
		int acc = 0; 
		for(int k = 1; k <= num; k += j)
		{
			srand(unsigned(time(0))); 
			for(int i = 0; i < num; i++)
				array[i] = rand() * RAND_MAX + rand(); 

			clock_t start = clock();  
            int data = kthElem(array, 0, num - 1, k);  
            clock_t end = clock();  
            acc += (end - start);
		}
		cout << "The average time of searching a value in the array size of " << num << " is " << acc / 10 << endl; 
	}

	return 0; 
}
```







参考链接：



http://blog.csdn.net/v_JULY_v/article/details/6370650


http://blog.csdn.net/v_JULY_v/article/details/6403777


http://blog.csdn.net/v_JULY_v/article/details/6262915


http://blog.csdn.net/v_july_v/article/details/6543438](https://so.csdn.net/so/search/s.do?q=快速选择&t=blog)](https://so.csdn.net/so/search/s.do?q=快排&t=blog)




