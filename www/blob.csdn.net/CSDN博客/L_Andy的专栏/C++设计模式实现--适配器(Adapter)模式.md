# C++设计模式实现--适配器(Adapter)模式 - L_Andy的专栏 - CSDN博客

2014年06月16日 11:00:46[卡哥](https://me.csdn.net/L_Andy)阅读数：680
个人分类：[设计模式](https://blog.csdn.net/L_Andy/article/category/1335578)



**一. 定义**

**适配器模式将一个类的接口转换成客户希望的另外一个接口，使得原本由于接口不兼容而不能一起工作的那些类可以一起工作。**

Adapter 模式的两种类别：**类模式**和**对象模式**。

**二. 举例说明**

实际中，我们为了完成某项工作购买了一个第三方的库来加快开发。这个库可能都是一些.dll文件，这就带来了一个问题！我们在应用程序中已经设计好了接口，与这个第三方提供的接口不一致，为了使得这些接口不兼容的类（不能在一起工作）可以在一起工作了，Adapter模式提供了将一个类（第三方库）的接口转化为客户（购买使用者）希望的接口的方法。

**三. 代码实现**

类模式的Adapter采用**继承的方式**复用Adaptee的接口。

![](https://img-my.csdn.net/uploads/201205/07/1336370899_7866.jpg)

**[cpp]**[view
 plain](http://blog.csdn.net/lwbeyond/article/details/7542211#)[copy](http://blog.csdn.net/lwbeyond/article/details/7542211#)

- //目标类，可以想像成客户所期代的接口
- class Target  
- {  
- public:  
-     Target()  
-     {  
-     }  
- 
- virtual ~Target()  
-     {  
-     }  
- 
- virtualvoid Request()  
-     {  
-         std::cout<<"Target::Request"<<std::endl;  
-     }  
- };  
- 
- //被适应者，可以看成 dll 提供的接口
- class Adaptee  
- {  
- public:  
-     Adaptee()  
-     {  
-     }  
- 
- virtual ~Adaptee()  
-     {  
-     }  
- 
- void SpecificRequest()  
-     {  
-         std::cout<<"Adaptee::SpecificRequest"<<std::endl;  
-     }  
- };  
- 
- //适配器类，通过在内部包装被适应者对象 adaptee
- //把源接口(SpecificRequest)转换成目标接口(Request)
- class Adapter: public Target, private Adaptee  
- {  
- public:  
- void Request()  
-     {  
- this->SpecificRequest();  
-     }  
- };  
- 
- 
- //测试代码
- int main(int argc,char* argv[])  
- {  
-     Adaptee* ade = new Adaptee;  
-     Target* adt = new Adapter;  
- 
-     adt->Request();  
- 
- return 0;  
- }  

对象模式的Adapter中则采用**组合的方式**实现Adaptee的复用。

![](https://img-my.csdn.net/uploads/201205/07/1336370905_7852.jpg)

**[cpp]**[view
 plain](http://blog.csdn.net/lwbeyond/article/details/7542211#)[copy](http://blog.csdn.net/lwbeyond/article/details/7542211#)

- //目标类
- class Target  
- {  
- public:  
- virtual ~Target() {}  
- 
- virtualvoid Request()  
-     {  
-         std::cout<<"Target::Request"<<std::endl;  
-     }  
- };  
- 
- //被适应类
- class Adaptee  
- {  
- public:  
- virtual ~Adaptee() {}  
- 
- void SpecificRequest()  
-     {  
-         std::cout<<"Adaptee::SpecificRequest"<<std::endl;  
-     }  
- };  
- 
- //适配器类，用于转接两者的接口
- class Adapter:public Target  
- {  
- public:  
-     Adapter(Adaptee* ade)  
-     {  
- this->_ade= ade;  
-     }  
- 
- void Request()  
-     {  
-         _ade->SpecificRequest();  
-     }  
- 
- private:  
-     Adaptee* _ade;  
- };  
- 
- 
- //测试代码
- int main(int argc,char* argv[])  
- {  
-     Adaptee* ade = new Adaptee;  
-     Target* adt = new Adapter(ade);  
- 
-     adt->Request();  
- 
- return 0;  
- }  

**四. 说明**

1. 当接口不同时，首先**不应该考虑**用适配器，而是应该考虑通过重构统一接口。

2. 使用适配器模式是**无奈之举**，通常在软件开发后期或维护期再考虑使用。

