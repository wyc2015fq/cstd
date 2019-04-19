# C++模块化思想的应用 - fanyun的博客 - CSDN博客
2017年06月30日 20:02:31[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：693
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
模块化思想：
1.   将系统分成很多的模块，模块内部关注自身需要实现的核心业务。
2.   模块化的典型特点是：“模块间的低耦合”和“模块内的高聚合”。
模块的构成：
1.   名称。全局范围内表明该模块独一无二的标识。
2.   依赖。称模块运行引用的其他模块为依赖。
3.   接口。将模块对外提供的服务称为接口。
模块的管理：
1.   扫描模块列表
2.   分析模块依赖
3.   重排模块顺序
4.   初始化前准备
5.   模块的初始化
6.   模块反初始化
7.   模块最终销毁
模块化实践：
```cpp
#include <vector>
#include <string>
#include <functional>
using namespace std;
 
#ifndef _UNICODE
typedef std::string StdString;
#else
typedef std::wstring StdString;
#endif
```
定义模块结构：
```cpp
/**
 * 定义初始化及反初始化函数类型
 */
typedef function<void()> SelfFunction;
 
/**
 * 定义模块类型
 */
struct SelfModule
{
         /**
          * 模块的ID
          */
         StdString _id;
 
         /**
          * 模块的依赖，不同的依赖项之间用';'分割
          */
         StdString _depends;
 
         /**
          * 初始化之前的操作
          */
         SelfFunction _preinit;
 
         /**
          * 初始化函数
          */
         SelfFunction _init;
 
         /**
          * 反初始化函数
          */
         SelfFunction _uninit;
};
```
声明模块对象：
```cpp
/**
 * 声明模块的定义
 */
class DefModule
{
public:
         /*
          * 构造函数
          *
          * @Param id
          *        模块的ID
          * @Param depends
          *        模块的依赖
          * @Param preinit
          *        初始化之前的操作
          * @Param init
          *        模块的初始化函数
          * @Param uninit
          *        模块的反初始化函数
          * @Param destructor
          *        模块的析构函数
          */
         DefModule(StdString id
                   , StdString depends
                   , SelfFunction preinit
                   , SelfFunction init
                   , SelfFunction uninit
                   , SelfFunction destructor);
 
         /**
          * 析构函数
          */
         ~DefModule();
 
protected:
         /**
          * 析构函数
          */
         SelfFunction _destructor;
};
 
/**
 * DefModule
 */
DefModule::DefModule(StdString id
         , StdString depends
         , SelfFunction preinit
         , SelfFunction init
         , SelfFunction uninit
         , SelfFunction destructor)
         : _destructor(destructor)
{
         SelfModuleList* list = SelfModuleList::GetInstance();
 
         size_t offset = 0;
         size_t lastOffset = offset;
         while ((offset = depends.find(';', lastOffset)) != -1)
         {
                   StdString dep = depends.substr(lastOffset, offset - lastOffset);
                  
                   SelfModule& mod = list->FindModule(dep);
                   if (&mod == &SelfModuleList::_NullModule)
                   {
                            SelfModule module;
                            module._id = dep;
                            list->AddModule(module);
                   }
 
                   lastOffset = offset + 1;
         }
 
         SelfModule& mod = list->FindModule(id);
         if (&mod != &SelfModuleList::_NullModule)
         {
                   mod._init = init;
                   mod._uninit = uninit;
                   mod._preinit = preinit;
                   mod._depends = depends;
         }
         else
         {
                   SelfModule module;
                   module._id = id;
                   module._init = init;
                   module._uninit = uninit;
                   module._preinit = preinit;
                   module._depends = depends;
                   list->AddModule(module);               
         }
}
 
DefModule::~DefModule()
{
         _destructor();
}
```
定义模块列表：
```cpp
/**
 * 模块列表
 */
class SelfModuleList
{
public:
         /**
          * 获取唯一实例
          *
          * @Return 返回全局唯一的实例
          */
         static SelfModuleList* GetInstance()
         {
                   if (_instance != NULL)
                   {
                            return _instance;
                   }
 
                   if (_instance == NULL)
                   {
                            _instance = new SelfModuleList();
                   }
 
                   return _instance;
         }
 
         /**
          * 删除唯一实例
          */
         static void DelInstance()
         {
                   if (_instance != NULL)
                   {
                            delete _instance;
                            _instance = NULL;
                   }
         }
 
public:
         /**
          * 初始化之前的操作
          */
         void PreInits()
         {
                   for (auto iter = _list.begin(); iter != _list.end(); iter++)
                   {
                            (*iter)._preinit();
                   }
         }
 
         /**
          * 初始化函数
          */
         void Inits()
         {
                   for (auto iter = _list.begin(); iter != _list.end(); iter++)
                   {
                            (*iter)._init();
                   }
         }
 
         /**
          * 反初始化函数
          */
         void Uninits()
         {
                   for (auto iter = _list.rbegin(); iter != _list.rend(); iter++)
                   {
                            (*iter)._uninit();
                   }
         }
 
         /**
          * 增加模块
          *
          * @Param mod
          *        模块
          */
         void AddModule(SelfModule mod)
         {
                   _list.push_back(mod);
         }
 
         /**
          * 查找模块
          *
          * @Param id
          *        待查找的模块的ID
          * @Return 返回查找到的模块
          */
         SelfModule& FindModule(StdString id);
 
         /**
          * 模块的空引用
          */
         static SelfModule _NullModule;
 
protected:
         /**
          * 模块列表
          */
         vector<SelfModule> _list;
 
protected:
         SelfModuleList(void) {}
         ~SelfModuleList(void) {}
 
private:
         /**
          * 全局唯一的实例
          */
         static SelfModuleList* _instance;
};
 
/**
 * 模块的空引用
 */
SelfModule SelfModuleList::_NullModule;
 
/**
 * 全局唯一的实例
 */
SelfModuleList* SelfModuleList::_instance = NULL;
 
SelfModule& SelfModuleList::FindModule(StdString id)
{
         for (auto iter = _list.begin(); iter != _list.end(); iter++)
         {
                   if ((*iter)._id.compare(id) == 0)
                   {
                            return (*iter);
                   }
         }
         return _NullModule;
}
```
模块列表对象：
```cpp
**
 * 定义当前模块
 */
static DefModule _Module(_T("SelfModuleList")
         , _T("")
         , [](){
         }
         , [](){
         }
         , [](){
         }
         , [](){
                   SelfModuleList::DelInstance();
         });
```
