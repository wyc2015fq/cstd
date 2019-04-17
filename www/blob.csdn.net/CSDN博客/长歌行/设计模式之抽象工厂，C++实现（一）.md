# 设计模式之抽象工厂，C++实现（一） - 长歌行 - CSDN博客





2013年01月05日 22:59:20[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：1081
个人分类：[设计架构																[C、C++](https://blog.csdn.net/iuhsihsow/article/category/1072760)](https://blog.csdn.net/iuhsihsow/article/category/1076810)

所属专栏：[初探设计模式（C++实现）](https://blog.csdn.net/column/details/baadf00df00dabadfds0.html)








本部分就说明最简单的抽象工厂实现，当然在实际项目应用中不会这么做，仅为解释该模式。




![](https://img-my.csdn.net/uploads/201301/05/1357398105_6033.PNG)


上图的抽象 工厂有两个工厂，分别生产对应的ProductA和ProductB（可以假想为蓝色或者红色的两种产品A22 = 4）




下面为简单代码示例



```cpp
// 抽象工厂模式（Abstract Factory），提供一个创建
 // 一系列相关或相互依赖对象的接口，而无需指定他们具体的类
 
 // 情景假设
 // 假设一个软件最终保存的工程文件有两个版本
 // 要求能够解析这两个版本的内容
 // 要求能够修改者两个版本的内容
 
 // 优点在与只需要修改具体的工厂就能实习不同的类的操作
 // 而且客户也无需知道版本对象的具体名称，只需要采用正确的
 // 工厂来生成即可，配置方便。
 // 当然，个人觉得工厂类也可以实现上面内容，增加Switch Case
 
 #include "stdafx.h"
 
 // 解释器基类
 class BaseParse
 {
 public:
 	virtual void Show() = 0;
 protected:
 private:
 };
 
 // 版本一解释器
 class VesionOneParse : BaseParse
 {
 public:
 	void Show()
 	{
 		printf("Now, Use VesionOneParse.\n");
 	}
 protected:
 private:
 };
 
 // 版本二解释器
 class VesionTwoParse : BaseParse
 {
 public:
 	void Show()
 	{
 		printf("Now, Use VesionTwoParse.\n");
 	}
 protected:
 private:
 };
 
 // 抽象的版本处理工厂
 class AbstractVesionFactory
 {
 public:
 	virtual BaseParse* CreateParse() = 0;
 protected:
 private:
 };
 
 // 对应版本一的工程文件的处理工厂
 class VesionOneFactory : public AbstractVesionFactory
 {
 public:
 	BaseParse * CreateParse()
 	{
 		return (BaseParse*)new VesionOneParse;
 	}
 protected:
 private:
 };
 
 // 对应版本二的工程文件的处理工厂
 class VesionTwoFactory : public AbstractVesionFactory
 {
 public:
 	BaseParse * CreateParse()
 	{
 		return (BaseParse*)new VesionTwoParse;
 	}
 protected:
 private:
 };
 
 
 
 
 
 
 int _tmain(int argc, _TCHAR* argv[])
 {
 	AbstractVesionFactory *pFactory
 		= new VesionOneFactory;			
 	// 以后要换为版本二的解释器的话，就修改为 = new VesionTwoFactory;
 	// 只需要修改一个地方即可
 	// 以后要增加一个对应版本的修改器VesionModify的话
 	// 就在各个工厂写对应的CreateModify
 	// 当然，这个可以用简单工厂实现
 	BaseParse *pParse = pFactory->CreateParse();
 	pParse->Show();
 
 	if (pFactory)
 	{
 		delete pFactory;
 		pFactory = NULL;
 	}
 	if (pParse)
 	{
 		delete pParse;
 		pParse = NULL;
 	}
 	return 0;
 }
```











