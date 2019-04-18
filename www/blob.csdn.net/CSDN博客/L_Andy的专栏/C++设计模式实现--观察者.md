# C++设计模式实现--观察者 - L_Andy的专栏 - CSDN博客

2014年06月11日 14:42:37[卡哥](https://me.csdn.net/L_Andy)阅读数：860


概述

　　在软件构建过程中，我们需要为某些对象建立一种“通知依赖关系” ——一个对象（目标对象）的状态发生改变，所有的依赖对象（观察者对象）都将得到通知。如果这样的依赖关系过于紧密，将使软件不能很好地抵御变化。使用面向对象技术，可以将这种依赖关系弱化，并形成一种稳定的依赖关系。从而实现软件体系结构的松耦合。
　    意图

　　定义对象间的一种一对多的依赖关系,当一个对象的状态发生改变时, 所有依赖于它的对象都得到通知并被自动更新。[GOF 《设计模式》]

** 特点：**

        1、  Subject和Observer之间是松偶合的，分别可以各自独立改变。

        2、  Subject在发送广播通知的时候，无须指定具体的Observer，Observer可以自己决定是否要订阅Subject的通知。

        3、  遵守大部分GRASP原则和常用设计原则，高内聚、低偶合。
**应用场景：**

        1、  对一个对象状态的更新，需要其他对象同步更新，而且其他对象的数量动态可变。

        2、  对象仅需要将自己的更新通知给其他对象而不需要知道其他对象的细节。

**        UML图**

**![](https://img-blog.csdn.net/20130506103630939)**

**          代码实现**

**[cpp]**[view
 plain](http://blog.csdn.net/cabinriver/article/details/8889387#)[copy](http://blog.csdn.net/cabinriver/article/details/8889387#)

- #pragma once
- #include <string>
- #include <list>
- #include "Define.h"
- 
- class IObserver;  
- class ISubject  
- {  
- public:  
-     ISubject(void);  
- virtual ~ISubject(void);  
- 
- virtualint Attach(IObserver* pObs) = 0;  
- 
- virtualint Detach(IObserver* pObs) = 0;  
- 
- virtualint Notify() = 0;  
- 
- virtual State GetState() = 0;  
- 
- virtualvoid SetState(const State& state) = 0;  
- 
- protected:  
- typedef std::list<IObserver*> L_OBS;  
-     L_OBS   m_obsArray;  
- };  

**[cpp]**[view
 plain](http://blog.csdn.net/cabinriver/article/details/8889387#)[copy](http://blog.csdn.net/cabinriver/article/details/8889387#)

- #pragma once
- #include "ISubject.h"
- 
- class CSubjectA : public ISubject  
- {  
- public:  
-     CSubjectA(void);  
- virtual ~CSubjectA(void);  
- 
- virtualint Attach(IObserver* pObs);  
- 
- virtualint Detach(IObserver* pObs);  
- 
- virtualint Notify();  
- 
- virtual State GetState();  
- 
- virtualvoid SetState(const State& state);  
- 
- private:  
- 
-     State m_state;  
- };  

**[cpp]**[view
 plain](http://blog.csdn.net/cabinriver/article/details/8889387#)[copy](http://blog.csdn.net/cabinriver/article/details/8889387#)

- #include "CSubjectA.h"
- #include "IObserver.h"
- 
- CSubjectA::CSubjectA(void)  
- {  
- }  
- 
- CSubjectA::~CSubjectA(void)  
- {  
- if (!m_obsArray.empty())  
-     {  
-         m_obsArray.clear();  
-         L_OBS().swap(m_obsArray);  
-     }  
- }  
- 
- int CSubjectA::Attach( IObserver* pObs )  
- {  
-     m_obsArray.push_back(pObs);  
- 
- return 0;  
- }  
- 
- int CSubjectA::Detach( IObserver* pObs )  
- {  
-     m_obsArray.remove(pObs);  
- 
- return 0;  
- }  
- 
- int CSubjectA::Notify()  
- {  
-     L_OBS::iterator it_beg, it_end=m_obsArray.end();  
- for (it_beg=m_obsArray.begin(); it_beg!=it_end; ++it_beg)  
-     {  
-         (*it_beg)->Update();  
-     }  
- 
- return 0;  
- }  
- 
- State CSubjectA::GetState()  
- {  
- return m_state;  
- }  
- 
- void CSubjectA::SetState( const State& state )  
- {  
-     m_state = state;  
- }  

**[cpp]**[view
 plain](http://blog.csdn.net/cabinriver/article/details/8889387#)[copy](http://blog.csdn.net/cabinriver/article/details/8889387#)

- #pragma once
- #include "Define.h"
- 
- class IObserver  
- {  
- public:  
-     IObserver(void);  
- virtual ~IObserver(void);  
- 
- virtualint Update() = 0;  
- };  

**[cpp]**[view
 plain](http://blog.csdn.net/cabinriver/article/details/8889387#)[copy](http://blog.csdn.net/cabinriver/article/details/8889387#)

- #pragma once
- #include "IObserver.h"
- 
- class ISubject;  
- class CObserveA : public IObserver  
- {  
- public:  
-     CObserveA(ISubject* pSubject);  
- virtual ~CObserveA(void);  
- 
- virtualint Update();  
- 
- private:  
-     ISubject* m_pSubject;  
- };  

**[cpp]**[view
 plain](http://blog.csdn.net/cabinriver/article/details/8889387#)[copy](http://blog.csdn.net/cabinriver/article/details/8889387#)

- #include "CObserveA.h"
- #include <cstdio>
- #include "ISubject.h"
- 
- CObserveA::CObserveA(ISubject* pSubject)  
-     :m_pSubject(pSubject)  
- {  
-     m_pSubject->Attach(this);  
- }  
- 
- CObserveA::~CObserveA(void)  
- {  
-     m_pSubject->Detach(this);  
- }  
- 
- int CObserveA::Update()  
- {  
-     State str = m_pSubject->GetState();  
- 
-     printf("first get: [%s]\n", str.c_str());  
- return 0;  
- }  

**[cpp]**[view
 plain](http://blog.csdn.net/cabinriver/article/details/8889387#)[copy](http://blog.csdn.net/cabinriver/article/details/8889387#)

- #include "CSubjectA.h"
- #include "CObserveA.h"
- 
- int main()  
- {  
-     CSubjectA subA;  
-     CObserveA first(&subA);  
- 
-     subA.SetState("test set state");  
- 
-     subA.Notify();  
- 
- return 0;  
- }  

