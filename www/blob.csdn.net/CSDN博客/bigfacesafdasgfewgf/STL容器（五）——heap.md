# STL容器（五）——heap - bigfacesafdasgfewgf - CSDN博客





2015年01月27日 18:08:31[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：878








**STL容器（五）——heap**



    我们可以调用STL中的heap堆，实现建堆和堆排序等操作。




    首先，STL中的heap默认构建的是最大堆，程序如下：



```cpp
#include<iostream>
#include<algorithm>
#include<vector>
using namespace std; 

int main()
{
	int a[]={2, 3, 5, 1, 9, 6}; 
	vector<int> vec(a, a+6); 
	vector<int>::iterator iter; 

	//建堆，默认是最大堆
	make_heap(vec.begin(), vec.end()); 
	cout<<"建堆之后："<<endl; 
	for(iter = vec.begin(); iter != vec.end(); iter++)
		cout<<*iter<<' '; 
	cout<<endl; 


	//增加元素
	vec.push_back(8);   // 先在vector容器中增加元素
	push_heap(vec.begin(), vec.end());   // 再做堆调整
	cout<<"增加元素之后："<<endl; 
	for(iter = vec.begin(); iter != vec.end(); iter++)
		cout<<*iter<<' '; 
	cout<<endl; 

	//删除元素，默认是堆顶元素
	pop_heap(vec.begin(), vec.end());   // 先做堆调整
	vec.pop_back();   // 再从vector容器中删除元素
	cout<<"删除元素之后："<<endl; 
	for(iter = vec.begin(); iter != vec.end(); iter++)
		cout<<*iter<<' '; 
	cout<<endl; 

	//堆排序，最大堆排序之后是正序
	sort_heap(vec.begin(), vec.end()); 
	cout<<"堆排序之后："<<endl; 
	for(iter = vec.begin(); iter != vec.end(); iter++)
		cout<<*iter<<' '; 
	cout<<endl; 

	return 0; 

}
```




结果：

![](https://img-blog.csdn.net/20150127180910401?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)








    我们发现，heap的操作还是要依靠vector容器的，它只是在vector的基础上，实现make_heap, push_heap, pop_heap, sort_heap的操作。




    如果我们想建立一个最小堆呢？这就需要我们看看STL中对heap操作的定义了，如下：

make_heap()函数原型是：void make_heap(first_pointer,end_pointer,compare_function); 一个参数是数组或向量的头指针，第二个向量是尾指针。第三个参数是比较函数的名字。在缺省的时候，默认是大跟堆。（下面的参数都一样就不解释了）作用：把这一段的数组或向量做成一个堆的结构。范围是(first,last)


pop_heap();函数原型是：void pop_heap(first_pointer,end_pointer,compare_function);作用：pop_heap()不是真的把最大（最小）的元素从堆中弹出来。而是重新排序堆。它把first和last交换，然后将[first,last-1)的数据再做成一个堆。


push_heap()的函数原型是void pushheap(first_pointer,end_pointer,compare_function); 作用：push_heap()假设由[first,last-1)是一个有效的堆，然后，再把堆中的新元素加进来，做成一个堆。


sort_heap()的函数原型是void sort_heap(first_pointer,end_pointer,compare_function);作用是sort_heap对[first,last)中的序列进行排序。





那么每个函数最后的compare-function到底是什么呢？其实这个就是决定了我们建立的是最大堆还是最小堆。这个比较函数和qsort一样，是可以自定义的，如下：






```cpp
#include<iostream>
#include<algorithm>
#include<vector>
using namespace std; 

bool cmp(int a, int b)
{
	return a > b;   //如果a较大，返回真， a排在后面，最小值在前面，即最小堆
}

int main()
{
	int a[]={2, 3, 5, 1, 9, 6}; 
	vector<int> vec(a, a+6); 
	vector<int>::iterator iter; 

	//建堆，此时是最小堆
	make_heap(vec.begin(), vec.end(), cmp); 
	cout<<"建堆之后："<<endl; 
	for(iter = vec.begin(); iter != vec.end(); iter++)
		cout<<*iter<<' '; 
	cout<<endl; 


	//增加元素
	vec.push_back(0);   // 先在vector容器中增加元素
	push_heap(vec.begin(), vec.end(), cmp);   // 再做堆调整
	cout<<"增加元素之后："<<endl; 
	for(iter = vec.begin(); iter != vec.end(); iter++)
		cout<<*iter<<' '; 
	cout<<endl; 

	//删除元素，默认是堆顶元素，这里我们删除两个元素
	pop_heap(vec.begin(), vec.end(), cmp);   // 先做堆调整
	vec.pop_back();   // 再从vector容器中删除元素
	pop_heap(vec.begin(), vec.end(), cmp);   // 先做堆调整
	vec.pop_back();   // 再从vector容器中删除元素
	cout<<"删除元素之后："<<endl; 
	for(iter = vec.begin(); iter != vec.end(); iter++)
		cout<<*iter<<' '; 
	cout<<endl; 

	//堆排序，由于构建的是最小堆，所以这里排序是逆序的
	sort_heap(vec.begin(), vec.end(), cmp); 
	cout<<"堆排序之后："<<endl; 
	for(iter = vec.begin(); iter != vec.end(); iter++)
		cout<<*iter<<' '; 
	cout<<endl; 

	return 0; 

}
```


结果：

![](https://img-blog.csdn.net/20150127180814834?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





注意：如果构建的是最大堆，那么排序之后就是正序的；如果构建的是最小堆，那么排序之后就是逆序的。




这样我们就会通过STL中的heap来使用堆排序了。




参考链接：




http://blog.csdn.net/puqutogether/article/details/43195703


http://blog.csdn.net/hnust_xiehonghao/article/details/9172875


http://blog.csdn.net/morewindows/article/details/6967409


http://blog.csdn.net/lwfcgz/article/details/8760092







