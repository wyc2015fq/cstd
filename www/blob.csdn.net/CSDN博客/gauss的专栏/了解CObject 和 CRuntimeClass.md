# 了解CObject 和 CRuntimeClass - gauss的专栏 - CSDN博客
2013年01月08日 13:14:40[gauss](https://me.csdn.net/mathlmx)阅读数：262
个人分类：[Window编程](https://blog.csdn.net/mathlmx/article/category/944343)

作者：林水怒 [shuinu@163.net](mailto:shuinu@163.net)
转载请保留作者署名 
CObject和CRuntimeClass是MFC中两个非常重要的类/结构,绝大部分MFC类都是以CObject做为基类， CRuntimeClass结构同CObject密不可分，了解它们对于深入理解MFC具有重要意义。
一、CRuntimeClass结构
要理解CObject，我们先来看一下CRuntimeClass这个在MFC中至关重要的一个结构。
每个从CObject中派生的类都有有一个CRuntimeClass对象同它关联以完成在运行时得到类实例的信息或者是它的基类。 在afx.h中它的定义如下: 
struct CRuntimeClass
{
// Attributes
LPCSTR m_lpszClassName; //类名，一般是指包含CRuntimeClass对象的类的名称
int m_nObjectSize; //包含CRuntimeClass对象的类sizeof的大小，不包括它分配的内存
UINT m_wSchema; // schema number of the loaded class
CObject* (PASCAL* m_pfnCreateObject)(); // NULL =< abstract class 指向一个建立实例的构造函数
#ifdef _AFXDLL
CRuntimeClass* (PASCAL* m_pfnGetBaseClass)();
#else
CRuntimeClass* m_pBaseClass;
#endif
//以上m_pBaseClass的指针(函数)是MFC运行时确定类层次的关键，它一个简单的单向链表
// Operations
CObject* CreateObject(); //这个函数给予CObject 派生类运行时动态建立的能力
BOOL IsDerivedFrom(const CRuntimeClass* pBaseClass) const;
//这个函数使用 m_pBaseClass或 m_pfnGetBaseClass遍历整个类层次确定是否pBaseClass指向的类是基类，
//使用它可以判断某类是否是从pBaseClass指向的类在派生来。
// Implementation
void Store(CArchive& ar) const;
static CRuntimeClass* PASCAL Load(CArchive& ar, UINT* pwSchemaNum);
// CRuntimeClass objects linked together in simple list
CRuntimeClass* m_pNextClass; // linked list of registered classes
};
二、CObject类
CObject是MFC类的大多数类的基类，主要是通过它实现：
(1)、运行类信息；(2)、序列化；(3)、对象诊断输出；(4)、同集合类相兼容；
(1)、运行时类信息:
注意：要想使用CRuntimeClass结构得到运行时类信息，你必须在你的类中包括DECLARE_DYNAMIC/IMPLEMENT_DYNAMIC、 DECLARE_DYNCREATE/IMPLEMENT_DYNCREATE或DECLARE_SERIAL/IMPLEMENT_SERIAL。但你的类必须是从CObject派生的才能使用这些宏， 因为通过DECLARE_DYNAMIC将定义一个实现如下的函数:
CRuntimeClass* PASCAL B::_GetBaseClass()
{ 
return RUNTIME_CLASS(base_name); 
}
其中的RUNTIME_CLASS是这样定义的 
#define RUNTIME_CLASS( class_name ) /
(CRuntimeClass *)(&class_name::class##class_name);
即得到类中的CRuntimeClass对象指针，显而易见，如果没有基类你用IMPLEMENT_DYNAMIC时将得到一个编译错误。 除非你象CObject一样不用DECLARE_DYNAMIC而定义和实现了这些函数，CObject中的GetBaseClass只是简单的返回NULL。 实际的DECLARE_DYNAMIC在afx.h中声明如下：
#define DECLARE_DYNAMIC(class_name) /
protected: /
static CRuntimeClass* PASCAL _GetBaseClass(); /
public: /
static const AFX_DATA CRuntimeClass class##class_name; /
virtual CRuntimeClass* GetRuntimeClass() const; /
IMPLEMENT_DYNAMIC在afx.h中定义如下： 
#define IMPLEMENT_DYNAMIC(class_name, base_class_name) /
IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, 0xFFFF, NULL)
#define IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, wSchema, pfnNew) /
CRuntimeClass* PASCAL class_name::_GetBaseClass() /
{ return RUNTIME_CLASS(base_class_name); } /
AFX_COMDAT const AFX_DATADEF CRuntimeClass class_name::class##class_name = { /
#class_name, sizeof(class class_name), wSchema, pfnNew, /
&class_name::_GetBaseClass, NULL }; /
CRuntimeClass* class_name::GetRuntimeClass() const /
{ return RUNTIME_CLASS(class_name); } /
其中的CRuntimeClass* GetRuntimeClass() const;被定义为虚函数，以完成在类层次上的重载。 这也是MFC利用多态实现运行时动态类信息的方法。
另外两个DECLARE_DYNCREATE和DECLARE_SERIAL类似。只不过它们多定义和实现了一些函数，对于使用DECLARE_DYNCREATE 要注意的是类必须要有一个无参数的缺省构造函数，因为在DECLARE_DYNCREATE中定义了一个CreateObject函数 用以在动态的建立对象，它只是一条简单的return new class_name。
我们先来看一下序列化：
，CObject实现这些功能绝大部分是通过它里面的CRuntimeClass对象classObject实现的, 
CObject不支持多重继承，即表示以CObject为基类的类层次中只能有一个CObject基类。
之所以会这样，就是因为CRuntimeClass对象的成员m_pBaseClass的关系。因为它只是一个单链表。
以下是它在afx.h中的定义： 
/////////////////////////////////////////////////////////////////////////////
// class CObject is the root of all compliant objects
#ifdef _AFXDLL
class CObject
#else
class AFX_NOVTABLE CObject
#endif
{
public:
// Object model (types, destruction, allocation)
virtual CRuntimeClass* GetRuntimeClass() const; 
// 上面的函数的实现只是很简单的return RUNTIME_CLASS(classObject);
virtual ~CObject(); // virtual destructors are necessary
// Diagnostic allocations
void* PASCAL operator new(size_t nSize);
void* PASCAL operator new(size_t, void* p);
void PASCAL operator delete(void* p);
#if _MSC_VER <= 1200
void PASCAL operator delete(void* p, void* pPlace);
#endif
#if defined(_DEBUG) && !defined(_AFX_NO_DEBUG_CRT)
// for file name/line number tracking using DEBUG_NEW
void* PASCAL operator new(size_t nSize, LPCSTR lpszFileName, int nLine);
#if _MSC_VER <= 1200
void PASCAL operator delete(void *p, LPCSTR lpszFileName, int nLine);
#endif
#endif
// Disable the copy constructor and assignment by default so you will get
// compiler errors instead of unexpected behaviour if you pass objects
// by value or assign objects.
protected:
CObject();
private:
CObject(const CObject& objectSrc); // no implementation
void operator=(const CObject& objectSrc); // no implementation
// Attributes
public:
BOOL IsSerializable() const; // 对对象进行序列化
BOOL IsKindOf(const CRuntimeClass* pClass) const; //判是否是
// Overridables
virtual void Serialize(CArchive& ar);
#if defined(_DEBUG) || defined(_AFXDLL)
// Diagnostic Support
virtual void AssertValid() const;
virtual void Dump(CDumpContext& dc) const;
#endif
// Implementation
public:
static const AFX_DATA CRuntimeClass classCObject;
#ifdef _AFXDLL
static CRuntimeClass* PASCAL _GetBaseClass();
#endif
};
如果你在你的类的实现和定义中使用可选宏的其中一个，你必须了解从CObject派生的好处。
第一级的宏是DECLARE_DYNAMIC/IMPLEMENT_DYNAMIC,它允许你在运行时处理类名和类层次中的位置，允许你做有意义的诊断Dump。
第二级的宏是DECLARE_SERIAL/IMPLEMENT_SERIAL,它包括第一级宏所有的功能，允许你进行对象的序列化。 
要想彻底了解序列化，不得不了解Archive类，我们将在下回详细论述这个类。希望感兴趣的朋友同我联系共同进步。 
Trackback: [http://tb.blog.csdn.net/TrackBack.aspx?PostId=3365](http://tb.blog.csdn.net/TrackBack.aspx?PostId=3365)
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/softart/archive/2007/10/27/1846440.aspx](http://blog.csdn.net/softart/archive/2007/10/27/1846440.aspx)
