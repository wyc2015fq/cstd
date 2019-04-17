# 【sizeof】关于各种东西的大小_Tencent - CD's Coding - CSDN博客





2015年03月26日 15:42:42[糖果天王](https://me.csdn.net/okcd00)阅读数：538标签：[sizeof																[各种东西的大小](https://so.csdn.net/so/search/s.do?q=各种东西的大小&t=blog)](https://so.csdn.net/so/search/s.do?q=sizeof&t=blog)
个人分类：[开发备忘](https://blog.csdn.net/okcd00/article/category/2611293)










```cpp
</pre><p></p><p></p><p>之前腾讯模拟笔试的时候，出了一道sizeof的题目，问没有规定大小的各种数据类型指针的size，于是这次还是写个文章测试顺便备忘一下。</p><p>测试代码：</p><p></p><pre code_snippet_id="628654" snippet_file_name="blog_20150326_2_2178915" name="code" class="cpp">#include <map>
#include <cmath>
#include <vector>
#include <cstdio>
#include <string>
#include <cstring> 
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;

int main()
{
	int *a,b;
	float *c,d;
	double *e,f;
	char *g,h;
	map<int,int> mii;
	map<double,double> mdd;
	map<char,char> mcc;
	map<string,int> msi;
	vector<int> vi;
	vector<double> vd;
	string *s,str;
	
	cout<<"int* \t"<<sizeof a<<endl;
	cout<<"int \t"<<sizeof b<<endl;
	cout<<"float* \t"<<sizeof c<<endl;
	cout<<"float \t"<<sizeof d<<endl;
	cout<<"double* "<<sizeof e<<endl;
	cout<<"double \t"<<sizeof f<<endl;
	cout<<"char* \t"<<sizeof g<<endl;
	cout<<"char \t"<<sizeof h<<endl;
	cout<<"mii \t"<<sizeof mii<<endl;
	cout<<"mdd \t"<<sizeof mdd<<endl;
	cout<<"mcc \t"<<sizeof mcc<<endl;
	cout<<"msi \t"<<sizeof msi<<endl;
	cout<<"veci \t"<<sizeof vi<<endl;
	cout<<"vecd \t"<<sizeof vd<<endl;
	cout<<"str* \t"<<sizeof s<<endl;
	cout<<"str \t"<<sizeof str<<endl;
	return 0; 
}
```









**结果**

**int*    8int     4float*  8float   4double* 8double  8char*   8char    1mii     48mdd     48mcc     48msi     48veci    24vecd    24str*    8str     8**










```cpp
#include<iostream>

class A
{
};

class B
{
private:
	int a;
};
class B1
{
private:
	void fun()
	{}
};


class C
{
	virtual void fun() = 0;
};

class D
{
	virtual void fun()
	{
		std::cout << "this is a D1.";
	}
};

class D1
{
	virtual void fun()
	{
		std::cout << "this is a D2.";
	}
	virtual void fun1()
	{
		std::cout << "this is a D2.";
	}
};

class D2:public D1,public D
{
	virtual void fun()
	{
		std::cout << "this is a D3.";
	}
	virtual void fun1()
	{
		std::cout << "this is a D3.";
	}
};

class D3 :public D1, public D
{
	virtual void fun3()
	{
		std::cout << "this is a D3.";
	}
	
};
class D4 :public D1, public B1
{
	virtual void fun3()
	{
		std::cout << "this is a D3.";
	}

};


class E 
{
	void show()
	{
		std::cout << "THIS IS E";
	}
};

class F :public A
{
	virtual void show()
	{
		std::cout << "THIS IS E";
	}
};


int main()
{
	std::cout << "empty class's size is " << sizeof(A) << std::endl;
	std::cout << "only one int class's size is " << sizeof(B) << std::endl;
	std::cout << "only one funciton class's size is " << sizeof(B1) << std::endl;
	std::cout << "only one virtual pure funciton class's size is " << sizeof(C) << std::endl;
	std::cout << "only one virtual function class's size is " << sizeof(D) << std::endl;
	std::cout << "two virtual function class's size is " << sizeof(D1) << std::endl;

	std::cout << "three virtual function two jicheng(TWO VIRTUAL) class's size is " << sizeof(D2) << std::endl;
	std::cout << "three virtual function two jicheng(ONE VIRTUAL) class's size is " << sizeof(D4) << std::endl;
	std::cout << "four virtual function two jicheng class's size is " << sizeof(D3) << std::endl;

	std::cout << "only one  function class's size is " << sizeof(E) << std::endl;
	std::cout << "only one  virtual function class's size is " << sizeof(E) << std::endl;
	getchar();


}
```

**反正写了也是写了，再看看类吧**





**empty class's size is 1only one int class's size is 4only one funciton class's size is 1only one virtual pure funciton class's size is 8only one virtual function class's size is 8two virtual function class's size is 8three virtual function two jicheng(TWO VIRTUAL) class's size is 16three virtual function two jicheng(ONE VIRTUAL) class's size is 8four virtual function two jicheng class's size is 16only one  function class's size is 1only one  virtual function class's size is 1**







