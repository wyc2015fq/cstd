# 设计模式之适配器，C++实现 - 长歌行 - CSDN博客





2013年01月31日 16:22:08[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：902
所属专栏：[初探设计模式（C++实现）](https://blog.csdn.net/column/details/baadf00df00dabadfds0.html)









大家应该用得比较多，就只上代码






```cpp
// AdapterMode.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

// 适配器模式（Adapter），将一个类的借口转换成客户端希望
// 的另外一个借口，适配器模式使得原本由于借口不兼容而不能
// 一起工作的那些类可以一起工作


// 说白了，就是改名

// 情景假设：一个计算类的派生类由新手和老手分别写
// 新手的命名有可能有些问题

class MathAdd
{
public:
	virtual int Add(int a, int b) = 0;
protected:
private:
};

// 老手这么写
class MathAdd_Veteran : public MathAdd
{
public:
	int Add(int a, int b)
	{
		return a + b;
	}
};

// 菜鸟这么写
class MathAdd_Novice
{
public:
	int jiafa(int a, int b)
	{
		return a + b;
	}
protected:
private:
};

// 好吧，为了让新手有满足感，使用下新手的代码
// 就需要这么做了。。

class MathAdd_Helper : public MathAdd
{
public:
	int Add(int a, int b)
	{
		return m_Mn.jiafa(a, b);
	}
private:
	MathAdd_Novice m_Mn;
};

// 规定必须使用的函数接口
int Add(MathAdd* pMa, int a, int b)
{
	return pMa->Add(a, b);
}

int _tmain(int argc, _TCHAR* argv[])
{
	MathAdd *pMv = (MathAdd*) new MathAdd_Veteran;
	int r = Add(pMv, 100, -20);

	// 好吧，唯一的遗憾是，新手的名字没有在客户端展示
	// 但是实际上调用的是他的类哦
	// 谁让你不按接口编程的！
	MathAdd *pMh = (MathAdd*) new MathAdd_Helper;
	r = Add(pMh, 1, 2);
	return 0;
}
```







