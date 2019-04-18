# C++设计模式实现--建造者 - L_Andy的专栏 - CSDN博客

2014年06月09日 17:42:04[卡哥](https://me.csdn.net/L_Andy)阅读数：627
个人分类：[设计模式](https://blog.csdn.net/L_Andy/article/category/1335578)


概述:
        将一个复杂对象的构建与它的表示分离，使得同样的构建过程可以创建不同的表示。
UML结构图:
![](https://img-blog.csdn.net/20130507104841515)

适用于以下情况:
        1)当创建复杂对象的算法应该独立于该对象的组成部分以及它们的装配方式时。
        2)当构造过程必须允许被构造的对象有不同的表示时。
抽象基类:
        Builder基类是全部创建对象过程的抽象,提供构建不同组成部分的接口函数
Construct:
        在Director::Construct函数里，通过调用Builder的接口函数完成对象的构建--也就是说各个不同部分装配的过程都是一致的(同样的调用的Construct函数),但是不同的构建方式会有不同的表示(根据Builder的实际类型来决定如何构建,也就是多态)
解析:
        Builder模式是基于这样的一个情况:一个对象可能有不同的组成部分,这几个部分的不同的创建对象会有不同的表示,但是各个部分之间装配的方式是一致的.
比方说：
        1、一辆单车,都是由车轮车座等等的构成的(一个对象不同的组成部分),不同的品牌生产出来的也不一样(不同的构建方式).虽然不同的品牌构建出来的单车不同,但是构建的过程还是一样的(哦,你见过车轮长在车座上的么?).
        2、要盖一座房子住，可是我不知道怎么盖 （简单的砌墙，层次较低），也不知道怎么样设计（建几个房间，几个门好看，层次较高），于是我需要找一帮民工，他们会砌墙，还得找个设计师，他知道怎么设计，我还要确保民工听设计师的领导，而设计师本身也不干活，光是下命令，这里砌一堵墙， 这里砌一扇门，这样民工开始建设，最后，我可以向民工要房子了。在这个过程中，设计师是什么也没有，除了他在脑子里的设计和命令，所以要房子也是跟民工要，记住了！(让设计和施工解耦，互不干扰)。
也就是说,Director::Construct函数中固定了各个组成部分的装配方式,而具体是装配怎样的组成部分由Builder的派生类实现.
实现:
Builder模式的实现基于以下几个面向对象的设计原则:
       1)把变化的部分提取出来形成一个基类和对应的接口函数,在这里不会变化的是都会创建PartA和PartB,变化的则是不同的创建方法,于是就抽取出这里的Builder基类和BuildPartA,BuildPartB接口函数

       2)采用聚合的方式聚合了会发生变化的基类,就是这里Director聚合了Builder类的指针.

**代码实现：**

**[cpp]**[view
 plain](http://blog.csdn.net/cabinriver/article/details/8894055#)[copy](http://blog.csdn.net/cabinriver/article/details/8894055#)

- #include <cstdio>
- #include <vector>
- 
- struct Product  
- {  
-     std::vector<int> vInt;  
- };  
- 
- class IBuilder  
- {  
- public:  
-     IBuilder(){}  
- virtual ~IBuilder(){}  
- 
- virtualint BuildPartA() = 0;  
- 
- virtualint BuildPartB() = 0;  
- };  
- 
- class CConcreteBuilder : public IBuilder  
- {  
- public:  
-     CConcreteBuilder(){}  
- virtual ~CConcreteBuilder(){}  
- 
- virtualint BuildPartA()  
-     {  
-         m_product.vInt.push_back(1);  
-         printf("Build part A：Insert “1”\n");  
- 
- return 1;  
-     }  
- 
- virtualint BuildPartB()  
-     {  
-         m_product.vInt.push_back(2);  
-         printf("Build part B：Insert “2”\n");  
- 
- return 1;  
-     }  
- 
-     Product& GetResult(){return m_product;}  
- 
- private:  
-     Product m_product;  
- };  
- 
- class CDirector  
- {  
- public:  
-     CDirector(IBuilder* builder):m_pBuilder(builder){}  
-     ~CDirector(){}  
- 
- void Construct()  
-     {  
-         m_pBuilder->BuildPartA();  
-         m_pBuilder->BuildPartB();  
-     }  
- 
- private:  
-     IBuilder* m_pBuilder;  
- };  
- 
- int main(int argc, char **argv)  
- {  
-     CConcreteBuilder builder;  
- 
-     CDirector director(&builder);  
-     director.Construct();  
-     Product& pdt = builder.GetResult();  
- 
-     printf("打印产品: 【");  
- size_t count = pdt.vInt.size();  
- for (size_t i=0; i<count; ++i)  
-     {  
-         printf("%d", pdt.vInt[i]);  
-     }  
-     printf("】\n");  
- 
- return 0;  
- }  

