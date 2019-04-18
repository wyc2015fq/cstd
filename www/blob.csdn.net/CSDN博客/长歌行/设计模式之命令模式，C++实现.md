# 设计模式之命令模式，C++实现 - 长歌行 - CSDN博客





2013年01月31日 23:46:25[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：1808
个人分类：[设计架构](https://blog.csdn.net/iuhsihsow/article/category/1076810)

所属专栏：[初探设计模式（C++实现）](https://blog.csdn.net/column/details/baadf00df00dabadfds0.html)












命令模式应该是应用比较广泛的模式

各种编辑操作都会用到，可以将实现与操作分离，

解析用户操作为一部分代码，实现用户操作的目的又封装到另外一个地方，或者是命令模式的派生类或者是对象自身内部




《大话设计模式》中的贴图

![](https://img-my.csdn.net/uploads/201301/31/1359647475_6259.png)





代码：



```cpp
// Command.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <vector>

// 命令模式（Command）：将一个请求封装为一个对象，从而使你可以用不同的请求对客户
// 进行参数化，对请求排队或记录请求日志，以及支持可撤销的操作。

// 就是将命令的产生于命令的执行分离，这样就能生成一个命令队列

class Receiver
{
public:
	Receiver(const std::string &strName)
	{
		m_strName = strName;
	}
	std::string m_strName;
};
class Cmd
{
public:
	Cmd(Receiver *pRcv)
		: m_pReceiver(pRcv)
	{;}
	virtual ~Cmd(){;}

	virtual void Execute() = 0;
protected:
	Receiver	*m_pReceiver;
};

class MoveCmd : public Cmd
{
public:
	MoveCmd(Receiver *pRcv)
		: Cmd(pRcv)
	{

	}
	void Execute()
	{
		printf("Move %s\n", m_pReceiver->m_strName.c_str());
	}
};

class RotateCmd : public Cmd
{
public:
	RotateCmd(Receiver *pRcv)
		: Cmd(pRcv)
	{

	}

	void Execute()
	{
		printf("Rotate %s\n", m_pReceiver->m_strName.c_str());
	}
};

class CmdMgr
{
public:
	void SetCommand(Cmd *pCmd)
	{
		m_VecCmd.push_back(pCmd);
	}
	void ExecuteCommand()
	{
		std::vector<Cmd*>::iterator iter = m_VecCmd.begin();
		for (; iter != m_VecCmd.end();)
		{
			Cmd* pCmd = (*iter);
			pCmd->Execute();
			if (pCmd)
			{
				delete pCmd;
				pCmd = NULL;
			}
			iter = m_VecCmd.erase(iter);
		}
	}
private:
	std::vector<Cmd*>  m_VecCmd;
};

int _tmain(int argc, _TCHAR* argv[])
{
	Receiver	recvr("Cube");
	CmdMgr		mgr;

	Cmd* pMoveCmd = new MoveCmd(&recvr);
	Cmd* pRotateCmd = new RotateCmd(&recvr);

	mgr.SetCommand(pMoveCmd);
	mgr.SetCommand(pRotateCmd);

	mgr.ExecuteCommand();


	return 0;
}
```





