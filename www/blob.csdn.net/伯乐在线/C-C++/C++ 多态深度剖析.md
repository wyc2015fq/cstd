# C++ 多态深度剖析 - 文章 - 伯乐在线
原文出处： [Fireplusplus](http://blog.csdn.net/qq_33724710/article/details/51645228)
> 
测试环境：Target: x86_64-linux-gnu
gcc version 5.3.1 20160413 (Ubuntu 5.3.1-14ubuntu2.1)
### 什么是多态？
多态一词最初来源于希腊语,意思是具有多种形式或形态的情形,当然这只是字面意思，它在C++语言中多态有着更广泛的含义。
这要先从对象的类型说起！对象的类型有两种：
![](http://jbcdn2.b0.upaiyun.com/2016/11/09fd60863c46352f2a0d5ae710cba51f.png)
举个栗子：Derived1类和Derived2类继承Base类


```
class Base  
{};  
  
class Derived1 : public Base  
{};  
  
class Derived2 : public Base  
{};  
  
int main()  
{  
    Derived1 pd1 = new Derived1; //pd1的静态类型为Derived1，动态类型为Derived1  
    Base *pb = pd1; //pb的静态类型为Base，动态类型现在为Derived1  
    Derived2 pd2 = new Derived2; //pd2的静态类型为Derived2，动态类型现在为Derived2  
    pb = pd2; //pb的静态类型为Base，动态类型现在为Derived2  
  
    return 0;  
}
```
对象有静态类型，也有动态类型，这就是一种类型的多态。
### 多态分类
多态有静态多态，也有动态多态，静态多态，比如函数重载，能够在编译器确定应该调用哪个函数；动态多态，比如继承加虚函数的方式（与对象的动态类型紧密联系，后面详解），通过对象调用的虚函数是哪个是在运行时才能确定的。
![](http://jbcdn2.b0.upaiyun.com/2016/11/ae91bded2fb09a4ae9b2d9d051bf7528.png)
#### 【静态多态】
栗子：函数重载


```
long long Add(int left, int right)  
{  
    return left + right;  
}  
  
double Add(float left, float right)  
{  
    return left + right;  
}  
  
int main()  
{  
    cout<<Add(10, 20)<<endl; //语句一  
    cout<<Add(12.34f, 43.12f)<<endl; //语句二  
  
    return 0;  
}
```
![](http://jbcdn2.b0.upaiyun.com/2016/11/2e245e54e869acafc6f35b73d53f1aa3.png)
编译器在编译期间完成的,编译器根据函数实参的类型(可能会进行隐式类型转换),即可推断出要调用哪个函数,如果有对应的函数就调用该函数,否则出现编译错误。
#### 【动态多态】
进入动态多态前，先看一个普通继承的栗子：


```
class Person  
{  
public:  
    void GoToWashRoom()  
    {  
        cout<<"Person-->?"<<endl;  
    }  
};  
  
class Man : public Person  
{  
public:  
    void GoToWashRoom()  
    {  
        cout<<"Man-->Please Left"<<endl;  
    }  
};  
  
class Woman : public Person  
{  
public:  
    void GoToWashRoom()  
    {  
        cout<<"Woman-->Please Right"<<endl;  
    }  
};  
  
int main()  
{  
    Person per, *pp;  
    Man man, *pm;  
    Woman woman, *pw;  
  
    pp = &per;  
    pm = &man;  
    pw = &woman;  
      
    //第一组       //这些都是毫无疑问的  
    per.GoToWashRoom(); //调用基类Person类的函数  
    pp->GoToWashRoom();  //调用基类Person类的函数  
    man.GoToWashRoom(); //调用派生类Man类的函数   
    pm->GoToWashRoom();  //调用派生类Man类的函数   
    woman.GoToWashRoom();   //调用派生类Woman类的函数   
    pw->GoToWashRoom();  //调用派生类Woman类的函数   
      
    //第二组  
    pp = &man;  
    pp->GoToWashRoom();  //调用基类Person类的函数  
    pp = &woman;  
    pp->GoToWashRoom();  //调用基类Person类的函数  
      
    return 0;  
}
```
运行结果：
![](http://jbcdn2.b0.upaiyun.com/2016/11/056e78720c1d0dc2d7586c26ae72594a.png)
第一组毫无疑问，通过本类对象和本类对象的指针就是调用本类的函数；第二组中先让基类指针指向子类对象，然后调用该函数，调用的是子类的，后让基类指针指向另一个子类对象，调用的是子类的函数。这是因为p的类型是一个基类的指针类型，那么在p看来，它指向的就是一个基类对象，所以调用了基类函数。就像一个int型的指针，不论它指向哪，读出来的都是一个整型（在没有崩溃的前提下），即使将它指向一个float。再来对比着看下一个栗子。
栗子：继承+虚函数


```
class Person  
{  
public:  
    virtual void GoToWashRoom() = 0;  
};  
  
class Man : public Person  
{  
public:  
    virtual void GoToWashRoom()  
    {  
        cout<<"Man-->Please Left"<<endl;  
    }  
};  
  
class Woman : public Person  
{  
public:  
    virtual void GoToWashRoom()  
    {  
        cout<<"Woman-->Please Right"<<endl;  
    }  
};  
  
int main()  
{  
    for (int i = 0; i < 10; i++)  
    {  
        Person *p;  
        if (i&0x01)  
            p = new Man;  
        else  
            p = new Woman;  
  
        p->GoToWashRoom();  
        delete p;  
        sleep(1);  
    }  
  
    return 0;  
}
```
运行结果：
![](http://jbcdn2.b0.upaiyun.com/2016/11/c5ece40ecda571fb66655c0b51947a29.png)
就像上边这个栗子所演示的那样，通过重写虚函数（不再是普通的成员函数，是虚函数！），实现了动态绑定，即在程序执行期间(非编译期)判断所引用对象的实际类型,根据其实际类型调用相应的方法。使用virtual关键字修饰函数时,指明该函数为虚函数（在栗子中为纯虚函数）,派生类需要重新实现,编译器将实现动态绑定。在上边栗子中，当指针p指向Man类的对象时，调用了Man类自己的函数，p指向Woman类对象时，调用了Woman类字几的函数。
今天的重点来了，就是要分析这个动态绑定实现的原理（以下测试在VS2013环境下进行）：
为了方便调试，我将程序修改如下：


```
class Person  
{  
public:  
    void GoToWashRoom()  
    {};  
};  
  
class Man : public Person  
{  
public:  
    void GoToWashRoom()  
    {  
        cout << "Man-->Please Left" << endl;  
    }  
};  
  
int main()  
{  
    cout << sizeof(Person) << endl;  
    cout << sizeof(Person) << endl;  
    return 0;  
}
```
先求一下两个普通的继承类的大小，在这里为空类，没有包含成员变量，所以为1，表示占位：
![](http://jbcdn2.b0.upaiyun.com/2016/11/00bfc3647bbb09f825e911c53db6d38d.png)
假如基类中包含一个int型变量，那么这里的大小都会是4。这不是今天的重点，不再叙说。主要是想看看普通空类的大小。
再改一下程序，在基类的成员函数前加virtual关键字，将这个函数变为虚函数。


```
class Person  
{  
public:  
    virtual void GoToWashRoom()  
    {};  
};
```
其它部分代码不变，运行结果：
![](http://jbcdn2.b0.upaiyun.com/2016/11/177c3dce5fdfa42b355c72b976c31e07.png)
大小变成了4.所以这个类里面肯定是有什么东西的。
在main中加入以下代码：
Man man;
Person *p = &man;
p->GoToWashRoom();
查看监视窗口：
![](http://jbcdn2.b0.upaiyun.com/2016/11/839e02c07f0b4d8329379d4573df5c6f.png)
man对象里存在了一个指针，而且是void**类型的，那么这个指针指向哪呢？可以在内存中查看一下这个地址所在内存中的内容。
![](http://jbcdn2.b0.upaiyun.com/2016/11/094f7c7400bc324759533c4ad24e2bef.png)
是一个可能是地址的东西，然后下边是一排的 00 00 00 00，貌似相当于NULL。继续看一下这个类似于地址的东西里面又是什么：
![](http://jbcdn2.b0.upaiyun.com/2016/11/7c0943c84ecd54749520ad36eae3e60d.png)
嗯，看不懂，不要紧，把这个数字记下来：0x01 27 13 de
继续运行程序，转到反汇编：
![](http://jbcdn2.b0.upaiyun.com/2016/11/20596aa5d641406cf6fa17cf5df0f5ba.png)
这两句取到man的首地址然后通过eax寄存器赋值给p，这样p就指向了man对象。目前对象模型是这样的：
![](http://jbcdn2.b0.upaiyun.com/2016/11/e663ff10f771ddd33aa00089f9158db3.png)
同时，_vfptr的值为0x01 27 13 de。接下来就要准备调用函数了。
![](http://jbcdn2.b0.upaiyun.com/2016/11/0f1b4bd813c87b8425c59afd90ec32e4.png)
一句句分析：
01276036  mov         eax,dword ptr [p]    //从p所指位置取4个字节内容放到eax，其实就是取的man对象的地址：0x008BFC1C
01276039  mov         edx,dword ptr [eax]  //从eax所指位置取4个字节内容放到edx，就是我们之前看到的不明变量_vfptr的值：0x0127dc80
0127603B  mov         esi,esp  //这句先不用管，esp是栈顶指针
0127603D  mov         ecx,dword ptr [p]  //将对象地址给ecx也保存了一份，此时ecx和eax放的都是对象地址（其实这句就是调用函数之前通过ecx传递this指针）
01276040  mov         eax,dword ptr [edx]  //取对象前四个字节给eax，eax和edx都变成了_vfptr的值。
01276042  call        eax  //调用函数，函数地址在eax中，说明_vfptr指向的内容是函数的地址
01276044  cmp         esi,esp
01276046  call        __RTC_CheckEsp (01271334h)
到call这条语句跟进去看一下：
![](http://jbcdn2.b0.upaiyun.com/2016/11/bc24777f77bc6bbe12945ecb6ca72c0c.png)
这下明白了吧，其实_vfptr存放的内容就是存放函数地址的地址，即_vfptr指向函数地址所在的内存空间，如图：
![](http://jbcdn2.b0.upaiyun.com/2016/11/45b6a2beed8b14d2829aed90a3953337.png)
分析暂告一段落。我们知道了man对象中维护了一个虚表指针，虚表中存放着虚函数的地址。基于这个虚表指针，实现动态绑定，才可以用基类指针调用了Man类中的虚函数。因为指针p看到的是虚表的指针，它调用的虚函数是从虚表中查找的。如果基类中有多个虚函数的话，那么虚表中也会依次按基类中虚函数定义顺序存放虚函数的地址，并以0x 00 00 00 00 结尾。再如果子类中有自己定义的新的虚函数，那么会排在虚函数表的后边。在调用虚函数时由编译器自动计算偏移取得相应的虚函数地址。
看看是如何构造子类对象的：


```
class Person  
{  
public:  
    Person()  
    {  
        cout << "Person()" << endl;  
    }  
    ~Person()  
    {  
        cout << "~Person()" << endl;  
    }  
  
    virtual void GoToWashRoom()  
    {};  
};  
  
class Man : public Person  
{  
public:  
    Man()  
    {  
        cout << "Man()" << endl;  
    }  
    ~Man()  
    {  
        cout << "~Man()" << endl;  
    }  
  
    void GoToWashRoom()  
    {  
        cout << "Man-->Please Left" << endl;  
    }  
};  
  
int main()  
{  
    Man man;  
  
    return 0;  
}
```
运行结果：
![](http://jbcdn2.b0.upaiyun.com/2016/11/91f4c26ee9d1b53577636890ea21d84d.png)
先调用基类构造函数，虚表指针先指向基类虚表，然后调用子类构造函数，这时候子类对象的虚表指针就指向了子类自己的虚表。这才是动态绑定实现原理。详细内容可以查看反汇编，这里不再写了。
再举个栗子看看虚表指针在对象的首部还是尾部，又或者是在中间某个地方存放：


```
class Person  
{  
public:  
    virtual void GoToWashRoom()  
    {};  
public:  
    int i1;  
    int i2;  
    int i3;  
};  
  
class Man : public Person  
{  
    void GoToWashRoom()  
    {  
        cout << "Man-->Please Left" << endl;  
    }  
};  
  
int main()  
{  
    Man man;  
    man.i1 = 0x01;  
    man.i2 = 0x02;  
    man.i3 = 0x03;  
  
    return 0;  
}
```
查看内存中：&man
![](http://jbcdn2.b0.upaiyun.com/2016/11/6092b9e74b5f27c12a859c7e7d3ed29b.png)
可以看出，虚表指针位于对象的首部。
【动态绑定条件】
- 必须是虚函数
- 通过基类类型的引用或者指针调用
### 总结
- 派生类重写基类的虚函数实现多态，要求函数名、参数列表、返回值完全相同。(协变除外)
- 基类中定义了虚函数，在派生类中该函数始终保持虚函数的特性
- 只有类的成员函数才能定义为虚函数，静态成员函数不能定义为虚函数
- 如果在类外定义虚函数，只能在声明函数时加virtual关键字，定义时不用加
- 构造函数不能定义为虚函数，虽然可以将operator=定义为虚函数，但最好不要这么做，使用时容 易混淆
- 不要在构造函数和析构函数中调用虚函数，在构造函数和析构函数中，对象是不完整的，可能会 出现未定义的行为
- 最好将基类的析构函数声明为虚函数。(析构函数比较特殊，因为派生类的析构函数跟基类的析构 函数名称不一样，但是构成覆盖，这里编译器做了特殊处理)
- 虚表是所有类对象实例共用的


```
//协变，也可以构成重写（覆盖），但返回值是该类类型的指针或引用  
class Base     
{         
    virtual Base * FunTest()    
    {    
        //do something    
    }    
};    
class Derived : public Base    
{    
    Derived * FunTest()    
    {    
        //do something    
    }    
};
```
容易混淆的点：
![](http://jbcdn2.b0.upaiyun.com/2016/11/4c1d2e025201c66bcf9cb7fb4993182e.png)
