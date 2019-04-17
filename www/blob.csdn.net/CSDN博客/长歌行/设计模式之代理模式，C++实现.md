# 设计模式之代理模式，C++实现 - 长歌行 - CSDN博客





2012年11月29日 09:32:45[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：1667
个人分类：[C、C++																[设计架构](https://blog.csdn.net/iuhsihsow/article/category/1076810)](https://blog.csdn.net/iuhsihsow/article/category/1072760)

所属专栏：[初探设计模式（C++实现）](https://blog.csdn.net/column/details/baadf00df00dabadfds0.html)








原创：[http://blog.csdn.net/iuhsihsow](http://blog.csdn.net/iuhsihsow)




这一篇主要参照了《大话设计模式》







//代理模式：为其他对象提供一种代理以控制这个对象的访问

 //简而言之就是给对象加一个壳


 //1.远程代理

 //2.虚拟代理

 //3.安全代理

 //4.智能指引


 //问1：代理模式和聚合关系有什么区别，感觉适配器模式也能解决类似问题？

 //答1：强调了对象的行为





截下来的图，自己用visio画的太丑了

![](https://img-my.csdn.net/uploads/201211/29/1354153033_4099.png)







```cpp
#include "stdafx.h"
 #include <windows.h>
 #include <iostream>
 using namespace std;
 
 //大话设计模式中的例子
 //A君让代理人B君给C小姐送礼物
 
 class IGiveGift
 {
 public:
 			IGiveGift(){;}
 	virtual ~IGiveGift(){;}
 
 	virtual void GiveGift() = 0; 
 };
 
 class PersonA : public IGiveGift
 {
 public:
 	PersonA(const std::string & strGirlName)
 	{
 		m_strGirlName = strGirlName;
 	}
 	void GiveGift()
 	{
 		std::cout<<m_strGirlName.c_str()<<","<<"送你的礼物！"<<endl;
 	}
 private:
 	std::string m_strGirlName;
 
 };
 
 class ProxyB : public IGiveGift
 {
 public:
 	void SetClientPerson(PersonA *pA)
 	{
 		m_pPersonA = pA;
 	}
 	void GiveGift()
 	{
 		m_pPersonA->GiveGift();
 	}
 private:
 	PersonA *m_pPersonA;
 };
 
 int _tmain(int argc, _TCHAR* argv[])
 {
 	//A君出现了，怀着对心上人的思念
 	PersonA *pA = new PersonA("C小姐");
 	//A君不好意思送礼物，于是找到了代理B君
 	ProxyB *pB = new ProxyB();
 	pB->SetClientPerson(pA);
 	//B君送礼来了
 	pB->GiveGift();
 
 	//落幕
 	if (pA)
 	{
 		delete pA;
 		pA = NULL;
 	}
 	if (pB)
 	{
 		delete pB;
 		pB = NULL;
 	}
 	return 0;
 }
```











