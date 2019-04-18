# 设计模式之组合模式（Composite Pattern） - 江南烟雨 - CSDN博客
2012年08月27日 20:17:21[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：1581
**作用:**
将对象组合成树形结构以表示“部分-整体”的层次结构。Composite使得用户对单个对象和组合对象的使用具有一致性。
**UML结构图:**
![](https://img-my.csdn.net/uploads/201208/27/1346070355_3645.jpg)
抽象基类:
1)Component:为组合中的对象声明接口,声明了类共有接口的缺省行为(如这里的Add,Remove,GetChild函数),声明一个接口函数可以访问Component的子组件.
接口函数:
1)Component::Operatation:定义了各个组件共有的行为接口,由各个组件的具体实现.
2)Component::Add添加一个子组件
3)Component::Remove::删除一个子组件.
4)Component::GetChild:获得子组件的指针.
解析:
Component模式是为解决组件之间的递归组合提供了解决的办法,它主要分为两个派生类,其中的Leaf是叶子结点,也就是不含有子组件的结点,而Composite是含有子组件的类.举一个例子来说明这个模式,在UI的设计中,最基本的控件是诸如Button,Edit这样的控件,相当于是这里的Leaf组件,而比较复杂的控件比如List则可也看做是由这些基本的组件组合起来的控件,相当于这里的Composite,它们之间有一些行为含义是相同的,比如在控件上作一个点击,移动操作等等的,这些都可以定义为抽象基类中的接口虚函数,由各个派生类去实现之,这些都会有的行为就是这里的Operation函数,而添加,删除等进行组件组合的操作只有非叶子结点才可能有,所以虚拟基类中只是提供接口而且默认的实现是什么都不做.
实例：
一个新闻的树形菜单，它包含很多原始菜单（如国内、国际），以及由其子菜单组成的组合节点（如国内新闻下的时事、社会等），结构图如下：
![](https://img-my.csdn.net/uploads/201208/27/1346070449_1732.jpg)
代码：
Menu.h
```cpp
//Menu.h
#pragma once
#include <iostream>
#include <string>
using namespace std;
class Menu  
{
public:
	Menu();
	Menu(std::string);
	virtual ~Menu();
	virtual void Add(Menu*);
	virtual void Remove(Menu*);
	virtual Menu* GetChild(int);
	virtual void Display() = 0;
	std::string getName()
	{
		return m_strName;
	}
protected:
private:
	std::string m_strName;
};
```
Menu.cpp
```cpp
#include "Menu.h"
Menu::Menu()
{
}
Menu::Menu(std::string strName) : m_strName(strName)
{
}
Menu::~Menu()
{
}
void Menu::Add(Menu* pMenu)
{}
void Menu::Remove(Menu* pMenu)
{}
Menu* Menu::GetChild(int index)
{
	return NULL;
}
```
SubMenu.h:
```cpp
#include "Menu.h"
class SubMenu : public Menu  
{
public:
	SubMenu();
	SubMenu(std::string);
	virtual ~SubMenu();
	void Display();
};
```
SubMenu.cpp:
```cpp
#include "SubMenu.h"
#include <iostream>
using namespace std;
SubMenu::SubMenu()
{
}
SubMenu::SubMenu(string strName) : Menu(strName)
{
}
SubMenu::~SubMenu()
{
}
void SubMenu::Display()
{
	cout << getName() << endl;
}
```
CompositeMenu.h:
```cpp
#include "Menu.h"
#include <vector>
class CompositMenu : public Menu
{
public:
	CompositMenu();
	CompositMenu(std::string);
	virtual ~CompositMenu();
	void Add(Menu*);
	void Remove(Menu*);
	Menu* GetChild(int);
	void Display();
private:
	//注意：vector里面放入指针的话，内存需要自己释放，而如果是对象或者
	//对象引用，vector会调用其析构函数
	std::vector<Menu*> m_vMenu;
};
```
CompositeMenu.cpp:
```cpp
#include "CompositeMenu.h"
CompositMenu::CompositMenu()
{
}
CompositMenu::CompositMenu(string strName) : Menu(strName)
{
}
CompositMenu::~CompositMenu()
{
	//释放指针所指的内存
	std::vector<Menu*>::iterator pos = m_vMenu.begin();
	while(pos != m_vMenu.end())
	{
		delete (*pos);
	}
}
void CompositMenu::Add(Menu* pMenu)
{
	m_vMenu.push_back(pMenu);
}
void CompositMenu::Remove(Menu* pMenu)
{
	std::vector<Menu*>::iterator pos = m_vMenu.begin();
	while(pos != m_vMenu.end())
	{
		if ((*pos)->getName() == pMenu->getName())
		{
			m_vMenu.erase(pos);
		}
	}
}
Menu* CompositMenu::GetChild(int index)
{
	return m_vMenu[index];
}
void CompositMenu::Display()
{
	cout << "+" << getName() << endl;
	vector<Menu*>::iterator it = m_vMenu.begin();
	for (; it != m_vMenu.end(); ++it)
	{
		cout << "    |-";
		(*it)->Display();
	}
}
```
Main.cpp:
```cpp
#include "Menu.h"
#include "SubMenu.h"
#include "CompositeMenu.h"
int main(int argc, char* argv[])
{
	Menu* pMenu = new CompositMenu("国内新闻");
	pMenu->Add(new SubMenu("时事新闻"));
	pMenu->Add(new SubMenu("社会新闻"));
	pMenu->Display();
	pMenu = new CompositMenu("国际新闻");
	pMenu->Add(new SubMenu("国际要闻"));
	pMenu->Add(new SubMenu("环球视野"));
	pMenu->Display();
	return 0;
}
```
运行结果：
![](https://img-my.csdn.net/uploads/201208/27/1346070601_2216.jpg)
