# C++反射机制 - youfangyuan - CSDN博客
2012年02月13日 17:29:32[youfangyuan](https://me.csdn.net/youfangyuan)阅读数：403
### [C++反射方法](http://blog.csdn.net/wrq147/article/details/5603262)
2010-05-18 10:55887人阅读[评论](http://blog.csdn.net/wrq147/article/details/5603262#comments)(0)收藏举报
众所周知C++语言本身是没有反射这个东东的，但C++的强大之处在于它把不能的化为可能。
要实现反射主要就几点问题：
1、每个要反射的类里定义一个回调函数（用来创建这个类实例）
2、在程序开始时把每个要反射的类的回调函数以类名为键存入全局哈希表
第一个问题我们使用继承就可以为每个类添加回调了
第二个问题我们知道程序开始时都会先初始化类的静态成员
现在看看我们要有一个类用来注册和获取类
#include <stdafx.h>
 #include   <string> 
  #include   <map> 
  #include   <iostream>
using namespace std;
typedef void* (*CreateFuntion)(void);//回调函数指针
class ClassFactory
{
public:
 static void* GetClassByName(std::string name)
 {
  std::map<std::string,CreateFuntion>::const_iterator find;
  find=m_clsMap.find(name);
  if(find==m_clsMap.end())
  {
   return NULL;
  }
  else
  {
   return find->second();
  }
 }
 static void RegistClass(std::string name,CreateFuntion method)
 {
  m_clsMap.insert(std::make_pair(name,method));
 }
private:
 static std::map<std::string,CreateFuntion> m_clsMap;
};
std::map<std::string,CreateFuntion>   ClassFactory::m_clsMap; 
我们还需要一个类包含回调函数和静态变量
template<class T,const char name[]>
class Register
{
public:
 Register()
 {
  //这个一定要加，因为编译器不保证程序开始时就初始化变量rc
  const RegistyClass tmp=rc;
 }
 static void* CreateInstance()
 {
  return new T;
 }
public:
 static const RegistyClass rc;
};
template<class T,const char name[]>
const RegistyClass Register<T,name>::rc(name,Register<T,name>::CreateInstance);
这里我们用宏来定义要反射的类会更方便
#define DEFINE_CLASS(class_name) /
char NameArray##class_name[]=#class_name;/
class class_name:public Register<class_name,NameArray##class_name>
#define DEFINE_CLASS_EX(class_name,father_class) /
char NameArray##class_name[]=#class_name;/
class class_name:public Register<class_name,NameArray##class_name>,public father_class
可以看到还需要一个RegistyClass 类来进行变量初始化并注册反射类
class RegistyClass
{
public:
 RegistyClass(std::string name,CreateFuntion method)
 {
  ClassFactory::RegistClass(name,method);
 }
};
最后的代码就是如下了
#include <stdafx.h>
 #include   <string> 
  #include   <map> 
  #include   <iostream>
using namespace std;
typedef void* (*CreateFuntion)(void);
class ClassFactory
{
public:
 static void* GetClassByName(std::string name)
 {
  std::map<std::string,CreateFuntion>::const_iterator find;
  find=m_clsMap.find(name);
  if(find==m_clsMap.end())
  {
   return NULL;
  }
  else
  {
   return find->second();
  }
 }
 static void RegistClass(std::string name,CreateFuntion method)
 {
  m_clsMap.insert(std::make_pair(name,method));
 }
private:
 static std::map<std::string,CreateFuntion> m_clsMap;
};
std::map<std::string,CreateFuntion>   ClassFactory::m_clsMap; 
class RegistyClass
{
public:
 RegistyClass(std::string name,CreateFuntion method)
 {
  ClassFactory::RegistClass(name,method);
 }
};
template<class T,const char name[]>
class Register
{
public:
 Register()
 {
  //这个一定要加，因为编译器不保证程序开始时就初始化变量rc
  const RegistyClass tmp=rc;
 }
 static void* CreateInstance()
 {
  return new T;
 }
public:
 static const RegistyClass rc;
};
template<class T,const char name[]>
const RegistyClass Register<T,name>::rc(name,Register<T,name>::CreateInstance);
#define DEFINE_CLASS(class_name) /
char NameArray[]=#class_name;/
class class_name:public Register<class_name,NameArray>
#define DEFINE_CLASS_EX(class_name,father_class) /
char NameArray[]=#class_name;/
class class_name:public Register<class_name,NameArray>,public father_class
DEFINE_CLASS(CG)
{
public:
 void Display()
 {
  printf("fff");
 }
};
int _tmain(int argc, _TCHAR* argv[])
{
 CG* tmp=(CG*)ClassFactory::GetClassByName("CG");
 tmp->Display();
 return 0;
}
