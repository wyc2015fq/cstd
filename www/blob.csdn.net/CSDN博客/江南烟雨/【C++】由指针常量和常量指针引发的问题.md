# 【C++】由指针常量和常量指针引发的问题 - 江南烟雨 - CSDN博客
2013年07月18日 21:30:20[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：3506标签：[C++																[stl](https://so.csdn.net/so/search/s.do?q=stl&t=blog)](https://so.csdn.net/so/search/s.do?q=C++&t=blog)
个人分类：[C/C++](https://blog.csdn.net/xiajun07061225/article/category/723127)
今天在写一个C++程序的时候，要用到STL的set容器，而且我想让它利用自定义的排序准则来实现自动排序。而且set中元素类型是指向自定义的结构体的指针。结果运行总是出错。
下面先贴出起初的代码：
```cpp
#include <iostream>
#include <set>
using namespace std;
typedef struct test_t{
	int a;
	int b;
}test;
class PersonalCriterion{
public:
	bool operator()(const test* &obj1,const test* &obj2){
		return obj1->a < obj2->a;
	}
};
int main(){
	set<test*,PersonalCriterion> myset;
	test* obj1 = new test();
	obj1->a = 10;
	obj1->b = 9;
	test* obj2 = new test();
	obj2->a = 5;
	obj2->b = 9;
	myset.insert(obj1);
	myset.insert(obj2);
	return 0;
}
```
结果在VS2008中编译运行出错，错误信息如下：
![](https://img-blog.csdn.net/20130718211830109?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhanVuMDcwNjEyMjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
显然问题出在这个自定义的仿函数排序准则这里。那么究竟是什么问题呢？错误信息显示这里丢失了限定符，即const。但是我这里参数上明明有const呀，疑惑！
再次仔细看输出的错误信息，不能将参数一从类型1转化到类型2.这两个类型有什么区别呢？答案是const的位置不同：指针常量和常量指针的区别！在有指针的地方，const放在不同的位置会导致完全不同的结果。前一种类型中，const是来修饰指针本身的，即指针常量，它指向的对象确定了之后就不能修改了，即不能修改使得它指向另外一个对象。而第二种类型，const是来修饰指针所指向的类型的，即常量指针，指针本身不是const的，可以修改指向别的对象。但是指针所指向的地址处的内容却不能修改，它是const的。
![](https://img-blog.csdn.net/20130718212126343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhanVuMDcwNjEyMjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
接下来就可以对代码进行修改了。为了避免出现上述的问题，我们用typedef重新定义了指向自定义结构体的指针类型：
![](https://img-blog.csdn.net/20130718212900546?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhanVuMDcwNjEyMjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这样就不会出现原来的问题了。修改正确的代码如下：
```cpp
#include <iostream>
#include <set>
using namespace std;
typedef struct test_t{
	int a;
	int b;
}test;
typedef test* test_ptr;
class PersonalCriterion{
public:
	bool operator()(const test_ptr &obj1,const test_ptr &obj2){
		return obj1->a < obj2->a;
	}
};
int main(){
	set<test_ptr,PersonalCriterion> myset;
	test_ptr obj1 = new test();
	obj1->a = 10;
	obj1->b = 9;
	test_ptr obj2 = new test();
	obj2->a = 5;
	obj2->b = 9;
	test_ptr obj3 = new test();
	obj3->a = 20;
	obj3->b = 48;
	test_ptr obj4 = new test();
	obj4->a = 1;
	obj4->b = 2;
	test_ptr obj5 = new test();
	obj5->a = 3;
	obj5->b = 39;
	myset.insert(obj1);
	myset.insert(obj2);
	myset.insert(obj3);
	myset.insert(obj4);
	myset.insert(obj5);
	set<test_ptr,PersonalCriterion>::iterator iter;
	for (iter = myset.begin();iter != myset.end();++iter)
	{
		cout << (*iter)->a << endl;
	}
	return 0;
}
```
运行结果：
![](https://img-blog.csdn.net/20130718214337562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhanVuMDcwNjEyMjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
参考资料：
[http://www.cppblog.com/cc/archive/2006/03/12/4045.html](http://www.cppblog.com/cc/archive/2006/03/12/4045.html)
