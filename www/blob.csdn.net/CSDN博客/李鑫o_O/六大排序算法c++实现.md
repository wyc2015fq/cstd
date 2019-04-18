# 六大排序算法c++实现 - 李鑫o_O - CSDN博客

2016年03月19日 11:04:42[hustlx](https://me.csdn.net/HUSTLX)阅读数：585
个人分类：[c/c++																[算法](https://blog.csdn.net/HUSTLX/article/category/6145896)](https://blog.csdn.net/HUSTLX/article/category/6122695)



# ![](https://img-blog.csdn.net/20160502151018258?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

# 插入排序：

```cpp
void insertionSort(vector<int> &A) {
	for (int j = 1;j < A.size();j++) {
		int key = A[j];
		int i=j-1;
		while(i>=0&&key < A[i]){
			A[i + 1] = A[i];
			i--;
		}
		A[i + 1] = key;
	}
}
```
选择排序

```cpp
void selectionSort(vector<int> &A) {
	for (int j = 0;j < A.size();j++) {
		int min = j;
		for (int i = j + 1;i < A.size();i++) {
			if (A[i] < A[min])
				min = i;
		}
		if(j!=min) swap(A[j],A[min]);
	}
}
```
归并排序

```java
//
void Merge(vector<int>&A, int p, int q, int r)
{
	vector<int> L(A.begin() + p, A.begin() + q+1);   //A[p......q]
	vector<int> R(A.begin() + q+1, A.begin() + r+1); //A[q+1......r]
	L.push_back(INT_MAX);
	R.push_back(INT_MAX);
	for (int i = 0, j = 0, k = p; k <= r; k++)
	{
		if (L[i] <= R[j]) A[k] = L[i++];
		else A[k] = R[j++];
	}
}
void MergeSort(vector<int>& A, int p, int r)
{
	if (p<r)
	{
		int q = (p + r) / 2;
		MergeSort(A, p, q);
		MergeSort(A, q + 1, r);
		Merge(A, p, q, r);
	}
}
```

```java
* merge_sort: 非递归实现 --迭代
* 非递归思想: 将数组中的相邻元素两两配对。用merge函数将他们排序，
* 构成n/2组长度为2的排序好的子数组段，然后再将他们排序成长度为4的子数组段，
* 如此继续下去，直至整个数组排好序。
**/
// merge_sort(): 非递归实现-自底向上
// 将原数组划分为left[min...max] 和 right[min...max]两部分
void merge_sort(vector<int> &list)
{
	int length = list.size();
	vector<int> temp(length, 0);
	int i, left_min, left_max, right_min, right_max, next;
	for (i = 1; i < length; i *= 2) // i为步长，1,2,4,8……
	{
		for (left_min = 0; left_min < length - i; left_min = right_max)
		{
			right_min = left_max = left_min + i;
			right_max = left_max + i;
			if (right_max > length)
				right_max = length;
			next = 0;
			while (left_min < left_max && right_min < right_max)
				temp[next++] = list[left_min] > list[right_min] ? list[right_min++] : list[left_min++];
			
			while (left_min < left_max)
				list[--right_min] = list[--left_max];

			while (next > 0)
				list[--right_min] = temp[--next];
		}
	}
}
```

冒泡排序

```cpp
void bubbleSort(vector<int> &A) {
    for (int j = 0; j < A.size(); j++)
        for (int i = A.size() - 1; i > j; i--)
            if (A[i] < A[i - 1]) swap(A[i], A[i - 1]);
}
```

堆排序

非递归：

```cpp
void maxHeapify(vector<int>& A, int heap_size, int i) {
    int largest;
    while (i * 2 < heap_size) {
        largest = i * 2 + 1;
        if (largest<heap_size&&A[largest + 1]>A[largest]) {
            largest++;
        }
        if (A[i]<A[largest]) {//如果自己的孩子比自己大则交换两者的值  
            swap(A[i], A[largest]);
        }
        else break; //如果不比自己大则不交换  
        i = largest;
    }
}
```

递归：

```cpp
void maxHeapify(vector<int>& A, int heap_size, int i) {
    int largest = i * 2 + 1;
    if (largest > heap_size) return;
    if (largest<heap_size&&A[largest + 1]>A[largest]) {
        largest++;
    }
    if (A[i]<A[largest]) {//如果自己的孩子比自己大则交换两者的值  
        swap(A[i], A[largest]);
        maxHeapify(A, heap_size, largest);
    }
}
```

```cpp
void buildMaxHeap(vector<int>& A) {
	int heap_size = A.size() - 1;
	for (int i = heap_size / 2;i >= 0;i--) {
		maxHeapify(A, heap_size,i);
	}
}

void HeapSort(vector<int>& A) {
	buildMaxHeap(A);
	int heap_size = A.size() - 1;
	while (heap_size >= 1) {
		swap(A[heap_size], A[0]);
		heap_size--;
		maxHeapify(A, heap_size, 0);
	}
}
```

快排

```cpp
int partition(vector<int>& A, int p, int r) {
	int x = A[r];
	int i = p;
	for (int j = p;j < r;j++) {
		if (A[j] <= x) {
			swap(A[j], A[i]);
			i++;
		}
	}
	swap(A[r], A[i]);
	return i;
}
void QuickSort(vector<int>& A, int p, int r)
{
	if (p<r)
	{
		int q = partition(A,p,r);
		QuickSort(A, p, q-1);
		QuickSort(A, q + 1, r);
	}
}
```

快排非递归实现：

数据规模很大时，递归的算法很容易导致栈溢出，改为非递归，模拟栈操作，

最大长度为n，每次压栈时先压长度较大的，此时栈深度为logn。

```cpp
/**使用栈的非递归快速排序**/
template<typename Comparable>
void quicksort2(vector<Comparable> &vec,int low,int high){
    stack<int> st;
    if(low<high){
        int mid=partition(vec,low,high);
        if(low<mid-1){
            st.push(low);
            st.push(mid-1);
        }
        if(mid+1<high){
            st.push(mid+1);
            st.push(high);
        }
        //其实就是用栈保存每一个待排序子串的首尾元素下标，下一次while循环时取出这个范围，对这段子序列进行partition操作
        while(!st.empty()){
            int q=st.top();
            st.pop();
            int p=st.top();
            st.pop();
            mid=partition(vec,p,q);
            if(p<mid-1){
                st.push(p);
                st.push(mid-1);
            }
            if(mid+1<q){
                st.push(mid+1);
                st.push(q);
            }       
        }
    }
}
```


