# Serializable 的必要条件 - ljx0305的专栏 - CSDN博客
2010年03月11日 15:47:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：623标签：[serialization																[constructor																[class																[mfc																[schema																[null](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=schema&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=constructor&t=blog)](https://so.csdn.net/so/search/s.do?q=serialization&t=blog)
个人分类：[VC](https://blog.csdn.net/ljx0305/article/category/401790)
**Serializable 的必要条件**
欲让一个对象有Serialize 能力，它必须衍生自一个Serializable 类别。一个类别意欲成Serializable必须有下列五大条件；至于其原因，前面的讨论已经全部交待过了。
1. 从CObject 衍生下来。如此一来可保有RTTI、Dynamic Creation 等机能。
2. 类别的声明部份必须有DECLARE_SERIAL 宏。此宏需要一个参数：类别名称。
3. 类别的实作部份必须有IMPLEMENT_SERIAL 宏。此宏需要三个参数：
     一是类别名称，二是父类别名称，三是schema no.。
4. 改写Serialize 虚拟函数，使它能够适当地把类别的成员变量写入文件中。
5. 为此类别加上一个default 构造式（也就是无参数之构造式）。这个条件常为人所忽略，但它是必要的，因为若一个对象来自文件，MFC 必须先动态生成它，而且在没有任何参数的情况下调用其构造式，然后才从文件中读出对象资料.
如此，让我们再复习一次本例之CStroke，看看是否符合上述五大条件：
// in SCRIBBLEDOC.H
class CStroke : public CObject //衍生自CObject（条件１）
{
public:
   CStroke(UINT nPenWidth);
protected:
   CStroke(); //拥有一个default constructor （条件５）
   DECLARE_SERIAL(CStroke) //使用SERIAL 宏（条件２）
protected:
   UINT m_nPenWidth;
public:
   CArray<CPoint,CPoint> m_pointArray;
public:
   virtual void Serialize(CArchive& ar); //改写Serialize 函数（条件４）
};
// in SCRIBBLEDOC.CPP
IMPLEMENT_SERIAL(CStroke, CObject, 1) //使用SERIAL 宏（条件３）
CStroke::CStroke() //拥有一个default constructor（条件５）
{
   // This empty constructor should be used by serialization only
}
void CStroke::Serialize(CArchive& ar) //改写Serialize 函数（条件４）
{
   CObject::Serialize(ar); //手册上告诉我们最好先调用此函数。
   //目前MFC 版本中它是空函数，所以不调用也没关系。
   if (ar.IsStoring())
   {
      ar << (WORD)m_nPenWidth;
      m_pointArray.Serialize(ar);
   }
   else
  {
      WORD w;
      ar >> w;
      m_nPenWidth = w;
      m_pointArray.Serialize(ar);
   }
}
一个类别如果希望有Serialization 机制，它的第二要件就是使用SERIAL 宏。这个巨
集包容DYNCREATE 宏，并且在类别的声明之中加上：
friend CArchive& AFXAPI operator>>(CArchive& ar, class_name* &pOb);
在类别的实作档中加上：
CArchive& AFXAPI operator>>(CArchive& ar, class_name* &pOb) /
{ pOb = (class_name*) ar.ReadObject(RUNTIME_CLASS(class_name)); /
return ar; } /
宏替换后的结果是:
CArchive& AFXAPI operator>>(CArchive& ar, CStroke* &pOb)
{ pOb = (CStroke*) ar.ReadObject(RUNTIME_CLASS(CStroke));
return ar; }
为什么只改写operator>>，而没有改写operator<<？原因是WriteObject 并
不需要CRuntimeClass 信息，但ReadObject 需要，因为在读完文件后还要做动态生成
的动作。
自定 SERIAL 宏给抽象类别使用
你是知道的，所谓抽象类别就是包含纯虚拟函数的类别，所谓纯虚拟函数就是只有声明
没有定义的虚拟函数。所以，你不可能）将。抽那象么类，别具现化（instantiated
IMPLEMENT_SERIAL 展开所得的这段码：
面对如果一个抽象类别class_name 就行不通了，编译时会产生错误消息。这时你得自行
定义宏如下：
也就是，令CreateObject 函数为NULL，这才能够使用于抽象类别之中。
