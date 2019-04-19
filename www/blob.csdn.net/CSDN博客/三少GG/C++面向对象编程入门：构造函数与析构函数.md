# C++面向对象编程入门：构造函数与析构函数 - 三少GG - CSDN博客
2012年02月28日 17:27:50[三少GG](https://me.csdn.net/scut1135)阅读数：881标签：[c++																[编程																[input																[delete																[化工																[vector](https://so.csdn.net/so/search/s.do?q=vector&t=blog)](https://so.csdn.net/so/search/s.do?q=化工&t=blog)](https://so.csdn.net/so/search/s.do?q=delete&t=blog)](https://so.csdn.net/so/search/s.do?q=input&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
个人分类：[重构C/C++](https://blog.csdn.net/scut1135/article/category/621651)
【转载地址】[http://pcedu.pconline.com.cn/empolder/gj/c/0503/568909.html](http://pcedu.pconline.com.cn/empolder/gj/c/0503/568909.html)
“系统碰到创建代码的时候就会自动调用Teacher类中的Teacher()构造函数对对象进行初始化工作！
 ”
请注意，这一节内容是c++的重点，要特别注意！ 
　　我们先说一下什么是构造函数。 
　　上一个教程我们简单说了关于类的一些基本内容，对于类对象成员的初始化我们始终是建立成员函数然后手工调用该函数对成员进行赋值的，那么在c++中对于类来说有没有更方便的方式能够在对象创建的时候就自动初始化成员变量呢，这一点对操作保护成员是至关重要的，答案是肯定的。关于c++类成员的初始化，有专门的构造函数来进行自动操作而无需要手工调用，在正式讲解之前先看看c++对构造函数的一个基本定义。 
     1.C++规定，每个类必须有默认的构造函数，没有构造函数就不能创建对象。
　　2.若没有提供任何构造函数，那么c++提供自动提供一个默认的构造函数，该默认构造函数是一个没有参数的构造函数，它仅仅负责创建对象而不做任何赋值操作。
　　3.只要类中提供了任意一个构造函数，那么c++就不在自动提供默认构造函数。
　　4.类对象的定义和变量的定义类似，使用默认构造函数创建对象的时候，如果创建的是静态或者是全局对象，则对象的位模式全部为0，否则将会是随即的。
　　我们来看下面的代码：
#include <iostream>  
usingnamespace std;    
class Student  
{  
public:  
    Student()//无参数构造函数 
    {  
        number = 1;  
        score = 100;  
    }  
void show();  
protected:  
int number;  
int score;  
};  
void Student::show()  
{  
cout<<number<<endl<<score<<endl;  
}  
voidmain()  
{  
    Student a;  
    a.show();  
cin.get();  
}
　　在类中的定义的和类名相同，并且没有任何返回类型的Student()就是构造函数，这是一个无参数的构造函数，他在对象创建的时候自动调用，如果去掉Student()函数体内的代码那么它和c++的默认提供的构造函数等价的。
　　构造函数可以带任意多个的形式参数，这一点和普通函数的特性是一样的！
************************************
下面我们来看一个带参数的构造函数是如何进行对象的始化操作的。 
　　代码如下：
#include <iostream>  
usingnamespace std;    
class Teacher  
{  
public:  
    Teacher(char *input_name)//有参数的构造函数 
    {  
        name=newchar[10];  
//name=input_name;//这样赋值是错误的 
        strcpy(name,input_name);  
    }  
void show();  
protected:  
char *name;  
};  
void Teacher::show()  
{  
cout<<name<<endl;  
}  
voidmain()  
{  
//Teacher a;//这里是错误的,因为没有无参数的构造函数 
    Teacher a("test");  
    a.show();  
cin.get();  
}
　　我们创建了一个带有字符指针的带有形参的Teacher(char *input_name)的构造函数，调用它创建对象的使用类名加对象名称加扩号和扩号内参数的方式调用，这和调用函数有点类似，但意义也有所不同，因为构造函数是为创建对象而设立的，这里的意义不单纯是调用函数，而是创建一个类对象。 
　　一旦类中有了一个带参数的构造函数而又没无参数构造函数的时候系统将无法创建不带参数的对象，所以上面的代码
Teacher a;
　　就是错误的！！！ 
**　　这里还有一处也要注意**：
//name=input_name;//这样赋值是错误的
　　因为name指是指向内存堆区的，如果使用name=input_name;会造成指针指向改变不是指向堆区而是指向栈区，导致在后面调用析构函数delete释放堆空间出错！(析构函数的内容我们后面将要介绍) 
　　如果需要调用能够执行就需要再添加一个没有参数的构造函数 
　　对上面的代码改造如下：
#include <iostream>  
usingnamespace std;    
class Teacher  
{  
public:  
    Teacher(char *input_name)  
    {  
        name=newchar[10];  
//name=input_name;//这样赋值是错误的 
        strcpy(name,input_name);  
    }  
    Teacher()//无参数构造函数,进行函数重载 
    {  
    }  
void show();  
protected:  
char *name;  
};  
void Teacher::show()  
{  
cout<<name<<endl;  
}  
voidmain()  
{  
    Teacher test;  
    Teacher a("test");  
    a.show();  
cin.get();  
}
　　创建一个无阐述的同名的Teacher()无参数函数，一重载方式区分调用，由于构造函数和普通函数一样具有重载特性所以编写程序的人可以给一个类添加任意多个构造函数，来使用不同的参数来进行初始话对象。
************************************
　现在我们来说一下，一个类对象是另外一类的数据成员的情况，如果有点觉得饶人那么可以简单理解成:类成员的定义可以相互嵌套定义，一个类的成员可以用另一个类进行定义声明。
　　c++规定如果一个类对象是另外一类的数据成员，那么在创建对象的时候系统将自动调用那个类的构造函数。 
　　下面我们看一个例子。 
　　代码如下：
#include <iostream>  
usingnamespace std;    
class Teacher  
{  
public:  
    Teacher()  
    {  
        director =newchar[10];  
        strcpy(director,"王大力");  
    }  
char *show();  
protected:  
char *director;  
};  
char *Teacher::show()  
{  
return director;  
}  
class Student  
{  
public:  
    Student()  
    {  
        number = 1;  
        score = 100;  
    }  
void show();  
protected:  
int number;  
int score;  
    Teacher teacher;//这个类的成员teacher是用Teacher类进行创建并初始化的 
};  
void Student::show()  
{  
cout<<teacher.show()<<endl<<number<<endl<<score<<endl;  
}  
voidmain()  
{  
    Student a;  
    a.show();  
    Student b[5];  
for(int i=0; i<sizeof(b)/sizeof(Student); i++)  
    {  
        b[i].show();  
    }  
cin.get();  
}
　　上面代码中的Student类成员中teacher成员是的定义是用类Teacher进行定义创建的，那么系统碰到创建代码的时候就会自动调用Teacher类中的Teacher()构造函数对对象进行初始化工作！ 
　　这个例子说明类的分工很明确，只有碰到自己的对象的创建的时候才自己调用自己的构造函数。
　　一个类可能需要在构造函数内动态分配资源，那么这些动态开辟的资源就需要在对象不复存在之前被销毁掉，那么c++类的析构函数就提供了这个方便。 
**　　析构函数的定义:**析构函数也是特殊的类成员函数，它没有返回类型，没有参数，不能随意调用，也没有重载，只有在类对象的生命期结束的时候，由系统自动调用。
　　析构函数与构造函数最主要大不同就是在于调用期不同，构造函数可以有参数可以重载！ 
　　我们前面例子中的Teacher类中就使用new操作符进行了动态堆内存的开辟，由于上面的代码缺少析构函数，所以在程序结束后，动态开辟的内存空间并没有随着程序的结束而消失，如果没有析构函数在程序结束的时候逐一清除被占用的动态堆空间那么就会造成内存泄露，使系统内存不断减少系统效率将大大降低！
***********************************
那么我们将如何编写类的析构函数呢？ 
　　析构函数可以的特性是在程序结束的时候逐一调用，那么正好与构造函数的情况是相反，属于互逆特性，所以定义析构函数因使用"~"符号(逻辑非运算符)，表示它为腻构造函数，加上类名称来定义。 
　　看如下代码：
#include <iostream>  
#include <string>  
usingnamespace std;    
class Teacher  
{  
public:  
    Teacher()  
    {  
        director =newchar[10];  
        strcpy(director,"王大力");  
//director = new string; 
// *director="王大力";//string情况赋值 
    }  
    ~Teacher()  
    {  
cout<<"释放堆区director内存空间1次";  
delete[] director;  
cin.get();  
    }  
char *show();  
protected:  
char *director;  
//string *director; 
};  
char *Teacher::show()  
{  
return director;  
}  
class Student  
{  
public:  
    Student()  
    {  
        number = 1;  
        score = 100;  
    }  
void show();  
protected:  
int number;  
int score;  
    Teacher teacher;  
};  
void Student::show()  
{  
cout<<teacher.show()<<endl<<number<<endl<<score<<endl;  
}  
voidmain()  
{  
    Student a;  
    a.show();  
    Student b[5];  
for(int i=0; i<sizeof(b)/sizeof(Student); i++)  
    {  
        b[i].show();  
    }  
cin.get();  
}
　　上面的代码中我们为Teacher类添加了一个名为~Teacher()的析构函数用于清空堆内存。 
　　建议大家编译运行代码观察调用情况，程序将在结束前也就是对象生命周期结束的时候自动调用~Teacher() 
　　~Teache()中的delete[] director;就是清除堆内存的代码，这与我们前面一开始提到的。 
name=input_name;//这样赋值是错误的 
　　有直接的关系，因为delete操作符只能清空堆空间而不能清楚桟空间，如果强行清除栈空间内存的话将导致程序崩溃！
***********************************
前面我们已经简单的说了类的构造函数和析构函数，我们知道一个类的成员可以是另外一个类的对象，构造函数允许带参数，那么我们可能会想到上面的程序我们可以在类中把Student类中的teacher成员用带参数的形式调用Student类的构造函数，不必要再在Teacher类中进行操作，由于这一点构想我们把程序修改成如下形式：
#include <iostream>    
#include <string>    
usingnamespace std;      
class Teacher    
{    
public:    
    Teacher(char *temp)    
    {    
        director =newchar[10];    
        strcpy(director,temp);  
    }  
    ~Teacher()    
    {    
cout<<"释放堆区director内存空间1次";    
delete[] director;    
cin.get();    
    }    
char *show();    
protected:    
char *director;    
};    
char *Teacher::show()    
{    
return director;    
}    
class Student    
{    
public:    
    Student()    
    {    
        number = 1;    
        score = 100;    
    }    
void show();    
protected:    
int number;    
int score;    
    Teacher teacher("王大力");//错误，一个类的成员如果是另外一个类的对象的话，不能在类中使用带参数的构造函数进行初始化 
};    
void Student::show()    
{    
cout<<teacher.show()<<endl<<number<<endl<<score<<endl;    
}    
voidmain()    
{    
    Student a;    
    a.show();    
    Student b[5];    
for(int i=0; i<sizeof(b)/sizeof(Student); i++)    
    {    
        b[i].show();    
    }    
cin.get();    
}
　　可是很遗憾，程序不能够被编译成功,为什么呢？ 
　　因为：类是一个抽象的概念，并不是一个实体，并不能包含属性值(这里来说也就是构造函数的参数了)，只有对象才占有一定的内存空间，含有明确的属性值！
　　这一个问题是类成员初始化比较尴尬的一个问题，是不是就没有办法解决了呢？呵呵。。。。。。 
　　c++为了解决此问题，有一个很独特的方法，下一小节我们将介绍。
　　对于上面的那个"尴尬"问题，我们可以在构造函数头的后面加上:号并指定调用哪那个类成员的构造函数来解决！ 
　　教程写到这里的时候对比了很多书籍，发现几乎所有的书都把这一章节叫做**构造类成员**，笔者在此觉得有所不妥，因为从读音上容易混淆概念，所以把这一小节的名称改为**构造类的成员**比较合适！
**********************************************
　代码如下：
#include <iostream>    
usingnamespace std;      
class Teacher    
{    
public:    
    Teacher(char *temp)    
    {    
        director =newchar[10];    
        strcpy(director,temp);    
    }  
    ~Teacher()    
    {    
cout<<"释放堆区director内存空间1次";    
delete[] director;    
cin.get();    
    }    
char *show();    
protected:    
char *director;    
};    
char *Teacher::show()    
{    
return director;    
}    
class Student    
{    
public:    
    Student(char *temp):teacher(temp)  
    {    
        number = 1;    
        score = 100;    
    }    
void show();    
protected:    
int number;    
int score;    
    Teacher teacher;    
};    
void Student::show()    
{    
cout<<teacher.show()<<endl<<number<<endl<<score<<endl;    
}    
voidmain()    
{    
    Student a("王大力");    
    a.show();    
//Student b[5]("王大力");  //这里这么用是不对的,数组不能够使用带参数的构造函数,以后我们将详细介绍vector类型 
// for(int i=0; i<sizeof(b)/sizeof(Student); i++)   
//{   
//    b[i].show();   
//}   
cin.get();    
}
　　大家可以发现最明显的改变在这里 
Student(char *temp):teacher(temp) 
　　冒号后的teacher就是告诉调用Student类的构造函数的时候把参数传递给成员teacher的Teacher类的构造函数，这样一来我们就成功的在类体外对teacher成员进行了初始化，既方便也高效，这种冒号后指定调用某成员构造函数的方式，可以同时制定多个成员，这一特性使用逗号方式，例如： 
Student(char *temp):teacher(temp),abc(temp),def(temp) 
　　由冒号后可指定调用哪那个类成员的构造函数的特性，使得我们可以给类的常量和引用成员进行初始化成为可能。 
　　我们修改上面的程序，得到如下代码：
#include <iostream>    
#include <string>    
usingnamespace std;      
class Teacher    
{    
public:    
    Teacher(char *temp)    
    {    
        director =newchar[10];    
        strcpy(director,temp);    
    }  
    ~Teacher()    
    {    
cout<<"释放堆区director内存空间1次";    
delete[] director;    
cin.get();  
    }    
char *show();    
protected:    
char *director;    
};    
char *Teacher::show()    
{    
return director;    
}    
class Student    
{    
public:    
    Student(char *temp,int &pk):teacher(temp),pk(pk),ps(10)  
    {    
        number = 1;    
        score = 100;  
    }    
void show();    
protected:    
int number;    
int score;    
    Teacher teacher;  
int &pk;  
constint ps;  
};    
void Student::show()    
{    
cout<<teacher.show()<<endl<<number<<endl<<score<<endl<<pk<<endl<<ps<<endl;    
}    
voidmain()    
{    
char *t_name="王大力";  
int b=99;  
    Student a(t_name,b);  
    a.show();  
cin.get();  
}
　　改变之处最重要的在这里Student(char *temp,int &pk):teacher(temp),pk(pk),ps(10) 
　　调用的时候我们使用 
Student a(t_name,b); 
　　我们将b的地址传递给了int &pk这个引用，使得Student类的引用成员pk和常量成员ps进行了成功的初始化。
但是细心的人会发现，我们在这里使用的初始化方式并不是在构造函数内进行的，而是在外部进行初始化的，的确，在冒号后和在构造函数括号内的效果是一样的，但和teacher(temp)所不同的是，pk(pk)的括号不是调用函数的意思，而是赋值的意思，我想有些读者可能不清楚新标准的c++对变量的初始化是允许使用括号方式的，int a=10和int a(10)的等价的，但冒号后是不允许使用=方式只允许()括号方式，所以这里只能使用pk(pk)而不能是pk=pk了。
　　这一小节的内容是说**对象构造的顺序**的，对象构造的顺序直接关系程序的运行结果，有时候我们写的程序不错，但运行出来的结果却超乎我们的想象，了解c++对对象的构造顺序有助于解决这些问题。 
　　c++规定，所有的全局对象和全局变量一样都在主函数main()之前被构造，函数体内的静态对象则只构造一次，也就是说只在首次进入这个函数的时候进行构造！
　　代码如下：
#include <iostream>    
#include <string>    
usingnamespace std;      
class Test  
{  
public:  
    Test(int a)  
    {  
        kk=a;  
cout<<"构造参数a:"<<a<<endl;  
    }  
public:  
int kk;  
};  
void fun_t(int n)  
{  
static Test a(n);  
//static Test a=n;//这么写也是对的 
cout<<"函数传入参数n:"<<n<<endl;  
cout<<"对象a的属性kk的值:"<<a.kk<<endl;  
}  
Test m(100);  
voidmain()  
{  
    fun_t(20);  
    fun_t(30);  
cin.get();  
}
    运行结果：
![](http://hi.csdn.net/attachment/201202/28/0_1330421601vv5D.gif)
　　下面我们来看一下，**类成员的构造顺序**的问题。 
　　先看下面的代码：
#include <iostream>    
usingnamespace std;      
class Test  
{  
public:  
    Test(int j):pb(j),pa(pb+5)  
    {  
    }  
public:  
int pa;  
int pb;  
};  
voidmain()  
{  
    Test a(10);  
cout<<a.pa<<endl;  
cout<<a.pb<<endl;  
cin.get();  
}
　　上面的程序在代码上是没有任何问题的，但运行结果可能并不如人意。 
　　pa并没有得到我们所希望的15而是一个随机的任意地址的值。 
　　这又是为什么呢？ 
　　类成员的构造是按照在类中定义的顺序进行的，而不是按照构造函数说明后的冒号顺序进行构造的，这一点需要记住！
