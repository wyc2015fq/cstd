# C++设计模式实现--中介者(Mediator)模式 - L_Andy的专栏 - CSDN博客

2014年06月30日 15:20:01[卡哥](https://me.csdn.net/L_Andy)阅读数：626
个人分类：[设计模式](https://blog.csdn.net/L_Andy/article/category/1335578)



**一. 举例**

比如，现在中图和日本在关于钓鱼岛问题上存在争端。这时，联合国就会站出来，做为调解者，其实也没什么好调解的，钓鱼岛本来就是中国的，这是不争的事实！联合国也就是个传话者、发言人。
结构图如下：
![](https://img-my.csdn.net/uploads/201205/17/1337226545_1636.jpg)
代码如下：

**[cpp]**[view
 plain](http://blog.csdn.net/lwbeyond/article/details/7575776#)[copy](http://blog.csdn.net/lwbeyond/article/details/7575776#)

- //抽象国家
- class Country    
- {  
- protected:    
-     Mediator *m_mediator; //中介
- public:  
- virtualvoid SetMediator(Mediator *mediator){} //设置中介
- virtualvoid SendMessage(string message) {}    //向中介发信息
- virtualvoid GetMessage(string message) {}     //从中介获取信息
- };  
- 
- //抽象中介者
- class Mediator    
- {  
- public:    
- virtualvoid Send(string message, Country *person) {}    
- virtualvoid SetJanpa(Country *Janpa) {}  //让中介认识具体的对象
- virtualvoid SetChina(Country *China) {}    
- };  
- 
- //日本
- class Janpa: public Country    
- {    
- public:    
- void SetMediator(Mediator *mediator) { m_mediator = mediator; }    
- void SendMessage(string message) { m_mediator->Send(message, this); }    
- void GetMessage(string message) { cout<<"日本收到信息: "<<message; }    
- };  
- 
- //中国
- class China: public Country    
- {    
- public:    
- void SetMediator(Mediator *mediator) { m_mediator = mediator; }    
- void SendMessage(string message) { m_mediator->Send(message, this); }    
- void GetMessage(string message) { cout<<"中国收到信息: "<<message; }    
- };  
- 
- //联合国
- class UN : public Mediator    
- {    
- private:    
-     Country *m_Janpa; //日本
-     Country *m_China; //中国
- public:    
-     UN(): m_Janpa(NULL), m_China(NULL) {}    
- void SetJanpa(Country *Janpa) { m_Janpa = Janpa; }    
- void SetChina(Country *China) { m_China = China; }    
- void Send(string message, Country *country)     
-     {    
- if(country == m_Janpa) //日本给中图发信息
-             m_China->GetMessage(message); //中国收到信息
- else
-             m_Janpa->GetMessage(message);    
-     }  
- };  
- 
- //测试代码
- int main()    
- {       
-     Mediator *mediator = new UN();    
-     Country *pJanpa = new Janpa();    //日本
-     Country *pChina = new China();  //中国
- 
-     mediator->SetJanpa(Janpa);    
-     mediator->SetChina(China);    
- 
-     pJanpa->SetMediator(mediator);    
-     pChina->SetMediator(mediator);    
- 
-     pJanpa->SendMessage("钓鱼岛是我们的，哈哈哈...\n");    
-     pChina->SendMessage("操，你他妈再乱放屁，小心我弄死你！\n");    
- 
- delete pJanpa;   
- delete pChina;   
- delete mediator;    
- 
- return 0;    
- }  

**二. 中介者模式**

**定义：用一个中介对象来封装一系列的对象交互。中介者使各对象不需要显式地相互引用，从而使其耦合松散，而且可以独立地改变它们之间的交互。**
![](https://img-my.csdn.net/uploads/201205/17/1337226550_9084.jpg)

说明：

1. Mediator 模式中，每个Colleague 维护一个 Mediator，当要进行通信时，每个具体的 Colleague 直接向ConcreteMediator 发信息，至于信息发到哪里，则由 ConcreteMediator 来决定。

2. ConcreteColleagueA 和 ConcreteColleagueB 不必维护对各自的引用，甚至它们也不知道各个的存在。

3. 优点是，各个 Colleague 减少了耦合。

4. 缺点是，由于 Mediator 控制了集中化，于是就把 Colleague 之间的交互复杂性变为了中介者的复杂性，也就是中介者会变的比任何一个 Colleague 都复杂。

