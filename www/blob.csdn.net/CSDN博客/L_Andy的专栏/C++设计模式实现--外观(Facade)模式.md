# C++设计模式实现--外观(Facade)模式 - L_Andy的专栏 - CSDN博客

2014年06月13日 12:27:48[卡哥](https://me.csdn.net/L_Andy)阅读数：631
个人分类：[设计模式](https://blog.csdn.net/L_Andy/article/category/1335578)



**一. 举例说明**

还以我以前做的文件系统(FileSys)为例：

文件系统是一个独立的系统，它提供一套核心的文件操作。

除了文件系统，还有四个子系统，分别是杀毒子系统(KillVirus)，压缩子系统(ZipFile)，加密子系统(EncrypeFile)和刻录子系统(BurnCD)，这四个子系统相互独立，但又可以做为主系统功能的一部分。

假设客户需要我这个文件系统有两种执行模式，一种是完全模式，一种是简单模式。
完全模式，要求杀毒子，压缩，加密和刻录功能都有。
简单模式，要求只要有杀毒，刻录就行了。

**第一种设计：**

文件系统自己管理所有的子系统，并实现客户的需求。

![](https://img-my.csdn.net/uploads/201205/14/1336965272_9282.jpg)

最开始的话，我们是按上面的结构来设计的，这个文件系统(FileSys)就要自己管理和组织上面的四个子系统。问题是子系统变化比较多，特别是重构之后，接口也变了，这时也要相应的修改这个文件系统。最麻烦的是，有时一个子系统要分离出好多小类，这对子系统是好事，但是对FileSys来说，调用越来越复杂和困难了。

这种设计的问题是：文件系统和子系统耦合性太高了！

**第二种设计：**

后来我们独立出一个中间层，由中间层来统一管理这些子系统，并对外提供相对简单的接口，使它们之间减少依赖。
![](https://img-my.csdn.net/uploads/201205/14/1336965658_9121.jpg)
代码实现：

**[cpp]**[view
 plain](http://blog.csdn.net/lwbeyond/article/details/7564162#)[copy](http://blog.csdn.net/lwbeyond/article/details/7564162#)

- //杀毒
- class KillVirus    
- {    
- public:    
- void Operation1() { cout<<"杀毒"<<endl; }    
- };  
- 
- //压缩
- class ZipFile  
- {  
- public:  
- void Operation2() { cout<<"压缩"<<endl; }  
- };  
- 
- //加密
- class EncryptFile    
- {  
- public:  
- void Operation3() { cout<<"加密"<<endl; }    
- };  
- 
- //刻录
- class BurnCD  
- {    
- public:  
- void Operation4() { cout<<"刻录"<<endl;}    
- };  
- 
- //高层接口
- class OperatorWapper  
- {    
- public:  
- //完全功能
- void MethodA()     
-     {  
-         KillVirus kill;  
-         ZipFile zip;  
-         EncryptFile encrypt;  
-         BurnCD burn;  
- 
-         kill.Operation1();  
-         zip.Operation2();  
-         encrypt.Operation3();  
-         burn.Operation4();  
-     }  
- 
- //简单功能
- void MethodB()     
-     {  
-         KillVirus kill;  
-         BurnCD burn;  
- 
-         kill.Operation1();  
-         burn.Operation4();  
-     }  
- };  
- 
- //测试代码
- int main()    
- {    
-     OperatorWapper op;  
- 
-     op.MethodA();//完全功能
- 
-     op.MethodB();//简单功能
- 
- return 0;    
- }  

**二. 外观模式**

**定义：为子系统中的一组接口提供一个一致的界面， 外观模式定义了一个高层接口，这个接口使得这一子系统更加容易使用。**

![](https://img-my.csdn.net/uploads/201205/14/1336965898_4315.jpg)

简单的说，就是**分层的概念**。

**说明：**

1. 在设计初期，应该有意识的将不同层分离，比如常用的三层架构，就是考虑在数据访问层，业务逻辑层与表示层之间，建立Facade，使复杂的子系统提供一个简单的接口，降低耦合性。

2. 在开发阶段，子系统往往因为不断的重构而变的越来越复杂，增加外观Facade可以提供一个简单的接口，减少它们之间的依赖。

3. 在维护阶段，可能这个系统已经非常难以维护和扩展了，此时你可以为新系统开发一个外观类，来提供设计粗糙或高度复杂的遗留代码的比较清晰简单的接口，让新系统与Facade对象交互，Facade与遗留代码交互所有复杂的工作。

