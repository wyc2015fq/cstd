# C++中new和delete的使用.c - qq_37385726的博客 - CSDN博客





2017年04月15日 13:08:40[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：147
个人分类：[C++](https://blog.csdn.net/qq_37385726/article/category/6862768)










```
#include
using namespace std;
#include
int main(void)
{
	int m = 5;//二维数组的行数
	int n = 4;//二维数组的列数

//关于new运算符的使用
	int *p1;
	p1 = new int;//为int型变量分配堆内存（这个堆内存就是指针p的内存，通过指针p可以改变这个int型的变量）
	*p1 = 3;//将p1指向地址的值赋为3
	int *p2;
	p2 = new int(3);//这里与上面的p1是类似的，这个也是将p2分配一个内存空间，并且将p2指向的地址的值赋为3====这个是将前者作为主语
	                //为初值为3的int型变量分配堆内存=====这个是将后者作为主语
	                //这两种表述都是对的
	int *p3;
	p3 = new int[5];//这个是将含有5个元素的int型数组分配堆内存
	int **p4;
	p4 = new int*[m];
	for (int i = 0; i < m; i++)
	{
		p4[i] = new int[n];
	}

//关于delete运算符的使用
	delete p1;
	delete p2;
	delete[]p3;
	for (int k = 0; k < m; k++)
	{
		delete[]p4[k];
	}
	system("pause");
	return 0;
}
```




