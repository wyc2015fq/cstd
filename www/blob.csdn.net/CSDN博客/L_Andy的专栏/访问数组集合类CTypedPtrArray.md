# 访问数组集合类CTypedPtrArray - L_Andy的专栏 - CSDN博客

2012年10月11日 10:48:03[卡哥](https://me.csdn.net/L_Andy)阅读数：2086


张明奇（卡哥）

集合形状

集合类以它的“形状”及其元素类型为主要特性。形状是指集合组织和存储对象的方法。MFC 提供三种基本集合形状：列表、数组和映射（也称作字典）。您可以根据自己的特定编程问题选取最适合的集合形状。

列表 

列表类提供元素的有序、非索引列表，并作为双向链接表实现。列表有“头”和“尾”，不论是从列表头或列表尾添加或移除元素，还是在列表中间插入或删除元素，速度都非常快。

数组 

数组类提供对象的动态调整大小、有序的整数索引数组。 

映射（也称作字典） 

映射是将键对象与值对象相关联的集合。

基于模板的集合类

类型安全集合包含任何类型的对象，而实现这类集合的最容易的方法是使用一种 MFC 基于模板的类。

下面列出了 MFC 基于模板的集合类：

集合模板类

    集合内容                  数组                列表          

任何类型的对象的集合           CArray               CList        

指向任何类型的对象的指针集合 CTypedPtrArray     CTypedPtrList

不基于模板的集合类

如果应用程序已经使用 MFC 非模板类，则可以继续使用它们，但是对于新的集合，应考虑使用基于模板的类。下面列出了 MFC 不基于模板的集合类：

非模板集合类

        数组                列表  

       CObArray             CObList  

       CByteArray           CPtrList  

       CDWordArray           CStringList  

       CPtrArray    

       CStringArray    

       CWordArray    

       CUIntArray    

在有关选择集合类的建议中，“MFC 集合类的特性”表就 MFC 集合类的特性（形状除外）对其进行了描述： 

1－类是否使用 C++ 模板 

2－存储在集合中的元素是否可以序列化 

3－存储在集合中的元素是否可以转储以用于诊断 

4－集合是否是类型安全的  

   MFC 数组集合类（无论是基于模板的还是不基于模板的）都使用索引访问它们的元素。MFC 列表和映射集合类（无论是基于模板的还是不基于模板的）都使用 POSITION 类型的指针描述集合内的给定位置。若要访问这些集合的一个或多个成员，请首先初始化位置指针，然后重复将该位置传递到集合并要求集合返回下一个元素。集合不负责维护迭代进度的状态信息。此信息保留在位置指针中。但是，如果给定特定位置，则集合负责返回下一个元素。

下面的过程显示了如何在由 MFC 提供的三种主要集合类型上遍历：

  1:遍历数组  

对 GetAt 成员函数使用顺序索引号： 

CTypedPtrArray<CObArray, CPerson*> myArray;

for( int i = 0; i < myArray.GetSize();i++ )

{

     CPerson* thePerson = myArray.GetAt( i );

     ...

}

此示例使用包含指向 CPerson 对象的指针的类型指针数组。数组从 CObArray 类（非模板预定义类之一）中派生。GetAt 返回指向 CPerson 对象的指针。对于类型指针集合类（即数组或列表），第一个参数指定基类；第二个参数指定要存储的类型。

CTypedPtrArray 类也重载 [ ] 运算符，以便可以使用自定义的数组下标语法访问数组元素。以上 for 循环主体中语句的另一种替换方法是：

CPerson* thePerson = myArray[ i ];

此运算符既在 const 版本中存在，也在非 const 版本中存在。const 版本（为 const 数组调用）只能显示在赋值语句的右侧。

2:遍历列表  

使用成员函数 GetHeadPosition 和 GetNext 迭代列表： 

CTypedPtrList<CObList, CPerson*> myList;

POSITION pos = myList.GetHeadPosition();

while( pos != NULL )

{

     CPerson* thePerson = myList.GetNext( pos );

     ...

}

此示例使用类型指针列表包含指向 CPerson 对象的指针。该列表声明类似于过程迭代数组中的数组列表声明，但它是从 CObList 类派生的。GetNext 返回指向 CPerson 对象的指针。

/////////////////////////////////////////////////////////////////

CTypedPtrArray Class的定义

template< class BASE_CLASS, class TYPE >

class CTypedPtrArray : public BASE_CLASS

Parameters

BASE_CLASS 

Base class of the typed pointer array class; must be an array class (CObArray or CPtrArray).

TYPE 

Type of the elements stored in the base-class array. 

Remarks

The CTypedPtrArray class provides a type-safe "wrapper" for objects of class CPtrArray or CObArray. When you use CTypedPtrArray rather than CPtrArray or CObArray, the C++ type-checking facility helps eliminate errors caused by mismatched
 pointer types.

In addition, the CTypedPtrArray wrapper performs much of the casting that would be required if you used CObArray or CPtrArray.

Because all CTypedPtrArray functions are inline, use of this template does not significantly affect the size or speed of your code.

For more information on using CTypedPtrArray, see the articles Collections and Template-Based Classes.

Requirements

Header: afxtempl.h

/////////////////////////////////////////////////////////////////

CTypedPtrArray Members list:

Element Access

Add()

---Adds a new element to the end of an array. Grows the array if necessary.

Append()

---Adds the contents of one array to the end of another. Grows the array if necessary.

Copy()

---Copies another array to the array; grows the array if necessary.

ElementAt()

---Returns a temporary reference to the element pointer within the array.

GetAt()

---Returns the value at a given index.

InsertAt()

---Inserts an element (or all the elements in another array) at a specified index.

SetAt()

---Sets the value for a given index; array not allowed to grow.

SetAtGrow()

---Sets the value for a given index; grows the array if necessary.

Operators

operator[]

---Sets or gets the element at the specified index.

///////////////////////////////////////////

CPtrArray 与 CObArray的区别

就是说用CTypedPtrArray能够利用C++   type-checking机制，防止类型不匹配的错误。比如： 

CObArray只能容纳CObject的派生类对象。 

CPtrArray的操作都是关于void*的，编译器不知道内部存储对象的类型。如果不匹配，将造成运行时的错误。 

CTypedPtrArray是模版类，可以通过CTypedPtrArray <type*>   list申明操作任意type*的list对象。如果类型与type*不匹配，仅仅出现编译错误。

通常数组里面存放的都是指针，因为CArray分配和释放空间的时候没有考虑到构造和析构函数。不需要构造和析构的对象可以直接存放在数组中，比如RECT,int等。如果速度不是太重要的话，建议同时使用CTypedPtrArray和CObArray，可以获得转储、调试和诊断支持。

typedef   CMyArray   CTypedPtrArray <CObArray,CMyObject*> ;

