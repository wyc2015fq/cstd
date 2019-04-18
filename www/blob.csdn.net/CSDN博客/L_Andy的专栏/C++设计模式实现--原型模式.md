# C++设计模式实现--原型模式 - L_Andy的专栏 - CSDN博客

2014年06月09日 17:40:41[卡哥](https://me.csdn.net/L_Andy)阅读数：607


一、概述：
    用原型实例指定创建对象的种类，并且通过拷贝这些原型创建新的对象。其中有一个词很重要，那就是拷贝。可以说，拷贝是原型模式的精髓所在。举个现实中的例子来介绍原型模式。找工作的时候，我们需要准备简历。假设没有打印设备，因此需手写简历，这些简历的内容都是一样的。这样有个缺陷，如果要修改简历中的某项，那么所有已写好的简历都要修改，工作量很大。随着科技的进步，出现了打印设备。我们只需手写一份，然后利用打印设备复印多份即可。如果要修改简历中的某项，那么修改原始的版本就可以了，然后再复印。原始的那份手写稿相当于是一个原型，有了它，就可以通过复印（拷贝）创造出更多的新简历。这就是原型模式的基本思想。

**二、UML图：**

**![](https://img-blog.csdn.net/20130507151457479)**
三、优缺点：
    优点：复制自身。客户不知道需要对象的实际类型，只需知道它的抽象基类即可。(即有继承树的情况)
    缺点：必须先有一个对象实例(即原型)才能clone。 

**四、代码实现：**

**[cpp]**[view
 plain](http://blog.csdn.net/cabinriver/article/details/8895372#)[copy](http://blog.csdn.net/cabinriver/article/details/8895372#)

- #include <cstdio>
- 
- //接口
- class CPrototype  
- {  
- public:  
-     CPrototype(){}  
- virtual ~CPrototype(){}  
- 
- virtual CPrototype* Clone() = 0;  
- };  
- 
- //实现
- class CConcretePrototype : public CPrototype  
- {  
- public:  
-     CConcretePrototype():m_counter(0){}  
- virtual ~CConcretePrototype(){}  
- 
- //拷贝构造函数
-     CConcretePrototype(const CConcretePrototype& rhs)  
-     {  
-         m_counter = rhs.m_counter;  
-     }  
- 
- //复制自身
- virtual CPrototype* Clone()  
-     {  
- //调用拷贝构造函数
- returnnew CConcretePrototype(*this);  
-     }  
- 
- private:  
- int m_counter;  
- };  
- 
- int main(int argc, char **argv)  
- {  
- //生成对像
-     CPrototype* conProA = new CConcretePrototype();  
- //复制自身
-     CPrototype* conProB = conProA->Clone();  
- 
- delete conProA; conProA=NULL;  
- delete conProB; conProB=NULL;  
- 
- return 0;  
- }  

**五、综述：**
```cpp

```

```cpp

```

```cpp
Prototype模式同工厂模式,同样对客户隐藏了对象的创建工作.
```

```cpp

```

```cpp
但是,与通过对一个类进行实例化来构造新对象不同的是,原型模式是通过拷贝一个现有对象生成新对象的,
```

```cpp

```
达到了"隔离类对象的使用者和具体类型(易变类)之间的耦合关系"的目的.

```cpp

```
     原型模式可以很方便的快速构建相同类型的对象.其构造过程有点类似细胞分裂.通过原型模式的克隆接口克隆对象,我们可以完全不关心待克隆对象本身的类型,只需要调用接口就可以产生一个新的同类对象.

