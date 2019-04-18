# C++设计模式实现--装饰者模式 - L_Andy的专栏 - CSDN博客

2014年07月25日 10:45:36[卡哥](https://me.csdn.net/L_Andy)阅读数：701
个人分类：[设计模式](https://blog.csdn.net/L_Andy/article/category/1335578)



        软件领域中的设计模式为开发人员提供了一种使用专家设计经验的有效途径。设计模式中运用了面向对象编程语言的重要特性：封装、继承、多态，真正领悟设计模式的精髓是可能一个漫长的过程，需要大量实践经验的积累。最近看设计模式的书，对于每个模式，用C++写了个小例子，加深一下理解。主要参考《大话设计模式》和《设计模式:可复用面向对象软件的基础》两本书。本文介绍装饰模式的实现。

       装饰模式：动态地给一个对象添加一些额外的职责。就增加功能来说，装饰模式相比生成子类更为灵活。有时我们希望给某个对象而不是整个类添加一些功能。比如有一个手机，允许你为手机添加特性，比如增加挂件、屏幕贴膜等。一种灵活的设计方式是，将手机嵌入到另一对象中，由这个对象完成特性的添加，我们称这个嵌入的对象为装饰。这个装饰与它所装饰的组件接口一致，因此它对使用该组件的客户透明。下面给出装饰模式的UML图。

![](http://hi.csdn.net/attachment/201108/9/0_13128725192ROQ.gif)

        在这种设计中，手机的装饰功能被独立出来，可以单独发展，进而简化了具体手机类的设计。下面给出Phone类的实现：

**[cpp]**[view
 plain](http://blog.csdn.net/wuzhekai1985/article/details/6672614#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6672614#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6672614#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6672614#)

- //公共抽象类
- class Phone  
- {  
- public:  
-     Phone() {}  
- virtual ~Phone() {}  
- virtualvoid ShowDecorate() {}  
- };  

        具体的手机类的定义：

**[cpp]**[view
 plain](http://blog.csdn.net/wuzhekai1985/article/details/6672614#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6672614#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6672614#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6672614#)

- //具体的手机类
- class iPhone : public Phone  
- {  
- private:  
-     string m_name; //手机名称
- public:  
-     iPhone(string name): m_name(name){}  
-     ~iPhone() {}  
- void ShowDecorate() { cout<<m_name<<"的装饰"<<endl;}  
- };  
- //具体的手机类
- class NokiaPhone : public Phone  
- {  
- private:  
-     string m_name;  
- public:  
-     NokiaPhone(string name): m_name(name){}  
-     ~NokiaPhone() {}  
- void ShowDecorate() { cout<<m_name<<"的装饰"<<endl;}  
- };  

        装饰类的实现：

**[cpp]**[view
 plain](http://blog.csdn.net/wuzhekai1985/article/details/6672614#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6672614#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6672614#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6672614#)

- //装饰类
- class DecoratorPhone : public Phone  
- {  
- private:  
-     Phone *m_phone;  //要装饰的手机
- public:  
-     DecoratorPhone(Phone *phone): m_phone(phone) {}  
- virtualvoid ShowDecorate() { m_phone->ShowDecorate(); }  
- };  
- //具体的装饰类
- class DecoratorPhoneA : public DecoratorPhone  
- {  
- public:  
-     DecoratorPhoneA(Phone *phone) : DecoratorPhone(phone) {}  
- void ShowDecorate() { DecoratorPhone::ShowDecorate(); AddDecorate(); }  
- private:  
- void AddDecorate() { cout<<"增加挂件"<<endl; } //增加的装饰
- };  
- //具体的装饰类
- class DecoratorPhoneB : public DecoratorPhone  
- {  
- public:  
-     DecoratorPhoneB(Phone *phone) : DecoratorPhone(phone) {}  
- void ShowDecorate() { DecoratorPhone::ShowDecorate(); AddDecorate(); }  
- private:  
- void AddDecorate() { cout<<"屏幕贴膜"<<endl; } //增加的装饰
- };  

         客户使用方式：

**[cpp]**[view
 plain](http://blog.csdn.net/wuzhekai1985/article/details/6672614#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6672614#)[print](http://blog.csdn.net/wuzhekai1985/article/details/6672614#)[?](http://blog.csdn.net/wuzhekai1985/article/details/6672614#)

- int main()  
- {  
-     Phone *iphone = new NokiaPhone("6300");  
-     Phone *dpa = new DecoratorPhoneA(iphone); //装饰，增加挂件
-     Phone *dpb = new DecoratorPhoneB(dpa);    //装饰，屏幕贴膜
-     dpb->ShowDecorate();  
- delete dpa;  
- delete dpb;  
- delete iphone;  
- return 0;  
- }  

       装饰模式提供了更加灵活的向对象添加职责的方式。可以用添加和分离的方法，用装饰在运行时刻增加和删除职责。装饰模式提供了一种“即用即付”的方

法来添加职责。它并不试图在一个复杂的可定制的类中支持所有可预见的特征，相反，你可以定义一个简单的类，并且用装饰类给它逐渐地添加功能。可以从简单的部件组合出复杂的功能。[DP]

       在本文的例子中，我们定义了两个具体的手机类，iPhone类和NokiaPhone类，通过单独的装饰类为它们添加特性，从而组合出复杂的功能。

