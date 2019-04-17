# 设计模式之状态模式，C++实现 - 长歌行 - CSDN博客





2013年01月31日 15:55:05[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：775
个人分类：[C、C++](https://blog.csdn.net/iuhsihsow/article/category/1072760)

所属专栏：[初探设计模式（C++实现）](https://blog.csdn.net/column/details/baadf00df00dabadfds0.html)









![](https://img-my.csdn.net/uploads/201301/31/1359619167_5831.png)




代码：

这次代码质量不佳啊，估计是应用场景选择错误了。






```cpp
// StateMode.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>

#ifdef	_DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new   new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif


inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}
// 状态模式（State）， 当一个对象的内在状态改变时允许改变其行为，这个对象看起来像是改变了其类



// 惨不忍睹的程序，几乎没有产生的必要，算作是记录吧
// 有可能是应用场景错了吧，貌似没有什么用处

// 根据作业流程的顺序，连环创建指针，没有太长的分支判断语句
// 不过如果作业流程顺序不确定，或者有偏差，会有重复的new
// 如果流程顺序有跳跃，会顺序转到该顺序执行
// 而且如此频繁的new和delete对于实时渲染来说，确实不好




// 假设一个数据从加载到显示有这么些状态
enum E_State
{
	WAITING_LOAD,		// 等待加载
	LOADED,				// 加载完成
	SHOWING,			// 正在显示
	SHOWED,				// 显示完成
	ERROR				// 在某一步中发生错误了
};

class Context;

// 各种状态的基类，实现了各种操作
class State
{
public:
	virtual ~State()
	{
		;
	}
	virtual void Handle(Context *pCt) = 0;
};

// 待渲染的模型
class Model
{
public:
	std::string  m_strDescription;
protected:
private:
};
class LoadData;
class BuildDataRenderObject;
class ShowData;


// Context作为上下文，包括了状态子类的指针，并存储了当前状态,以及物体的信息
class Context
{
public:
	Context();
	~Context();

	void SetState(State *pSt)
	{
		m_pSt = pSt;
	}
	void Request();

	void ReleaseStatePtr();
	State		*m_pSt;
	E_State		m_eState;
	Model		m_Model;
};



// 渲染体创建完之后会显示数据
class ShowData : public State
{
public:
	void Handle(Context *pCt)
	{
		if (SHOWING == pCt->m_eState)
		{
			printf("Start a new thread to Show data!\n");
		}
		else
		{
			pCt->ReleaseStatePtr();
			printf("A frame End!\n");
		}
	}
};


// 加载完数据后会创建数据的渲染体
class BuildDataRenderObject : public State
{
public:
	void Handle(Context *pCt)
	{
		if (LOADED == pCt->m_eState)
		{
			printf("Start a new thread to Build RenderObject of item!\n");
		}
		else
		{	
			pCt->ReleaseStatePtr();
			pCt->SetState(new ShowData);
			pCt->Request();
		}
	}
};

// 程序开始的时候会加载数据
class LoadData : public State
{
public:
	void Handle(Context *pCt)
	{
		if (WAITING_LOAD == pCt->m_eState)
		{
			printf("Start a new thread to Parse file and loading data!\n");
		}
		else
		{
			pCt->ReleaseStatePtr();
			pCt->SetState(new BuildDataRenderObject);
			pCt->Request();

		}
	}
};


Context::Context()
{
	m_pSt = new LoadData;
	m_Model.m_strDescription = "A New Model ,need to loaded";
}

Context::~Context()
{
	ReleaseStatePtr();
}

void Context::ReleaseStatePtr()
{
	if (m_pSt)
	{
		delete m_pSt;
		m_pSt = NULL;
	}
}

void Context::Request()
{
	if (!m_pSt)
	{
		m_pSt = new LoadData;
	}
	m_pSt->Handle(this);
}


int _tmain(int argc, _TCHAR* argv[])
{
	EnableMemLeakCheck();
	Context *pCtxt = new Context();

	// 不同状态（流程顺序）的数据进行处理

	printf("\nstate is waiting load:\n");
	pCtxt->m_eState = WAITING_LOAD;
	pCtxt->m_Model.m_strDescription = "Loaded";
	pCtxt->Request();

	printf("\nstate is loaded:\n");
	pCtxt->m_eState = SHOWED;
	pCtxt->m_Model.m_strDescription = "...";
	pCtxt->Request();

	if (pCtxt)
	{
		delete pCtxt;
		pCtxt = NULL;
	}

	return 0;
}


// 输出结果：

//state is waiting load:
//Start a new thread to Parse file and loading data!
//
//state is loaded:
//A frame End!
//请按任意键继续. . .
```














