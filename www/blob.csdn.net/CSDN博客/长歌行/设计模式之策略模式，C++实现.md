# 设计模式之策略模式，C++实现 - 长歌行 - CSDN博客





2012年11月29日 09:35:58[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：1098
所属专栏：[初探设计模式（C++实现）](https://blog.csdn.net/column/details/baadf00df00dabadfds0.html)









原创： [http://blog.csdn.net/iuhsihsow](http://blog.csdn.net/iuhsihsow)




//策略模式

 //抽象出算法

 //可适应算放的频繁改动


 //问1：和简单工厂有什么区别？


//答1：策略模式主要是将算法提炼出来，一个大的context中可以包括多个算法，便于context中算法的的自由配置，

 //一次配置，多次使用，维持context的稳定性。

 //简单类工厂模式主要是用于创建一个完整的对象，级别和策略模式不同。

 //策略模式中可以包括一个工厂模式的抽象类的对象。





自己画的图，囧，将就吧

![](https://img-my.csdn.net/uploads/201211/29/1354153225_9921.png)







```cpp
#include "stdafx.h"
 
 class CashSuper
 {
 public:
 	virtual double CalcCash() = 0;
 	void SetOrginCash(double dOrgCash)
 	{
 		m_dOrgCash = dOrgCash;
 	}
 protected:
 	double m_dOrgCash;
 };
 
 class CashReBate : public CashSuper
 {
 public:
 	double CalcCash()
 	{
 		return m_dOrgCash * 0.85;
 	}
 };
 
 class CashReturn : public CashSuper
 {
 public:
 	double CalcCash()
 	{
 		if (m_dOrgCash > 300)
 		{
 			return m_dOrgCash - 100;
 		}
 		else
 		{
 			return m_dOrgCash;
 		}
 	}
 };
 
 class CashContext
 {
 public:
 	double GetResult(double dMoney)
 	{
 		m_pCashSuper->SetOrginCash(dMoney);
 		return m_pCashSuper->CalcCash();
 	}
 	void SetCashSuper(CashSuper *pCashSuper)
 	{
 		m_pCashSuper = pCashSuper;
 	}
 	/*void ReleaseCashSuper()
 	{
 		if (m_pCashSuper)
 		{
 			delete m_pCashSuper;
 			m_pCashSuper = NULL;
 		}
 	}*/
 protected:
 	CashSuper *m_pCashSuper;
 
 };
 
 int _tmain(int argc, _TCHAR* argv[])
 {
 	//现在商场进入打折季节，都使用打折suanfa
 	CashSuper *pCSuper = new CashReBate;
 	CashContext CContext;
 	CContext.SetCashSuper(pCSuper);
 	//CContext.SetCashSuper(new CashReturn)
 	//CContext.ReleaseCashSuper();
 	double dPay = CContext.GetResult(100.0f);
 
 	if (pCSuper)
 	{
 		delete pCSuper;
 		pCSuper = NULL;
 	}
 	return 0;
 }
```





