# C++接口定义及实现举例 - fanyun的博客 - CSDN博客
2016年04月05日 13:33:34[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：1338标签：[C++](https://so.csdn.net/so/search/s.do?q=C++&t=blog)
个人分类：[C++](https://blog.csdn.net/fanyun_01/article/category/6102444)
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)

C++接口定义及实现举例
一、接口的定义
    有时候，我们得提供一些接口给别人使用。接口的作用，就是提供一个与其他系统交互的方法。其他系统无需了解你内部细节，并且也无法了解内部细节，只能通过你提供给外部的接口来与你进行通信。根据c++的特点，我们可以采用纯虚函数的方式来实现。这样做的好处是能够实现封装和多态。现举一实例，供大家参考。（不想做过多说明，大家一看就应该能够明白）面向接口编程是oop的精华所在接口用于描述一组类的公共方法/公共属性.
 它不实现任何的方法或属性，只是告诉继承它的类至少要实现哪些功能,继承它的类可以增加自己的方法.使用接口可以使继承它的类: 命名统一/规范,易于维护.提供永远的接口。
 当类增加时，现有接口方法能够满足继承类中的大多数方法，没必要重新给新类设计一组方法，也节省了代码，提高了开发效率.接口用来定义必须实现的属性和方法，不需要实现。继承它的类必须实现这些属性和方法。一般接口用于工厂模式，在工作中用到的接口的地方不是很多，用工厂模式的时候用到了接口，实际上接口其实就是相当于一个规范。
Class IPerson
{
public：
   IPerson() {};
   virtual ~IPerson()=0 {}; //注意，最好定义此虚析构函数，够避免子类不能正常调用析构函数；如果定义为纯虚析构函数，则必须带定义体，因为子类隐含调用该析构函数。
   //提供给外面使用的接口一般采用纯虚函数
   virtual  void SetName（const string &strName）= 0;
   virtual  const string GetName（）= 0;
   virtual  void Work（）= 0;
}
二、接口实现
   实现接口是通过继承接口的子类来实现的，不同的子类可以实现不同效果，即使所谓多态。
Class CTeacher：public IPerson
{
public：
   CTeacher(){};
   virtual ~CTeacher（）；
   string m_strName；
   void SetName（const string &strName）;
   const string GetName（）;
   void Work（）;
}
void CTeacher：：SetName（const string &strName）
{
    m_strName = name;
}
const string CTeacher：：GetName（）
{
    return m_strName;
}
void CTeacher：：Work（）
{
    cout <<"I am teaching!"<<endl;//老师的工作是教书，其他职业的人做的工作是不一样的。
}
三、接口导出
bool GetIPersonObject(void** _RtObject)
{
   IPerson* pMan = NULL;
   pMan = new CTeacher();
   *_RtObject = (void*)pMan ;
   return true;
}
__declspec(dllexport) bool GetIPersonObject(void** _RtObject);
四、接口使用
#include "IPerson.h"
#pragma  comment(lib,"IPerson.lib")
bool  __declspec(dllimport)  GetIPersonObject(void** _RtObject);
/* 测试例子 */
void main()
{
       IPerson *    _IPersonObj = NULL;
   void*      pObj=NULL;
       if (GetIPersonObject(&pObj))       
        {
            // 获取对象
           _IPersonObj = (IPerson *)pObj;
            // 调用接口，执行操作
            _IPersonObj ->SetName（"Tom"）；
             string strName = _IPersonObj->GetName;
           _IPersonObj->work();
       }
       if (_IPersonObj !=NULL)
       {
           Delete _IPersonObj ;
           _IPersonObj  = NULL;
       }
}
