# friend的作用[转载] - xqhrs232的专栏 - CSDN博客
2010年02月22日 16:15:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：751标签：[internet																[iostream																[class																[编程																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=iostream&t=blog)](https://so.csdn.net/so/search/s.do?q=internet&t=blog)
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
在说明什么是友元之前，我们先说明一下为什么需要友元与友元的缺点: 
　　通常对于普通函数来说,要访问类的保护成员是不可能的，如果想这么做那么必须把类的成员都生命成为public(共用的)，然而这做带来的问题遍是任何外部函数都可以毫无约束的访问它操作它，c++利用friend修饰符，可以让一些你设定的函数能够对这些保护数据进行操作，避免把类成员全部设置成public，最大限度的保护数据成员的安全。
　　友元能够使得普通函数直接访问类的保护数据，避免了类成员函数的频繁调用，可以节约处理器开销，提高程序的效率，但矛盾的是，即使是最大限度的保护，同样也破坏了类的封装特性，这即是友元的缺点，在现在cpu速度越来越快的今天我们并不推荐使用它，但它作为c++一个必要的知识点，一个完整的组成部分，我们还是需要讨论一下的。
　　在类里声明一个普通函数，在前面加上friend修饰，那么这个函数就成了该类的友元，可以访问该类的一切成员。 
　　下面我们来看一段代码，看看我们是如何利用友元来访问类的一切成员的。
#include <iostream>    
using namespace std;  
class Internet    
{    
public:    
    Internet(char *name,char *address)    
    {    
        strcpy(Internet::name,name);    
        strcpy(Internet::address,address);     
    }  
friend void ShowN(Internet &obj);//友元函数的声明  
public:    
    char name[20];  
    char address[20];  
};  
void ShowN(Internet &obj)//函数定义,不能写成,void Internet::ShowN(Internet &obj)  
{  
    cout<<obj.name<<endl;  
}  
void main()    
{  
    Internet a("中国软件开发实验室","[www.cndev-lab.com](http://www.cndev-lab.com)"); 
    ShowN(a);  
    cin.get();  
} 
　　上面的代码通过友元函数的定义，我们成功的访问到了a对象的保护成员name,友元函数并不能看做是类的成员函数，它只是个被声明为类友元的普通函数，所以在类外部函数的定义部分不能够写成void Internet::ShowN(Internet &obj)，这一点要注意。
一个普通函数可以是多个类的友元函数，对上面的代码我们进行修改，注意观察变化：
#include <iostream>    
using namespace std;  
class Country;  
class Internet    
{    
public:    
    Internet(char *name,char *address)    
    {    
        strcpy(Internet::name,name);    
        strcpy(Internet::address,address);     
    }  
friend void ShowN(Internet &obj,Country &cn);//注意这里  
public:    
    char name[20];  
    char address[20];  
};  
class Country  
{  
public:  
    Country()  
    {  
        strcpy(cname,"中国");  
    }  
friend void ShowN(Internet &obj,Country &cn);//注意这里  
protected:  
    char cname[30];  
};  
void ShowN(Internet &obj,Country &cn)  
{  
    cout<<cn.cname<<"|"<<obj.name<<endl;  
}  
void main()    
{  
    Internet a("中国软件开发实验室","[www.cndev-lab.com](http://www.cndev-lab.com)"); 
    Country b;  
    ShowN(a,b);  
    cin.get();  
}
　　一个类的成员函数函数也可以是另一个类的友元，从而可以使得一个类的成员函数可以操作另一个类的数据成员，我们在下面的代码中增加一类Country，注意观察：
#include <iostream>    
using namespace std;  
class Internet;  
class Country  
{  
public:  
    Country()  
    {  
        strcpy(cname,"中国");  
    }  
    void Editurl(Internet &temp);//成员函数的声明  
protected:  
    char cname[30];  
};  
class Internet  
{    
public:    
    Internet(char *name,char *address)    
    {    
        strcpy(Internet::name,name);    
        strcpy(Internet::address,address);   
    }  
    friend void Country::Editurl(Internet &temp);//友元函数的声明  
protected:    
    char name[20];  
    char address[20];  
};  
void Country::Editurl(Internet &temp)//成员函数的外部定义  
{  
    strcpy(temp.address,"edu.cndev-lab.com");  
    cout<<temp.name<<"|"<<temp.address<<endl;  
}  
void main()  
{  
    Internet a("中国软件开发实验室","[www.cndev-lab.com](http://www.cndev-lab.com)"); 
    Country b;  
    b.Editurl(a);  
    cin.get();  
}
　　整个类也可以是另一个类的友元，该友元也可以称做为友类。友类的每个成员函数都可以访问另一个类的所有成员。 
示例代码如下：
#include <iostream>    
using namespace std;  
class Internet;  
class Country  
{  
public:  
    Country()  
    {  
        strcpy(cname,"中国");  
    }  
    friend class Internet;//友类的声明  
protected:  
    char cname[30];  
};  
class Internet  
{  
public:    
    Internet(char *name,char *address)    
    {    
        strcpy(Internet::name,name);    
        strcpy(Internet::address,address);   
    }  
    void Editcname(Country &temp);  
protected:    
    char name[20];  
    char address[20];  
};  
void Internet::Editcname(Country &temp)  
{  
    strcpy(temp.cname,"中华人民共和国");   
}  
void main()  
{  
    Internet a("中国软件开发实验室","[www.cndev-lab.com](http://www.cndev-lab.com)"); 
    Country b;  
    a.Editcname(b);  
    cin.get();  
}
　　在上面的代码中我们成功的通过Internet类Editcname成员函数操作了Country类的保护成员cname。 
　　在编程中，我们使用友元的另外一个重要原因是为了方便重载操作符的使用，这些内容我们将在后面的教程着重讨论！
