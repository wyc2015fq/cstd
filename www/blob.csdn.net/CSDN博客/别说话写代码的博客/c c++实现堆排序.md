# c/c++实现堆排序 - 别说话写代码的博客 - CSDN博客





2017年08月02日 21:18:38[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：106标签：[c++																[数据结构																[排序](https://so.csdn.net/so/search/s.do?q=排序&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)





```cpp
///
 /// @file    heapSort.cc
 /// @author  guoleida(496919313@qq.com)
 /// @date    2017-08-02 14:57:43
 ///
#include <string> 
#include <iostream>
using std::cout;
using std::endl;
using std::string;

template <typename T >
class HeapSort
{
public:
	HeapSort(T *arr,int size);
	void heapAdjust(int k,int len);
	void sort();
	void buildMaxHeap();
private:
	T *_arr;
	int _size;
};

template<typename T>
HeapSort<T>::HeapSort(T *arr,int size)//构造函数
: _arr(arr)
, _size(size)
{}

template<typename T>
void HeapSort<T>::sort()
{
	buildMaxHeap();//初始建堆
	T tmp;
	for(int i=_size;i>1;i--)//n-1趟交换和建堆过程
	{
		tmp = _arr[i];//将堆底元素和堆顶元素交换，并输出堆顶元素
		_arr[i] = _arr[1];
		_arr[1] = tmp;

		heapAdjust(1,i-1);
	}


}

template<typename T>
void HeapSort<T>::buildMaxHeap()
{
	for(int i=_size/2;i>0;i--)
		heapAdjust(i,_size);
}

template<typename T>
void HeapSort<T>::heapAdjust(int k,int len)
//函数heapadjust将元素k向下进行调整
{
	_arr[0] = _arr[k];
	for(int i=2*k; i<=len; i*=2)//沿着key较大的子节点向下筛选
	{
		if(i < len && _arr[i] < _arr[i+1])
			i++; //取key较大的子节点的下标
		if(_arr[0] >= _arr[i])
			break;//筛选结束
		else
		{
			_arr[k] = _arr[i];//将_arr[i]调整到双亲节点上
			k=i;//修改k值，一边继续向下筛选
		}
	}
	_arr[k] = _arr[0];
}


int main()
{
	int a[]={0,7,5,2,9,8,1,2,5,8,7};//第一个位置不存放实际数据
	for(int i=1;i<11;i++)
		cout << a[i] <<" ";
	cout <<endl;
	HeapSort<int> hp(a,10);
	hp.sort();
	for(int i=1;i<11;i++)
		cout << a[i] <<" ";
	cout <<endl;
}
```](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)




