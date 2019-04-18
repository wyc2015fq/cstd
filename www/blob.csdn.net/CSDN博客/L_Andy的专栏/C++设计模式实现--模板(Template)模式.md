# C++设计模式实现--模板(Template)模式 - L_Andy的专栏 - CSDN博客

2014年06月13日 16:00:25[卡哥](https://me.csdn.net/L_Andy)阅读数：1654


**一. 问题**

在面向对象系统的分析与设计过程中经常会遇到这样一种情况：对于某一个业务逻辑（算法实现）在不同的对象中有不同的细节实现，但是逻辑（算法）的框架（或通用的应用算法）是相同的。Template提供了这种情况的一个实现框架。

**二. 模式**

Template 模式是采用继承的方式实现这一点：**将逻辑（算法）框架放在抽象基类中，并定义好细节的接口，子类中实现细节**。

![](https://img-my.csdn.net/uploads/201204/27/1335512435_3302.jpg)

**三. 代码**

**[cpp]**[view
 plain](http://blog.csdn.net/lwbeyond/article/details/7517679#)[copy](http://blog.csdn.net/lwbeyond/article/details/7517679#)

- //抽象基类，实现了一个模板方法
- class AbstractClass  
- {  
- public:  
- virtual ~AbstractClass()  
-     {  
-     }  
- 
- //模板方法，只在抽象基类中实现
- void TemplateMethod()  
-     {  
- this->PrimitiveOperation1();  
- this->PrimitiveOperation2();  
-     }  
- 
- protected:  
- virtualvoid PrimitiveOperation1() = 0;  
- 
- virtualvoid PrimitiveOperation2() = 0;  
- 
-     AbstractClass()  
-     {  
-     }  
- };  
- 
- //具体子类，实现操作的特定细节
- class ConcreteClass1:public AbstractClass  
- {  
- public:  
-     ConcreteClass1()  
-     {  
-     }  
- 
-     ~ConcreteClass1()  
-     {  
-     }  
- 
- protected:  
- void PrimitiveOperation1()  
-     {  
-         cout<<"ConcreteClass1...PrimitiveOperation1"<<endl;  
-     }  
- 
- void PrimitiveOperation2()  
-     {  
-         cout<<"ConcreteClass1...PrimitiveOperation2"<<endl;  
-     }  
- };  
- 
- //具体子类，实现操作的特定细节
- class ConcreteClass2:public AbstractClass  
- {  
- public:  
-     ConcreteClass2()   
-     {  
-     }  
- 
-     ~ConcreteClass2()  
-     {  
-     }  
- 
- protected:  
- void PrimitiveOperation1()  
-     {  
-         cout<<"ConcreteClass2...PrimitiveOperation1"<<endl;  
-     }  
- 
- void PrimitiveOperation2()  
-     {  
-         cout<<"ConcreteClass2...PrimitiveOperation2"<<endl;  
-     }  
- };  
- 
- 
- int main()  
- {  
-     AbstractClass* p1 = new ConcreteClass1();  
-     AbstractClass* p2 = new ConcreteClass2();  
- 
-     p1->TemplateMethod();  
-     p2->TemplateMethod();  
- 
- return 0;  
- }  

其关键点就是将通用算法封装在抽象基类中，并将不同的算法细节放到子类中实现。

