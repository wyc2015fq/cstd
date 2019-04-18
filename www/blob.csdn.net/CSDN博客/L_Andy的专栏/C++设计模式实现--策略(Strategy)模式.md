# C++设计模式实现--策略(Strategy)模式 - L_Andy的专栏 - CSDN博客

2014年06月13日 15:17:17[卡哥](https://me.csdn.net/L_Andy)阅读数：852
个人分类：[设计模式](https://blog.csdn.net/L_Andy/article/category/1335578)



**一. 举例说明**

以前做了一个程序，程序的功能是评价几种加密算法时间，程序的使用操作不怎么变，变的是选用各种算法。

**结构如下：**

![](https://img-my.csdn.net/uploads/201205/14/1336969977_8822.jpg)

Algorithm：抽象类，提供算法的公共接口。

RSA_Algorithm：具体的RSA算法。

DES_Algorithm：具体的DES算法。

BASE64_Algorithm：具体的Base64算法。

在使用过程中，我只需要对外公布Algorithm_Context这个类及接口即可。

**代码实现：**

**[cpp]**[view
 plain](http://blog.csdn.net/lwbeyond/article/details/7564758#)[copy](http://blog.csdn.net/lwbeyond/article/details/7564758#)

- //策略类
- class Algorithm    
- {  
- public:  
- virtualvoid calculate() = 0;    
- };  
- 
- //具体RSA算法
- class RSA_Algorithm : public Algorithm    
- {    
- public:    
- void calculate() { cout<<"RSA algorithm..."<<endl; }    
- };    
- 
- //具体DES算法
- class DES_Algorithm : public Algorithm    
- {    
- public:    
- void calculate() { cout<<"DES algorithm..."<<endl; }    
- };  
- 
- //具体Base64算法
- class BASE64_Algorithm: public Algorithm    
- {    
- public:  
- void calculate() { cout<<"Base64 algorithm..."<<endl; }    
- };   
- 
- //策略上下文
- class Algorithm_Context  
- {  
- private:  
-     Algorithm *m_ra;  
- 
- public:  
-     Algorithm_Context(Algorithm *ra) { m_ra = ra; }  
-     ~Algorithm_Context() { delete m_ra; }  
- 
- void calculate() { m_ra->calculate(); }  
- };  
- 
- //测试代码
- int main()  
- {  
-     Algorithm_Context context(new RSA_Algorithm()); //使用具体算法
- 
-     context.calculate();  
- 
- return 0;    
- }    

**一. 策略模式**

**定义：它定义了算法家族，分别封装起来，让它们之间可以互相替换，此算法的变化，不会影响到使用算法的客户。**

![](https://img-my.csdn.net/uploads/201205/14/1336975508_6801.jpg)

这里的关键就是将算法的逻辑抽象接口（DoAction）封装到一个类中（Context），再通过委托的方式将具体的算法实现委托给具体的 Strategy 类来实现（ConcreteStrategeA类）。

策略模式是一种定义一系列算法的方法，从概念上来看，所有这些算法完成的都是相同的工作，只是实现不同，它可以以相同

的方式调用所有的算法，减少了各种算法类与使用算法类之间的耦合。

策略模式的优点是简化了单元测试，因为每个算法都有自己的类，可以通过自己的接口单独测试。

策略模式就是用来封装算法的，但在实践中，我们发现可以用它来封装几乎任何类型的规则，只要在分析过程中听到需要在不同实践应用不同

的业务规则，就可以考虑使用策略模式处理这种变化的可能性。


