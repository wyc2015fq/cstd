# C++设计模式实现--备忘录(Memento)模式 - L_Andy的专栏 - CSDN博客

2014年06月17日 11:50:20[卡哥](https://me.csdn.net/L_Andy)阅读数：959


**一. 备忘录模式**

**定义：在不破坏封装性的前提下，捕获一个对象的内部状态，并在该对象之外保存这个状态。这样以后就可将该对象恢复到原先保存的状态。**

**结构图：**

![](https://img-my.csdn.net/uploads/201205/16/1337151119_5412.jpg)

**使用范围：**

Memento 模式比较适用于功能比较复杂的，但需要维护或记录属性历史的类，或者需要保存的属性只是众多属性中的一小部分时，Originator 可以根据保存的 Memento 信息还原到前一状态。

**代码：**

**[cpp]**[view
 plain](http://blog.csdn.net/lwbeyond/article/details/7572707#)[copy](http://blog.csdn.net/lwbeyond/article/details/7572707#)

- //备忘录类
- //负责存储 Originator 对象的内部状态
- class Memento   
- {  
- //注意：备忘录类中的方法是私有的！！！！！！！
- private:   
- //这是最关键的地方，将 Originator 为friend类
- //这样 Originator 就可以访问 Memento 类的内部信息
- friendclass Originator;   
- 
-     Memento() {}  
- 
-     Memento(const string& sdt)  
-     {  
-         _sdt = sdt;  
-     }  
- 
- void SetState(const string& sdt)  
-     {  
-         _sdt = sdt;  
-     }  
- 
-     string GetState()  
-     {  
- return _sdt;  
-     }  
- 
- private:  
-     string _sdt;  
- 
- };  
- 
- //原始类
- //负责创建备忘录
- class Originator   
- {  
- public:   
-     Originator()  
-     {  
-         _sdt = "";  
-         _mt = 0;   
-     }  
- 
-     Originator(const string& sdt)  
-     {  
-         _sdt = sdt;        
-         _mt = 0;   
-     }  
- 
- //创建备忘录，将当前信息保存在一个备忘录对象中
-     Memento* CreateMemento()  
-     {  
- returnnew Memento(_sdt);   
-     }  
- 
- //恢复备忘录，将备忘录对象上的信息恢复出来
- void RestoreToMemento(Memento* mt)  
-     {  
- this->_sdt = mt->GetState();  
-     }  
- 
-     string GetState()  
-     {  
- return _sdt;  
-     }  
- 
- void SetState(const string& sdt)  
-     {  
-         _sdt = sdt;   
-     }  
- 
- void PrintState()  
-     {  
-         cout<<this->_sdt<<"....."<<endl;   
-     }  
- 
- private:   
-     string _sdt;  
-     Memento* _mt;   
- };  
- 
- 
- //测试代码
- int main(int argc,char* argv[])   
- {  
-     Originator* o = new Originator();  
- 
-     o->SetState("old"); //备忘以前的状态
-     o->PrintState();  
- 
-     Memento* m = o->CreateMemento(); //创建一个备忘录
-     o->SetState("new"); //修改状态
-     o->PrintState();  
- 
-     o->RestoreToMemento(m); //恢复修改前的状态
-     o->PrintState();  
- 
- return 0;  
- }  

**二. 说明**

**1. Memento 负责存储 Originator 对象的内部状态，并可防止 Originator 以外的其它对象访问备忘录（具体实现方法，就是让其所有方法都为私有的）。**

**2. Memento 声明 Originator 为它的友元类，这样 Originator 就可以访问它的所有函数，即对 Originator 是开放的。**

