# C++设计模式实现--访问者(Visitor)模式 - L_Andy的专栏 - CSDN博客

2014年07月04日 16:12:22[卡哥](https://me.csdn.net/L_Andy)阅读数：1758


**一. 访问者模式**

**定义：表示一个作用于某对象结构中的各元素的操作。它你可以在不改变各元素的类的前提下定义作用于这些元素的新操作。**

**结构如下：**

![](https://img-my.csdn.net/uploads/201205/21/1337581991_7875.jpg)

**二. 举例**

假设有一项科学实验，是用来对比两种种子在不同环境下的生长情况。

两种种子，一种是普通的种子(Seed_A)，一种是太空运回的种子(Seed_B)。

生长环境，分别是在多雨环境下(Rain_Status)，阳光环境下(Sun_Status)等等。

**结构如下：**

![](https://img-my.csdn.net/uploads/201205/22/1337655836_9650.jpg)

**代码如下：**

**[cpp]**[view
 plain](http://blog.csdn.net/lwbeyond/article/details/7587191#)[copy](http://blog.csdn.net/lwbeyond/article/details/7587191#)

- //状态
- class Status  
- {  
- public:  
- virtual ~Status() {}  
- 
- virtualvoid VisitSeed_A(Seed* elm) {}  
- 
- virtualvoid VisitSeed_B(Seed* elm) {}  
- 
- protected:  
-     Status() {}  
- };   
- 
- //下雨状态
- class Rain_Status:public Status  
- {  
- public:  
-     Rain_Status() {}  
- 
- virtual ~Rain_Status() {}  
- 
- //下雨状态下A种子生长的情况
- virtualvoid VisitSeed_A(Seed* elm)  
-     {  
-         cout<<"Rain will visit Seed A..."<<endl;  
-     }  
- 
- //下雨状态下B种子生长的情况
- virtualvoid VisitSeed_B(Seed* elm)  
-     {  
-         cout<<"Rain will visit Seed B..."<<endl;  
-     }  
- };   
- 
- //阳光状态
- class Sun_Status:public Status  
- {  
- public:  
-     Sun_Status() {}  
- 
- virtual ~Sun_Status() {}  
- 
- //阳光状态下A种子生长的情况
- virtualvoid VisitSeed_A(Seed* elm)  
-     {  
-         cout<<"Sun will visit Seed A..."<<endl;  
-     }  
- 
- //阳光状态下B种子生长的情况
- virtualvoid VisitSeed_B(Seed* elm)  
-     {  
-         cout<<"Sun will visit Seed B..."<<endl;  
-     }  
- };  
- 
- 
- 
- //种子
- class Seed  
- {  
- public:  
- virtual ~Seed() {}  
- virtualvoid Accept(Status* vis) = 0;  
- 
- protected:  
-     Seed() {}  
- };  
- 
- //种子A，假设为普通种子
- class Seed_A:public Seed  
- {  
- public:  
-     Seed_A() {}  
- 
-     ~Seed_A() {}  
- 
- void Accept(Status* vis)  
-     {  
-         vis->VisitSeed_A(this);  
-     }  
- };   
- 
- //种子B，假设为从太空带回来的种子
- class Seed_B:public Seed  
- {  
- public:  
-     Seed_B() {}  
-     ~Seed_B() {}  
- 
- void Accept(Status* vis)  
-     {  
-         vis->VisitSeed_B(this);  
-     }  
- };  
- 
- 
- //对象结构类,为了对比不同种子
- class ObjectStructure  
- {  
- private:  
-     list<Seed*> lseed;  
- 
- public:  
- //Add
- void Attach(Seed* seed)  
-     {  
-         lseed.push_back(seed);  
-     }  
- 
- //Delete
- void Detach(Seed* seed)  
-     {  
-         lseed.remove(seed);  
-     }  
- 
- //Show
- void Display(Status* status)  
-     {  
-         list<Seed*>::iterator it = lseed.begin();  
- 
- for (it; it != lseed.end(); ++it)  
-         {  
-             (*it)->Accept(status);  
-         }  
-     }  
- };  
- 
- 
- //测试代码
- int main(int argc,char* argv[])  
- {  
-     ObjectStructure obj;  
- 
- //加入要对比的两个种子
-     obj.Attach(new Seed_A());  
-     obj.Attach(new Seed_B());  
- 
- //查看各种状态下两个种子的情况
-     obj.Display(new Rain_Status());  
- 
- //Sun Satte
-     obj.Display(new Sun_Status());  
- 
- return 0;  
- }  

**三. 说明**

**1. **首先有一点要明确，就是两种种子不会轻易改变，也就是只有普通和太空种子两种。换句话说就是，数据结构比较稳定。

**2. **可以变的是新增的状态，比如增加一个X光下的生成情况，等等。说白了就是，操作集合可以相对自由的演化。

**3. **这种结构的优点是，增加新的操作很容易；缺点是，增加新的数据结构有点困难，因为你要在每一个访问者里都添加相应的操作。

**4. **种子生长图相对于访问者模式的结构图有如下关系：

seed（种子）相当于 element（元素），这个是不怎么变的。

status（状态） 相当于 visitor（访问者），这个是可变且易变的。要注意的是，每个访问者都要对所有的元素（element）进行操作。

**5. **事实上我们很少用这种模式，因为数据结构（element）不变的情况很少。

