# 设计模式之建造者模式，C++实现 - 长歌行 - CSDN博客





2012年12月13日 10:04:15[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：1073
个人分类：[设计架构](https://blog.csdn.net/iuhsihsow/article/category/1076810)

所属专栏：[初探设计模式（C++实现）](https://blog.csdn.net/column/details/baadf00df00dabadfds0.html)









原文地址： [http://blog.csdn.net/iuhsihsow](http://blog.csdn.net/iuhsihsow)




其实一直在文中注明C++实现确实没有太大意义，无非是C++用指针来实现多态而非对象本身。

今天事情比较多，就偷下懒吧。




抄的《大话设计模式》的图

![](https://img-my.csdn.net/uploads/201212/13/1355364513_5005.png)





下面是实现代码，恩，基本上也是抄的。照着敲了一遍。![大笑](http://static.blog.csdn.net/xheditor/xheditor_emot/default/laugh.gif)



```cpp
// 建造者模式（Builder），将一个复杂对象的构造与它的表示分离，
 // 使得同样的构建过程可以创建不同的表示
 
 
 // 大概就是把一个整体结构的几个可能变化的部分拆为独立的函数
 // 然后再派生类重载实现的方式
 
 // 主要应对与整体中变化的部件，
 // 而不是流程状态，要不然可以用状态模式，
 // 请求处理的流程可以用职责链模式
 
 // 这次状态不好，就偷懒，直接用《大话设计模式》Java代码，翻译为C++了
 
 
 // 情景，画人物
 // 人物包括 头 手 脚 肚子
 // 人物有两种 胖子和瘦子
 // 胖瘦的区别主要在于肚子不同。
 
 #include "stdafx.h"
 #include <Windows.h>
 #include <iostream>
 
 using namespace std;
 
 
 class PersonBuilder
 {
 public:
 	PersonBuilder(const string & strGrap, const string & strPen)
 	{
 		m_strGraphics = strGrap;
 		m_strPen = strPen;
 	}
 	virtual ~PersonBuilder()
 	{
 
 	}
 	virtual void BuildHead() = 0;
 	virtual void BuildBody() = 0;
 	virtual void BuildLeg() = 0;
 
 protected:
 	string	m_strGraphics;
 	string  m_strPen;
 
 private:
 };
 
 class PersonThinBuilder : public PersonBuilder
 {
 public:
 	PersonThinBuilder(const string & strGrap, const string & strPen)
 		: PersonBuilder(strGrap, strPen)
 	{
 
 	}
 	 void BuildHead()
 	 {
 		 cout<<"With "<<m_strPen.c_str()<<endl;
 		 printf("Draw normal head\n");
 		 cout<<"On"<<m_strGraphics.c_str()<<endl;
 
 	 }
 	 void BuildBody()
 	 {
 		 cout<<"With "<<m_strPen.c_str()<<endl;
 		 printf("Draw thin body\n");
 		 cout<<"On"<<m_strGraphics.c_str()<<endl;
 
 	 }
 	 void BuildLeg()
 	 {
 		 cout<<"With "<<m_strPen.c_str()<<endl;
 		 printf("Draw normal leg\n");
 		 cout<<"On"<<m_strGraphics.c_str()<<endl;
 
 	 }
 
 protected:
 private:
 };
 
 
 class PersonFatBuilder : public PersonBuilder
 {
 public:
 	PersonFatBuilder(const string & strGrap, const string & strPen)
 		: PersonBuilder(strGrap, strPen)
 	{
 
 	}
 	void BuildHead()
 	{
 		cout<<"With "<<m_strPen.c_str()<<endl;
 		printf("Draw normal head\n");
 		cout<<"On"<<m_strGraphics.c_str()<<endl;
 	}
 	void BuildBody()
 	{
 		cout<<"With "<<m_strPen.c_str()<<endl;
 		printf("Draw Fat body\n");
 		cout<<"On"<<m_strGraphics.c_str()<<endl;
 	}
 	void BuildLeg()
 	{
 		cout<<"With "<<m_strPen.c_str()<<endl;
 		printf("Draw normal leg\n");
 		cout<<"On"<<m_strGraphics.c_str()<<endl;
 	}
 
 protected:
 private:
 };
 
 
 class PersonDirector
 {
 public:
 
 	void SetPersonBuilder(PersonBuilder * pb)
 	{
 		m_pPbuilder = pb;
 	}
 
 	void CreatePerson()
 	{
 		m_pPbuilder->BuildHead();
 		m_pPbuilder->BuildBody();
 		m_pPbuilder->BuildLeg();
 	}
 protected:
 private:
 	PersonBuilder * m_pPbuilder;
 
 };
 
 int _tmain(int argc, _TCHAR* argv[])
 {
 	PersonBuilder *pThinBuilder = new PersonThinBuilder("Screen", "YellowPen");
 	PersonBuilder *pFatBuilder  = new PersonFatBuilder("Screen", "WhitePen");
 	PersonDirector pd;
 	pd.SetPersonBuilder(pThinBuilder);
 	pd.CreatePerson();
 	pd.SetPersonBuilder(pFatBuilder);
 	pd.CreatePerson();
 
 	if (pThinBuilder)
 	{
 		delete pThinBuilder;
 		pThinBuilder = NULL;
 	}
 	if (pFatBuilder)
 	{
 		delete pFatBuilder;
 		pFatBuilder = NULL;
 	}
 	return 0;
 }
 
 
 //输出结果
 //With YellowPen
 //Draw normal head
 //OnScreen
 //With YellowPen
 //Draw thin body
 //OnScreen
 //With YellowPen
 //Draw normal leg
 //OnScreen
 //With WhitePen
 //Draw normal head
 //OnScreen
 //With WhitePen
 //Draw Fat body
 //OnScreen
 //With WhitePen
 //Draw normal leg
 //OnScreen
 //请按任意键继续. . .
```








