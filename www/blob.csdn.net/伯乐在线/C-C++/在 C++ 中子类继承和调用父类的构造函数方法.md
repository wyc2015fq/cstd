# 在 C++ 中子类继承和调用父类的构造函数方法 - 文章 - 伯乐在线
原文出处： [hemmingway，2012-08-11](http://blog.csdn.net/hemmingway/article/details/7853713)
构造方法用来初始化类的对象，与父类的其它成员不同，它不能被子类继承（子类可以继承父类所有的成员变量和成员方法，但不继承父类的构造方法）。因此，在创建子类对象时，为了初始化从父类继承来的数据成员，系统需要调用其父类的构造方法。
如果没有显式的构造函数，编译器会给一个默认的构造函数，并且该默认的构造函数仅仅在没有显式地声明构造函数情况下创建。
构造原则如下：
1. 如果子类没有定义构造方法，则调用父类的无参数的构造方法。
2. 如果子类定义了构造方法，不论是无参数还是带参数，在创建子类的对象的时候,首先执行父类无参数的构造方法，然后执行自己的构造方法。
3. 在创建子类对象时候，如果子类的构造函数没有显示调用父类的构造函数，则会调用父类的默认无参构造函数。
4. 在创建子类对象时候，如果子类的构造函数没有显示调用父类的构造函数且父类自己提供了无参构造函数，则会调用父类自己的无参构造函数。
5. 在创建子类对象时候，如果子类的构造函数没有显示调用父类的构造函数且父类只定义了自己的有参构造函数，则会出错（如果父类只有有参数的构造方法，则子类必须显示调用此带参构造方法）。
6. 如果子类调用父类带参数的构造方法，需要用初始化父类成员对象的方式，比如：


```
#include <iostream.h>  
　class animal  
　{  
　public:  
　　　animal(int height, int weight)  
　　　{  
　　　　　cout<<"animal construct"<<endl;  
　　　}  
　　　…  
　};  
　class fish:public animal  
　{  
　public:  
　　　fish():animal(400,300)  
　　　{  
　　　　　cout<<"fish construct"<<endl;  
　　　}  
　　　…  
　};  
　void main()  
　{  
　　　fish fh;  
　}
```
在fish类的构造函数后，加一个冒号（:），然后加上父类的带参数的构造函数。这样，在子类的构造函数被调用时，系统就会去调用父类的带参数的构造函数去构造对象。
例外像MFC常见的CDialog继承可以看到这种例子..
头文件中


```
#pragma once  
class CDrugDlg : public CDialogEx  
{  
    DECLARE_DYNAMIC(CDrugDlg)  
  
  
public:  
    CDrugDlg(CWnd* pParent = NULL);   // 标准构造函数  
    virtual ~CDrugDlg();  
  
  
// 对话框数据  
    enum { IDD = IDD_DRUG_DIALOG };  
  
protected:  
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持  
  
  
    DECLARE_MESSAGE_MAP()  
public:  
    afx_msg void OnSize(UINT nType, int cx, int cy);  
    virtual BOOL OnInitDialog();  
    afx_msg void OnPaint();  
    afx_msg void OnDestroy();  
};
```
实现文件


```
// DrugDlg.cpp : 实现文件  
//  
  
#include "stdafx.h"  
#include "Medical.h"  
#include "DrugDlg.h"  
#include "afxdialogex.h"  
  
// CDrugDlg 对话框  
  
IMPLEMENT_DYNAMIC(CDrugDlg, CDialog)  
  
CDrugDlg::CDrugDlg(CWnd* pParent /*=NULL*/)  
    :CDialogEx(CDrugDlg::IDD, pParent)  
{  
  
}  
  
CDrugDlg::~CDrugDlg()  
{  
}
```
这种初始化方式，还常用来对类中的常量（const）成员进行初始化，如下面的代码所示：


```
class point  
　　{  
　　public:  
　　　　 point():x(0),y(0)  
　　private:  
　　　　 const int x;  
　　　　 const int y;  
　　};
```
当然，类中普通的成员变量也可以采取此种方式进行初始化，然而，这就没有必要了..
