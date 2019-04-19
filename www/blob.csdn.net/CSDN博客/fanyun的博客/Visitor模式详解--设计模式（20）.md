# Visitor模式详解--设计模式（20） - fanyun的博客 - CSDN博客
2016年07月07日 08:40:36[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：1843
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
**Visitor模式来源：**
  在面向对象系统的开发和设计过程，经常会遇到一种情况就是需求变更（RequirementChanging），经常我们做好的一个设计、实现了一个系统原型，咱们的客户又会有了新的需求。我们又因此不得不去修改已有的设计，最常见就是解决方案就是给已经设计、实现好的类添加新的方法去实现客户新的需求，这样就陷入了设计变更的梦魇：不停地打补丁，其带来的后果就是设计根本就不可能封闭、编译永远都是整个系统代码。Visitor模式则提供了一种解决方案。
**Visitor****模式****作用****：**
  将更新（变更）封装到一个类中（访问操作），并由待更改类提供一个接收接口，则可达到效果。
**Visitor模式UML结构图如图1所示：**
**![](https://img-blog.csdn.net/20160706181252780?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**
Visitor模式构成：
抽象访问者(Visitor)：抽象出访问元素的动作
具体访问者(ConcreteVisitor)：实现访问元素的动作
抽象元素(Element)：定义一个接受访问的操作，其参数为访问者
具体元素(ConcreteElement)：实现接受访问操作
对象结构类(ObjectStructure)：可以枚举元素，并且管理元素
客户端(Client) ：定义元素集合，然后接受不同访问者的访问
Visitor模式源码示例：
Visitor.h
```cpp
#pragma once
class CCommonEmployee;
class CManager;
class IVisitor
{
public:
    IVisitor(void)
    {
    }
    virtual ~IVisitor(void)
    {
    }
    virtual void Visit(CCommonEmployee commonEmployee) = 0;
    virtual void Visit(CManager manager) = 0;
    virtual int GetTotalSalary() = 0;
};
//BaseVisitor.h
#pragma once
#include "ivisitor.h"
#include "CommonEmployee.h"
#include "Manager.h"
#include <iostream>
using std::string;
class CBaseVisitor :
    public IVisitor
{
public:
    CBaseVisitor(void);
    ~CBaseVisitor(void);
    void Visit(CCommonEmployee commonEmployee);
    void Visit(CManager manager);
    int GetTotalSalary();
private:
    string GetBasicInfo(CEmployee *pemployee);
    string GetManagerInfo(CManager manager);
    string GetCommonEmployee(CCommonEmployee employee);
    static const int MANAGER_COEFFICENT = 5;
    static const int COMMONEMPLOYEE_COEFFICENT = 2;
    int m_commonTotal;
    int m_managerTotal;
    void CalCommonSalary(int salary);
    void CalManagerSalary(int salary);
};
//BaseVisitor.cpp
#include "StdAfx.h"
#include "..\CommonDeclare\Convert.h"
#include "BaseVisitor.h"
#include <iostream>
using std::string;
using std::cout;
using std::endl;
CBaseVisitor::CBaseVisitor(void)
{
    m_commonTotal = 0;
    m_managerTotal = 0;
}
CBaseVisitor::~CBaseVisitor(void)
{
}
void CBaseVisitor::Visit(CCommonEmployee commonEmployee)
{
    cout << this->GetCommonEmployee(commonEmployee).c_str() << endl;
    this->CalCommonSalary(commonEmployee.GetSalary());
}
void CBaseVisitor::Visit(CManager manager)
{
    cout << this->GetManagerInfo(manager).c_str() << endl;
    this->CalManagerSalary(manager.GetSalary());
}
string CBaseVisitor::GetBasicInfo(CEmployee *pemployee)
{
    string info = "";
    info.append("姓名：");
    info.append(pemployee->GetName());
    info.append("\t");
    info.append("性别：");
    info.append(CConvert::ToString(pemployee->GetSex()));
    info.append("\t");
    info.append("薪水：");
    info.append(CConvert::ToString(pemployee->GetSalary()));
    info.append("\t");
    return info;
}
string CBaseVisitor::GetManagerInfo(CManager manager)
{
    string basicInfo = this->GetBasicInfo(&manager);
    string otherInfo = "";
    otherInfo.append("业绩：");
    otherInfo.append(manager.GetPerformance());
    otherInfo.append("\t");
    basicInfo.append(otherInfo);
    return basicInfo;
}
string CBaseVisitor::GetCommonEmployee(CCommonEmployee employee)
{
    string basicInfo = this->GetBasicInfo(&employee);
    string otherInfo = "";
    otherInfo.append("工作：");
    otherInfo.append(employee.GetJob());
    otherInfo.append("\t");
    basicInfo.append(otherInfo);
    return basicInfo;
}
int CBaseVisitor::GetTotalSalary()
{
    return this->m_commonTotal + this->m_managerTotal;
}
void CBaseVisitor::CalCommonSalary(int salary)
{
    this->m_commonTotal += salary;
}
void CBaseVisitor::CalManagerSalary(int salary)
{
    this->m_managerTotal += salary;
}
```
```cpp
//Employee.h
#pragma once
#include "Visitor.h"
#include <iostream>
using std::string;
class CEmployee
{
public:
static int MALE;
static int FEMALE;
CEmployee(void);
virtual ~CEmployee(void);
string GetName();
void SetName(string name);
int GetSalary();
void SetSalary(int v);
int GetSex();
void SetSex(int v);
virtual void Accept(IVisitor *pVisitor) = 0;
private:
string m_name;
int m_salary;
int m_sex;
};
//Employee.cpp
#include "StdAfx.h"
#include "Employee.h"
int CEmployee::MALE = 0;
int CEmployee::FEMALE = 1;
CEmployee::CEmployee(void)
{
}
CEmployee::~CEmployee(void)
{
}
string CEmployee::GetName()
{
return m_name;
}
void CEmployee::SetName( string name )
{
m_name = name;
}
int CEmployee::GetSalary()
{
return m_salary;
}
void CEmployee::SetSalary( int v )
{
m_salary = v;
}
int CEmployee::GetSex()
{
return m_sex;
}
void CEmployee::SetSex( int v )
{
m_sex = v;
}
//Manager.h
#pragma once
#include "employee.h"
#include "IVisitor.h"
#include <iostream>
using std::string;
class CManager :
public CEmployee
{
public:
CManager(void);
~CManager(void);
string GetPerformance();
void SetPerformance(string per);
void Accept(IVisitor *pVisitor);
protected:
string GetOtherInfo();
private:
string m_performance;
};
//Manager.cpp
#include "StdAfx.h"
#include "Manager.h"
#include <iostream>
using std::string;
CManager::CManager(void)
{
this->m_performance = "";
}
CManager::~CManager(void)
{
}
string CManager::GetPerformance()
{
return m_performance;
}
void CManager::SetPerformance(string per)
{
this->m_performance = per;
}
string CManager::GetOtherInfo()
{
string info = "";
info.append("业绩：");
info.append(this->m_performance);
info.append("\t");
return info;
}
void CManager::Accept(IVisitor *pVisitor)
{
pVisitor->Visit(*this);
}
// Visitor.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "Employee.h"
#include "CommonEmployee.h"
#include "Manager.h"
#include "BaseVisitor.h"
#include "..\CommonDeclare\Convert.h"
#include <vector>
#include <iostream>
using std::vector;
using std::cout;
using std::endl;
void MockEmployee(vector<CEmployee*> *pvce)
{
CCommonEmployee *pZhangSan = new CCommonEmployee();
pZhangSan->SetJob("编写Java程序，绝对的蓝领、苦工加搬运工");
pZhangSan->SetName("张三");
pZhangSan->SetSalary(1800);
pZhangSan->SetSex(CEmployee::MALE);
pvce->push_back(pZhangSan);
CCommonEmployee *pLiSi = new CCommonEmployee();
pLiSi->SetJob("页面美工，审美素质太不流行了！");
pLiSi->SetName("李四");
pLiSi->SetSalary(1900);
pLiSi->SetSex(CEmployee::FEMALE);
pvce->push_back(pLiSi);
CManager *pWangWu = new CManager();
pWangWu->SetPerformance("基本上是负值，但是我会拍马屁呀");
pWangWu->SetName("王五");
pWangWu->SetSalary(1900);
pWangWu->SetSex(CEmployee::FEMALE);
pvce->push_back(pWangWu);
}
void DoIt()
{
vector<CEmployee*> vce;
MockEmployee(&vce);
vector<CEmployee*>::const_iterator readIt = vce.begin();
CBaseVisitor visitor;
for (; readIt != vce.end(); readIt ++)
{
(*readIt)->Accept(&visitor);
}
cout << "本公司的月工资总额是：" << CConvert::ToString(visitor.GetTotalSalary()).c_str() << endl;
vector<CEmployee*>::reverse_iterator delIt = vce.rbegin();
for (; delIt != vce.rend(); delIt++)
{
delete (*delIt);
(*delIt) = NULL;
}
vce.clear();
}
```
Main.cpp
```cpp
main(int argc, _TCHAR* argv[])
{
DoIt();
return 0;
}
```
**Visitor模式优缺点总结：**
Visitor模式优点：
(1).实现简单
**Visitor模式缺点：**
(1).破坏了封装性。Visitor模式要求Visitor可以从外部修改Element对象的状态，这一般通过两个方式来实现：a）Element提供足够的public接口，使得Visitor可以通过调用这些接口达到修改Element状态的目的；b）Element暴露更多的细节给Visitor，或者让Element提供public的实现给Visitor（当然也给了系统中其他的对象），或者将Visitor声明为Element的friend类，仅将细节暴露给Visitor。但是无论那种情况，特别是后者都将是破坏了封装性原则（实际上就是C++的friend机制得到了很多的面向对象专家的诟病）。
(2).ConcreteElement的扩展很困难：每增加一个Element的子类，就要修改Visitor的接口，使得可以提供给这个新增加的子类的访问机制。从上面我们可以看到，或者增加一个用于处理新增类的Visit（）接口，或者重载一个处理新增类的Visit（）操作，或者要修改RTTI方式实现的Visit（）实现。无论那种方式都给扩展新的Element子类带来了困难。
