# C++“隐藏实现，开放接口”的实现方案 - 文章 - 伯乐在线
原文出处： [luoweifu](http://blog.csdn.net/luoweifu/article/details/47030857)
### 为什么要有接口？
接口就是一个程序与其它程序交流的窗口。就比如有一个电视机，我并不需要知道它是怎样工作的，我只要知道按电源键就可以开启电视，按节目加(+)减(-)可以切换电视频道就可以了。
Java程序员都知道Java中有interface可以实现对外的接口，但C++并没有接口这样的语法，那它要好怎样实现对外提供接口呢？我们可以通过纯虚函数定义一个抽象类，专门用来声明一个类的功能。
我们完成了一个程序模块的开发，要把这个程序模块给别人用，你肯定不会把源代码给他(那别人就完全撑屋你的技术了)，你会把这个程序模块编译成一个库(静态库lib或动态库dll)再给别人用。那别人拿到你的库后怎样用呢？这就需要看你的程序所提供的接口。C++的封装性是特别好的(个人觉得比Java好多了，Java打成的jar包很容易就可以被反编译，C++要反编译就困难多了)，我只要给你编译出的库和接口的头文件就可以了。
### 从一个实例讲讲实现方案
#### 需要
我们先来看一个场景。假设有一个电子文档(Document)、一个文档下有多个页(Page)，每个页下有多个文本单元(TextUnit,表示文档内元素的基本单位),一个文档中的所有文本单元对象都有唯一的ID。其类图关系如下：
![类的关系图](http://jbcdn2.b0.upaiyun.com/2016/11/a9ca6c7b635e7e857cf679ac162929c5.png)
图1 ：类的关系图
#### 设计
根据需求，我们可以定义三个类Document、Page、TextUnit分别表示文档、页、文本单元，每个类我们还需要一个对外的接口，于是需要三个对外的接口类IDocument、IPage、ITextUnit。
根据这些类我们先创建.cpp文件和.h文件，组织一下工程(EBook)目录结构如下：
![这里写图片描述](http://jbcdn2.b0.upaiyun.com/2016/11/e94d40e3819ec4c51365a5962828de2a.png)
图2: 工程目录结构
这里Document、Page、TextUnit就是具体的实现类，IDocument、IPage、ITextUnit就是对外提供的接口，这样就实现了实现与接口分离。
#### 代码实现
**IDocument.h：**


```
#pragma once
class IPage;
class IDocument
{
public:
    virtual ~IDocument(void){}
public:
    //---------------------------------------------------------------
    //function: 
    //          GenerateId 生成本文档内唯一的文本对象ID
    //Access:
    //          virtual  public  
    //Parameter:
    //Returns:
    //          int - 返回ID
    //Remarks:
    //          ...
    //author:    luoweifu
    //---------------------------------------------------------------
    virtual int GenerateId() = 0;
    //---------------------------------------------------------------
    //function: 
    //          AddPage 添加一页
    //Access:
    //          virtual  public  
    //Parameter:
    //Returns:
    //          IPage* - 返回页对象
    //Remarks:
    //          ...
    //author:    luoweifu
    //---------------------------------------------------------------
    virtual IPage* AddPage() = 0;
};
```
**IPage.h：**


```
#pragma once
class ITextUnit;
class IPage
{
public:
    virtual ~IPage(void){}
public:
    //---------------------------------------------------------------
    //function: 
    //          AddTextUnit 添加一个文本单元
    //Access:
    //          virtual  public  
    //Parameter:
    //Returns:
    //          ITextUnit* - 文本单元对象
    //Remarks:
    //          ...
    //author:   luoweifu
    //---------------------------------------------------------------
    virtual ITextUnit* AddTextUnit() = 0;
};
```
**ITextUnit.h**


```
#pragma once
class ITextUnit
{
public:
    ~ITextUnit(void){}
public:
    //---------------------------------------------------------------
    //function: 
    //          GetId 获得ID
    //Access:
    //          virtual  public  
    //Parameter:
    //Returns:
    //          int - 返回ID
    //Remarks:
    //          ...
    //author:   luoweifu
    //---------------------------------------------------------------
    virtual int GetId() = 0;
    //---------------------------------------------------------------
    //function: 
    //          SetId 设置ID
    //Access:
    //          virtual  public  
    //Parameter:
    //          [in] int id - 要设置的ID
    //Returns:
    //          void - 
    //Remarks:
    //          ...
    //author:   luoweifu
    //---------------------------------------------------------------
    virtual void SetId(int id) = 0;
};
```
### 提供C接口
从上面的代码我们可以看到IPage可以由IDocument创建,ITextUnit可以由IPage创建。那问题来了，IDocument由谁来创建呢？这时我们可以提供两个全局的函数CreateDoc和DestroyDoc用来创建和销毁IDocument的对象指针，这两个函数是全局函数(C类型的函数)，我们需要为其提供C的导出接口(这很重要)。其接口定义如下：


```
#pragma once
#include "IDocument.h"
#include "IPage.h"
#include "ITextUnit.h"
//===============================================================
//要导出静态库时，导出库的工程要加预编译宏STATIC_LIBRARY，使用库的工程什么也要加STATIC_LIBRARY宏
//要导出动态库时，导出库的工程要加预编译宏EXPORT，使用库的工程什么也不用加
//===============================================================
#ifdef EXPORT   //导出库
#define _API_  __declspec(dllexport)
#else           //导入库
#define _API_  __declspec(dllimport)
#endif  //EXPORT
#ifdef STATIC_LIBRARY   //导出静态库
#define EBAPI int
#else                   //导出动态库
#define EBAPI extern "C" _API_ int
#endif  //STATIC_LIBRARY
//---------------------------------------------------------------
//function:  
//      CreateDoc 创建Document对象
//Access:
//       public  
//Parameter:
//      [in] IDocument * & pDocument -
//Returns:
//      EBAPI -
//Remarks:
//      ...
//author:    luowf[/luoweifu]
//---------------------------------------------------------------
EBAPI CreateDoc(IDocument*& pDocument);
//---------------------------------------------------------------
//function:  
//      DestroyDoc 销毁一个Document对象
//Access:
//       public  
//Parameter:
//      [in] IDocument * pDocument -
//Returns:
//      EBAPI -
//Remarks:
//      ...
//author:    luowf[/luoweifu]
//---------------------------------------------------------------
EBAPI DestroyDoc(IDocument* pDocument);
```
### ``使用库
我们可以将EBook编译成一个静态库，然后再创建一个新的工程使用它。EBook工程设置：
![这里写图片描述](http://jbcdn2.b0.upaiyun.com/2016/11/53059bc94df7c1b4fd1cdc1b811e464a.png)
![这里写图片描述](http://jbcdn2.b0.upaiyun.com/2016/11/4d21440893fe30e0e335beec99fe7e2a.png)
图3： 工程配置(说明：上图中红色框中的EXPORT_STATIC已重命名为STATIC_LIBRARY的)
创建一个新的工程UseEBook使用EBook库。UseEBook工程配制：
Generation PropertiesC++PreprocessPreprocess Definitions:STATIC_LIBRARY
Generation PropertiesLinkerGeneralAddtional Library Directories:lib库所在路径
Generation PropertiesLinkerInputAddtional Dependencies:EBook.lib
**测试代码：**


```
#include "stdafx.h"
#include <iostream>
int _tmain(int argc, _TCHAR* argv[])
{
    IDocument* pDoc = NULL;
    if(CreateDoc(pDoc) != 0)
    {
        return -1;
    }
    IPage* pPage = pDoc->AddPage();
    ITextUnit* pTextUnit = pPage->AddTextUnit();
    std::cout << pTextUnit->GetId() << std::endl;
    DestroyDoc(pDoc);
    return 0;
}
```
